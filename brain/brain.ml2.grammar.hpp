#ifndef IG_BRAIN_ML2_GRAMMAR
#define IG_BRAIN_ML2_GRAMMAR

#include "brain.ml2.ast.hpp"
#include "brain.ml2.whitespace_grammar.hpp"

namespace brain {
	namespace ml2 {
		class grammar;

		#define GRAMMAR_ID   grammar
		#define GRAMMAR_EXPR expression
		#define GRAMMAR_FILE "brain.ml2.grammar.hpp"
		#include GRAMMAR_GENERATE()
	}
}

#elif defined( GENERATING_GRAMMAR )

	// Precd 0: Unassociative (var,23.4,(...))
	LEAF_RULE( l_int , ((double,value)), real_p[value=arg1] ) { return value_expression(value); }
	LEAF_RULE( l_var , ((std::string,value)), ((alpha_p|'_') >> *(alnum_p|'_'))[value=construct_<std::string>(arg1,arg2)] ) { return variable_expression(value); }
	RULE( op_parens, (subexpr), '(' >> precdN[subexpr=arg1] >> ')' ) { return subexpr; }
	RULE_GROUP( precd0, (op_parens)(l_int)(l_var) );

	// Precd 1: LTOR associative (f(...))
	RULE( call_op_init, (f), eps_p[f=call_op_tail.f] ) { return call_expression(f); }
	RULE( call_op_arg, (call)(arg), eps_p[call=call_op_tail.call] >> precdN[arg=arg1] ) { call->add_argument(arg); return arg; }
	RULE( precd1, (root), precd0[root=arg1] >> *(precd1_op_tail[root=arg1]) ) { return root; }
	RULE( call_op_tail, (f)(call), eps_p[f=precd1.root] >> '(' >> call_op_init[call=arg1] >> !list_p(call_op_arg,',') >> ')' ) { return call; }
	RULE_GROUP( precd1_op_tail, (call_op_tail) );
	//RULE_GROUP( precd1, (precd0) );

	// Precd 2: LTOR associative (*,/)
	RULE( precd2, (root), precd1[root=arg1] >> *(precd2_op_tail[root=arg1]) ) { return root; }
	RULE( div_op_tail, (lhs)(rhs), eps_p[lhs=precd2.root] >> '/' >> precd1[rhs=arg1] ) { return div_expression( lhs, rhs ); }
	RULE( mul_op_tail, (lhs)(rhs), eps_p[lhs=precd2.root] >> '*' >> precd1[rhs=arg1] ) { return mul_expression( lhs, rhs ); }
	RULE_GROUP( precd2_op_tail, (mul_op_tail)(div_op_tail) );

	// Precd 3: LTOR associative (+,-)
	RULE( precd3, (root), precd2[root=arg1] >> *(precd3_op_tail[root=arg1]) ) { return root; }
	RULE( add_op_tail, (lhs)(rhs), eps_p[lhs=precd3.root] >> '+' >> precd2[rhs=arg1] ) { return add_expression( lhs, rhs ); }
	RULE( sub_op_tail, (lhs)(rhs), eps_p[lhs=precd3.root] >> '-' >> precd2[rhs=arg1] ) { return sub_expression( lhs, rhs ); }
	RULE_GROUP( precd3_op_tail, (add_op_tail)(sub_op_tail) );

	// Precd 4: RTOL associative (**)
	RULE( precd4, (root), precd3[root=arg1] >> *(precd4_op_tail[root=arg1]) ) { return root; }
	RULE( pow_op_tail, (lhs)(rhs), eps_p[lhs=precd4.root] >> "**" >> precd4[rhs=arg1] ) { return pow_expression( lhs, rhs ); }
	RULE_GROUP( precd4_op_tail, (pow_op_tail) );

	// Precd N:
	RULE_GROUP( precdN, (precd4) );

	// Definitions:
	RULE( statement, (f)(s), eps_p[f=scope.f] >> precdN[s=arg1] >> ';' ) { f->add_statement(s); return s; }

	RULE_GROUP( function_id      , (l_var) );
	RULE_GROUP( function_param_id, (l_var) );
	RULE( function_def_preamble , (name), lexeme_d["def" >> whitespace] >> function_id[name=arg1] ) { return function_def_expression(name); }
	RULE( function_def_parameter, (f)(p), function_param_id[p=arg1][f=scope.f] ) { f->add_parameter(p); return nothing; }
	RULE( function_def          , (f)(oldf)
		,	eps_p[oldf=scope.f] >> function_def_preamble[scope.f=f=arg1] >> '(' >> !list_p(function_def_parameter,',') >> ')'
		>>	(	(!str_p(":=") >> '{'  >> +statement >> '}')
			||	(!str_p(":=") >> lexeme_d["do">>whitespace] >> +statement >> lexeme_d["end">>whitespace])
			||	(str_p(":=")  >> statement)
			)
		||	( eps_p[scope.f=oldf] >> nothing_p )
	) {
		oldf->add_statement(f);
		return nothing;
	}
	
	RULE_GROUP( def, (function_def) );
	
	RULE( create_main_scope_id,, eps_p ) { return variable_expression(":main:"); }
	RULE( create_main_scope,(name), create_main_scope_id[name=arg1] ) { return function_def_expression(name); }
	RULE( scope, (f), create_main_scope[f=arg1] >> +( def | statement ) ) { return f; }
	START_AT( scope );

#endif //ndef IG_BRAIN_ML2_GRAMMAR
