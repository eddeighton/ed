

#include "ed/identifier.hpp"
#include "ed/serialise.hpp"

#include <gtest/gtest.h>

#include <string>
#include <sstream>

TEST( EdGrammarTests, Identifier_1 )
{
    const std::string strInput =
            "abcdefghijklmnopqrstuvwxyzABCDEGHIJKLMNOPQRSTUVWXYZ";
    const std::string strExpected = strInput;
    Ed::Identifier identifier = Ed::parse< Ed::Identifier >( strInput );
    ASSERT_EQ( strExpected, identifier );
}

TEST( EdGrammarTests, Identifier_2 )
{
    Ed::Identifier identifier;
    std::string strInput( "n_ABC_abc_123" );
    const Ed::ParseResult result = Ed::parse( strInput, identifier );
    ASSERT_EQ( identifier, std::string( "n_ABC_abc_123" ) );
    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
}

TEST( EdGrammarTests, IdentifierGrammar_Fail1 )
{
    Ed::Identifier identifier;
    std::string strInput( "N_ABC_abc_123" );
    const Ed::ParseResult result = Ed::parse( strInput, identifier );
    ASSERT_FALSE( result.first );
}

TEST( EdGrammarTests, IdentifierGrammar_Fail2 )
{
    Ed::Identifier identifier;
    std::string strInput( " N_ABC_abc_123" );
    const Ed::ParseResult result = Ed::parse( strInput, identifier );
    ASSERT_FALSE( result.first );
}

TEST( EdGrammarTests, IdentifierGrammar_Fail3 )
{
    Ed::Identifier identifier;
    std::string strInput( "1" );
    const Ed::ParseResult result = Ed::parse( strInput, identifier );
    ASSERT_FALSE( result.first );
}

TEST( EdGrammarTests, IdentifierGrammar_Fail4 )
{
    Ed::Identifier identifier;
    std::string strInput( "" );
    const Ed::ParseResult result = Ed::parse( strInput, identifier );
    ASSERT_FALSE( result.first );
}