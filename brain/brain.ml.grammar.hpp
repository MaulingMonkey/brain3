#ifndef IG_BRAIN_ML_GRAMMAR
#define IG_BRAIN_ML_GRAMMAR

#include "brain.ml.ast.hpp"
namespace brain {
	namespace ml {
		class grammar;
		#define GRAMMAR_ID   grammar
		#define GRAMMAR_EXPR expression
		#define GRAMMAR_FILE "brain.ml.grammar.hpp"
		#include GRAMMAR_GENERATE()
	}
}

#elif defined( GENERATING_GRAMMAR )

	LEAF_RULE( l_flt , ((double,value)), real_p[value=arg1] ) { return literal_expression<double>(value); }
	LEAF_RULE( l_int , ((int   ,value)),  int_p[value=arg1] ) { return literal_expression<int>   (value); }
	LEAF_RULE( l_var , ((std::string,value)), ((alpha_p|'_') >> *(alnum_p|'_'))[value=construct_<std::string>(arg1,arg2)] ) { return variable_expression(value); }
	RULE( op_parens, (subexpr), '(' >> top[subexpr=arg1] >> ')' ) { return subexpr; }
	RULE_GROUP( precd0, (op_parens)(l_flt)(l_int)(l_var) );

	// LTOR associative operators:
	RULE( precd1, (root), precd0[root=arg1] >> *(precd1_op_tail[root=arg1]) ) { return root; }
	RULE( div_op_tail, (lhs)(rhs), eps_p[lhs=precd1.root] >> '/' >> precd0[rhs=arg1] ) { return div_expression( lhs, rhs ); }
	RULE( mul_op_tail, (lhs)(rhs), eps_p[lhs=precd1.root] >> '*' >> precd0[rhs=arg1] ) { return mul_expression( lhs, rhs ); }
	RULE_GROUP( precd1_op_tail, (mul_op_tail)(div_op_tail) );

	RULE( precd2, (root), precd1[root=arg1] >> *(precd2_op_tail[root=arg1]) ) { return root; }
	RULE( add_op_tail, (lhs)(rhs), eps_p[lhs=precd2.root] >> '+' >> precd1[rhs=arg1] ) { return add_expression( lhs, rhs ); }
	RULE( sub_op_tail, (lhs)(rhs), eps_p[lhs=precd2.root] >> '-' >> precd1[rhs=arg1] ) { return sub_expression( lhs, rhs ); }
	RULE_GROUP( precd2_op_tail, (add_op_tail)(sub_op_tail) );

	RULE_GROUP( top, (precd2) );

#if 0
	RULE( precd3, (root), precd2[root=arg1] >> *(precd3_op_tail[root=arg1]) ) { return root; }
	RULE( pow_op_tail, (lhs)(rhs), eps_p[lhs=precd3.root] >> '^' >> precd3[rhs=arg1] ) { return pow_expression( lhs, rhs ); }
	RULE_GROUP( precd3_op_tail, (pow_op_tail) );
#endif

#if 0
	// Whoops, these were RTOL associative:
	RULE( op_div, (lhs)(rhs), precd0[lhs=arg1] >> '/' >> precd1[rhs=arg1] ) { return div_expression( lhs, rhs ); }
	RULE( op_mul, (lhs)(rhs), precd0[lhs=arg1] >> '*' >> precd1[rhs=arg1] ) { return mul_expression( lhs, rhs ); }
	RULE_GROUP( precd1, (op_div)(op_mul)(precd0) );
	RULE( op_add, (lhs)(rhs), precd1[lhs=arg1] >> '+' >> precd2[rhs=arg1] ) { return add_expression( lhs, rhs ); }
	RULE( op_sub, (lhs)(rhs), precd1[lhs=arg1] >> '-' >> precd2[rhs=arg1] ) { return sub_expression( lhs, rhs ); }
	RULE_GROUP( precd2, (op_add)(op_sub)(precd1) );
	RULE_GROUP( top, (precd2) );
#endif

	START_AT( top );

#endif // def IG_BRAIN_ML_GRAMMAR || GENERATING_GRAMMAR
