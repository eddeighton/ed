#ifndef TYPE_25_12_2013
#define TYPE_25_12_2013

#include "skip.hpp"
#include "parser.hpp"

#include <boost/spirit/include/qi.hpp>

#include <boost/phoenix/core.hpp>
#include <boost/phoenix/stl.hpp>

#include <string>
#include <vector>

namespace Ed
{

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//Type
class Type : public std::string
{
public:
    Type(){}
    Type( const std::string& str ) : std::string( str ) {}
};

template< typename Iterator >
class TypeGrammar : public boost::spirit::qi::grammar< Iterator, SkipGrammar< Iterator >, Type() >
{
public:
    TypeGrammar() : TypeGrammar::base_type( m_main_rule, "namespace" )
    {
        using namespace boost::spirit;
        using namespace boost::spirit::qi;
        using namespace boost::phoenix;
        m_main_rule = lexeme[char_( "A-Z" )[ push_back( _val, qi::_1 ) ] >>
                        *(char_( "a-zA-Z0-9_" )[ push_back( _val, qi::_1 ) ])];
    }

    boost::spirit::qi::rule< Iterator, SkipGrammar< Iterator >, Type() >  m_main_rule;
};

ParseResult parse( const std::string& strInput, Type& type, std::ostream& errorStream );


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//TypeList
static const char TYPELIST_DELIMITER[] = "::";

class TypeList : public std::vector< Type >
{
public:
    TypeList(){}
    TypeList( const Type& type1 );
    TypeList( const Type& type1, const Type& type2 );
    TypeList( const Type& type1, const Type& type2, const Type& type3 );
};

template< typename Iterator >
class TypeListGrammar : public boost::spirit::qi::grammar< Iterator, SkipGrammar< Iterator >, TypeList() >
{
public:
    TypeListGrammar() : TypeListGrammar::base_type( m_main_rule, "typelist" )
    {
        using namespace boost::spirit;
        using namespace boost::spirit::qi;
        using namespace boost::phoenix;
        m_main_rule = m_type_grammar[ push_back( _val, qi::_1 ) ] >>
                                    *( TYPELIST_DELIMITER >> m_type_grammar[ push_back( _val, qi::_1 ) ] );
    }

    TypeGrammar< Iterator >                                                     m_type_grammar;
    boost::spirit::qi::rule< Iterator, SkipGrammar< Iterator >, TypeList() >    m_main_rule;
};

ParseResult parse( const std::string& strInput, TypeList& typeList, std::ostream& errorStream );


}

#endif //TYPE_25_12_2013
