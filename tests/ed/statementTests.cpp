

#include "ed/statement.h"
#include "ed/serialise.h"

#include <gtest/gtest.h>

#include <string>
#include <sstream>

std::string toStr( const Ed::Statement& statement )
{
    std::ostringstream os;
    os << statement;
    return os.str();
}

TEST( EdGrammarTests, Statement_1 )
{
    const std::string strInput = "ident Type";
    Ed::Statement expected(
        Ed::Declarator( Ed::Identifier( "ident" ), Ed::TypeList( Ed::Type( "Type" ) ) ) );

    Ed::Statement result;
    Ed::parse( strInput, result );

    const std::string strExpected = toStr( expected );
    const std::string strResult = toStr( result );

    ASSERT_EQ( expected, result );
    ASSERT_EQ( strExpected, strResult );
}
TEST( EdGrammarTests, Statement_2 )
{
    const std::string strInput = "ident<tag>";
    Ed::Statement expected(
        Ed::Declarator( Ed::Identifier( "ident" ) ),
        Ed::TagList( Ed::Identifier( "tag" ) ) );

    Ed::Statement result;
    const Ed::ParseResult parseResult = Ed::parse( strInput, result );

    const std::string strExpected = toStr( expected );
    const std::string strResult = toStr( result );
    const std::string strRemain = std::string( parseResult.second.base(), strInput.end() );

    ASSERT_EQ( expected, result );
}
TEST( EdGrammarTests, Statement_3 )
{
    const std::string strInput = "ident( 1.0f; )";
    Ed::Statement expected(
        Ed::Declarator( Ed::Identifier( "ident" ) ),
        Ed::Shorthand( Ed::Expression( "1.0f" ) ) );

    Ed::Statement result;
    const Ed::ParseResult parseResult = Ed::parse( strInput, result );

    const std::string strExpected = toStr( expected );
    const std::string strResult = toStr( result );
    const std::string strRemain = std::string( parseResult.second.base(), strInput.end() );

    ASSERT_EQ( expected, result );
}
TEST( EdGrammarTests, Statement_4 )
{
    const std::string strInput = "ident<tag>( 1.0f; )";
    Ed::Statement expected(
        Ed::Declarator( Ed::Identifier( "ident" ) ),
        Ed::TagList( Ed::Identifier( "tag" ) ),
        Ed::Shorthand( Ed::Expression( "1.0f" ) ) );

    Ed::Statement result;
    const Ed::ParseResult parseResult = Ed::parse( strInput, result );

    const std::string strExpected = toStr( expected );
    const std::string strResult = toStr( result );
    const std::string strRemain = std::string( parseResult.second.base(), strInput.end() );

    ASSERT_EQ( expected, result );
}

TEST( EdGrammarTests, Statement_5 )
{
    const std::string strInput = "Type<tag>";
    Ed::Statement expected(
        Ed::Declarator( Ed::TypeList( Ed::Type( "Type" ) ) ),
        Ed::TagList( Ed::Identifier( "tag" ) ) );

    Ed::Statement result;
    const Ed::ParseResult parseResult = Ed::parse( strInput, result );

    const std::string strExpected = toStr( expected );
    const std::string strResult = toStr( result );
    const std::string strRemain = std::string( parseResult.second.base(), strInput.end() );

    ASSERT_EQ( expected, result );
}
TEST( EdGrammarTests, Statement_6 )
{
    const std::string strInput = "Type( 1.0f; )";
    Ed::Statement expected(
        Ed::Declarator( Ed::TypeList( Ed::Type( "Type" ) ) ),
        Ed::Shorthand( Ed::Expression( "1.0f" ) ) );

    Ed::Statement result;
    const Ed::ParseResult parseResult = Ed::parse( strInput, result );

    const std::string strExpected = toStr( expected );
    const std::string strResult = toStr( result );
    const std::string strRemain = std::string( parseResult.second.base(), strInput.end() );

    ASSERT_EQ( expected, result );
}
TEST( EdGrammarTests, Statement_7 )
{
    const std::string strInput = "Type<tag>( 1.0f; )";
    Ed::Statement expected(
        Ed::Declarator( Ed::TypeList( Ed::Type( "Type" ) ) ),
        Ed::TagList( Ed::Identifier( "tag" ) ),
        Ed::Shorthand( Ed::Expression( "1.0f" ) ) );

    Ed::Statement result;
    const Ed::ParseResult parseResult = Ed::parse( strInput, result );

    const std::string strExpected = toStr( expected );
    const std::string strResult = toStr( result );
    const std::string strRemain = std::string( parseResult.second.base(), strInput.end() );

    ASSERT_EQ( expected, result );
}

TEST( EdGrammarTests, Statement_8 )
{
    const std::string strInput = "ident Type<tag>";
    Ed::Statement expected(
        Ed::Declarator( Ed::Identifier( "ident" ), Ed::TypeList( Ed::Type( "Type" ) ) ),
        Ed::TagList( Ed::Identifier( "tag" ) ) );

    Ed::Statement result;
    const Ed::ParseResult parseResult = Ed::parse( strInput, result );

    const std::string strExpected = toStr( expected );
    const std::string strResult = toStr( result );
    const std::string strRemain = std::string( parseResult.second.base(), strInput.end() );

    ASSERT_EQ( expected, result );
}
TEST( EdGrammarTests, Statement_9 )
{
    const std::string strInput = "ident Type( 1.0f; )";
    Ed::Statement expected(
        Ed::Declarator( Ed::Identifier( "ident" ), Ed::TypeList( Ed::Type( "Type" ) ) ),
        Ed::Shorthand( Ed::Expression( "1.0f" ) ) );

    Ed::Statement result;
    const Ed::ParseResult parseResult = Ed::parse( strInput, result );

    const std::string strExpected = toStr( expected );
    const std::string strResult = toStr( result );
    const std::string strRemain = std::string( parseResult.second.base(), strInput.end() );

    ASSERT_EQ( expected, result );
}
TEST( EdGrammarTests, Statement_10 )
{
    const std::string strInput = "ident Type<tag>( 1.0f; )";
    Ed::Statement expected(
        Ed::Declarator( Ed::Identifier( "ident" ), Ed::TypeList( Ed::Type( "Type" ) ) ),
        Ed::TagList( Ed::Identifier( "tag" ) ),
        Ed::Shorthand( Ed::Expression( "1.0f" ) ) );

    Ed::Statement result;
    const Ed::ParseResult parseResult = Ed::parse( strInput, result );

    const std::string strExpected = toStr( expected );
    const std::string strResult = toStr( result );
    const std::string strRemain = std::string( parseResult.second.base(), strInput.end() );

    ASSERT_EQ( expected, result );
}