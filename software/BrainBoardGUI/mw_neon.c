/* Copyright 2010-2011 The MathWorks, Inc. */
        
#include "mw_neon.h"

/*
 * Matrix + Matrix
 */

/* f32x4 add */
void mw_neon_mm_add_f32x4(float * A, int Row, int Col, float * B, float * C)
{
	float32x4_t neon_a, neon_b, neon_c;
	int size = Row * Col;
	int i = 0;
	int k = 0;

	for (i = 4; i <= size ; i+=4)
	{
		k = i - 4;
		neon_a = vld1q_f32(A + k);
		neon_b = vld1q_f32(B + k);
		neon_c = vaddq_f32(neon_a, neon_b);
		vst1q_f32(C + k, neon_c);
	}

	k = i - 4;
    for (i = 0; i < size % 4; i++)
	{
		C[k + i] = A[k + i] + B[k + i];
	}
}

/* s32x4 add */
void mw_neon_mm_add_s32x4(int * A, int Row, int Col, int * B, int * C)
{
	int32x4_t neon_a, neon_b, neon_c;
	int size = Row * Col;
	int i = 0;
	int k = 0;

	for (i = 4; i <= size ; i+=4)
	{
		k = i - 4;
		neon_a = vld1q_s32(A + k);
		neon_b = vld1q_s32(B + k);
		neon_c = vaddq_s32(neon_a, neon_b);
		vst1q_s32(C + k, neon_c);
	}

	k = i - 4;
    for (i = 0; i < size % 4; i++)
	{
		C[k + i] = A[k + i] + B[k + i];
	}
}

/* s16x8 add */
void mw_neon_mm_add_s16x8(short * A, int Row, int Col, short * B, short * C)
{
	int16x8_t neon_a, neon_b, neon_c;
	int size = Row * Col;
	int i = 0;
	int k = 0;

	for (i = 8; i <= size ; i+=8)
	{
		k = i - 8;
		neon_a = vld1q_s16(A + k);
		neon_b = vld1q_s16(B + k);
		neon_c = vaddq_s16(neon_a, neon_b);
		vst1q_s16(C + k, neon_c);
	}

	k = i - 8;
    for (i = 0; i < size % 8; i++)
	{
		C[k + i] = A[k + i] + B[k + i];
	}
}

/* s8x16 add */
void mw_neon_mm_add_s8x16(signed char * A, int Row, int Col, signed char * B, signed char * C)
{
	int8x16_t neon_a, neon_b, neon_c;
	int size = Row * Col;
	int i = 0;
	int k = 0;

	for (i = 16; i <= size ; i+=16)
	{
		k = i - 16;
		neon_a = vld1q_s8(A + k);
		neon_b = vld1q_s8(B + k);
		neon_c = vaddq_s8(neon_a, neon_b);
		vst1q_s8(C + k, neon_c);
	}

	k = i - 16;
    for (i = 0; i < size % 16; i++)
	{
		C[k + i] = A[k + i] + B[k + i];
	}
}

/* u32x4 add */
void mw_neon_mm_add_u32x4(unsigned int * A, int Row, int Col, unsigned int * B, unsigned int * C)
{
	uint32x4_t neon_a, neon_b, neon_c;
	int size = Row * Col;
	int i = 0;
	int k = 0;

	for (i = 4; i <= size ; i+=4)
	{
		k = i - 4;
		neon_a = vld1q_u32(A + k);
		neon_b = vld1q_u32(B + k);
		neon_c = vaddq_u32(neon_a, neon_b);
		vst1q_u32(C + k, neon_c);
	}

	k = i - 4;
    for (i = 0; i < size % 4; i++)
	{
		C[k + i] = A[k + i] + B[k + i];
	}
}

/* u16x8 add */
void mw_neon_mm_add_u16x8(unsigned short * A, int Row, int Col, unsigned short * B, unsigned short * C)
{
	uint16x8_t neon_a, neon_b, neon_c;
	int size = Row * Col;
	int i = 0;
	int k = 0;

	for (i = 8; i <= size ; i+=8)
	{
		k = i - 8;
		neon_a = vld1q_u16(A + k);
		neon_b = vld1q_u16(B + k);
		neon_c = vaddq_u16(neon_a, neon_b);
		vst1q_u16(C + k, neon_c);
	}

	k = i - 8;
    for (i = 0; i < size % 8; i++)
	{
		C[k + i] = A[k + i] + B[k + i];
	}
}

/* u8x16 add */
void mw_neon_mm_add_u8x16(unsigned char * A, int Row, int Col, unsigned char * B, unsigned char * C)
{
	uint8x16_t neon_a, neon_b, neon_c;
	int size = Row * Col;
	int i = 0;
	int k = 0;

	for (i = 16; i <= size ; i+=16)
	{
		k = i - 16;
		neon_a = vld1q_u8(A + k);
		neon_b = vld1q_u8(B + k);
		neon_c = vaddq_u8(neon_a, neon_b);
		vst1q_u8(C + k, neon_c);
	}

	k = i - 16;
    for (i = 0; i < size % 16; i++)
	{
		C[k + i] = A[k + i] + B[k + i];
	}
}

/* s32x4 saturated add */
void mw_neon_mm_qadd_s32x4(int * A, int Row, int Col, int * B, int * C)
{
	int32x4_t neon_a, neon_b, neon_c;
	int size = Row * Col;
	int i = 0;
	int k = 0;

	for (i = 4; i <= size ; i+=4)
	{
		k = i - 4;
		neon_a = vld1q_s32(A + k);
		neon_b = vld1q_s32(B + k);
		neon_c = vqaddq_s32(neon_a, neon_b);
		vst1q_s32(C + k, neon_c);
	}

	k = i - 4;
    for (i = 0; i < size % 4; i++)
	{
		C[k + i] = A[k + i] + B[k + i];
	}
}

/* s16x8 saturated add */
void mw_neon_mm_qadd_s16x8(short * A, int Row, int Col, short * B, short * C)
{
	int16x8_t neon_a, neon_b, neon_c;
	int size = Row * Col;
	int i = 0;
	int k = 0;

	for (i = 8; i <= size ; i+=8)
	{
		k = i - 8;
		neon_a = vld1q_s16(A + k);
		neon_b = vld1q_s16(B + k);
		neon_c = vqaddq_s16(neon_a, neon_b);
		vst1q_s16(C + k, neon_c);
	}

	k = i - 8;
    for (i = 0; i < size % 8; i++)
	{
		C[k + i] = A[k + i] + B[k + i];
	}
}

