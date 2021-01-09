#include <stdint.h>
#include "params.h"
#include "sign.h"
#include "packing.h"
#include "polyvec.h"
#include "poly.h"
#include "randombytes.h"
#include "symmetric.h"
#include "fips202.h"
#ifdef DILITHIUM_USE_AES
#include "aes256ctr.h"
#endif

/*************************************************
* Name:        challenge
*
* Description: Implementation of H. Samples polynomial with 60 nonzero
*              coefficients in {-1,1} using the output stream of
*              SHAKE256(mu|w1).
*
* Arguments:   - poly *c: pointer to output polynomial
*              - const uint8_t mu[]: byte array containing mu
*              - const polyveck *w1: pointer to vector w1
**************************************************/
void challenge(poly *c,
               const uint8_t mu[CRHBYTES],
               const polyveck *w1)
{
  unsigned int i, b, pos;
  uint64_t signs;
  __attribute__((aligned(32)))
  uint8_t buf[CRHBYTES + K*POLYW1_PACKEDBYTES];
  keccak_state state;

  for(i = 0; i < CRHBYTES; ++i)
    buf[i] = mu[i];
  for(i = 0; i < K; ++i)
    polyw1_pack(buf + CRHBYTES + i*POLYW1_PACKEDBYTES, &w1->vec[i]);

  shake256_init(&state);
  shake256_absorb(&state, buf, sizeof(buf));
  shake256_finalize(&state);
  shake256_squeezeblocks(buf, 1, &state);

  signs = 0;
  for(i = 0; i < 8; ++i)
    signs |= (uint64_t)buf[i] << 8*i;

  pos = 8;

  for(i = 0; i < N; ++i)
    c->coeffs[i] = 0;

  for(i = 196; i < 256; ++i) {
    do {
      if(pos >= SHAKE256_RATE) {
        shake256_squeezeblocks(buf, 1, &state);
        pos = 0;
      }

      b = buf[pos++];
    } while(b > i);

    c->coeffs[i] = c->coeffs[b];
    c->coeffs[b] = 1;
    c->coeffs[b] ^= -((uint32_t)signs & 1) & (1 ^ (Q-1));
    signs >>= 1;
  }
}

