

#include "meta/parser.h"

#include <gtest/gtest.h>

#include <string>
#include <sstream>


TEST( MetaParserTests, MetacodeCodeParser_1 )
{
    meta::MetaCode m;
    std::ostringstream os;
    std::string strInput = "This<> is#<># a simple test<#";
    Ed::ParseResult result = meta::parse( strInput, m, os );
    ASSERT_EQ( m, std::string( "This<> is#<># a simple test" ) );
    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end()-2 );
}

TEST( MetaParserTests, MetacodeCodeParser_2 )
{
    meta::MetaCode m;
    std::ostringstream os;
    std::string strInput = "This<> is#<># a simple test#>";
    Ed::ParseResult result = meta::parse( strInput, m, os );
    ASSERT_EQ( m, std::string( "This<> is#<># a simple test" ) );
    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end()-2);
}

TEST( MetaParserTests, MetacodeCodeParser_3 )
{
    meta::MetaCode m;
    std::ostringstream os;
    std::string strInput = "This<> is#<># a simple test";
    Ed::ParseResult result = meta::parse( strInput, m, os );
    ASSERT_EQ( m, std::string( "This<> is#<># a simple test" ) );
    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
}

TEST( MetaParserTests, MetaParser_1 )
{
    meta::Node::Vector nodes;
    std::string strInput = "Literal";
    std::ostringstream os;
    Ed::ParseResult result = meta::parse( strInput, nodes, os );
    ASSERT_EQ( 1, nodes.size() );

}


TEST( MetaParserTests, MetaParser_2 )
{
    meta::Node::Vector nodes;
    std::string strInput = "Literal<#meta code here...#>More Literal<#more meta<#nested literal#>#><#chained meta#>Ending literal";
    std::ostringstream os;
    Ed::ParseResult result = meta::parse( strInput, nodes, os );
    ASSERT_EQ( 6, nodes.size() );

    ASSERT_EQ( "Literal", nodes[0].get().get() );
    ASSERT_EQ( "meta code here...", nodes[1].nodes[0].get().get() );
    ASSERT_EQ( "More Literal", nodes[2].get().get() );
    ASSERT_EQ( "more meta", nodes[3].nodes[0].get().get() );
    ASSERT_EQ( "nested literal", nodes[3].nodes[1].nodes[0].get().get() );
    ASSERT_EQ( "chained meta", nodes[4].nodes[0].get().get() );
    ASSERT_EQ( "Ending literal", nodes[5].get().get() );

    {
        std::ostringstream os;
        os << nodes;
        ASSERT_EQ( strInput, os.str() );
    }
}

TEST( MetaParserTests, MetaParser_3 )
{
    meta::Node::Vector nodes;
    std::string strInput = "a<#b<#c#>d<#e#>f#>g";
    std::ostringstream os;
    Ed::ParseResult result = meta::parse( strInput, nodes, os );

    ASSERT_EQ( "a", nodes[0].get().get() );
    ASSERT_EQ( "b", nodes[1].nodes[0].get().get() );
    ASSERT_EQ( "c", nodes[1].nodes[1].nodes[0].get().get() );
    ASSERT_EQ( "d", nodes[1].nodes[2].get().get() );
    ASSERT_EQ( "e", nodes[1].nodes[3].nodes[0].get().get() );
    ASSERT_EQ( "f", nodes[1].nodes[4].get().get() );
    ASSERT_EQ( "g", nodes[2].get().get() );
    {
        std::ostringstream os;
        os << nodes;
        ASSERT_EQ( strInput, os.str() );
    }
}

TEST( MetaParserTests, MetaParser_4 )
{
    std::string strInput =
"\
    //This is a comment at the start of the code..\
    /* this is a c++ comment */\
    <# //this is meta code #>\
    //finally some more stuff\
";

    meta::Node::Vector nodes;
    std::ostringstream os;
    Ed::ParseResult result = meta::parse( strInput, nodes, os );
    {
        std::ostringstream os;
        os << nodes;
        ASSERT_EQ( strInput, os.str() );
    }

}

