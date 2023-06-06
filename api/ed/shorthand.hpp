#ifndef SHORTHAND_25_12_2013
#define SHORTHAND_25_12_2013

#include "argument.hpp"
#include "skip.hpp"
#include "parser.hpp"

#include "common/variant_utils.hpp"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_real.hpp>
#include <boost/spirit/include/qi_int.hpp>
#include <boost/spirit/include/qi_bool.hpp>

#include <boost/phoenix/core.hpp>
#include <boost/phoenix/stl.hpp>
#include <boost/phoenix/fusion.hpp>
#include <boost/phoenix/operator.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

#include <boost/variant.hpp>

#include <vector>
#include <stdint.h>

namespace Ed
{

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//Shorthand
class Shorthand;

typedef boost::variant< boost::recursive_wrapper< Shorthand >, ArgumentVariant > ShorthandVariant;

class Shorthand
{
public:
    Shorthand(){}
    Shorthand( const Shorthand& sh );
    Shorthand( const ArgumentVariant& value );
    Shorthand( const ArgumentVariant& v1, const ArgumentVariant& v2 );
    Shorthand( const ArgumentVariant& v1, const ArgumentVariant& v2, const ArgumentVariant& v3 );
    Shorthand( const ArgumentVariant& v1, const ArgumentVariant& v2, const ArgumentVariant& v3, const ArgumentVariant& v4 );
    Shorthand( const std::vector< ShorthandVariant >& _children );
    Shorthand( const Ed::Ref& ref );
    Shorthand( const Ed::FileRef& ref );
    Shorthand( const std::vector< ArgumentVariant >& args );

    template<typename T,
        template< typename ELEM, typename ALLOC = std::allocator<ELEM> > class CONT >
    Shorthand( const CONT< T >& vec )
    {
        for( typename CONT< T >::const_iterator i = vec.begin(),
            iEnd = vec.end(); i!=iEnd; ++i )
        {
            children.push_back( ( ShorthandVariant( ( ed_cast( *i ) ) ) ) );
        }
    }
	
	void push_back( const ArgumentVariant& av );

    boost::optional< const ShorthandVariant& > get( uint32_t uiIndex ) const
    {
        boost::optional< const ShorthandVariant& > result;
        if( children.size() > uiIndex )
        {
            std::vector< ShorthandVariant >::const_iterator i = children.begin() + uiIndex;
            result = *i;
        }
        return result;
    }

    template< class T >
    boost::optional< const T& > getArg( uint32_t uiIndex ) const
    {
        boost::optional< const T& > result;
        if( boost::optional< const ShorthandVariant& > shOpt = get( uiIndex ) )
            result = boost::apply_visitor( boost::TypeAccessor< const T >(), shOpt.get() );
        return result;
    }

    template< class T >
    inline boost::optional< const T& > getValue( uint32_t uiIndex ) const
    {
        boost::optional< const T& > result;
        if( boost::optional< const ArgumentVariant& > arg = getArg< ArgumentVariant >( uiIndex ) )
        {
             result = boost::apply_visitor( boost::TypeAccessor< const T >(), arg.get() );
        }
        return result;
    }

    template< class T >
    inline boost::optional< const T& > getValue( uint32_t uiIndex1, uint32_t uiIndex2 ) const
    {
        boost::optional< const T& > result;
        if( boost::optional< const Shorthand& > s = getArg< Shorthand >( uiIndex1 ) )
        {
            result = s.get().getValue< T >( uiIndex2 );
        }
        return result;
    }

    template< class T >
    inline boost::optional< const T& > getValue( uint32_t uiIndex1, uint32_t uiIndex2, uint32_t uiIndex3 ) const
    {
        boost::optional< const T& > result;
        if( boost::optional< const Shorthand& > s = getArg< Shorthand >( uiIndex1 ) )
        {
            result = s.get().getValue< T >( uiIndex2, uiIndex3 );
        }
        return result;
    }
	
    typedef std::vector< ShorthandVariant > ShortHandVariantVector;
    ShortHandVariantVector children;        // children
};
    
template< typename Iterator >
class ShorthandGrammar : public boost::spirit::qi::grammar< Iterator, SkipGrammar< Iterator >, Shorthand() >
{
public:
    ShorthandGrammar() : ShorthandGrammar::base_type( m_main_rule, "shorthand" )
    {
        using namespace boost::spirit;
        using namespace boost::spirit::qi;
        using namespace boost::phoenix;

        m_main_rule = lit( SH_OPEN ) >> ( m_main_rule | m_grammar_argument )[ push_back( at_c<0>(_val), qi::_1 ) ] >> 
                            *( SH_DELIMITER >> ( m_main_rule | m_grammar_argument )[ push_back( at_c<0>(_val), qi::_1 ) ] ) > SH_CLOSE;

    }
    
    ArgumentGrammar< Iterator > m_grammar_argument;
    boost::spirit::qi::rule< Iterator, SkipGrammar< Iterator >, Shorthand() > m_main_rule;
};

}

BOOST_FUSION_ADAPT_STRUCT(
    Ed::Shorthand,
    (std::vector< Ed::ShorthandVariant >, children)
)

namespace Ed
{

bool compareShortHand( const Shorthand& sh, const ShorthandVariant& v );
ParseResult parse( const std::string& strInput, Shorthand& shortHand, std::ostream& errorStream );
bool operator==( const Shorthand& left, const Shorthand& right );

void propagate( ShorthandVariant& shorthand );
}

#endif //SHORTHAND_25_12_2013