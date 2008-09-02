#include "brain.ml2.grammar.hpp"
#include <iostream>

class print_f {
	std::ostream& os;
public:
	print_f( std::ostream& os ): os(os) {}

	brain::ml2::value operator()( const std::vector< brain::ml2::value >& args ) const {
		BOOST_FOREACH( const brain::ml2::value& v, args ) os << v;
		return brain::ml2::value();
	}
};

class and_now_for_something_completely_different_f {
public:
	int operator()() { std::cout << "!!!\n"; return 42; }
};

void eval( const std::string& op ) {
	using namespace brain;
	using namespace brain::ml2;
	using namespace boost::spirit;
	using namespace std;

	ml2::grammar g;
	expr_ref_t expression;
	parse_info<string::const_iterator> info = parse( op.begin(), op.end(), g[assign_a(expression)], +space_p );
	if ( info.full ) {
		ml2::expression::calculate_args_type args;
		args["a"]      = 42.1;
		args["b"]      = 24.3;
		args["print"]  = print_f(std::cout);
		args["anfscd"] = and_now_for_something_completely_different_f();
		std::cout << "\n=> " << expression->calculate(args) << std::endl;
	} else {
		std::cout << "Failed to fully match!" << std::endl;
	}
}

int main() {
	eval( "anfscd();" );
	eval( "print(4*a+b);" );
}