#include <immintrin.h>
#include <stdint.h>
#include "crypto_encode.h"

void crypto_encode(unsigned char *s,const void *v)
{
  __m256i a0 = _mm256_loadu_si256(0+(__m256i *) v);
  __m256i a1 = _mm256_loadu_si256(1+(__m256i *) v);
  __m256i a2 = _mm256_loadu_si256(2+(__m256i *) v);
  __m256i a3 = _mm256_loadu_si256(3+(__m256i *) v);
  __m256i a4 = _mm256_loadu_si256(4+(__m256i *) v);
  __m256i a5 = _mm256_loadu_si256(5+(__m256i *) v);
  __m256i a6 = _mm256_loadu_si256(6+(__m256i *) v);
  __m256i a7 = _mm256_loadu_si256(7+(__m256i *) v);
  __m256i bottom = _mm256_set1_epi8(1);
  __m256i zero = _mm256_setzero_si256();
  __m256i b0 = _mm256_cmpgt_epi8(a0&bottom,zero);
  __m256i b1 = _mm256_cmpgt_epi8(a1&bottom,zero);
  __m256i b2 = _mm256_cmpgt_epi8(a2&bottom,zero);
  __m256i b3 = _mm256_cmpgt_epi8(a3&bottom,zero);
  __m256i b4 = _mm256_cmpgt_epi8(a4&bottom,zero);
  __m256i b5 = _mm256_cmpgt_epi8(a5&bottom,zero);
  __m256i b6 = _mm256_cmpgt_epi8(a6&bottom,zero);
  __m256i b7 = _mm256_cmpgt_epi8(a7&bottom,zero);
  int32_t c0 = _mm256_movemask_epi8(b0);
  int32_t c1 = _mm256_movemask_epi8(b1);
  int32_t c2 = _mm256_movemask_epi8(b2);
  int32_t c3 = _mm256_movemask_epi8(b3);
  int32_t c4 = _mm256_movemask_epi8(b4);
  int32_t c5 = _mm256_movemask_epi8(b5);
  int32_t c6 = _mm256_movemask_epi8(b6);
  int32_t c7 = _mm256_movemask_epi8(b7);
  0[(int32_t *) s] = c0;
  1[(int32_t *) s] = c1;
  2[(int32_t *) s] = c2;
  3[(int32_t *) s] = c3;
  4[(int32_t *) s] = c4;
  5[(int32_t *) s] = c5;
  6[(int32_t *) s] = c6;
  7[(int32_t *) s] = c7;
}
