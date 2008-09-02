#ifndef IG_BRAIN_ML2_VALUE
#define IG_BRAIN_ML2_VALUE

#include <industry/enum/basic_enum.hpp>
#include <boost/function.hpp>
#include <boost/range.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/static_assert.hpp>
#include <ostream>
#include <limits>
#include <vector>
#include <climits>

namespace brain {
	namespace ml2 {
		struct nil {};

		namespace {
			const size_t value_data_size = 16;
		}

		class value;

		template < typename R > class value_visitor;

		class object {
		public:
			virtual ~object() {}
			virtual value visit( const value_visitor<value>& visitor );
			virtual void clone_to( char (&data)[value_data_size] ) const = 0;
		};
		class functor_object;

		template < typename R > class value_visitor {
		public:
			virtual R operator()( signed char           ) const = 0;
			virtual R operator()( signed short          ) const = 0;
			virtual R operator()( signed int            ) const = 0;
			virtual R operator()( signed long           ) const = 0;
			virtual R operator()( signed long long      ) const = 0;

			virtual R operator()( unsigned char         ) const = 0;
			virtual R operator()( unsigned short        ) const = 0;
			virtual R operator()( unsigned int          ) const = 0;
			virtual R operator()( unsigned long         ) const = 0;
			virtual R operator()( unsigned long long    ) const = 0;

			virtual R operator()( bool                  ) const = 0;
			virtual R operator()( char                  ) const = 0;
			virtual R operator()( float                 ) const = 0;
			virtual R operator()( double                ) const = 0;
			virtual R operator()( long double           ) const = 0;

			virtual R operator()( const functor_object& ) const = 0;

			virtual R operator()( const object&         ) const = 0;

			virtual R operator()( nil                   ) const = 0;
		};

		template < typename F , typename R = typename F::result_type > class value_visitor_adapter : public value_visitor< R > {
			F f;
		public:
			value_visitor_adapter( F f ): f(f) {}

			virtual R operator()( signed char           arg ) const { return f(arg); }
			virtual R operator()( signed short          arg ) const { return f(arg); }
			virtual R operator()( signed int            arg ) const { return f(arg); }
			virtual R operator()( signed long           arg ) const { return f(arg); }
			virtual R operator()( signed long long      arg ) const { return f(arg); }

			virtual R operator()( unsigned char         arg ) const { return f(arg); }
			virtual R operator()( unsigned short        arg ) const { return f(arg); }
			virtual R operator()( unsigned int          arg ) const { return f(arg); }
			virtual R operator()( unsigned long         arg ) const { return f(arg); }
			virtual R operator()( unsigned long long    arg ) const { return f(arg); }

			virtual R operator()( bool                  arg ) const { return f(arg); }
			virtual R operator()( char                  arg ) const { return f(arg); }
			virtual R operator()( float                 arg ) const { return f(arg); }
			virtual R operator()( double                arg ) const { return f(arg); }
			virtual R operator()( long double           arg ) const { return f(arg); }

			virtual R operator()( const functor_object& arg ) const { return f(arg); }

			virtual R operator()( const object&         arg ) const { return f(arg); }

			virtual R operator()( nil                   arg ) const { return f(arg); }
		};

		

		class value {
			char data[value_data_size];
			
			template < typename O > void init( const O& o ) {
				BOOST_STATIC_ASSERT( sizeof(O) <= sizeof(data) );
				new (data) O(o);
			}
			void init_f( const boost::function<void ()>& f );
			void init_f( const boost::function<value()>& f );
			void init_f( const boost::function<void (value)>& f );
			void init_f( const boost::function<value(value)>& f );
			void init_f( const boost::function<void (std::vector<value>)>& f );
			void init_f( const boost::function<value(std::vector<value>)>& f );

			template < typename R > struct to_value_or_void { typedef value type; };
			template <> struct to_value_or_void<void> { typedef void type; };
			
