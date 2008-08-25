#ifndef IG_BRAIN_ML_AST
#define IG_BRAIN_ML_AST

#include "brain.grammar.hpp"
#include "brain.x86.assembler.hpp"
#include "brain.x86.registers.hpp"
#include <industry/enum.hpp>
#include <boost/function.hpp>
#include <boost/variant.hpp>
#include <map>
#include <set>
#include <ostream>
#include <string>
#include <cmath>

namespace brain {
	namespace ml {
		INDUSTRY_BASIC_ENUM( expression_type, (floating)(integer) );

		struct x86_compile_args {
			x86::assembler& assembler;
			std::map<std::string,boost::function<void(x86::assembler,x86::r32)> > args;

			x86_compile_args( x86::assembler& assembler ): assembler(assembler) {}
		};

		class expression {
		public:
			typedef boost::variant<double,int> calculate_result_type;
			typedef std::map<std::string,calculate_result_type> calculate_args_type;

			virtual ~expression() {}
			virtual calculate_result_type calculate( const calculate_args_type& ) const = 0;
			virtual expression_type       type( const calculate_args_type& args ) const {
				calculate_result_type r = calculate(args);
				if ( boost::get<int>(&r) ) return expression_type::integer;
				else                       return expression_type::floating;
			}
			virtual int precedence() const = 0;
			virtual void print_to( std::ostream& os ) const = 0;
			friend std::ostream& operator<<( std::ostream& os, const expression& expr ) { expr.print_to(os); return os; }


			virtual bool x86_must_use_eax() const { return false; }
			virtual bool x86_must_use_edx() const { return false; }
			virtual void x86_compile( const x86_compile_args& args, x86::r32 result_in, const std::set<x86::r32>& clobberable ) const = 0;
		};

		typedef grammar::expression_ref<expression> expr_ref_t;

		template < typename T >
		class literal_expression : public expression {
			T value;
		public:
			literal_expression( T value ): value(value) {}
			virtual calculate_result_type calculate( const calculate_args_type& ) const { return value; }
			virtual int precedence() const { return 0; }
			virtual void print_to( std::ostream& os ) const { os << value; }
			virtual void x86_compile( const x86_compile_args& args, x86::r32 result_in, const std::set<x86::r32>& clobberable ) const {
				args.assembler
					.mov(result_in,(int)value)
					;
			}
		};

		class variable_expression : public expression {
			std::string name;
		public:
			variable_expression( const std::string& name ): name(name) {}
			virtual calculate_result_type calculate( const calculate_args_type& args ) const { return args.find(name)->second; }
			virtual int precedence() const { return 0; }
			virtual void print_to( std::ostream& os ) const { os << name; }
			virtual void x86_compile( const x86_compile_args& args, x86::r32 result_in, const std::set<x86::r32>& clobberable ) const {
				args.args.find(name)->second(args.assembler,result_in);
			}
		};

		template < char op, size_t preced, bool ltor = true > class binary_expression : public expression {
		protected:
			expr_ref_t lhs, rhs;
		public:
			binary_expression( expr_ref_t lhs, expr_ref_t rhs ): lhs(lhs), rhs(rhs) {}
			virtual int precedence() const { return preced; }
			virtual void print_to( std::ostream& os ) const {
				bool lp = ltor ? lhs->precedence() >  precedence() : lhs->precedence() >= precedence();
				bool rp = ltor ? rhs->precedence() >= precedence() : rhs->precedence() >  precedence();
				os << (lp?"(":"") << *lhs << (lp?")":"") << op << (rp?"(":"") << *rhs << (rp?")":"");
			}
			virtual bool x86_must_use_eax() const { return lhs->x86_must_use_eax() && rhs->x86_must_use_eax(); }
			virtual bool x86_must_use_edx() const { return lhs->x86_must_use_edx() && rhs->x86_must_use_edx(); }
		};

		template < char op, size_t preced, bool commutative = true, bool ltor = true > class simple_binary_expression : public binary_expression<op,preced,ltor> {
		public:
			simple_binary_expression( expr_ref_t lhs, expr_ref_t rhs ): binary_expression(lhs,rhs) {}

			virtual void x86_compile( const x86_compile_args& args, x86::r32 result_in, const std::set<x86::r32>& clobberable ) const {
				using namespace x86;

				expr_ref_t lhs=this->lhs, rhs=this->rhs;
				if ( commutative
					&& !(dynamic_cast<mul_expression*>(&*lhs) || dynamic_cast<div_expression*>(&*lhs))
					&&  (dynamic_cast<mul_expression*>(&*rhs) || dynamic_cast<div_expression*>(&*rhs))
					) std::swap(lhs,rhs);

				lhs->x86_compile( args, result_in, clobberable );
				std::set<r32> rhs_clobberable(clobberable);
				rhs_clobberable.erase(result_in);
				if ( rhs_clobberable.empty() ) {
					r32 r = result_in==ecx?edx:ecx;
					args.assembler
						.push(r)
						;
					rhs->x86_compile( args, r, rhs_clobberable );
					x86_compile_op( args, result_in, r );
					args.assembler
						.pop(r)
						;
				} else {
					r32 r = *rhs_clobberable.begin();
					rhs->x86_compile( args, r, rhs_clobberable );
					x86_compile_op( args, result_in, r );
				}
			}
		protected:
			virtual void x86_compile_op( const x86_compile_args& args, x86::r32 l, x86::r32 r ) const = 0;
		};

