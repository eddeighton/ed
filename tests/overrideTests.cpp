
#include "ed/file.hpp"
#include "ed/statement.hpp"
#include "ed/shorthand.hpp"
#include "ed/override.hpp"
#include "ed/serialise.hpp"

#include <gtest/gtest.h>

#include <boost/optional/optional_io.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//Inheritance Tests

template< class T >
std::ostream& operator<<( std::ostream& os, const boost::optional< T >& opt )
{
    return opt ? ( os << opt.get() ) : os;
}

TEST( EdGrammarTests, Override_Declarator_1 )
{
    Ed::Declarator overrider   = Ed::parse< Ed::Declarator >( "test1" );
    Ed::Declarator origin     = Ed::parse< Ed::Declarator >( "test1 Test2" );
    Ed::Declarator expected = Ed::parse< Ed::Declarator >( "test1" );
    Ed::overrideAndExtend( origin, overrider );
    ASSERT_EQ( expected, overrider );
}

TEST( EdGrammarTests, Override_Declarator_2 )
{
    Ed::Declarator overrider   = Ed::parse< Ed::Declarator >( "test1" );
    Ed::Declarator origin     = Ed::parse< Ed::Declarator >( "test1 Test2" );
    Ed::Declarator expected = Ed::parse< Ed::Declarator >( "test1" );
    Ed::overrideAndExtend( origin, overrider );
    ASSERT_EQ( expected, overrider );
}

TEST( EdGrammarTests, Override_Declarator_3 )
{
    Ed::Declarator overrider    = Ed::parse< Ed::Declarator >( "test3 Test4" );
    Ed::Declarator origin       = Ed::parse< Ed::Declarator >( "test1 Test2" );
    Ed::Declarator expected     = Ed::parse< Ed::Declarator >( "test3 Test4" );
    Ed::overrideAndExtend( origin, overrider );
    ASSERT_EQ( expected, overrider );
}

TEST( EdGrammarTests, Override_Declarator_4 )
{
    Ed::Declarator overrider    = Ed::parse< Ed::Declarator >( "Test4" );
    Ed::Declarator origin       = Ed::parse< Ed::Declarator >( "test1 Test2" );
    Ed::Declarator expected     = Ed::parse< Ed::Declarator >( "test1 Test4" );
    Ed::overrideAndExtend( origin, overrider );
    ASSERT_EQ( expected, overrider );
}

TEST( EdGrammarTests, Override_Shorthand1 )
{
    boost::optional< Ed::Shorthand > overrider =   Ed::parse< Ed::Shorthand >( "(1,2,3)" );
    boost::optional< Ed::Shorthand > origin =       Ed::parse< Ed::Shorthand >( "(4,5,6,7)" );
    boost::optional< Ed::Shorthand > expected =    Ed::parse< Ed::Shorthand >( "(1,2,3,7)" );
    Ed::overrideAndExtend( origin, overrider );
    ASSERT_EQ( expected.get(), overrider.get() );
}

TEST( EdGrammarTests, Override_Shorthand2 )
{
    boost::optional< Ed::Shorthand > overrider =   Ed::parse< Ed::Shorthand >( "(1,2,3,4)" );
    boost::optional< Ed::Shorthand > origin;
    boost::optional< Ed::Shorthand > expected =    Ed::parse< Ed::Shorthand >( "(1,2,3,4)" );
    Ed::overrideAndExtend( origin, overrider );
    ASSERT_EQ( expected, overrider );
}

TEST( EdGrammarTests, Override_Shorthand3 )
{
    boost::optional< Ed::Shorthand > overrider;
    boost::optional< Ed::Shorthand > origin =        Ed::parse< Ed::Shorthand >( "(4,5,6,7)" );
    boost::optional< Ed::Shorthand > expected =    Ed::parse< Ed::Shorthand >( "(4,5,6,7)" );
    Ed::overrideAndExtend( origin, overrider );
    ASSERT_EQ( expected, overrider );
}

