#ifndef IG_BRAIN_X86_ASSEMBLER_V2_EMIT
#define IG_BRAIN_X86_ASSEMBLER_V2_EMIT

#include "brain.x86.registers.hpp"
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor.hpp>

#define DO_BRAIN_X86_ASM_EMIT( z, unused, hex ) emit(BOOST_PP_CAT(0x,hex));
#define BRAIN_X86_ASM_EMIT( hex_seq ) BOOST_PP_SEQ_FOR_EACH( DO_BRAIN_X86_ASM_EMIT, ~, hex_seq )

#define BRAIN_X86_ASM_EMITnil( name, preamble ) assembler& name () { BRAIN_X86_ASM_EMIT(preamble); return *this; }

#define BRAIN_X86_ASM_EMITib(type) BRAIN_X86_ASM_EMITib_ ## type
#define BRAIN_X86_ASM_EMITiw(type) BRAIN_X86_ASM_EMITiw_ ## type
#define BRAIN_X86_ASM_EMITid(type) BRAIN_X86_ASM_EMITid_ ## type
#define BRAIN_X86_ASM_EMITib_imm8(  name, preamble ) assembler& name ( imm8  a ) { BRAIN_X86_ASM_EMIT(preamble); ib(a); return *this; }
#define BRAIN_X86_ASM_EMITiw_imm16( name, preamble ) assembler& name ( imm16 a ) { BRAIN_X86_ASM_EMIT(preamble); iw(a); return *this; }
#define BRAIN_X86_ASM_EMITid_imm32( name, preamble ) assembler& name ( imm32 a ) { BRAIN_X86_ASM_EMIT(preamble); id(a); return *this; }

#define BRAIN_X86_ASM_EMIT_r(a,b) BRAIN_X86_ASM_EMIT_r_ ## a ## _ ## b

#define BRAIN_X86_ASM_EMIT_r_r8_r8(   name, preamble, op ) assembler& name ( r8  a, r8  b ) { BRAIN_X86_ASM_EMIT(preamble); op(a,b); return *this; }
#define BRAIN_X86_ASM_EMIT_r_r8_r16(  name, preamble, op ) assembler& name ( r8  a, r16 b ) { BRAIN_X86_ASM_EMIT(preamble); op(a,b); return *this; }
#define BRAIN_X86_ASM_EMIT_r_r8_r32(  name, preamble, op ) assembler& name ( r8  a, r32 b ) { BRAIN_X86_ASM_EMIT(preamble); op(a,b); return *this; }
#define BRAIN_X86_ASM_EMIT_r_r16_r8(  name, preamble, op ) assembler& name ( r16 a, r8  b ) { BRAIN_X86_ASM_EMIT(preamble); op(a,b); return *this; }
#define BRAIN_X86_ASM_EMIT_r_r16_r16( name, preamble, op ) assembler& name ( r16 a, r16 b ) { BRAIN_X86_ASM_EMIT(preamble); op(a,b); return *this; }
#define BRAIN_X86_ASM_EMIT_r_r16_r32( name, preamble, op ) assembler& name ( r16 a, r32 b ) { BRAIN_X86_ASM_EMIT(preamble); op(a,b); return *this; }
#define BRAIN_X86_ASM_EMIT_r_r32_r8(  name, preamble, op ) assembler& name ( r32 a, r8  b ) { BRAIN_X86_ASM_EMIT(preamble); op(a,b); return *this; }
#define BRAIN_X86_ASM_EMIT_r_r32_r16( name, preamble, op ) assembler& name ( r32 a, r16 b ) { BRAIN_X86_ASM_EMIT(preamble); op(a,b); return *this; }
#define BRAIN_X86_ASM_EMIT_r_r32_r32( name, preamble, op ) assembler& name ( r32 a, r32 b ) { BRAIN_X86_ASM_EMIT(preamble); op(a,b); return *this; }

#define BRAIN_X86_ASM_EMIT_r_m8_r8(   name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& lhs, r8  rhs ) { BRAIN_X86_ASM_EMIT(preamble); _r_rm_r(lhs,rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_r_m8_r16(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& lhs, r16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _r_rm_r(lhs,rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_r_m8_r32(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& lhs, r32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _r_rm_r(lhs,rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_r_m16_r8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& lhs, r8  rhs ) { BRAIN_X86_ASM_EMIT(preamble); _r_rm_r(lhs,rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_r_m16_r16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& lhs, r16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _r_rm_r(lhs,rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_r_m16_r32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& lhs, r32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _r_rm_r(lhs,rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_r_m32_r8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& lhs, r8  rhs ) { BRAIN_X86_ASM_EMIT(preamble); _r_rm_r(lhs,rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_r_m32_r16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& lhs, r16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _r_rm_r(lhs,rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_r_m32_r32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& lhs, r32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _r_rm_r(lhs,rhs); return *this; }

