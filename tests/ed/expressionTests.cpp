
#include "ed/expression.h"
#include "ed/serialise.h"

#include <gtest/gtest.h>

#include <string>
#include <sstream>

TEST( EdGrammarTests, Expression_Succeed1 )
{
    Ed::Expression exp;
    std::string strInput( " 2.0f * ..x / ( sin( 123.0f ) - ..y);" );
    const Ed::ParseResult result = Ed::parse( strInput, exp );
    ASSERT_EQ( std::string( "2.0f * ..x / ( sin( 123.0f ) - ..y)"), exp );
    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
}

TEST( EdGrammarTests, Expression_Succeed2 )
{
    Ed::Expression exp;
    std::string strInput( " ..x / ( sin( 123.0f ) - ..y);" );
    const Ed::ParseResult result = Ed::parse( strInput, exp );
    ASSERT_EQ( std::string( "..x / ( sin( 123.0f ) - ..y)"), exp );
    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
}

TEST( EdGrammarTests, Expression_Succeed3 )
{
    Ed::Expression exp;
    std::string strInput( " ../../e/e/e ...e.e.e.e 1.0 ,2.0,3.0,4.0 true, false ;" );
    const Ed::ParseResult result = Ed::parse( strInput, exp );
    ASSERT_EQ( std::string( "../../e/e/e ...e.e.e.e 1.0 ,2.0,3.0,4.0 true, false "), exp );
    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
}

TEST( EdGrammarTests, Expression_Fail1 )
{
    ASSERT_THROW( Ed::parse< Ed::Expression >( " ..x / ( sin( 123.0f ) - ..y;" ), std::runtime_error );
}

TEST( EdGrammarTests, Expression_Fail2 )
{
    ASSERT_THROW( Ed::parse< Ed::Expression >( " ..x / ( sin( 123.0f ) - ..y)" ), std::runtime_error );
}

TEST( EdGrammarTests, Expression_Fail3 )
{
    ASSERT_THROW( Ed::parse< Ed::Expression >( "a)(;" ), std::runtime_error );
}

TEST( EdGrammarTests, Expression_Fail4 )
{
    ASSERT_THROW( Ed::parse< Ed::Expression >( "thing.SomeFunction( 123; );" ), std::runtime_error );
}