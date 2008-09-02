#ifndef IG_BRAIN_ML2_AST
#define IG_BRAIN_ML2_AST

#include "brain.grammar.hpp"
#include "brain.ml2.value.hpp"
#include "brain.ml2.visitors.hpp"
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/variant.hpp>
#include <map>
#include <vector>
#include <ostream>
#include <cmath>

namespace brain {
	namespace ml2 {
		class expression {
		public:
			typedef value calculate_result_type;
			typedef std::map<std::string,value> calculate_args_type;
			typedef grammar::expression_ref<expression> ref;

			// Manditory overloads:
			virtual ~expression() {}
			virtual calculate_result_type calculate( const calculate_args_type& ) const = 0;
			virtual int precedence() const = 0;
			virtual void print_to( std::ostream& os ) const = 0;

			// Eww fat base class:
			virtual void add_argument( const expression::ref& ) { throw std::runtime_error( (std::string)"Unexpected " + typeid(*this).name() + " :: add_argument( ... )" ); }

			// Utility functions:
			friend std::ostream& operator<<( std::ostream& os, const expression& expr ) { expr.print_to(os); return os; }
		};

		typedef grammar::expression_ref<expression> expr_ref_t;

		class value_expression : public expression {
			value data;
		public:
			typedef grammar::expression_ref<value_expression> ref;

			value_expression( value v ): data(v) {}
			virtual calculate_result_type calculate( const calculate_args_type& ) const { return data; }
			virtual int precedence() const { return 0; }
			virtual void print_to( std::ostream& os ) const { os << data; }
		};

		class variable_expression : public expression {
			std::string name;
		public:
			typedef grammar::expression_ref<variable_expression> ref;

			variable_expression( const std::string& name ): name(name) {}
			virtual calculate_result_type calculate( const calculate_args_type& args ) const { return args.find(name)->second; }
			virtual int precedence() const { return 0; }
			virtual void print_to( std::ostream& os ) const { os << name; }
		};

		class call_expression : public expression {
			variable_expression::ref callee;
			std::vector<expression::ref> args;
		public:
			typedef grammar::expression_ref<call_expression> ref;

			call_expression( const expression::ref& callee ): callee(callee) {}
			virtual calculate_result_type calculate( const calculate_args_type& calc_args ) const {
				value f = callee->calculate(calc_args);

				std::vector<value> results;
				BOOST_FOREACH( const expression::ref& arg, args ) results.push_back( arg->calculate(calc_args) );
				
				return apply_visitor( call_visitor(results), f );
			}
			
			virtual int precedence() const { return 1; }
			virtual void print_to( std::ostream& os ) const {
				os << *callee << "(";
				for ( size_t i = 0 ; i < args.size() ; ++i ) os << (i?",":"") << *args[i];
				os << ")";
			}
			virtual void add_argument( const expression::ref& arg ) { args.push_back(arg); }
		};

		template < typename CalcV, char op, size_t preced, bool ltor = true > class binary_expression : public expression {
			expr_ref_t lhs, rhs;
		public:
			binary_expression( expr_ref_t lhs, expr_ref_t rhs ): lhs(lhs), rhs(rhs) {}
			virtual int precedence() const { return preced; }
			virtual void print_to( std::ostream& os ) const {
				bool lp = ltor ? lhs->precedence() >  precedence() : lhs->precedence() >= precedence();
				bool rp = ltor ? rhs->precedence() >= precedence() : rhs->precedence() >  precedence();
				os << (lp?"(":"") << *lhs << (lp?")":"") << op << (rp?"(":"") << *rhs << (rp?")":"");
			}
			virtual calculate_result_type calculate( const calculate_args_type& args ) const {
				return apply_visitor(CalcV(),lhs->calculate(args),rhs->calculate(args));
			}
		};

		typedef binary_expression<add_visitor,'+',3      > add_expression;
		typedef binary_expression<sub_visitor,'-',3      > sub_expression;
		typedef binary_expression<mul_visitor,'*',2      > mul_expression;
		typedef binary_expression<div_visitor,'/',2      > div_expression;
		typedef binary_expression<pow_visitor,'^',4,false> pow_expression;
	}
}

#endif // def IG_BRAIN_ML2_AST || GENERATING_GRAMMAR