/* s8x16 saturated add */
void mw_neon_mm_qadd_s8x16(signed char * A, int Row, int Col, signed char * B, signed char * C)
{
	int8x16_t neon_a, neon_b, neon_c;
	int size = Row * Col;
	int i = 0;
	int k = 0;

	for (i = 16; i <= size ; i+=16)
	{
		k = i - 16;
		neon_a = vld1q_s8(A + k);
		neon_b = vld1q_s8(B + k);
		neon_c = vqaddq_s8(neon_a, neon_b);
		vst1q_s8(C + k, neon_c);
	}

	k = i - 16;
    for (i = 0; i < size % 16; i++)
	{
		C[k + i] = A[k + i] + B[k + i];
	}
}

/* u32x4 saturated add */
void mw_neon_mm_qadd_u32x4(unsigned int * A, int Row, int Col, unsigned int * B, unsigned int * C)
{
	uint32x4_t neon_a, neon_b, neon_c;
	int size = Row * Col;
	int i = 0;
	int k = 0;

	for (i = 4; i <= size ; i+=4)
	{
		k = i - 4;
		neon_a = vld1q_u32(A + k);
		neon_b = vld1q_u32(B + k);
		neon_c = vqaddq_u32(neon_a, neon_b);
		vst1q_u32(C + k, neon_c);
	}

	k = i - 4;
    for (i = 0; i < size % 4; i++)
	{
		C[k + i] = A[k + i] + B[k + i];
	}
}

/* u16x8 saturated add */
void mw_neon_mm_qadd_u16x8(unsigned short * A, int Row, int Col, unsigned short * B, unsigned short * C)
{
	uint16x8_t neon_a, neon_b, neon_c;
	int size = Row * Col;
	int i = 0;
	int k = 0;

	for (i = 8; i <= size ; i+=8)
	{
		k = i - 8;
		neon_a = vld1q_u16(A + k);
		neon_b = vld1q_u16(B + k);
		neon_c = vqaddq_u16(neon_a, neon_b);
		vst1q_u16(C + k, neon_c);
	}

	k = i - 8;
    for (i = 0; i < size % 8; i++)
	{
		C[k + i] = A[k + i] + B[k + i];
	}
}

/* u8x16 saturated add */
void mw_neon_mm_qadd_u8x16(unsigned char * A, int Row, int Col, unsigned char * B, unsigned char * C)
{
	uint8x16_t neon_a, neon_b, neon_c;
	int size = Row * Col;
	int i = 0;
	int k = 0;

	for (i = 16; i <= size ; i+=16)
	{
		k = i - 16;
		neon_a = vld1q_u8(A + k);
		neon_b = vld1q_u8(B + k);
		neon_c = vqaddq_u8(neon_a, neon_b);
		vst1q_u8(C + k, neon_c);
	}

	k = i - 16;
    for (i = 0; i < size % 16; i++)
	{
		C[k + i] = A[k + i] + B[k + i];
	}
}

/*
 * Matrix - Matrix
 */

/* f32x4 sub */
void mw_neon_mm_sub_f32x4(float * A, int Row, int Col, float * B, float * C)
{
	float32x4_t neon_a, neon_b, neon_c;
	int size = Row * Col;
	int i = 0;
	int k = 0;

	for (i = 4; i <= size ; i+=4)
	{
		k = i - 4;
		neon_a = vld1q_f32(A + k);
		neon_b = vld1q_f32(B + k);
		neon_c = vsubq_f32(neon_a, neon_b);
		vst1q_f32(C + k, neon_c);
	}

	k = i - 4;
    for (i = 0; i < size % 4; i++)
	{
		C[k + i] = A[k + i] - B[k + i];
	}
}

/* s32x4 sub */
void mw_neon_mm_sub_s32x4(int * A, int Row, int Col, int * B, int * C)
{
	int32x4_t neon_a, neon_b, neon_c;
	int size = Row * Col;
	int i = 0;
	int k = 0;

	for (i = 4; i <= size ; i+=4)
	{
		k = i - 4;
		neon_a = vld1q_s32(A + k);
		neon_b = vld1q_s32(B + k);
		neon_c = vsubq_s32(neon_a, neon_b);
		vst1q_s32(C + k, neon_c);
	}

	k = i - 4;
    for (i = 0; i < size % 4; i++)
	{
		C[k + i] = A[k + i] - B[k + i];
	}
}

/* s16x8 sub */
void mw_neon_mm_sub_s16x8(short * A, int Row, int Col, short * B, short * C)
{
	int16x8_t neon_a, neon_b, neon_c;
	int size = Row * Col;
	int i = 0;
	int k = 0;

	for (i = 8; i <= size ; i+=8)
	{
		k = i - 8;
		neon_a = vld1q_s16(A + k);
		neon_b = vld1q_s16(B + k);
		neon_c = vsubq_s16(neon_a, neon_b);
		vst1q_s16(C + k, neon_c);
	}

	k = i - 8;
    for (i = 0; i < size % 8; i++)
	{
		C[k + i] = A[k + i] - B[k + i];
	}
}

/* s8x16 sub */
void mw_neon_mm_sub_s8x16(signed char * A, int Row, int Col, signed char * B, signed char * C)
{
	int8x16_t neon_a, neon_b, neon_c;
	int size = Row * Col;
	int i = 0;
	int k = 0;

	for (i = 16; i <= size ; i+=16)
	{
		k = i - 16;
		neon_a = vld1q_s8(A + k);
		neon_b = vld1q_s8(B + k);
		neon_c = vsubq_s8(neon_a, neon_b);
		vst1q_s8(C + k, neon_c);
	}

	k = i - 16;
    for (i = 0; i <= size % 16; i++)
	{
		C[k + i] = A[k + i] - B[k + i];
	}
}

/* u32x4 sub */
void mw_neon_mm_sub_u32x4(unsigned int * A, int Row, int Col, unsigned int * B, unsigned int * C)
{
	uint32x4_t neon_a, neon_b, neon_c;
	int size = Row * Col;
	int i = 0;
	int k = 0;

	for (i = 4; i <= size ; i+=4)
	{
		k = i - 4;
		neon_a = vld1q_u32(A + k);
		neon_b = vld1q_u32(B + k);
		neon_c = vsubq_u32(neon_a, neon_b);
		vst1q_u32(C + k, neon_c);
	}

	k = i - 4;
    for (i = 0; i < size % 4; i++)
	{
		C[k + i] = A[k + i] - B[k + i];
	}
}

/* u16x8 sub */
void mw_neon_mm_sub_u16x8(unsigned short * A, int Row, int Col, unsigned short * B, unsigned short * C)
{
	uint16x8_t neon_a, neon_b, neon_c;
	int size = Row * Col;
	int i = 0;
	int k = 0;

	for (i = 8; i <= size ; i+=8)
	{
		k = i - 8;
		neon_a = vld1q_u16(A + k);
		neon_b = vld1q_u16(B + k);
		neon_c = vsubq_u16(neon_a, neon_b);
		vst1q_u16(C + k, neon_c);
	}

	k = i - 8;
    for (i = 0; i < size % 8; i++)
	{
		C[k + i] = A[k + i] - B[k + i];
	}
}

