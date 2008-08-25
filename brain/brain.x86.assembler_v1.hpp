#ifndef IG_BRAIN_X86_ASSEMBLER
#define IG_BRAIN_X86_ASSEMBLER

#include "brain.x86.registers.hpp"
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/cstdint.hpp>
#include <vector>

namespace brain {
	namespace x86 {
		class assembler {
		public:
			typedef boost::uint8_t  byte;
			typedef boost::uint16_t word;
			typedef boost::uint32_t dword;
			//typedef boost::uint64_t qword;

			std::vector<byte> code;

			void clear() { code.resize(0); }

			void emit( byte b1 ) { code.push_back(b1); }
			void emit( byte b1, byte b2 ) { emit(b1); emit(b2); }
			void emit( byte b1, byte b2, byte b3 ) { emit(b1,b2); emit(b3); }
			void emit( byte b1, byte b2, byte b3, byte b4 ) { emit(b1,b2,b3); emit(b4); }

		private:
			// +----------------------+--------+--------+-----+--------------+-----------+
			// | Instruction Prefixes | Opcode | ModR/M | SIB | Displacement | Immediate |
			// +----------------------+--------+--------+-----+--------------+-----------+
			//  up to 4 1-byte prefix  1-3byte  0-1byte  0-1b  0, 1, 2, or 4   0, 1, 2, or 4

			// Opcodes ib, iw, and id
			void ib( byte  value ) { emit(value); }
			void iw( word  value ) { emit((value>>0)&0xFF,(value>>8)&0xFF); } // little endian
			void id( dword value ) { emit((value>>0)&0xFF,(value>>8)&0xFF,(value>>16)&0xFF,(value>>24)&0xFF); }

			bool legal_disp8( int value ) { return (-128 <= value && value <= +127); }
			void disp8 ( int value ) { assert(legal_disp8(value)); emit( (value>=0)?(value):(0x100+value) ); }
			void disp32( int value ) { unsigned uvalue = (value>=0) ? (value) : (0xFFFFFFFFu+value+1); id(uvalue); }
			void disp_8_or_32( int value ) { if (legal_disp8(value)) disp8(value); else disp32(value); }

			// Opcode /r  
			void _r_r_rm( r8  r, r8  rm ) { ModR_M(3,r.value,rm.value); }
			void _r_r_rm( r8  r, r16 rm ) { ModR_M(3,r.value,rm.value); }
			void _r_r_rm( r8  r, r32 rm ) { ModR_M(3,r.value,rm.value); }
			void _r_r_rm( r16 r, r8  rm ) { ModR_M(3,r.value,rm.value); }
			void _r_r_rm( r16 r, r16 rm ) { ModR_M(3,r.value,rm.value); }
			void _r_r_rm( r16 r, r32 rm ) { ModR_M(3,r.value,rm.value); }
			void _r_r_rm( r32 r, r8  rm ) { ModR_M(3,r.value,rm.value); }
			void _r_r_rm( r32 r, r16 rm ) { ModR_M(3,r.value,rm.value); }
			void _r_r_rm( r32 r, r32 rm ) { ModR_M(3,r.value,rm.value); }
			void _r_rm_r( r8  rm, r8  r ) { ModR_M(3,r.value,rm.value); }
			void _r_rm_r( r8  rm, r16 r ) { ModR_M(3,r.value,rm.value); }
			void _r_rm_r( r8  rm, r32 r ) { ModR_M(3,r.value,rm.value); }
			void _r_rm_r( r16 rm, r8  r ) { ModR_M(3,r.value,rm.value); }
			void _r_rm_r( r16 rm, r16 r ) { ModR_M(3,r.value,rm.value); }
			void _r_rm_r( r16 rm, r32 r ) { ModR_M(3,r.value,rm.value); }
			void _r_rm_r( r32 rm, r8  r ) { ModR_M(3,r.value,rm.value); }
			void _r_rm_r( r32 rm, r16 r ) { ModR_M(3,r.value,rm.value); }
			void _r_rm_r( r32 rm, r32 r ) { ModR_M(3,r.value,rm.value); }
			
			// needs to assert 16 bit addressing
			// needs to assert [si], [di], or [bx], the only valid registers
			// see Table 2-1.
			void _r_r_rm( r16 r, const r16*rm ) {} // ModR_M(0,r.value,rm->value); }
			void _r_rm_r( const r16*rm, r16 r ) {} // ModR_M(0,r.value,rm->value); }
			void _r_r_rm( r16 r, disp_expr<r16> rm ) {}
			void _r_rm_r( disp_expr<r16> rm, r16 r ) {}

