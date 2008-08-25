#ifndef IG_BRAIN_ML2_TYPE
#define IG_BRAIN_ML2_TYPE

#include <industry/type.hpp>
#include <vector>

namespace brain {
	namespace ml2 {
		class type : public industry::type {
		public:
			type(): industry::type(id<void>()) {}
			type( const type& other ) throw() : industry::type( other ) {}

			friend std::size_t size_of( const type& t ) { return size_db[t.key()]; }
			
			template < typename T > struct id_impl { static const type call() { static const type t(typeid(T),sizeof(T)); return t; } };
			template <> struct id_impl<void> { static const type call() { static const type t(typeid(void),0); return t; } };
			template < typename T > static type id() { return id_impl<T>::call(); }
		private:
			type( const std::type_info& info, std::size_t size ) : industry::type(info) {
				unsigned k = key();
				if ( k > size_db.size() ) size_db.resize(k);
				size_db[k] = size;
			}
			static std::vector<size_t> size_db;
		};
		template < typename T > inline type dynamic_type_of( const T& expr ); // NO IMPLEMENTATION, size of dynamic types is unknowable!
		template < typename T > inline type static_type_of ( const T& expr ) { return type::id<T>(); }
	}
}

#endif //ndef IG_BRAIN_ML2_TYPE
