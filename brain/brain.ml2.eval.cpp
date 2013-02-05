#include "brain.ml2.grammar.hpp"
#include <iostream>

namespace brain {
	namespace ml2 {
		namespace {
			class print_f {
				std::ostream& os;
				const bool newline;
			public:
				print_f( std::ostream& os, bool newline ): os(os), newline(newline) {}

				void operator()( const std::vector< brain::ml2::value >& args ) const {
					BOOST_FOREACH( const brain::ml2::value& v, args ) os << v;
					if ( newline ) os << "\n";
				}
			};

			struct needs_more_cowbell_f {
				void operator()() {}
			};

			struct and_now_for_something_completely_different_f {
				int operator()() { std::cout << "!!!\n"; return 42; }
			};
		}

		void eval( const std::string& op ) {
			using namespace brain;
			using namespace brain::ml2;
			using namespace boost::spirit;
			using namespace std;

			std::cout << "eval(\"" << op << "\");\n";

			ml2::grammar g;
			ml2::expression::ref expression;
			parse_info<string::const_iterator> info = parse( op.begin(), op.end(), g[assign_a(expression)], whitespace );
			if ( info.full ) {
				ml2::expression::calculate_args_type args;
				args["a"]      = 42.1;
				args["b"]      = 24.3;
				args["print"]  = print_f(std::cout,false);
				args["puts"]   = print_f(std::cout,true );
				args["nmcb"]   = needs_more_cowbell_f();
				args["anfscd"] = and_now_for_something_completely_different_f();
				value f = expression->calculate(args);
				value r = f();
				std::cout << " => " << r << "\n";
			} else {
				std::cout << "Failed to fully match!\n";
			}
			std::cout << std::endl;
		}
	}
}
