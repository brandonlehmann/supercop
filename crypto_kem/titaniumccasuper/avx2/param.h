/* ****************************** *
 * Titanium_CCA_super             *
 * Implemented by Raymond K. ZHAO *
 *                                *
 * Parameters                     *
 * ****************************** */
 
#ifndef PARAM_H
#define PARAM_H

#define CRYPTO_RANDOMBYTES 32

#define Q 1198081
#define Q2 (Q << 1) /* Q*2 */
#define Q_BITS 21 /* 21 bits for a number in Z_q */

#define ZQ_BYTES 3 /* 3 random bytes to sample a number in Z_q */
#define LOAD_ZQ load_24

/* the actual number of random coordinates generated by sampler_zq */
#define ZQ_BYTPCS 3386
#define ZQ_BYTPCA 2100

#define N 2048

#define K1 1023

#define D 256
#define D_BYTES 32 /* 256 --> 32 bytes */

/* function names of the NTTs */
#define NTT_N_NDK ntt_2048_3328 //8*256-->13*256
#define INTT_NDK_DK intt_3328_1280 //13*256-->5*256

#define NTT_K_NK ntt_1024_3072 //1*1024-->3*1024
#define NTT_N_NK ntt_2048_3072 //2*1024-->3*1024
#define INTT_NK_NK_INV intt_3072_3072_inv //3*1024-->3*1024

#define NTT_DK_DK_INV ntt_1280_1280_inv //5*256-->5*256
#define NTT_K_DK ntt_1024_1280 //4*256-->5*256
#define INTT_DK_D intt_1280_256 //5*256-->1*256

#define NTT_NK_NDK ntt_3072_3328 //12*256-->13*256
#define INTT_NDK_D intt_3328_256 //13*256-->1*256

#define T 8

/* the standard deviation of binomial sampler is sqrt(k/2)=1.41 */
#define BINOMIAL_K 4
#define BINOMIAL_BYTE 1
#define BINOMIAL_ADDMASK 0x11
#define BINOMIAL_MASK 0x0f
#define BINOMIAL_SHIFT 4
#define LOAD_BINOMIAL load_8

/* B_1/2=128 and B_2/2=256 */
#define B_1_BIT 7 /* 2^7=128 */
#define B_1_BITMASK ((1 << B_1_BIT) - 1)

#define B_2_BIT 8 /* 2^8=256 */
#define B_2_BITMASK ((1 << B_2_BIT) - 1)

#define NUM_B1 5904 /* Ndec1 */

#define B_BYTE 1 /* 1 random bytes to sample a number in Z_B */
#define LOAD_B load_8

/* c_2 compression bit number
 * discard the least significant 13 bits to make each c_2 coordinate fit in one byte */
#define C2_COMPRESSION_BITS 13
#define C2_COMPRESSION_BYTE 1
#define LOAD_C2 load_8
#define STORE_C2 store_8

#endif
