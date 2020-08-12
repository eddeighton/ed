
#include "ed/nodeio.hpp"
#include "ed/parser.hpp"

#include <gtest/gtest.h>

#include <string>
#include <sstream>


TEST( EdGrammarTests, NodeIO_1 )
{
    using namespace Ed;
    
    Node n;
    ONodeStream ar( n );
    ar << Entry< const int >( "test1", 123 );
    
    ASSERT_EQ( 1, n.children.size() );
}

TEST( EdGrammarTests, NodeIO_2 )
{
    using namespace Ed;
    
    Node n;
    ONodeStream ar( n );
    ar << Entry< const int >( "test1", 123 );

    int iResult = 0;
    INodeStream is( n );
    is >> Entry< int >( "test1", iResult );
    
    ASSERT_EQ( 123, iResult );
}
