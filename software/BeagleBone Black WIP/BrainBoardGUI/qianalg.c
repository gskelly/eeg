/*
 * qianalg.c
 *
 * Code generation for function 'qianalg'
 *
 * C source code generated on: Fri Jun 21 17:20:30 2013
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "qianalg.h"
#include <stdio.h>
/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
static real32_T rt_hypotf_snf(real32_T u0, real32_T u1);
static real32_T rt_powf_snf(real32_T u0, real32_T u1);

/* Function Definitions */
static real32_T rt_hypotf_snf(real32_T u0, real32_T u1)
{
  real32_T y;
  real32_T a;
  a = (real32_T)fabs(u0);
  y = (real32_T)fabs(u1);
  if (a < y) {
    a /= y;
    y *= (real32_T)sqrt(a * a + 1.0F);
  } else if (a > y) {
    y /= a;
    y = a * (real32_T)sqrt(y * y + 1.0F);
  } else if (rtIsNaNF(y)) {
  } else {
    y = a * 1.41421354F;
  }

  return y;
}

static real32_T rt_powf_snf(real32_T u0, real32_T u1)
{
  real32_T y;
  real32_T f0;
  real32_T f1;
  if (rtIsNaNF(u0) || rtIsNaNF(u1)) {
    y = ((real32_T)rtNaN);
  } else {
    f0 = (real32_T)fabs(u0);
    f1 = (real32_T)fabs(u1);
    if (rtIsInfF(u1)) {
      if (f0 == 1.0F) {
        y = ((real32_T)rtNaN);
      } else if (f0 > 1.0F) {
        if (u1 > 0.0F) {
          y = ((real32_T)rtInf);
        } else {
          y = 0.0F;
        }
      } else if (u1 > 0.0F) {
        y = 0.0F;
      } else {
        y = ((real32_T)rtInf);
      }
    } else if (f1 == 0.0F) {
      y = 1.0F;
    } else if (f1 == 1.0F) {
      if (u1 > 0.0F) {
        y = u0;
      } else {
        y = 1.0F / u0;
      }
    } else if (u1 == 2.0F) {
      y = u0 * u0;
    } else if ((u1 == 0.5F) && (u0 >= 0.0F)) {
      y = (real32_T)sqrt(u0);
    } else if ((u0 < 0.0F) && (u1 > (real32_T)floor(u1))) {
      y = ((real32_T)rtNaN);
    } else {
      y = (real32_T)pow(u0, u1);
    }
  }

  return y;
}

/*
 * function [tf, Pxx, s1] = qianalg(erd,Pref,thresh)
 */
