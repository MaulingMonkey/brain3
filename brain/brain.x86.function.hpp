#ifndef IG_BRAIN_X86_FUNCTION
#define IG_BRAIN_X86_FUNCTION

#include "brain.x86.assembler.hpp"
#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor.hpp>
#include <boost/bind.hpp>

namespace brain {
	namespace x86 {
		template < typename Signature > class function;

/*----------------------------------------------------------------------------------------------------*/
#define FUNCTION_ARITY_CONVENTION( z, n, convention )                                                  \
	template < typename R  BOOST_PP_ENUM_TRAILING_PARAMS(n,typename A) >                               \
	class function< R convention (BOOST_PP_ENUM_PARAMS(n,A)) > : public assembler {                    \
	public:                                                                                            \
		R operator()( BOOST_PP_ENUM_BINARY_PARAMS(n,A,a) ) const {                                     \
			return ((R (convention *)(BOOST_PP_ENUM_PARAMS(n,A)))&code->at(0))(BOOST_PP_ENUM_PARAMS(n,a)); \
		}                                                                                              \
	};                                                                                                 \
/*----------------------------------------------------------------------------------------------------*/
#define FUNCTION_ARITY( z, n, unused ) BOOST_PP_SEQ_FOR_EACH( FUNCTION_ARITY_CONVENTION, n, (__cdecl)(__stdcall)(__fastcall) )
BOOST_PP_REPEAT( 10, FUNCTION_ARITY, ~ )
#undef FUNCTION_ARITY
#undef FUNCTION_ARITY_CONVENTION

	}
}

#endif //ndef IG_BRAIN_X86_FUNCTION
