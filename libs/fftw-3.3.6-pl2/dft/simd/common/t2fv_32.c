/*
 * Copyright (c) 2003, 2007-14 Matteo Frigo
 * Copyright (c) 2003, 2007-14 Massachusetts Institute of Technology
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

/* This file was automatically generated --- DO NOT EDIT */
/* Generated on Fri Jan 27 16:11:08 EST 2017 */

#include "codelet-dft.h"

#ifdef HAVE_FMA

/* Generated by: ../../../genfft/gen_twiddle_c.native -fma -reorder-insns -schedule-for-pipeline -simd -compact -variables 4 -pipeline-latency 8 -n 32 -name t2fv_32 -include t2f.h */

/*
 * This function contains 217 FP additions, 160 FP multiplications,
 * (or, 119 additions, 62 multiplications, 98 fused multiply/add),
 * 112 stack variables, 7 constants, and 64 memory accesses
 */
#include "t2f.h"

static void t2fv_32(R *ri, R *ii, const R *W, stride rs, INT mb, INT me, INT ms)
{
     DVK(KP831469612, +0.831469612302545237078788377617905756738560812);
     DVK(KP980785280, +0.980785280403230449126182236134239036973933731);
     DVK(KP668178637, +0.668178637919298919997757686523080761552472251);
     DVK(KP198912367, +0.198912367379658006911597622644676228597850501);
     DVK(KP923879532, +0.923879532511286756128183189396788286822416626);
     DVK(KP707106781, +0.707106781186547524400844362104849039284835938);
     DVK(KP414213562, +0.414213562373095048801688724209698078569671875);
     {
	  INT m;
	  R *x;
	  x = ri;
	  for (m = mb, W = W + (mb * ((TWVL / VL) * 62)); m < me; m = m + VL, x = x + (VL * ms), W = W + (TWVL * 62), MAKE_VOLATILE_STRIDE(32, rs)) {
	       V T26, T25, T1Z, T22, T1W, T2a, T2k, T2g;
	       {
		    V T4, T1z, T2o, T32, T2r, T3f, Tf, T1A, T34, T2L, T1D, TC, T33, T2O, T1C;
		    V Tr, T2C, T3a, T2F, T3b, T1r, T21, T1k, T20, TQ, TM, TS, TL, T2t, TJ;
		    V T10, T2u;
		    {
			 V Tt, T9, T2p, Te, T2q, TA, Tu, Tx;
			 {
			      V T1, T1x, T2, T1v;
			      T1 = LD(&(x[0]), ms, &(x[0]));
			      T1x = LD(&(x[WS(rs, 24)]), ms, &(x[0]));
			      T2 = LD(&(x[WS(rs, 16)]), ms, &(x[0]));
			      T1v = LD(&(x[WS(rs, 8)]), ms, &(x[0]));
			      {
				   V T5, Tc, T7, Ta, T2m, T2n;
				   T5 = LD(&(x[WS(rs, 4)]), ms, &(x[0]));
				   Tc = LD(&(x[WS(rs, 12)]), ms, &(x[0]));
				   T7 = LD(&(x[WS(rs, 20)]), ms, &(x[0]));
				   Ta = LD(&(x[WS(rs, 28)]), ms, &(x[0]));
				   {
					V T1y, T3, T1w, T6, Td, T8, Tb, Ts, Tz;
					Ts = LD(&(x[WS(rs, 30)]), ms, &(x[0]));
					T1y = BYTWJ(&(W[TWVL * 46]), T1x);
					T3 = BYTWJ(&(W[TWVL * 30]), T2);
					T1w = BYTWJ(&(W[TWVL * 14]), T1v);
					T6 = BYTWJ(&(W[TWVL * 6]), T5);
					Td = BYTWJ(&(W[TWVL * 22]), Tc);
					T8 = BYTWJ(&(W[TWVL * 38]), T7);
					Tb = BYTWJ(&(W[TWVL * 54]), Ta);
					Tt = BYTWJ(&(W[TWVL * 58]), Ts);
					Tz = LD(&(x[WS(rs, 6)]), ms, &(x[0]));
					T4 = VSUB(T1, T3);
					T2m = VADD(T1, T3);
					T1z = VSUB(T1w, T1y);
					T2n = VADD(T1w, T1y);
					T9 = VSUB(T6, T8);
					T2p = VADD(T6, T8);
					Te = VSUB(Tb, Td);
					T2q = VADD(Tb, Td);
					TA = BYTWJ(&(W[TWVL * 10]), Tz);
				   }
				   Tu = LD(&(x[WS(rs, 14)]), ms, &(x[0]));
				   T2o = VADD(T2m, T2n);
				   T32 = VSUB(T2m, T2n);
				   Tx = LD(&(x[WS(rs, 22)]), ms, &(x[0]));
			      }
			 }
			 {
			      V Tv, To, Ty, Ti, Tj, Tm, Th;
			      Th = LD(&(x[WS(rs, 2)]), ms, &(x[0]));
			      T2r = VADD(T2p, T2q);
			      T3f = VSUB(T2q, T2p);
			      Tf = VADD(T9, Te);
			      T1A = VSUB(Te, T9);
			      Tv = BYTWJ(&(W[TWVL * 26]), Tu);
			      To = LD(&(x[WS(rs, 26)]), ms, &(x[0]));
			      Ty = BYTWJ(&(W[TWVL * 42]), Tx);
			      Ti = BYTWJ(&(W[TWVL * 2]), Th);
			      Tj = LD(&(x[WS(rs, 18)]), ms, &(x[0]));
			      Tm = LD(&(x[WS(rs, 10)]), ms, &(x[0]));
			      {
				   V T1f, T1h, T1a, T1c, T18, T2A, T2B, T1p;
				   {
					V T15, T17, T1o, T1m;
					{
					     V Tw, T2J, Tp, T2K, TB, Tk, Tn, T1n, T14, T16;
					     T14 = LD(&(x[WS(rs, 31)]), ms, &(x[WS(rs, 1)]));
					     T16 = LD(&(x[WS(rs, 15)]), ms, &(x[WS(rs, 1)]));
					     Tw = VSUB(Tt, Tv);
					     T2J = VADD(Tt, Tv);
					     Tp = BYTWJ(&(W[TWVL * 50]), To);
					     T2K = VADD(TA, Ty);
					     TB = VSUB(Ty, TA);
					     Tk = BYTWJ(&(W[TWVL * 34]), Tj);
					     Tn = BYTWJ(&(W[TWVL * 18]), Tm);
					     T15 = BYTWJ(&(W[TWVL * 60]), T14);
					     T17 = BYTWJ(&(W[TWVL * 28]), T16);
					     T1n = LD(&(x[WS(rs, 7)]), ms, &(x[WS(rs, 1)]));
					     {
						  V T2M, Tl, T2N, Tq, T1l;
						  T1l = LD(&(x[WS(rs, 23)]), ms, &(x[WS(rs, 1)]));
						  T34 = VSUB(T2J, T2K);
						  T2L = VADD(T2J, T2K);
						  T1D = VFMA(LDK(KP414213562), Tw, TB);
						  TC = VFNMS(LDK(KP414213562), TB, Tw);
						  T2M = VADD(Ti, Tk);
						  Tl = VSUB(Ti, Tk);
						  T2N = VADD(Tn, Tp);
						  Tq = VSUB(Tn, Tp);
						  T1o = BYTWJ(&(W[TWVL * 12]), T1n);
						  T1m = BYTWJ(&(W[TWVL * 44]), T1l);
						  {
						       V T1e, T1g, T19, T1b;
						       T1e = LD(&(x[WS(rs, 27)]), ms, &(x[WS(rs, 1)]));
						       T1g = LD(&(x[WS(rs, 11)]), ms, &(x[WS(rs, 1)]));
						       T19 = LD(&(x[WS(rs, 3)]), ms, &(x[WS(rs, 1)]));
						       T1b = LD(&(x[WS(rs, 19)]), ms, &(x[WS(rs, 1)]));
						       T33 = VSUB(T2M, T2N);
						       T2O = VADD(T2M, T2N);
						       T1C = VFMA(LDK(KP414213562), Tl, Tq);
						       Tr = VFNMS(LDK(KP414213562), Tq, Tl);
						       T1f = BYTWJ(&(W[TWVL * 52]), T1e);
						       T1h = BYTWJ(&(W[TWVL * 20]), T1g);
						       T1a = BYTWJ(&(W[TWVL * 4]), T19);
						       T1c = BYTWJ(&(W[TWVL * 36]), T1b);
						  }
					     }
					}
					T18 = VSUB(T15, T17);
					T2A = VADD(T15, T17);
					T2B = VADD(T1o, T1m);
					T1p = VSUB(T1m, T1o);
				   }
				   {
					V TG, TI, TZ, TX;
					{
					     V T1i, T2E, T1d, T2D, TH, TY, TF;
					     TF = LD(&(x[WS(rs, 1)]), ms, &(x[WS(rs, 1)]));
					     T1i = VSUB(T1f, T1h);
					     T2E = VADD(T1f, T1h);
					     T1d = VSUB(T1a, T1c);
					     T2D = VADD(T1a, T1c);
					     TH = LD(&(x[WS(rs, 17)]), ms, &(x[WS(rs, 1)]));
					     TY = LD(&(x[WS(rs, 25)]), ms, &(x[WS(rs, 1)]));
					     T2C = VADD(T2A, T2B);
					     T3a = VSUB(T2A, T2B);
					     TG = BYTWJ(&(W[0]), TF);
					     {
						  V TW, T1j, T1q, TP, TR, TK;
						  TW = LD(&(x[WS(rs, 9)]), ms, &(x[WS(rs, 1)]));
						  T2F = VADD(T2D, T2E);
						  T3b = VSUB(T2E, T2D);
						  T1j = VADD(T1d, T1i);
						  T1q = VSUB(T1i, T1d);
						  TI = BYTWJ(&(W[TWVL * 32]), TH);
						  TZ = BYTWJ(&(W[TWVL * 48]), TY);
						  TP = LD(&(x[WS(rs, 29)]), ms, &(x[WS(rs, 1)]));
						  TX = BYTWJ(&(W[TWVL * 16]), TW);
						  TR = LD(&(x[WS(rs, 13)]), ms, &(x[WS(rs, 1)]));
						  TK = LD(&(x[WS(rs, 5)]), ms, &(x[WS(rs, 1)]));
						  T1r = VFMA(LDK(KP707106781), T1q, T1p);
						  T21 = VFNMS(LDK(KP707106781), T1q, T1p);
						  T1k = VFMA(LDK(KP707106781), T1j, T18);
						  T20 = VFNMS(LDK(KP707106781), T1j, T18);
						  TQ = BYTWJ(&(W[TWVL * 56]), TP);
						  TM = LD(&(x[WS(rs, 21)]), ms, &(x[WS(rs, 1)]));
						  TS = BYTWJ(&(W[TWVL * 24]), TR);
						  TL = BYTWJ(&(W[TWVL * 8]), TK);
					     }
					}
					T2t = VADD(TG, TI);
					TJ = VSUB(TG, TI);
					T10 = VSUB(TX, TZ);
					T2u = VADD(TX, TZ);
				   }
			      }
			 }
		    }
		    {
			 V T2s, TT, T2x, T2P, T2Y, T2G, T37, T2v, T2w, TO, T2W, T30, T2U, TN, T2V;
			 T2s = VSUB(T2o, T2r);
			 T2U = VADD(T2o, T2r);
			 TN = BYTWJ(&(W[TWVL * 40]), TM);
			 TT = VSUB(TQ, TS);
			 T2x = VADD(TQ, TS);
			 T2P = VSUB(T2L, T2O);
			 T2V = VADD(T2O, T2L);
			 T2Y = VADD(T2C, T2F);
			 T2G = VSUB(T2C, T2F);
			 T37 = VSUB(T2t, T2u);
			 T2v = VADD(T2t, T2u);
			 T2w = VADD(TL, TN);
			 TO = VSUB(TL, TN);
			 T2W = VADD(T2U, T2V);
			 T30 = VSUB(T2U, T2V);
			 {
			      V T3i, T3o, T36, T3r, T3h, T3j, T12, T1Y, TV, T1X, T3s, T3d, T2Q, T2H, T31;
			      V T2Z;
			      {
				   V T35, T3g, T38, T2y, T11, TU;
				   T35 = VADD(T33, T34);
				   T3g = VSUB(T34, T33);
				   T38 = VSUB(T2w, T2x);
				   T2y = VADD(T2w, T2x);
				   T11 = VSUB(TO, TT);
				   TU = VADD(TO, TT);
				   {
					V T3c, T39, T2X, T2z;
					T3c = VFNMS(LDK(KP414213562), T3b, T3a);
					T3i = VFMA(LDK(KP414213562), T3a, T3b);
					T3o = VFNMS(LDK(KP707106781), T35, T32);
					T36 = VFMA(LDK(KP707106781), T35, T32);
					T3r = VFNMS(LDK(KP707106781), T3g, T3f);
					T3h = VFMA(LDK(KP707106781), T3g, T3f);
					T39 = VFNMS(LDK(KP414213562), T38, T37);
					T3j = VFMA(LDK(KP414213562), T37, T38);
					T2X = VADD(T2v, T2y);
					T2z = VSUB(T2v, T2y);
					T12 = VFMA(LDK(KP707106781), T11, T10);
					T1Y = VFNMS(LDK(KP707106781), T11, T10);
					TV = VFMA(LDK(KP707106781), TU, TJ);
					T1X = VFNMS(LDK(KP707106781), TU, TJ);
					T3s = VSUB(T3c, T39);
					T3d = VADD(T39, T3c);
					T2Q = VSUB(T2G, T2z);
					T2H = VADD(T2z, T2G);
					T31 = VSUB(T2Y, T2X);
					T2Z = VADD(T2X, T2Y);
				   }
			      }
			      {
				   V Tg, T1U, TD, T1G, T13, T1s, T1H, T1B, T1V, T1E, T3k, T3p, T2e, T2f;
				   Tg = VFMA(LDK(KP707106781), Tf, T4);
				   T1U = VFNMS(LDK(KP707106781), Tf, T4);
				   T3k = VSUB(T3i, T3j);
				   T3p = VADD(T3j, T3i);
				   {
					V T3v, T3t, T3e, T3m;
					T3v = VFNMS(LDK(KP923879532), T3s, T3r);
					T3t = VFMA(LDK(KP923879532), T3s, T3r);
					T3e = VFNMS(LDK(KP923879532), T3d, T36);
					T3m = VFMA(LDK(KP923879532), T3d, T36);
					{
					     V T2R, T2T, T2I, T2S;
					     T2R = VFNMS(LDK(KP707106781), T2Q, T2P);
					     T2T = VFMA(LDK(KP707106781), T2Q, T2P);
					     T2I = VFNMS(LDK(KP707106781), T2H, T2s);
					     T2S = VFMA(LDK(KP707106781), T2H, T2s);
					     ST(&(x[WS(rs, 24)]), VFNMSI(T31, T30), ms, &(x[0]));
					     ST(&(x[WS(rs, 8)]), VFMAI(T31, T30), ms, &(x[0]));
					     ST(&(x[0]), VADD(T2W, T2Z), ms, &(x[0]));
					     ST(&(x[WS(rs, 16)]), VSUB(T2W, T2Z), ms, &(x[0]));
					     {
						  V T3u, T3q, T3l, T3n;
						  T3u = VFMA(LDK(KP923879532), T3p, T3o);
						  T3q = VFNMS(LDK(KP923879532), T3p, T3o);
						  T3l = VFNMS(LDK(KP923879532), T3k, T3h);
						  T3n = VFMA(LDK(KP923879532), T3k, T3h);
						  ST(&(x[WS(rs, 4)]), VFMAI(T2T, T2S), ms, &(x[0]));
						  ST(&(x[WS(rs, 28)]), VFNMSI(T2T, T2S), ms, &(x[0]));
						  ST(&(x[WS(rs, 20)]), VFMAI(T2R, T2I), ms, &(x[0]));
						  ST(&(x[WS(rs, 12)]), VFNMSI(T2R, T2I), ms, &(x[0]));
						  ST(&(x[WS(rs, 22)]), VFNMSI(T3t, T3q), ms, &(x[0]));
						  ST(&(x[WS(rs, 10)]), VFMAI(T3t, T3q), ms, &(x[0]));
						  ST(&(x[WS(rs, 26)]), VFMAI(T3v, T3u), ms, &(x[0]));
						  ST(&(x[WS(rs, 6)]), VFNMSI(T3v, T3u), ms, &(x[0]));
						  ST(&(x[WS(rs, 2)]), VFMAI(T3n, T3m), ms, &(x[0]));
						  ST(&(x[WS(rs, 30)]), VFNMSI(T3n, T3m), ms, &(x[0]));
						  ST(&(x[WS(rs, 18)]), VFMAI(T3l, T3e), ms, &(x[0]));
						  ST(&(x[WS(rs, 14)]), VFNMSI(T3l, T3e), ms, &(x[0]));
						  T26 = VSUB(TC, Tr);
						  TD = VADD(Tr, TC);
					     }
					}
				   }
				   T1G = VFMA(LDK(KP198912367), TV, T12);
				   T13 = VFNMS(LDK(KP198912367), T12, TV);
				   T1s = VFNMS(LDK(KP198912367), T1r, T1k);
				   T1H = VFMA(LDK(KP198912367), T1k, T1r);
				   T1B = VFNMS(LDK(KP707106781), T1A, T1z);
				   T25 = VFMA(LDK(KP707106781), T1A, T1z);
				   T1V = VADD(T1C, T1D);
				   T1E = VSUB(T1C, T1D);
				   {
					V T1S, T1O, T1K, T1u, T1R, T1T, T1L, T1J;
					{
					     V TE, T1M, T1I, T1N, T1t, T1Q, T1F, T1P, T28, T29;
					     TE = VFMA(LDK(KP923879532), TD, Tg);
					     T1M = VFNMS(LDK(KP923879532), TD, Tg);
					     T1I = VSUB(T1G, T1H);
					     T1N = VADD(T1G, T1H);
					     T1t = VADD(T13, T1s);
					     T1Q = VSUB(T1s, T13);
					     T1F = VFMA(LDK(KP923879532), T1E, T1B);
					     T1P = VFNMS(LDK(KP923879532), T1E, T1B);
					     T28 = VFNMS(LDK(KP668178637), T1X, T1Y);
					     T1Z = VFMA(LDK(KP668178637), T1Y, T1X);
					     T1S = VFMA(LDK(KP980785280), T1N, T1M);
					     T1O = VFNMS(LDK(KP980785280), T1N, T1M);
					     T22 = VFMA(LDK(KP668178637), T21, T20);
					     T29 = VFNMS(LDK(KP668178637), T20, T21);
					     T1K = VFMA(LDK(KP980785280), T1t, TE);
					     T1u = VFNMS(LDK(KP980785280), T1t, TE);
					     T1R = VFNMS(LDK(KP980785280), T1Q, T1P);
					     T1T = VFMA(LDK(KP980785280), T1Q, T1P);
					     T1L = VFMA(LDK(KP980785280), T1I, T1F);
					     T1J = VFNMS(LDK(KP980785280), T1I, T1F);
					     T2e = VFNMS(LDK(KP923879532), T1V, T1U);
					     T1W = VFMA(LDK(KP923879532), T1V, T1U);
					     T2a = VSUB(T28, T29);
					     T2f = VADD(T28, T29);
					}
					ST(&(x[WS(rs, 23)]), VFMAI(T1R, T1O), ms, &(x[WS(rs, 1)]));
					ST(&(x[WS(rs, 9)]), VFNMSI(T1R, T1O), ms, &(x[WS(rs, 1)]));
					ST(&(x[WS(rs, 25)]), VFNMSI(T1T, T1S), ms, &(x[WS(rs, 1)]));
					ST(&(x[WS(rs, 7)]), VFMAI(T1T, T1S), ms, &(x[WS(rs, 1)]));
					ST(&(x[WS(rs, 31)]), VFMAI(T1L, T1K), ms, &(x[WS(rs, 1)]));
					ST(&(x[WS(rs, 1)]), VFNMSI(T1L, T1K), ms, &(x[WS(rs, 1)]));
					ST(&(x[WS(rs, 15)]), VFMAI(T1J, T1u), ms, &(x[WS(rs, 1)]));
					ST(&(x[WS(rs, 17)]), VFNMSI(T1J, T1u), ms, &(x[WS(rs, 1)]));
				   }
				   T2k = VFNMS(LDK(KP831469612), T2f, T2e);
				   T2g = VFMA(LDK(KP831469612), T2f, T2e);
			      }
			 }
		    }
	       }
	       {
		    V T2i, T23, T2h, T27;
		    T2i = VSUB(T22, T1Z);
		    T23 = VADD(T1Z, T22);
		    T2h = VFNMS(LDK(KP923879532), T26, T25);
		    T27 = VFMA(LDK(KP923879532), T26, T25);
		    {
			 V T2c, T24, T2j, T2l, T2d, T2b;
			 T2c = VFMA(LDK(KP831469612), T23, T1W);
			 T24 = VFNMS(LDK(KP831469612), T23, T1W);
			 T2j = VFMA(LDK(KP831469612), T2i, T2h);
			 T2l = VFNMS(LDK(KP831469612), T2i, T2h);
			 T2d = VFMA(LDK(KP831469612), T2a, T27);
			 T2b = VFNMS(LDK(KP831469612), T2a, T27);
			 ST(&(x[WS(rs, 21)]), VFNMSI(T2j, T2g), ms, &(x[WS(rs, 1)]));
			 ST(&(x[WS(rs, 11)]), VFMAI(T2j, T2g), ms, &(x[WS(rs, 1)]));
			 ST(&(x[WS(rs, 27)]), VFMAI(T2l, T2k), ms, &(x[WS(rs, 1)]));
			 ST(&(x[WS(rs, 5)]), VFNMSI(T2l, T2k), ms, &(x[WS(rs, 1)]));
			 ST(&(x[WS(rs, 3)]), VFMAI(T2d, T2c), ms, &(x[WS(rs, 1)]));
			 ST(&(x[WS(rs, 29)]), VFNMSI(T2d, T2c), ms, &(x[WS(rs, 1)]));
			 ST(&(x[WS(rs, 19)]), VFMAI(T2b, T24), ms, &(x[WS(rs, 1)]));
			 ST(&(x[WS(rs, 13)]), VFNMSI(T2b, T24), ms, &(x[WS(rs, 1)]));
		    }
	       }
	  }
     }
     VLEAVE();
}

