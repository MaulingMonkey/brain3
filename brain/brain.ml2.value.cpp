#include "brain.ml2.value.hpp"
#include "brain.ml2.visitors.hpp"

namespace brain {
	namespace ml2 {
		value object::visit( const value_visitor<value>& visitor ) {
			return visitor(this);
		}

		template < typename O > void value::init( const O& value ) {
			BOOST_STATIC_ASSERT( sizeof(O) <= sizeof(data) );
			new (data) O(value);
		}

		value::value()                         { init(nil_object()); }
		value::~value()                        { ((object*)data)->~object(); }
			
		value::value( signed char        arg ) { init(numeric_object<signed char       >(arg)); }
		value::value( signed short       arg ) { init(numeric_object<signed short      >(arg)); }
		value::value( signed int         arg ) { init(numeric_object<signed int        >(arg)); }
		value::value( signed long        arg ) { init(numeric_object<signed long       >(arg)); }
		value::value( signed long long   arg ) { init(numeric_object<signed long long  >(arg)); }

		value::value( unsigned char      arg ) { init(numeric_object<unsigned char     >(arg)); }
		value::value( unsigned short     arg ) { init(numeric_object<unsigned short    >(arg)); }
		value::value( unsigned int       arg ) { init(numeric_object<unsigned int      >(arg)); }
		value::value( unsigned long      arg ) { init(numeric_object<unsigned long     >(arg)); }
		value::value( unsigned long long arg ) { init(numeric_object<unsigned long long>(arg)); }
		
		value::value( bool               arg ) { init(numeric_object<bool              >(arg)); }
		value::value( char               arg ) { init(numeric_object<char              >(arg)); }
		value::value( float              arg ) { init(numeric_object<float             >(arg)); }
		value::value( double             arg ) { init(numeric_object<double            >(arg)); }
		value::value( long double        arg ) { init(numeric_object<long double       >(arg)); }

		value::value( const boost::function<void ()>                  & f ) { init(function_object()); }
		value::value( const boost::function<value()>                  & f ) { init(function_object()); }
		value::value( const boost::function<void (value)>             & f ) { init(function_object()); }
		value::value( const boost::function<value(value)>             & f ) { init(function_object()); }
		value::value( const boost::function<void (std::vector<value>)>& f ) { init(function_object()); }
		value::value( const boost::function<value(std::vector<value>)>& f ) { init(function_object()); }

		struct print_visitor {
			typedef value result_type;
			std::ostream* os;
			print_visitor( std::ostream& os ): os(&os) {}
			template < typename V > result_type operator()( V v ) const { *os << v; return value(); }
		};

		value operator+( const value& lhs, const value& rhs ) { return apply_visitor( add_visitor(), lhs, rhs ); }
		value operator-( const value& lhs, const value& rhs ) { return apply_visitor( sub_visitor(), lhs, rhs ); }
		value operator*( const value& lhs, const value& rhs ) { return apply_visitor( mul_visitor(), lhs, rhs ); }
		value operator/( const value& lhs, const value& rhs ) { return apply_visitor( div_visitor(), lhs, rhs ); }
		std::ostream& operator<<( std::ostream& os, const value& v ) { apply_visitor( print_visitor(os), v ); return os; }

		value& value::operator+=( const value& other ) { *this = apply_visitor( add_visitor(), *this, other ); return *this; }
		value& value::operator-=( const value& other ) { *this = apply_visitor( sub_visitor(), *this, other ); return *this; }
		value& value::operator*=( const value& other ) { *this = apply_visitor( mul_visitor(), *this, other ); return *this; }
		value& value::operator/=( const value& other ) { *this = apply_visitor( div_visitor(), *this, other ); return *this; }
	}
}
