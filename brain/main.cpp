#include "brain.ml.grammar.hpp"
#include "brain.ml2.grammar.hpp"
#include "brain.x86.function.hpp"
#include <iostream>

unsigned __fastcall f4( unsigned a, unsigned b, unsigned c ) {
	return a+b+c;
}

void test1() {
	using namespace brain::x86;
	function< unsigned __fastcall ( unsigned a, unsigned b ) > f1;
	f1	.mov( eax, ecx )
		.add( eax, edx )
		.ret()
		;

	function< unsigned __fastcall ( unsigned *a, unsigned *b ) > f2;
	f2	.mov( eax, dword[ecx+0] )
		.add( eax, dword[edx  ] )
		.ret()
		;

	function< unsigned __fastcall ( unsigned a, unsigned b, unsigned c ) > f3;
	f3	.mov( eax, ecx )
		.add( eax, edx )
		.add( eax, dword[esp+4] )
		.ret(4)
		;

	function< unsigned __fastcall ( unsigned a, unsigned b ) > f5;
	f5	.xor( eax, eax ) // eax = 0
		.mov( ebx, edx ) // ebx <- a
		.mov( edx, ecx ) // edx <- b
		.div( /* edx:eax, */ ebx ) // eax = a 0000 0000 / b
		.ret()
		;

	function< unsigned __cdecl ( unsigned a, unsigned b, unsigned c ) > f6;
	f6	.xor( eax, eax ) // eax = 0
		.add( eax, dword[esp+ 4] )
		.add( eax, dword[esp+ 8] )
		.add( eax, dword[esp+12] )
		.ret()
		;

	unsigned a = 2, b = 4;
	std::cout << std::dec << f1( 2, 4 ) << std::endl;
	std::cout << std::dec << f2( &a, &b ) << std::endl;
	std::cout << std::dec << f3( 1, 2, 3 ) << std::endl;
	std::cout << std::dec << f4( 1, 2, 3 ) << std::endl;
	std::cout << std::hex << f5( 6, 0x10000 ) << std::endl;
	std::cout << std::dec << f6( 2, 3, 4 ) << std::endl;
}

class load_cdecl_arg {
	int arg;
public:
	load_cdecl_arg( int arg ): arg(arg) {}

	void operator()( brain::x86::assembler& assembler, brain::x86::r32 target ) const {
		// XXX: O DEAR GOD HORRIBLY NAUGHTY ASSUMPTIONS:
		// every argument is 4 bytes.
		using namespace brain::x86;
		assembler.mov( target, dword[ebp+(arg*4+8)] );
	}
};

void test2() {
	using namespace brain::x86;
	using namespace boost::spirit;
	using namespace phoenix;

	std::string op = "4*a+b";
	brain::ml::grammar g;
	brain::ml::expr_ref_t expression;
	parse_info<std::string::const_iterator> info = parse( op.begin(), op.end(), g[assign_a(expression)], +space_p );
	if ( info.full ) {
		brain::ml::expression::calculate_args_type eargs;
		eargs["a"] = 42;
		eargs["b"] = 24;
		brain::ml::expression::calculate_result_type result = expression->calculate(eargs);
		std::cout << "f(a=42,b=24) = " << *expression << " := " << result << std::endl;

		std::cout << "Attempting to compile into a function..." << std::endl;
		function< int __cdecl ( int a, int b ) > f;
		f	.push(ebp)
			.mov(ebp,esp)
			;

		brain::ml::x86_compile_args cargs(f);
		cargs.args["a"] = load_cdecl_arg(0);
		cargs.args["b"] = load_cdecl_arg(1);
		std::set<r32> clobberable;
		clobberable.insert( eax );
		clobberable.insert( ecx );
		clobberable.insert( edx );
		expression->x86_compile(cargs,eax,clobberable);

		f	.mov(esp,ebp)
			.pop(ebp)
			.ret()
			;

		std::cout << "But will it blend?    Attempting to run..." << std::endl;
		result = f(42,24);
		std::cout << "ZOMG no access violations!    Result: " << result << std::endl;
	} else {
		std::cout << "Failed to fully match!" << std::endl;
	}
	
	//4*a + b -> bin_op( bin_op( lit(4), arg("a") ), arg("b") )
	//brain::ml::compile< unsigned ( unsigned, unsigned ) >( "|a,b| 4*a + b" );
}

class print_f {
	std::ostream& os;
public:
	print_f( std::ostream& os ): os(os) {}

	brain::ml2::value operator()( const std::vector< brain::ml2::value* >& args ) const {
		BOOST_FOREACH( const brain::ml2::value* v, args ) os << *v;
		return brain::ml2::value();
	}
};

void test3() {
	using namespace brain;
	using namespace brain::ml2;
	using namespace brain::x86;
	using namespace boost::spirit;
	using namespace phoenix;
	using namespace std;

	typedef brain::ml2::value value;

	//string op = "print(4*a+b);";
	string op = "print(4*a+b);";
	ml2::grammar g;
	expr_ref_t expression;
	parse_info<string::const_iterator> info = parse( op.begin(), op.end(), g[assign_a(expression)], +space_p );
	if ( info.full ) {
		ml2::expression::calculate_args_type args;
		args["a"]     = 42.1;
		args["b"]     = 24.3;
		args["print"] = boost::function< value(std::vector<value*> ) >(print_f(std::cout));
		std::cout << "\n=> " << expression->calculate(args) << std::endl;
	} else {
		std::cout << "Failed to fully match!" << std::endl;
	}
}

int main() {
	//test1();
	//test2();
	test3();
}
