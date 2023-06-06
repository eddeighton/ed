#ifndef REF_25_12_2013
#define REF_25_12_2013

#include "identifier.hpp"
#include "skip.hpp"
#include "parser.hpp"

#include <boost/spirit/include/qi.hpp>

#include <boost/phoenix/core.hpp>
#include <boost/phoenix/stl.hpp>
#include <boost/phoenix/fusion.hpp>
#include <boost/phoenix/operator.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <vector>
#include <string>

namespace Ed
{

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//Reference
static const char REF_DELIMITER = '.';
static const char ASSOCIATION_DELIMITER = ':';
static const char REF_BRANCH_OPEN = '[';
static const char REF_BRANCH_CLOSE = ']';
static const char REF_BRANCH_DELIMITER = ',';

enum RefActionType
{
    eRefUp,
    TOTAL_REF_ACTION_TYPES
};

class Reference;
class Ref;

typedef boost::variant< RefActionType, Ed::Identifier, boost::recursive_wrapper< Ref > > RefVariant;

class Reference : public std::vector< RefVariant >
{
public:
    Reference(){}
    Reference( std::initializer_list< Ed::Identifier > events )
    {
        std::copy( events.begin(), events.end(), std::back_inserter( *this ) );
    }
    Reference( const_iterator iBegin, const_iterator iEnd ) : std::vector< RefVariant >( iBegin, iEnd ) {}
    Reference& operator+=( const Reference& ref )
    {
        std::copy( ref.begin(), ref.end(), std::back_inserter( *this ) );
        return *this;
    }
};

class Ref : public std::vector< Reference >
{
public:
    typedef std::vector< Ref > Vector;

};

}

namespace Ed
{
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
template< typename Iterator >
struct ReferenceGrammar : boost::spirit::qi::grammar< Iterator, SkipGrammar< Iterator >, Reference() >
{
    ReferenceGrammar() : ReferenceGrammar::base_type( m_main_rule, "reference" )
    {
        using namespace boost::spirit;
        using namespace boost::spirit::qi;
        using namespace boost::phoenix;
        
        using boost::phoenix::insert;
        using boost::phoenix::end;
        using boost::phoenix::begin;

        m_recursive_rule1 =
            ( m_identifier_grammar[ push_back( _val, qi::_1 ) ] || m_recursive_rule2[ push_back( _val, qi::_1 ) ] ) >> 
            *( char_( REF_DELIMITER ) >> ( m_identifier_grammar[ push_back( _val, qi::_1 ) ] || m_recursive_rule2[ push_back( _val, qi::_1 ) ] ) );

        m_recursive_rule2 = 
            char_( REF_BRANCH_OPEN ) >> m_recursive_rule1[ push_back( _val, qi::_1 ) ] >> 
                +( char_( REF_BRANCH_DELIMITER ) >> m_recursive_rule1[ push_back( _val, qi::_1 ) ] ) >> char_( REF_BRANCH_CLOSE );

        m_main_rule = 
            char_( REF_DELIMITER ) >> 
                   -( 
                        *char_( REF_DELIMITER )[ push_back( _val, eRefUp ) ] >> //initial ref ups
                        *m_recursive_rule1[ insert( _val, end( _val ), begin( qi::_1 ), end( qi::_1 ) ) ]
                    );
    }
    
    IdentifierGrammar< Iterator > m_identifier_grammar;
    boost::spirit::qi::rule< Iterator, SkipGrammar< Iterator >, Reference() >   m_recursive_rule1;
    boost::spirit::qi::rule< Iterator, SkipGrammar< Iterator >, Ref() >         m_recursive_rule2;
    boost::spirit::qi::rule< Iterator, SkipGrammar< Iterator >, Reference() >   m_main_rule;
};

ParseResult parse( const std::string& strInput, Reference& reference, std::ostream& errorStream );
bool operator==( const Reference& left, const Reference& right );

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//Ref

inline Ref make_ref( const Reference& r )
{
    Ref ref;
    ref.push_back( r );
    return ref;
}

template< typename Iterator >
struct RefGrammar : boost::spirit::qi::grammar< Iterator, SkipGrammar< Iterator >, Ref() >
{
    RefGrammar() : RefGrammar::base_type( m_main_rule, "ref" )
    {
        using namespace boost::spirit;
        using namespace boost::spirit::qi;
        using namespace boost::phoenix;

        m_main_rule =
            m_reference_grammar[ push_back( _val, qi::_1 ) ] >> 
                *( char_( ASSOCIATION_DELIMITER ) >> m_reference_grammar[ push_back( _val, qi::_1 ) ] );
    }
    
    ReferenceGrammar< Iterator > m_reference_grammar;
    boost::spirit::qi::rule< Iterator, SkipGrammar< Iterator >, Ref() >        m_main_rule;
};

ParseResult parse( const std::string& strInput, Ref& reference, std::ostream& errorStream );
bool operator==( const Ref& left, const Ref& right );

}

#endif //REF_25_12_2013