		class add_expression : public simple_binary_expression<'+',2> {
			struct calc_f {
				typedef expression::calculate_result_type result_type;
				template < typename L, typename R > result_type operator()( L l, R r ) const { return l+r; }
			};
		public:
			add_expression( expr_ref_t lhs, expr_ref_t rhs ): simple_binary_expression(lhs,rhs) {}
			virtual calculate_result_type calculate( const calculate_args_type& args ) const {
				return boost::apply_visitor(calc_f(),lhs->calculate(args),rhs->calculate(args));
			}

			virtual void x86_compile_op( const x86_compile_args& args, x86::r32 l_and_result, x86::r32 r ) const {
				args.assembler
					.add( l_and_result, r )
					;
			}
		};

		class sub_expression : public simple_binary_expression<'-',2> {
			struct calc_f {
				typedef expression::calculate_result_type result_type;
				template < typename L, typename R > result_type operator()( L l, R r ) const { return l-r; }
			};
		public:
			sub_expression( expr_ref_t lhs, expr_ref_t rhs ): simple_binary_expression(lhs,rhs) {}
			virtual calculate_result_type calculate( const calculate_args_type& args ) const {
				return boost::apply_visitor(calc_f(),lhs->calculate(args),rhs->calculate(args));
			}

			virtual void x86_compile_op( const x86_compile_args& args, x86::r32 l_and_result, x86::r32 r ) const {
				args.assembler
					.sub( l_and_result, r )
					;
			}
		};

		class div_expression : public binary_expression<'/',1> {
			struct calc_f {
				typedef expression::calculate_result_type result_type;
				template < typename L, typename R > result_type operator()( L l, R r ) const { return l/r; }
			};
		public:
			div_expression( expr_ref_t lhs, expr_ref_t rhs ): binary_expression(lhs,rhs) {}
			virtual calculate_result_type calculate( const calculate_args_type& args ) const {
				return boost::apply_visitor(calc_f(),lhs->calculate(args),rhs->calculate(args));
			}

			virtual bool x86_must_use_eax() const { return true; }
			virtual bool x86_must_use_edx() const { return true; }
			
			virtual void x86_compile( const x86_compile_args& args, x86::r32 result_in, const std::set<x86::r32>& clobberable ) const {
				using namespace x86;
				
				bool preserve_eax = result_in != eax || clobberable.find(eax)==clobberable.end();
				bool preserve_edx = result_in != edx || clobberable.find(edx)==clobberable.end();
				if ( preserve_eax ) args.assembler.push(eax);
				if ( preserve_edx ) args.assembler.push(edx);

				lhs->x86_compile( args, eax, clobberable );
				args.assembler
					.xor(edx,edx)
					;

				std::set<r32> rhs_clobberable(clobberable);
				rhs_clobberable.erase(eax);
				rhs_clobberable.erase(edx);

				if ( rhs_clobberable.empty() ) {
					r32 r = result_in==ecx?ebx:ecx;
					args.assembler
						.push(r)
						;
					rhs->x86_compile( args, r, rhs_clobberable );
					args.assembler
						.idiv(r)
						.pop(r)
						;
				} else {
					r32 r = *rhs_clobberable.begin();
					rhs->x86_compile( args, r, rhs_clobberable );
					args.assembler
						.idiv(r)
						;
				}

				if ( result_in != eax ) args.assembler.mov(result_in,eax);
				if ( preserve_edx ) args.assembler.pop(edx);
				if ( preserve_eax ) args.assembler.pop(eax);
			}
		};

		class mul_expression : public binary_expression<'*',1> {
			struct calc_f {
				typedef expression::calculate_result_type result_type;
				template < typename L, typename R > result_type operator()( L l, R r ) const { return l*r; }
			};
		public:
			mul_expression( expr_ref_t lhs, expr_ref_t rhs ): binary_expression(lhs,rhs) {}
			virtual calculate_result_type calculate( const calculate_args_type& args ) const {
				return boost::apply_visitor(calc_f(),lhs->calculate(args),rhs->calculate(args));
			}

			virtual bool x86_must_use_eax() const { return true; }
			virtual bool x86_must_use_edx() const { return true; }
			
			virtual void x86_compile( const x86_compile_args& args, x86::r32 result_in, const std::set<x86::r32>& clobberable ) const {
				using namespace x86;

				expr_ref_t lhs=this->lhs, rhs=this->rhs;
				if (  !(dynamic_cast<mul_expression*>(&*lhs) || dynamic_cast<div_expression*>(&*lhs))
					&& (dynamic_cast<mul_expression*>(&*rhs) || dynamic_cast<div_expression*>(&*rhs))
					) std::swap(lhs,rhs);
				
				bool preserve_eax = result_in != eax || clobberable.find(eax)==clobberable.end();
				bool preserve_edx = result_in != edx || clobberable.find(edx)==clobberable.end();
				if ( preserve_eax ) args.assembler.push(eax);
				if ( preserve_edx ) args.assembler.push(edx);

				lhs->x86_compile( args, eax, clobberable );
				args.assembler
					.xor(edx,edx)
					;

				std::set<r32> rhs_clobberable(clobberable);
				rhs_clobberable.erase(eax);
				rhs_clobberable.erase(edx);

				if ( rhs_clobberable.empty() ) {
					r32 r = result_in==ecx?ebx:ecx;
					args.assembler
						.push(r)
						;
					rhs->x86_compile( args, r, rhs_clobberable );
					args.assembler
						.imul(r)
						.pop(r)
						;
				} else {
					r32 r = *rhs_clobberable.begin();
					rhs->x86_compile( args, r, rhs_clobberable );
					args.assembler
						.imul(r)
						;
				}

				if ( result_in != eax ) args.assembler.mov(result_in,eax);
				if ( preserve_edx ) args.assembler.pop(edx);
				if ( preserve_eax ) args.assembler.pop(eax);
			}
		};
	}
}

#endif //ndef IG_BRAIN_ML_AST