#define BRAIN_X86_ASM_EMIT_r_r8_m8(   name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( r8  lhs, const ExprTT<R,8 >& rhs ) { BRAIN_X86_ASM_EMIT(preamble); _r_r_rm(lhs,rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_r_r8_m16(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( r8  lhs, const ExprTT<R,16>& rhs ) { BRAIN_X86_ASM_EMIT(preamble); _r_r_rm(lhs,rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_r_r8_m32(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( r8  lhs, const ExprTT<R,32>& rhs ) { BRAIN_X86_ASM_EMIT(preamble); _r_r_rm(lhs,rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_r_r16_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( r16 lhs, const ExprTT<R,8 >& rhs ) { BRAIN_X86_ASM_EMIT(preamble); _r_r_rm(lhs,rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_r_r16_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( r16 lhs, const ExprTT<R,16>& rhs ) { BRAIN_X86_ASM_EMIT(preamble); _r_r_rm(lhs,rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_r_r16_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( r16 lhs, const ExprTT<R,32>& rhs ) { BRAIN_X86_ASM_EMIT(preamble); _r_r_rm(lhs,rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_r_r32_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( r32 lhs, const ExprTT<R,8 >& rhs ) { BRAIN_X86_ASM_EMIT(preamble); _r_r_rm(lhs,rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_r_r32_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( r32 lhs, const ExprTT<R,16>& rhs ) { BRAIN_X86_ASM_EMIT(preamble); _r_r_rm(lhs,rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_r_r32_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( r32 lhs, const ExprTT<R,32>& rhs ) { BRAIN_X86_ASM_EMIT(preamble); _r_r_rm(lhs,rhs); return *this; }

#define BRAIN_X86_ASM_EMIT_r_rm8_r8(   name, preamble ) BRAIN_X86_ASM_EMIT_r_r8_r8(   name, preamble, _r_rm_r ) BRAIN_X86_ASM_EMIT_r_m8_r8(   name, preamble )
#define BRAIN_X86_ASM_EMIT_r_rm8_r16(  name, preamble ) BRAIN_X86_ASM_EMIT_r_r8_r16(  name, preamble, _r_rm_r ) BRAIN_X86_ASM_EMIT_r_m8_r16(  name, preamble )
#define BRAIN_X86_ASM_EMIT_r_rm8_r32(  name, preamble ) BRAIN_X86_ASM_EMIT_r_r8_r32(  name, preamble, _r_rm_r ) BRAIN_X86_ASM_EMIT_r_m8_r32(  name, preamble )
#define BRAIN_X86_ASM_EMIT_r_rm16_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_r_r16_r8(  name, preamble, _r_rm_r ) BRAIN_X86_ASM_EMIT_r_m16_r8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_r_rm16_r16( name, preamble ) BRAIN_X86_ASM_EMIT_r_r16_r16( name, preamble, _r_rm_r ) BRAIN_X86_ASM_EMIT_r_m16_r16( name, preamble )
#define BRAIN_X86_ASM_EMIT_r_rm16_r32( name, preamble ) BRAIN_X86_ASM_EMIT_r_r16_r32( name, preamble, _r_rm_r ) BRAIN_X86_ASM_EMIT_r_m16_r32( name, preamble )
#define BRAIN_X86_ASM_EMIT_r_rm32_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_r_r32_r8(  name, preamble, _r_rm_r ) BRAIN_X86_ASM_EMIT_r_m32_r8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_r_rm32_r16( name, preamble ) BRAIN_X86_ASM_EMIT_r_r32_r16( name, preamble, _r_rm_r ) BRAIN_X86_ASM_EMIT_r_m32_r16( name, preamble )
#define BRAIN_X86_ASM_EMIT_r_rm32_r32( name, preamble ) BRAIN_X86_ASM_EMIT_r_r32_r32( name, preamble, _r_rm_r ) BRAIN_X86_ASM_EMIT_r_m32_r32( name, preamble )

#define BRAIN_X86_ASM_EMIT_r_r8_rm8(   name, preamble ) BRAIN_X86_ASM_EMIT_r_r8_r8(   name, preamble, _r_r_rm ) BRAIN_X86_ASM_EMIT_r_m8_r8(   name, preamble )
#define BRAIN_X86_ASM_EMIT_r_r8_rm16(  name, preamble ) BRAIN_X86_ASM_EMIT_r_r8_r16(  name, preamble, _r_r_rm ) BRAIN_X86_ASM_EMIT_r_m8_r16(  name, preamble )
#define BRAIN_X86_ASM_EMIT_r_r8_rm32(  name, preamble ) BRAIN_X86_ASM_EMIT_r_r8_r32(  name, preamble, _r_r_rm ) BRAIN_X86_ASM_EMIT_r_m8_r32(  name, preamble )
#define BRAIN_X86_ASM_EMIT_r_r16_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_r_r16_r8(  name, preamble, _r_r_rm ) BRAIN_X86_ASM_EMIT_r_m16_r8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_r_r16_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_r_r16_r16( name, preamble, _r_r_rm ) BRAIN_X86_ASM_EMIT_r_m16_r16( name, preamble )
#define BRAIN_X86_ASM_EMIT_r_r16_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_r_r16_r32( name, preamble, _r_r_rm ) BRAIN_X86_ASM_EMIT_r_m16_r32( name, preamble )
#define BRAIN_X86_ASM_EMIT_r_r32_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_r_r32_r8(  name, preamble, _r_r_rm ) BRAIN_X86_ASM_EMIT_r_m32_r8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_r_r32_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_r_r32_r16( name, preamble, _r_r_rm ) BRAIN_X86_ASM_EMIT_r_m32_r16( name, preamble )
#define BRAIN_X86_ASM_EMIT_r_r32_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_r_r32_r32( name, preamble, _r_r_rm ) BRAIN_X86_ASM_EMIT_r_m32_r32( name, preamble )

