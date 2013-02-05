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
			assembler& adc ( rm8  l, imm   r ) { emit(          0x80, _2(l), imm8 (r) ); return *this; }
			assembler& adc ( rm16 l, imm   r ) { emit( mode16b, 0x81, _2(l), imm16(r) ); return *this; }
			assembler& adc ( rm32 l, imm   r ) { emit( mode32b, 0x81, _2(l), imm32(r) ); return *this; }
			assembler& adc ( rm8  l, r8    r ) { emit(          0x10, _r_rm_r(l,r)    ); return *this; }
			assembler& adc ( rm16 l, r16   r ) { emit( mode16b, 0x11, _r_rm_r(l,r)    ); return *this; }
			assembler& adc ( rm32 l, r32   r ) { emit( mode32b, 0x11, _r_rm_r(l,r)    ); return *this; }
			assembler& adc ( r8   l, mem8  r ) { emit(          0x12, _r_r_rm(l,r)    ); return *this; }
			assembler& adc ( r16  l, mem16 r ) { emit( mode16b, 0x13, _r_r_rm(l,r)    ); return *this; }
			assembler& adc ( r32  l, mem32 r ) { emit( mode32b, 0x13, _r_r_rm(l,r)    ); return *this; }
			
			assembler& add ( rm8  l, imm   r ) { emit(          0x80, _0(l), imm8 (r) ); return *this; }
			assembler& add ( rm16 l, imm   r ) { emit( mode16b, 0x81, _0(l), imm16(r) ); return *this; }
			assembler& add ( rm32 l, imm   r ) { emit( mode32b, 0x81, _0(l), imm32(r) ); return *this; }
			assembler& add ( rm8  l, r8    r ) { emit(          0x00, _r_rm_r(l,r)    ); return *this; }
			assembler& add ( rm16 l, r16   r ) { emit( mode16b, 0x01, _r_rm_r(l,r)    ); return *this; }
			assembler& add ( rm32 l, r32   r ) { emit( mode32b, 0x01, _r_rm_r(l,r)    ); return *this; }
			assembler& add ( r8   l, mem8  r ) { emit(          0x02, _r_r_rm(l,r)    ); return *this; }
			assembler& add ( r16  l, mem16 r ) { emit( mode16b, 0x03, _r_r_rm(l,r)    ); return *this; }
			assembler& add ( r32  l, mem32 r ) { emit( mode32b, 0x03, _r_r_rm(l,r)    ); return *this; }
			
			assembler& and ( rm8  l, imm   r ) { emit(          0x80, _4(l), imm8 (r) ); return *this; }
			assembler& and ( rm16 l, imm   r ) { emit( mode16b, 0x81, _4(l), imm16(r) ); return *this; }
			assembler& and ( rm32 l, imm   r ) { emit( mode32b, 0x81, _4(l), imm32(r) ); return *this; }
			assembler& and ( rm8  l, r8    r ) { emit(          0x20, _r_rm_r(l,r)    ); return *this; }
			assembler& and ( rm16 l, r16   r ) { emit( mode16b, 0x21, _r_rm_r(l,r)    ); return *this; }
			assembler& and ( rm32 l, r32   r ) { emit( mode32b, 0x21, _r_rm_r(l,r)    ); return *this; }
			assembler& and ( r8   l, mem8  r ) { emit(          0x22, _r_r_rm(l,r)    ); return *this; }
			assembler& and ( r16  l, mem16 r ) { emit( mode16b, 0x23, _r_r_rm(l,r)    ); return *this; }
			assembler& and ( r32  l, mem32 r ) { emit( mode32b, 0x23, _r_r_rm(l,r)    ); return *this; }
			
			assembler& cmp ( rm8  l, imm   r ) { emit(          0x80, _7(l), imm8 (r) ); return *this; }
			assembler& cmp ( rm16 l, imm   r ) { emit( mode16b, 0x81, _7(l), imm16(r) ); return *this; }
			assembler& cmp ( rm32 l, imm   r ) { emit( mode32b, 0x81, _7(l), imm32(r) ); return *this; }
			assembler& cmp ( rm8  l, r8    r ) { emit(          0x38, _r_rm_r(l,r)    ); return *this; }
			assembler& cmp ( rm16 l, r16   r ) { emit( mode16b, 0x39, _r_rm_r(l,r)    ); return *this; }
			assembler& cmp ( rm32 l, r32   r ) { emit( mode32b, 0x39, _r_rm_r(l,r)    ); return *this; }
			assembler& cmp ( r8   l, mem8  r ) { emit(          0x3A, _r_r_rm(l,r)    ); return *this; }
			assembler& cmp ( r16  l, mem16 r ) { emit( mode16b, 0x3B, _r_r_rm(l,r)    ); return *this; }
			assembler& cmp ( r32  l, mem32 r ) { emit( mode32b, 0x3B, _r_r_rm(l,r)    ); return *this; }

			assembler& idiv( /* eax:edx l, */ rm8  r ) { emit(          0xF6, _7(r)   ); return *this; }
			assembler& idiv( /* eax:edx l, */ rm16 r ) { emit( mode16b, 0xF7, _7(r)   ); return *this; }
			assembler& idiv( /* eax:edx l, */ rm32 r ) { emit( mode32b, 0xF7, _7(r)   ); return *this; }
			
			assembler& imul( /* eax:[edx=0] l, */ rm8  r ) { emit(          0xF6, _5(r)    ); return *this; }
			assembler& imul( /* eax:[edx=0] l, */ rm16 r ) { emit( mode16b, 0xF7, _5(r)    ); return *this; }
			assembler& imul( /* eax:[edx=0] l, */ rm32 r ) { emit( mode32b, 0xF7, _5(r)    ); return *this; }
			assembler& imul( r16 l, rm16 r ) { emit( mode16b, 0x0F, 0xAF, _r_r_rm(l,r) ); return *this; }
			assembler& imul( r32 l, rm32 r ) { emit( mode32b, 0x0F, 0xAF, _r_r_rm(l,r) ); return *this; }
			assembler& imul( r16 d, rm16 l, imm8 r ) { emit( mode16b, 0x6B, _r_r_rm(d,l), r ); return *this; }
			assembler& imul( r32 d, rm32 l, imm8 r ) { emit( mode32b, 0x6B, _r_r_rm(d,l), r ); return *this; }
			assembler& imul( r16 l, imm8 r ) { emit( mode16b, 0x6B, _r(l), r ); return *this; }
			assembler& imul( r32 l, imm8 r ) { emit( mode32b, 0x6B, _r(l), r ); return *this; }
			assembler& imul( r16 d, rm16 l, imm16 r ) { emit( mode16b, 0x69, _r_r_rm(d,l), r ); return *this; }
			assembler& imul( r32 d, rm32 l, imm32 r ) { emit( mode32b, 0x69, _r_r_rm(d,l), r ); return *this; }
			assembler& imul( r16 l, imm16 r ) { emit( mode16b, 0x69, _r(l), r ); return *this; }
			assembler& imul( r32 l, imm32 r ) { emit( mode32b, 0x69, _r(l), r ); return *this; }
			
			assembler& mov( rm8  l, imm   r ) { emit(          0x80, _7(l), imm8 (r) ); return *this; }
			assembler& mov( rm16 l, imm   r ) { emit( mode16b, 0x81, _7(l), imm16(r) ); return *this; }
			assembler& mov( rm32 l, imm   r ) { emit( mode32b, 0x81, _7(l), imm32(r) ); return *this; }
			assembler& mov( rm8  l, r8    r ) { emit(          0x88, _r_rm_r(l,r)    ); return *this; }
			assembler& mov( rm16 l, r16   r ) { emit( mode16b, 0x89, _r_rm_r(l,r)    ); return *this; }
			assembler& mov( rm32 l, r32   r ) { emit( mode32b, 0x89, _r_rm_r(l,r)    ); return *this; }
			assembler& mov( r8   l, mem8  r ) { emit(          0x8A, _r_r_rm(l,r)    ); return *this; }
			assembler& mov( r16  l, mem16 r ) { emit( mode16b, 0x8B, _r_r_rm(l,r)    ); return *this; }
			assembler& mov( r32  l, mem32 r ) { emit( mode32b, 0x8B, _r_r_rm(l,r)    ); return *this; }
			
			assembler& or ( rm8  l, imm   r ) { emit(          0x80, _1(l), imm8 (r) ); return *this; }
			assembler& or ( rm16 l, imm   r ) { emit( mode16b, 0x81, _1(l), imm16(r) ); return *this; }
			assembler& or ( rm32 l, imm   r ) { emit( mode32b, 0x81, _1(l), imm32(r) ); return *this; }
			assembler& or ( rm8  l, r8    r ) { emit(          0x08, _r_rm_r(l,r)    ); return *this; }
			assembler& or ( rm16 l, r16   r ) { emit( mode16b, 0x09, _r_rm_r(l,r)    ); return *this; }
			assembler& or ( rm32 l, r32   r ) { emit( mode32b, 0x09, _r_rm_r(l,r)    ); return *this; }
			assembler& or ( r8   l, mem8  r ) { emit(          0x0A, _r_r_rm(l,r)    ); return *this; }
			assembler& or ( r16  l, mem16 r ) { emit( mode16b, 0x0B, _r_r_rm(l,r)    ); return *this; }
			assembler& or ( r32  l, mem32 r ) { emit( mode32b, 0x0B, _r_r_rm(l,r)    ); return *this; }
			
			assembler& sub( rm8  l, imm   r ) { emit(          0x80, _5(l), imm8 (r) ); return *this; }
			assembler& sub( rm16 l, imm   r ) { emit( mode16b, 0x81, _5(l), imm16(r) ); return *this; }
			assembler& sub( rm32 l, imm   r ) { emit( mode32b, 0x81, _5(l), imm32(r) ); return *this; }
			assembler& sub( rm8  l, r8    r ) { emit(          0x28, _r_rm_r(l,r)    ); return *this; }
			assembler& sub( rm16 l, r16   r ) { emit( mode16b, 0x29, _r_rm_r(l,r)    ); return *this; }
			assembler& sub( rm32 l, r32   r ) { emit( mode32b, 0x29, _r_rm_r(l,r)    ); return *this; }
			assembler& sub( r8   l, mem8  r ) { emit(          0x2A, _r_r_rm(l,r)    ); return *this; }
			assembler& sub( r16  l, mem16 r ) { emit( mode16b, 0x2B, _r_r_rm(l,r)    ); return *this; }
			assembler& sub( r32  l, mem32 r ) { emit( mode32b, 0x2B, _r_r_rm(l,r)    ); return *this; }
			
			assembler& xor( rm8  l, imm   r ) { emit(          0x80, _6(l), imm8 (r) ); return *this; }
			assembler& xor( rm16 l, imm   r ) { emit( mode16b, 0x81, _6(l), imm16(r) ); return *this; }
			assembler& xor( rm32 l, imm   r ) { emit( mode32b, 0x81, _6(l), imm32(r) ); return *this; }
			assembler& xor( rm8  l, r8    r ) { emit(          0x30, _r_rm_r(l,r)    ); return *this; }
			assembler& xor( rm16 l, r16   r ) { emit( mode16b, 0x31, _r_rm_r(l,r)    ); return *this; }
			assembler& xor( rm32 l, r32   r ) { emit( mode32b, 0x31, _r_rm_r(l,r)    ); return *this; }
			assembler& xor( r8   l, mem8  r ) { emit(          0x32, _r_r_rm(l,r)    ); return *this; }
			assembler& xor( r16  l, mem16 r ) { emit( mode16b, 0x33, _r_r_rm(l,r)    ); return *this; }
			assembler& xor( r32  l, mem32 r ) { emit( mode32b, 0x33, _r_r_rm(l,r)    ); return *this; }
		};
	}
}

#endif //ndef IG_BRAIN_X86_ASSEMBLER
