#ifndef IG_BRAIN_X86_EXPRESSIONS
#define IG_BRAIN_X86_EXPRESSIONS

#include "brain.x86.registers.hpp"

namespace brain {
	namespace x86 {
		template < typename R, size_t A > struct typed_reg_expr                   { R reg;                               };
		template < typename R           > struct untyped_reg_expr                 { R reg;                               template < size_t A > operator typed_reg_expr<R,A>()                 const { typed_reg_expr<R,A>                 expr = { reg }; return expr; } };
		template < typename R           > struct scaled_reg_expr                  { int scale; R reg;                    };
		template < typename R, size_t A > struct typed_scaled_reg_expr            { int scale; R reg;                    };
		template < typename R           > struct untyped_scaled_reg_expr          { int scale; R reg;                    template < size_t A > operator typed_scaled_reg_expr<R,A>()          const { typed_scaled_reg_expr<R,A>          expr = { scale, reg }; return expr; } };
		template < typename R           > struct reg_reg_expr                     { R reg1;            R reg2;           };
		template < typename R, size_t A > struct typed_reg_reg_expr               { R reg1;            R reg2;           };
		template < typename R           > struct untyped_reg_reg_expr             { R reg1;            R reg2;           template < size_t A > operator typed_reg_reg_expr<R,A>()             const { typed_reg_reg_expr<R,A>             expr = { reg1, reg2 }; return expr; } };
		template < typename R           > struct reg_scaled_reg_expr              { R reg1; int scale; R reg2;           };
		template < typename R, size_t A > struct typed_reg_scaled_reg_expr        { R reg1; int scale; R reg2;           };
		template < typename R           > struct untyped_reg_scaled_reg_expr      { R reg1; int scale; R reg2;           template < size_t A > operator typed_reg_scaled_reg_expr<R,A>()      const { typed_reg_scaled_reg_expr<R,A>      expr = { reg1, scale, reg2 }; return expr; } };
		template < typename R           > struct reg_disp_expr                    { R reg;                     int disp; };
		template < typename R, size_t A > struct typed_reg_disp_expr              { R reg;                     int disp; };
		template < typename R           > struct untyped_reg_disp_expr            { R reg;                     int disp; template < size_t A > operator typed_reg_disp_expr<R,A>()            const { typed_reg_disp_expr<R,A>            expr = { reg1, disp }; return expr; } };
		template < typename R           > struct reg_reg_disp_expr                { R reg1;            R reg2; int disp; };
		template < typename R, size_t A > struct typed_reg_reg_disp_expr          { R reg1;            R reg2; int disp; };
		template < typename R           > struct untyped_reg_reg_disp_expr        { R reg1;            R reg2; int disp; template < size_t A > operator typed_reg_reg_disp_expr<R,A>()        const { typed_reg_reg_disp_expr<R,A>        expr = { reg1, reg2, disp }; return expr; } };
		template < typename R           > struct reg_scaled_reg_disp_expr         { R reg1; int scale; R reg2; int disp; };
		template < typename R, size_t A > struct typed_reg_scaled_reg_disp_expr   { R reg1; int scale; R reg2; int disp; };
		template < typename R           > struct untyped_reg_scaled_reg_disp_expr { R reg1; int scale; R reg2; int disp; template < size_t A > operator typed_reg_scaled_reg_disp_expr<R,A>() const { typed_reg_scaled_reg_disp_expr<R,A> expr = { reg1, scale, reg2, disp }; return expr; } };
		
