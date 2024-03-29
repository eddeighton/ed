

#include "ed/tag.hpp"
#include "ed/serialise.hpp"
#include "ed/serialise.hpp"

#include <gtest/gtest.h>

#include <string>
#include <sstream>

TEST( EdGrammarTests, Tag_1 )
{
    Ed::Tag               tag;
    const std::string     strInput = "always";
    const Ed::Tag         expected = Ed::Identifier( strInput );
    const Ed::ParseResult result   = Ed::parse( "always", tag );
    ASSERT_TRUE( result.first );
    ASSERT_EQ( expected, tag );
}

TEST( EdGrammarTests, TagList_1 )
{
    const std::string     strInput = "<one,two>";
    const Ed::TagList     expected = Ed::TagList( Ed::Identifier( "one" ), Ed::Identifier( "two" ) );
    Ed::TagList           resultTagList;
    const Ed::ParseResult result = Ed::parse( strInput, resultTagList );
    ASSERT_TRUE( result.first );
    ASSERT_EQ( expected, resultTagList );
}
