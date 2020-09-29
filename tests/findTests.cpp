
#include "ed/node.hpp"
#include "ed/shorthandio.hpp"

#include <gtest/gtest.h>

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//Ed Grammar Tests

static const std::string strInput =
R"(
    root
    {
        a( 1 )
        b( 2 )
        
        c
        {
            d( 5 )
            e( 6 )
        }
        f
        {
            g( 7 )
            h( 8 )
        }
    }
)";

template< typename TExpected >
void testFind( const Ed::Node& rootNode, const std::string& ref, TExpected expectedValue )
{
    boost::optional< const Ed::Node& > findResult = 
        Ed::find( rootNode, ref );
    ASSERT_TRUE( findResult );
    {
        Ed::IShorthandStream is( findResult.get().statement.shorthand.get() );
        TExpected actualValue;
        is >> actualValue;
        ASSERT_EQ( actualValue, expectedValue );
    }
}

TEST( EDNodeFindTests, Find1 )
{
    std::vector< Ed::Node > edNodes;
    {
        std::ostringstream osError;
        const Ed::ParseResult result = Ed::parse( strInput, edNodes, osError );
        ASSERT_TRUE( result.first );
        ASSERT_TRUE( result.second.base() == strInput.end() );
    }
    
    ASSERT_EQ( edNodes.size(), 1U );
    const Ed::Node& rootNode = edNodes.front();
    
    testFind< int >( rootNode, ".a", 1 );
    testFind< int >( rootNode, ".b", 2 );
    testFind< int >( rootNode, ".c.d", 5 );
    testFind< int >( rootNode, ".c.e", 6 );
    testFind< int >( rootNode, ".f.g", 7 );
    testFind< int >( rootNode, ".f.h", 8 );
    
    
}