/* u8x16 sub */
void mw_neon_mm_sub_u8x16(unsigned char * A, int Row, int Col, unsigned char * B, unsigned char * C)
{
	uint8x16_t neon_a, neon_b, neon_c;
	int size = Row * Col;
	int i = 0;
	int k = 0;

	for (i = 16; i <= size ; i+=16)
	{
		k = i - 16;
		neon_a = vld1q_u8(A + k);
		neon_b = vld1q_u8(B + k);
		neon_c = vsubq_u8(neon_a, neon_b);
		vst1q_u8(C + k, neon_c);
	}

	k = i - 16;
    for (i = 0; i < size % 16; i++)
	{
		C[k + i] = A[k + i] - B[k + i];
	}
}

/* s32x4 saturated sub */
void mw_neon_mm_qsub_s32x4(int * A, int Row, int Col, int * B, int * C)
{
	int32x4_t neon_a, neon_b, neon_c;
	int size = Row * Col;
	int i = 0;
	int k = 0;

	for (i = 4; i <= size ; i+=4)
	{
		k = i - 4;
		neon_a = vld1q_s32(A + k);
		neon_b = vld1q_s32(B + k);
		neon_c = vqsubq_s32(neon_a, neon_b);
		vst1q_s32(C + k, neon_c);
	}

	k = i - 4;
    for (i = 0; i < size % 4; i++)
	{
		C[k + i] = A[k + i] - B[k + i];
	}
}

/* s16x8 saturated sub */
void mw_neon_mm_qsub_s16x8(short * A, int Row, int Col, short * B, short * C)
{
	int16x8_t neon_a, neon_b, neon_c;
	int size = Row * Col;
	int i = 0;
	int k = 0;

	for (i = 8; i <= size ; i+=8)
	{
		k = i - 8;
		neon_a = vld1q_s16(A + k);
		neon_b = vld1q_s16(B + k);
		neon_c = vqsubq_s16(neon_a, neon_b);
		vst1q_s16(C + k, neon_c);
	}

	k = i - 8;
    for (i = 0; i < size % 8; i++)
	{
		C[k + i] = A[k + i] - B[k + i];
	}
}

/* s8x16 saturated sub */
void mw_neon_mm_qsub_s8x16(signed char * A, int Row, int Col, signed char * B, signed char * C)
{
	int8x16_t neon_a, neon_b, neon_c;
	int size = Row * Col;
	int i = 0;
	int k = 0;

	for (i = 16; i <= size ; i+=16)
	{
		k = i - 16;
		neon_a = vld1q_s8(A + k);
		neon_b = vld1q_s8(B + k);
		neon_c = vqsubq_s8(neon_a, neon_b);
		vst1q_s8(C + k, neon_c);
	}

	k = i - 16;
    for (i = 0; i < size % 16; i++)
	{
		C[k + i] = A[k + i] - B[k + i];
	}
}

/* u32x4 saturated sub */
void mw_neon_mm_qsub_u32x4(unsigned int * A, int Row, int Col, unsigned int * B, unsigned int * C)
{
	uint32x4_t neon_a, neon_b, neon_c;
	int size = Row * Col;
	int i = 0;
	int k = 0;

	for (i = 4; i <= size ; i+=4)
	{
		k = i - 4;
		neon_a = vld1q_u32(A + k);
		neon_b = vld1q_u32(B + k);
		neon_c = vqsubq_u32(neon_a, neon_b);
		vst1q_u32(C + k, neon_c);
	}

	k = i - 4;
    for (i = 0; i < size % 4; i++)
	{
		C[k + i] = A[k + i] - B[k + i];
	}
}

/* u16x8 saturated sub */
void mw_neon_mm_qsub_u16x8(unsigned short * A, int Row, int Col, unsigned short * B, unsigned short * C)
{
	uint16x8_t neon_a, neon_b, neon_c;
	int size = Row * Col;
	int i = 0;
	int k = 0;

	for (i = 8; i <= size ; i+=8)
	{
		k = i - 8;
		neon_a = vld1q_u16(A + k);
		neon_b = vld1q_u16(B + k);
		neon_c = vqsubq_u16(neon_a, neon_b);
		vst1q_u16(C + k, neon_c);
	}

	k = i - 8;
    for (i = 0; i < size % 8; i++)
	{
		C[k + i] = A[k + i] + B[k + i];
	}
}

/* u8x16 saturated sub */
void mw_neon_mm_qsub_u8x16(unsigned char * A, int Row, int Col, unsigned char * B, unsigned char * C)
{
	uint8x16_t neon_a, neon_b, neon_c;
	int size = Row * Col;
	int i = 0;
	int k = 0;

	for (i = 16; i <= size ; i+=16)
	{
		k = i - 16;
		neon_a = vld1q_u8(A + k);
		neon_b = vld1q_u8(B + k);
		neon_c = vqsubq_u8(neon_a, neon_b);
		vst1q_u8(C + k, neon_c);
	}

	k = i - 16;
    for (i = 0; i < size % 16; i++)
	{
		C[k + i] = A[k + i] - B[k + i];
	}
}

/*
 * Matrix * Vector
 */

/* f32x4 mv mul */
void mw_neon_mv_mul_f32x4(float * A, int Row, int T, float * B, float * C)
{
	int i = 0;
	int k = 0;

	float32x4_t neon_b, neon_c;
	float32x4_t neon_a0, neon_a1, neon_a2, neon_a3;
	float32x4_t neon_b0, neon_b1, neon_b2, neon_b3;

	for (i = 0; i < Row; i+=4)
	{
		neon_c = vmovq_n_f32(0);

		for (k = 0; k < T; k+=4)
		{
			int j = k * T + i;

			neon_a0 = vld1q_f32(A + j);
			neon_a1 = vld1q_f32(A + j + Row);
			neon_a2 = vld1q_f32(A + j + 2 * Row);
			neon_a3 = vld1q_f32(A + j + 3 * Row);

			neon_b = vld1q_f32(B + k);
			neon_b0 = vdupq_n_f32(vgetq_lane_f32(neon_b, 0));
			neon_b1 = vdupq_n_f32(vgetq_lane_f32(neon_b, 1));
			neon_b2 = vdupq_n_f32(vgetq_lane_f32(neon_b, 2));
			neon_b3 = vdupq_n_f32(vgetq_lane_f32(neon_b, 3));

			neon_c = vaddq_f32(vmulq_f32(neon_a0, neon_b0), neon_c);
			neon_c = vaddq_f32(vmulq_f32(neon_a1, neon_b1), neon_c);
			neon_c = vaddq_f32(vmulq_f32(neon_a2, neon_b2), neon_c);
			neon_c = vaddq_f32(vmulq_f32(neon_a3, neon_b3), neon_c);

		}

		vst1q_f32(C + i, neon_c);
	}
}

