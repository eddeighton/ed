
#include "ed/ref.h"
#include "ed/serialise.h"

#include <gtest/gtest.h>

#include <string>
#include <sstream>


TEST( EdGrammarTests, Reference_Succeed1 )
{
    Ed::Ref ref;
    std::string strInput( ".a.b.c.d" );
    const Ed::ParseResult result = Ed::parse( strInput, ref );

    std::ostringstream os;
    os << ref;
    ASSERT_EQ( std::string( ".a.b.c.d" ), os.str() );
    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
}

TEST( EdGrammarTests, Reference_Succeed2 )
{
    Ed::Ref ref;
    std::string strInput( "....a.b.c" );
    const Ed::ParseResult result = Ed::parse( strInput, ref );

    std::ostringstream os;
    os << ref;
    ASSERT_EQ( std::string( "....a.b.c" ), os.str() );
    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
}

TEST( EdGrammarTests, Reference_Succeed3 )
{
    Ed::Ref ref;
    std::string strInput( " . . . . a . b . c " );
    const Ed::ParseResult result = Ed::parse( strInput, ref );

    std::ostringstream os;
    os << ref;
    ASSERT_EQ( std::string( "....a.b.c" ), os.str() );
    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
}

TEST( EdGrammarTests, Reference_Succeed4 )
{
    Ed::Ref ref;
    std::string strInput( ".a.b.c.d:.a.b.c.d:....a.b.c" );
    const Ed::ParseResult result = Ed::parse( strInput, ref );

    std::ostringstream os;
    os << ref;
    std::string sResult = os.str();
    ASSERT_EQ( std::string( ".a.b.c.d:.a.b.c.d:....a.b.c" ), sResult );
    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
}

TEST( EdGrammarTests, Reference_Succeed5 )
{
    Ed::Ref ref;
    std::string strInput( ".a.b.c.d:....a.b.c:....a.b.c" );
    const Ed::ParseResult result = Ed::parse( strInput, ref );

    std::ostringstream os;
    os << ref;
    std::string sResult = os.str();
    ASSERT_EQ( std::string( ".a.b.c.d:....a.b.c:....a.b.c" ), sResult );
    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
}

TEST( EdGrammarTests, Reference_Succeed6 )
{
    Ed::Ref ref;
    std::string strInput( "." );
    const Ed::ParseResult result = Ed::parse( strInput, ref );

    std::ostringstream os;
    os << ref;
    std::string sResult = os.str();
    ASSERT_EQ( std::string( "." ), sResult );
    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
}
TEST( EdGrammarTests, Reference_Succeed7 )
{
    Ed::Ref ref;
    std::string strInput( ".." );
    const Ed::ParseResult result = Ed::parse( strInput, ref );

    std::ostringstream os;
    os << ref;
    std::string sResult = os.str();
    ASSERT_EQ( std::string( ".." ), sResult );
    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
}
TEST( EdGrammarTests, Reference_Succeed8 )
{
    Ed::Ref ref;
    std::string strInput( "..." );
    const Ed::ParseResult result = Ed::parse( strInput, ref );

    std::ostringstream os;
    os << ref;
    std::string sResult = os.str();
    ASSERT_EQ( std::string( "..." ), sResult );
    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
}

TEST( EdGrammarTests, Reference_Branches_1 )
{
    Ed::Ref ref;
    std::string strInput( "..[a,b].[a,c].a.b.c" );
    const Ed::ParseResult result = Ed::parse( strInput, ref );
    std::ostringstream os;
    os << ref;
    std::string sResult = os.str();
    ASSERT_EQ( std::string( "..[a,b].[a,c].a.b.c" ), sResult );
    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
    ASSERT_EQ( 6, ref[0].size() );
    {
        const Ed::Ref& r = boost::apply_visitor( TypeAccessor< Ed::Ref >(), ref[0][1] ).get();
        std::ostringstream os;
        os << r[1];
        ASSERT_EQ( std::string( ".b" ), os.str() );
    }
    {
        const Ed::Ref& r = boost::apply_visitor( TypeAccessor< Ed::Ref >(), ref[0][2] ).get();
        std::ostringstream os;
        os << r[1];
        ASSERT_EQ( std::string( ".c" ), os.str() );
    }
}

TEST( EdGrammarTests, Reference_Branches_2 )
{
    Ed::Ref ref;
    std::string strInput(   "..[a,[a.b.c.d,e.f.g.h,i.j.k.l],b.c.d.e].[a,c].a.b.c" );
    const Ed::ParseResult result = Ed::parse( strInput, ref );
    std::ostringstream os;
    os << ref;
    std::string sResult = os.str();
    ASSERT_EQ( std::string( "..[a,[a.b.c.d,e.f.g.h,i.j.k.l],b.c.d.e].[a,c].a.b.c" ), sResult );
    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
}

TEST( EdGrammarTests, Reference_Branches_3 )
{
    Ed::Ref ref;
    std::string strInput(   "..[a,[a.b.c.d,e.f.g.[a,[a.b.c.d,e.f.g.h,i.j.k.l],b.c.d.e],i.j.k.l],b.c.d.e].[a,[a,[a.b.c.d,e.f.g.h,i.j.k.l],b.c.d.e]].a.b.c" );
    const Ed::ParseResult result = Ed::parse( strInput, ref );
    std::ostringstream os;
    os << ref;
    std::string sResult = os.str();
    ASSERT_EQ( std::string( "..[a,[a.b.c.d,e.f.g.[a,[a.b.c.d,e.f.g.h,i.j.k.l],b.c.d.e],i.j.k.l],b.c.d.e].[a,[a,[a.b.c.d,e.f.g.h,i.j.k.l],b.c.d.e]].a.b.c" ), sResult );
    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
}

TEST( EdGrammarTests, Reference_Fail1 )
{
    Ed::Ref ref;
    ASSERT_FALSE( Ed::parse( "", ref ).first );
}
TEST( EdGrammarTests, Reference_Fail5 )
{
    Ed::Ref ref;
    std::string s(".a.b.");
    ASSERT_NE( Ed::parse( s, ref ).second.base(), s.end() );
}
TEST( EdGrammarTests, Reference_Fail6 )
{
    Ed::Ref ref;
    ASSERT_FALSE( Ed::parse( "a.b", ref ).first );
}
TEST( EdGrammarTests, Reference_Fail7 )
{
    Ed::Ref ref;
    std::string s(".a..b");
    ASSERT_NE( Ed::parse( s, ref ).second.base(), s.end() );
}
TEST( EdGrammarTests, Reference_Fail8 )
{
    Ed::Ref ref;
    std::string s(".a.b.c:");
    ASSERT_NE( Ed::parse( s, ref ).second.base(), s.end() );
}
TEST( EdGrammarTests, Reference_Fail9 )
{
    Ed::Ref ref;
    std::string s(":.a.b.c");
    ASSERT_NE( Ed::parse( s, ref ).second.base(), s.end() );
}
