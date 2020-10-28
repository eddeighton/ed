
#include "ed/shorthand.hpp"
#include "ed/shorthandio.hpp"
#include "ed/serialise.hpp"
#include "ed/stlio.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <tuple>

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//Shord Hand Grammar Tests

TEST( EdGrammarTests, ShorthandIO_Test1 )
{
    using namespace Ed;
    Shorthand s;
    OShorthandStream os( s );
    os << 1.0 << 2.0 << 3.0 << 4.0;
    Shorthand expected( 1.0,2.0,3.0,4.0 );
    ASSERT_EQ( expected, s );
}

TEST( EdGrammarTests, ShorthandIO_Test2 )
{
    using namespace Ed;

    ShorthandVariant s1( Shorthand( 1.0,2.0,3.0,4.0 ) );
    ShorthandVariant s2( Shorthand( 5.0,6.0,7.0,8.0 ) );
    ShorthandVariant s3( Shorthand( 9.0,10.0,11.0,12.0 ) );
    ShorthandVariant s4( Shorthand( 13.0,14.0,15.0,16.0 ) );

    std::vector< ShorthandVariant > v = 
    {
        s1, s2, s3, s4
    };
    Shorthand expected( v );
    
    Shorthand s;
    OShorthandStream os( s );
    os <<   ePush << 1 << 2 << 3 << 4 << ePop <<
            ePush << 5.0 << 6.0 << 7.0 << 8.0 << ePop <<
            ePush << 9.0f << 10.0f << 11.0f << 12.0f << ePop <<
            ePush << 13U << 14U << 15U << 16U << ePop;
    ASSERT_EQ( expected, s );
}

TEST( EdGrammarTests, ShorthandIO_Test3 )
{
    using namespace Ed;

    ShorthandVariant s1( Shorthand( 1.0,2.0,3.0,4.0 ) );
    ShorthandVariant s2( Shorthand( true, false ) );
    ShorthandVariant s3( Shorthand( Ed::parse< Ed::FileRef >( "/e" ), Ed::parse< Ed::FileRef >( "../../e/e/e" ) ) );
    ShorthandVariant s4( Shorthand( Ed::parse< Ed::Ref >( ".e" ), Ed::parse< Ed::Ref >( "...e.e.e.e" ) ) );
    ShorthandVariant s5( Shorthand( Ed::parse< Ed::Expression >( " ...e.e.e.e 1.0, 2.0, 3.0, 4.0, true, false, ;" ) ) );
    ShorthandVariant s6( Shorthand( Ed::parse< Ed::Code >( "{ std::string( \";/..{}[]:@~;#()\" ); }" ) ) );

    std::vector< ShorthandVariant > v = 
    {
        s1, s2, s3, s4, s5, s6
    };
    Shorthand expected( v );
    
    Shorthand s;
    OShorthandStream os( s );
    os <<   ePush << 1U << 2 << 3.0f << 4.0 << ePop <<
            ePush << true << false << ePop <<
            ePush << "/e" << "../../e/e/e" << ePop <<
            ePush << ".e" << "...e.e.e.e" << ePop <<
            ePush << " ...e.e.e.e 1.0, 2.0, 3.0, 4.0, true, false, ;" << ePop <<
            ePush << "{ std::string( \";/..{}[]:@~;#()\" ); }" << ePop;
    ASSERT_EQ( expected, s );
}

TEST( EdGrammarTests, ShorthandIO_Test4 )
{
    using namespace Ed;

    ShorthandVariant s1( Shorthand( 1.0,2.0,3.0,4.0 ) );
    ShorthandVariant s2( Shorthand( true, false ) );
    ShorthandVariant s3( Shorthand( Ed::parse< Ed::FileRef >( "/e" ), Ed::parse< Ed::FileRef >( "../../e/e/e" ) ) );
    ShorthandVariant s4( Shorthand( Ed::parse< Ed::Ref >( ".e" ), Ed::parse< Ed::Ref >( "...e.e.e.e" ) ) );
    ShorthandVariant s5( Shorthand( Ed::parse< Ed::Expression >( " ...e.e.e.e 1.0, 2.0, 3.0, 4.0, true, false, ;" ) ) );
    ShorthandVariant s6( Shorthand( Ed::parse< Ed::Code >( "{ std::string( \";/..{}[]:@~;#()\" ); }" ) ) );

    std::vector< ShorthandVariant > v = 
    {
        s1, s2, s3, s4, s5, s6
    };
    Shorthand expected( v );
    
    Shorthand s;
    OShorthandStream os( s );
    os <<   Shorthand( 1.0,2.0,3.0,4.0 ) <<
            Shorthand( true, false )  <<
            ePush << Ed::parse< Ed::FileRef >( "/e" ) << Ed::parse< Ed::FileRef >( "../../e/e/e" ) << ePop <<
            ePush << Ed::parse< Ed::Ref >( ".e" ) << Ed::parse< Ed::Ref >( "...e.e.e.e" ) << ePop <<
            ePush << Ed::parse< Ed::Expression >( " ...e.e.e.e 1.0, 2.0, 3.0, 4.0, true, false, ;" ) << ePop <<
            ePush << Ed::parse< Ed::Code >( "{ std::string( \";/..{}[]:@~;#()\" ); }" ) << ePop;
    ASSERT_EQ( expected, s );
}

TEST( EdGrammarTests, ShorthandIO_Test5 )
{
    using namespace Ed;

    Shorthand s;
    OShorthandStream os( s );
    os << 1u << 2 << 3.0f << 4.0;

    IShorthandStream is( s );
    unsigned int i1;
    int i2;
    float i3;
    double i4;
    is >> i1 >> i2 >> i3 >> i4;

    ASSERT_EQ( 1, i1 );
    ASSERT_EQ( 2, i2 );
    ASSERT_EQ( 3, i3 );
    ASSERT_EQ( 4, i4 );
}

