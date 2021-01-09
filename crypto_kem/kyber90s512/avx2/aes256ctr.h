#ifndef AES256CTR_H
#define AES256CTR_H

#include <stddef.h>
#include <stdint.h>
#include <immintrin.h>
#include "params.h"

#define _AES256CTR_NAMESPACE(s) KYBER_NAMESPACE(_aes256ctr##s)
#define AES256CTR_NAMESPACE(s) _AES256CTR_NAMESPACE(s)

#define AES256CTR_BLOCKBYTES 64

typedef struct {
  __m128i rkeys[16];
  __m128i n;
} aes256ctr_ctx;

#define aes256ctr_init AES256CTR_NAMESPACE(_init)
void aes256ctr_init(aes256ctr_ctx *state,
                    const uint8_t key[32],
                    uint64_t nonce);

#define aes256ctr_squeezeblocks AES256CTR_NAMESPACE(_squeezeblocks)
void aes256ctr_squeezeblocks(uint8_t *out,
                             size_t nblocks,
                             aes256ctr_ctx *state);

#define aes256ctr_prf AES256CTR_NAMESPACE(_prf)
void aes256ctr_prf(uint8_t *out,
                   size_t outlen,
                   const uint8_t key[32],
                   uint64_t nonce);

#endif
