#ifndef IG_BRAIN_ML2_WHITESPACE_GRAMMAR
#define IG_BRAIN_ML2_WHITESPACE_GRAMMAR

#include "brain.ml2.ast.hpp"

namespace brain {
	namespace ml2 {
		class whitespace_grammar;

		#define GRAMMAR_ID   whitespace_grammar
		#define GRAMMAR_EXPR expression
		#define GRAMMAR_FILE "brain.ml2.whitespace_grammar.hpp"
		#include GRAMMAR_GENERATE()

		namespace {
			const whitespace_grammar whitespace;
		}
	}
}

#elif defined( GENERATING_GRAMMAR )

	RULE( line_comment,, (ch_p('#')|"//") >> +(anychar_p-'\n') >> '\n' ) { return nothing; }
	RULE( miscellanious,, +space_p ) { return nothing; }
	RULE_GROUP( top , (line_comment)(miscellanious) );

	START_AT(top);

#endif //ndef IG_BRAIN_ML2_WHITESPACE_GRAMMAR
