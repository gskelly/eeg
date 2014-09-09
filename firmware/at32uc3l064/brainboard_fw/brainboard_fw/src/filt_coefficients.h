/*
 * filt_coefficients.h
 *
 * Created: 8/4/2014 3:17:25 PM
 *  Author: Graham Kelly
 */ 


#ifndef FILT_COEFF_H_
#define FILT_COEFF_H_

/* Note: since the ASF's DSP library automatically trims edge transients from FIR filter output,
 * the output vector will be (FRAME_SIZE - FILT_COEFF_LENGTH + 1) elements long. User must ensure
 * this is a useful length, and not zero or negative.
 */
#define FILT_COEFF_LENGTH		(64)

/* Coefficients are given as signed 16-bit [-32768,32767] representations of Q15 fractional numbers [-1,1) */

__attribute__((aligned(4))) const dsp16_t mu_h[FILT_COEFF_LENGTH] = 
						 {-332,	  -179,  -5,     180,   362,  527,  661,   752,   791,   772,   694,   557,
						   369,	   140,  -116,  -380,  -634, -859, -1034, -1144, -1177, -1127, -992,  -779, 
						  -501,   -177,   171,   516,   831,  1093, 1281,  1378,  1378,  1281,  1093,  831,
						   516,    171,  -177,  -501,  -779, -992, -1127, -1177, -1144, -1034, -859,  -634,
						  -380,	  -116,   140,   369,   557,  694,  772,   791,   752,   661,   527,   362,
						   180,   -5,    -179,  -332};

__attribute__((aligned(4))) const dsp16_t beta_h[FILT_COEFF_LENGTH] = 
							{13,    19,    156,   330,   409,  292, -27,   -440,  -773,  -869,  -674,  -271, 
							156,   417,   415,   206,  -23,  -53,   238,   779,   1312,  1490,  1050, -29, 
						   -1466, -2748, -3328, -2856, -1357, 743,  2754,  3976,  3976,  2754,  743,  -1357,
						   -2856, -3328, -2748, -1466, -29,   1050, 1490,  1312,  779,   238,  -53,	  -23,
						   	206,   415,   417,	 156,  -271, -674, -869,  -773,  -440,  -27,    292,   409,
							330,   156,   19,    13};


#endif /* FILT_COEFF_H_ */