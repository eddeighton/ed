
#include "ed/node.hpp"
#include "ed/file.hpp"
#include "ed/serialise.hpp"
#include "ed/shorthandio.hpp"
#include "ed/stlio.hpp"

#include "common/file.hpp"

#include <gtest/gtest.h>

#include <boost/filesystem.hpp>

#include <vector>

std::string getTestFile( const std::string& strFileName )
{
    static const std::string strTestFolder = "../tests/ed/inheritTestData";

    boost::filesystem::path actualPath = boost::filesystem::edsCannonicalise(
        boost::filesystem::absolute( strTestFolder ) / boost::filesystem::path( strFileName ) );
    VERIFY_RTE( boost::filesystem::exists( actualPath ) );
    return actualPath.string();
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
// Inheritance Tests
TEST( EdGrammarTests, Inherit_1 )
{
    /*
    std::vector< Ed::Node > expected, input;
    Ed::loadEd( getTestFile( "t1_expected.ed" ), expected );
    Ed::loadEdAndInherit( getTestFile( "t1.ed" ), input );
    ASSERT_EQ( expected, input );*/
}

TEST( EdGrammarTests, Inherit_2 )
{
    /*
    std::vector< Ed::Node > expected, input;
    Ed::loadEd( getTestFile( "t2_expected.ed" ), expected );
    Ed::loadEdAndInherit( getTestFile( "t2.ed" ), input );
    ASSERT_EQ( expected, input );*/
}

namespace Ed
{
    
inline OShorthandStream& operator<<( OShorthandStream& os, const std::vector< int >& data )
{
    return serialiseOut( os, data );
}

inline IShorthandStream& operator>>( IShorthandStream& is, std::vector< int >& data )
{
    return serialiseIn( is, data );
}
}

TEST( EdGrammarTests, Basic )
{
    Ed::BasicFileSystem fileSystem;
    Ed::File            file( fileSystem, "/home/foobar/test_Debug/test.ed" );
    file.expandShorthand();
    file.removeTypes();
    Ed::Node n;
    file.toNode( n );

    ASSERT_EQ( "", file.getStatement().getIdentifier() );

    auto nodeOptEd = Ed::find( n, ".ed" );
    ASSERT_TRUE( nodeOptEd.has_value() );

    {
        auto nodeOpt = Ed::find( nodeOptEd.value(), ".list_of_floats" );
        ASSERT_TRUE( nodeOpt.has_value() );
        const Ed::Node& node = nodeOpt.value();

        std::vector< float > list_of_floats;
        Ed::IShorthandStream is( node.getShorty().value() );

        is >> list_of_floats;

        auto expected = std::vector< float >{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f };
        ASSERT_EQ( list_of_floats, expected );
    }

    {
        auto nodeOpt = Ed::find( nodeOptEd.value(), ".list_of_ints" );
        ASSERT_TRUE( nodeOpt.has_value() );
        const Ed::Node& node = nodeOpt.value();

        std::vector< int > list_of_ints;
        Ed::IShorthandStream is( node.getShorty().value() );

        is >> list_of_ints;

        auto expected = std::vector< int >{ 1, 2, 3, 4, 5, 6 };
        ASSERT_EQ( list_of_ints, expected );
    }

}
