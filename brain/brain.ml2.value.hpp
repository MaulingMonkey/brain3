#ifndef IG_BRAIN_ML2_VALUE
#define IG_BRAIN_ML2_VALUE

#include "brain.ml2.type.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/static_assert.hpp>
#include <ostream>
#include <climits>

namespace brain {
	namespace ml2 {
		class object {
		public:
			virtual ~object() {}
		};

		class value {
			union {
				signed char         i8;
				signed short        i16;
				signed int          i32;
				signed long long    i64;
				
				unsigned char       u8;
				unsigned short      u16;
				unsigned int        u32;
				unsigned long long  u64;

				float               f32;
				double              f64;

				char                data[64/CHAR_BIT];

				object*             o;
			};

			ml2::type type;

			BOOST_STATIC_ASSERT( sizeof(char     )*CHAR_BIT >=  8 );
			BOOST_STATIC_ASSERT( sizeof(short    )*CHAR_BIT >= 16 );
			BOOST_STATIC_ASSERT( sizeof(int      )*CHAR_BIT >= 32 );
			BOOST_STATIC_ASSERT( sizeof(long long)*CHAR_BIT >= 64 );
		public:
			value()                        : type(type::id<void>())                         {}
			
			value( signed char        i8  ): i8 (i8 ), type(type::id<  signed char     >()) {}
			value( signed short       i16 ): i16(i16), type(type::id<  signed short    >()) {}
			value( signed int         i32 ): i16(i16), type(type::id<  signed int      >()) {}
			value( signed long long   i64 ): i16(i16), type(type::id<  signed long long>()) {}
			
			value( unsigned char      u8  ): u8 (u8 ), type(type::id<unsigned char     >()) {}
			value( unsigned short     u16 ): u16(u16), type(type::id<unsigned short    >()) {}
			value( unsigned int       u32 ): u32(u32), type(type::id<unsigned int      >()) {}
			value( unsigned long long u64 ): u64(u64), type(type::id<unsigned long long>()) {}
			
			value( float              f32 ): f32(f32), type(type::id<float             >()) {}
			value( double             f64 ): f64(f64), type(type::id<double            >()) {}

			template < typename F > friend typename F::result_type apply_visitor( F f, const value& v ) {
				typedef ml2::type::keys t;

				switch ( v.type.key() ) {
				case t::  void_            : return f((object*)0);
				case t::  signed_char_     : return f(v.i8 );
				case t::  signed_short_    : return f(v.i16);
				case t::  signed_int_      : return f(v.i32);
				case t::  signed_long_long_: return f(v.i64);
				case t::unsigned_char_     : return f(v.u8 );
				case t::unsigned_short_    : return f(v.u16);
				case t::unsigned_int_      : return f(v.u32);
				case t::unsigned_long_long_: return f(v.u64);
				case t::float_             : return f(v.f32);
				case t::double_            : return f(v.f64);
				default: return f( (size_of(v.type)*CHAR_BIT>64) ? v.o : ((object*)v.data) );
				}
			}
			template < typename F > friend typename F::result_type apply_visitor( F f, const value& lhs, const value& rhs ) {
				return apply_visitor( binary_visitor_f_lhs<F>(f,rhs), lhs );
			}
		private:
			template < typename F > class binary_visitor_f_lhs {
				F f; const value* rhs;
			public:
				typedef typename F::result_type result_type;
				binary_visitor_f_lhs( F f, const value& rhs ): f(f), rhs(&rhs) {}
				template < typename L > typename F::result_type operator()( L lhs ) const {
					return apply_visitor( binary_visitor_f_rhs<F,L>(f,lhs), *rhs );
				}
			};
			template < typename F, typename L > class binary_visitor_f_rhs {
				F f; L lhs;
			public:
				typedef typename F::result_type result_type;
				binary_visitor_f_rhs( F f, L lhs ): f(f), lhs(lhs) {}
				template < typename R > typename F::result_type operator()( R rhs ) const {
					return f(lhs,rhs);
				}
			};

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
		public:
			friend value operator+( const value& lhs, const value& rhs ) { return apply_visitor( add_f(), lhs, rhs ); }
			friend value operator-( const value& lhs, const value& rhs ) { return apply_visitor( sub_f(), lhs, rhs ); }
			friend value operator*( const value& lhs, const value& rhs ) { return apply_visitor( mul_f(), lhs, rhs ); }
			friend value operator/( const value& lhs, const value& rhs ) { return apply_visitor( div_f(), lhs, rhs ); }
			friend std::ostream& operator<<( std::ostream& os, const value& v ) { apply_visitor( print_f(os), v ); return os; }

			value& operator+=( const value& other ) { *this = apply_visitor( add_f(), *this, other ); return *this; }
			value& operator-=( const value& other ) { *this = apply_visitor( sub_f(), *this, other ); return *this; }
			value& operator*=( const value& other ) { *this = apply_visitor( mul_f(), *this, other ); return *this; }
			value& operator/=( const value& other ) { *this = apply_visitor( div_f(), *this, other ); return *this; }

			//value operator()( ... )
		};3

		//...
	}
}

#endif //ndef IG_BRAIN_ML2_VALUE