/*************************************************
* Name:        crypto_sign_keypair
*
* Description: Generates public and private key.
*
* Arguments:   - unsigned char *pk: pointer to output public key (allocated
*                                   array of CRYPTO_PUBLICKEYBYTES bytes)
*              - unsigned char *sk: pointer to output private key (allocated
*                                   array of CRYPTO_SECRETKEYBYTES bytes)
*
* Returns 0 (success)
**************************************************/
int crypto_sign_keypair(unsigned char *pk, unsigned char *sk) {
  unsigned int i;
  __attribute__((aligned(32)))
  uint8_t seedbuf[3*SEEDBYTES];
  __attribute__((aligned(32)))
  uint8_t tr[CRHBYTES];
  const uint8_t *rho, *rhoprime, *key;
  polyvecl mat[K];
  polyvecl s1, s1hat;
  polyveck s2, t, t1, t0;

  /* Get randomness for rho, rhoprime and key */
  randombytes(seedbuf, 3*SEEDBYTES);
  rho = seedbuf;
  rhoprime = seedbuf + SEEDBYTES;
  key = seedbuf + 2*SEEDBYTES;

  /* Expand matrix */
  expand_mat(mat, rho);

  /* Sample short vectors s1 and s2 */
#ifdef DILITHIUM_USE_AES
  __attribute__((aligned(16)))
  uint64_t nonce = 0;
  aes256ctr_ctx aesctx;
  aes256ctr_init(&aesctx, rhoprime, nonce++);
  for(i = 0; i < L; ++i) {
    poly_uniform_eta_preinit(&s1.vec[i], &aesctx);
    aesctx.n = _mm_loadl_epi64((__m128i *)&nonce);
    nonce++;
  }
  for(i = 0; i < K; ++i) {
    poly_uniform_eta_preinit(&s2.vec[i], &aesctx);
    aesctx.n = _mm_loadl_epi64((__m128i *)&nonce);
    nonce++;
  }
#elif L == 2 && K == 3
  poly_uniform_eta_4x(&s1.vec[0], &s1.vec[1], &s2.vec[0], &s2.vec[1], rhoprime,
                      0, 1, 2, 3);
  poly_uniform_eta(&s2.vec[2], rhoprime, 4);
#elif L == 3 && K == 4
  poly_uniform_eta_4x(&s1.vec[0], &s1.vec[1], &s1.vec[2], &s2.vec[0], rhoprime,
                      0, 1, 2, 3);
  poly_uniform_eta_4x(&s2.vec[1], &s2.vec[2], &s2.vec[3], &t.vec[0], rhoprime,
                      4, 5, 6, 7);
#elif L == 4 && K == 5
  poly_uniform_eta_4x(&s1.vec[0], &s1.vec[1], &s1.vec[2], &s1.vec[3], rhoprime,
                      0, 1, 2, 3);
  poly_uniform_eta_4x(&s2.vec[0], &s2.vec[1], &s2.vec[2], &s2.vec[3], rhoprime,
                      4, 5, 6, 7);
  poly_uniform_eta(&s2.vec[4], rhoprime, 8);
#elif L == 5 && K == 6
  poly_uniform_eta_4x(&s1.vec[0], &s1.vec[1], &s1.vec[2], &s1.vec[3], rhoprime,
                      0, 1, 2, 3);
  poly_uniform_eta_4x(&s1.vec[4], &s2.vec[0], &s2.vec[1], &s2.vec[2], rhoprime,
                      4, 5, 6, 7);
  poly_uniform_eta_4x(&s2.vec[3], &s2.vec[4], &s2.vec[5], &t.vec[0], rhoprime,
                      8, 9, 10, 11);
#else
#error
#endif

  /* Matrix-vector multiplication */
  s1hat = s1;
  polyvecl_ntt(&s1hat);
  for(i = 0; i < K; ++i) {
    polyvecl_pointwise_acc_montgomery(&t.vec[i], &mat[i], &s1hat);
    //poly_reduce(&t.vec[i]);
    poly_invntt_tomont(&t.vec[i]);
  }

  /* Add error vector s2 */
  polyveck_add(&t, &t, &s2);

  /* Extract t1 and write public key */
  polyveck_freeze(&t);
  polyveck_power2round(&t1, &t0, &t);
  pack_pk(pk, rho, &t1);

  /* Compute CRH(rho, t1) and write secret key */
  crh(tr, pk, CRYPTO_PUBLICKEYBYTES);
  pack_sk(sk, rho, key, tr, &s1, &s2, &t0);

  return 0;
}

