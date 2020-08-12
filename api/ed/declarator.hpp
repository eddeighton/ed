#ifndef DECLARATOR_25_12_2013
#define DECLARATOR_25_12_2013

#include "identifier.hpp"
#include "type.hpp"
#include "skip.hpp"
#include "parser.hpp"

#include <boost/spirit/include/qi.hpp>

#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

#include <boost/variant.hpp>
#include <boost/optional.hpp>

#include <string>

namespace Ed
{
    
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//Declarator
class Declarator
{
public:
    Declarator()
    {}
    Declarator( const Identifier& _identifier )
        :   identifier( _identifier )
    {}
    Declarator( const TypeList& _typeList )
        :   typeList( _typeList )
    {}
    Declarator( const Identifier& _identifier, const TypeList& _typeList )
        :   identifier( _identifier ),
            typeList( _typeList )
    {}

    boost::optional< Identifier >   identifier;
    TypeList typeList;
};

}

BOOST_FUSION_ADAPT_STRUCT(
    Ed::Declarator,
    (boost::optional< Ed::Identifier >, identifier)
    (Ed::TypeList, typeList)
)

namespace Ed
{

template< typename Iterator >
struct DeclaratorGrammar : public boost::spirit::qi::grammar< Iterator, SkipGrammar< Iterator >, Declarator() >
{
    DeclaratorGrammar() : DeclaratorGrammar::base_type( m_main_rule, "declarator" )
    {        
        using namespace boost::spirit;
        using namespace boost::spirit::qi;
        using namespace boost::phoenix;

        m_main_rule = m_grammar_identifier[ at_c<0>(_val) = qi::_1 ] || m_grammar_typeList[ at_c<1>(_val) = qi::_1 ];
    }

    IdentifierGrammar< Iterator >                                                   m_grammar_identifier;
    TypeListGrammar< Iterator >                                                     m_grammar_typeList;
    boost::spirit::qi::rule< Iterator, SkipGrammar< Iterator >, Declarator() >      m_main_rule;
};

ParseResult parse( const std::string& strInput, Declarator& declarator, std::ostream& errorStream );

bool operator==( const Declarator& left, const Declarator& right );

}

#endif //DECLARATOR_25_12_2013