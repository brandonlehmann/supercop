/*
  This file is for public-key generation
*/

#include "pk_gen.h"

#include "controlbits.h"
#include "transpose.h"
#include "params.h"
#include "benes.h"
#include "util.h"
#include "fft.h"

#include <stdint.h>

#define min(a, b) ((a < b) ? a : b)

#define nBlocks_I ((PK_NROWS + 255) / 256)
#define par_width 11

/* return number of trailing zeros of the non-zero input in */
static inline int ctz(uint64_t in)
{
	return __builtin_ctzll(in);
}

/* return 11...1 if x = y; return 00...0 otherwise */
static inline uint64_t same_mask(uint16_t x, uint16_t y)
{
	uint64_t mask;

	mask = x ^ y;
	mask -= 1;
	mask >>= 63;
	mask = -mask;

	return mask;
}

/* set m and mm to 11...1 if the i-th bit of x is 0 and the i-th bit of y is 1 */
/* set m and mm to 00...0 otherwise */
static inline void extract_01_masks(uint32_t *m, vec256 *mm, uint64_t *x, uint64_t *y, int i)
{
	*m = (((~x[ i>>6 ]) & y[ i>>6 ]) >> (i&63)) & 1;
	*m = -(*m);
	*mm = vec256_set1_32b(*m);
}

/* return a 128-bit vector of which each bits is set to the i-th bit of v */
static inline vec256 extract_mask256(uint64_t v[], int i)
{
	uint32_t mask;

	mask = (v[ i>>6 ] >> (i&63)) & 1;
	mask = -mask;

	return vec256_set1_32b(mask);
}

// swap x and y if m = 11...1
static inline void uint32_cswap(uint32_t *x, uint32_t *y, uint32_t m)
{
	uint32_t d;

	d = *x ^ *y;
	d &= m;
	*x ^= d;
	*y ^= d;
}

// swap x and y if m = 11...1
static inline void vec256_cswap(vec256 *x, vec256 *y, vec256 m)
{
	vec256 d;

	d = *x ^ *y;
	d &= m;
	*x ^= d;
	*y ^= d;
}

/* swap   x[i0] and   x[i1]  if x[i1] > x[i0] */
/* swap mat[i0] and mat[i1]  if x[i1] > x[i0] */
static inline void minmax_rows(uint32_t *x, vec256 (*mat)[par_width], int i0, int i1)
{
	int i;
	uint32_t m;
	vec256 mm;

	m = x[i1] - x[i0];
	m >>= 31;
	m = -m;
	mm  = vec256_set1_32b(m);

	uint32_cswap(&x[i0], &x[i1], m);

	for (i = 0; i < par_width; i++)
		vec256_cswap(&mat[i0][i], &mat[i1][i], mm);
}

/* merge first half of x[0],x[step],...,x[(2*n-1)*step] with second half */
/* requires n to be a power of 2 */
static void merge_rows(int n, int bound, uint32_t *x, vec256 (*mat)[par_width], int off, int step)
{
	int i;

	if (n == 1) 
	{
		if(off + step < bound)
			minmax_rows(x, mat, off, off + step);
	}
	else 
	{
		merge_rows(n/2, bound, x, mat, off, step * 2);
		merge_rows(n/2, bound, x, mat, off + step, step * 2);

		for (i = 1; i < 2*n-1 && off + (i+1) * step < bound; i += 2)
			minmax_rows(x, mat, off + i*step, off + (i+1)*step);
	}
}

// permute the rows of mat by sorting x
static void sort_rows(int n, int bound, uint32_t *x, vec256 (*mat)[par_width], int off)
{
	if (n <= 1) return;
	sort_rows(n/2, bound, x, mat, off);
	sort_rows(n/2, bound, x, mat, off + n/2);
	merge_rows(n/2, bound, x, mat, off, 1);
}

