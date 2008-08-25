#ifndef IG_BRAIN_X86_ASSEMBLER_V2
#define IG_BRAIN_X86_ASSEMBLER_V2

#include "brain.x86.assembler.base.hpp"
#include "brain.x86.assembler.emit.hpp"
#include <boost/preprocessor.hpp>
#include <vector>

#define OP( name, preamble, signature ) BRAIN_X86_ASM_EMIT ## signature BOOST_PP_LPAREN() name BOOST_PP_COMMA() preamble BOOST_PP_RPAREN()
#define ARITH_RM_IRM_OP( name, ip0, ip1, immflag, rp0, rp1, rp2, rp3 ) \
	OP( name, ip0, immflag ## _ib(rm8 ) ) \
	OP( name, ip1, immflag ## _id(rm32) ) \
	OP( name, rp0, _r(rm8 ,r8 ) )         \
	OP( name, rp1, _r(rm16,r16) )         \
	OP( name, rp1, _r(rm32,r32) )         \
	OP( name, rp2, _r(r8  ,m8 ) )         \
	OP( name, rp3, _r(r16 ,m16) )         \
	OP( name, rp3, _r(r32 ,m32) )         \
/*---------------------------------------*/
#define ARITH_RM_IR_OP( name, ip0, ip1, immflag, rp0, rp1 ) \
	OP( name, ip0, immflag ## _ib(rm8 ) ) \
	OP( name, ip1, immflag ## _id(rm32) ) \
	OP( name, rp0, _r(rm8 ,r8 ) )         \
	OP( name, rp1, _r(rm16,r16) )         \
	OP( name, rp1, _r(rm32,r32) )         \
/*---------------------------------------*/
#define ARITH_IRM_OP( name, dp0, dp1, dpflag ) \
	OP( name, dp0, dpflag(rm8 ) ) \
	OP( name, dp1, dpflag(rm16) ) \
	OP( name, dp1, dpflag(rm32) ) \
/*-------------------------------*/

namespace brain {
	namespace x86 {
		class assembler : public assembler_base {
		public:
			ARITH_RM_IRM_OP( adc     , (80), (81), _2, (10), (11), (12), (13) )
			ARITH_RM_IRM_OP( add     , (80), (81), _0, (00), (01), (02), (03) )
			ARITH_RM_IRM_OP( and     , (80), (81), _4, (20), (21), (22), (23) )
			ARITH_RM_IRM_OP( cmp     , (80), (81), _7, (38), (39), (3A), (3B) )
			ARITH_IRM_OP(    dec     , (FE), (FF), _1 )
			ARITH_IRM_OP(    div     , (F6), (F7), _6 )
			// m32fp, m64fp, m80fp
			// OP( fadd , (D8)    , _0(m32fp)  )
			// OP( fadd , (DC)    , _0(m64fp)  )
			// OP( fadd , (D8)    , C0+i       ) // FADD ST(0), ST(i)
			// OP( fadd , (DC)    , C0+i       ) // FADD ST(i), ST(0)
			// OP( faddp, (DE)    , C0+i       ) // FADDP ST(i), ST(0)
			// OP( faddp, (DE)(C1), nil        ) // FADDP
			// OP( fiadd, (DA)    , _0(m32int) ) // FIADD m32int
			// OP( fiadd, (DE)    , _0(m16int) ) // FIADD m16int
			ARITH_IRM_OP(    idiv    , (F6), (F7), _7 )
			ARITH_IRM_OP(    imul    , (F6), (F7), _5 )
			//OP(              imul    , (6B), _r_ib(r32,rm32,imm8 ) )
			//OP(              imul    , (69), _r_id(r32,rm32,imm32) )
			ARITH_RM_IRM_OP( mov     , (C6), (C7), _0, (88), (89), (8A), (8B) )
			ARITH_IRM_OP(    mul     , (F6), (F7), _4 )
			OP(              nop     , (90), nil )
			ARITH_IRM_OP(    not     , (F6), (F7), _2 )
			ARITH_RM_IRM_OP( or      , (80), (81), _1, (08), (09), (0A), (0B) )
			OP(              pop     , (8F), _0(rm16) )
			OP(              pop     , (8F), _0(rm32) )
			OP(              popa    , (61), nil )
			OP(              popad   , (61), nil )
			OP(              push    , (FF), _6(rm16) )
			OP(              push    , (FF), _6(rm32) )
			OP(              push    , (6A), ib(imm8) )
			OP(              push    , (68), id(imm32) )
			OP(              pusha   , (60), nil )
			OP(              pushad  , (60), nil )
			OP(              ret_near, (C3), nil       )
			OP(              ret_near, (C2), iw(imm16) )
			OP(              ret_far , (CB), nil       )
			OP(              ret_far , (CA), iw(imm16) )
			OP(              ret     , (C3), nil       )
			OP(              ret     , (C2), iw(imm16) )
			OP(              syscall , (0F)(05), nil )
			OP(              sysenter, (0F)(34), nil )
			OP(              sysexit , (0F)(35), nil )
			OP(              sysret  , (0F)(07), nil )
			ARITH_RM_IR_OP(  test    , (F6), (F7), _0, (84), (85) )
			ARITH_RM_IRM_OP( sub     , (80), (81), _5, (28), (29), (2A), (2B) )
			ARITH_RM_IRM_OP( xor     , (80), (81), _6, (30), (31), (32), (33) )
		};
	}
}

#endif //ndef IG_BRAIN_X86_ASSEMBLER_V2
