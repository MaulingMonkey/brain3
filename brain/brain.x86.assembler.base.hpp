#ifndef IG_BRAIN_X86_ASSEMBLER_V2_BASE
#define IG_BRAIN_X86_ASSEMBLER_V2_BASE

#include "brain.x86.expressions.hpp"
#include <boost/shared_ptr.hpp>
#include <vector>

namespace brain {
	namespace x86 {
		class assembler_base {
		public:
			boost::shared_ptr< std::vector< unsigned char > > code;
			assembler_base(): code( new std::vector< unsigned char > ) {}
		protected:
			typedef unsigned char  imm8 ;
			typedef unsigned short imm16;
			typedef unsigned int   imm32;

			typedef unsigned char  byte;
			typedef unsigned short word;
			typedef unsigned int   dword;

			void ModR_M( byte mod, byte reg_or_opcode, byte r_m ) {
				assert( 0 <= mod           && mod           <= 3 );
				assert( 0 <= reg_or_opcode && reg_or_opcode <= 7 );
				assert( 0 <= r_m           && r_m           <= 7 );
				emit( (mod<<6) + (reg_or_opcode<<3)  + (r_m<<0) );
			}

			void SIB( byte scale, byte index, byte base ) {
				assert( 0 <= scale && scale <= 3 );
				assert( 0 <= index && index <= 7 );
				assert( 0 <= base  && base  <= 7 );
				emit( (scale<<6) + (index<<3) + (base<<0) );
			}

			template < typename R, size_t M_B > void _r_r_rm( R r, const typed_reg_expr<r32,M_B>& rm ) {
				switch ( rm.reg.value ) {
				case r32::_esp: ModR_M(0,r.value,4);         SIB(0,4,4); break;
				case r32::_ebp: ModR_M(1,r.value,ebp.value); disp8(0);   break;
				default:        ModR_M(0,r.value,rm.reg.value);          break;
				}
			}
			template < typename R, size_t M_B > void _r_r_rm( R r, const typed_scaled_reg_expr<r32,M_B>& rm ) {
				if ( rm.scale == 1 ) {
					typed_reg_expr<r32,M_B> expr = { rm.reg };
					_r_r_rm( r, expr );
				} else if ( rm.reg == esp ) {
					throw std::runtime_error( "Cannot do [#*esp]" );
				} else {
					int scale;
					switch ( rm.scale ) {
					case 2: scale=1; break;
					case 4: scale=2; break;
					case 8: scale=3; break;
					default: throw std::runtime_error( "Can only scale register by 1, 2, 4, or 8" );
					}
					
					ModR_M(0,r.value,4); // [--][--]
					SIB(scale,rm.value,5); //[*]
					disp32(0);
				}
			}
			template < typename R, size_t M_B > void _r_r_rm( R r, const typed_reg_reg_expr<r32,M_B>& rm ) {
				r32 reg1 = rm.reg1;
				r32 reg2 = rm.reg2;

				if ( reg1 == reg2 ) {
					if ( reg1 == esp ) throw std::runtime_error( "Cannot do [esp + esp]" );
					if ( reg1 == ebp ) throw std::runtime_error( "Cannot do [ebp + ebp]" );
				}

				if ( reg1 == esp ) std::swap(reg1,reg2);

				ModR_M(0,r.value,4); // [--][--]
				SIB(0,reg1.value,reg2.value); // [reg1][reg2]
			}
			template < typename R, size_t M_B > void _r_r_rm( R r, const typed_reg_scaled_reg_expr<r32,M_B>& rm ) {
				if ( rm.scale == 1 ) {
					typed_reg_reg_expr<r32,M_B> expr = { rm.reg1, rm.reg2 };
					_r_r_rm( r, expr );
				} else if ( rm.reg2 == esp ) {
					throw std::runtime_error( "Cannot scale esp" );
				} else {
					int scale;
					switch ( rm.scale ) {
					case 2: scale=1; break;
					case 4: scale=2; break;
					case 8: scale=3; break;
					default: throw std::runtime_error( "Can only scale register by 1, 2, 4, or 8" );
					}

					if ( rm.reg1 == ebp ) {
						ModR_M(0,r.value,4); // [--][--]
						SIB( scale, rm.reg2.value, 5 ); // [*] -- [_*reg2] + disp8 + [ebp]
						disp8(0);
					} else {
						ModR_M(0,r.value,4); // [--][--]
						SIB( scale, rm.reg2.value, rm.reg1.value ); // [reg1 + scale*reg2]
					}
				}
			}

