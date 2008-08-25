#define GENERATING_GRAMMAR

// -> GRAMMAR_ID
// -> GRAMMAR_EXPR
// -> GRAMMAR_FILE


//-----------------------------------------------------------------------------------------------------------
// Definitions that last throughout the grammar
//-----------------------------------------------------------------------------------------------------------
#define RULE( name, expressions, expression ) LEAF_RULE( name, BOOST_PP_SEQ_FOR_EACH( DO_RULE_EXPRS_TO_LEAF, ~, expressions ), expression )
#define GRAMMAR_CLOSURE  BOOST_PP_CAT(GRAMMAR_ID,_closure)
#define GRAMMAR_EXPR_REF ::brain::grammar::expression_ref< GRAMMAR_EXPR >
//-----------------------------------------------------------------------------------------------------------




struct GRAMMAR_CLOSURE : public boost::spirit::closure< GRAMMAR_CLOSURE, GRAMMAR_EXPR_REF > {
	member1 expr;
};

class GRAMMAR_ID : public boost::spirit::grammar< GRAMMAR_ID, GRAMMAR_CLOSURE::context_t > {
public:
	typedef ::brain::grammar::expression_ref< GRAMMAR_EXPR > _expression_reference_t;

public:
	template < typename ScannerT > struct definition {
		struct group_closure : boost::spirit::closure< group_closure, _expression_reference_t > {
			member1 expression;
		};




//-----------------------------------------------------------------------------------------------------------
// Step 1:  Generate LEAF_RULE, RULE, and RULE_GROUP rule classes and actor classes, the rule member variables, and start()
//-----------------------------------------------------------------------------------------------------------
#define DO_RULE_EXPRS_TO_LEAF(             z, unused, expression ) (( _expression_reference_t , expression ))
#define DO_CLOSURE_MEMBER(                 z, unused, i, closure ) BOOST_PP_CAT(member,BOOST_PP_ADD(i,1)) BOOST_PP_TUPLE_ELEM(2,1,closure);
#define DO_CLOSURE_TYPES(                  z, unused, closure ) , BOOST_PP_TUPLE_ELEM(2,0,closure)
#define DO_CLONE_CLOSURE_VALUE_IN_CONTEXT( z, unused, closure ) BOOST_PP_TUPLE_ELEM(2,0,closure) BOOST_PP_TUPLE_ELEM(2,1,closure);
#define DO_CLONE_CLOSURE_VALUE_INIT(       z, unused, closure ) , BOOST_PP_TUPLE_ELEM(2,1,closure)( rule.BOOST_PP_TUPLE_ELEM(2,1,closure)() )
#define DO_CLONE_CLOSURE_VALUE_SAVE(       z, unused, closure ) (rule.BOOST_PP_TUPLE_ELEM(2,1,closure) = BOOST_PP_TUPLE_ELEM(2,1,closure) )();
//-----------------------------------------------------------------------------------------------------------
#define LEAF_RULE( name, closures, _ /* expression */ )                                                     \
	public:                                                                                                 \
		struct name##_closure : boost::spirit::closure< name##_closure                                      \
			BOOST_PP_SEQ_FOR_EACH( DO_CLOSURE_TYPES, ~, ((_expression_reference_t,expression))closures ) >  \
		{                                                                                                   \
			BOOST_PP_SEQ_FOR_EACH_I( DO_CLOSURE_MEMBER, ~, ((_expression_reference_t,expression))closures ) \
		};                                                                                                  \
		struct name##_rule : boost::spirit::rule< ScannerT, typename name##_closure::context_t > {          \
			inline name##_rule();                                                                           \
			template < typename T > void operator=( const T& expr ) { rule::operator=(expr); }              \
		};                                                                                                  \
		struct name##_context {                                                                             \
			name##_rule& rule;                                                                              \
			BOOST_PP_SEQ_FOR_EACH(DO_CLONE_CLOSURE_VALUE_IN_CONTEXT,~,closures)                             \
		    inline _expression_reference_t::autoconv act() const;                                           \
			name##_context( name##_rule& rule ): rule(rule)                                                 \
				BOOST_PP_SEQ_FOR_EACH( DO_CLONE_CLOSURE_VALUE_INIT,~,closures )                             \
			{                                                                                               \
			}                                                                                               \
			inline void save() const {                                                                      \
				BOOST_PP_SEQ_FOR_EACH(DO_CLONE_CLOSURE_VALUE_SAVE,~,closures)                               \
			}                                                                                               \
		};                                                                                                  \
		static name##_rule  name;                                                                           \
	private:                                                                                                \
		struct name##_actor {                                                                               \
		private:                                                                                            \
			name##_rule& rule;                                                                              \
		public:                                                                                             \
			name##_actor( name##_rule& rule ): rule(rule) {}                                                \
			template < typename U > void operator()( const U& value ) const {                               \
				name##_context context(rule);                                                               \
				_expression_reference_t expression = context.act();                                         \
				(rule.expression = expression)(value); context.save();                                      \
			}                                                                                               \
			template < typename I > void operator()( const I& first, const I& last ) const {                \
				name##_context context(rule);                                                               \
				_expression_reference_t expression = context.act();                                         \
				(rule.expression = expression)(first,last); context.save();                                 \
			}                                                                                               \
		};                                                                                                  \
	private:                                                                                                \
		template < typename T > static void eat_ ## name ## _rule_impl()                /* {...} goes here */
