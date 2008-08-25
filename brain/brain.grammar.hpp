#ifndef IG_BRAIN_GRAMMAR
#define IG_BRAIN_GRAMMAR

//#include <boost/cast.hpp>
#include <boost/preprocessor.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/spirit.hpp>
#include <cassert>
#define GRAMMAR_GENERATE() "brain.grammar.gen.hpp"

namespace brain {
	namespace grammar {
		template < typename T = void >
		class expression_ref {
			boost::shared_ptr<T> ptr;
		protected:
			expression_ref( const boost::shared_ptr<T>& ptr ): ptr(ptr) {}
		public:
			expression_ref() {}
			expression_ref( const expression_ref& o ): ptr(o.ptr) {}
			template < typename U > expression_ref( const expression_ref<U>& o )
				: ptr(boost::static_pointer_cast<T>(o.ptr))
			{
				assert( ptr == boost::dynamic_pointer_cast<T>(o.ptr) );
			}
			template < typename U > friend class expression_ref;

			struct autoconv {
				boost::shared_ptr<T> ptr;
				autoconv( const autoconv& o ): ptr(o.ptr) {}
				autoconv( const expression_ref& o ): ptr(o.ptr) {}
				template < typename O > autoconv( const O& o ): ptr(new O(o)) {}
			};
			expression_ref( const autoconv& ac ): ptr(ac.ptr) {}

			T* get()        const { return ptr.get(); }
			T* operator->() const { return ptr.get(); }
			T& operator* () const { return *ptr; }
			operator const void* () const { return ptr.get(); }

			friend bool operator==( const expression_ref& lhs, const expression_ref& rhs ) { return lhs.ptr == rhs.ptr; }
			friend bool operator!=( const expression_ref& lhs, const expression_ref& rhs ) { return lhs.ptr != rhs.ptr; }
			friend bool operator>=( const expression_ref& lhs, const expression_ref& rhs ) { return lhs.ptr <= rhs.ptr; }
			friend bool operator<=( const expression_ref& lhs, const expression_ref& rhs ) { return lhs.ptr >= rhs.ptr; }
			friend bool operator> ( const expression_ref& lhs, const expression_ref& rhs ) { return lhs.ptr >  rhs.ptr; }
			friend bool operator< ( const expression_ref& lhs, const expression_ref& rhs ) { return lhs.ptr <  rhs.ptr; }
		};
	}
}

#endif //ndef IG_BRAIN_GRAMMAR