			template < typename R, size_t M_B > void _r_r_rm( R r, const typed_reg_disp_expr<r32,M_B>& rm ) {
				if ( rm.disp == 0 ) {
					typed_reg_expr<r32,M_B> expr = { rm.reg };
					_r_r_rm(r,expr);
				} else if ( rm.reg == esp ) {
					const bool fits_in_8 = is_legal_disp8(rm.disp);
					ModR_M( fits_in_8?1:2, r.value, 4 ); // [--][--]
					SIB(0,4,4); // esp + 1* none
					fits_in_8 ? disp8(rm.disp) : disp32(rm.disp);
				} else {
					const bool fits_in_8 = is_legal_disp8(rm.disp);
					ModR_M( fits_in_8?1:2, r.value, rm.reg.value );
					fits_in_8 ? disp8(rm.disp) : disp32(rm.disp);
				}
			}
			template < typename R, size_t M_B > void _r_r_rm( R r, const typed_reg_reg_disp_expr<r32,M_B>& rm ) {
				if ( rm.disp == 0 ) {
					typed_reg_reg_expr<r32,M_B> expr = { rm.reg1, rm.reg2 };
					_r_r_rm(r,expr);
				} else {
					r32 reg1 = rm.reg1;
					r32 reg2 = rm.reg2;

					if ( reg1 == reg2 ) {
						if ( reg1 == esp ) throw std::runtime_error( "Cannot do [esp + esp + #]" );
						if ( reg1 == ebp ) throw std::runtime_error( "Cannot do [ebp + ebp + #]" );
					}

					if ( reg1 == esp ) std::swap(reg1,reg2);

					const bool fits_in_8 = is_legal_disp8(rm.disp);
					ModR_M( fits_in_8?1:2, r.value, 4); // [--][--] + disp8|32
					SIB(0,reg2.value,reg1.value); // [reg1][reg2]
				}
			}
			template < typename R, size_t M_B > void _r_r_rm( R r, const typed_reg_scaled_reg_disp_expr<r32,M_B>& rm ) {
				if ( rm.disp == 0 ) {
					typed_reg_scaled_reg_expr<r32,M_B> expr = { rm.reg1, rm.scale, rm.reg2 };
					_r_r_rm(r,expr);
				} else if ( rm.scale == 1 ) {
					typed_reg_reg_disp_expr<r32,M_B> expr = { rm.reg1, rm.reg2, rm.disp };
					_r_r_rm(r,expr);
				} else if ( rm.reg1 == ebp ) {
					throw std::runtime_error( "Cannot do [ebp + #*??? + #]" );
				} else if ( rm.reg2 == esp ) {
					throw std::runtime_error( "Cannot do [??? + #*esp + #]" );
				} else {
					const bool fits_in_8 = is_legal_disp8(rm.disp);
					ModR_M( fits_in_8?1:2, r.value, 4); // [--][--] + disp8|32
					SIB(rm.scale,rm.reg2.value,rm.reg1.value);
					fits_in_8 ? disp8(rm.disp) : disp32(rm.disp);
				}
			}

			template < typename R > void _r_r_rm( R r, r8  rm ) { ModR_M(3,r.value,rm.value); }
			template < typename R > void _r_r_rm( R r, r16 rm ) { ModR_M(3,r.value,rm.value); }
			template < typename R > void _r_r_rm( R r, r32 rm ) { ModR_M(3,r.value,rm.value); }

			template < typename RM, typename R > void _r_rm_r( RM rm, R r ) { _r_r_rm(r,rm); }

			template < typename R > void _N( int N, R reg ) { _r_r_rm(r8(r8::_(N)),reg.value); } // r8(N) is an ugly hack

			void emit( unsigned char ch ) { code->push_back(ch); }

			void ib( imm8  value ) { emit(value); }
			void iw( imm16 value ) { emit((value>>0)&0xFF); emit((value>>8)&0xFF); } // little endian
			void id( imm32 value ) { emit((value>>0)&0xFF); emit((value>>8)&0xFF); emit((value>>16)&0xFF); emit((value>>24)&0xFF); }
			//void ib( int value ) { assert( -0x80 <= value           && value           <= 0x7F ); ib( imm8 (value>=0 ? value : 0xFFu      +value+1) ); }
			//void iw( int value ) { assert( -0x80 <= value/0x100     && value/0x100     <= 0x7F ); iw( imm16(value>=0 ? value : 0xFFFFu    +value+1) ); }
			//void id( int value ) { assert( -0x80 <= value/0x1000000 && value/0x1000000 <= 0x7F ); id( imm32(value>=0 ? value : 0xFFFFFFFFu+value+1) ); }

			bool is_legal_disp8( int value ) { return (-128 <= value && value <= +127); }
			void disp8 ( int value ) { assert(is_legal_disp8(value)); emit( (value>=0)?(value):(0x100+value) ); }
			void disp32( int value ) { unsigned uvalue = (value>=0) ? (value) : (0xFFFFFFFFu+value+1); id(uvalue); }
			void disp_8_or_32( int value ) { if (is_legal_disp8(value)) disp8(value); else disp32(value); }
		};
	}
}

#endif //ndef IG_BRAIN_X86_ASSEMBLER_V2_BASE