		scaled_reg_expr<r16>          operator*( int scale                          , r16 reg  ) { scaled_reg_expr<r16>          d = { scale, reg }; return d; }
		scaled_reg_expr<r32>          operator*( int scale                          , r32 reg  ) { scaled_reg_expr<r32>          d = { scale, reg }; return d; }
		reg_reg_expr<r16>             operator+( r16 reg1                           , r16 reg2 ) { reg_reg_expr<r16>             d = { reg1   ,             reg2           }; return d; }
		reg_reg_expr<r32>             operator+( r32 reg1                           , r32 reg2 ) { reg_reg_expr<r32>             d = { reg1   ,             reg2           }; return d; }
		reg_scaled_reg_expr<r16>      operator+( r16 reg1   , const scaled_reg_expr<r16>& reg2 ) { reg_scaled_reg_expr<r16>      d = { reg1   , reg2.scale, reg2.reg       }; return d; }
		reg_scaled_reg_expr<r32>      operator+( r32 reg1   , const scaled_reg_expr<r32>& reg2 ) { reg_scaled_reg_expr<r32>      d = { reg1   , reg2.scale, reg2.reg       }; return d; }
		reg_disp_expr<r16>            operator+( r16 reg                            , int disp ) { reg_disp_expr<r16>            d = { reg    ,                      +disp }; return d; }
		reg_disp_expr<r32>            operator+( r32 reg                            , int disp ) { reg_disp_expr<r32>            d = { reg    ,                      +disp }; return d; }
		reg_disp_expr<r16>            operator-( r16 reg                            , int disp ) { reg_disp_expr<r16>            d = { reg    ,                      -disp }; return d; }
		reg_disp_expr<r32>            operator-( r32 reg                            , int disp ) { reg_disp_expr<r32>            d = { reg    ,                      -disp }; return d; }
		reg_reg_disp_expr<r16>        operator+( const reg_reg_expr<r16>       & rr , int disp ) { reg_reg_disp_expr<r16>        d = { rr.reg1,             rr.reg2, +disp }; return d; }
		reg_reg_disp_expr<r32>        operator+( const reg_reg_expr<r32>       & rr , int disp ) { reg_reg_disp_expr<r32>        d = { rr.reg1,             rr.reg2, +disp }; return d; }
		reg_reg_disp_expr<r16>        operator-( const reg_reg_expr<r16>       & rr , int disp ) { reg_reg_disp_expr<r16>        d = { rr.reg1,             rr.reg2, -disp }; return d; }
		reg_reg_disp_expr<r32>        operator-( const reg_reg_expr<r32>       & rr , int disp ) { reg_reg_disp_expr<r32>        d = { rr.reg1,             rr.reg2, -disp }; return d; }
		reg_scaled_reg_disp_expr<r16> operator+( const reg_scaled_reg_expr<r16>& rr , int disp ) { reg_scaled_reg_disp_expr<r16> d = { rr.reg1, rr.scale  , rr.reg2, +disp }; return d; }
		reg_scaled_reg_disp_expr<r32> operator+( const reg_scaled_reg_expr<r32>& rr , int disp ) { reg_scaled_reg_disp_expr<r32> d = { rr.reg1, rr.scale  , rr.reg2, +disp }; return d; }
		reg_scaled_reg_disp_expr<r16> operator-( const reg_scaled_reg_expr<r16>& rr , int disp ) { reg_scaled_reg_disp_expr<r16> d = { rr.reg1, rr.scale  , rr.reg2, -disp }; return d; }
		reg_scaled_reg_disp_expr<r32> operator-( const reg_scaled_reg_expr<r32>& rr , int disp ) { reg_scaled_reg_disp_expr<r32> d = { rr.reg1, rr.scale  , rr.reg2, -disp }; return d; }

