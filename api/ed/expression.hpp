#ifndef EXPRESSION_25_12_2013
#define EXPRESSION_25_12_2013

#include "skip.hpp"
#include "parser.hpp"

#include <boost/spirit/include/qi.hpp>

#include <boost/phoenix/core.hpp>
#include <boost/phoenix/stl.hpp>
#include <boost/phoenix/fusion.hpp>
#include <boost/phoenix/operator.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

#include <boost/algorithm/string/erase.hpp>

namespace Ed
{

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//Expression
class Expression : public std::string
{
public:
    Expression(){}
    Expression( const std::string& str ) : std::string( str ) {}
    Expression( const char* pszString ) : std::string( pszString ) {}

    std::string string() const { return boost::erase_all_copy( *this, "\"" ); }
};

static const char EXPRESSION_DELIMITER = ';';
static const char EXP_OPEN = '(';
static const char EXP_CLOSE = ')';
//static const char EXPRESSION_NOTALLOWED[] = "{}";

template< typename Iterator >
struct ExpressionGrammar : boost::spirit::qi::grammar< Iterator, SkipGrammar< Iterator >, Expression() >
{
    ExpressionGrammar() : ExpressionGrammar::base_type( m_main_rule, "expression" )
    {
        using namespace boost::spirit;
        using namespace boost::spirit::qi;
        using namespace boost::phoenix;

        m_rule_string = +(char_ - char_( EXP_OPEN ) - char_( EXP_CLOSE ) - 
            char_( EXPRESSION_DELIMITER ) )[ push_back( _val, qi::_1 ) ];
        
        m_rule_code_block =    m_rule_string[ _val += qi::_1 ] |
                                (
                                    char_( EXP_OPEN )[ push_back( _val, EXP_OPEN )  ] >>
                                    *m_rule_code_block[ _val += qi::_1 ]   >>
                                    char_( EXP_CLOSE )[ push_back( _val, EXP_CLOSE )  ]       
                                );

        m_main_rule =   *( 
                            m_rule_string[ _val += qi::_1 ] ||
                            ( 
                                char_( EXP_OPEN )[ _val += EXP_OPEN ] >> 
                                no_skip[ *m_rule_code_block[ _val += qi::_1 ] ] >> 
                                char_( EXP_CLOSE )[ _val += EXP_CLOSE ]  
                            )
                        ) >>
                        char_( EXPRESSION_DELIMITER );
    }
    
    boost::spirit::qi::rule< Iterator, Expression() > m_rule_string;
    boost::spirit::qi::rule< Iterator, Expression() > m_rule_code_block;
    boost::spirit::qi::rule< Iterator, SkipGrammar< Iterator >, Expression() > m_main_rule;
};

ParseResult parse( const std::string& strInput, Expression& expression, std::ostream& errorStream );

}

#endif //EXPRESSION_25_12_2013