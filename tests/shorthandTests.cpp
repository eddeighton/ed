
#include "ed/shorthand.hpp"
#include "ed/serialise.hpp"

#include <gtest/gtest.h>

#include <boost/bind.hpp>

#include <algorithm>

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//Shord Hand Grammar Tests

TEST( EdGrammarTests, ShortHand_Simple1 )
{
    std::string strInput( "(..a)" );

    Ed::Ref ref;
    Ed::parse( "..a", ref );

    Ed::Shorthand expected( ref );

    Ed::Shorthand resultShorthand;
    const Ed::ParseResult result = Ed::parse( strInput, resultShorthand );

    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
    ASSERT_EQ( expected, resultShorthand );
}

TEST( EdGrammarTests, ShortHand_Simple2 )
{
    std::string strInput( "(1;)" );

    Ed::Expression exp;
    Ed::parse( "1;", exp );

    Ed::Shorthand expected( exp );
    Ed::Shorthand resultShorthand;
    const Ed::ParseResult result = Ed::parse( strInput, resultShorthand );

    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
    ASSERT_EQ( expected, resultShorthand );
}

TEST( EdGrammarTests, ShortHand_Simple3 )
{
    std::string strInput( "(../a/b/c)" );

    Ed::FileRef file;
    Ed::parse( "../a/b/c", file );

    Ed::Shorthand expected( file );
    Ed::Shorthand resultShorthand;
    const Ed::ParseResult result = Ed::parse( strInput, resultShorthand );

    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
    ASSERT_EQ( expected, resultShorthand );
}

TEST( EdGrammarTests, ShortHand_Simple4 )
{
    std::string strInput( "({return 0.0f;})" );

    Ed::Code code;
    Ed::parse( "{return 0.0f;}", code );

    Ed::Shorthand expected( code );
    Ed::Shorthand resultShorthand;
    const Ed::ParseResult result = Ed::parse( strInput, resultShorthand );

    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
    ASSERT_EQ( expected, resultShorthand );
}

TEST( EdGrammarTests, ShortHand_Simple5 )
{
    std::string strInput( "(123.456)" );

    Ed::Shorthand expected( 123.456 );
    Ed::Shorthand resultShorthand;
    const Ed::ParseResult result = Ed::parse( strInput, resultShorthand );

    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
    ASSERT_EQ( expected, resultShorthand );
}

TEST( EdGrammarTests, ShortHand_Simple6 )
{
    std::string strInput( "(true,false)" );

    Ed::Shorthand expected( true, false );
    Ed::Shorthand resultShorthand;
    const Ed::ParseResult result = Ed::parse( strInput, resultShorthand );

    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
    ASSERT_EQ( expected, resultShorthand );
}

TEST( EdGrammarTests, ShortHand_Simple7 )
{
    std::string strInput( "(1,true)" );

    Ed::Shorthand expected( 1.0,true );
    Ed::Shorthand resultShorthand;
    const Ed::ParseResult result = Ed::parse( strInput, resultShorthand );

    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );

    std::ostringstream os1;
    os1 << resultShorthand;
    std::string s1 = os1.str();

    ASSERT_EQ( expected, resultShorthand );
}

TEST( EdGrammarTests, ShortHand_Composition1 )
{
	std::string strInput( "( ../e, .e, ..e, ../e/e, ..; )" );

	Ed::Shorthand expected;
	expected.push_back( Ed::parse< Ed::FileRef >( "../e" ) );
	expected.push_back( Ed::parse< Ed::Ref >( ".e" ) );
	expected.push_back( Ed::parse< Ed::Ref >( "..e" ) );
	expected.push_back( Ed::parse< Ed::FileRef >( "../e/e" ) );
	expected.push_back( Ed::parse< Ed::Expression >( "..;" ) );

    Ed::Shorthand resultShorthand;
    const Ed::ParseResult result = Ed::parse( strInput, resultShorthand );

    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
    ASSERT_EQ( expected, resultShorthand );
}

TEST( EdGrammarTests, ShortHand_Composition2 )
{
    std::string strInput( "( 1;, 1.0 )" );
	Ed::Shorthand expected;
	expected.push_back( Ed::parse< Ed::Expression >( "1;" ) );
	expected.push_back( 1.0 );

    Ed::Shorthand resultShorthand;
    const Ed::ParseResult result = Ed::parse( strInput, resultShorthand );

    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
    ASSERT_EQ( expected, resultShorthand );
}

TEST( EdGrammarTests, ShortHand_Composition3 )
{
    std::string strInput( "( 1.0, 1; )" );
	Ed::Shorthand expected;
	expected.push_back( 1.0 );
	expected.push_back( Ed::parse< Ed::Expression >( "1;" ) );

    Ed::Shorthand resultShorthand;
    const Ed::ParseResult result = Ed::parse( strInput, resultShorthand );

    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
    ASSERT_EQ( expected, resultShorthand );
}

