

#include "ed/type.hpp"
#include "ed/serialise.hpp"

#include <gtest/gtest.h>

#include <string>
#include <sstream>

TEST( EdGrammarTests, Type_1 )
{
    const std::string strInput = "Abc";
    const Ed::Type strExpected = strInput;
    Ed::Type type;
    const Ed::ParseResult result = Ed::parse( strInput, type );
    ASSERT_EQ( strExpected, type );
    ASSERT_EQ( result.second.base(), strInput.end() );
}

TEST( EdGrammarTests, Type_2 )
{
    const std::string strInput = "A";
    const Ed::Type strExpected = strInput;
    Ed::Type type;
    const Ed::ParseResult result = Ed::parse( strInput, type );
    ASSERT_EQ( strExpected, type );
    ASSERT_EQ( result.second.base(), strInput.end() );
}

TEST( EdGrammarTests, Type_Fail_1 )
{
    Ed::Type type;
    std::string strInput( "n" );
    const Ed::ParseResult result = Ed::parse( strInput, type );
    ASSERT_FALSE( result.first );
}

TEST( EdGrammarTests, Type_Fail_2 )
{
    Ed::Type type;
    std::string strInput( "1" );
    const Ed::ParseResult result = Ed::parse( strInput, type );
    ASSERT_FALSE( result.first );
}

TEST( EdGrammarTests, Type_Fail_3 )
{
    Ed::Type type;
    std::string strInput( "_" );
    const Ed::ParseResult result = Ed::parse( strInput, type );
    ASSERT_FALSE( result.first );
}

TEST( EdGrammarTests, Type_Fail_4 )
{
    Ed::Type type;
    std::string strInput( "" );
    const Ed::ParseResult result = Ed::parse( strInput, type );
    ASSERT_FALSE( result.first );
}

TEST( EdGrammarTests, TypeList_1 )
{
    const std::string strInput = "A";
    Ed::TypeList typeList, expected;
    expected.push_back( Ed::Type( "A" ) );
    const Ed::ParseResult result = Ed::parse( strInput, typeList );
    ASSERT_EQ( result.second.base(), strInput.end() );
    ASSERT_EQ( expected, typeList );
}
TEST( EdGrammarTests, TypeList_2 )
{
    const std::string strInput = "A::B::C::D";
    Ed::TypeList typeList, expected;
    expected.push_back( Ed::Type( "A" ) );
    expected.push_back( Ed::Type( "B" ) );
    expected.push_back( Ed::Type( "C" ) );
    expected.push_back( Ed::Type( "D" ) );
    const Ed::ParseResult result = Ed::parse( strInput, typeList );
    ASSERT_EQ( result.second.base(), strInput.end() );
    ASSERT_EQ( expected, typeList );
}

