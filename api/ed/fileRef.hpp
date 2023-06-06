#ifndef FILE_25_12_2013
#define FILE_25_12_2013

#include "skip.hpp"
#include "parser.hpp"

#include <boost/spirit/include/qi.hpp>

#include <boost/phoenix/core.hpp>
#include <boost/phoenix/stl.hpp>

#include <boost/variant.hpp>

#include <vector>

namespace Ed
{

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//File
    
static const char PATH_DELIMITER = '/';
static const char PATH_UP[] = "..";

enum FileActionType
{
    eFileUp,
    TOTAL_FILEREF_ACTION_TYPES
};
typedef boost::variant< FileActionType, std::string > FileVariant;

class FileRef : public std::vector< FileVariant >
{
public:
    typedef std::vector< FileRef > Vector;

};

template< typename Iterator >
class FileRefGrammar : public boost::spirit::qi::grammar< Iterator, SkipGrammar< Iterator >, FileRef() >
{
public:
    FileRefGrammar() : FileRefGrammar::base_type( m_main_rule, "file" )
    {
        using namespace boost::spirit;
        using namespace boost::spirit::qi;
        using namespace boost::phoenix;

        m_rule_pathstring = +char_( "a-zA-Z0-9_" );
        m_rule_dash = char_( PATH_DELIMITER );
        m_rule_dotdot = qi::string( ".." );

        m_main_rule = -m_rule_dotdot[ push_back( _val, eFileUp ) ] >> 
            *( m_rule_dash >> m_rule_dotdot[ push_back( _val, eFileUp ) ] ) >>
            +( m_rule_dash >> m_rule_pathstring[ push_back( _val, qi::_1 ) ] );

        m_rule_pathstring.name( "path_string" );
        m_rule_dash.name( "dash" );
        m_rule_dotdot.name( "dotdot" );
    }
    
    boost::spirit::qi::rule< Iterator, SkipGrammar< Iterator >, std::string() > m_rule_pathstring;
    boost::spirit::qi::rule< Iterator, SkipGrammar< Iterator >, std::string() > m_rule_dash;
    boost::spirit::qi::rule< Iterator, SkipGrammar< Iterator >, std::string() > m_rule_dotdot;
    boost::spirit::qi::rule< Iterator, SkipGrammar< Iterator >, FileRef() >     m_main_rule;
};

ParseResult parse( const std::string& strInput, FileRef& fileReference, std::ostream& errorStream );
bool operator==( const FileRef& left, const FileRef& right );

}

#endif //FILE_25_12_2013