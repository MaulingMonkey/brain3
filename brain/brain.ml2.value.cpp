#include "brain.ml2.value.hpp"
#include "brain.ml2.visitors.hpp"

namespace brain {
	namespace ml2 {
		template < typename Self > struct object_ : public object {
			virtual void clone_to( char (&data)[value_data_size] ) const {
				BOOST_STATIC_ASSERT( sizeof(Self) <= sizeof(data) );
				new (data) Self(*static_cast<const Self*>(this));
			}
		};
		
		value object::visit( const value_visitor<value>& visitor ) {
			return visitor(*this);
		}
		
		//class indirect_object : public object_<indirect_object> {
		//	object* o;
		//public:
		//	indirect_object( object* o ): o(o) {}
		//	virtual value visit( const value_visitor<value>& visitor ) { if (o) return o->visit(visitor); else return visitor(nil()); }
		//};

		class nil_object : public object_<nil_object> {
		public:
			virtual value visit( const value_visitor<value>& visitor ) { return visitor(nil()); }
		};

		template < typename T > class numeric_object : public object_<numeric_object<T> > {
			T data;
		public:
			numeric_object( T data ): data(data) {}
			virtual value visit( const value_visitor<value>& visitor ) { return visitor(data); }
		};

		//template < typename T > class object_adapter : public object_<object_adapter<T> > {
		//	T self;
		//public:
		//	object_adapter( T instance ): self(instance) {}
		//};

#if 0
		template < typename F > class functor_object_adapter_base : public functor_object {
		protected:
			F* f;
			virtual value invoke( value* begin, value* end ) const;
		};
#else
		template < typename F > class functor_object_adapter_base;
		template <> class functor_object_adapter_base< boost::function<void ()> >                    : public functor_object { protected: boost::function<void ()>     * f;               virtual value invoke( const value* begin, const value* end ) const { if ( end-begin != 0 ) throw std::runtime_error( "Expected 0 arguments." );        (*f)(); return value(); } };
		template <> class functor_object_adapter_base< boost::function<value()> >                    : public functor_object { protected: boost::function<value()>     * f;               virtual value invoke( const value* begin, const value* end ) const { if ( end-begin != 0 ) throw std::runtime_error( "Expected 0 arguments." ); return (*f)(); } };
		template <> class functor_object_adapter_base< boost::function<void (value)> >               : public functor_object { protected: boost::function<void (value)>* f;               virtual value invoke( const value* begin, const value* end ) const { if ( end-begin != 1 ) throw std::runtime_error( "Expected 1 arguments." );        (*f)(boost::cref(*begin)); return value(); } };
		template <> class functor_object_adapter_base< boost::function<value(value)> >               : public functor_object { protected: boost::function<value(value)>* f;               virtual value invoke( const value* begin, const value* end ) const { if ( end-begin != 1 ) throw std::runtime_error( "Expected 1 arguments." ); return (*f)(boost::cref(*begin)); } };
		template <> class functor_object_adapter_base< boost::function<void (std::vector<value>)> >  : public functor_object { protected: boost::function<void (std::vector<value>)>* f;  virtual value invoke( const value* begin, const value* end ) const { const std::vector<value> v(begin,end);                                            (*f)(boost::cref(v)); return value(); } };
		template <> class functor_object_adapter_base< boost::function<value(std::vector<value>)> >  : public functor_object { protected: boost::function<value(std::vector<value>)>* f;  virtual value invoke( const value* begin, const value* end ) const { const std::vector<value> v(begin,end);                                     return (*f)(boost::cref(v)); } };
#endif

		template < typename F > class functor_object_adapter : public functor_object_adapter_base<F> {
			typedef functor_object_adapter<F> Self;
		public:
			virtual void clone_to( char (&data)[value_data_size] ) const {
				BOOST_STATIC_ASSERT( sizeof(Self) <= sizeof(data) );
				new (data) Self(*static_cast<const Self*>(this));
			}
			functor_object_adapter( const F& f ) { this->f = new F(f); }
			functor_object_adapter( const functor_object_adapter& o ) { this->f = new F(*o.f); }
			~functor_object_adapter() { delete f; }
		};


		value::value()                         { init(nil_object()); }
		value::value( const value& other )     { ((object*)other.data)->clone_to(data); }
		value::~value()                        { ((object*)data)->~object(); }

		value& value::operator=( const value& other ) {
			object* self = ((object*)data);
			object* o    = ((object*)other.data);
			self->~object();
			o->clone_to(data);
			return *this;
		}

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

		void value::init_f( const boost::function<void ()>                   & f ) { init(functor_object_adapter<boost::function<void ()>                   >(f)); }
		void value::init_f( const boost::function<value()>                   & f ) { init(functor_object_adapter<boost::function<value()>                   >(f)); }
		void value::init_f( const boost::function<void (value)>              & f ) { init(functor_object_adapter<boost::function<void (value)>              >(f)); }
		void value::init_f( const boost::function<value(value)>              & f ) { init(functor_object_adapter<boost::function<value(value)>              >(f)); }
		void value::init_f( const boost::function<void (std::vector<value>)> & f ) { init(functor_object_adapter<boost::function<void (std::vector<value>)> >(f)); }
		void value::init_f( const boost::function<value(std::vector<value>)> & f ) { init(functor_object_adapter<boost::function<value(std::vector<value>)> >(f)); }
		//void value::init_f( const boost::function<value(std::vector<value>)>& f ) { init(functor_object_adapter<boost::function<value(std::vector<value>)> >(f)); }

		struct print_visitor {
			typedef value result_type;
			std::ostream* os;
			print_visitor( std::ostream& os ): os(&os) {}
			template < typename V > value operator()( const V& v ) const { *os << v; return value(); }
			value operator()( const functor_object& f ) const { *os << "<function>"; return value(); }
			value operator()( const object&         o ) const { *os << "<object>"; return value(); }
			value operator()( nil                     ) const { *os << "nil"; return value(); }
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

		// XXX:  implement value::operator()( ... )
	}
}