		namespace {
			struct {
				typed_reg_expr<r16,8 >                                       operator[](       r16                          expr ) const { typed_reg_expr<r16,8 >               d = { expr                                        }; return d; }
				typed_reg_expr<r32,8 >                                       operator[](       r32                          expr ) const { typed_reg_expr<r32,8 >               d = { expr                                        }; return d; }
				template < typename R > typed_scaled_reg_expr<R,8 >          operator[]( const scaled_reg_expr<R>&          expr ) const { typed_scaled_reg_expr<R,8 >          d = { expr.scale, expr.reg                        }; return d; }
				template < typename R > typed_reg_reg_expr<R,8 >             operator[]( const reg_reg_expr<R>&             expr ) const { typed_reg_reg_expr<R,8 >             d = { expr.reg1,             expr.reg2            }; return d; }
				template < typename R > typed_reg_scaled_reg_expr<R,8 >      operator[]( const reg_scaled_reg_expr<R>&      expr ) const { typed_reg_scaled_reg_expr<R,8 >      d = { expr.reg1, expr.scale, expr.reg2            }; return d; }
				template < typename R > typed_reg_disp_expr<R,8 >            operator[]( const reg_disp_expr<R>&            expr ) const { typed_reg_disp_expr<R,8 >            d = { expr.reg ,                        expr.disp }; return d; }
				template < typename R > typed_reg_reg_disp_expr<R,8 >        operator[]( const reg_reg_disp_expr<R>&        expr ) const { typed_reg_reg_disp_expr<R,8 >        d = { expr.reg1,             expr.reg2, expr.disp }; return d; }
				template < typename R > typed_reg_scaled_reg_disp_expr<R,8 > operator[]( const reg_scaled_reg_disp_expr<R>& expr ) const { typed_reg_scaled_reg_disp_expr<R,8 > d = { expr.reg1, expr.scale, expr.reg2, expr.disp }; return d; }
			} const byte ;
			struct {
				typed_reg_expr<r16,16>                                       operator[](       r16                          expr ) const { typed_reg_expr<r16,16>               d = { expr                                        }; return d; }
				typed_reg_expr<r32,16>                                       operator[](       r32                          expr ) const { typed_reg_expr<r32,16>               d = { expr                                        }; return d; }
				template < typename R > typed_reg_reg_expr<R,16>             operator[]( const reg_reg_expr<R>&             expr ) const { typed_reg_reg_expr<R,16>             d = { expr.reg1,             expr.reg2            }; return d; }
				template < typename R > typed_reg_scaled_reg_expr<R,16>      operator[]( const reg_scaled_reg_expr<R>&      expr ) const { typed_reg_scaled_reg_expr<R,16>      d = { expr.reg1, expr.scale, expr.reg2            }; return d; }
				template < typename R > typed_reg_disp_expr<R,16>            operator[]( const reg_disp_expr<R>&            expr ) const { typed_reg_disp_expr<R,16>            d = { expr.reg ,                        expr.disp }; return d; }
				template < typename R > typed_reg_reg_disp_expr<R,16>        operator[]( const reg_reg_disp_expr<R>&        expr ) const { typed_reg_reg_disp_expr<R,16>        d = { expr.reg1,             expr.reg2, expr.disp }; return d; }
				template < typename R > typed_reg_scaled_reg_disp_expr<R,16> operator[]( const reg_scaled_reg_disp_expr<R>& expr ) const { typed_reg_scaled_reg_disp_expr<R,16> d = { expr.reg1, expr.scale, expr.reg2, expr.disp }; return d; }
			} const word ;
			struct {
				typed_reg_expr<r16,32>                                       operator[](       r16                          expr ) const { typed_reg_expr<r16,32>               d = { expr                                        }; return d; }
				typed_reg_expr<r32,32>                                       operator[](       r32                          expr ) const { typed_reg_expr<r32,32>               d = { expr                                        }; return d; }
				template < typename R > typed_reg_reg_expr<R,32>             operator[]( const reg_reg_expr<R>&             expr ) const { typed_reg_reg_expr<R,32>             d = { expr.reg1,             expr.reg2            }; return d; }
				template < typename R > typed_reg_scaled_reg_expr<R,32>      operator[]( const reg_scaled_reg_expr<R>&      expr ) const { typed_reg_scaled_reg_expr<R,32>      d = { expr.reg1, expr.scale, expr.reg2            }; return d; }
				template < typename R > typed_reg_disp_expr<R,32>            operator[]( const reg_disp_expr<R>&            expr ) const { typed_reg_disp_expr<R,32>            d = { expr.reg ,                        expr.disp }; return d; }
				template < typename R > typed_reg_reg_disp_expr<R,32>        operator[]( const reg_reg_disp_expr<R>&        expr ) const { typed_reg_reg_disp_expr<R,32>        d = { expr.reg1,             expr.reg2, expr.disp }; return d; }
				template < typename R > typed_reg_scaled_reg_disp_expr<R,32> operator[]( const reg_scaled_reg_disp_expr<R>& expr ) const { typed_reg_scaled_reg_disp_expr<R,32> d = { expr.reg1, expr.scale, expr.reg2, expr.disp }; return d; }
			} const dword;
			struct {
				untyped_reg_expr<r16>                                       operator[](       r16                          expr ) const { untyped_reg_expr<r16>               d = { expr                                        }; return d; }
				untyped_reg_expr<r32>                                       operator[](       r32                          expr ) const { untyped_reg_expr<r32>               d = { expr                                        }; return d; }
				template < typename R > untyped_reg_reg_expr<R>             operator[]( const reg_reg_expr<R>&             expr ) const { untyped_reg_reg_expr<R>             d = { expr.reg1,             expr.reg2            }; return d; }
				template < typename R > untyped_reg_scaled_reg_expr<R>      operator[]( const reg_scaled_reg_expr<R>&      expr ) const { untyped_reg_scaled_reg_expr<R>      d = { expr.reg1, expr.scale, expr.reg2            }; return d; }
				template < typename R > untyped_reg_disp_expr<R>            operator[]( const reg_disp_expr<R>&            expr ) const { untyped_reg_disp_expr<R>            d = { expr.reg ,                        expr.disp }; return d; }
				template < typename R > untyped_reg_reg_disp_expr<R>        operator[]( const reg_reg_disp_expr<R>&        expr ) const { untyped_reg_reg_disp_expr<R>        d = { expr.reg1,             expr.reg2, expr.disp }; return d; }
				template < typename R > untyped_reg_scaled_reg_disp_expr<R> operator[]( const reg_scaled_reg_disp_expr<R>& expr ) const { untyped_reg_scaled_reg_disp_expr<R> d = { expr.reg1, expr.scale, expr.reg2, expr.disp }; return d; }
			} const _;
		}
	}
}

#endif //ndef IG_BRAIN_X86_EXPRESSIONS
