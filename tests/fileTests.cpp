
#include "ed/node.hpp"
#include "ed/file.hpp"
#include "ed/serialise.hpp"

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
//Inheritance Tests
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