/*************************************************
* Name:        crypto_sign_signature
*
* Description: Computes signature.
*
* Arguments:   - unsigned char *sig:         pointer to output signature (of length CRYPTO_BYTES)
*              - unsigned long long *siglen: pointer to output length of signed message
*              - unsigned char *m:           pointer to message to be signed
*              - unsigned long long mlen:    length of message
*              - unsigned char *sk:          pointer to bit-packed secret key
*
* Returns 0 (success)
**************************************************/
int crypto_sign_signature(unsigned char *sig,
                          unsigned long long *siglen,
                          const unsigned char *m,
                          unsigned long long mlen,
                          const unsigned char *sk)
{
  unsigned long long i;
  unsigned int n;
  __attribute__((aligned(32)))
  uint8_t seedbuf[2*SEEDBYTES + 3*CRHBYTES];
  uint8_t *rho, *tr, *key, *mu, *rhoprime;
  __attribute__((aligned(16)))
  uint64_t nonce = 0;
  poly c, chat;
  polyvecl mat[K], s1, y, yhat, z;
  polyveck t0, s2, w, w1, w0;
  polyveck h, cs2, ct0;
  keccak_state state;

  rho = seedbuf;
  tr = rho + SEEDBYTES;
  key = tr + CRHBYTES;
  mu = key + SEEDBYTES;
  rhoprime = mu + CRHBYTES;
  unpack_sk(rho, key, tr, &s1, &s2, &t0, sk);

  /* Compute CRH(tr, msg) */
  shake256_init(&state);
  shake256_absorb(&state, tr, CRHBYTES);
  shake256_absorb(&state, m, mlen);
  shake256_finalize(&state);
  shake256_squeeze(mu, CRHBYTES, &state);

#ifdef DILITHIUM_RANDOMIZED_SIGNING
  randombytes(rhoprime, CRHBYTES);
#else
  crh(rhoprime, key, SEEDBYTES + CRHBYTES);
#endif

  /* Expand matrix and transform vectors */
  expand_mat(mat, rho);
  polyvecl_ntt(&s1);
  polyveck_ntt(&s2);
  polyveck_ntt(&t0);

#ifdef DILITHIUM_USE_AES
  aes256ctr_ctx aesctx;
  aes256ctr_init(&aesctx, rhoprime, nonce++);
#endif

rej:
  /* Sample intermediate vector y */
#ifdef DILITHIUM_USE_AES
  for(i = 0; i < L; ++i) {
    poly_uniform_gamma1m1_preinit(&y.vec[i], &aesctx);
    aesctx.n = _mm_loadl_epi64((__m128i *)&nonce);
    nonce++;
  }
#elif L == 2
  poly_uniform_gamma1m1_4x(&y.vec[0], &y.vec[1], &yhat.vec[0], &yhat.vec[1],
                           rhoprime, nonce, nonce + 1, 0, 0);
  nonce += 2;
#elif L == 3
  poly_uniform_gamma1m1_4x(&y.vec[0], &y.vec[1], &y.vec[2], &yhat.vec[0],
                           rhoprime, nonce, nonce + 1, nonce + 2, 0);
  nonce += 3;
#elif L == 4
  poly_uniform_gamma1m1_4x(&y.vec[0], &y.vec[1], &y.vec[2], &y.vec[3],
                           rhoprime, nonce, nonce + 1, nonce + 2, nonce + 3);
  nonce += 4;
#elif L == 5
  poly_uniform_gamma1m1_4x(&y.vec[0], &y.vec[1], &y.vec[2], &y.vec[3],
                           rhoprime, nonce, nonce + 1, nonce + 2, nonce + 3);
  poly_uniform_gamma1m1(&y.vec[4], rhoprime, nonce + 4);
  nonce += 5;
#else
#error
#endif

  /* Matrix-vector multiplication */
  yhat = y;
  polyvecl_ntt(&yhat);
  for(i = 0; i < K; ++i) {
    polyvecl_pointwise_acc_montgomery(&w.vec[i], &mat[i], &yhat);
    //poly_reduce(&w.vec[i]);
    poly_invntt_tomont(&w.vec[i]);
  }

  /* Decompose w and call the random oracle */
  polyveck_csubq(&w);
  polyveck_decompose(&w1, &w0, &w);
  challenge(&c, mu, &w1);
  chat = c;
  poly_ntt(&chat);

  /* Check that subtracting cs2 does not change high bits of w and low bits
   * do not reveal secret information */
  for(i = 0; i < K; ++i) {
    poly_pointwise_montgomery(&cs2.vec[i], &chat, &s2.vec[i]);
    poly_invntt_tomont(&cs2.vec[i]);
  }
  polyveck_sub(&w0, &w0, &cs2);
  polyveck_freeze(&w0);
  if(polyveck_chknorm(&w0, GAMMA2 - BETA))
    goto rej;

  /* Compute z, reject if it reveals secret */
  for(i = 0; i < L; ++i) {
    poly_pointwise_montgomery(&z.vec[i], &chat, &s1.vec[i]);
    poly_invntt_tomont(&z.vec[i]);
  }
  polyvecl_add(&z, &z, &y);
  polyvecl_freeze(&z);
  if(polyvecl_chknorm(&z, GAMMA1 - BETA))
    goto rej;

  /* Compute hints for w1 */
  for(i = 0; i < K; ++i) {
    poly_pointwise_montgomery(&ct0.vec[i], &chat, &t0.vec[i]);
    poly_invntt_tomont(&ct0.vec[i]);
  }

  polyveck_csubq(&ct0);
  if(polyveck_chknorm(&ct0, GAMMA2))
    goto rej;

  polyveck_add(&w0, &w0, &ct0);
  polyveck_csubq(&w0);
  n = polyveck_make_hint(&h, &w0, &w1);
  if(n > OMEGA)
    goto rej;

  /* Write signature */
  pack_sig(sig, &z, &h, &c);
  *siglen = CRYPTO_BYTES;
  return 0;
}

/*************************************************
* Name:        crypto_sign
*
* Description: Compute signed message.
*
* Arguments:   - unsigned char *sm: pointer to output signed message (allocated
*                                   array with CRYPTO_BYTES + mlen bytes),
*                                   can be equal to m
*              - unsigned long long *smlen: pointer to output length of signed
*                                           message
*              - const unsigned char *m: pointer to message to be signed
*              - unsigned long long mlen: length of message
*              - const unsigned char *sk: pointer to bit-packed secret key
*
* Returns 0 (success)
**************************************************/
int crypto_sign(unsigned char *sm,
                unsigned long long *smlen,
                const unsigned char *m,
                unsigned long long mlen,
                const unsigned char *sk)
{
  unsigned long long i;

  for(i = 0; i < mlen; ++i)
    sm[CRYPTO_BYTES + mlen - 1 - i] = m[mlen - 1 - i];
  crypto_sign_signature(sm, smlen, sm + CRYPTO_BYTES, mlen, sk);
  *smlen += mlen;
  return 0;
}