#define BRAIN_X86_ASM_EMIT_0(a) BRAIN_X86_ASM_EMIT_0_ ## a
#define BRAIN_X86_ASM_EMIT_1(a) BRAIN_X86_ASM_EMIT_1_ ## a
#define BRAIN_X86_ASM_EMIT_2(a) BRAIN_X86_ASM_EMIT_2_ ## a
#define BRAIN_X86_ASM_EMIT_3(a) BRAIN_X86_ASM_EMIT_3_ ## a
#define BRAIN_X86_ASM_EMIT_4(a) BRAIN_X86_ASM_EMIT_4_ ## a
#define BRAIN_X86_ASM_EMIT_5(a) BRAIN_X86_ASM_EMIT_5_ ## a
#define BRAIN_X86_ASM_EMIT_6(a) BRAIN_X86_ASM_EMIT_6_ ## a
#define BRAIN_X86_ASM_EMIT_7(a) BRAIN_X86_ASM_EMIT_7_ ## a

#define BRAIN_X86_ASM_EMIT_0_ib(a) BRAIN_X86_ASM_EMIT_0_ib_ ## a
#define BRAIN_X86_ASM_EMIT_1_ib(a) BRAIN_X86_ASM_EMIT_1_ib_ ## a
#define BRAIN_X86_ASM_EMIT_2_ib(a) BRAIN_X86_ASM_EMIT_2_ib_ ## a
#define BRAIN_X86_ASM_EMIT_3_ib(a) BRAIN_X86_ASM_EMIT_3_ib_ ## a
#define BRAIN_X86_ASM_EMIT_4_ib(a) BRAIN_X86_ASM_EMIT_4_ib_ ## a
#define BRAIN_X86_ASM_EMIT_5_ib(a) BRAIN_X86_ASM_EMIT_5_ib_ ## a
#define BRAIN_X86_ASM_EMIT_6_ib(a) BRAIN_X86_ASM_EMIT_6_ib_ ## a
#define BRAIN_X86_ASM_EMIT_7_ib(a) BRAIN_X86_ASM_EMIT_7_ib_ ## a

#define BRAIN_X86_ASM_EMIT_0_iw(a) BRAIN_X86_ASM_EMIT_0_iw_ ## a
#define BRAIN_X86_ASM_EMIT_1_iw(a) BRAIN_X86_ASM_EMIT_1_iw_ ## a
#define BRAIN_X86_ASM_EMIT_2_iw(a) BRAIN_X86_ASM_EMIT_2_iw_ ## a
#define BRAIN_X86_ASM_EMIT_3_iw(a) BRAIN_X86_ASM_EMIT_3_iw_ ## a
#define BRAIN_X86_ASM_EMIT_4_iw(a) BRAIN_X86_ASM_EMIT_4_iw_ ## a
#define BRAIN_X86_ASM_EMIT_5_iw(a) BRAIN_X86_ASM_EMIT_5_iw_ ## a
#define BRAIN_X86_ASM_EMIT_6_iw(a) BRAIN_X86_ASM_EMIT_6_iw_ ## a
#define BRAIN_X86_ASM_EMIT_7_iw(a) BRAIN_X86_ASM_EMIT_7_iw_ ## a

#define BRAIN_X86_ASM_EMIT_0_id(a) BRAIN_X86_ASM_EMIT_0_id_ ## a
#define BRAIN_X86_ASM_EMIT_1_id(a) BRAIN_X86_ASM_EMIT_1_id_ ## a
#define BRAIN_X86_ASM_EMIT_2_id(a) BRAIN_X86_ASM_EMIT_2_id_ ## a
#define BRAIN_X86_ASM_EMIT_3_id(a) BRAIN_X86_ASM_EMIT_3_id_ ## a
#define BRAIN_X86_ASM_EMIT_4_id(a) BRAIN_X86_ASM_EMIT_4_id_ ## a
#define BRAIN_X86_ASM_EMIT_5_id(a) BRAIN_X86_ASM_EMIT_5_id_ ## a
#define BRAIN_X86_ASM_EMIT_6_id(a) BRAIN_X86_ASM_EMIT_6_id_ ## a
#define BRAIN_X86_ASM_EMIT_7_id(a) BRAIN_X86_ASM_EMIT_7_id_ ## a

#define BRAIN_X86_ASM_EMIT_0_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_0_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_0_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_0_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_0_r16( name, preamble ) BRAIN_X86_ASM_EMIT_0_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_0_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_0_r32( name, preamble ) BRAIN_X86_ASM_EMIT_0_m32( name, preamble )
#define BRAIN_X86_ASM_EMIT_1_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_1_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_1_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_1_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_1_r16( name, preamble ) BRAIN_X86_ASM_EMIT_1_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_1_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_1_r32( name, preamble ) BRAIN_X86_ASM_EMIT_1_m32( name, preamble )
#define BRAIN_X86_ASM_EMIT_2_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_2_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_2_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_2_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_2_r16( name, preamble ) BRAIN_X86_ASM_EMIT_2_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_2_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_2_r32( name, preamble ) BRAIN_X86_ASM_EMIT_2_m32( name, preamble )
#define BRAIN_X86_ASM_EMIT_3_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_3_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_3_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_3_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_3_r16( name, preamble ) BRAIN_X86_ASM_EMIT_3_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_3_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_3_r32( name, preamble ) BRAIN_X86_ASM_EMIT_3_m32( name, preamble )
#define BRAIN_X86_ASM_EMIT_4_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_4_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_4_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_4_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_4_r16( name, preamble ) BRAIN_X86_ASM_EMIT_4_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_4_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_4_r32( name, preamble ) BRAIN_X86_ASM_EMIT_4_m32( name, preamble )
#define BRAIN_X86_ASM_EMIT_5_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_5_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_5_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_5_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_5_r16( name, preamble ) BRAIN_X86_ASM_EMIT_5_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_5_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_5_r32( name, preamble ) BRAIN_X86_ASM_EMIT_5_m32( name, preamble )
#define BRAIN_X86_ASM_EMIT_6_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_6_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_6_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_6_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_6_r16( name, preamble ) BRAIN_X86_ASM_EMIT_6_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_6_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_6_r32( name, preamble ) BRAIN_X86_ASM_EMIT_6_m32( name, preamble )
#define BRAIN_X86_ASM_EMIT_7_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_7_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_7_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_7_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_7_r16( name, preamble ) BRAIN_X86_ASM_EMIT_7_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_7_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_7_r32( name, preamble ) BRAIN_X86_ASM_EMIT_7_m32( name, preamble )

