#ifndef STATEMENT_25_12_2013
#define STATEMENT_25_12_2013

#include "shorthand.hpp"
#include "tag.hpp"
#include "declarator.hpp"
#include "skip.hpp"
#include "parser.hpp"

#include <boost/spirit/include/qi.hpp>

#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

#include <boost/optional.hpp>

#include <string>
#include <sstream>

namespace Ed
{

//Statement
class Statement
{
public:
    Statement()
    {}
    Statement( const Declarator& _declarator )
        : declarator( _declarator )
    {}
    Statement( const Declarator& _declarator, const TagList& _tagList )
        : declarator( _declarator ), tagList( _tagList )
    {}
    Statement( const Declarator& _declarator, const Shorthand& _shorthand )
        : declarator( _declarator ), shorthand( _shorthand )
    {}
    Statement( const Declarator& _declarator, const TagList& _tagList, const Shorthand& _shorthand )
        : declarator( _declarator ), tagList( _tagList ), shorthand( _shorthand )
    {}
    std::string getIdentifier() const;
    bool getIdentifiers( IdentifierSet& identifiers ) const;
    bool getFileRefs( FileRef::Vector& files ) const;
    bool getRefs( Ref::Vector& refs ) const;
    
    template< class T >
    void addTag( const T& tag )
    {
        if( !tagList ) tagList = TagList();
        tagList.get().push_back( tag );
    }

    Declarator declarator;
    boost::optional< TagList > tagList;
    boost::optional< Shorthand > shorthand;
};

}

BOOST_FUSION_ADAPT_STRUCT(
    Ed::Statement,
    (Ed::Declarator, declarator)
    (boost::optional< Ed::TagList >, tagList)
    (boost::optional< Ed::Shorthand >, shorthand)
)

namespace Ed
{

template< typename Iterator >
class StatementGrammar : public boost::spirit::qi::grammar< Iterator, SkipGrammar< Iterator >, Statement() >
{
public:
    StatementGrammar() : StatementGrammar::base_type( m_main_rule, "statement" )
    {
        using namespace boost::spirit;
        using namespace boost::spirit::qi;
        using namespace boost::phoenix;

        m_main_rule = m_grammarDeclarator[ at_c<0>(_val) = qi::_1 ] >> 
                        -m_grammarTagList[ at_c<1>(_val) = qi::_1 ] >>
                        -m_grammarShorthand[ at_c<2>(_val) = qi::_1 ];
    }

    DeclaratorGrammar< Iterator >   m_grammarDeclarator;
    TagListGrammar< Iterator >      m_grammarTagList;
    ShorthandGrammar< Iterator >    m_grammarShorthand;
    boost::spirit::qi::rule< Iterator, SkipGrammar< Iterator >, Statement() >   m_main_rule;
};

ParseResult parse( const std::string& strInput, Statement& statement, std::ostream& errorStream );

bool operator==( const Statement& left, const Statement& right );

}

#endif //STATEMENT_25_12_2013
