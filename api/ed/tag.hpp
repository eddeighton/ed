#ifndef TAG_25_12_2013
#define TAG_25_12_2013

#include "identifier.hpp"
#include "fileRef.hpp"
#include "ref.hpp"
#include "skip.hpp"
#include "parser.hpp"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_uint.hpp>

#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <boost/variant.hpp>

#include <vector>
#include <ostream>

namespace Ed
{

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//Tag
typedef boost::variant< unsigned int, Identifier, FileRef, Ref > Tag;


template< typename Iterator >
class TagGrammar : public boost::spirit::qi::grammar< Iterator, SkipGrammar< Iterator >, Tag() >
{
public:
    TagGrammar() : TagGrammar::base_type( m_main_rule, "tag" )
    {
        using namespace boost::spirit;
        using namespace boost::spirit::qi;
        using namespace boost::phoenix;
        
        m_main_rule = ( uint_ | m_grammar_identifier | m_grammar_fileref | m_grammar_ref )[ _val = qi::_1 ];
    }
    
    IdentifierGrammar< Iterator >               m_grammar_identifier;
    FileRefGrammar< Iterator >                  m_grammar_fileref;
    RefGrammar< Iterator >                      m_grammar_ref;
    
    boost::spirit::qi::rule< Iterator, SkipGrammar< Iterator >, Tag() >  m_main_rule;
};

ParseResult parse( const std::string& strInput, Tag& tag, std::ostream& errorStream );

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//TagList
static const char TAGS_OPEN = '<';
static const char TAGS_CLOSE = '>';
static const char TAGS_DELIMITER = ',';

class TagList : public std::vector< Tag >
{
public:
    TagList(){};
    TagList( const Tag& type1 );
    TagList( const Tag& type1, const Tag& type2 );
    TagList( const Tag& type1, const Tag& type2, const Tag& type3 );
    
    static const unsigned int NO_INDEX = 0xFFFFFFFF;
    unsigned int getShorthandIndex() const;
};

template< typename Iterator >
class TagListGrammar : public boost::spirit::qi::grammar< Iterator, SkipGrammar< Iterator >, TagList() >
{
public:
    TagListGrammar() : TagListGrammar::base_type( m_main_rule, "taglist" )
    {
        using namespace boost::spirit;
        using namespace boost::spirit::qi;
        using namespace boost::phoenix;

        m_main_rule = char_( TAGS_OPEN ) >> m_tag_grammar[ push_back( _val, qi::_1 ) ] >> 
                                    *( TAGS_DELIMITER >> m_tag_grammar[ push_back( _val, qi::_1 ) ] ) >> TAGS_CLOSE;
    }
    
    TagGrammar< Iterator >                                                      m_tag_grammar;
    boost::spirit::qi::rule< Iterator, SkipGrammar< Iterator >, TagList() >     m_main_rule;
};

ParseResult parse( const std::string& strInput, TagList& typeList, std::ostream& errorStream );


}

#endif //TAG_25_12_2013