//-----------------------------------------------------------------------------------------------------------
#define RULE_GROUP( name, rules )                                                                           \
	public:                                                                                                 \
		struct name##_rule : boost::spirit::rule< ScannerT, typename group_closure::context_t > {           \
			inline name##_rule();                                                                           \
			template < typename T > void operator=( const T& expr ) { rule::operator=(expr); }              \
		};                                                                                                  \
		static name##_rule name;                                                                         /**/
//-----------------------------------------------------------------------------------------------------------
#define START_AT( name )                                                                                    \
	private:                                                                                                \
		struct start1_rule : boost::spirit::rule< ScannerT, typename group_closure::context_t > {           \
			inline start1_rule() { using namespace boost::spirit; *this = name[this->expression=phoenix::arg1]; } \
			template < typename T > void operator=( const T& expr ) { rule::operator=(expr); }              \
		} start1;                                                                                           \
		typedef boost::spirit::rule< ScannerT > start0_type;                                                \
		start0_type start0;                                                                                 \
	public:                                                                                                 \
		const start0_type& start() const { return start0; }                                              /**/
//-----------------------------------------------------------------------------------------------------------
#include GRAMMAR_FILE
#undef LEAF_RULE
#undef RULE_GROUP
#undef START_AT
#undef DO_RULE_EXPRS_TO_LEAF
#undef DO_CLOSURE_MEMBER
#undef DO_CLOSURE_TYPES
#undef DO_CLONE_CLOSURE_VALUE_IN_CONTEXT
#undef DO_CLONE_CLOSURE_VALUE_INIT
#undef DO_CLONE_CLOSURE_VALUE_SAVE
//-----------------------------------------------------------------------------------------------------------




		const GRAMMAR_ID& self;
	public:
		definition( const GRAMMAR_ID& self ): self(self) {
			using namespace boost::spirit;
			start0 = start1[self.expr=phoenix::arg1];
		}
	};
};





//-----------------------------------------------------------------------------------------------------------
// Step 2:  Generate RULE and RULE_GROUP initializations and actor function definition bodies
//-----------------------------------------------------------------------------------------------------------
#define DO_LOCAL_SCOPE_CLOSURE_VAR( z, name, i, closure ) typename BOOST_PP_CAT(name,_closure) :: BOOST_PP_CAT( member, BOOST_PP_ADD(2,i) )& BOOST_PP_TUPLE_ELEM(2,1,closure) = name.BOOST_PP_TUPLE_ELEM(2,1,closure);
#define DO_RULE_GROUP_ALT( z, rulegroup, i, name ) BOOST_PP_IF(i,|,) (name[rulegroup.expression=phoenix::arg1])
//-----------------------------------------------------------------------------------------------------------
#define LEAF_RULE( name, closures, expression )                                                             \
template < typename ScannerT > typename GRAMMAR_ID::definition<ScannerT>::name##_rule GRAMMAR_ID::definition<ScannerT>::name; \
template < typename ScannerT > GRAMMAR_ID::definition<ScannerT>::name##_rule::name##_rule() {               \
	using namespace boost::spirit;                                                                          \
	using namespace phoenix;                                                                                \
	*this = ( expression ) >> eps_p[ name##_actor(*this) ];                                                 \
}                                                                                                           \
template < typename ScannerT > GRAMMAR_ID::_expression_reference_t::autoconv GRAMMAR_ID::definition<ScannerT>::name##_context::act() const /* {...} */
//-----------------------------------------------------------------------------------------------------------
#define RULE_GROUP( name, rules )                                                                           \
template < typename ScannerT > typename GRAMMAR_ID::definition<ScannerT>::name##_rule GRAMMAR_ID::definition<ScannerT>::name; \
template < typename ScannerT > GRAMMAR_ID::definition<ScannerT>::name##_rule::name##_rule() {               \
	using namespace boost::spirit;                                                                          \
	*this = ( BOOST_PP_SEQ_FOR_EACH_I( DO_RULE_GROUP_ALT, (*this), rules ) );                               \
}                                                                                                        /**/
//-----------------------------------------------------------------------------------------------------------
#define START_AT( name )
#include GRAMMAR_FILE
#undef LEAF_RULE
#undef RULE_GROUP
#undef START_AT
#undef DO_LOCAL_SCOPE_CLOSURE_VAR
#undef DO_RULE_GROUP_ALT
//-----------------------------------------------------------------------------------------------------------



#undef RULE
#undef GRAMMAR_CLOSURE
#undef GRAMMAR_EXPR_REF
#undef GRAMMAR_FILE
#undef GRAMMAR_EXPR
#undef GRAMMAR_ID
#undef GENERATING_GRAMMAR
