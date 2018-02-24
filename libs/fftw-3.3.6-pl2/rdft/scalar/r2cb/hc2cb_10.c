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
/* Generated on Fri Jan 27 16:13:22 EST 2017 */

#include "codelet-rdft.h"

#ifdef HAVE_FMA

/* Generated by: ../../../genfft/gen_hc2c.native -fma -reorder-insns -schedule-for-pipeline -compact -variables 4 -pipeline-latency 4 -sign 1 -n 10 -dif -name hc2cb_10 -include hc2cb.h */

/*
 * This function contains 102 FP additions, 72 FP multiplications,
 * (or, 48 additions, 18 multiplications, 54 fused multiply/add),
 * 71 stack variables, 4 constants, and 40 memory accesses
 */
#include "hc2cb.h"

static void hc2cb_10(R *Rp, R *Ip, R *Rm, R *Im, const R *W, stride rs, INT mb, INT me, INT ms)
{
     DK(KP951056516, +0.951056516295153572116439333379382143405698634);
     DK(KP559016994, +0.559016994374947424102293417182819058860154590);
     DK(KP250000000, +0.250000000000000000000000000000000000000000000);
     DK(KP618033988, +0.618033988749894848204586834365638117720309180);
     {
	  INT m;
	  for (m = mb, W = W + ((mb - 1) * 18); m < me; m = m + 1, Rp = Rp + ms, Ip = Ip + ms, Rm = Rm - ms, Im = Im - ms, W = W + 18, MAKE_VOLATILE_STRIDE(40, rs)) {
	       E T21, T1Y, T1X;
	       {
		    E T1B, TH, T1g, T3, T1V, T1x, T1G, T1E, TM, TK, T11, TB, T7, T1m, T1J;
		    E TO, Th, T1h, T6, T8, TF, TG, T1i, T9;
		    TF = Ip[0];
		    TG = Im[WS(rs, 4)];
		    {
			 E T1u, Tp, Tu, T1s, Tz, T1v, Ts, Tv;
			 {
			      E Tx, Ty, Tn, To, Tq, Tr;
			      Tn = Ip[WS(rs, 4)];
			      To = Im[0];
			      Tx = Ip[WS(rs, 3)];
			      T1B = TF + TG;
			      TH = TF - TG;
			      T1u = Tn + To;
			      Tp = Tn - To;
			      Ty = Im[WS(rs, 1)];
			      Tq = Ip[WS(rs, 1)];
			      Tr = Im[WS(rs, 3)];
			      Tu = Ip[WS(rs, 2)];
			      T1s = Tx + Ty;
			      Tz = Tx - Ty;
			      T1v = Tq + Tr;
			      Ts = Tq - Tr;
			      Tv = Im[WS(rs, 2)];
			 }
			 {
			      E T1, T1w, T1D, TJ, Tt, T1r, Tw, T2;
			      T1 = Rp[0];
			      T1w = T1u + T1v;
			      T1D = T1u - T1v;
			      TJ = Tp + Ts;
			      Tt = Tp - Ts;
			      T1r = Tu + Tv;
			      Tw = Tu - Tv;
			      T2 = Rm[WS(rs, 4)];
			      {
				   E Tb, Tc, Te, Tf;
				   Tb = Rp[WS(rs, 4)];
				   {
					E T1t, T1C, TI, TA;
					T1t = T1r + T1s;
					T1C = T1r - T1s;
					TI = Tw + Tz;
					TA = Tw - Tz;
					T1g = T1 - T2;
					T3 = T1 + T2;
					T1V = FNMS(KP618033988, T1t, T1w);
					T1x = FMA(KP618033988, T1w, T1t);
					T1G = T1C - T1D;
					T1E = T1C + T1D;
					TM = TI - TJ;
					TK = TI + TJ;
					T11 = FMA(KP618033988, Tt, TA);
					TB = FNMS(KP618033988, TA, Tt);
					Tc = Rm[0];
				   }
				   Te = Rm[WS(rs, 3)];
				   Tf = Rp[WS(rs, 1)];
				   {
					E T4, T1k, Td, T1l, Tg, T5;
					T4 = Rp[WS(rs, 2)];
					T1k = Tb - Tc;
					Td = Tb + Tc;
					T1l = Te - Tf;
					Tg = Te + Tf;
					T5 = Rm[WS(rs, 2)];
					T7 = Rm[WS(rs, 1)];
					T1m = T1k + T1l;
					T1J = T1k - T1l;
					TO = Td - Tg;
					Th = Td + Tg;
					T1h = T4 - T5;
					T6 = T4 + T5;
					T8 = Rp[WS(rs, 3)];
				   }
			      }
			 }
		    }
		    Rm[0] = TH + TK;
		    T1i = T7 - T8;
		    T9 = T7 + T8;
		    {
			 E T2d, T1F, T29, T1I, TP, T2c, T1p, Tl, T1o, Tk, T2b, T2e, T17, T14, T13;
			 T2d = T1B + T1E;
			 T1F = FNMS(KP250000000, T1E, T1B);
			 {
			      E T1j, Ta, T1n, Ti, T2a;
			      T29 = W[8];
			      T1I = T1h - T1i;
			      T1j = T1h + T1i;
			      TP = T6 - T9;
			      Ta = T6 + T9;
			      T2c = W[9];
			      T1p = T1j - T1m;
			      T1n = T1j + T1m;
			      Tl = Ta - Th;
			      Ti = Ta + Th;
			      T1o = FNMS(KP250000000, T1n, T1g);
			      T2a = T1g + T1n;
			      Rp[0] = T3 + Ti;
			      Tk = FNMS(KP250000000, Ti, T3);
			      T2b = T29 * T2a;
			      T2e = T2c * T2a;
			 }
			 {
			      E T16, TQ, T10, Tm, TL;
			      T16 = FMA(KP618033988, TO, TP);
			      TQ = FNMS(KP618033988, TP, TO);
			      Ip[WS(rs, 2)] = FNMS(T2c, T2d, T2b);
			      Im[WS(rs, 2)] = FMA(T29, T2d, T2e);
			      T10 = FMA(KP559016994, Tl, Tk);
			      Tm = FNMS(KP559016994, Tl, Tk);
			      TL = FNMS(KP250000000, TK, TH);
			      {
				   E TE, TU, T12, TR, TX, T1d, T1c, T19, TD, T1e, T1b, TW, TT;
				   {
					E TC, T15, T1a, TS, Tj, TN;
					TE = W[3];
					TC = FMA(KP951056516, TB, Tm);
					TU = FNMS(KP951056516, TB, Tm);
					TN = FNMS(KP559016994, TM, TL);
					T15 = FMA(KP559016994, TM, TL);
					T12 = FMA(KP951056516, T11, T10);
					T1a = FNMS(KP951056516, T11, T10);
					TS = TE * TC;
					TR = FNMS(KP951056516, TQ, TN);
					TX = FMA(KP951056516, TQ, TN);
					Tj = W[2];
					T1d = FMA(KP951056516, T16, T15);
					T17 = FNMS(KP951056516, T16, T15);
					T1c = W[11];
					T19 = W[10];
					Rm[WS(rs, 1)] = FMA(Tj, TR, TS);
					TD = Tj * TC;
					T1e = T1c * T1a;
					T1b = T19 * T1a;
				   }
				   Rp[WS(rs, 1)] = FNMS(TE, TR, TD);
				   Rm[WS(rs, 3)] = FMA(T19, T1d, T1e);
				   Rp[WS(rs, 3)] = FNMS(T1c, T1d, T1b);
				   TW = W[15];
				   TT = W[14];
				   {
					E TZ, T18, TY, TV;
					T14 = W[7];
					TY = TW * TU;
					TV = TT * TU;
					TZ = W[6];
					T18 = T14 * T12;
					Rm[WS(rs, 4)] = FMA(TT, TX, TY);
					Rp[WS(rs, 4)] = FNMS(TW, TX, TV);
					T13 = TZ * T12;
					Rm[WS(rs, 2)] = FMA(TZ, T17, T18);
				   }
			      }
			 }
			 {
			      E T20, T1K, T1q, T1U;
			      T20 = FNMS(KP618033988, T1I, T1J);
			      T1K = FMA(KP618033988, T1J, T1I);
			      Rp[WS(rs, 2)] = FNMS(T14, T17, T13);
			      T1q = FMA(KP559016994, T1p, T1o);
			      T1U = FNMS(KP559016994, T1p, T1o);
			      {
				   E T1A, T1O, T1W, T1R, T1L, T27, T26, T23, T1z, T28, T25, T1Q, T1N;
				   {
					E T1y, T1Z, T24, T1M, T1f, T1H;
					T1A = W[1];
					T1O = FMA(KP951056516, T1x, T1q);
					T1y = FNMS(KP951056516, T1x, T1q);
					T1Z = FNMS(KP559016994, T1G, T1F);
					T1H = FMA(KP559016994, T1G, T1F);
					T24 = FMA(KP951056516, T1V, T1U);
					T1W = FNMS(KP951056516, T1V, T1U);
					T1M = T1A * T1y;
					T1R = FNMS(KP951056516, T1K, T1H);
					T1L = FMA(KP951056516, T1K, T1H);
					T1f = W[0];
					T21 = FMA(KP951056516, T20, T1Z);
					T27 = FNMS(KP951056516, T20, T1Z);
					T26 = W[13];
					T23 = W[12];
					Im[0] = FMA(T1f, T1L, T1M);
					T1z = T1f * T1y;
					T28 = T26 * T24;
					T25 = T23 * T24;
				   }
				   Ip[0] = FNMS(T1A, T1L, T1z);
				   Im[WS(rs, 3)] = FMA(T23, T27, T28);
				   Ip[WS(rs, 3)] = FNMS(T26, T27, T25);
				   T1Q = W[17];
				   T1N = W[16];
				   {
					E T1T, T22, T1S, T1P;
					T1Y = W[5];
					T1S = T1Q * T1O;
					T1P = T1N * T1O;
					T1T = W[4];
					T22 = T1Y * T1W;
					Im[WS(rs, 4)] = FMA(T1N, T1R, T1S);
					Ip[WS(rs, 4)] = FNMS(T1Q, T1R, T1P);
					T1X = T1T * T1W;
					Im[WS(rs, 1)] = FMA(T1T, T21, T22);
				   }
			      }
			 }
		    }
	       }
	       Ip[WS(rs, 1)] = FNMS(T1Y, T21, T1X);
	  }
     }
}

