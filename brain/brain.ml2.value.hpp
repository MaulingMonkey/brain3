#ifndef IG_BRAIN_ML2_VALUE
#define IG_BRAIN_ML2_VALUE

#include <industry/enum/basic_enum.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/static_assert.hpp>
#include <ostream>
#include <limits>
#include <vector>
#include <climits>

namespace brain {
	namespace ml2 {
		
		class value;

		template < typename R > class value_visitor;

		class object {
		public:
			virtual ~object() {}
			virtual value visit( const value_visitor<value>& visitor );
		};

		template < typename R > class value_visitor {
		public:
			virtual R operator()( signed char        ) const = 0;
			virtual R operator()( signed short       ) const = 0;
			virtual R operator()( signed int         ) const = 0;
			virtual R operator()( signed long        ) const = 0;
			virtual R operator()( signed long long   ) const = 0;

			virtual R operator()( unsigned char      ) const = 0;
			virtual R operator()( unsigned short     ) const = 0;
			virtual R operator()( unsigned int       ) const = 0;
			virtual R operator()( unsigned long      ) const = 0;
			virtual R operator()( unsigned long long ) const = 0;

			virtual R operator()( bool               ) const = 0;
			virtual R operator()( char               ) const = 0;
			virtual R operator()( float              ) const = 0;
			virtual R operator()( double             ) const = 0;
			virtual R operator()( long double        ) const = 0;

			virtual R operator()( object*            ) const = 0;
		};

		template < typename F , typename R = typename F::result_type > class value_visitor_adapter : public value_visitor< R > {
			F f;
		public:
			value_visitor_adapter( F f ): f(f) {}

			virtual R operator()( signed char        arg ) const { return f(arg); }
			virtual R operator()( signed short       arg ) const { return f(arg); }
			virtual R operator()( signed int         arg ) const { return f(arg); }
			virtual R operator()( signed long        arg ) const { return f(arg); }
			virtual R operator()( signed long long   arg ) const { return f(arg); }

			virtual R operator()( unsigned char      arg ) const { return f(arg); }
			virtual R operator()( unsigned short     arg ) const { return f(arg); }
			virtual R operator()( unsigned int       arg ) const { return f(arg); }
			virtual R operator()( unsigned long      arg ) const { return f(arg); }
			virtual R operator()( unsigned long long arg ) const { return f(arg); }

			virtual R operator()( bool               arg ) const { return f(arg); }
			virtual R operator()( char               arg ) const { return f(arg); }
			virtual R operator()( float              arg ) const { return f(arg); }
			virtual R operator()( double             arg ) const { return f(arg); }
			virtual R operator()( long double        arg ) const { return f(arg); }

			virtual R operator()( object*            arg ) const { return f(arg); }
		};

		

		class value { // XXX: conflicts with brain.ml2.ast "value" class currently :o -- silly ODR violating monkey
			static const size_t data_size = 16;
			char data[data_size];
			
			template < typename O > void init( const O& object );
		public:
			value();
			~value();
			
			value( signed char        i8  );
			value( signed short       i16 );
			value( signed int         i32 );
			value( signed long        iwtf);
			value( signed long long   i64 );
			
			value( unsigned char      u8  );
			value( unsigned short     u16 );
			value( unsigned int       u32 );
			value( unsigned long      uwtf);
			value( unsigned long long u64 );
			
			value( bool               b   );
			value( char               ch  );
			value( float              f32 );
			value( double             f64 );
			value( long double        fwtf);

			value( const boost::function<void ()>& f );
			value( const boost::function<value()>& f );
			value( const boost::function<void (value)>& f );
			value( const boost::function<value(value)>& f );
			value( const boost::function<void (std::vector<value>)>& f );
			value( const boost::function<value(std::vector<value>)>& f );

			template < typename F > friend value apply_visitor( F f, const value& v ) {
				return ((object*)v.data)->visit( value_visitor_adapter<F>(f) );
				//(object*)data;
				//return v;
			}
			template < typename F > friend value apply_visitor( F f, const value& lhs, const value& rhs ) {
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

		class indirect_object : public object {
		protected:
			object* o;
		public:
			indirect_object( object* o ): o(o) {}
			virtual value visit( const value_visitor<value>& visitor ) { if (o) return o->visit(visitor); else return visitor((object*)0); }
		};

		class nil_object : public object {
		public:
			virtual value visit( const value_visitor<value>& visitor ) { return visitor((object*)0); }
		};

		template < typename T > class numeric_object : public object {
			T data;
		public:
			numeric_object( T data ): data(data) {}

			virtual value visit( const value_visitor<value>& visitor ) { return visitor(data); }
		};

		template < typename T > class object_adapter : public object {
			T self;
		public:
			object_adapter( T instance ): self(instance) {}
		};

		class function_object : public object {
		};
	}
}

#endif //ndef IG_BRAIN_ML2_VALUE
