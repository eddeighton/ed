
#include "ed/code.h"
#include "ed/serialise.h"

#include <gtest/gtest.h>

#include <string>
#include <sstream>

TEST( EdGrammarTests, Code_1 )
{
    Ed::Code code;
    std::string strInput( "{{}{{{}}{}}}" );
    const Ed::ParseResult result = Ed::parse( strInput, code );
    ASSERT_EQ( code.code, std::string( "{}{{{}}{}}" ) );
    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
}

TEST( EdGrammarTests, Code_Succeed2 )
{
    Ed::Code code;
    std::string strInput( "{ int foo(){ int dostuff = 0; return dostuff; } }" );
    const Ed::ParseResult result = Ed::parse( strInput, code );
    ASSERT_EQ( code.code, std::string( " int foo(){ int dostuff = 0; return dostuff; } " ) );
    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
}

TEST( EdGrammarTests, Code_Extract1 )
{
    Ed::Code code;
    std::string strInput( "{ int x = ..a.b.c:.d.e.f; return x / 2.0f; }" );
    const Ed::ParseResult result = Ed::parse( strInput, code );
    ASSERT_EQ( code.code, std::string( " int x = ..a.b.c:.d.e.f; return x / 2.0f; " ) );
    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );

    Ed::CodeReferenceVector references;
    Ed::extractCodeReferences( code.code, references );
    ASSERT_EQ( 1u, references.size() );
}

TEST( EdGrammarTests, Code_Fail1 )
{
    Ed::Code code;
    ASSERT_FALSE( Ed::parse( "{{{{}}}", code ).first );
}

TEST( EdGrammarTests, Code_Fail2 )
{
    Ed::Code code;
    ASSERT_FALSE( Ed::parse( "", code ).first );
}

TEST( EdGrammarTests, Code_Fail3 )
{
    Ed::Code code;
    ASSERT_FALSE( Ed::parse( "}{", code ).first );
}

TEST( EdGrammarTests, Code_Stress1 )
{
    std::ostringstream os1, os2;
    unsigned int uiCount = 0u;
    for( int i = 0; i < 10000; ++i )
    {
        if( (rand() % 2) && (uiCount > 0u) )
        {
            os2 << "}";
            --uiCount;
        }
        else
        {
            os2 << "{";
            ++uiCount;
        }
    }
    while( uiCount ){ --uiCount; os2 << "}"; }
    os1 << '{' << os2.str() << '}';

    Ed::Code code;
    std::string strInput( os1.str() );
    const Ed::ParseResult result = Ed::parse( strInput, code );
    ASSERT_EQ( code.code, os2.str() );
    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
}