static const tw_instr twinstr[] = {
     VTW(0, 1),
     VTW(0, 2),
     VTW(0, 3),
     VTW(0, 4),
     VTW(0, 5),
     VTW(0, 6),
     VTW(0, 7),
     VTW(0, 8),
     VTW(0, 9),
     VTW(0, 10),
     VTW(0, 11),
     VTW(0, 12),
     VTW(0, 13),
     VTW(0, 14),
     VTW(0, 15),
     VTW(0, 16),
     VTW(0, 17),
     VTW(0, 18),
     VTW(0, 19),
     VTW(0, 20),
     VTW(0, 21),
     VTW(0, 22),
     VTW(0, 23),
     VTW(0, 24),
     VTW(0, 25),
     VTW(0, 26),
     VTW(0, 27),
     VTW(0, 28),
     VTW(0, 29),
     VTW(0, 30),
     VTW(0, 31),
     {TW_NEXT, VL, 0}
};

static const ct_desc desc = { 32, XSIMD_STRING("t2fv_32"), twinstr, &GENUS, {119, 62, 98, 0}, 0, 0, 0 };

void XSIMD(codelet_t2fv_32) (planner *p) {
     X(kdft_dit_register) (p, t2fv_32, &desc);
}
#else				/* HAVE_FMA */