#define BRAIN_X86_ASM_EMIT_0_ib_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_0_ib_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_0_ib_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_0_ib_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_0_ib_r16( name, preamble ) BRAIN_X86_ASM_EMIT_0_ib_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_0_ib_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_0_ib_r32( name, preamble ) BRAIN_X86_ASM_EMIT_0_ib_m32( name, preamble )
#define BRAIN_X86_ASM_EMIT_1_ib_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_1_ib_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_1_ib_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_1_ib_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_1_ib_r16( name, preamble ) BRAIN_X86_ASM_EMIT_1_ib_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_1_ib_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_1_ib_r32( name, preamble ) BRAIN_X86_ASM_EMIT_1_ib_m32( name, preamble )
#define BRAIN_X86_ASM_EMIT_2_ib_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_2_ib_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_2_ib_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_2_ib_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_2_ib_r16( name, preamble ) BRAIN_X86_ASM_EMIT_2_ib_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_2_ib_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_2_ib_r32( name, preamble ) BRAIN_X86_ASM_EMIT_2_ib_m32( name, preamble )
#define BRAIN_X86_ASM_EMIT_3_ib_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_3_ib_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_3_ib_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_3_ib_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_3_ib_r16( name, preamble ) BRAIN_X86_ASM_EMIT_3_ib_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_3_ib_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_3_ib_r32( name, preamble ) BRAIN_X86_ASM_EMIT_3_ib_m32( name, preamble )
#define BRAIN_X86_ASM_EMIT_4_ib_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_4_ib_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_4_ib_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_4_ib_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_4_ib_r16( name, preamble ) BRAIN_X86_ASM_EMIT_4_ib_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_4_ib_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_4_ib_r32( name, preamble ) BRAIN_X86_ASM_EMIT_4_ib_m32( name, preamble )
#define BRAIN_X86_ASM_EMIT_5_ib_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_5_ib_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_5_ib_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_5_ib_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_5_ib_r16( name, preamble ) BRAIN_X86_ASM_EMIT_5_ib_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_5_ib_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_5_ib_r32( name, preamble ) BRAIN_X86_ASM_EMIT_5_ib_m32( name, preamble )
#define BRAIN_X86_ASM_EMIT_6_ib_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_6_ib_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_6_ib_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_6_ib_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_6_ib_r16( name, preamble ) BRAIN_X86_ASM_EMIT_6_ib_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_6_ib_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_6_ib_r32( name, preamble ) BRAIN_X86_ASM_EMIT_6_ib_m32( name, preamble )
#define BRAIN_X86_ASM_EMIT_7_ib_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_7_ib_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_7_ib_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_7_ib_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_7_ib_r16( name, preamble ) BRAIN_X86_ASM_EMIT_7_ib_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_7_ib_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_7_ib_r32( name, preamble ) BRAIN_X86_ASM_EMIT_7_ib_m32( name, preamble )

#define BRAIN_X86_ASM_EMIT_0_iw_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_0_iw_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_0_iw_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_0_iw_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_0_iw_r16( name, preamble ) BRAIN_X86_ASM_EMIT_0_iw_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_0_iw_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_0_iw_r32( name, preamble ) BRAIN_X86_ASM_EMIT_0_iw_m32( name, preamble )
#define BRAIN_X86_ASM_EMIT_1_iw_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_1_iw_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_1_iw_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_1_iw_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_1_iw_r16( name, preamble ) BRAIN_X86_ASM_EMIT_1_iw_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_1_iw_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_1_iw_r32( name, preamble ) BRAIN_X86_ASM_EMIT_1_iw_m32( name, preamble )
#define BRAIN_X86_ASM_EMIT_2_iw_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_2_iw_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_2_iw_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_2_iw_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_2_iw_r16( name, preamble ) BRAIN_X86_ASM_EMIT_2_iw_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_2_iw_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_2_iw_r32( name, preamble ) BRAIN_X86_ASM_EMIT_2_iw_m32( name, preamble )
#define BRAIN_X86_ASM_EMIT_3_iw_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_3_iw_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_3_iw_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_3_iw_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_3_iw_r16( name, preamble ) BRAIN_X86_ASM_EMIT_3_iw_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_3_iw_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_3_iw_r32( name, preamble ) BRAIN_X86_ASM_EMIT_3_iw_m32( name, preamble )
#define BRAIN_X86_ASM_EMIT_4_iw_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_4_iw_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_4_iw_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_4_iw_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_4_iw_r16( name, preamble ) BRAIN_X86_ASM_EMIT_4_iw_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_4_iw_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_4_iw_r32( name, preamble ) BRAIN_X86_ASM_EMIT_4_iw_m32( name, preamble )
#define BRAIN_X86_ASM_EMIT_5_iw_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_5_iw_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_5_iw_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_5_iw_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_5_iw_r16( name, preamble ) BRAIN_X86_ASM_EMIT_5_iw_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_5_iw_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_5_iw_r32( name, preamble ) BRAIN_X86_ASM_EMIT_5_iw_m32( name, preamble )
#define BRAIN_X86_ASM_EMIT_6_iw_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_6_iw_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_6_iw_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_6_iw_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_6_iw_r16( name, preamble ) BRAIN_X86_ASM_EMIT_6_iw_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_6_iw_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_6_iw_r32( name, preamble ) BRAIN_X86_ASM_EMIT_6_iw_m32( name, preamble )
#define BRAIN_X86_ASM_EMIT_7_iw_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_7_iw_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_7_iw_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_7_iw_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_7_iw_r16( name, preamble ) BRAIN_X86_ASM_EMIT_7_iw_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_7_iw_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_7_iw_r32( name, preamble ) BRAIN_X86_ASM_EMIT_7_iw_m32( name, preamble )