static const tw_instr twinstr[] = {
     {TW_FULL, 1, 10},
     {TW_NEXT, 1, 0}
};

static const hc2c_desc desc = { 10, "hc2cb_10", twinstr, &GENUS, {48, 18, 54, 0} };

void X(codelet_hc2cb_10) (planner *p) {
     X(khc2c_register) (p, hc2cb_10, &desc, HC2C_VIA_RDFT);
}
#else				/* HAVE_FMA */

/* Generated by: ../../../genfft/gen_hc2c.native -compact -variables 4 -pipeline-latency 4 -sign 1 -n 10 -dif -name hc2cb_10 -include hc2cb.h */

/*
 * This function contains 102 FP additions, 60 FP multiplications,
 * (or, 72 additions, 30 multiplications, 30 fused multiply/add),
 * 39 stack variables, 4 constants, and 40 memory accesses
 */
#include "hc2cb.h"

static void hc2cb_10(R *Rp, R *Ip, R *Rm, R *Im, const R *W, stride rs, INT mb, INT me, INT ms)
{
     DK(KP250000000, +0.250000000000000000000000000000000000000000000);
     DK(KP951056516, +0.951056516295153572116439333379382143405698634);
     DK(KP587785252, +0.587785252292473129168705954639072768597652438);
     DK(KP559016994, +0.559016994374947424102293417182819058860154590);
     {
	  INT m;
	  for (m = mb, W = W + ((mb - 1) * 18); m < me; m = m + 1, Rp = Rp + ms, Ip = Ip + ms, Rm = Rm - ms, Im = Im - ms, W = W + 18, MAKE_VOLATILE_STRIDE(40, rs)) {
	       E T3, T18, TJ, T1i, TE, TF, T1B, T1A, T1f, T1t, Ti, Tl, Tt, TA, T1w;
	       E T1v, T1p, T1E, TM, TO;
	       {
		    E T1, T2, TH, TI;
		    T1 = Rp[0];
		    T2 = Rm[WS(rs, 4)];
		    T3 = T1 + T2;
		    T18 = T1 - T2;
		    TH = Ip[0];
		    TI = Im[WS(rs, 4)];
		    TJ = TH - TI;
		    T1i = TH + TI;
	       }
	       {
		    E T6, T19, Tg, T1d, T9, T1a, Td, T1c;
		    {
			 E T4, T5, Te, Tf;
			 T4 = Rp[WS(rs, 2)];
			 T5 = Rm[WS(rs, 2)];
			 T6 = T4 + T5;
			 T19 = T4 - T5;
			 Te = Rm[WS(rs, 3)];
			 Tf = Rp[WS(rs, 1)];
			 Tg = Te + Tf;
			 T1d = Te - Tf;
		    }
		    {
			 E T7, T8, Tb, Tc;
			 T7 = Rm[WS(rs, 1)];
			 T8 = Rp[WS(rs, 3)];
			 T9 = T7 + T8;
			 T1a = T7 - T8;
			 Tb = Rp[WS(rs, 4)];
			 Tc = Rm[0];
			 Td = Tb + Tc;
			 T1c = Tb - Tc;
		    }
		    TE = T6 - T9;
		    TF = Td - Tg;
		    T1B = T1c - T1d;
		    T1A = T19 - T1a;
		    {
			 E T1b, T1e, Ta, Th;
			 T1b = T19 + T1a;
			 T1e = T1c + T1d;
			 T1f = T1b + T1e;
			 T1t = KP559016994 * (T1b - T1e);
			 Ta = T6 + T9;
			 Th = Td + Tg;
			 Ti = Ta + Th;
			 Tl = KP559016994 * (Ta - Th);
		    }
	       }
	       {
		    E Tp, T1j, Tz, T1n, Ts, T1k, Tw, T1m;
		    {
			 E Tn, To, Tx, Ty;
			 Tn = Ip[WS(rs, 2)];
			 To = Im[WS(rs, 2)];
			 Tp = Tn - To;
			 T1j = Tn + To;
			 Tx = Ip[WS(rs, 1)];
			 Ty = Im[WS(rs, 3)];
			 Tz = Tx - Ty;
			 T1n = Tx + Ty;
		    }
		    {
			 E Tq, Tr, Tu, Tv;
			 Tq = Ip[WS(rs, 3)];
			 Tr = Im[WS(rs, 1)];
			 Ts = Tq - Tr;
			 T1k = Tq + Tr;
			 Tu = Ip[WS(rs, 4)];
			 Tv = Im[0];
			 Tw = Tu - Tv;
			 T1m = Tu + Tv;
		    }
		    Tt = Tp - Ts;
		    TA = Tw - Tz;
		    T1w = T1m + T1n;
		    T1v = T1j + T1k;
		    {
			 E T1l, T1o, TK, TL;
			 T1l = T1j - T1k;
			 T1o = T1m - T1n;
			 T1p = T1l + T1o;
			 T1E = KP559016994 * (T1l - T1o);
			 TK = Tp + Ts;
			 TL = Tw + Tz;
			 TM = TK + TL;
			 TO = KP559016994 * (TK - TL);
		    }
	       }
	       Rp[0] = T3 + Ti;
	       Rm[0] = TJ + TM;
	       {
		    E T1g, T1q, T17, T1h;
		    T1g = T18 + T1f;
		    T1q = T1i + T1p;
		    T17 = W[8];
		    T1h = W[9];
		    Ip[WS(rs, 2)] = FNMS(T1h, T1q, T17 * T1g);
		    Im[WS(rs, 2)] = FMA(T1h, T1g, T17 * T1q);
	       }
	       {
		    E TB, TG, T11, TX, TP, T10, Tm, TW, TN, Tk;
		    TB = FNMS(KP951056516, TA, KP587785252 * Tt);
		    TG = FNMS(KP951056516, TF, KP587785252 * TE);
		    T11 = FMA(KP951056516, TE, KP587785252 * TF);
		    TX = FMA(KP951056516, Tt, KP587785252 * TA);
		    TN = FNMS(KP250000000, TM, TJ);
		    TP = TN - TO;
		    T10 = TO + TN;
		    Tk = FNMS(KP250000000, Ti, T3);
		    Tm = Tk - Tl;
		    TW = Tl + Tk;
		    {
			 E TC, TQ, Tj, TD;
			 TC = Tm - TB;
			 TQ = TG + TP;
			 Tj = W[2];
			 TD = W[3];
			 Rp[WS(rs, 1)] = FNMS(TD, TQ, Tj * TC);
			 Rm[WS(rs, 1)] = FMA(TD, TC, Tj * TQ);
		    }
		    {
			 E T14, T16, T13, T15;
			 T14 = TW - TX;
			 T16 = T11 + T10;
			 T13 = W[10];
			 T15 = W[11];
			 Rp[WS(rs, 3)] = FNMS(T15, T16, T13 * T14);
			 Rm[WS(rs, 3)] = FMA(T15, T14, T13 * T16);
		    }
		    {
			 E TS, TU, TR, TT;
			 TS = Tm + TB;
			 TU = TP - TG;
			 TR = W[14];
			 TT = W[15];
			 Rp[WS(rs, 4)] = FNMS(TT, TU, TR * TS);
			 Rm[WS(rs, 4)] = FMA(TT, TS, TR * TU);
		    }
		    {
			 E TY, T12, TV, TZ;
			 TY = TW + TX;
			 T12 = T10 - T11;
			 TV = W[6];
			 TZ = W[7];
			 Rp[WS(rs, 2)] = FNMS(TZ, T12, TV * TY);
			 Rm[WS(rs, 2)] = FMA(TZ, TY, TV * T12);
		    }
	       }
	       {
		    E T1x, T1C, T1Q, T1N, T1F, T1R, T1u, T1M, T1D, T1s;
		    T1x = FNMS(KP951056516, T1w, KP587785252 * T1v);
		    T1C = FNMS(KP951056516, T1B, KP587785252 * T1A);
		    T1Q = FMA(KP951056516, T1A, KP587785252 * T1B);
		    T1N = FMA(KP951056516, T1v, KP587785252 * T1w);
		    T1D = FNMS(KP250000000, T1p, T1i);
		    T1F = T1D - T1E;
		    T1R = T1E + T1D;
		    T1s = FNMS(KP250000000, T1f, T18);
		    T1u = T1s - T1t;
		    T1M = T1t + T1s;
		    {
			 E T1y, T1G, T1r, T1z;
			 T1y = T1u - T1x;
			 T1G = T1C + T1F;
			 T1r = W[12];
			 T1z = W[13];
			 Ip[WS(rs, 3)] = FNMS(T1z, T1G, T1r * T1y);
			 Im[WS(rs, 3)] = FMA(T1r, T1G, T1z * T1y);
		    }
		    {
			 E T1U, T1W, T1T, T1V;
			 T1U = T1M + T1N;
			 T1W = T1R - T1Q;
			 T1T = W[16];
			 T1V = W[17];
			 Ip[WS(rs, 4)] = FNMS(T1V, T1W, T1T * T1U);
			 Im[WS(rs, 4)] = FMA(T1T, T1W, T1V * T1U);
		    }
		    {
			 E T1I, T1K, T1H, T1J;
			 T1I = T1u + T1x;
			 T1K = T1F - T1C;
			 T1H = W[4];
			 T1J = W[5];
			 Ip[WS(rs, 1)] = FNMS(T1J, T1K, T1H * T1I);
			 Im[WS(rs, 1)] = FMA(T1H, T1K, T1J * T1I);
		    }
		    {
			 E T1O, T1S, T1L, T1P;
			 T1O = T1M - T1N;
			 T1S = T1Q + T1R;
			 T1L = W[0];
			 T1P = W[1];
			 Ip[0] = FNMS(T1P, T1S, T1L * T1O);
			 Im[0] = FMA(T1L, T1S, T1P * T1O);
		    }
	       }
	  }
     }
}

static const tw_instr twinstr[] = {
     {TW_FULL, 1, 10},
     {TW_NEXT, 1, 0}
};

static const hc2c_desc desc = { 10, "hc2cb_10", twinstr, &GENUS, {72, 30, 30, 0} };

void X(codelet_hc2cb_10) (planner *p) {
     X(khc2c_register) (p, hc2cb_10, &desc, HC2C_VIA_RDFT);
}
#endif				/* HAVE_FMA */