void qianalg(const real32_T erd[1152], real32_T Pref, real32_T thresh, boolean_T
             *tf, real32_T Pxx[64], real32_T s1[64])
{
  __attribute__((aligned(16))) real32_T fv0[64];
  __attribute__((aligned(16))) real32_T C3_win[64];
  int32_T i;
  static const real32_T fv1[64] = { 0.08F, 0.0822858438F, 0.0891206563F,
    0.100436509F, 0.116120942F, 0.136018082F, 0.15993017F, 0.187619552F,
    0.218811065F, 0.25319469F, 0.290428728F, 0.330143094F, 0.371943116F,
    0.41541338F, 0.46012184F, 0.505624175F, 0.551468134F, 0.597198129F,
    0.642359614F, 0.686503887F, 0.729192078F, 0.77F, 0.808522105F, 0.844375491F,
    0.877203882F, 0.906680942F, 0.932513833F, 0.95444566F, 0.972258627F,
    0.98577553F, 0.994862199F, 0.999428213F, 0.999428213F, 0.994862199F,
    0.98577553F, 0.972258627F, 0.95444566F, 0.932513833F, 0.906680942F,
    0.877203882F, 0.844375491F, 0.808522105F, 0.77F, 0.729192078F, 0.686503887F,
    0.642359614F, 0.597198129F, 0.551468134F, 0.505624175F, 0.46012184F,
    0.41541338F, 0.371943116F, 0.330143094F, 0.290428728F, 0.25319469F,
    0.218811065F, 0.187619552F, 0.15993017F, 0.136018082F, 0.116120942F,
    0.100436509F, 0.0891206563F, 0.0822858438F, 0.08F };

  creal32_T y[64];
  int32_T ix;
  uint32_T ju;
  int32_T iy;
  uint32_T n;
  boolean_T tst;
  real32_T temp_re;
  real32_T temp_im;
  int32_T iDelta;
  int32_T iDelta2;
  int32_T k;
  int32_T iheight;
  int32_T ihi;
  static const real32_T fv2[33] = { 0.0F, -0.0980171412F, -0.195090324F,
    -0.290284663F, -0.382683456F, -0.471396744F, -0.555570245F, -0.634393334F,
    -0.707106769F, -0.773010433F, -0.831469595F, -0.881921232F, -0.923879504F,
    -0.956940353F, -0.980785251F, -0.99518472F, -1.0F, -0.99518472F,
    -0.980785251F, -0.956940353F, -0.923879504F, -0.881921232F, -0.831469595F,
    -0.773010433F, -0.707106769F, -0.634393334F, -0.555570245F, -0.471396744F,
    -0.382683456F, -0.290284663F, -0.195090324F, -0.0980171412F, -0.0F };

  static const real32_T fv3[33] = { 1.0F, 0.99518472F, 0.980785251F,
    0.956940353F, 0.923879504F, 0.881921232F, 0.831469595F, 0.773010433F,
    0.707106769F, 0.634393334F, 0.555570245F, 0.471396744F, 0.382683456F,
    0.290284663F, 0.195090324F, 0.0980171412F, 0.0F, -0.0980171412F,
    -0.195090324F, -0.290284663F, -0.382683456F, -0.471396744F, -0.555570245F,
    -0.634393334F, -0.707106769F, -0.773010433F, -0.831469595F, -0.881921232F,
    -0.923879504F, -0.956940353F, -0.980785251F, -0.99518472F, -1.0F };

  /* % Initialize outputs */
  /*  Note: format for erd is with columns as channels, and with channels */
  /*  selected columnwise along the 10-20 "grid," i.e., */
  /*        erd = [F3,C3,P3,Fz,Cz,Pz,F4,C4,P4] */
  /* 'qianalg:7' tf = false; */
  *tf = FALSE;

  /* 'qianalg:8' fs = 160; */
  /*  This may be different in other cases; should be an input */
  /* % Create Laplacian weight matrix if needed */
  /*  Not necessary for this algorithm */
  /*  map1020 = {'F3','Fz','F4';... */
  /*             'C3','Cz','C4';... */
  /*             'P3','Pz','P4'}'; */
  /*   */
  /*  Use the code below if montage is fixed 9x9 as seen in map1020 */
  /*  L = [1 -1/3 0 -1/3 0 0 0 0 0; */
  /*      -1/2 1 -1/2 0 -1/4 0 0 0 0; */
  /*      0 -1/3 1 0 0 -1/3 0 0 0; */
  /*      -1/2 0 0 1 -1/4 0 -1/2 0 0; */
  /*      0 -1/3 0 -1/3 1 -1/3 0 -1/3 0; */
  /*      0 0 -1/2 0 -1/4 1 0 0 -1/2; */
  /*      0 0 0 -1/3 0 0 1 -1/3 0; */
  /*      0 0 0 0 -1/4 0 -1/2 1 -1/2; */
  /*      0 0 0 0 0 -1/3 0 -1/3 1]; */
  /*  Otherwise use the following code: */
  /*  % Generate adjacency matrix */
  /*  % See first answer,  */
  /*  % stackoverflow.com/questions/3277541/construct-adjacency-matrix-in-matlab */
  /*  % Credit to gnovice for code lines 17-27 */
  /*  % Note that in this case indexing is column-major, so meaning of the two */
  /*  % diagonal matrices is reversed from gnovice's code */
  /*  [m,n] = size(map1020); */
  /*  % Vertical 4-neighbor connectivity vector */
  /*  diag1 = repmat([ones(n-1,1); 0],m,1); */
  /*  % Remove last entry (# connections = # elements - 1) */
  /*  diag1 = diag1(1:end-1); */
  /*  % Horizontal 4-neighbor connectivity vector */
  /*  diag2 = ones(n*(m-1),1); */
  /*  % Assemble connectivity vectors into diagonal matrix */
  /*  adj = diag(diag1,1) + diag(diag2,n); */
  /*  % Make adjacency matrix symmetric */
  /*  adj = adj + adj.'; */
  /*   */
  /*  % Find number of neighbors for each electrode */
  /*  s = repmat(sum(adj),m*n,1); */
  /*  % Generate channel weights for (Hjorth 1975) Laplacian referencing: */
  /*  % Channel = channel - mean(neighboring channels on 4-connected graph) */
  /*  L = eye(size(adj)) - (adj./s); */
  /*  erd_lap = erd*L; */
  /* % Generate re-referenced channel */
  /*  paper uses only channel C3(?) */
  /* 'qianalg:59' n = 64; */
  /* 'qianalg:60' F3 = erd(1:n,1); */
  /* 'qianalg:60' C3 = erd(1:n,2); */
  /* 'qianalg:60' P3 = erd(1:n,3); */
  /* 'qianalg:60' Cz = erd(1:n,5); */
  /* 'qianalg:61' s1 = F3+Cz; */
  mw_neon_mm_add_f32x4(*(real32_T (*)[64])&erd[0], 64, 1, *(real32_T (*)[64])&
                       erd[512], *(real32_T (*)[64])&s1[0]);
printf("%f+%f = %f\n", erd[0], erd[512], s1[0]);
  /* 'qianalg:62' s2 = s1+P3; */
  /* 'qianalg:63' C3_ = C3 - s2./3; */
  /* % Do Power Spectrum Decomposition */
  /*  The below code is inelegant, but needed to ensure compatibility with */
  /*  MATLAB Coder. It's equivalent to the following: */
  /*  */
  /*  h = spectrum.welch('Hamming',64,0.5); */
  /*  hpsd = psd(h,C3_,'NFFT',64,'Fs',fs); */
  /*  Pxx = hpsd.Data; */
  /*  f = hpsd.Frequencies; */
  /*  */
  /* 'qianalg:74' L = length(C3_); */
  /* 'qianalg:75' w = hamming(L); */
  /* 'qianalg:76' C3_win = w.*C3_; */
  mw_neon_mm_add_f32x4(s1, 64, 1, *(real32_T (*)[64])&erd[256], *(real32_T (*)
    [64])&fv0[0]);
  for (i = 0; i < 64; i++) {
    C3_win[i] = fv0[i] / 3.0F;
  }

  mw_neon_mm_sub_f32x4(*(real32_T (*)[64])&erd[128], 64, 1, C3_win, *(real32_T (*)
    [64])&fv0[0]);
  for (i = 0; i < 64; i++) {
    C3_win[i] = fv1[i] * fv0[i];
  }

  /* 'qianalg:77' C3_magfft = abs(fft(C3_win,64)).^2; */
  ix = 0;
  ju = 0U;
  iy = 0;
  for (i = 0; i < 63; i++) {
    y[iy].re = C3_win[ix];
    y[iy].im = 0.0F;
    n = 64U;
    tst = TRUE;
    while (tst) {
      n >>= 1U;
      ju ^= n;
      tst = ((int32_T)(ju & n) == 0);
    }

    iy = (int32_T)ju;
    ix++;
  }

  y[iy].re = C3_win[ix];
  y[iy].im = 0.0F;
  for (i = 0; i < 64; i += 2) {
    temp_re = y[i + 1].re;
    temp_im = y[i + 1].im;
    y[i + 1].re = y[i].re - y[i + 1].re;
    y[i + 1].im = y[i].im - y[i + 1].im;
    y[i].re += temp_re;
    y[i].im += temp_im;
  }

  iDelta = 2;
  iDelta2 = 4;
  k = 16;
  iheight = 61;
  while (k > 0) {
    for (i = 0; i < iheight; i += iDelta2) {
      ix = i + iDelta;
      temp_re = y[ix].re;
      temp_im = y[ix].im;
      y[i + iDelta].re = y[i].re - y[ix].re;
      y[i + iDelta].im = y[i].im - y[ix].im;
      y[i].re += temp_re;
      y[i].im += temp_im;
    }

    iy = 1;
    for (ix = k; ix < 32; ix += k) {
      i = iy;
      ihi = iy + iheight;
      while (i < ihi) {
        temp_re = fv3[ix] * y[i + iDelta].re - fv2[ix] * y[i + iDelta].im;
        temp_im = fv3[ix] * y[i + iDelta].im + fv2[ix] * y[i + iDelta].re;
        y[i + iDelta].re = y[i].re - temp_re;
        y[i + iDelta].im = y[i].im - temp_im;
        y[i].re += temp_re;
        y[i].im += temp_im;
        i += iDelta2;
      }

      iy++;
    }

    k /= 2;
    iDelta = iDelta2;
    iDelta2 <<= 1;
    iheight -= iDelta;
  }

  for (k = 0; k < 64; k++) {
    C3_win[k] = rt_powf_snf(rt_hypotf_snf((real32_T)fabs(y[k].re), (real32_T)
      fabs(y[k].im)), 2.0F);

    /* 'qianalg:78' b = ones(3,1)/3; */
    /* 'qianalg:79' C3_avgd = filter(b,1,C3_magfft); */
    Pxx[k] = 0.0F;
  }

  for (k = 0; k < 3; k++) {
    for (ix = k; ix + 1 < 65; ix++) {
      Pxx[ix] += 0.333333343F * C3_win[ix - k];
    }
  }

  /* 'qianalg:80' w_norm = 1./dot(w,w); */
  /* 'qianalg:81' ts = 1/fs; */
  /* 'qianalg:82' Pxx = C3_avgd.*w_norm.*ts; */
  for (ix = 0; ix < 64; ix++) {
    Pxx[ix] = Pxx[ix] * 0.0399319567F * 0.00625F;
  }

  /* % Extract relative power at desired PSD bin */
  /*  plot(f,Pxx) */
  /* 'qianalg:86' bin = 5; */
  /* 'qianalg:87' erdr = (Pref - Pxx(bin))/Pref; */
  /* % Threshold */
  /* 'qianalg:90' if erdr > thresh */
  if ((Pref - Pxx[4]) / Pref > thresh) {
    /* 'qianalg:91' tf = true; */
    *tf = TRUE;
  }
}

/* End of code generation (qianalg.c) */