#define BRAIN_X86_ASM_EMIT_0_id_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_0_id_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_0_id_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_0_id_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_0_id_r16( name, preamble ) BRAIN_X86_ASM_EMIT_0_id_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_0_id_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_0_id_r32( name, preamble ) BRAIN_X86_ASM_EMIT_0_id_m32( name, preamble )
#define BRAIN_X86_ASM_EMIT_1_id_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_1_id_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_1_id_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_1_id_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_1_id_r16( name, preamble ) BRAIN_X86_ASM_EMIT_1_id_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_1_id_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_1_id_r32( name, preamble ) BRAIN_X86_ASM_EMIT_1_id_m32( name, preamble )
#define BRAIN_X86_ASM_EMIT_2_id_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_2_id_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_2_id_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_2_id_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_2_id_r16( name, preamble ) BRAIN_X86_ASM_EMIT_2_id_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_2_id_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_2_id_r32( name, preamble ) BRAIN_X86_ASM_EMIT_2_id_m32( name, preamble )
#define BRAIN_X86_ASM_EMIT_3_id_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_3_id_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_3_id_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_3_id_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_3_id_r16( name, preamble ) BRAIN_X86_ASM_EMIT_3_id_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_3_id_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_3_id_r32( name, preamble ) BRAIN_X86_ASM_EMIT_3_id_m32( name, preamble )
#define BRAIN_X86_ASM_EMIT_4_id_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_4_id_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_4_id_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_4_id_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_4_id_r16( name, preamble ) BRAIN_X86_ASM_EMIT_4_id_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_4_id_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_4_id_r32( name, preamble ) BRAIN_X86_ASM_EMIT_4_id_m32( name, preamble )
#define BRAIN_X86_ASM_EMIT_5_id_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_5_id_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_5_id_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_5_id_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_5_id_r16( name, preamble ) BRAIN_X86_ASM_EMIT_5_id_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_5_id_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_5_id_r32( name, preamble ) BRAIN_X86_ASM_EMIT_5_id_m32( name, preamble )
#define BRAIN_X86_ASM_EMIT_6_id_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_6_id_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_6_id_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_6_id_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_6_id_r16( name, preamble ) BRAIN_X86_ASM_EMIT_6_id_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_6_id_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_6_id_r32( name, preamble ) BRAIN_X86_ASM_EMIT_6_id_m32( name, preamble )
#define BRAIN_X86_ASM_EMIT_7_id_rm8(  name, preamble ) BRAIN_X86_ASM_EMIT_7_id_r8(  name, preamble ) BRAIN_X86_ASM_EMIT_7_id_m8(  name, preamble )
#define BRAIN_X86_ASM_EMIT_7_id_rm16( name, preamble ) BRAIN_X86_ASM_EMIT_7_id_r16( name, preamble ) BRAIN_X86_ASM_EMIT_7_id_m16( name, preamble )
#define BRAIN_X86_ASM_EMIT_7_id_rm32( name, preamble ) BRAIN_X86_ASM_EMIT_7_id_r32( name, preamble ) BRAIN_X86_ASM_EMIT_7_id_m32( name, preamble )

