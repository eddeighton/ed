#ifndef ED_25_12_2013
#define ED_25_12_2013

#include "statement.hpp"
#include "skip.hpp"
#include "parser.hpp"

#include <boost/spirit/include/qi.hpp>

#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

#include <boost/filesystem.hpp>
#include <boost/bind.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>

namespace Ed
{
    
class Node
{
public:
    typedef std::vector< Node > Vector;

    Node()
    {}

    Node( const Statement& _statement )
        :   statement( _statement )
    {
    }

    Shorthand& getShorty()
    {
        if( !statement.shorthand )
            statement.shorthand = Shorthand();
        return statement.shorthand.get();
    }

    boost::optional< const Shorthand& > getShorty() const
    {
        boost::optional< const Shorthand& > result;
        if( statement.shorthand )
            result = statement.shorthand.get();
        return result;
    }

    Statement statement;
    Vector children;
};
ParseResult parse( const std::string& strInput, Node& ed, std::ostream& errorStream );
ParseResult parse( const std::string& strInput, std::vector< Node >& ed, std::ostream& errorStream );
bool operator==( const Node& left, const Node& right );
inline const Statement& getStatement( const Node& node )            { return node.statement; }
inline Node::Vector::const_iterator begin( const Node& node )       { return node.children.begin(); }
inline Node::Vector::const_iterator end( const Node& node )         { return node.children.end(); }
inline bool isEmpty( const Node& node )                             { return node.children.empty(); }

inline Node make_node( const std::string& strIdentifier ) 
{
    return Node( Statement( Declarator( Identifier( strIdentifier ) ) ) );
}

}
 
BOOST_FUSION_ADAPT_STRUCT(
    Ed::Node,
    (Ed::Statement, statement)
    (std::vector<Ed::Node>, children)
)

namespace Ed
{
    
static const char BODY_OPEN = '{';
static const char BODY_CLOSE = '}';

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//Ed
template< typename Iterator >
class EDGrammar : public boost::spirit::qi::grammar< Iterator, SkipGrammar< Iterator >, std::vector< Node >() >
{
public:
    EDGrammar() : EDGrammar::base_type( m_main_rule, "ed" )
    {
        using namespace boost::spirit;
        using namespace boost::spirit::qi;
        using namespace boost::phoenix;

        m_rule_block =    m_grammarStatement[ at_c<0>(_val) = qi::_1 ] > 
                        -(
                            char_( BODY_OPEN ) >> 
                            *m_rule_block[ push_back( at_c<1>(_val), qi::_1 ) ]  >
                            char_( BODY_CLOSE )
                        );

        m_main_rule = *m_rule_block[ push_back( _val, qi::_1 ) ];

        m_rule_block.name( "block" );
    }
    
    StatementGrammar< Iterator >                                                        m_grammarStatement;
    boost::spirit::qi::rule< Iterator, SkipGrammar< Iterator >, Node() >                m_rule_block;
    boost::spirit::qi::rule< Iterator, SkipGrammar< Iterator >, std::vector< Node >() > m_main_rule;

};


}

#endif //ED_25_12_2013