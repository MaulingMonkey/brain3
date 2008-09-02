#ifndef IG_BRAIN_ML2_AST
#define IG_BRAIN_ML2_AST

#include "brain.grammar.hpp"
#include "brain.ml2.value.hpp"
#include "brain.ml2.visitors.hpp"
#include <boost/bind.hpp>
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
			typedef std::map<std::string,value> calculate_args_type;
			typedef grammar::expression_ref<expression> ref;

			// Manditory overloads:
			virtual ~expression() {}
			virtual value calculate( calculate_args_type& ) const = 0;
			virtual int precedence() const = 0;
			virtual void print_to( std::ostream& os ) const = 0;

			// Eww fat base class:
			virtual void add_argument(  const ref& ) { throw std::runtime_error( (std::string)"Unexpected " + typeid(*this).name() + " :: add_argument( ... )" ); }
			virtual void add_parameter( const ref& ) { throw std::runtime_error( (std::string)"Unexpected " + typeid(*this).name() + " :: add_parameter( ... )" ); }
			virtual void add_statement( const ref& ) { throw std::runtime_error( (std::string)"Unexpected " + typeid(*this).name() + " :: add_statement( ... )" ); }

			// Utility functions:
			friend std::ostream& operator<<( std::ostream& os, const expression& expr ) { expr.print_to(os); return os; }
		};

		class value_expression : public expression {
			value data;
		public:
			typedef grammar::expression_ref<value_expression> ref;

			value_expression( value v ): data(v) {}
			virtual value calculate( calculate_args_type& ) const { return data; }
			virtual int precedence() const { return 0; }
			virtual void print_to( std::ostream& os ) const { os << data; }
		};

		class variable_expression : public expression {
		public:
			typedef grammar::expression_ref<variable_expression> ref;

			const std::string name;

			variable_expression( const std::string& name ): name(name) {}
			virtual value calculate( calculate_args_type& args ) const { return args.find(name)->second; }
			virtual int precedence() const { return 0; }
			virtual void print_to( std::ostream& os ) const { os << name; }
		};

		class call_expression : public expression {
			variable_expression::ref callee;
			std::vector<ref> args;
		public:
			call_expression( const ref& callee ): callee(callee) {}
			virtual value calculate( calculate_args_type& calc_args ) const {
				value f = callee->calculate(calc_args);

				std::vector<value> results;
				BOOST_FOREACH( const ref& arg, args ) results.push_back( arg->calculate(calc_args) );
				
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
			ref lhs, rhs;
		public:
			binary_expression( ref lhs, ref rhs ): lhs(lhs), rhs(rhs) {}
			virtual int precedence() const { return preced; }
			virtual void print_to( std::ostream& os ) const {
				bool lp = ltor ? lhs->precedence() >  precedence() : lhs->precedence() >= precedence();
				bool rp = ltor ? rhs->precedence() >= precedence() : rhs->precedence() >  precedence();
				os << (lp?"(":"") << *lhs << (lp?")":"") << op << (rp?"(":"") << *rhs << (rp?")":"");
			}
			virtual value calculate( calculate_args_type& args ) const {
				return apply_visitor(CalcV(),lhs->calculate(args),rhs->calculate(args));
			}
		};

		typedef binary_expression<add_visitor,'+',3      > add_expression;
		typedef binary_expression<sub_visitor,'-',3      > sub_expression;
		typedef binary_expression<mul_visitor,'*',2      > mul_expression;
		typedef binary_expression<div_visitor,'/',2      > div_expression;
		typedef binary_expression<pow_visitor,'^',4,false> pow_expression;

		class function_def_expression : public expression {
			variable_expression::ref                name;
			std::vector<variable_expression::ref>   parameters;
			std::vector<ref>                        statements;
			mutable calculate_args_type             context;

			value execute( const std::vector<value>& args ) const {
				if ( args.size() != parameters.size() ) {
					std::ostringstream ss; ss << "Expected " << parameters.size() << " argument(s), got " << args.size() << ".";
					throw std::runtime_error(ss.str());
				}

				std::vector<value>::const_iterator arg = args.begin();
				BOOST_FOREACH( const variable_expression::ref& p, parameters ) {
					context[p->name] = *arg++;
				}
				value result;
				BOOST_FOREACH( const ref& s, statements ) result = s->calculate(context);
				return result;
			}
		public:
			function_def_expression( const variable_expression::ref& name ): name(name) {}
			virtual value calculate( calculate_args_type& args ) const {
				value execute; execute.bind( &function_def_expression::execute, this );
				
				context = args;
				args[name->name] = execute;
				return execute;
			}
			virtual int precedence() const { return 9001; }
			virtual void print_to( std::ostream& os ) const {
				os << "<function " << *name << "( ";
				int i = 0;
				BOOST_FOREACH( const variable_expression::ref& p, parameters ) { os << ((i++)?", ":"") << p->name; }
				os << " ) := { ";
				BOOST_FOREACH( const ref& s, statements ) { os << *s << "; "; }
				os << "}>";
			}
			virtual void add_parameter( const ref& parameter ) { parameters.push_back(parameter); }
			virtual void add_statement( const ref& statement ) { statements.push_back(statement); }
		};
	}
}

#endif // def IG_BRAIN_ML2_AST || GENERATING_GRAMMAR