/* Generated by: ../../../genfft/gen_twiddle_c.native -simd -compact -variables 4 -pipeline-latency 8 -n 32 -name t2fv_32 -include t2f.h */

/*
 * This function contains 217 FP additions, 104 FP multiplications,
 * (or, 201 additions, 88 multiplications, 16 fused multiply/add),
 * 59 stack variables, 7 constants, and 64 memory accesses
 */
#include "t2f.h"

static void t2fv_32(R *ri, R *ii, const R *W, stride rs, INT mb, INT me, INT ms)
{
     DVK(KP555570233, +0.555570233019602224742830813948532874374937191);
     DVK(KP831469612, +0.831469612302545237078788377617905756738560812);
     DVK(KP195090322, +0.195090322016128267848284868477022240927691618);
     DVK(KP980785280, +0.980785280403230449126182236134239036973933731);
     DVK(KP382683432, +0.382683432365089771728459984030398866761344562);
     DVK(KP923879532, +0.923879532511286756128183189396788286822416626);
     DVK(KP707106781, +0.707106781186547524400844362104849039284835938);
     {
	  INT m;
	  R *x;
	  x = ri;
	  for (m = mb, W = W + (mb * ((TWVL / VL) * 62)); m < me; m = m + VL, x = x + (VL * ms), W = W + (TWVL * 62), MAKE_VOLATILE_STRIDE(32, rs)) {
	       V T4, T1A, T2o, T32, Tf, T1v, T2r, T3f, TC, T1C, T2L, T34, Tr, T1D, T2O;
	       V T33, T1k, T20, T2F, T3b, T1r, T21, T2C, T3a, TV, T1X, T2y, T38, T12, T1Y;
	       V T2v, T37;
	       {
		    V T1, T1z, T3, T1x, T1y, T2, T1w, T2m, T2n;
		    T1 = LD(&(x[0]), ms, &(x[0]));
		    T1y = LD(&(x[WS(rs, 24)]), ms, &(x[0]));
		    T1z = BYTWJ(&(W[TWVL * 46]), T1y);
		    T2 = LD(&(x[WS(rs, 16)]), ms, &(x[0]));
		    T3 = BYTWJ(&(W[TWVL * 30]), T2);
		    T1w = LD(&(x[WS(rs, 8)]), ms, &(x[0]));
		    T1x = BYTWJ(&(W[TWVL * 14]), T1w);
		    T4 = VSUB(T1, T3);
		    T1A = VSUB(T1x, T1z);
		    T2m = VADD(T1, T3);
		    T2n = VADD(T1x, T1z);
		    T2o = VADD(T2m, T2n);
		    T32 = VSUB(T2m, T2n);
	       }
	       {
		    V T6, Td, T8, Tb;
		    {
			 V T5, Tc, T7, Ta;
			 T5 = LD(&(x[WS(rs, 4)]), ms, &(x[0]));
			 T6 = BYTWJ(&(W[TWVL * 6]), T5);
			 Tc = LD(&(x[WS(rs, 12)]), ms, &(x[0]));
			 Td = BYTWJ(&(W[TWVL * 22]), Tc);
			 T7 = LD(&(x[WS(rs, 20)]), ms, &(x[0]));
			 T8 = BYTWJ(&(W[TWVL * 38]), T7);
			 Ta = LD(&(x[WS(rs, 28)]), ms, &(x[0]));
			 Tb = BYTWJ(&(W[TWVL * 54]), Ta);
		    }
		    {
			 V T9, Te, T2p, T2q;
			 T9 = VSUB(T6, T8);
			 Te = VSUB(Tb, Td);
			 Tf = VMUL(LDK(KP707106781), VADD(T9, Te));
			 T1v = VMUL(LDK(KP707106781), VSUB(Te, T9));
			 T2p = VADD(T6, T8);
			 T2q = VADD(Tb, Td);
			 T2r = VADD(T2p, T2q);
			 T3f = VSUB(T2q, T2p);
		    }
	       }
	       {
		    V Tt, TA, Tv, Ty;
		    {
			 V Ts, Tz, Tu, Tx;
			 Ts = LD(&(x[WS(rs, 30)]), ms, &(x[0]));
			 Tt = BYTWJ(&(W[TWVL * 58]), Ts);
			 Tz = LD(&(x[WS(rs, 22)]), ms, &(x[0]));
			 TA = BYTWJ(&(W[TWVL * 42]), Tz);
			 Tu = LD(&(x[WS(rs, 14)]), ms, &(x[0]));
			 Tv = BYTWJ(&(W[TWVL * 26]), Tu);
			 Tx = LD(&(x[WS(rs, 6)]), ms, &(x[0]));
			 Ty = BYTWJ(&(W[TWVL * 10]), Tx);
		    }
		    {
			 V Tw, TB, T2J, T2K;
			 Tw = VSUB(Tt, Tv);
			 TB = VSUB(Ty, TA);
			 TC = VFMA(LDK(KP923879532), Tw, VMUL(LDK(KP382683432), TB));
			 T1C = VFNMS(LDK(KP923879532), TB, VMUL(LDK(KP382683432), Tw));
			 T2J = VADD(Tt, Tv);
			 T2K = VADD(Ty, TA);
			 T2L = VADD(T2J, T2K);
			 T34 = VSUB(T2J, T2K);
		    }
	       }
	       {
		    V Ti, Tp, Tk, Tn;
		    {
			 V Th, To, Tj, Tm;
			 Th = LD(&(x[WS(rs, 2)]), ms, &(x[0]));
			 Ti = BYTWJ(&(W[TWVL * 2]), Th);
			 To = LD(&(x[WS(rs, 26)]), ms, &(x[0]));
			 Tp = BYTWJ(&(W[TWVL * 50]), To);
			 Tj = LD(&(x[WS(rs, 18)]), ms, &(x[0]));
			 Tk = BYTWJ(&(W[TWVL * 34]), Tj);
			 Tm = LD(&(x[WS(rs, 10)]), ms, &(x[0]));
			 Tn = BYTWJ(&(W[TWVL * 18]), Tm);
		    }
		    {
			 V Tl, Tq, T2M, T2N;
			 Tl = VSUB(Ti, Tk);
			 Tq = VSUB(Tn, Tp);
			 Tr = VFNMS(LDK(KP382683432), Tq, VMUL(LDK(KP923879532), Tl));
			 T1D = VFMA(LDK(KP382683432), Tl, VMUL(LDK(KP923879532), Tq));
			 T2M = VADD(Ti, Tk);
			 T2N = VADD(Tn, Tp);
			 T2O = VADD(T2M, T2N);
			 T33 = VSUB(T2M, T2N);
		    }
	       }
	       {
		    V T15, T17, T1p, T1n, T1f, T1h, T1i, T1a, T1c, T1d;
		    {
			 V T14, T16, T1o, T1m;
			 T14 = LD(&(x[WS(rs, 31)]), ms, &(x[WS(rs, 1)]));
			 T15 = BYTWJ(&(W[TWVL * 60]), T14);
			 T16 = LD(&(x[WS(rs, 15)]), ms, &(x[WS(rs, 1)]));
			 T17 = BYTWJ(&(W[TWVL * 28]), T16);
			 T1o = LD(&(x[WS(rs, 23)]), ms, &(x[WS(rs, 1)]));
			 T1p = BYTWJ(&(W[TWVL * 44]), T1o);
			 T1m = LD(&(x[WS(rs, 7)]), ms, &(x[WS(rs, 1)]));
			 T1n = BYTWJ(&(W[TWVL * 12]), T1m);
			 {
			      V T1e, T1g, T19, T1b;
			      T1e = LD(&(x[WS(rs, 27)]), ms, &(x[WS(rs, 1)]));
			      T1f = BYTWJ(&(W[TWVL * 52]), T1e);
			      T1g = LD(&(x[WS(rs, 11)]), ms, &(x[WS(rs, 1)]));
			      T1h = BYTWJ(&(W[TWVL * 20]), T1g);
			      T1i = VSUB(T1f, T1h);
			      T19 = LD(&(x[WS(rs, 3)]), ms, &(x[WS(rs, 1)]));
			      T1a = BYTWJ(&(W[TWVL * 4]), T19);
			      T1b = LD(&(x[WS(rs, 19)]), ms, &(x[WS(rs, 1)]));
			      T1c = BYTWJ(&(W[TWVL * 36]), T1b);
			      T1d = VSUB(T1a, T1c);
			 }
		    }
		    {
			 V T18, T1j, T2D, T2E;
			 T18 = VSUB(T15, T17);
			 T1j = VMUL(LDK(KP707106781), VADD(T1d, T1i));
			 T1k = VADD(T18, T1j);
			 T20 = VSUB(T18, T1j);
			 T2D = VADD(T1a, T1c);
			 T2E = VADD(T1f, T1h);
			 T2F = VADD(T2D, T2E);
			 T3b = VSUB(T2E, T2D);
		    }
		    {
			 V T1l, T1q, T2A, T2B;
			 T1l = VMUL(LDK(KP707106781), VSUB(T1i, T1d));
			 T1q = VSUB(T1n, T1p);
			 T1r = VSUB(T1l, T1q);
			 T21 = VADD(T1q, T1l);
			 T2A = VADD(T15, T17);
			 T2B = VADD(T1n, T1p);
			 T2C = VADD(T2A, T2B);
			 T3a = VSUB(T2A, T2B);
		    }
	       }
	       {
		    V TG, TI, T10, TY, TQ, TS, TT, TL, TN, TO;
		    {
			 V TF, TH, TZ, TX;
			 TF = LD(&(x[WS(rs, 1)]), ms, &(x[WS(rs, 1)]));
			 TG = BYTWJ(&(W[0]), TF);
			 TH = LD(&(x[WS(rs, 17)]), ms, &(x[WS(rs, 1)]));
			 TI = BYTWJ(&(W[TWVL * 32]), TH);
			 TZ = LD(&(x[WS(rs, 25)]), ms, &(x[WS(rs, 1)]));
			 T10 = BYTWJ(&(W[TWVL * 48]), TZ);
			 TX = LD(&(x[WS(rs, 9)]), ms, &(x[WS(rs, 1)]));
			 TY = BYTWJ(&(W[TWVL * 16]), TX);
			 {
			      V TP, TR, TK, TM;
			      TP = LD(&(x[WS(rs, 29)]), ms, &(x[WS(rs, 1)]));
			      TQ = BYTWJ(&(W[TWVL * 56]), TP);
			      TR = LD(&(x[WS(rs, 13)]), ms, &(x[WS(rs, 1)]));
			      TS = BYTWJ(&(W[TWVL * 24]), TR);
			      TT = VSUB(TQ, TS);
			      TK = LD(&(x[WS(rs, 5)]), ms, &(x[WS(rs, 1)]));
			      TL = BYTWJ(&(W[TWVL * 8]), TK);
			      TM = LD(&(x[WS(rs, 21)]), ms, &(x[WS(rs, 1)]));
			      TN = BYTWJ(&(W[TWVL * 40]), TM);
			      TO = VSUB(TL, TN);
			 }
		    }
		    {
			 V TJ, TU, T2w, T2x;
			 TJ = VSUB(TG, TI);
			 TU = VMUL(LDK(KP707106781), VADD(TO, TT));
			 TV = VADD(TJ, TU);
			 T1X = VSUB(TJ, TU);
			 T2w = VADD(TL, TN);
			 T2x = VADD(TQ, TS);
			 T2y = VADD(T2w, T2x);
			 T38 = VSUB(T2x, T2w);
		    }
		    {
			 V TW, T11, T2t, T2u;
			 TW = VMUL(LDK(KP707106781), VSUB(TT, TO));
			 T11 = VSUB(TY, T10);
			 T12 = VSUB(TW, T11);
			 T1Y = VADD(T11, TW);
			 T2t = VADD(TG, TI);
			 T2u = VADD(TY, T10);
			 T2v = VADD(T2t, T2u);
			 T37 = VSUB(T2t, T2u);
		    }
	       }
	       {
		    V T2W, T30, T2Z, T31;
		    {
			 V T2U, T2V, T2X, T2Y;
			 T2U = VADD(T2o, T2r);
			 T2V = VADD(T2O, T2L);
			 T2W = VADD(T2U, T2V);
			 T30 = VSUB(T2U, T2V);
			 T2X = VADD(T2v, T2y);
			 T2Y = VADD(T2C, T2F);
			 T2Z = VADD(T2X, T2Y);
			 T31 = VBYI(VSUB(T2Y, T2X));
		    }
		    ST(&(x[WS(rs, 16)]), VSUB(T2W, T2Z), ms, &(x[0]));
		    ST(&(x[WS(rs, 8)]), VADD(T30, T31), ms, &(x[0]));
		    ST(&(x[0]), VADD(T2W, T2Z), ms, &(x[0]));
		    ST(&(x[WS(rs, 24)]), VSUB(T30, T31), ms, &(x[0]));
	       }
	       {
		    V T2s, T2P, T2H, T2Q, T2z, T2G;
		    T2s = VSUB(T2o, T2r);
		    T2P = VSUB(T2L, T2O);
		    T2z = VSUB(T2v, T2y);
		    T2G = VSUB(T2C, T2F);
		    T2H = VMUL(LDK(KP707106781), VADD(T2z, T2G));
		    T2Q = VMUL(LDK(KP707106781), VSUB(T2G, T2z));
		    {
			 V T2I, T2R, T2S, T2T;
			 T2I = VADD(T2s, T2H);
			 T2R = VBYI(VADD(T2P, T2Q));
			 ST(&(x[WS(rs, 28)]), VSUB(T2I, T2R), ms, &(x[0]));
			 ST(&(x[WS(rs, 4)]), VADD(T2I, T2R), ms, &(x[0]));
			 T2S = VSUB(T2s, T2H);
			 T2T = VBYI(VSUB(T2Q, T2P));
			 ST(&(x[WS(rs, 20)]), VSUB(T2S, T2T), ms, &(x[0]));
			 ST(&(x[WS(rs, 12)]), VADD(T2S, T2T), ms, &(x[0]));
		    }
	       }
	       {
		    V T36, T3r, T3h, T3p, T3d, T3o, T3k, T3s, T35, T3g;
		    T35 = VMUL(LDK(KP707106781), VADD(T33, T34));
		    T36 = VADD(T32, T35);
		    T3r = VSUB(T32, T35);
		    T3g = VMUL(LDK(KP707106781), VSUB(T34, T33));
		    T3h = VADD(T3f, T3g);
		    T3p = VSUB(T3g, T3f);
		    {
			 V T39, T3c, T3i, T3j;
			 T39 = VFMA(LDK(KP923879532), T37, VMUL(LDK(KP382683432), T38));
			 T3c = VFNMS(LDK(KP382683432), T3b, VMUL(LDK(KP923879532), T3a));
			 T3d = VADD(T39, T3c);
			 T3o = VSUB(T3c, T39);
			 T3i = VFNMS(LDK(KP382683432), T37, VMUL(LDK(KP923879532), T38));
			 T3j = VFMA(LDK(KP382683432), T3a, VMUL(LDK(KP923879532), T3b));
			 T3k = VADD(T3i, T3j);
			 T3s = VSUB(T3j, T3i);
		    }
		    {
			 V T3e, T3l, T3u, T3v;
			 T3e = VADD(T36, T3d);
			 T3l = VBYI(VADD(T3h, T3k));
			 ST(&(x[WS(rs, 30)]), VSUB(T3e, T3l), ms, &(x[0]));
			 ST(&(x[WS(rs, 2)]), VADD(T3e, T3l), ms, &(x[0]));
			 T3u = VBYI(VADD(T3p, T3o));
			 T3v = VADD(T3r, T3s);
			 ST(&(x[WS(rs, 6)]), VADD(T3u, T3v), ms, &(x[0]));
			 ST(&(x[WS(rs, 26)]), VSUB(T3v, T3u), ms, &(x[0]));
		    }
		    {
			 V T3m, T3n, T3q, T3t;
			 T3m = VSUB(T36, T3d);
			 T3n = VBYI(VSUB(T3k, T3h));
			 ST(&(x[WS(rs, 18)]), VSUB(T3m, T3n), ms, &(x[0]));
			 ST(&(x[WS(rs, 14)]), VADD(T3m, T3n), ms, &(x[0]));
			 T3q = VBYI(VSUB(T3o, T3p));
			 T3t = VSUB(T3r, T3s);
			 ST(&(x[WS(rs, 10)]), VADD(T3q, T3t), ms, &(x[0]));
			 ST(&(x[WS(rs, 22)]), VSUB(T3t, T3q), ms, &(x[0]));
		    }
	       }
	       {
		    V TE, T1P, T1I, T1Q, T1t, T1M, T1F, T1N;
		    {
			 V Tg, TD, T1G, T1H;
			 Tg = VADD(T4, Tf);
			 TD = VADD(Tr, TC);
			 TE = VADD(Tg, TD);
			 T1P = VSUB(Tg, TD);
			 T1G = VFNMS(LDK(KP195090322), TV, VMUL(LDK(KP980785280), T12));
			 T1H = VFMA(LDK(KP195090322), T1k, VMUL(LDK(KP980785280), T1r));
			 T1I = VADD(T1G, T1H);
			 T1Q = VSUB(T1H, T1G);
		    }
		    {
			 V T13, T1s, T1B, T1E;
			 T13 = VFMA(LDK(KP980785280), TV, VMUL(LDK(KP195090322), T12));
			 T1s = VFNMS(LDK(KP195090322), T1r, VMUL(LDK(KP980785280), T1k));
			 T1t = VADD(T13, T1s);
			 T1M = VSUB(T1s, T13);
			 T1B = VSUB(T1v, T1A);
			 T1E = VSUB(T1C, T1D);
			 T1F = VADD(T1B, T1E);
			 T1N = VSUB(T1E, T1B);
		    }
		    {
			 V T1u, T1J, T1S, T1T;
			 T1u = VADD(TE, T1t);
			 T1J = VBYI(VADD(T1F, T1I));
			 ST(&(x[WS(rs, 31)]), VSUB(T1u, T1J), ms, &(x[WS(rs, 1)]));
			 ST(&(x[WS(rs, 1)]), VADD(T1u, T1J), ms, &(x[WS(rs, 1)]));
			 T1S = VBYI(VADD(T1N, T1M));
			 T1T = VADD(T1P, T1Q);
			 ST(&(x[WS(rs, 7)]), VADD(T1S, T1T), ms, &(x[WS(rs, 1)]));
			 ST(&(x[WS(rs, 25)]), VSUB(T1T, T1S), ms, &(x[WS(rs, 1)]));
		    }
		    {
			 V T1K, T1L, T1O, T1R;
			 T1K = VSUB(TE, T1t);
			 T1L = VBYI(VSUB(T1I, T1F));
			 ST(&(x[WS(rs, 17)]), VSUB(T1K, T1L), ms, &(x[WS(rs, 1)]));
			 ST(&(x[WS(rs, 15)]), VADD(T1K, T1L), ms, &(x[WS(rs, 1)]));
			 T1O = VBYI(VSUB(T1M, T1N));
			 T1R = VSUB(T1P, T1Q);
			 ST(&(x[WS(rs, 9)]), VADD(T1O, T1R), ms, &(x[WS(rs, 1)]));
			 ST(&(x[WS(rs, 23)]), VSUB(T1R, T1O), ms, &(x[WS(rs, 1)]));
		    }
	       }
	       {
		    V T1W, T2h, T2a, T2i, T23, T2e, T27, T2f;
		    {
			 V T1U, T1V, T28, T29;
			 T1U = VSUB(T4, Tf);
			 T1V = VADD(T1D, T1C);
			 T1W = VADD(T1U, T1V);
			 T2h = VSUB(T1U, T1V);
			 T28 = VFNMS(LDK(KP555570233), T1X, VMUL(LDK(KP831469612), T1Y));
			 T29 = VFMA(LDK(KP555570233), T20, VMUL(LDK(KP831469612), T21));
			 T2a = VADD(T28, T29);
			 T2i = VSUB(T29, T28);
		    }
		    {
			 V T1Z, T22, T25, T26;
			 T1Z = VFMA(LDK(KP831469612), T1X, VMUL(LDK(KP555570233), T1Y));
			 T22 = VFNMS(LDK(KP555570233), T21, VMUL(LDK(KP831469612), T20));
			 T23 = VADD(T1Z, T22);
			 T2e = VSUB(T22, T1Z);
			 T25 = VADD(T1A, T1v);
			 T26 = VSUB(TC, Tr);
			 T27 = VADD(T25, T26);
			 T2f = VSUB(T26, T25);
		    }
		    {
			 V T24, T2b, T2k, T2l;
			 T24 = VADD(T1W, T23);
			 T2b = VBYI(VADD(T27, T2a));
			 ST(&(x[WS(rs, 29)]), VSUB(T24, T2b), ms, &(x[WS(rs, 1)]));
			 ST(&(x[WS(rs, 3)]), VADD(T24, T2b), ms, &(x[WS(rs, 1)]));
			 T2k = VBYI(VADD(T2f, T2e));
			 T2l = VADD(T2h, T2i);
			 ST(&(x[WS(rs, 5)]), VADD(T2k, T2l), ms, &(x[WS(rs, 1)]));
			 ST(&(x[WS(rs, 27)]), VSUB(T2l, T2k), ms, &(x[WS(rs, 1)]));
		    }
		    {
			 V T2c, T2d, T2g, T2j;
			 T2c = VSUB(T1W, T23);
			 T2d = VBYI(VSUB(T2a, T27));
			 ST(&(x[WS(rs, 19)]), VSUB(T2c, T2d), ms, &(x[WS(rs, 1)]));
			 ST(&(x[WS(rs, 13)]), VADD(T2c, T2d), ms, &(x[WS(rs, 1)]));
			 T2g = VBYI(VSUB(T2e, T2f));
			 T2j = VSUB(T2h, T2i);
			 ST(&(x[WS(rs, 11)]), VADD(T2g, T2j), ms, &(x[WS(rs, 1)]));
			 ST(&(x[WS(rs, 21)]), VSUB(T2j, T2g), ms, &(x[WS(rs, 1)]));
		    }
	       }
	  }
     }
     VLEAVE();
}

