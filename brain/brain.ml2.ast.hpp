#ifndef IG_BRAIN_ML2_AST
#define IG_BRAIN_ML2_AST

#include "brain.grammar.hpp"
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/variant.hpp>
#include <map>
#include <vector>
#include <ostream>
#include <cmath>

namespace brain {
	namespace ml2 {
		class value {
			typedef boost::variant
				< boost::function< value() >
				, boost::function< value(const std::vector<value>&) >
				> function_types;

			typedef boost::variant
				< double
				, int
				> value_types;
			
			boost::variant< void*, function_types, value_types > data;
		public:
			value() : data() {}
			value( const boost::function< value(                         ) >& f ): data(function_types(f)) {}
			value( const boost::function< value(const std::vector<value>&) >& f ): data(function_types(f)) {}
			value( double d ): data(value_types(d)) {}
			value( int    i ): data(value_types(i)) {}

			value& operator=( const value                     & o ) { data = o.data; return *this; }
			value& operator=( const boost::function< value(                         ) >& f ) { data = function_types(f); return *this; }
			value& operator=( const boost::function< value(const std::vector<value>&) >& f ) { data = function_types(f); return *this; }
			value& operator=( double d ) { data = value_types(d); return *this; }
			value& operator=( int    i ) { data = value_types(i); return *this; }

			template < typename F > friend typename F::result_type apply_value_visitor( F f, const value& v ) {
				const value_types* value = boost::get<const value_types>(&v.data);
				if (!value) throw std::runtime_error( "Expected a value" );
				return boost::apply_visitor( f, *value );
			}
			template < typename F > friend typename F::result_type apply_function_visitor( F f, const value& v ) {
				const function_types* func = boost::get<const function_types>(&v.data);
				if (!func) throw std::runtime_error( "Expected a function" );
				return boost::apply_visitor( f, *func );
			}
			template < typename F > friend typename F::result_type apply_value_value_visitor( F f, const value& lhs, const value& rhs ) {
				const value_types* lhs_ = boost::get<const value_types>(&lhs.data);
				const value_types* rhs_ = boost::get<const value_types>(&rhs.data);
				if (!lhs_ || !rhs_) throw std::runtime_error( "Expected a value" );
				return boost::apply_visitor( f, *lhs_, *rhs_ );
			}
			struct print_to { std::ostream& os; print_to( std::ostream& os ): os(os) {} typedef void result_type; template < typename T > void operator()( const T& v ) { os << v; } };
			friend std::ostream& operator<<( std::ostream& os, const value& v ) {
				if ( boost::get<const value_types>(&v.data) ) apply_value_visitor( print_to(os), v );
				else os << "void";
				return os;
			}
		};

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

			class call_f {
				const std::vector<value>& args;
			public:
				call_f( const std::vector<value>& args ): args(args) {}

				typedef value result_type;
				result_type operator()( const boost::function< value () >& f ) const {
					if (!args.empty()) throw std::runtime_error( "Function doesn't expect any arguments, yet was passed some" );
					return f();
				}
				result_type operator()( const boost::function< value ( const std::vector<value>& ) >& f ) const { return f(args); }
			};
		public:
			typedef grammar::expression_ref<call_expression> ref;

			call_expression( const expression::ref& callee ): callee(callee) {}
			virtual calculate_result_type calculate( const calculate_args_type& calc_args ) const {
				calculate_result_type f = callee->calculate(calc_args);

				std::vector<calculate_result_type> arg_results;
				BOOST_FOREACH( const expression::ref& arg, args ) arg_results.push_back( arg->calculate(calc_args) );

				return apply_function_visitor( call_f(arg_results), f );
			}
			
			virtual int precedence() const { return 1; }
			virtual void print_to( std::ostream& os ) const {
				os << *callee << "(";
				for ( size_t i = 0 ; i < args.size() ; ++i ) os << (i?",":"") << *args[i];
				os << ")";
			}
			virtual void add_argument( const expression::ref& arg ) { args.push_back(arg); }
		};

		template < typename SelfT, char op, size_t preced, bool ltor = true > class binary_expression : public expression {
		protected:
			struct calc_f {
				typedef expression::calculate_result_type result_type;
				template < typename L, typename R > result_type operator()( L l, R r ) const {
					return SelfT::do_calculate(l,r);
				}
			};

			expr_ref_t lhs, rhs;
		public:
			typedef grammar::expression_ref<binary_expression> ref;

			binary_expression( expr_ref_t lhs, expr_ref_t rhs ): lhs(lhs), rhs(rhs) {}
			virtual int precedence() const { return preced; }
			virtual void print_to( std::ostream& os ) const {
				bool lp = ltor ? lhs->precedence() >  precedence() : lhs->precedence() >= precedence();
				bool rp = ltor ? rhs->precedence() >= precedence() : rhs->precedence() >  precedence();
				os << (lp?"(":"") << *lhs << (lp?")":"") << op << (rp?"(":"") << *rhs << (rp?")":"");
			}
			virtual calculate_result_type calculate( const calculate_args_type& args ) const {
				return apply_value_value_visitor(calc_f(),lhs->calculate(args),rhs->calculate(args));
			}
		};

		class add_expression : public binary_expression<add_expression,'+',3> {
		public:
			typedef grammar::expression_ref<add_expression> ref;

			add_expression( expr_ref_t lhs, expr_ref_t rhs ): binary_expression(lhs,rhs) {}
			template < typename L, typename R > static calculate_result_type do_calculate( L l, R r ) { return l+r; }
		};

		class sub_expression : public binary_expression<sub_expression,'-',3> {
		public:
			typedef grammar::expression_ref<sub_expression> ref;

			sub_expression( expr_ref_t lhs, expr_ref_t rhs ): binary_expression(lhs,rhs) {}
			template < typename L, typename R > static calculate_result_type do_calculate( L l, R r ) { return l-r; }
		};

		class div_expression : public binary_expression<div_expression,'/',2> {
		public:
			typedef grammar::expression_ref<div_expression> ref;

			div_expression( expr_ref_t lhs, expr_ref_t rhs ): binary_expression(lhs,rhs) {}
			template < typename L, typename R > static calculate_result_type do_calculate( L l, R r ) { return l/r; }
		};

		class mul_expression : public binary_expression<mul_expression,'*',2> {
		public:
			typedef grammar::expression_ref<mul_expression> ref;

			mul_expression( expr_ref_t lhs, expr_ref_t rhs ): binary_expression(lhs,rhs) {}
			template < typename L, typename R > static calculate_result_type do_calculate( L l, R r ) { return l*r; }
		};

		class pow_expression : public binary_expression<pow_expression,'^',4,false> {
		public:
			typedef grammar::expression_ref<pow_expression> ref;

			pow_expression( expr_ref_t lhs, expr_ref_t rhs ): binary_expression(lhs,rhs) {}
			template < typename L, typename R > static calculate_result_type do_calculate( L l, R r ) { return std::pow(double(l),r); }
		};
	}
}

#endif // def IG_BRAIN_ML2_AST || GENERATING_GRAMMAR
