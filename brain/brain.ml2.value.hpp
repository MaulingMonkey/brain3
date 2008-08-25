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

		template < typename T > class by_value_wrapper {
			T value;
		public:
			by_value_wrapper( T value ): value(value) {}
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
			value(); // XXX: conflicts with brain.ml2.ast "value" class currently :o -- silly ODR violating monkey
			
			value( signed char        i8  );
			value( signed short       i16 );
			value( signed int         i32 );
			value( signed long long   i64 );
			
			value( unsigned char      u8  );
			value( unsigned short     u16 );
			value( unsigned int       u32 );
			value( unsigned long long u64 );
			
			value( float              f32 );
			value( double             f64 );

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
		public:
			friend value operator+( const value& lhs, const value& rhs );
			friend value operator-( const value& lhs, const value& rhs );
			friend value operator*( const value& lhs, const value& rhs );
			friend value operator/( const value& lhs, const value& rhs );
			friend std::ostream& operator<<( std::ostream& os, const value& v );

			value& operator+=( const value& other );
			value& operator-=( const value& other );
			value& operator*=( const value& other );
			value& operator/=( const value& other );

			//value operator()( ... )
		};

		//...
	}
}

#endif //ndef IG_BRAIN_ML2_VALUE