/* s32x4 mv mul */
void mw_neon_mv_mul_s32x4(int * A, int Row, int T, int * B, int * C)
{
	int i = 0;
	int k = 0;

	int32x4_t neon_b, neon_c;
	int32x4_t neon_a0, neon_a1, neon_a2, neon_a3;
	int32x4_t neon_b0, neon_b1, neon_b2, neon_b3;

	for (i = 0; i < Row; i+=4)
	{
		neon_c = vmovq_n_s32(0);

		for (k = 0; k < T; k+=4)
		{
			int j = k * T + i;

			neon_a0 = vld1q_s32(A + j);
			j+=Row;
			neon_a1 = vld1q_s32(A + j);
			j+=Row;
			neon_a2 = vld1q_s32(A + j);
			j+=Row;
			neon_a3 = vld1q_s32(A + j);

			neon_b = vld1q_s32(B + k);
			neon_b0 = vdupq_n_s32(vgetq_lane_s32(neon_b, 0));
			neon_b1 = vdupq_n_s32(vgetq_lane_s32(neon_b, 1));
			neon_b2 = vdupq_n_s32(vgetq_lane_s32(neon_b, 2));
			neon_b3 = vdupq_n_s32(vgetq_lane_s32(neon_b, 3));

			neon_c = vaddq_s32(vmulq_s32(neon_a0, neon_b0), neon_c);
			neon_c = vaddq_s32(vmulq_s32(neon_a1, neon_b1), neon_c);
			neon_c = vaddq_s32(vmulq_s32(neon_a2, neon_b2), neon_c);
			neon_c = vaddq_s32(vmulq_s32(neon_a3, neon_b3), neon_c);

		}

		vst1q_s32(C + i, neon_c);
	}
}

/* s16x8 mv mul */
void mw_neon_mv_mul_s16x8(short * A, int Row, int T, short * B, short * C)
{
	int i = 0;
	int k = 0;

	int16x8_t neon_b, neon_c;
	int16x8_t neon_a0, neon_a1, neon_a2, neon_a3, neon_a4, neon_a5, neon_a6, neon_a7;
	int16x8_t neon_b0, neon_b1, neon_b2, neon_b3, neon_b4, neon_b5, neon_b6, neon_b7;

	for (i = 0; i < Row; i+=8)
	{
		neon_c = vmovq_n_s16(0);

		for (k = 0; k < T; k+=8)
		{
			int j = k * T + i;

			neon_a0 = vld1q_s16(A + j);
			j+=Row;
			neon_a1 = vld1q_s16(A + j);
			j+=Row;
			neon_a2 = vld1q_s16(A + j);
			j+=Row;
			neon_a3 = vld1q_s16(A + j);
			j+=Row;
			neon_a4 = vld1q_s16(A + j);
			j+=Row;
			neon_a5 = vld1q_s16(A + j);
			j+=Row;
			neon_a6 = vld1q_s16(A + j);
			j+=Row;
			neon_a7 = vld1q_s16(A + j);

			neon_b = vld1q_s16(B + k);
			neon_b0 = vdupq_n_s16(vgetq_lane_s16(neon_b, 0));
			neon_b1 = vdupq_n_s16(vgetq_lane_s16(neon_b, 1));
			neon_b2 = vdupq_n_s16(vgetq_lane_s16(neon_b, 2));
			neon_b3 = vdupq_n_s16(vgetq_lane_s16(neon_b, 3));
			neon_b4 = vdupq_n_s16(vgetq_lane_s16(neon_b, 4));
			neon_b5 = vdupq_n_s16(vgetq_lane_s16(neon_b, 5));
			neon_b6 = vdupq_n_s16(vgetq_lane_s16(neon_b, 6));
			neon_b7 = vdupq_n_s16(vgetq_lane_s16(neon_b, 7));

			neon_c = vaddq_s16(vmulq_s16(neon_a0, neon_b0), neon_c);
			neon_c = vaddq_s16(vmulq_s16(neon_a1, neon_b1), neon_c);
			neon_c = vaddq_s16(vmulq_s16(neon_a2, neon_b2), neon_c);
			neon_c = vaddq_s16(vmulq_s16(neon_a3, neon_b3), neon_c);
			neon_c = vaddq_s16(vmulq_s16(neon_a4, neon_b4), neon_c);
			neon_c = vaddq_s16(vmulq_s16(neon_a5, neon_b5), neon_c);
			neon_c = vaddq_s16(vmulq_s16(neon_a6, neon_b6), neon_c);
			neon_c = vaddq_s16(vmulq_s16(neon_a7, neon_b7), neon_c);

		}

		vst1q_s16(C + i, neon_c);
	}
}