TEST( EdGrammarTests, ShorthandIO_Test6 )
{
    using namespace Ed;

    Shorthand s;
    OShorthandStream os( s );
    os << ".a" << "/a" << "exp;" << "{code;}" << true;
    Ed::Ref i1;
    Ed::FileRef i2;
    Ed::Expression i3;
    Ed::Code i4;
    bool i5;
    IShorthandStream is( s );
    is >> i1 >> i2 >> i3 >> i4 >> i5;

    ASSERT_EQ( Ed::parse< Ed::Ref >( ".a" ), i1 );
    ASSERT_EQ( Ed::parse< Ed::FileRef >( "/a" ), i2 );
    ASSERT_EQ( Ed::parse< Ed::Expression >( "exp;" ), i3 );
    ASSERT_EQ( Ed::parse< Ed::Code >( "{code;}" ), i4 );
    ASSERT_EQ( true, i5 );
}

TEST( EdGrammarTests, ShorthandIO_Test7 )
{
    using namespace Ed;

    Shorthand s;
    OShorthandStream os( s );
    os << ".a" << "/a" << "exp;" << "{code;}" << true;
    std::string i1, i2, i3, i4, i5;
    IShorthandStream is( s );
    is >> i1 >> i2 >> i3 >> i4 >> i5;

    ASSERT_EQ( ".a", i1 );
    ASSERT_EQ( "/a", i2 );
    ASSERT_EQ( "exp;", i3 );
    ASSERT_EQ( "{code;}", i4 );
    ASSERT_EQ( "true", i5 );
}


TEST( EdGrammarTests, ShorthandIO_Test8 )
{
    using namespace Ed;

    Shorthand s;
    OShorthandStream os( s );
    os << ePush << 1u << 2 << 3.0f << 4.0 << ePop;

    IShorthandStream is( s );
    unsigned int i1;
    int i2;
    float i3;
    double i4;
    is >> ePush >> i1 >> i2 >> i3 >> i4 >> ePop;

    ASSERT_EQ( 1, i1 );
    ASSERT_EQ( 2, i2 );
    ASSERT_EQ( 3, i3 );
    ASSERT_EQ( 4, i4 );
}

TEST( EdGrammarTests, ShorthandIO_Test9 )
{
    using namespace Ed;

    Shorthand s;
    OShorthandStream os( s );
    os << ePush << ".a" << "/a" << "exp;" << "{code;}" << true << ePop;
    Ed::Ref i1;
    Ed::FileRef i2;
    Ed::Expression i3;
    Ed::Code i4;
    bool i5;
    IShorthandStream is( s );
    is >> ePush >> i1 >> i2 >> i3 >> i4 >> i5 >> ePop;

    ASSERT_EQ( Ed::parse< Ed::Ref >( ".a" ), i1 );
    ASSERT_EQ( Ed::parse< Ed::FileRef >( "/a" ), i2 );
    ASSERT_EQ( Ed::parse< Ed::Expression >( "exp;" ), i3 );
    ASSERT_EQ( Ed::parse< Ed::Code >( "{code;}" ), i4 );
    ASSERT_EQ( true, i5 );
}

TEST( EdGrammarTests, ShorthandIO_Test10 )
{
    using namespace Ed;

    Shorthand s;
    OShorthandStream os( s );
    os << ePush << ".a" << "/a" << "exp;" << "{code;}" << true << ePop;
    std::string i1, i2, i3, i4, i5;
    IShorthandStream is( s );
    is >> ePush >> i1 >> i2 >> i3 >> i4 >> i5 >> ePop;

    ASSERT_EQ( ".a", i1 );
    ASSERT_EQ( "/a", i2 );
    ASSERT_EQ( "exp;", i3 );
    ASSERT_EQ( "{code;}", i4 );
    ASSERT_EQ( "true", i5 );
}

TEST( EdGrammarTests, ShorthandIO_Test11 )
{
    using namespace Ed;

    Shorthand s = parse< Shorthand >( "( 1,2,(5,6,7),9,10,11,12 )" );

    IShorthandStream is( s );
    std::tuple< int, int > t1;
    std::tuple< int, int, int > t2;
    std::tuple< int, int, int, int > t3;
    is >> t1 >> ePush >> t2 >> ePop >> t3;
    ASSERT_EQ( std::make_tuple(1,2), t1 );
    ASSERT_EQ( std::make_tuple(5,6,7), t2 );
    ASSERT_EQ( std::make_tuple(9,10,11,12), t3 );
}

TEST( EdGrammarTests, ShorthandIO_Test12 )
{
    using namespace Ed;
    
    Shorthand s;
    {
        std::tuple< int, int > t1 = std::make_tuple(1,2);
        std::tuple< int, int, int > t2 = std::make_tuple(5,6,7);
        std::tuple< int, int, int, int > t3 = std::make_tuple(9,10,11,12);
        OShorthandStream os( s );
        os << t1 << ePush << t2 << ePush << t3 << ePop << ePop;
    }

    IShorthandStream is( s );
    std::tuple< int, int > t1;
    std::tuple< int, int, int > t2;
    std::tuple< int, int, int, int > t3;
    is >> t1 >> ePush >> t2 >> ePush >> t3 >> ePop >> ePop;

    ASSERT_EQ( std::make_tuple(1,2), t1 );
    ASSERT_EQ( std::make_tuple(5,6,7), t2 );
    ASSERT_EQ( std::make_tuple(9,10,11,12), t3 );

    std::ostringstream os;
    os << s;
    ASSERT_EQ( "(1,2,(5,6,7,(9,10,11,12)))", os.str() );
}