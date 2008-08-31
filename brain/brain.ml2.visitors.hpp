#ifndef IG_BRAIN_ML2_VISITORS
#define IG_BRAIN_ML2_VISITORS

namespace brain {
	namespace ml2 {
		namespace detail {
			template < typename Self > struct binary_arith_visitor {
				typedef value result_type;

				template < typename L , typename R > value operator()( L l, R r ) const {
					return impl<L,R>::call((Self*)this,l,r);
				}
			private:
				template < typename T > struct accept { static const bool value = false; }; // rename as ml2::is_arithmetic<T> ?
				
				template <> struct accept<   signed char      > { static const bool value = true; };
				template <> struct accept<   signed short     > { static const bool value = true; };
				template <> struct accept<   signed int       > { static const bool value = true; };
				template <> struct accept<   signed long      > { static const bool value = true; };
				template <> struct accept<   signed long long > { static const bool value = true; };

				template <> struct accept< unsigned char      > { static const bool value = true; };
				template <> struct accept< unsigned short     > { static const bool value = true; };
				template <> struct accept< unsigned int       > { static const bool value = true; };
				template <> struct accept< unsigned long      > { static const bool value = true; };
				template <> struct accept< unsigned long long > { static const bool value = true; };

				template <> struct accept< float              > { static const bool value = true; };
				template <> struct accept< double             > { static const bool value = true; };
				template <> struct accept< long double        > { static const bool value = true; };

				template < typename L , typename R , bool okay = accept<L>::value && accept<R>::value > struct impl { static value call( Self* self, L l, R r ) {
					return self->impl(l,r);
				}};

				template < typename L , typename R > struct impl<L,R,false /*!okay*/> { static value call( Self* self, L l, R r ) {
					if ( !accept<L>::value ) throw std::runtime_error( std::string(typeid(L).name()) + " is not valid for arithmetic expressions" );
					else                     throw std::runtime_error( std::string(typeid(R).name()) + " is not valid for arithmetic expressions" );
				}};
			};
		}

		struct add_visitor : detail::binary_arith_visitor<add_visitor> { template < typename L , typename R > value impl( L l, R r ) const { return l+r; } };
		struct sub_visitor : detail::binary_arith_visitor<sub_visitor> { template < typename L , typename R > value impl( L l, R r ) const { return l-r; } };
		struct mul_visitor : detail::binary_arith_visitor<mul_visitor> { template < typename L , typename R > value impl( L l, R r ) const { return l*r; } };
		struct div_visitor : detail::binary_arith_visitor<div_visitor> { template < typename L , typename R > value impl( L l, R r ) const { return l/r; } };
		struct pow_visitor : detail::binary_arith_visitor<pow_visitor> {
			template < typename T > double up( T value ) const { return (double)value; }
			long double up( long double value ) const { return value; }

			template < typename L, typename R > value impl2( L l, R r ) const { return std::pow(l,r); }
			value impl2( double l, long double r ) const { return std::pow((long double)l,r); }
			value impl2( long double l, double r ) const { return std::pow(l,(long double)r); }

			template < typename L, typename R > value impl( L l, R r ) const { return impl2(up(l),up(r)); }
		};
		struct call_visitor {
			typedef value result_type;
			
			call_visitor( const std::vector<value*>& args ): args(args) {}

			value operator()( const functor_object* f ) const {
				return (*f)(args);
			}
			template < typename T > value operator()( const T& other ) const {
				std::stringstream ss;
				ss << "Cannot call a " << typeid(T).name();
				throw std::runtime_error(ss.str());
			}
		private:
			const std::vector<value*>& args;
		};
	}
}

#endif //ndef IG_BRAIN_ML2_VISITORS