/* extract numbers represented in bitsliced form */
static void de_bitslicing(uint64_t * out, const vec256 in[][GFBITS])
{
	int i, j, k, r;
	uint64_t u = 0;

	for (i = 0; i < (1 << GFBITS); i++)
		out[i] = 0 ;

	for (i = 0; i < 32; i++)
	for (j = GFBITS-1; j >= 0; j--) {
	        u = vec256_extract(in[i][j], 0);
	        for (r = 0; r < 64; r++)
	        {
	                out[i*256 + 0*64 + r] <<= 1;
	                out[i*256 + 0*64 + r] |= (u >> r) & 1;
	        }
	        u = vec256_extract(in[i][j], 1);
	        for (r = 0; r < 64; r++)
	        {
	                out[i*256 + 1*64 + r] <<= 1;
	                out[i*256 + 1*64 + r] |= (u >> r) & 1;
	        }
	        u = vec256_extract(in[i][j], 2);
	        for (r = 0; r < 64; r++)
	        {
	                out[i*256 + 2*64 + r] <<= 1;
	                out[i*256 + 2*64 + r] |= (u >> r) & 1;
	        }
	        u = vec256_extract(in[i][j], 3);
	        for (r = 0; r < 64; r++)
	        {
	                out[i*256 + 3*64 + r] <<= 1;
	                out[i*256 + 3*64 + r] |= (u >> r) & 1;
	        }
	}
}

/* convert numbers into bitsliced form */
static void to_bitslicing_2x(vec256 out0[][GFBITS], vec256 out1[][GFBITS], const uint64_t * in)
{
	int i, j, k, r;
	uint64_t u[2][4];

	for (i = 0; i < 32; i++)
	for (j = GFBITS-1; j >= 0; j--)
	{
		for (k = 0; k < 4; k++)
		for (r = 63; r >= 0; r--)
		{
			u[0][k] <<= 1;
			u[0][k] |= (in[i*256 + k*64 + r] >> (GFBITS-1-j)) & 1;

			u[1][k] <<= 1;
			u[1][k] |= (in[i*256 + k*64 + r] >> (j + GFBITS)) & 1;
		}
    
		out0[i][j] = vec256_set4x(u[0][0], u[0][1], u[0][2], u[0][3]);
		out1[i][j] = vec256_set4x(u[1][0], u[1][1], u[1][2], u[1][3]);
	}
}

static int mov_columns(uint64_t mat[][ nBlocks_I * 4 ], uint32_t * perm)
{
	int i, j;
	uint64_t buf[32], pivot_col[32], t, d, mask, allone = -1; 
       
	int row = PK_NROWS - 32;
	int block_idx = row/64;
	int tail = row % 64;

	// extract the 32x64 matrix

	for (i = 0; i < 32; i++)
		buf[i] = (mat[ row + i ][ block_idx + 0 ] >> tail) | 
		         (mat[ row + i ][ block_idx + 1 ] << (64-tail));
        
	// compute the column indices of pivots by Gaussian elimination.
	// the indices are stored in pivot_col

	for (i = 0; i < 32; i++)
	{
		t = buf[i];
		for (j = i+1; j < 32; j++)
			t |= buf[j];

		if (t == 0) return -1; // return if buf is not full rank

		pivot_col[i] = ctz(t);

		for (j = i+1; j < 32; j++) { mask = (buf[i] >> pivot_col[i]) & 1; mask -= 1;    buf[i] ^= buf[j] & mask; }
		for (j = i+1; j < 32; j++) { mask = (buf[j] >> pivot_col[i]) & 1; mask = -mask; buf[j] ^= buf[i] & mask; }
	}
   
	// updating permutation
  
	for (i = 0;   i < 32; i++)
	for (j = i+1; j < 64; j++)
		uint32_cswap(&perm[ row + i ], &perm[ row + j ], same_mask(j, pivot_col[i]));
   
	// moving columns of mat according to the column indices of pivots

	for (i = 0; i < PK_NROWS; i++)
	{
		t = (mat[ i ][ block_idx + 0 ] >> tail) | 
		    (mat[ i ][ block_idx + 1 ] << (64-tail));
                
		for (j = 0; j < 32; j++)
		{
			d  = t >> j;
			d ^= t >> pivot_col[j];
			d &= 1;
        
			t ^= d << pivot_col[j];
			t ^= d << j;
		}

		mat[ i ][ block_idx + 0 ] = (mat[ i ][ block_idx + 0 ] & (allone >> (64-tail))) | (t << tail);
		mat[ i ][ block_idx + 1 ] = (mat[ i ][ block_idx + 1 ] & (allone << tail)) | (t >> (64-tail));
	}

	return 0;
}