TEST( EdGrammarTests, Override_Shorthand4 )
{
    boost::optional< Ed::Shorthand > overrider =   Ed::parse< Ed::Shorthand >( "(1,2,3,4)" );
    boost::optional< Ed::Shorthand > origin =      Ed::parse< Ed::Shorthand >( "(4,5,6,7)" );
    boost::optional< Ed::Shorthand > expected =    Ed::parse< Ed::Shorthand >( "(1,2,3,4)" );
    Ed::overrideAndExtend( origin, overrider );
    ASSERT_EQ( expected.get(), overrider.get() );
}

TEST( EdGrammarTests, Override_Shorthand5 )
{
    boost::optional< Ed::Shorthand > overrider;
    boost::optional< Ed::Shorthand > origin;
    boost::optional< Ed::Shorthand > expected;
    Ed::overrideAndExtend( origin, overrider );
    ASSERT_EQ( expected, overrider );
}

TEST( EdGrammarTests, Override_Tags1 )
{
    boost::optional< Ed::TagList > overrider =   Ed::parse< Ed::TagList >( "<e,f,g,h>" );
    boost::optional< Ed::TagList > origin =      Ed::parse< Ed::TagList >( "<a,b,c,d>" );
    boost::optional< Ed::TagList > expected =    Ed::parse< Ed::TagList >( "<a,b,c,d,e,f,g,h>" );
    Ed::overrideAndExtend( origin, overrider );
    ASSERT_EQ( expected, overrider );
}

TEST( EdGrammarTests, Override_Tags2 )
{
    boost::optional< Ed::TagList > overrider =   Ed::parse< Ed::TagList >( "<a,b,c,1>" );
    boost::optional< Ed::TagList > origin =      Ed::parse< Ed::TagList >( "<a,b,c,1>" );
    boost::optional< Ed::TagList > expected =    Ed::parse< Ed::TagList >( "<a,b,c,1,a,b,c,1>" );
    Ed::overrideAndExtend( origin, overrider );
    ASSERT_EQ( expected, overrider );
}

TEST( EdGrammarTests, Override_Tags3 )
{
    boost::optional< Ed::TagList > overrider;
    boost::optional< Ed::TagList > origin =      Ed::parse< Ed::TagList >( "<e,f,g,h>" );
    boost::optional< Ed::TagList > expected =    Ed::parse< Ed::TagList >( "<e,f,g,h>" );
    Ed::overrideAndExtend( origin, overrider );
    ASSERT_EQ( expected, overrider );
}

TEST( EdGrammarTests, Override_Tags4 )
{
    boost::optional< Ed::TagList > overrider =   Ed::parse< Ed::TagList >( "<a,b,c,d>" );
    boost::optional< Ed::TagList > origin;
    boost::optional< Ed::TagList > expected =    Ed::parse< Ed::TagList >( "<a,b,c,d>" );
    Ed::overrideAndExtend( origin, overrider );
    ASSERT_EQ( expected, overrider );
}

TEST( EdGrammarTests, Override_Tags5 )
{
    boost::optional< Ed::TagList > overrider;
    boost::optional< Ed::TagList > origin;
    boost::optional< Ed::TagList > expected;
    Ed::overrideAndExtend( origin, overrider );
    ASSERT_EQ( expected, overrider );
}

TEST( EdGrammarTests, Override_Node1 )
{
    Ed::EdNode overrider(   Ed::parse< Ed::Node >( "a(2){ f g h i }" ) );
    Ed::EdNode origin(      Ed::parse< Ed::Node >( "a(1){ a b c d e }" ) );
    Ed::EdNode expected(    Ed::parse< Ed::Node >( "a(2){ a b c d e f g h i }" ) );
    Ed::overrideAndExtend( origin, overrider );
    ASSERT_EQ( expected, overrider );
}