static const tw_instr twinstr[] = {
     VTW(0, 1),
     VTW(0, 2),
     VTW(0, 3),
     VTW(0, 4),
     VTW(0, 5),
     VTW(0, 6),
     VTW(0, 7),
     VTW(0, 8),
     VTW(0, 9),
     VTW(0, 10),
     VTW(0, 11),
     VTW(0, 12),
     VTW(0, 13),
     VTW(0, 14),
     VTW(0, 15),
     VTW(0, 16),
     VTW(0, 17),
     VTW(0, 18),
     VTW(0, 19),
     VTW(0, 20),
     VTW(0, 21),
     VTW(0, 22),
     VTW(0, 23),
     VTW(0, 24),
     VTW(0, 25),
     VTW(0, 26),
     VTW(0, 27),
     VTW(0, 28),
     VTW(0, 29),
     VTW(0, 30),
     VTW(0, 31),
     {TW_NEXT, VL, 0}
};

static const ct_desc desc = { 32, XSIMD_STRING("t2fv_32"), twinstr, &GENUS, {201, 88, 16, 0}, 0, 0, 0 };

void XSIMD(codelet_t2fv_32) (planner *p) {
     X(kdft_dit_register) (p, t2fv_32, &desc);
}
#endif				/* HAVE_FMA */
