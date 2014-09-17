/* Copyright 2010 The MathWorks, Inc. */

#include <arm_neon.h>

/* f32x4 */
void mw_neon_mm_mul_f32x4(float * A, int Row, int T, float * B, int Col, float * C);
void mw_neon_mv_mul_f32x4(float * A, int Row, int T, float * B, float * C);
void mw_neon_mm_add_f32x4(float * A, int Row, int Col, float * B, float * C);
void mw_neon_mm_sub_f32x4(float * A, int Row, int Col, float * B, float * C);

/* s32x4 */
void mw_neon_mm_mul_s32x4(int * A, int Row, int T, int * B, int Col, int * C);
void mw_neon_mv_mul_s32x4(int * A, int Row, int T, int * B, int * C);
void mw_neon_mm_add_s32x4(int * A, int Row, int Col, int * B, int * C);
void mw_neon_mm_sub_s32x4(int * A, int Row, int Col, int * B, int * C);
void mw_neon_mm_qadd_s32x4(int * A, int Row, int Col, int * B, int * C);
void mw_neon_mm_qsub_s32x4(int * A, int Row, int Col, int * B, int * C);

/* u32x4 */
void mw_neon_mm_mul_u32x4(unsigned int * A, int Row, int T, unsigned int * B, int Col, unsigned int * C);
void mw_neon_mv_mul_u32x4(unsigned int * A, int Row, int T, unsigned int * B, unsigned int * C);
void mw_neon_mm_add_u32x4(unsigned int * A, int Row, int Col, unsigned int * B, unsigned int * C);
void mw_neon_mm_sub_u32x4(unsigned int * A, int Row, int Col, unsigned int * B, unsigned int * C);
void mw_neon_mm_qadd_u32x4(unsigned int * A, int Row, int Col, unsigned int * B, unsigned int * C);
void mw_neon_mm_qsub_u32x4(unsigned int * A, int Row, int Col, unsigned int * B, unsigned int * C);

/* s16x8 */
void mw_neon_mm_mul_s16x8(short * A, int Row, int T, short * B, int Col, short * C);
void mw_neon_mv_mul_s16x8(short * A, int Row, int T, short * B, short * C);
void mw_neon_mm_add_s16x8(short * A, int Row, int Col, short * B, short * C);
void mw_neon_mm_sub_s16x8(short * A, int Row, int Col, short * B, short * C);
void mw_neon_mm_qadd_s16x8(short * A, int Row, int Col, short * B, short * C);
void mw_neon_mm_qsub_s16x8(short * A, int Row, int Col, short * B, short * C);

/* u16x8 */
void mw_neon_mm_mul_u16x8(unsigned short * A, int Row, int T, unsigned short * B, int Col, unsigned short * C);
void mw_neon_mv_mul_u16x8(unsigned short * A, int Row, int T, unsigned short * B, unsigned short * C);
void mw_neon_mm_add_u16x8(unsigned short * A, int Row, int Col, unsigned short * B, unsigned short * C);
void mw_neon_mm_sub_u16x8(unsigned short * A, int Row, int Col, unsigned short * B, unsigned short * C);
void mw_neon_mm_qadd_u16x8(unsigned short * A, int Row, int Col, unsigned short * B, unsigned short * C);
void mw_neon_mm_qsub_u16x8(unsigned short * A, int Row, int Col, unsigned short * B, unsigned short * C);

/* s8x16 */
void mw_neon_mm_mul_s8x16(signed char * A, int Row, int T, signed char * B, int Col, signed char * C);
void mw_neon_mv_mul_s8x16(signed char * A, int Row, int T, signed char * B, signed char * C);
void mw_neon_mm_add_s8x16(signed char * A, int Row, int Col, signed char * B, signed char * C);
void mw_neon_mm_sub_s8x16(signed char * A, int Row, int Col, signed char * B, signed char * C);
void mw_neon_mm_qadd_s8x16(signed char * A, int Row, int Col, signed char * B, signed char * C);
//void mw_neon_mm_qsub_s8x16(signed char * A, int Row, int Col, signed char * B, signed char * C);

/* u8x16 */
void mw_neon_mm_mul_u8x16(unsigned char * A, int Row, int T, unsigned char * B, int Col, unsigned char * C);
void mw_neon_mv_mul_u8x16(unsigned char * A, int Row, int T, unsigned char * B, unsigned char * C);
void mw_neon_mm_add_u8x16(unsigned char * A, int Row, int Col, unsigned char * B, unsigned char * C);
void mw_neon_mm_sub_u8x16(unsigned char * A, int Row, int Col, unsigned char * B, unsigned char * C);
void mw_neon_mm_qadd_u8x16(unsigned char * A, int Row, int Col, unsigned char * B, unsigned char * C);
//void mw_neon_mm_qsub_u8x16(unsigned char * A, int Row, int Col, unsigned char * B, unsigned char * C);