/* s8x16 mv mul */
void mw_neon_mv_mul_s8x16(signed char * A, int Row, int T, signed char * B, signed char * C)
{
	int i = 0;
	int k = 0;

	int8x16_t neon_b, neon_c;
	int8x16_t neon_a0, neon_a1, neon_a2, neon_a3, neon_a4, neon_a5, neon_a6, neon_a7;
	int8x16_t neon_a8, neon_a9, neon_a10, neon_a11, neon_a12, neon_a13, neon_a14, neon_a15;
	int8x16_t neon_b0, neon_b1, neon_b2, neon_b3, neon_b4, neon_b5, neon_b6, neon_b7;
	int8x16_t neon_b8, neon_b9, neon_b10, neon_b11, neon_b12, neon_b13, neon_b14, neon_b15;

	for (i = 0; i < Row; i+=16)
	{
		neon_c = vmovq_n_s8(0);

		for (k = 0; k < T; k+=16)
		{
			int j = k * T + i;

			neon_a0 = vld1q_s8(A + j);
			j+=Row;
			neon_a1 = vld1q_s8(A + j);
			j+=Row;
			neon_a2 = vld1q_s8(A + j);
			j+=Row;
			neon_a3 = vld1q_s8(A + j);
			j+=Row;
			neon_a4 = vld1q_s8(A + j);
			j+=Row;
			neon_a5 = vld1q_s8(A + j);
			j+=Row;
			neon_a6 = vld1q_s8(A + j);
			j+=Row;
			neon_a7 = vld1q_s8(A + j);
			j+=Row;
			neon_a8 = vld1q_s8(A + j);
			j+=Row;
			neon_a9 = vld1q_s8(A + j);
			j+=Row;
			neon_a10 = vld1q_s8(A + j);
			j+=Row;
			neon_a11 = vld1q_s8(A + j);
			j+=Row;
			neon_a12 = vld1q_s8(A + j);
			j+=Row;
			neon_a13 = vld1q_s8(A + j);
			j+=Row;
			neon_a14 = vld1q_s8(A + j);
			j+=Row;
			neon_a15 = vld1q_s8(A + j);

			neon_b = vld1q_s8(B + k);
			neon_b0 = vdupq_n_s8(vgetq_lane_s8(neon_b, 0));
			neon_b1 = vdupq_n_s8(vgetq_lane_s8(neon_b, 1));
			neon_b2 = vdupq_n_s8(vgetq_lane_s8(neon_b, 2));
			neon_b3 = vdupq_n_s8(vgetq_lane_s8(neon_b, 3));
			neon_b4 = vdupq_n_s8(vgetq_lane_s8(neon_b, 4));
			neon_b5 = vdupq_n_s8(vgetq_lane_s8(neon_b, 5));
			neon_b6 = vdupq_n_s8(vgetq_lane_s8(neon_b, 6));
			neon_b7 = vdupq_n_s8(vgetq_lane_s8(neon_b, 7));
			neon_b8 = vdupq_n_s8(vgetq_lane_s8(neon_b, 8));
			neon_b9 = vdupq_n_s8(vgetq_lane_s8(neon_b, 9));
			neon_b10 = vdupq_n_s8(vgetq_lane_s8(neon_b, 10));
			neon_b11 = vdupq_n_s8(vgetq_lane_s8(neon_b, 11));
			neon_b12 = vdupq_n_s8(vgetq_lane_s8(neon_b, 12));
			neon_b13 = vdupq_n_s8(vgetq_lane_s8(neon_b, 13));
			neon_b14 = vdupq_n_s8(vgetq_lane_s8(neon_b, 14));
			neon_b15 = vdupq_n_s8(vgetq_lane_s8(neon_b, 15));

			neon_c = vaddq_s8(vmulq_s8(neon_a0, neon_b0), neon_c);
			neon_c = vaddq_s8(vmulq_s8(neon_a1, neon_b1), neon_c);
			neon_c = vaddq_s8(vmulq_s8(neon_a2, neon_b2), neon_c);
			neon_c = vaddq_s8(vmulq_s8(neon_a3, neon_b3), neon_c);
			neon_c = vaddq_s8(vmulq_s8(neon_a4, neon_b4), neon_c);
			neon_c = vaddq_s8(vmulq_s8(neon_a5, neon_b5), neon_c);
			neon_c = vaddq_s8(vmulq_s8(neon_a6, neon_b6), neon_c);
			neon_c = vaddq_s8(vmulq_s8(neon_a7, neon_b7), neon_c);
			neon_c = vaddq_s8(vmulq_s8(neon_a8, neon_b8), neon_c);
			neon_c = vaddq_s8(vmulq_s8(neon_a9, neon_b9), neon_c);
			neon_c = vaddq_s8(vmulq_s8(neon_a10, neon_b10), neon_c);
			neon_c = vaddq_s8(vmulq_s8(neon_a11, neon_b11), neon_c);
			neon_c = vaddq_s8(vmulq_s8(neon_a12, neon_b12), neon_c);
			neon_c = vaddq_s8(vmulq_s8(neon_a13, neon_b13), neon_c);
			neon_c = vaddq_s8(vmulq_s8(neon_a14, neon_b14), neon_c);
			neon_c = vaddq_s8(vmulq_s8(neon_a15, neon_b15), neon_c);

		}

		vst1q_s8(C + i, neon_c);
	}
}

/* u32x4 mv mul */
void mw_neon_mv_mul_u32x4(unsigned int * A, int Row, int T, unsigned int * B, unsigned int * C)
{
	int i = 0;
	int k = 0;

	uint32x4_t neon_b, neon_c;
	uint32x4_t neon_a0, neon_a1, neon_a2, neon_a3;
	uint32x4_t neon_b0, neon_b1, neon_b2, neon_b3;

	for (i = 0; i < Row; i+=4)
	{
		neon_c = vmovq_n_u32(0);

		for (k = 0; k < T; k+=4)
		{
			int j = k * T + i;

			neon_a0 = vld1q_u32(A + j);
			j+=Row;
			neon_a1 = vld1q_u32(A + j);
			j+=Row;
			neon_a2 = vld1q_u32(A + j);
			j+=Row;
			neon_a3 = vld1q_u32(A + j);

			neon_b = vld1q_u32(B + k);
			neon_b0 = vdupq_n_u32(vgetq_lane_u32(neon_b, 0));
			neon_b1 = vdupq_n_u32(vgetq_lane_u32(neon_b, 1));
			neon_b2 = vdupq_n_u32(vgetq_lane_u32(neon_b, 2));
			neon_b3 = vdupq_n_u32(vgetq_lane_u32(neon_b, 3));

			neon_c = vaddq_u32(vmulq_u32(neon_a0, neon_b0), neon_c);
			neon_c = vaddq_u32(vmulq_u32(neon_a1, neon_b1), neon_c);
			neon_c = vaddq_u32(vmulq_u32(neon_a2, neon_b2), neon_c);
			neon_c = vaddq_u32(vmulq_u32(neon_a3, neon_b3), neon_c);

		}

		vst1q_u32(C + i, neon_c);
	}
}

/* u16x8 mv mul */
void mw_neon_mv_mul_u16x8(unsigned short * A, int Row, int T, unsigned short * B, unsigned short * C)
{
	int i = 0;
	int k = 0;

	uint16x8_t neon_b, neon_c;
	uint16x8_t neon_a0, neon_a1, neon_a2, neon_a3, neon_a4, neon_a5, neon_a6, neon_a7;
	uint16x8_t neon_b0, neon_b1, neon_b2, neon_b3, neon_b4, neon_b5, neon_b6, neon_b7;

	for (i = 0; i < Row; i+=8)
	{
		neon_c = vmovq_n_u16(0);

		for (k = 0; k < T; k+=8)
		{
			int j = k * T + i;

			neon_a0 = vld1q_u16(A + j);
			j+=Row;
			neon_a1 = vld1q_u16(A + j);
			j+=Row;
			neon_a2 = vld1q_u16(A + j);
			j+=Row;
			neon_a3 = vld1q_u16(A + j);
			j+=Row;
			neon_a4 = vld1q_u16(A + j);
			j+=Row;
			neon_a5 = vld1q_u16(A + j);
			j+=Row;
			neon_a6 = vld1q_u16(A + j);
			j+=Row;
			neon_a7 = vld1q_u16(A + j);

			neon_b = vld1q_u16(B + k);
			neon_b0 = vdupq_n_u16(vgetq_lane_u16(neon_b, 0));
			neon_b1 = vdupq_n_u16(vgetq_lane_u16(neon_b, 1));
			neon_b2 = vdupq_n_u16(vgetq_lane_u16(neon_b, 2));
			neon_b3 = vdupq_n_u16(vgetq_lane_u16(neon_b, 3));
			neon_b4 = vdupq_n_u16(vgetq_lane_u16(neon_b, 4));
			neon_b5 = vdupq_n_u16(vgetq_lane_u16(neon_b, 5));
			neon_b6 = vdupq_n_u16(vgetq_lane_u16(neon_b, 6));
			neon_b7 = vdupq_n_u16(vgetq_lane_u16(neon_b, 7));

			neon_c = vaddq_u16(vmulq_u16(neon_a0, neon_b0), neon_c);
			neon_c = vaddq_u16(vmulq_u16(neon_a1, neon_b1), neon_c);
			neon_c = vaddq_u16(vmulq_u16(neon_a2, neon_b2), neon_c);
			neon_c = vaddq_u16(vmulq_u16(neon_a3, neon_b3), neon_c);
			neon_c = vaddq_u16(vmulq_u16(neon_a4, neon_b4), neon_c);
			neon_c = vaddq_u16(vmulq_u16(neon_a5, neon_b5), neon_c);
			neon_c = vaddq_u16(vmulq_u16(neon_a6, neon_b6), neon_c);
			neon_c = vaddq_u16(vmulq_u16(neon_a7, neon_b7), neon_c);

		}

		vst1q_u16(C + i, neon_c);
	}
}

