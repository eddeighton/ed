
#include "ed/file.hpp"
#include "ed/serialise.hpp"

#include <gtest/gtest.h>
#include <string>
#include <sstream>

#include <boost/assign.hpp>

TEST( EdGrammarTests, FileRef_Succeed1 )
{
    Ed::FileRef fileRef;
    std::string strInput( "/a/b/c/d/e" );
    const Ed::ParseResult result = Ed::parse( strInput, fileRef );

    Ed::FileRef expected;
    {
        using namespace boost::assign;
        expected += "a","b","c","d","e";
    }

    ASSERT_EQ( expected, fileRef );
    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
}

TEST( EdGrammarTests, FileRef_Succeed2 )
{
    Ed::FileRef fileRef;
    std::string strInput( "../../../a/b/c/1/2/3/_/B/C" );
    const Ed::ParseResult result = Ed::parse( strInput, fileRef );

    Ed::FileRef expected;
    {
        using namespace boost::assign;
        expected += Ed::eFileUp,Ed::eFileUp,Ed::eFileUp,"a","b","c","1","2","3","_","B","C";
    };

    ASSERT_EQ( expected, fileRef );
    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
}

TEST( EdGrammarTests, FileRef_Fail1 )
{
    Ed::FileRef fileRef;
    ASSERT_FALSE( Ed::parse( "", fileRef ).first );
}

TEST( EdGrammarTests, FileRef_Fail2 )
{
    Ed::FileRef fileRef;
    ASSERT_FALSE( Ed::parse( "/", fileRef ).first );
}

TEST( EdGrammarTests, FileRef_Fail3 )
{
    Ed::FileRef fileRef;
    ASSERT_FALSE( Ed::parse( "..", fileRef ).first );
}

TEST( EdGrammarTests, FileRef_Fail4 )
{
    Ed::FileRef fileRef;
    ASSERT_FALSE( Ed::parse( "abc", fileRef ).first );
}

TEST( EdGrammarTests, FileRef_Fail5 )
{
    Ed::FileRef fileRef;
    std::string strInput( "/abc//" );
    const Ed::ParseResult result = Ed::parse( strInput, fileRef );
    ASSERT_NE( result.second.base(), strInput.end() );
}

TEST( EdGrammarTests, FileRef_Fail6 )
{
    Ed::FileRef fileRef;
    ASSERT_FALSE( Ed::parse( "//abc", fileRef ).first );
}

TEST( EdGrammarTests, FileRef_Fail7 )
{
    Ed::FileRef fileRef;
    std::string strInput( "../abc/../abc" );
    const Ed::ParseResult result = Ed::parse( strInput, fileRef );
    ASSERT_NE( result.second.base(), strInput.end() );
}

TEST( EdGrammarTests, FileRef_Fail8 )
{
    Ed::FileRef fileRef;
    std::string strInput( "/abc/" );
    const Ed::ParseResult result = Ed::parse( strInput, fileRef );
    ASSERT_NE( result.second.base(), strInput.end() );
}
