#ifndef IDENTIFIER_25_12_2013
#define IDENTIFIER_25_12_2013

#include "parser.hpp"

#include <boost/spirit/include/qi.hpp>

#include <boost/phoenix/core.hpp>
#include <boost/phoenix/stl.hpp>

#include <set>
#include <string>
#include <ostream>
#include <algorithm>

namespace Ed
{

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//Identifier
class Identifier : public std::string
{
public:
    class Compare
    {
    public:
        inline bool operator()( const Ed::Identifier& left, const Ed::Identifier& right ) const
        {
            return std::lexicographical_compare( 
                left.begin(), left.end(), right.begin(), right.end() );
        }
    };

    Identifier(){}
    Identifier( const std::string& str ) : std::string( str ) {}
    Identifier( const char* pszStr ) : std::string( pszStr ) {}
};

typedef std::set< Identifier > IdentifierSet;
    
template< typename Iterator >
class IdentifierGrammar : public boost::spirit::qi::grammar< Iterator, Identifier() >
{
public:
    IdentifierGrammar() : IdentifierGrammar::base_type( m_main_rule, "identifier" )
    {
        using namespace boost::spirit;
        using namespace boost::spirit::qi;
        using namespace boost::phoenix;
        m_main_rule = lexeme[char_( "a-z" )[ push_back( _val, qi::_1 ) ] >> 
                        *(char_( "a-zA-Z0-9_" )[ push_back( _val, qi::_1 ) ])];
    }

    boost::spirit::qi::rule< Iterator, Identifier() > m_main_rule;
};

ParseResult parse( const std::string& strInput, Identifier& identifier, std::ostream& errorStream );

}

#endif //IDENTIFIER_25_12_2013