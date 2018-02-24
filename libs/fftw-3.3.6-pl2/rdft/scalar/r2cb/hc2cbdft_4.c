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
/* Generated on Fri Jan 27 16:13:29 EST 2017 */

#include "codelet-rdft.h"

#ifdef HAVE_FMA

/* Generated by: ../../../genfft/gen_hc2cdft.native -fma -reorder-insns -schedule-for-pipeline -compact -variables 4 -pipeline-latency 4 -sign 1 -n 4 -dif -name hc2cbdft_4 -include hc2cb.h */

/*
 * This function contains 30 FP additions, 12 FP multiplications,
 * (or, 24 additions, 6 multiplications, 6 fused multiply/add),
 * 35 stack variables, 0 constants, and 16 memory accesses
 */
#include "hc2cb.h"

static void hc2cbdft_4(R *Rp, R *Ip, R *Rm, R *Im, const R *W, stride rs, INT mb, INT me, INT ms)
{
     {
	  INT m;
	  for (m = mb, W = W + ((mb - 1) * 6); m < me; m = m + 1, Rp = Rp + ms, Ip = Ip + ms, Rm = Rm - ms, Im = Im - ms, W = W + 6, MAKE_VOLATILE_STRIDE(16, rs)) {
	       E Ty, TB, Tw, TE, TA, TF, Tz, TG, TC;
	       {
		    E T4, Tg, T3, Tm, Tc, T5, Th, Ti;
		    {
			 E T1, T2, Ta, Tb;
			 T1 = Rp[0];
			 T2 = Rm[WS(rs, 1)];
			 Ta = Ip[0];
			 Tb = Im[WS(rs, 1)];
			 T4 = Rp[WS(rs, 1)];
			 Tg = T1 - T2;
			 T3 = T1 + T2;
			 Tm = Ta - Tb;
			 Tc = Ta + Tb;
			 T5 = Rm[0];
			 Th = Ip[WS(rs, 1)];
			 Ti = Im[0];
		    }
		    {
			 E T8, Td, T7, Ts, To, Tv, Tk, Te, Tf;
			 T8 = W[0];
			 {
			      E T9, T6, Tn, Tj;
			      T9 = T4 - T5;
			      T6 = T4 + T5;
			      Tn = Th - Ti;
			      Tj = Th + Ti;
			      Ty = Tc - T9;
			      Td = T9 + Tc;
			      T7 = T3 + T6;
			      Ts = T3 - T6;
			      To = Tm + Tn;
			      Tv = Tm - Tn;
			      TB = Tg + Tj;
			      Tk = Tg - Tj;
			      Te = T8 * Td;
			 }
			 Tf = W[1];
			 {
			      E Tr, Tu, Tt, TD, Tx, Tp, Tl, Tq;
			      Tr = W[2];
			      Tp = T8 * Tk;
			      Tu = W[3];
			      Tl = FMA(Tf, Tk, Te);
			      Tt = Tr * Ts;
			      Tq = FNMS(Tf, Td, Tp);
			      TD = Tu * Ts;
			      Rm[0] = T7 + Tl;
			      Rp[0] = T7 - Tl;
			      Im[0] = Tq - To;
			      Ip[0] = To + Tq;
			      Tx = W[4];
			      Tw = FNMS(Tu, Tv, Tt);
			      TE = FMA(Tr, Tv, TD);
			      TA = W[5];
			      TF = Tx * TB;
			      Tz = Tx * Ty;
			 }
		    }
	       }
	       TG = FNMS(TA, Ty, TF);
	       TC = FMA(TA, TB, Tz);
	       Im[WS(rs, 1)] = TG - TE;
	       Ip[WS(rs, 1)] = TE + TG;
	       Rm[WS(rs, 1)] = Tw + TC;
	       Rp[WS(rs, 1)] = Tw - TC;
	  }
     }
}

static const tw_instr twinstr[] = {
     {TW_FULL, 1, 4},
     {TW_NEXT, 1, 0}
};

static const hc2c_desc desc = { 4, "hc2cbdft_4", twinstr, &GENUS, {24, 6, 6, 0} };

