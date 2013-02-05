#ifndef IG_BRAIN_X86_ASSEMBLER_V3
#define IG_BRAIN_X86_ASSEMBLER_V3

#include "brain.x86.assembler.base.hpp"
#include <stdexcept>

namespace brain {
	namespace x86 {
		class assembler : public assembler_base {
			template < typename T1, typename T2, typename A1, typename A2 > bool is( A1 a1, A2 a2 ) {
				// ...
			}
		
			template < byte rm_base , typename rm_func ,                  typename A > void emit_pre1_rm_not8( A a, const char* name ) {
				if      ( is<rm16>(a) ) emit( mode16bit, rm_base, rm_func(a) );
				else if ( is<rm32>(a) ) emit( mode32bit, rm_base, rm_func(a) );
				else if ( is<rm64>(a) ) emit( mode64bit, rm_base, rm_func(a) );
				else
				{
					std::ostringstream ss;
					ss << "Illegal operand: " << a << " to " << name << ".";
					throw std::runtime_error( ss.str() );
				}
			}
			template < byte rm_base , typename rm_func ,                  typename A > void emit_pre1_rm( A a, const char* name ) {
				if      ( is<rm8 >(a) ) emit(            rm_base+0, rm_func(a) );
				else if ( is<rm16>(a) ) emit( mode16bit, rm_base+1, rm_func(a) );
				else if ( is<rm32>(a) ) emit( mode32bit, rm_base+1, rm_func(a) );
				else
				{
					std::ostringstream ss;
					ss << "Illegal operand: " << a << " to " << name << ".";
					throw std::runtime_error( ss.str() );
				}
			}
			template < byte imm_base, typename imm_func, byte rm_rm_base, typename A1, typename A2 > void emit_pre1_arith( A1 a1, A2 a2, const char* name ) {
				if      ( is<rm8 ,imm>(a1,a2) ) emit(             imm_base+0, imm_func(a1), imm8 (a2) );
				else if ( is<rm16,imm>(a1,a2) ) emit( mode16bit , imm_base+1, imm_func(a1), imm16(a2) );
				else if ( is<rm32,imm>(a1,a2) ) emit( mode32bit , imm_base+1, imm_func(a1), imm32(a2) );
				else if ( is<rm8 ,r8 >(a1,a2) ) emit(             rm_rm_base+0, _r(a1,a2) );
				else if ( is<rm16,r16>(a1,a2) ) emit( mode16bit , rm_rm_base+1, _r(a1,a2) );
				else if ( is<rm32,r32>(a1,a2) ) emit( mode32bit , rm_rm_base+1, _r(a1,a2) );
				else if ( is<r8 ,rm8 >(a1,a2) ) emit(             rm_rm_base+2, _r(a1,a2) );
				else if ( is<r16,rm16>(a1,a2) ) emit( mode16bit , rm_rm_base+3, _r(a1,a2) );
				else if ( is<r32,rm32>(a1,a2) ) emit( mode32bit , rm_rm_base+3, _r(a1,a2) );
				else
				{
					std::ostringstream ss;
					ss << "Illegal operands: " << a1 << " and " << a2 << " to " << name << ".";
					throw std::runtime_error( ss.str() );
				}
			}
		public:
			template < typename A1, typename A2 > assembler& adc     ( A1 a1, A2 a2 ) { emit_pre1_arith<0x80,_2,0x10>(a1,a2,"adc" ); return *this; }
			template < typename A1, typename A2 > assembler& add     ( A1 a1, A2 a2 ) { emit_pre1_arith<0x80,_0,0x00>(a1,a2,"add" ); return *this; }
			template < typename A1, typename A2 > assembler& and     ( A1 a1, A2 a2 ) { emit_pre1_arith<0x80,_4,0x20>(a1,a2,"and" ); return *this; }
			template < typename A1, typename A2 > assembler& cmp     ( A1 a1, A2 a2 ) { emit_pre1_arith<0x80,_7,0x38>(a1,a2,"cmp" ); return *this; }
			template < typename A >               assembler& dec     ( A a          ) { emit_pre1_rm   <0xFE,_1     >(a    ,"dec" ); return *this; }
			template < typename A >               assembler& div     ( A a          ) { emit_pre1_rm   <0xF6,_6     >(a    ,"div" ); return *this; }
			template < typename A1, typename A2 > assembler& mov     ( A1 a1, A2 a2 ) { emit_pre1_arith<0xC6,_0,0x88>(a1,a2,"mov" ); return *this; }
			template < typename A >               assembler& idiv    ( A a          ) { emit_pre1_rm   <0xF6,_7     >(a    ,"idiv"); return *this; }
			template < typename A >               assembler& imul    ( A a          ) { emit_pre1_rm   <0xF6,_5     >(a    ,"imul"); return *this; }
			template < typename A >               assembler& mul     ( A a          ) { emit_pre1_rm   <0xF6,_4     >(a    ,"mul" ); return *this; }
			                                      assembler& nop     (              ) { emit( 0x90 );                                return *this; }
			template < typename A >               assembler& not     ( A a          ) { emit_pre1_rm   <0xF6,_2     >(a    ,"not" ); return *this; }
			template < typename A1, typename A2 > assembler& or      ( A1 a1, A2 a2 ) { emit_pre1_arith<0x80,_1,0x08>(a1,a2,"or"  ); return *this; }
			template < typename A >               assembler& pop     ( A a          ) { emit_pre1_rm_not8<0x8F,_0   >(a    ,"pop" ); return *this; }
			                                      assembler& popa    (              ) { emit( 0x61 );                                return *this; }
			                                      assembler& popad   (              ) { emit( 0x61 );                                return *this; }
			template < typename A >               assembler& push    ( A a          ) { emit_pre1_rm_not8<0xFF,_6   >(a    ,"push"); return *this; }
			//OP(              push    , (6A), ib(imm8) )
			//OP(              push    , (68), id(imm32) )
			                                      assembler& pusha   (              ) { emit( 0x60 );                                return *this; }
			                                      assembler& pushad  (              ) { emit( 0x60 );                                return *this; }
			                                      assembler& syscall (              ) { emit( 0x0F, 0x05 );                          return *this; }
			                                      assembler& sysenter(              ) { emit( 0x0F, 0x34 );                          return *this; }
			                                      assembler& sysexit (              ) { emit( 0x0F, 0x35 );                          return *this; }
			                                      assembler& sysret  (              ) { emit( 0x0F, 0x07 );                          return *this; }
			                                      assembler& ret_near(              ) { emit( 0xC3 );                                return *this; }
			                                      assembler& ret_far (              ) { emit( 0xCB );                                return *this; }
			                                      assembler& ret     (              ) { emit( 0xC3 );                                return *this; }
			                                      assembler& ret_near( imm16 bytes  ) { emit( 0xC2, iw(bytes) );                     return *this; }
			                                      assembler& ret_far ( imm16 bytes  ) { emit( 0xCA, iw(bytes) );                     return *this; }
			                                      assembler& ret     ( imm16 bytes  ) { emit( 0xC2, iw(bytes) );                     return *this; }
			template < typename A >               assembler& test    ( A a          ) { emit_pre1_rm   <0xF6,_0     >(a    ,"test"); return *this; }
			template < typename A1, typename A2 > assembler& sub     ( A1 a1, A2 a2 ) { emit_pre1_arith<0x80,_5,0x28>(a1,a2,"sub" ); return *this; }
			template < typename A1, typename A2 > assembler& xor     ( A1 a1, A2 a2 ) { emit_pre1_arith<0x80,_6,0x30>(a1,a2,"xor" ); return *this; }
		};
	}
}

#endif //ndef IG_BRAIN_X86_ASSEMBLER_V3