#define BRAIN_X86_ASM_EMIT_0_r8(  name, preamble ) assembler& name( r8  a ) { BRAIN_X86_ASM_EMIT(preamble); _N(0,a); return *this; }
#define BRAIN_X86_ASM_EMIT_0_r16( name, preamble ) assembler& name( r16 a ) { BRAIN_X86_ASM_EMIT(preamble); _N(0,a); return *this; }
#define BRAIN_X86_ASM_EMIT_0_r32( name, preamble ) assembler& name( r32 a ) { BRAIN_X86_ASM_EMIT(preamble); _N(0,a); return *this; }
#define BRAIN_X86_ASM_EMIT_1_r8(  name, preamble ) assembler& name( r8  a ) { BRAIN_X86_ASM_EMIT(preamble); _N(1,a); return *this; }
#define BRAIN_X86_ASM_EMIT_1_r16( name, preamble ) assembler& name( r16 a ) { BRAIN_X86_ASM_EMIT(preamble); _N(1,a); return *this; }
#define BRAIN_X86_ASM_EMIT_1_r32( name, preamble ) assembler& name( r32 a ) { BRAIN_X86_ASM_EMIT(preamble); _N(1,a); return *this; }
#define BRAIN_X86_ASM_EMIT_2_r8(  name, preamble ) assembler& name( r8  a ) { BRAIN_X86_ASM_EMIT(preamble); _N(2,a); return *this; }
#define BRAIN_X86_ASM_EMIT_2_r16( name, preamble ) assembler& name( r16 a ) { BRAIN_X86_ASM_EMIT(preamble); _N(2,a); return *this; }
#define BRAIN_X86_ASM_EMIT_2_r32( name, preamble ) assembler& name( r32 a ) { BRAIN_X86_ASM_EMIT(preamble); _N(2,a); return *this; }
#define BRAIN_X86_ASM_EMIT_3_r8(  name, preamble ) assembler& name( r8  a ) { BRAIN_X86_ASM_EMIT(preamble); _N(3,a); return *this; }
#define BRAIN_X86_ASM_EMIT_3_r16( name, preamble ) assembler& name( r16 a ) { BRAIN_X86_ASM_EMIT(preamble); _N(3,a); return *this; }
#define BRAIN_X86_ASM_EMIT_3_r32( name, preamble ) assembler& name( r32 a ) { BRAIN_X86_ASM_EMIT(preamble); _N(3,a); return *this; }
#define BRAIN_X86_ASM_EMIT_4_r8(  name, preamble ) assembler& name( r8  a ) { BRAIN_X86_ASM_EMIT(preamble); _N(4,a); return *this; }
#define BRAIN_X86_ASM_EMIT_4_r16( name, preamble ) assembler& name( r16 a ) { BRAIN_X86_ASM_EMIT(preamble); _N(4,a); return *this; }
#define BRAIN_X86_ASM_EMIT_4_r32( name, preamble ) assembler& name( r32 a ) { BRAIN_X86_ASM_EMIT(preamble); _N(4,a); return *this; }
#define BRAIN_X86_ASM_EMIT_5_r8(  name, preamble ) assembler& name( r8  a ) { BRAIN_X86_ASM_EMIT(preamble); _N(5,a); return *this; }
#define BRAIN_X86_ASM_EMIT_5_r16( name, preamble ) assembler& name( r16 a ) { BRAIN_X86_ASM_EMIT(preamble); _N(5,a); return *this; }
#define BRAIN_X86_ASM_EMIT_5_r32( name, preamble ) assembler& name( r32 a ) { BRAIN_X86_ASM_EMIT(preamble); _N(5,a); return *this; }
#define BRAIN_X86_ASM_EMIT_6_r8(  name, preamble ) assembler& name( r8  a ) { BRAIN_X86_ASM_EMIT(preamble); _N(6,a); return *this; }
#define BRAIN_X86_ASM_EMIT_6_r16( name, preamble ) assembler& name( r16 a ) { BRAIN_X86_ASM_EMIT(preamble); _N(6,a); return *this; }
#define BRAIN_X86_ASM_EMIT_6_r32( name, preamble ) assembler& name( r32 a ) { BRAIN_X86_ASM_EMIT(preamble); _N(6,a); return *this; }
#define BRAIN_X86_ASM_EMIT_7_r8(  name, preamble ) assembler& name( r8  a ) { BRAIN_X86_ASM_EMIT(preamble); _N(7,a); return *this; }
#define BRAIN_X86_ASM_EMIT_7_r16( name, preamble ) assembler& name( r16 a ) { BRAIN_X86_ASM_EMIT(preamble); _N(7,a); return *this; }
#define BRAIN_X86_ASM_EMIT_7_r32( name, preamble ) assembler& name( r32 a ) { BRAIN_X86_ASM_EMIT(preamble); _N(7,a); return *this; }

#define BRAIN_X86_ASM_EMIT_0_ib_r8(  name, preamble ) assembler& name( r8  lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(0,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_0_ib_r16( name, preamble ) assembler& name( r16 lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(0,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_0_ib_r32( name, preamble ) assembler& name( r32 lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(0,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_1_ib_r8(  name, preamble ) assembler& name( r8  lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(1,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_1_ib_r16( name, preamble ) assembler& name( r16 lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(1,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_1_ib_r32( name, preamble ) assembler& name( r32 lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(1,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_2_ib_r8(  name, preamble ) assembler& name( r8  lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(2,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_2_ib_r16( name, preamble ) assembler& name( r16 lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(2,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_2_ib_r32( name, preamble ) assembler& name( r32 lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(2,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_3_ib_r8(  name, preamble ) assembler& name( r8  lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(3,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_3_ib_r16( name, preamble ) assembler& name( r16 lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(3,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_3_ib_r32( name, preamble ) assembler& name( r32 lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(3,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_4_ib_r8(  name, preamble ) assembler& name( r8  lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(4,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_4_ib_r16( name, preamble ) assembler& name( r16 lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(4,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_4_ib_r32( name, preamble ) assembler& name( r32 lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(4,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_5_ib_r8(  name, preamble ) assembler& name( r8  lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(5,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_5_ib_r16( name, preamble ) assembler& name( r16 lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(5,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_5_ib_r32( name, preamble ) assembler& name( r32 lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(5,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_6_ib_r8(  name, preamble ) assembler& name( r8  lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(6,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_6_ib_r16( name, preamble ) assembler& name( r16 lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(6,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_6_ib_r32( name, preamble ) assembler& name( r32 lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(6,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_7_ib_r8(  name, preamble ) assembler& name( r8  lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(7,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_7_ib_r16( name, preamble ) assembler& name( r16 lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(7,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_7_ib_r32( name, preamble ) assembler& name( r32 lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(7,lhs); ib(rhs); return *this; }

