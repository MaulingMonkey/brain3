#include "brain.ml2.eval.hpp"

int main() {
	using namespace brain::ml2;
	eval( "puts(4*a+b);" );
	eval( "anfscd();" );
	eval( "nmcb();" );
	eval( "def f() := 4*a; f();" );
	eval( "def f() { puts(1,2,3); 4*a; } f();" );
	eval( "3/2;" );
	eval( "3.0/2.0;" );
	eval( "1**2+3**4;" );
}