			// These need to assert 32 bit addressing:
			// see Table 2-2.
			void _r_r_rm( r32 r, const r32* rm ) { if (*rm==esp) { ModR_M(0,r.value,4); SIB(0,4,4); } else if (*rm==ebp) { ModR_M(1,r.value,ebp.value); disp8(0); } else { ModR_M(0,r.value,rm->value); } }
			void _r_rm_r( const r32* rm, r32 r ) { if (*rm==esp) { ModR_M(0,r.value,4); SIB(0,4,4); } else if (*rm==ebp) { ModR_M(1,r.value,ebp.value); disp8(0); } else { ModR_M(0,r.value,rm->value); } }
			void _r_r_rm( r32 r, disp_expr<r32> rm ) { const bool d8 = legal_disp8(rm.disp); ModR_M(d8?1:2,r.value,rm.reg.value); if (rm.reg==esp) SIB(0,4,esp); disp_8_or_32(rm.disp); }
			void _r_rm_r( disp_expr<r32> rm, r32 r ) { const bool d8 = legal_disp8(rm.disp); ModR_M(d8?1:2,r.value,rm.reg.value); if (rm.reg==esp) SIB(0,4,esp); disp_8_or_32(rm.disp); }

			// Opcodes /0../7 -- doesn't use the Mod field according to 3.1.1.1 of the instruction set reference

			void _N( byte N, byte value ) { ModR_M(0,N,value); }
			void _N( byte N, r8  reg ) { ModR_M(3,N,reg.value); }
			void _N( byte N, r16 reg ) { ModR_M(3,N,reg.value); }
			void _N( byte N, r32 reg ) { ModR_M(3,N,reg.value); }
			//void _N( byte N, const r16* reg ) { ModR_M(0,N,reg->value); }
			//void _N( byte N, const r32* reg ) { ModR_M(0,N,reg->value); }
			
			template < typename T > void _0(T value) { _N(0,value); }
			template < typename T > void _1(T value) { _N(1,value); }
			template < typename T > void _2(T value) { _N(2,value); }
			template < typename T > void _3(T value) { _N(3,value); }
			template < typename T > void _4(T value) { _N(4,value); }
			template < typename T > void _5(T value) { _N(5,value); }
			template < typename T > void _6(T value) { _N(6,value); }
			template < typename T > void _7(T value) { _N(7,value); }

			//  r8,   r16,   r32,   r64 = registers
			//imm8, imm16, imm32, imm64 = immediate <bits> length value
			//r/m8, r/m16, r/m32, r/m64 = register refering to an address of a value of <bits> length

			////////////////////////////////////////////////////////
			//               bits:     7   6 5          3 2   0   //
			//                        +-----+------------+-----+  //
			//  ModR/M byte format:   | Mod | Reg/Opcode | R/M |  //
			//                        +-----+------------+-----+  //
			void ModR_M( byte mod, byte reg_or_opcode, byte r_m ) {
				assert( 0 <= mod           && mod           <= 3 );
				assert( 0 <= reg_or_opcode && reg_or_opcode <= 7 );
				assert( 0 <= r_m           && r_m           <= 7 );
				emit( (mod<<6) + (reg_or_opcode<<3)  + (r_m<<0) );
			}
			////////////////////////////////////////////////////////

			// SIB format:
			//  7     6 5     3 2    0
			// +-------+-------+------+
			// | Scale | Index | Base |
			// +-------+-------+------+
			void SIB( byte scale, byte index, byte base ) {
				assert( 0 <= scale && scale <= 3 );
				assert( 0 <= index && index <= 7 );
				assert( 0 <= base  && base  <= 7 );
				emit( (scale<<6) + (index<<3) + (base<<0) );
			}
			void SIB( byte scale, byte index, r32 base ) { assert( base  != ebp ); SIB(scale,index,base.value); }
			void SIB( byte scale, r32 index, byte base ) { assert( index != esp ); SIB(scale,index.value,base); }
			void SIB( byte scale, r32 index, r32 base ) { assert( base != ebp && index != esp ); SIB(scale,index.value,base.value); }
			
			// See table 2-1. on page 38 of the instruction set reference
			// __asm xor eax,ecx; // C1
			// __asm xor ecx,eax; // C8
		public:

			void hlt() { code.push_back(0xF4); }
			void nop() { code.push_back(0x90); }

/*-------------------------------------------------------------------------------------------------------------*/
#define OP_RM_N( n, name, preamble, argparity, arg )                                                            \
	assembler& name( r ## argparity arg ) { emit(BOOST_PP_SEQ_ENUM(preamble)); _N(n,arg.value); return *this; } \
	assembler& name( r ## argparity arg ) { emit(BOOST_PP_SEQ_ENUM(preamble)); _N(n,arg.value); return *this; } \
	assembler& name( r ## argparity arg ) { emit(BOOST_PP_SEQ_ENUM(preamble)); _N(n,arg.value); return *this; } \
/*-------------------------------------------------------------------------------------------------------------*/
#define OP_RM_0( name, preamble, argparity, arg ) OP_RM_N( 0, name, preamble, argparity, arg )
#define OP_RM_1( name, preamble, argparity, arg ) OP_RM_N( 1, name, preamble, argparity, arg )
#define OP_RM_2( name, preamble, argparity, arg ) OP_RM_N( 2, name, preamble, argparity, arg )
#define OP_RM_3( name, preamble, argparity, arg ) OP_RM_N( 3, name, preamble, argparity, arg )
#define OP_RM_4( name, preamble, argparity, arg ) OP_RM_N( 4, name, preamble, argparity, arg )
#define OP_RM_5( name, preamble, argparity, arg ) OP_RM_N( 5, name, preamble, argparity, arg )
#define OP_RM_6( name, preamble, argparity, arg ) OP_RM_N( 6, name, preamble, argparity, arg )
#define OP_RM_7( name, preamble, argparity, arg ) OP_RM_N( 7, name, preamble, argparity, arg )
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
#define OP_R_RM( name, preamble, arg1parity, arg2parity, arg1, arg2 )                                                                                 \
	assembler& name( r ## arg1parity arg1,           r ## arg2parity  arg2 ) { emit(BOOST_PP_SEQ_ENUM(preamble)); _r_r_rm(arg1,arg2); return *this; } \
	assembler& name( r ## arg1parity arg1,     const r ## arg2parity *arg2 ) { emit(BOOST_PP_SEQ_ENUM(preamble)); _r_r_rm(arg1,arg2); return *this; } \
	assembler& name( r ## arg1parity arg1, disp_expr<r ## arg2parity> arg2 ) { emit(BOOST_PP_SEQ_ENUM(preamble)); _r_r_rm(arg1,arg2); return *this; } \
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
#define OP_RM_R( name, preamble, arg1parity, arg2parity, arg1, arg2 )                                                                                 \
	assembler& name(           r ## arg1parity  arg1, r ## arg2parity arg2 ) { emit(BOOST_PP_SEQ_ENUM(preamble)); _r_rm_r(arg1,arg2); return *this; } \
	assembler& name(     const r ## arg1parity *arg1, r ## arg2parity arg2 ) { emit(BOOST_PP_SEQ_ENUM(preamble)); _r_rm_r(arg1,arg2); return *this; } \
	assembler& name( disp_expr<r ## arg1parity> arg1, r ## arg2parity arg2 ) { emit(BOOST_PP_SEQ_ENUM(preamble)); _r_rm_r(arg1,arg2); return *this; } \
/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define OP_R_RM_AND_RM_R( name, rm_r_preamble, r_rm_preamble, arg1parity, arg2parity, arg1, arg2 )                                                         \
	assembler& name( r ## arg1parity arg1,            r ## arg2parity arg2 ) { emit(BOOST_PP_SEQ_ENUM(rm_r_preamble)); _r_rm_r(arg1,arg2); return *this; } \
	assembler& name(     const r ## arg1parity *arg1, r ## arg2parity arg2 ) { emit(BOOST_PP_SEQ_ENUM(rm_r_preamble)); _r_rm_r(arg1,arg2); return *this; } \
	assembler& name( disp_expr<r ## arg1parity> arg1, r ## arg2parity arg2 ) { emit(BOOST_PP_SEQ_ENUM(rm_r_preamble)); _r_rm_r(arg1,arg2); return *this; } \
	assembler& name( r ## arg1parity arg1,     const r ## arg2parity *arg2 ) { emit(BOOST_PP_SEQ_ENUM(r_rm_preamble)); _r_r_rm(arg1,arg2); return *this; } \
	assembler& name( r ## arg1parity arg1, disp_expr<r ## arg2parity> arg2 ) { emit(BOOST_PP_SEQ_ENUM(r_rm_preamble)); _r_r_rm(arg1,arg2); return *this; } \
/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ARITH_OP( name, imm_base, imm_mod, r_base )                                                              \
	assembler& name( r8  dest, byte  value ) { emit(imm_base+0); imm_mod(dest.value); ib(value); return *this; } \
	assembler& name( r16 dest, word  value ) { emit(imm_base+1); imm_mod(dest.value); iw(value); return *this; } \
	assembler& name( r32 dest, dword value ) { emit(imm_base+1); imm_mod(dest.value); id(value); return *this; } \
	assembler& name( r8  dest, r8    src   ) { emit(r_base+0); _r_rm_r(dest,src); return *this; }                \
	OP_R_RM_AND_RM_R( name, (r_base+1), (r_base+3), 16, 16, dest, value )                                        \
	OP_R_RM_AND_RM_R( name, (r_base+1), (r_base+3), 32, 32, dest, value )                                        \
/*--------------------------------------------------------------------------------------------------------------*/

			ARITH_OP( adc, 0x80, _2, 0x10 ) // add with carry
			ARITH_OP( add, 0x80, _2, 0x00 ) // add
			ARITH_OP( and, 0x80, _4, 0x20 ) // logical and

			// assembler& call( rel16 ) { emit(0xE8); cw; }
			// assembler& call( rel32 ) { emit(0xE8); cd; }
			// assembler& call( r16 address ) { emit(0xFF); _2(address.value); return *this; } r/m16
			// assembler& call( r32 address ) { emit(0xFF); _2(address.value); return *this; } r/m32
			// assembler& call( r64 address ) { emit(0xFF); _2(address.value); return *this; } r/m64

			assembler& clc()  { emit(0xF8); return *this; } // Clear Carry Flag
			assembler& cld()  { emit(0xFC); return *this; } // Clear Direction Flag
			assembler& cli()  { emit(0xFA); return *this; } // Clear Interrupt Flag
			assembler& clts() { emit(0x0F,0x06); return *this; } // Clear Task-Switched Flag in CR0 -- requires privilege level 0!!!
			assembler& cmc()  { emit(0xF5); return *this; } // Complement Carry Flag

			// CMOVcc -- Conditional Move
/*-------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define CMOV_OP( z, unused, postfix_code )                                                                                                                      \
	OP_R_RM( BOOST_PP_CAT(cmov,BOOST_PP_TUPLE_ELEM(2,0,postfix_code)) , (0x0F)(BOOST_PP_CAT(0x,BOOST_PP_TUPLE_ELEM(2,1,postfix_code))) , 16 , 16 , dest , src ) \
	OP_R_RM( BOOST_PP_CAT(cmov,BOOST_PP_TUPLE_ELEM(2,0,postfix_code)) , (0x0F)(BOOST_PP_CAT(0x,BOOST_PP_TUPLE_ELEM(2,1,postfix_code))) , 32 , 32 , dest , src ) \
/*-------------------------------------------------------------------------------------------------------------------------------------------------------------*/
			BOOST_PP_SEQ_FOR_EACH( CMOV_OP, ~, \
				((a ,47)) ((ae,43)) ((b  ,42)) ((be ,46)) ((c  ,42)) ((e  ,44)) ((g ,4F)) ((ge,4D)) \
				((l ,4C)) ((le,4E)) ((na ,46)) ((nae,42)) ((nb ,43)) ((nbe,47))                     \
				((nc,43)) ((ng,4E)) ((nge,4C)) ((nl ,4D)) ((nle,4F)) ((no ,41)) ((np,4B))           \
				((ns,49)) ((nz,45)) ((o  ,40)) ((p  ,4A)) ((pe ,4A)) ((po ,4B)) ((s ,48)) ((z ,44)) \
				)
#undef CMOV_OP

			ARITH_OP( cmp, 0x80, _7, 0x38 ) // compare

			assembler& cpuid() { emit( 0x0F, 0xA2 ); return *this; }
			
			OP_R_RM( crc32, (0xF2)(0x0F)(0x38)(0xF0), 32,  8, dest, src )
			OP_R_RM( crc32, (0xF2)(0x0F)(0x38)(0xF1), 32, 16, dest, src )
			OP_R_RM( crc32, (0xF2)(0x0F)(0x38)(0xF1), 32, 32, dest, src )

			OP_RM_1( dec, (0xFE),  8, target )
			OP_RM_1( dec, (0xFF), 16, target )
			OP_RM_1( dec, (0xFF), 32, target )

			assembler& ret_near() { emit(0xC3); return *this; }
			assembler& ret_far () { emit(0xCB); return *this; }
			assembler& ret_near( word n_bytes_pop ) { emit(0xC2); iw(n_bytes_pop); return *this; }
			assembler& ret_far ( word n_bytes_pop ) { emit(0xCA); iw(n_bytes_pop); return *this; }
			assembler& ret() { ret_near(); return *this; }
			assembler& ret( word n_bytes_pop ) { ret_near(n_bytes_pop); return *this; }

			assembler& xor( r8  dest, r8  src ) { emit(0x32); _r_r_rm(dest,src); return *this; }
			assembler& xor( r16 dest, r16 src ) { emit(0x33); _r_r_rm(dest,src); return *this; }
			assembler& xor( r32 dest, r32 src ) { emit(0x33); _r_r_rm(dest,src); return *this; }
		};
	}
}

#endif //ndef IG_BRAIN_X86_ASSEMBLER
