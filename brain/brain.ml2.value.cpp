#include "brain.ml2.value.hpp"

namespace brain {
	namespace ml2 {
		value::value()                        : type(type::id<void>())                         {}
			
		value::value( signed char        i8  ): i8 (i8 ), type(type::id<  signed char     >()) {}
		value::value( signed short       i16 ): i16(i16), type(type::id<  signed short    >()) {}
		value::value( signed int         i32 ): i16(i16), type(type::id<  signed int      >()) {}
		value::value( signed long long   i64 ): i16(i16), type(type::id<  signed long long>()) {}
		
		value::value( unsigned char      u8  ): u8 (u8 ), type(type::id<unsigned char     >()) {}
		value::value( unsigned short     u16 ): u16(u16), type(type::id<unsigned short    >()) {}
		value::value( unsigned int       u32 ): u32(u32), type(type::id<unsigned int      >()) {}
		value::value( unsigned long long u64 ): u64(u64), type(type::id<unsigned long long>()) {}
		
		value::value( float              f32 ): f32(f32), type(type::id<float             >()) {}
		value::value( double             f64 ): f64(f64), type(type::id<double            >()) {}

		template < typename SelfT > struct bin_op_f {
			typedef value result_type;
			template < typename L, typename R > result_type operator()( L l, R r ) const { return SelfT::primitive(l,r); }
			template < typename L > result_type operator()( L l, object* r ) const { return 0; }
			template < typename R > result_type operator()( object* l, R r ) const { return 0; }
			result_type operator()( object* l, object* r ) const { return 0; }
		};
		struct add_f : bin_op_f<add_f> { template < typename L, typename R > static result_type primitive( L l, R r ) { return l+r; } };
		struct sub_f : bin_op_f<sub_f> { template < typename L, typename R > static result_type primitive( L l, R r ) { return l-r; } };
		struct mul_f : bin_op_f<mul_f> { template < typename L, typename R > static result_type primitive( L l, R r ) { return l*r; } };
		struct div_f : bin_op_f<div_f> { template < typename L, typename R > static result_type primitive( L l, R r ) { return l/r; } };
		struct print_f {
			typedef void result_type;
			std::ostream* os;
			print_f( std::ostream& os ): os(&os) {}
			template < typename V > result_type operator()( V v ) const { *os << v; }
		};

		value operator+( const value& lhs, const value& rhs ) { return apply_visitor( add_f(), lhs, rhs ); }
		value operator-( const value& lhs, const value& rhs ) { return apply_visitor( sub_f(), lhs, rhs ); }
		value operator*( const value& lhs, const value& rhs ) { return apply_visitor( mul_f(), lhs, rhs ); }
		value operator/( const value& lhs, const value& rhs ) { return apply_visitor( div_f(), lhs, rhs ); }
		std::ostream& operator<<( std::ostream& os, const value& v ) { apply_visitor( print_f(os), v ); return os; }

		value& value::operator+=( const value& other ) { *this = apply_visitor( add_f(), *this, other ); return *this; }
		value& value::operator-=( const value& other ) { *this = apply_visitor( sub_f(), *this, other ); return *this; }
		value& value::operator*=( const value& other ) { *this = apply_visitor( mul_f(), *this, other ); return *this; }
		value& value::operator/=( const value& other ) { *this = apply_visitor( div_f(), *this, other ); return *this; }
	}
}
