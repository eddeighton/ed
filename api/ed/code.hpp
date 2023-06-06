#ifndef CODE_25_12_2013
#define CODE_25_12_2013

#include "ref.hpp"
#include "parser.hpp"
#include "skip.hpp"

#include <boost/spirit/include/qi.hpp>

#include <boost/phoenix/core.hpp>
#include <boost/phoenix/stl.hpp>
#include <boost/phoenix/fusion.hpp>
#include <boost/phoenix/operator.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

namespace Ed
{

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//Code
static const char CODE_OPEN = '{';
static const char CODE_CLOSE = '}';

class Code
{
public:
    std::string code;
};
bool operator==( const Code& left, const Code& right );

inline Code make_code( const std::string& str )
{
    Code c;
    c.code = str;
    return c;
}

struct CodeReference
{
    std::size_t szStart, szEnd;
    Ref ref;
};
typedef std::vector< CodeReference > CodeReferenceVector;
void extractCodeReferences( const std::string& strCode, CodeReferenceVector& references );

}

BOOST_FUSION_ADAPT_STRUCT(
    Ed::Code,
    (std::string, code)
)

namespace Ed
{

template< typename Iterator >
class CodeGrammar : public boost::spirit::qi::grammar< Iterator, SkipGrammar< Iterator >, Code() >
{
public:
    CodeGrammar() : CodeGrammar::base_type( m_main_rule, "code" )
    {
        using namespace boost::spirit;
        using namespace boost::spirit::qi;
        using namespace boost::phoenix;

        m_rule_string = +(char_ - char_( CODE_OPEN ) - char_( CODE_CLOSE ))[ push_back( at_c<0>(_val), qi::_1 ) ];
        
        m_rule_code_block =    m_rule_string[ at_c<0>(_val) += at_c<0>( qi::_1 ) ] |
                                (
                                    char_( CODE_OPEN )[ push_back( at_c<0>(_val), CODE_OPEN )  ] >>
                                    *m_rule_code_block[ at_c<0>(_val) += at_c<0>( qi::_1 ) ]   >>
                                    char_( CODE_CLOSE )[ push_back( at_c<0>(_val), CODE_CLOSE )  ]       
                                );

        m_main_rule = char_( CODE_OPEN ) >> no_skip[ *m_rule_code_block[ at_c<0>(_val) += at_c<0>( qi::_1 ) ] ] >> char_( CODE_CLOSE );

        m_rule_string.name( "code_string" );
        m_rule_code_block.name( "code_block" );
    }
    
    boost::spirit::qi::rule< Iterator, Code() > m_rule_string;
    boost::spirit::qi::rule< Iterator, Code() > m_rule_code_block;
    boost::spirit::qi::rule< Iterator, SkipGrammar< Iterator >, Code() > m_main_rule;
};

ParseResult parse( const std::string& strInput, Code& code, std::ostream& errorStream );

}

#endif //CODE_25_12_2013