/* u8x16 mv mul */
void mw_neon_mv_mul_u8x16(unsigned char * A, int Row, int T, unsigned char * B, unsigned char * C)
{
	int i = 0;
	int k = 0;

	uint8x16_t neon_b, neon_c;
	uint8x16_t neon_a0, neon_a1, neon_a2, neon_a3, neon_a4, neon_a5, neon_a6, neon_a7;
	uint8x16_t neon_a8, neon_a9, neon_a10, neon_a11, neon_a12, neon_a13, neon_a14, neon_a15;
	uint8x16_t neon_b0, neon_b1, neon_b2, neon_b3, neon_b4, neon_b5, neon_b6, neon_b7;
	uint8x16_t neon_b8, neon_b9, neon_b10, neon_b11, neon_b12, neon_b13, neon_b14, neon_b15;

	for (i = 0; i < Row; i+=16)
	{
		neon_c = vmovq_n_u8(0);

		for (k = 0; k < T; k+=16)
		{
			int j = k * T + i;

			neon_a0 = vld1q_u8(A + j);
			j+=Row;
			neon_a1 = vld1q_u8(A + j);
			j+=Row;
			neon_a2 = vld1q_u8(A + j);
			j+=Row;
			neon_a3 = vld1q_u8(A + j);
			j+=Row;
			neon_a4 = vld1q_u8(A + j);
			j+=Row;
			neon_a5 = vld1q_u8(A + j);
			j+=Row;
			neon_a6 = vld1q_u8(A + j);
			j+=Row;
			neon_a7 = vld1q_u8(A + j);
			j+=Row;
			neon_a8 = vld1q_u8(A + j);
			j+=Row;
			neon_a9 = vld1q_u8(A + j);
			j+=Row;
			neon_a10 = vld1q_u8(A + j);
			j+=Row;
			neon_a11 = vld1q_u8(A + j);
			j+=Row;
			neon_a12 = vld1q_u8(A + j);
			j+=Row;
			neon_a13 = vld1q_u8(A + j);
			j+=Row;
			neon_a14 = vld1q_u8(A + j);
			j+=Row;
			neon_a15 = vld1q_u8(A + j);

			neon_b = vld1q_u8(B + k);
			neon_b0 = vdupq_n_u8(vgetq_lane_u8(neon_b, 0));
			neon_b1 = vdupq_n_u8(vgetq_lane_u8(neon_b, 1));
			neon_b2 = vdupq_n_u8(vgetq_lane_u8(neon_b, 2));
			neon_b3 = vdupq_n_u8(vgetq_lane_u8(neon_b, 3));
			neon_b4 = vdupq_n_u8(vgetq_lane_u8(neon_b, 4));
			neon_b5 = vdupq_n_u8(vgetq_lane_u8(neon_b, 5));
			neon_b6 = vdupq_n_u8(vgetq_lane_u8(neon_b, 6));
			neon_b7 = vdupq_n_u8(vgetq_lane_u8(neon_b, 7));
			neon_b8 = vdupq_n_u8(vgetq_lane_u8(neon_b, 8));
			neon_b9 = vdupq_n_u8(vgetq_lane_u8(neon_b, 9));
			neon_b10 = vdupq_n_u8(vgetq_lane_u8(neon_b, 10));
			neon_b11 = vdupq_n_u8(vgetq_lane_u8(neon_b, 11));
			neon_b12 = vdupq_n_u8(vgetq_lane_u8(neon_b, 12));
			neon_b13 = vdupq_n_u8(vgetq_lane_u8(neon_b, 13));
			neon_b14 = vdupq_n_u8(vgetq_lane_u8(neon_b, 14));
			neon_b15 = vdupq_n_u8(vgetq_lane_u8(neon_b, 15));

			neon_c = vaddq_u8(vmulq_u8(neon_a0, neon_b0), neon_c);
			neon_c = vaddq_u8(vmulq_u8(neon_a1, neon_b1), neon_c);
			neon_c = vaddq_u8(vmulq_u8(neon_a2, neon_b2), neon_c);
			neon_c = vaddq_u8(vmulq_u8(neon_a3, neon_b3), neon_c);
			neon_c = vaddq_u8(vmulq_u8(neon_a4, neon_b4), neon_c);
			neon_c = vaddq_u8(vmulq_u8(neon_a5, neon_b5), neon_c);
			neon_c = vaddq_u8(vmulq_u8(neon_a6, neon_b6), neon_c);
			neon_c = vaddq_u8(vmulq_u8(neon_a7, neon_b7), neon_c);
			neon_c = vaddq_u8(vmulq_u8(neon_a8, neon_b8), neon_c);
			neon_c = vaddq_u8(vmulq_u8(neon_a9, neon_b9), neon_c);
			neon_c = vaddq_u8(vmulq_u8(neon_a10, neon_b10), neon_c);
			neon_c = vaddq_u8(vmulq_u8(neon_a11, neon_b11), neon_c);
			neon_c = vaddq_u8(vmulq_u8(neon_a12, neon_b12), neon_c);
			neon_c = vaddq_u8(vmulq_u8(neon_a13, neon_b13), neon_c);
			neon_c = vaddq_u8(vmulq_u8(neon_a14, neon_b14), neon_c);
			neon_c = vaddq_u8(vmulq_u8(neon_a15, neon_b15), neon_c);

		}

		vst1q_u8(C + i, neon_c);
	}
}

/*
 * Matrix * Matrix
 */

