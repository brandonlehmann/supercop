/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */


#include "io.h"
#include "lowmc.h"
#include "mzd_additional.h"
#include "simd.h"

#if !defined(_MSC_VER)
#include <stdalign.h>
#endif
#include <string.h>
#include <assert.h>

#include "lowmc_129_129_4.h"


#if !defined(NO_UINT64_FALLBACK)
/**
 * S-box for m = 43
 */
static void sbox_uint64_lowmc_129_129_4(mzd_local_t* in) {
  mzd_local_t x0m[1], x1m[1], x2m[1];
  // a
  mzd_and_uint64_192(x0m, mask_129_129_43_a, in);
  // b
  mzd_and_uint64_192(x1m, mask_129_129_43_b, in);
  // c
  mzd_and_uint64_192(x2m, mask_129_129_43_c, in);

  mzd_shift_left_uint64_192(x0m, x0m, 2);
  mzd_shift_left_uint64_192(x1m, x1m, 1);

  mzd_local_t t0[1], t1[1], t2[1];
  // b & c
  mzd_and_uint64_192(t0, x1m, x2m);
  // c & a
  mzd_and_uint64_192(t1, x0m, x2m);
  // a & b
  mzd_and_uint64_192(t2, x0m, x1m);

  // (b & c) ^ a
  mzd_xor_uint64_192(t0, t0, x0m);

  // (c & a) ^ a ^ b
  mzd_xor_uint64_192(t1, t1, x0m);
  mzd_xor_uint64_192(t1, t1, x1m);

  // (a & b) ^ a ^ b ^c
  mzd_xor_uint64_192(t2, t2, x0m);
  mzd_xor_uint64_192(t2, t2, x1m);
  mzd_xor_uint64_192(t2, t2, x2m);

  mzd_shift_right_uint64_192(t0, t0, 2);
  mzd_shift_right_uint64_192(t1, t1, 1);

  mzd_xor_uint64_192(t2, t2, t1);
  mzd_xor_uint64_192(in, t2, t0);
}


#endif /* NO_UINT_FALLBACK */

ATTR_TARGET_S128
static inline void sbox_s128_full(mzd_local_t* in, const word128* mask_a, const word128* mask_b,
                                  const word128* mask_c) {
  word128 x0m[2] ATTR_ALIGNED(alignof(word128)), x1m[2] ATTR_ALIGNED(alignof(word128)),
      x2m[2] ATTR_ALIGNED(alignof(word128));
  mm128_and_256(x0m, CONST_BLOCK(in, 0)->w128, mask_a);
  mm128_and_256(x1m, CONST_BLOCK(in, 0)->w128, mask_b);
  mm128_and_256(x2m, CONST_BLOCK(in, 0)->w128, mask_c);

  mm128_shift_left_256(x0m, x0m, 2);
  mm128_shift_left_256(x1m, x1m, 1);

  word128 t0[2] ATTR_ALIGNED(alignof(word128)), t1[2] ATTR_ALIGNED(alignof(word128)),
      t2[2] ATTR_ALIGNED(alignof(word128));
  mm128_and_256(t0, x1m, x2m);
  mm128_and_256(t1, x0m, x2m);
  mm128_and_256(t2, x0m, x1m);

  mm128_xor_256(t0, t0, x0m);

  mm128_xor_256(x0m, x0m, x1m);
  mm128_xor_256(t1, t1, x0m);

  mm128_xor_256(t2, t2, x0m);
  mm128_xor_256(t2, t2, x2m);

  mm128_shift_right_256(t0, t0, 2);
  mm128_shift_right_256(t1, t1, 1);

  mm128_xor_256(t0, t0, t1);
  mm128_xor_256(in->w128, t0, t2);
}

ATTR_TARGET_S128
static inline void sbox_s128_lowmc_129_129_4(mzd_local_t* in) {
  sbox_s128_full(in, mask_129_129_43_a->w128, mask_129_129_43_b->w128, mask_129_129_43_c->w128);
}