/*************************************************
* Name:        crypto_sign_verify
*
* Description: Verifies signature.
*
* Arguments:   - unsigned char *m: pointer to input signature
*              - unsigned long long siglen: length of signature
*              - const unsigned char *m: pointer to message
*              - unsigned long long mlen: length of message
*              - const unsigned char *pk: pointer to bit-packed public key
*
* Returns 0 if signature could be verified correctly and -1 otherwise
**************************************************/
int crypto_sign_verify(const unsigned char *sig,
                       unsigned long long siglen,
                       const unsigned char *m,
                       unsigned long long mlen,
                       const unsigned char *pk)
{
  unsigned long long i;
  __attribute__((aligned(32)))
  uint8_t rho[SEEDBYTES];
  __attribute__((aligned(32)))
  uint8_t mu[CRHBYTES];
  poly c, chat, cp;
  polyvecl mat[K], z;
  polyveck t1, w1, h, tmp;
  keccak_state state;

  if(siglen != CRYPTO_BYTES)
    return -1;

  unpack_pk(rho, &t1, pk);
  if(unpack_sig(&z, &h, &c, sig))
    return -1;
  if(polyvecl_chknorm(&z, GAMMA1 - BETA))
    return -1;

  /* Compute CRH(CRH(rho, t1), msg) */
  crh(mu, pk, CRYPTO_PUBLICKEYBYTES);
  shake256_init(&state);
  shake256_absorb(&state, mu, CRHBYTES);
  shake256_absorb(&state, m, mlen);
  shake256_finalize(&state);
  shake256_squeeze(mu, CRHBYTES, &state);

  /* Matrix-vector multiplication; compute Az - c2^dt1 */
  expand_mat(mat, rho);
  polyvecl_ntt(&z);
  for(i = 0; i < K ; ++i)
    polyvecl_pointwise_acc_montgomery(&w1.vec[i], &mat[i], &z);

  chat = c;
  poly_ntt(&chat);
  polyveck_shiftl(&t1);
  polyveck_ntt(&t1);
  for(i = 0; i < K; ++i)
    poly_pointwise_montgomery(&tmp.vec[i], &chat, &t1.vec[i]);

  polyveck_sub(&w1, &w1, &tmp);
  polyveck_reduce(&w1);
  polyveck_invntt_tomont(&w1);

  /* Reconstruct w1 */
  polyveck_csubq(&w1);
  polyveck_use_hint(&w1, &w1, &h);

  /* Call random oracle and verify challenge */
  challenge(&cp, mu, &w1);
  for(i = 0; i < N; ++i)
    if(c.coeffs[i] != cp.coeffs[i])
      return -1;

  return 0;
}

/*************************************************
* Name:        crypto_sign_open
*
* Description: Verify signed message.
*
* Arguments:   - unsigned char *m: pointer to output message (allocated
*                                  array with smlen bytes), can be equal to sm
*              - unsigned long long *mlen: pointer to output length of message
*              - const unsigned char *sm: pointer to signed message
*              - unsigned long long smlen: length of signed message
*              - const unsigned char *pk: pointer to bit-packed public key
*
* Returns 0 if signed message could be verified correctly and -1 otherwise
**************************************************/
int crypto_sign_open(unsigned char *m,
                     unsigned long long *mlen,
                     const unsigned char *sm,
                     unsigned long long smlen,
                     const unsigned char *pk)
{
  unsigned long long i;

  if(smlen < CRYPTO_BYTES)
    goto badsig;

  *mlen = smlen - CRYPTO_BYTES;
  if(crypto_sign_verify(sm, CRYPTO_BYTES, sm + CRYPTO_BYTES, *mlen, pk))
    goto badsig;
  else {
    /* All good, copy msg, return 0 */
    for(i = 0; i < *mlen; ++i)
      m[i] = sm[CRYPTO_BYTES + i];

    return 0;
  }

badsig:
  /* Signature verification failed */
  *mlen = (unsigned long long) -1;
  for(i = 0; i < smlen; ++i)
    m[i] = 0;

  return -1;
}