void X(codelet_hc2cbdft_4) (planner *p) {
     X(khc2c_register) (p, hc2cbdft_4, &desc, HC2C_VIA_DFT);
}
#else				/* HAVE_FMA */

/* Generated by: ../../../genfft/gen_hc2cdft.native -compact -variables 4 -pipeline-latency 4 -sign 1 -n 4 -dif -name hc2cbdft_4 -include hc2cb.h */

/*
 * This function contains 30 FP additions, 12 FP multiplications,
 * (or, 24 additions, 6 multiplications, 6 fused multiply/add),
 * 19 stack variables, 0 constants, and 16 memory accesses
 */
#include "hc2cb.h"

static void hc2cbdft_4(R *Rp, R *Ip, R *Rm, R *Im, const R *W, stride rs, INT mb, INT me, INT ms)
{
     {
	  INT m;
	  for (m = mb, W = W + ((mb - 1) * 6); m < me; m = m + 1, Rp = Rp + ms, Ip = Ip + ms, Rm = Rm - ms, Im = Im - ms, W = W + 6, MAKE_VOLATILE_STRIDE(16, rs)) {
	       E T3, Tl, T6, Tm, Td, Tj, Tx, Tv, Ts, Tq;
	       {
		    E Tf, Tc, T9, Ti;
		    {
			 E T1, T2, Ta, Tb;
			 T1 = Rp[0];
			 T2 = Rm[WS(rs, 1)];
			 T3 = T1 + T2;
			 Tf = T1 - T2;
			 Ta = Ip[0];
			 Tb = Im[WS(rs, 1)];
			 Tc = Ta + Tb;
			 Tl = Ta - Tb;
		    }
		    {
			 E T4, T5, Tg, Th;
			 T4 = Rp[WS(rs, 1)];
			 T5 = Rm[0];
			 T6 = T4 + T5;
			 T9 = T4 - T5;
			 Tg = Ip[WS(rs, 1)];
			 Th = Im[0];
			 Ti = Tg + Th;
			 Tm = Tg - Th;
		    }
		    Td = T9 + Tc;
		    Tj = Tf - Ti;
		    Tx = Tf + Ti;
		    Tv = Tc - T9;
		    Ts = Tl - Tm;
		    Tq = T3 - T6;
	       }
	       {
		    E T7, Tn, Tk, To, T8, Te;
		    T7 = T3 + T6;
		    Tn = Tl + Tm;
		    T8 = W[0];
		    Te = W[1];
		    Tk = FMA(T8, Td, Te * Tj);
		    To = FNMS(Te, Td, T8 * Tj);
		    Rp[0] = T7 - Tk;
		    Ip[0] = Tn + To;
		    Rm[0] = T7 + Tk;
		    Im[0] = To - Tn;
	       }
	       {
		    E Tt, Tz, Ty, TA;
		    {
			 E Tp, Tr, Tu, Tw;
			 Tp = W[2];
			 Tr = W[3];
			 Tt = FNMS(Tr, Ts, Tp * Tq);
			 Tz = FMA(Tr, Tq, Tp * Ts);
			 Tu = W[4];
			 Tw = W[5];
			 Ty = FMA(Tu, Tv, Tw * Tx);
			 TA = FNMS(Tw, Tv, Tu * Tx);
		    }
		    Rp[WS(rs, 1)] = Tt - Ty;
		    Ip[WS(rs, 1)] = Tz + TA;
		    Rm[WS(rs, 1)] = Tt + Ty;
		    Im[WS(rs, 1)] = TA - Tz;
	       }
	  }
     }
}

static const tw_instr twinstr[] = {
     {TW_FULL, 1, 4},
     {TW_NEXT, 1, 0}
};

static const hc2c_desc desc = { 4, "hc2cbdft_4", twinstr, &GENUS, {24, 6, 6, 0} };

void X(codelet_hc2cbdft_4) (planner *p) {
     X(khc2c_register) (p, hc2cbdft_4, &desc, HC2C_VIA_DFT);
}
#endif				/* HAVE_FMA */