class ArgPairBase
{
public:
    typedef boost::shared_ptr< ArgPairBase > Ptr;
    typedef std::vector< Ptr > PtrVector;
    virtual ~ArgPairBase(){};
    virtual void toStream( std::ostream& os )=0;
    virtual void toExpected( Ed::Shorthand& expected )=0;
};

template< class T >
class ArgPairImpl : public ArgPairBase
{
public:
    ArgPairImpl( const T& actual, const std::string& str )
        :   m_actual( actual ),
            m_str( str )
    {
    }

    virtual void toStream( std::ostream& os )
    {
        os << m_str;
    }
    virtual void toExpected( Ed::Shorthand& expected )
    {
        expected.push_back( m_actual );
    }
    const T m_actual;
    const std::string m_str;
};

TEST( EdGrammarTests, ShortHand_Permutations1 )
{
    ArgPairBase::PtrVector pairs;
    //pairs.push_back( ArgPairBase::Ptr( new ArgPairImpl< bool >( true, "true" ) ) );
    pairs.push_back( ArgPairBase::Ptr( new ArgPairImpl< double >( 1, "1" ) ) );
    pairs.push_back( ArgPairBase::Ptr( new ArgPairImpl< Ed::Ref >( Ed::parse< Ed::Ref >( "..a.b.c.d" ), "..a.b.c.d" ) ) );
    pairs.push_back( ArgPairBase::Ptr( new ArgPairImpl< Ed::FileRef >( Ed::parse< Ed::FileRef >( "/a/b/c/d/e" ), "/a/b/c/d/e" ) ) );
    pairs.push_back( ArgPairBase::Ptr( new ArgPairImpl< Ed::Code >( Ed::parse< Ed::Code >(
        "{ std::string( \";/..{}[]:@~;#()\" ); }" ),
        "{ std::string( \";/..{}[]:@~;#()\" ); }" ) ) );
    pairs.push_back( ArgPairBase::Ptr( new ArgPairImpl< Ed::Expression >( Ed::parse< Ed::Expression >( "1;" ), "1;" ) ) );

    std::sort( pairs.begin(), pairs.end() );
    int iPermCounter = 0;
    do
    {
        std::ostringstream osInput;
        Ed::Shorthand expected;

        osInput << "( ";
        for( ArgPairBase::PtrVector::iterator
            i = pairs.begin(),
            iNext = pairs.begin(),
            iEnd = pairs.end(); i!=iEnd; ++i )
        {
            ++iNext;
            (*i)->toExpected( expected );
            (*i)->toStream( osInput );
            if( iNext != iEnd )
                osInput << " , ";
        }
        osInput << " )";

        Ed::Shorthand resultShorthand;
        const std::string strInput = osInput.str();
        const Ed::ParseResult result = Ed::parse( strInput, resultShorthand );
        ASSERT_TRUE( result.first );
        ASSERT_EQ( result.second.base(), strInput.end() );
        ASSERT_EQ( expected, resultShorthand );
        ++iPermCounter;

    }while( std::next_permutation( pairs.begin(), pairs.end() ) );

    std::cout << "ShortHand_Permutations tested: " << iPermCounter << " permutations\n";
}