#define BRAIN_X86_ASM_EMIT_0_iw_r8(  name, preamble ) assembler& name( r8  lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(0,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_0_iw_r16( name, preamble ) assembler& name( r16 lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(0,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_0_iw_r32( name, preamble ) assembler& name( r32 lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(0,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_1_iw_r8(  name, preamble ) assembler& name( r8  lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(1,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_1_iw_r16( name, preamble ) assembler& name( r16 lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(1,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_1_iw_r32( name, preamble ) assembler& name( r32 lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(1,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_2_iw_r8(  name, preamble ) assembler& name( r8  lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(2,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_2_iw_r16( name, preamble ) assembler& name( r16 lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(2,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_2_iw_r32( name, preamble ) assembler& name( r32 lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(2,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_3_iw_r8(  name, preamble ) assembler& name( r8  lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(3,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_3_iw_r16( name, preamble ) assembler& name( r16 lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(3,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_3_iw_r32( name, preamble ) assembler& name( r32 lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(3,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_4_iw_r8(  name, preamble ) assembler& name( r8  lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(4,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_4_iw_r16( name, preamble ) assembler& name( r16 lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(4,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_4_iw_r32( name, preamble ) assembler& name( r32 lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(4,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_5_iw_r8(  name, preamble ) assembler& name( r8  lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(5,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_5_iw_r16( name, preamble ) assembler& name( r16 lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(5,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_5_iw_r32( name, preamble ) assembler& name( r32 lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(5,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_6_iw_r8(  name, preamble ) assembler& name( r8  lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(6,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_6_iw_r16( name, preamble ) assembler& name( r16 lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(6,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_6_iw_r32( name, preamble ) assembler& name( r32 lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(6,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_7_iw_r8(  name, preamble ) assembler& name( r8  lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(7,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_7_iw_r16( name, preamble ) assembler& name( r16 lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(7,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_7_iw_r32( name, preamble ) assembler& name( r32 lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(7,lhs); iw(rhs); return *this; }

#define BRAIN_X86_ASM_EMIT_0_id_r8(  name, preamble ) assembler& name( r8  lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(0,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_0_id_r16( name, preamble ) assembler& name( r16 lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(0,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_0_id_r32( name, preamble ) assembler& name( r32 lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(0,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_1_id_r8(  name, preamble ) assembler& name( r8  lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(1,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_1_id_r16( name, preamble ) assembler& name( r16 lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(1,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_1_id_r32( name, preamble ) assembler& name( r32 lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(1,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_2_id_r8(  name, preamble ) assembler& name( r8  lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(2,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_2_id_r16( name, preamble ) assembler& name( r16 lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(2,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_2_id_r32( name, preamble ) assembler& name( r32 lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(2,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_3_id_r8(  name, preamble ) assembler& name( r8  lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(3,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_3_id_r16( name, preamble ) assembler& name( r16 lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(3,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_3_id_r32( name, preamble ) assembler& name( r32 lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(3,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_4_id_r8(  name, preamble ) assembler& name( r8  lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(4,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_4_id_r16( name, preamble ) assembler& name( r16 lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(4,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_4_id_r32( name, preamble ) assembler& name( r32 lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(4,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_5_id_r8(  name, preamble ) assembler& name( r8  lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(5,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_5_id_r16( name, preamble ) assembler& name( r16 lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(5,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_5_id_r32( name, preamble ) assembler& name( r32 lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(5,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_6_id_r8(  name, preamble ) assembler& name( r8  lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(6,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_6_id_r16( name, preamble ) assembler& name( r16 lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(6,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_6_id_r32( name, preamble ) assembler& name( r32 lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(6,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_7_id_r8(  name, preamble ) assembler& name( r8  lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(7,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_7_id_r16( name, preamble ) assembler& name( r16 lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(7,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_7_id_r32( name, preamble ) assembler& name( r32 lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(7,lhs); id(rhs); return *this; }