/* f32x4 mm mul */
void mw_neon_mm_mul_f32x4(float * A, int Row, int T, float * B, int Col, float * C)
{
	int i, k, j;

	float32x4_t neon_b, neon_c;
	float32x4_t neon_a0, neon_a1, neon_a2, neon_a3;
	float32x4_t neon_b0, neon_b1, neon_b2, neon_b3;

	for (i = 0; i < Row; i+=4)
	{

		for (k = 0; k < Col; k+=1)
		{
			neon_c = vmovq_n_f32(0);

			for (j = 0; j < T; j+=4)
			{

				int j_T = j * T + i;
				int k_Row = k * Row;

				neon_a0 = vld1q_f32(A + j_T);
				j_T+=Row;
				neon_a1 = vld1q_f32(A + j_T);
				j_T+=Row;
				neon_a2 = vld1q_f32(A + j_T);
				j_T+=Row;
				neon_a3 = vld1q_f32(A + j_T);

				neon_b = vld1q_f32(B + k_Row + j);
				neon_b0 = vdupq_n_f32(vgetq_lane_f32(neon_b, 0));
				neon_b1 = vdupq_n_f32(vgetq_lane_f32(neon_b, 1));
				neon_b2 = vdupq_n_f32(vgetq_lane_f32(neon_b, 2));
				neon_b3 = vdupq_n_f32(vgetq_lane_f32(neon_b, 3));

				neon_c = vaddq_f32(vmulq_f32(neon_a0, neon_b0), neon_c);
				neon_c = vaddq_f32(vmulq_f32(neon_a1, neon_b1), neon_c);
				neon_c = vaddq_f32(vmulq_f32(neon_a2, neon_b2), neon_c);
				neon_c = vaddq_f32(vmulq_f32(neon_a3, neon_b3), neon_c);

				vst1q_lane_f32(C + k_Row + i, neon_c, 0);
				vst1q_lane_f32(C + k_Row + i + 1, neon_c, 1);
				vst1q_lane_f32(C + k_Row + i + 2, neon_c, 2);
				vst1q_lane_f32(C + k_Row + i + 3, neon_c, 3);

			}
		}
	}
}

/* s32x4 mm mul */
void mw_neon_mm_mul_s32x4(int * A, int Row, int T, int * B, int Col, int * C)
{
	int i, k, j;

	int32x4_t neon_b, neon_c;
	int32x4_t neon_a0, neon_a1, neon_a2, neon_a3;
	int32x4_t neon_b0, neon_b1, neon_b2, neon_b3;

	for (i = 0; i < Row; i+=4)
	{

		for (k = 0; k < Col; k+=1)
		{
			neon_c = vmovq_n_s32(0);

			for (j = 0; j < T; j+=4)
			{

				int j_T = j * T + i;
				int k_Row = k * Row;

				neon_a0 = vld1q_s32(A + j_T);
				j_T+=Row;
				neon_a1 = vld1q_s32(A + j_T);
				j_T+=Row;
				neon_a2 = vld1q_s32(A + j_T);
				j_T+=Row;
				neon_a3 = vld1q_s32(A + j_T);

				neon_b = vld1q_s32(B + k_Row + j);
				neon_b0 = vdupq_n_s32(vgetq_lane_s32(neon_b, 0));
				neon_b1 = vdupq_n_s32(vgetq_lane_s32(neon_b, 1));
				neon_b2 = vdupq_n_s32(vgetq_lane_s32(neon_b, 2));
				neon_b3 = vdupq_n_s32(vgetq_lane_s32(neon_b, 3));

				neon_c = vaddq_s32(vmulq_s32(neon_a0, neon_b0), neon_c);
				neon_c = vaddq_s32(vmulq_s32(neon_a1, neon_b1), neon_c);
				neon_c = vaddq_s32(vmulq_s32(neon_a2, neon_b2), neon_c);
				neon_c = vaddq_s32(vmulq_s32(neon_a3, neon_b3), neon_c);

				vst1q_lane_s32(C + k_Row + i, neon_c, 0);
				vst1q_lane_s32(C + k_Row + i + 1, neon_c, 1);
				vst1q_lane_s32(C + k_Row + i + 2, neon_c, 2);
				vst1q_lane_s32(C + k_Row + i + 3, neon_c, 3);

			}
		}
	}
}

/* s16x8 mm mul */
void mw_neon_mm_mul_s16x8(short * A, int Row, int T, short * B, int Col, short * C)
{
	int i, k, j;

	int16x8_t neon_b, neon_c;
	int16x8_t neon_a0, neon_a1, neon_a2, neon_a3, neon_a4, neon_a5, neon_a6, neon_a7;
	int16x8_t neon_b0, neon_b1, neon_b2, neon_b3, neon_b4, neon_b5, neon_b6, neon_b7;

	for (i = 0; i < Row; i+=8)
	{

		for (k = 0; k < Col; k+=1)
		{
			neon_c = vmovq_n_s16(0);

			for (j = 0; j < T; j+=8)
			{

				int j_T = j * T + i;
				int k_Row = k * Row;

				neon_a0 = vld1q_s16(A + j_T);
				j_T+=Row;
				neon_a1 = vld1q_s16(A + j_T);
				j_T+=Row;
				neon_a2 = vld1q_s16(A + j_T);
				j_T+=Row;
				neon_a3 = vld1q_s16(A + j_T);
				j_T+=Row;
				neon_a4 = vld1q_s16(A + j_T);
				j_T+=Row;
				neon_a5 = vld1q_s16(A + j_T);
				j_T+=Row;
				neon_a6 = vld1q_s16(A + j_T);
				j_T+=Row;
				neon_a7 = vld1q_s16(A + j_T);

				neon_b = vld1q_s16(B + k_Row + j);
				neon_b0 = vdupq_n_s16(vgetq_lane_s16(neon_b, 0));
				neon_b1 = vdupq_n_s16(vgetq_lane_s16(neon_b, 1));
				neon_b2 = vdupq_n_s16(vgetq_lane_s16(neon_b, 2));
				neon_b3 = vdupq_n_s16(vgetq_lane_s16(neon_b, 3));
				neon_b4 = vdupq_n_s16(vgetq_lane_s16(neon_b, 4));
				neon_b5 = vdupq_n_s16(vgetq_lane_s16(neon_b, 5));
				neon_b6 = vdupq_n_s16(vgetq_lane_s16(neon_b, 6));
				neon_b7 = vdupq_n_s16(vgetq_lane_s16(neon_b, 7));

				neon_c = vaddq_s16(vmulq_s16(neon_a0, neon_b0), neon_c);
				neon_c = vaddq_s16(vmulq_s16(neon_a1, neon_b1), neon_c);
				neon_c = vaddq_s16(vmulq_s16(neon_a2, neon_b2), neon_c);
				neon_c = vaddq_s16(vmulq_s16(neon_a3, neon_b3), neon_c);
				neon_c = vaddq_s16(vmulq_s16(neon_a4, neon_b4), neon_c);
				neon_c = vaddq_s16(vmulq_s16(neon_a5, neon_b5), neon_c);
				neon_c = vaddq_s16(vmulq_s16(neon_a6, neon_b6), neon_c);
				neon_c = vaddq_s16(vmulq_s16(neon_a7, neon_b7), neon_c);

				vst1q_lane_s16(C + k_Row + i, neon_c, 0);
				vst1q_lane_s16(C + k_Row + i + 1, neon_c, 1);
				vst1q_lane_s16(C + k_Row + i + 2, neon_c, 2);
				vst1q_lane_s16(C + k_Row + i + 3, neon_c, 3);
				vst1q_lane_s16(C + k_Row + i + 4, neon_c, 4);
				vst1q_lane_s16(C + k_Row + i + 5, neon_c, 5);
				vst1q_lane_s16(C + k_Row + i + 6, neon_c, 6);
				vst1q_lane_s16(C + k_Row + i + 7, neon_c, 7);

			}
		}
	}
}