			template < typename F1, typename F2, typename R > void init_f_helper( const F1& f, R (F2::*)() ) {
				using namespace boost;
				typedef typename to_value_or_void<R>::type R2;
				init_f( function< R2() >(f) );
			}
			template < typename F1, typename F2, typename R, typename Arg > void init_f_helper( const F1& f, R (F2::*)( Arg ) ) {
				using namespace boost;
				typedef typename to_value_or_void<R>::type R2;
				typedef typename remove_const< typename remove_reference<Arg>::type >::type Arg2;
				init_f( function< R2(Arg2) >(f) );
			}
			template < typename F1, typename F2, typename R > void init_f_helper( const F1& f, R (F2::*fptr)() const ) { init_f_helper( f, (R (F2::*)())0 ); }
			template < typename F1, typename F2, typename R, typename Arg > void init_f_helper( const F1& f, R (F2::*fptr)( Arg ) const ) { init_f_helper( f, (R (F2::*)(Arg))0 ); }
		public:
			template < typename R, typename O             > void bind( R (O::*f)( )       ,       O* o ) { init_f_helper(boost::bind(f,o),f); }
			template < typename R, typename O             > void bind( R (O::*f)( ) const , const O* o ) { init_f_helper(boost::bind(f,o),f); }
			template < typename R, typename O, typename A > void bind( R (O::*f)(A)       ,       O* o ) { init_f_helper(boost::bind(f,o,_1),f); }
			template < typename R, typename O, typename A > void bind( R (O::*f)(A) const , const O* o ) { init_f_helper(boost::bind(f,o,_1),f); }

			value();
			value( const value& );
			value& operator=( const value& other );
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

			template < typename F > value( const F& f ) { init_f_helper(f,&F::operator()); }

			template < typename F > friend value apply_visitor( F f, const value& v ) {
				return ((object*)v.data)->visit( value_visitor_adapter<F>(f) );
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
				template < typename L > typename F::result_type operator()( const L& lhs ) const {
					return apply_visitor( binary_visitor_f_rhs<F,L>(f,lhs), *rhs );
				}
			};
			template < typename F, typename L > class binary_visitor_f_rhs {
				F f; const L* lhs;
			public:
				typedef typename F::result_type result_type;
				binary_visitor_f_rhs( F f, const L& lhs ): f(f), lhs(&lhs) {}
				template < typename R > typename F::result_type operator()( const R& rhs ) const {
					return f(*lhs,rhs);
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

			value operator()() const;
			value operator()( const value& arg1 ) const;
			value operator()( const value& arg1, const value& arg2 ) const;
			value operator()( const value& arg1, const value& arg2, const value& arg3 ) const;
			value operator()( const value& arg1, const value& arg2, const value& arg3, const value& arg4 ) const;
		};

		class functor_object : public object {
		protected:
			virtual value invoke( const value* begin, const value* end ) const = 0;
		public:
			virtual value visit( const value_visitor<value>& visitor ) { return visitor(*this); }

			value operator()() const { return invoke(0,0); }
			value operator()( value arg1 ) const { const value args[] = { arg1 }; return invoke(boost::begin(args),boost::end(args)); }
			value operator()( value arg1, value arg2 ) const { const value args[] = { arg1,arg2 }; return invoke(boost::begin(args),boost::end(args)); }
			value operator()( value arg1, value arg2, value arg3 ) const { const value args[] = { arg1,arg2,arg3 }; return invoke(boost::begin(args),boost::end(args)); }
			value operator()( value arg1, value arg2, value arg3, value arg4 ) const { const value args[] = { arg1,arg2,arg3,arg4 }; return invoke(boost::begin(args),boost::end(args)); }
			value operator()( const value* begin, const value* end ) const { return invoke(begin,end); }
			value operator()( const std::vector<value>& v ) const { if (v.empty()) return invoke(0,0); else return invoke(&v[0],&v[0]+v.size()); }
		};
	}
}

#endif //ndef IG_BRAIN_ML2_VALUE