#define BRAIN_X86_ASM_EMIT_0_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& a ) { BRAIN_X86_ASM_EMIT(preamble); _N(0,a); return *this; }
#define BRAIN_X86_ASM_EMIT_0_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& a ) { BRAIN_X86_ASM_EMIT(preamble); _N(0,a); return *this; }
#define BRAIN_X86_ASM_EMIT_0_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& a ) { BRAIN_X86_ASM_EMIT(preamble); _N(0,a); return *this; }
#define BRAIN_X86_ASM_EMIT_1_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& a ) { BRAIN_X86_ASM_EMIT(preamble); _N(1,a); return *this; }
#define BRAIN_X86_ASM_EMIT_1_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& a ) { BRAIN_X86_ASM_EMIT(preamble); _N(1,a); return *this; }
#define BRAIN_X86_ASM_EMIT_1_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& a ) { BRAIN_X86_ASM_EMIT(preamble); _N(1,a); return *this; }
#define BRAIN_X86_ASM_EMIT_2_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& a ) { BRAIN_X86_ASM_EMIT(preamble); _N(2,a); return *this; }
#define BRAIN_X86_ASM_EMIT_2_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& a ) { BRAIN_X86_ASM_EMIT(preamble); _N(2,a); return *this; }
#define BRAIN_X86_ASM_EMIT_2_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& a ) { BRAIN_X86_ASM_EMIT(preamble); _N(2,a); return *this; }
#define BRAIN_X86_ASM_EMIT_3_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& a ) { BRAIN_X86_ASM_EMIT(preamble); _N(3,a); return *this; }
#define BRAIN_X86_ASM_EMIT_3_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& a ) { BRAIN_X86_ASM_EMIT(preamble); _N(3,a); return *this; }
#define BRAIN_X86_ASM_EMIT_3_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& a ) { BRAIN_X86_ASM_EMIT(preamble); _N(3,a); return *this; }
#define BRAIN_X86_ASM_EMIT_4_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& a ) { BRAIN_X86_ASM_EMIT(preamble); _N(4,a); return *this; }
#define BRAIN_X86_ASM_EMIT_4_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& a ) { BRAIN_X86_ASM_EMIT(preamble); _N(4,a); return *this; }
#define BRAIN_X86_ASM_EMIT_4_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& a ) { BRAIN_X86_ASM_EMIT(preamble); _N(4,a); return *this; }
#define BRAIN_X86_ASM_EMIT_5_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& a ) { BRAIN_X86_ASM_EMIT(preamble); _N(5,a); return *this; }
#define BRAIN_X86_ASM_EMIT_5_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& a ) { BRAIN_X86_ASM_EMIT(preamble); _N(5,a); return *this; }
#define BRAIN_X86_ASM_EMIT_5_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& a ) { BRAIN_X86_ASM_EMIT(preamble); _N(5,a); return *this; }
#define BRAIN_X86_ASM_EMIT_6_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& a ) { BRAIN_X86_ASM_EMIT(preamble); _N(6,a); return *this; }
#define BRAIN_X86_ASM_EMIT_6_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& a ) { BRAIN_X86_ASM_EMIT(preamble); _N(6,a); return *this; }
#define BRAIN_X86_ASM_EMIT_6_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& a ) { BRAIN_X86_ASM_EMIT(preamble); _N(6,a); return *this; }
#define BRAIN_X86_ASM_EMIT_7_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& a ) { BRAIN_X86_ASM_EMIT(preamble); _N(7,a); return *this; }
#define BRAIN_X86_ASM_EMIT_7_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& a ) { BRAIN_X86_ASM_EMIT(preamble); _N(7,a); return *this; }
#define BRAIN_X86_ASM_EMIT_7_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& a ) { BRAIN_X86_ASM_EMIT(preamble); _N(7,a); return *this; }

#define BRAIN_X86_ASM_EMIT_0_ib_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(0,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_0_ib_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(0,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_0_ib_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(0,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_1_ib_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(1,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_1_ib_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(1,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_1_ib_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(1,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_2_ib_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(2,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_2_ib_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(2,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_2_ib_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(2,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_3_ib_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(3,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_3_ib_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(3,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_3_ib_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(3,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_4_ib_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(4,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_4_ib_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(4,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_4_ib_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(4,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_5_ib_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(5,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_5_ib_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(5,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_5_ib_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(5,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_6_ib_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(6,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_6_ib_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(6,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_6_ib_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(6,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_7_ib_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(7,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_7_ib_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(7,lhs); ib(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_7_ib_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& lhs, imm8 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(7,lhs); ib(rhs); return *this; }

#define BRAIN_X86_ASM_EMIT_0_iw_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(0,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_0_iw_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(0,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_0_iw_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(0,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_1_iw_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(1,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_1_iw_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(1,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_1_iw_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(1,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_2_iw_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(2,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_2_iw_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(2,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_2_iw_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(2,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_3_iw_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(3,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_3_iw_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(3,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_3_iw_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(3,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_4_iw_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(4,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_4_iw_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(4,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_4_iw_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(4,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_5_iw_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(5,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_5_iw_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(5,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_5_iw_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(5,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_6_iw_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(6,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_6_iw_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(6,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_6_iw_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(6,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_7_iw_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(7,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_7_iw_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(7,lhs); iw(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_7_iw_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& lhs, imm16 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(7,lhs); iw(rhs); return *this; }

#define BRAIN_X86_ASM_EMIT_0_id_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(0,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_0_id_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(0,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_0_id_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(0,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_1_id_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(1,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_1_id_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(1,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_1_id_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(1,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_2_id_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(2,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_2_id_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(2,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_2_id_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(2,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_3_id_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(3,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_3_id_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(3,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_3_id_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(3,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_4_id_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(4,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_4_id_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(4,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_4_id_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(4,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_5_id_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(5,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_5_id_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(5,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_5_id_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(5,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_6_id_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(6,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_6_id_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(6,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_6_id_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(6,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_7_id_m8(  name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,8 >& lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(7,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_7_id_m16( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,16>& lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(7,lhs); id(rhs); return *this; }
#define BRAIN_X86_ASM_EMIT_7_id_m32( name, preamble ) template < typename R, template < typename, size_t > class ExprTT > assembler& name( const ExprTT<R,32>& lhs, imm32 rhs ) { BRAIN_X86_ASM_EMIT(preamble); _N(7,lhs); id(rhs); return *this; }

#endif //ndef IG_BRAIN_X86_ASSEMBLER_V2_EMIT