#if !defined(NO_UINT64_FALLBACK)
// uint64 based implementation
#define IMPL uint64

#include "lowmc_129_129_4_fns_uint64.h"
#include "lowmc.c.i"

#include "lowmc_192_192_4_fns_uint64.h"
#include "lowmc.c.i"

#include "lowmc_255_255_4_fns_uint64.h"
#include "lowmc.c.i"

#include "lowmc_128_128_20_fns_uint64.h"
#include "lowmc.c.i"

#include "lowmc_192_192_30_fns_uint64.h"
#include "lowmc.c.i"

#include "lowmc_256_256_38_fns_uint64.h"
#include "lowmc.c.i"
#endif

#define FN_ATTR ATTR_TARGET_S128
#undef IMPL
#define IMPL s128


#include "lowmc_129_129_4_fns_s128.h"
#include "lowmc.c.i"

#include "lowmc_192_192_4_fns_s128.h"
#include "lowmc.c.i"

#include "lowmc_255_255_4_fns_s128.h"
#include "lowmc.c.i"

#include "lowmc_128_128_20_fns_s128.h"
#include "lowmc.c.i"

#include "lowmc_192_192_30_fns_s128.h"
#include "lowmc.c.i"

#include "lowmc_256_256_38_fns_s128.h"
#include "lowmc.c.i"


lowmc_implementation_f lowmc_get_implementation(const lowmc_parameters_t* lowmc) {
  assert((lowmc->m == 43 && lowmc->n == 129) || (lowmc->m == 64 && lowmc->n == 192) ||
         (lowmc->m == 85 && lowmc->n == 255) ||
         (lowmc->m == 10 && (lowmc->n == 128 || lowmc->n == 192 || lowmc->n == 256)));


  /* SSE2/NEON enabled instances */
  if (CPU_SUPPORTS_SSE2 || CPU_SUPPORTS_NEON) {
    /* Instances with partial Sbox layer */
    if (lowmc->m == 10) {
      switch (lowmc->n) {
      }
    }

    /* Instances with full Sbox layer */
    if (lowmc->n == 129 && lowmc->m == 43)
      return lowmc_s128_lowmc_129_129_4;
  }

#if !defined(NO_UINT64_FALLBACK)
  /* uint64_t implementations */
  /* Instances with partial Sbox layer */
  if (lowmc->m == 10) {
    switch (lowmc->n) {
    }
  }

  /* Instances with full Sbox layer */
  if (lowmc->n == 129 && lowmc->m == 43)
    return lowmc_uint64_lowmc_129_129_4;
#endif

  return NULL;
}

lowmc_store_implementation_f lowmc_store_get_implementation(const lowmc_parameters_t* lowmc) {
  assert((lowmc->m == 43 && lowmc->n == 129) || (lowmc->m == 64 && lowmc->n == 192) ||
         (lowmc->m == 85 && lowmc->n == 255) ||
         (lowmc->m == 10 && (lowmc->n == 128 || lowmc->n == 192 || lowmc->n == 256)));


  /* SSE2/NEON enabled instances */
  if (CPU_SUPPORTS_SSE2 || CPU_SUPPORTS_NEON) {
    /* Instances with partial Sbox layer */
    if (lowmc->m == 10) {
      switch (lowmc->n) {
      }
    }

    /* Instances with full Sbox layer */
    if (lowmc->n == 129 && lowmc->m == 43)
      return lowmc_store_s128_lowmc_129_129_4;
  }

#if !defined(NO_UINT64_FALLBACK)
  /* uint64_t implementations */
  /* Instances with partial Sbox layer */
  if (lowmc->m == 10) {
    switch (lowmc->n) {
    }
  }

  /* Instances with full Sbox layer */
  if (lowmc->n == 129 && lowmc->m == 43)
    return lowmc_store_uint64_lowmc_129_129_4;
#endif

  return NULL;
}

