/*
 * qianalg.h
 *
 * Code generation for function 'qianalg'
 *
 * C source code generated on: Fri Jun 21 17:20:30 2013
 *
 */

#ifndef __QIANALG_H__
#define __QIANALG_H__
/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include "mw_neon.h"
#include "rt_nonfinite.h"

#include "rtwtypes.h"
#include "qianalg_types.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
extern void qianalg(const real32_T erd[1152], real32_T Pref, real32_T thresh, boolean_T *tf, real32_T Pxx[64], real32_T s1[64]);
#endif
/* End of code generation (qianalg.h) */
