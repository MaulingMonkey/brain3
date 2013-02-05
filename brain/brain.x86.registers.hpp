#ifndef IG_BRAIN_X86_REGISTERS
#define IG_BRAIN_X86_REGISTERS

#include <industry/enum.hpp>
#include <boost/variant.hpp>

namespace brain {
	namespace x86 {
		// Arranged by R/M identifiers:                                000  001  010  011  100  101  110  111
		INDUSTRY_ENUM(  r8, (os)(duplicate_constants_in_outer_scope), ( al)( cl)( dl)( bl)( ah)( ch)( dh)( bh) );
		INDUSTRY_ENUM( r16, (os)(duplicate_constants_in_outer_scope), ( ax)( cx)( dx)( bx)( sp)( bp)( si)( di) );
		INDUSTRY_ENUM( r32, (os)(duplicate_constants_in_outer_scope), (eax)(ecx)(edx)(ebx)(esp)(ebp)(esi)(edi) );
		
		typedef boost::variant< r8, r16, r32 > reg;
	}
}

#endif //ndef IG_BRAIN_X86_REGISTERS
