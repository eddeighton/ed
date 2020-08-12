
#include "ed/declarator.hpp"
#include "ed/serialise.hpp"

#include <gtest/gtest.h>

#include <sstream>
#include <string>

template< class T >
std::ostream& operator<<( std::ostream& os, const boost::optional< T >& opt )
{
    return opt ? ( os << opt.get() ) : os;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//Declarator Tests

TEST( EdGrammarTests, Declarator_1 )
{
    Ed::Declarator declarator;
    std::string strInput( " nodeName_1 SystemName_1" );
    const Ed::ParseResult result = Ed::parse( strInput, declarator );
    Ed::TypeList resultTypeList = Ed::TypeList( Ed::Type( "SystemName_1" ) );
    ASSERT_EQ( Ed::Identifier( "nodeName_1" ), declarator.identifier.get() );
    ASSERT_EQ( resultTypeList, declarator.typeList );
}

TEST( EdGrammarTests, Declarator_2 )
{
    Ed::Declarator declarator;
    std::string strInput( " nodeName_1" );
    const Ed::ParseResult result = Ed::parse( strInput, declarator );
    Ed::TypeList resultTypeList;
    ASSERT_EQ( Ed::Identifier( "nodeName_1" ), declarator.identifier.get() );
    ASSERT_EQ( resultTypeList, declarator.typeList );
}

TEST( EdGrammarTests, Declarator_3 )
{
    Ed::Declarator declarator;
    std::string strInput( "  SystemName_1 " );
    const Ed::ParseResult result = Ed::parse( strInput, declarator );
    Ed::TypeList resultTypeList = Ed::TypeList( Ed::Type( "SystemName_1" ) );
    ASSERT_EQ( boost::optional< Ed::Identifier >(), declarator.identifier );
    ASSERT_EQ( resultTypeList, declarator.typeList );
}

TEST( EdGrammarTests, Declarator_4 )
{
    Ed::Declarator declarator;
    std::string strInput( " nodeName_1 SystemName_1::Two::Three" );
    const Ed::ParseResult result = Ed::parse( strInput, declarator );
    Ed::TypeList resultTypeList = Ed::TypeList( Ed::Type( "SystemName_1" ), Ed::Type( "Two" ), Ed::Type( "Three" ) );
    ASSERT_EQ( Ed::Identifier( "nodeName_1" ), declarator.identifier.get() );
    ASSERT_EQ( resultTypeList, declarator.typeList );
}