TEST( EdGrammarTests, ShortHand_Permutations2 )
{
    ArgPairBase::PtrVector pairs;
    pairs.push_back( ArgPairBase::Ptr( new ArgPairImpl< double >( 123.0123, "123.0123" ) ) );
    //pairs.push_back( ArgPairBase::Ptr( new ArgPairImpl< double >( 1, "1" ) ) );
    pairs.push_back( ArgPairBase::Ptr( new ArgPairImpl< Ed::Ref >( Ed::parse< Ed::Ref >( "." ), "." ) ) );
    pairs.push_back( ArgPairBase::Ptr( new ArgPairImpl< Ed::FileRef >( Ed::parse< Ed::FileRef >( "../../e" ), "../../e" ) ) );
    pairs.push_back( ArgPairBase::Ptr( new ArgPairImpl< Ed::Code >( Ed::parse< Ed::Code >( "{{}}" ), "{{}}" ) ) );
    pairs.push_back( ArgPairBase::Ptr( new ArgPairImpl< Ed::Expression >(
        Ed::parse< Ed::Expression >(
            "something::Instance( \"string\" ).thing( (abs ) ).x;" ),
            "something::Instance( \"string\" ).thing( (abs ) ).x;" ) ) );

    std::sort( pairs.begin(), pairs.end() );
    int iPermCounter = 0;
    do
    {
        std::ostringstream osInput;
        Ed::Shorthand expected;

        osInput << "( ";
        for( ArgPairBase::PtrVector::iterator
            i = pairs.begin(),
            iNext = pairs.begin(),
            iEnd = pairs.end(); i!=iEnd; ++i )
        {
            ++iNext;
            (*i)->toExpected( expected );
            (*i)->toStream( osInput );
            if( iNext != iEnd )
                osInput << " , ";
        }
        osInput << " )";

        Ed::Shorthand resultShorthand;
        const std::string strInput = osInput.str();
        const Ed::ParseResult result = Ed::parse( strInput, resultShorthand );
        ASSERT_TRUE( result.first );
        ASSERT_EQ( result.second.base(), strInput.end() );
        ASSERT_EQ( expected, resultShorthand );
        ++iPermCounter;

    }while( std::next_permutation( pairs.begin(), pairs.end() ) );

    std::cout << "ShortHand_Permutations tested: " << iPermCounter << " permutations\n";
}
TEST( EdGrammarTests, ShortHand_Permutations3 )
{
    ArgPairBase::PtrVector pairs;
    pairs.push_back( ArgPairBase::Ptr( new ArgPairImpl< double >( 1234.0, "1.234e3" ) ) );
    //pairs.push_back( ArgPairBase::Ptr( new ArgPairImpl< double >( 1, "1" ) ) );
    pairs.push_back( ArgPairBase::Ptr( new ArgPairImpl< Ed::Ref >( Ed::parse< Ed::Ref >( ".." ), ".." ) ) );
    pairs.push_back( ArgPairBase::Ptr( new ArgPairImpl< Ed::FileRef >( Ed::parse< Ed::FileRef >( "/e" ), "/e" ) ) );
    pairs.push_back( ArgPairBase::Ptr( new ArgPairImpl< Ed::Code >( Ed::parse< Ed::Code >( "{}" ), "{}" ) ) );
    pairs.push_back( ArgPairBase::Ptr( new ArgPairImpl< Ed::Expression >( Ed::parse< Ed::Expression >( "1;" ), "1;" ) ) );

    std::sort( pairs.begin(), pairs.end() );
    int iPermCounter = 0;
    do
    {
        std::ostringstream osInput;
        Ed::Shorthand expected;

        osInput << "( ";
        for( ArgPairBase::PtrVector::iterator
            i = pairs.begin(),
            iNext = pairs.begin(),
            iEnd = pairs.end(); i!=iEnd; ++i )
        {
            ++iNext;
            (*i)->toExpected( expected );
            (*i)->toStream( osInput );
            if( iNext != iEnd )
                osInput << " , ";
        }
        osInput << " )";

        Ed::Shorthand resultShorthand;
        const std::string strInput = osInput.str();
        const Ed::ParseResult result = Ed::parse( strInput, resultShorthand );
        ASSERT_TRUE( result.first );
        ASSERT_EQ( result.second.base(), strInput.end() );
        ASSERT_EQ( expected, resultShorthand );
        ++iPermCounter;

    }while( std::next_permutation( pairs.begin(), pairs.end() ) );

    std::cout << "ShortHand_Permutations tested: " << iPermCounter << " permutations\n";
}
TEST( EdGrammarTests, ShortHand_Permutations4 )
{
    ArgPairBase::PtrVector pairs;
    pairs.push_back( ArgPairBase::Ptr( new ArgPairImpl< bool >( false, "false" ) ) );
    pairs.push_back( ArgPairBase::Ptr( new ArgPairImpl< double >( 1, "1" ) ) );
    pairs.push_back( ArgPairBase::Ptr( new ArgPairImpl< Ed::Ref >( Ed::parse< Ed::Ref >( "..." ), "..." ) ) );
    pairs.push_back( ArgPairBase::Ptr( new ArgPairImpl< Ed::FileRef >( Ed::parse< Ed::FileRef >( "../e" ), "../e" ) ) );
    pairs.push_back( ArgPairBase::Ptr( new ArgPairImpl< Ed::Expression >( Ed::parse< Ed::Expression >( "true;" ), "true;" ) ) );
    //pairs.push_back( ArgPairBase::Ptr( new ArgPairImpl< Ed::Expression >( Ed::parse< Ed::Expression >( "1;" ), "1;" ) ) );

    std::sort( pairs.begin(), pairs.end() );
    int iPermCounter = 0;
    do
    {
        std::ostringstream osInput;
        Ed::Shorthand expected;

        osInput << "( ";
        for( ArgPairBase::PtrVector::iterator
            i = pairs.begin(),
            iNext = pairs.begin(),
            iEnd = pairs.end(); i!=iEnd; ++i )
        {
            ++iNext;
            (*i)->toExpected( expected );
            (*i)->toStream( osInput );
            if( iNext != iEnd )
                osInput << " , ";
        }
        osInput << " )";

        Ed::Shorthand resultShorthand;
        const std::string strInput = osInput.str();
        const Ed::ParseResult result = Ed::parse( strInput, resultShorthand );
        ASSERT_TRUE( result.first );
        ASSERT_EQ( result.second.base(), strInput.end() );
        ASSERT_EQ( expected, resultShorthand );
        ++iPermCounter;

    }while( std::next_permutation( pairs.begin(), pairs.end() ) );

    std::cout << "ShortHand_Permutations tested: " << iPermCounter << " permutations\n";
}