/* u32x4 mm mul */
void mw_neon_mm_mul_u32x4(unsigned int * A, int Row, int T, unsigned int * B, int Col, unsigned int * C)
{
	int i, k, j;

	uint32x4_t neon_b, neon_c;
	uint32x4_t neon_a0, neon_a1, neon_a2, neon_a3;
	uint32x4_t neon_b0, neon_b1, neon_b2, neon_b3;

	for (i = 0; i < Row; i+=4)
	{

		for (k = 0; k < Col; k+=1)
		{
			neon_c = vmovq_n_u32(0);

			for (j = 0; j < T; j+=4)
			{

				int j_T = j * T + i;
				int k_Row = k * Row;

				neon_a0 = vld1q_u32(A + j_T);
				j_T+=Row;
				neon_a1 = vld1q_u32(A + j_T);
				j_T+=Row;
				neon_a2 = vld1q_u32(A + j_T);
				j_T+=Row;
				neon_a3 = vld1q_u32(A + j_T);

				neon_b = vld1q_u32(B + k_Row + j);
				neon_b0 = vdupq_n_u32(vgetq_lane_u32(neon_b, 0));
				neon_b1 = vdupq_n_u32(vgetq_lane_u32(neon_b, 1));
				neon_b2 = vdupq_n_u32(vgetq_lane_u32(neon_b, 2));
				neon_b3 = vdupq_n_u32(vgetq_lane_u32(neon_b, 3));

				neon_c = vaddq_u32(vmulq_u32(neon_a0, neon_b0), neon_c);
				neon_c = vaddq_u32(vmulq_u32(neon_a1, neon_b1), neon_c);
				neon_c = vaddq_u32(vmulq_u32(neon_a2, neon_b2), neon_c);
				neon_c = vaddq_u32(vmulq_u32(neon_a3, neon_b3), neon_c);

				vst1q_lane_u32(C + k_Row + i, neon_c, 0);
				vst1q_lane_u32(C + k_Row + i + 1, neon_c, 1);
				vst1q_lane_u32(C + k_Row + i + 2, neon_c, 2);
				vst1q_lane_u32(C + k_Row + i + 3, neon_c, 3);

			}
		}
	}
}

/* u16x8 mm mul */
void mw_neon_mm_mul_u16x8(unsigned short * A, int Row, int T, unsigned short * B, int Col, unsigned short * C)
{
	int i, k, j;

	uint16x8_t neon_b, neon_c;
	uint16x8_t neon_a0, neon_a1, neon_a2, neon_a3, neon_a4, neon_a5, neon_a6, neon_a7;
	uint16x8_t neon_b0, neon_b1, neon_b2, neon_b3, neon_b4, neon_b5, neon_b6, neon_b7;

	for (i = 0; i < Row; i+=8)
	{

		for (k = 0; k < Col; k+=1)
		{
			neon_c = vmovq_n_u16(0);

			for (j = 0; j < T; j+=8)
			{

				int j_T = j * T + i;
				int k_Row = k * Row;

				neon_a0 = vld1q_u16(A + j_T);
				j_T+=Row;
				neon_a1 = vld1q_u16(A + j_T);
				j_T+=Row;
				neon_a2 = vld1q_u16(A + j_T);
				j_T+=Row;
				neon_a3 = vld1q_u16(A + j_T);
				j_T+=Row;
				neon_a4 = vld1q_u16(A + j_T);
				j_T+=Row;
				neon_a5 = vld1q_u16(A + j_T);
				j_T+=Row;
				neon_a6 = vld1q_u16(A + j_T);
				j_T+=Row;
				neon_a7 = vld1q_u16(A + j_T);

				neon_b = vld1q_u16(B + k_Row + j);
				neon_b0 = vdupq_n_u16(vgetq_lane_u16(neon_b, 0));
				neon_b1 = vdupq_n_u16(vgetq_lane_u16(neon_b, 1));
				neon_b2 = vdupq_n_u16(vgetq_lane_u16(neon_b, 2));
				neon_b3 = vdupq_n_u16(vgetq_lane_u16(neon_b, 3));
				neon_b4 = vdupq_n_u16(vgetq_lane_u16(neon_b, 4));
				neon_b5 = vdupq_n_u16(vgetq_lane_u16(neon_b, 5));
				neon_b6 = vdupq_n_u16(vgetq_lane_u16(neon_b, 6));
				neon_b7 = vdupq_n_u16(vgetq_lane_u16(neon_b, 7));

				neon_c = vaddq_u16(vmulq_u16(neon_a0, neon_b0), neon_c);
				neon_c = vaddq_u16(vmulq_u16(neon_a1, neon_b1), neon_c);
				neon_c = vaddq_u16(vmulq_u16(neon_a2, neon_b2), neon_c);
				neon_c = vaddq_u16(vmulq_u16(neon_a3, neon_b3), neon_c);
				neon_c = vaddq_u16(vmulq_u16(neon_a4, neon_b4), neon_c);
				neon_c = vaddq_u16(vmulq_u16(neon_a5, neon_b5), neon_c);
				neon_c = vaddq_u16(vmulq_u16(neon_a6, neon_b6), neon_c);
				neon_c = vaddq_u16(vmulq_u16(neon_a7, neon_b7), neon_c);

				vst1q_lane_u16(C + k_Row + i, neon_c, 0);
				vst1q_lane_u16(C + k_Row + i + 1, neon_c, 1);
				vst1q_lane_u16(C + k_Row + i + 2, neon_c, 2);
				vst1q_lane_u16(C + k_Row + i + 3, neon_c, 3);
				vst1q_lane_u16(C + k_Row + i + 4, neon_c, 4);
				vst1q_lane_u16(C + k_Row + i + 5, neon_c, 5);
				vst1q_lane_u16(C + k_Row + i + 6, neon_c, 6);
				vst1q_lane_u16(C + k_Row + i + 7, neon_c, 7);

			}
		}
	}
}