/* input: irr, an irreducible polynomial */
/*        perm, a permutation represented as an array of 32-bit numbers */
/* output: pk, the public key*/
/* return: 0 if pk is successfully generated, -1 otherwise */
int pk_gen(unsigned char * pk, const unsigned char * irr, uint32_t * perm)
{
//	const int nBlocks_I = (PK_NROWS + 255) / 256;
	const int head = PK_NROWS % 64;
	const int tail = PK_NCOLS % 64;
	const uint64_t one = 1;

	int i, j, k, b;
	int row, c;
	
	union
	{
		uint64_t w[ PK_NROWS ][ nBlocks_I * 4 ];
		vec256 v[ PK_NROWS ][ nBlocks_I ];
	} mat;	

	union 
	{
		uint64_t w[ PK_NROWS ][ 44 ];
		vec256 v[ PK_NROWS ][ 11 ];
	} par;

	uint32_t m;	
	vec256 mm;

	vec128 sk_int[ GFBITS ];

	vec256 consts[ 32 ][ GFBITS ];
	vec256 eval[ 32 ][ GFBITS ];
	vec256 prod[ 32 ][ GFBITS ];
	vec256 tmp[ GFBITS ];

	uint64_t list[1 << GFBITS];
	uint64_t buf[ PK_NROWS ];
	uint64_t t;

	uint32_t ind[ PK_NROWS ];
	uint32_t ind_inv[ PK_NROWS ];

	// compute the inverses 

	irr_load(sk_int, irr);

	fft(eval, sk_int);

	vec256_copy(prod[0], eval[0]);

	for (i = 1; i < 32; i++)
		vec256_mul(prod[i], prod[i-1], eval[i]);

	vec256_inv(tmp, prod[31]);

	for (i = 30; i >= 0; i--)
	{
		vec256_mul(prod[i+1], prod[i], tmp);
		vec256_mul(tmp, tmp, eval[i+1]);
	}

	vec256_copy(prod[0], tmp);

	// fill matrix 

	de_bitslicing(list, prod);

	for (i = 0; i < (1 << GFBITS); i++)
	{	
		list[i] <<= GFBITS;
		list[i] |= i;	
		list[i] |= ((uint64_t) perm[i]) << 31;
	}

	sort_63b(1 << GFBITS, list);

	for (i = 1; i < (1 << GFBITS); i++)
		if ((list[i-1] >> 31) == (list[i] >> 31))
			return -1;

	to_bitslicing_2x(consts, prod, list);

	for (i = 0; i < (1 << GFBITS); i++)
		perm[i] = list[i] & GFMASK;

	for (j = 0; j < nBlocks_I; j++)
	for (k = 0; k < GFBITS; k++)
		mat.v[ k ][ j ] = prod[ j ][ k ];

	for (i = 1; i < SYS_T; i++)
	for (j = 0; j < nBlocks_I; j++)
	{
		vec256_mul(prod[j], prod[j], consts[j]);

		for (k = 0; k < GFBITS; k++)
			mat.v[ i*GFBITS + k ][ j ] = prod[ j ][ k ];
	}

	// gaussian elimination to obtain L, U, and P such that LP M = U
	// L and U are stored in the space of M
	// P is stored in ind

	for (i = 0; i < PK_NROWS; i++)
		ind_inv[i] = ind[ i ] = i;

	for (row = 0; row < PK_NROWS; row++)
	{
		i = row >> 6;
		j = row & 63;

		if (row == PK_NROWS - 32)
		{
			if (mov_columns(mat.w, perm))
				return -1;
		}

		for (k = row + 1; k < PK_NROWS; k++)
		{
			extract_01_masks(&m , &mm, mat.w[ row ], mat.w[ k ], row);

			uint32_cswap(&ind[row], &ind[k], m);

			for (c = 0; c < nBlocks_I; c++)
				vec256_cswap(&mat.v[ row ][ c ], &mat.v[ k ][ c ], mm);
		}

		if ( ((mat.w[ row ][ i ] >> j) & 1) == 0 ) // return if not systematic
		{
			return -1;
		}

		for (k = row+1; k < PK_NROWS; k++)
		{
			t = mat.w[ k ][ i ] & (one << j);
			mm = extract_mask256(mat.w[k], row);

			for (c = 0; c < nBlocks_I; c++)
				mat.v[ k ][ c ] ^= mat.v[ row ][ c ] & mm;

			mat.w[ k ][ i ] |= t;
		}
	}

	// apply the linear map to the non-systematic part

	composeinv(PK_NROWS, ind_inv, ind_inv, ind);

	for (j = 6; j < 28; j += 11)
	{
		for (k = 0; k < GFBITS; k++)
		for (b = 0; b < 11; b++) 
			par.v[ k ][ b ] = prod[ j+b ][ k ];
		      
		for (i = 1; i < SYS_T; i++)
		{
			for (b = 0; b < 11; b++) 
				vec256_mul(prod[j+b], prod[j+b], consts[j+b]);
        
			for (k = 0; k < GFBITS; k++)
			for (b = 0; b < 11; b++) 
				par.v[ i*GFBITS + k ][ b ] = prod[ j+b ][ k ];
		}

		// apply P

		for (i = 0; i < PK_NROWS; i++)
			ind[i] = ind_inv[i];

		sort_rows((1 << GFBITS)/4, PK_NROWS, ind, par.v, 0);

		// apply L

		for (row = PK_NROWS-1; row >= 0; row--)
		for (i = row-1; i >= 0; i--)
		{
			mm = extract_mask256(mat.w[row], i);
    
			for (k = 0; k < 11; k++)
				par.v[ row ][ k ] ^= par.v[ i ][ k ] & mm;
		}
    
		// apply U^-1

		if (j == 6)
		{
			for (i = 0; i < PK_NROWS; i++)
				par.v[i][0] = mat.v[ i ][ nBlocks_I-1 ];
		}

		for (row = PK_NROWS-1; row >= 0; row--)
		for (i = PK_NROWS-1; i > row; i--)
		{
			mm = extract_mask256(mat.w[row], i);
    
			for (k = 0; k < 11; k++)
				par.v[ row ][ k ] ^= par.v[ i ][ k ] & mm;
		}

		if (j == 6)
		{
			for (row = 0; row < PK_NROWS; row++)
			{
				for (i = 0; i < 43; i++)
					store8(pk + PK_ROW_BYTES * row + i*8, (par.w[row][i] >> head) | (par.w[row][i+1] << (64-head)));

				buf[row] = par.w[row][i] >> head;
			}
		}
		else
		{
			for (row = 0; row < PK_NROWS; row++)
			{
				store8(pk + PK_ROW_BYTES * row + 43*8, buf[row] | (par.w[row][0] << (64-head)));

				for (i = 0; i < 40; i++)
					store8(pk + PK_ROW_BYTES * row + (i+44)*8, (par.w[row][i] >> head) | (par.w[row][i+1] << (64-head)));

				par.w[row][i] >>= head;
				par.w[row][i] &= (one << tail) - 1;

				store_i(pk + PK_ROW_BYTES * row + (i+44)*8, par.w[row][i], (tail + 7)/8);
			}
		}

	}

	//

	return 0;
}

