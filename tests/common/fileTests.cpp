
#include "common/filepath.h"

#include <gtest/gtest.h>
#include <fstream>

TEST( FileUtils, edsCannonicalise1 )
{
    const boost::filesystem::path p = "c:/a/b/c.def";
    ASSERT_EQ( p, boost::filesystem::edsCannonicalise( p ) );
}

TEST( FileUtils, edsCannonicalise2 )
{
    const boost::filesystem::path p = "c:/a/d/../b/../../c.def";
    const boost::filesystem::path p2 = "c:/c.def";
    ASSERT_EQ( p2, boost::filesystem::edsCannonicalise( p ) );
}

TEST( FileUtils, edsCannonicalise3 )
{
    const boost::filesystem::path p = "c:/a/d/./././../b/././../../c.def";
    const boost::filesystem::path p2 = "c:/c.def";
    ASSERT_EQ( p2, boost::filesystem::edsCannonicalise( p ) );
}

TEST( FileUtils, edsCannonicalise4 )
{
    const boost::filesystem::path p = "a/b/../../../c";
    ASSERT_THROW( boost::filesystem::edsCannonicalise( p ), std::runtime_error );
}

TEST( FileUtils, edsCannonicalise5 )
{
    const boost::filesystem::path p = "c:/a/d/./././../b/././../../../c.def";
    ASSERT_THROW( boost::filesystem::edsCannonicalise( p ), std::runtime_error );
}

TEST( FileUtils, edsInclude1 )
{
    const boost::filesystem::path pFile = "c:/a/b/c.h";
    const boost::filesystem::path pInclude = "c:/a/b/d.h";
    const boost::filesystem::path pResult = "d.h";
    ASSERT_EQ( pResult, boost::filesystem::edsInclude( pFile, pInclude ) );
}

TEST( FileUtils, edsInclude2 )
{
    const boost::filesystem::path pFile = "c:/a/b/c.h";
    const boost::filesystem::path pInclude = "c:/a/d.h";
    const boost::filesystem::path pResult = "../d.h";
    ASSERT_EQ( pResult, boost::filesystem::edsInclude( pFile, pInclude ) );
}

TEST( FileUtils, edsInclude3 )
{
    const boost::filesystem::path pFile = "c:/a/b/c.h";
    const boost::filesystem::path pInclude = "c:/d.h";
    const boost::filesystem::path pResult = "../../d.h";
    ASSERT_EQ( pResult, boost::filesystem::edsInclude( pFile, pInclude ) );
}

TEST( FileUtils, edsInclude4 )
{
    const boost::filesystem::path pFile = "c:/a/b/c.h";
    const boost::filesystem::path pInclude = "c:/e/d.h";
    const boost::filesystem::path pResult = "../../e/d.h";
    ASSERT_EQ( pResult, boost::filesystem::edsInclude( pFile, pInclude ) );
}

TEST( FileUtils, edsInclude5 )
{
    const boost::filesystem::path pFile = "c:/a/a/a.h";
    const boost::filesystem::path pInclude = "c:/a/a/a/a/a.h";
    const boost::filesystem::path pResult = "a/a/a.h";
    ASSERT_EQ( pResult, boost::filesystem::edsInclude( pFile, pInclude ) );
}

TEST( FileUtils, loadAsciiFile1 )
{
    //hmmm?
    const std::string strTempFileName = "temp123987123987123987.txt";
    const std::string strData =
            "THE QUICK BROWN FOX JUMPED OVER THE LAZY DOG 0123456789\n"\
            "the quick brown fox jumped over the lazy dog\n"\
            "!\"£%^&*()_+-={}[]:@~;'#<>?,./";
    {
        std::ofstream f( strTempFileName.c_str() );
        ASSERT_TRUE( f.good() );
        f << strData;
    }
    std::string str;
    boost::filesystem::loadAsciiFile( strTempFileName, str, false );
    ASSERT_STREQ( strData.c_str(), str.c_str() );

    boost::filesystem::remove( strTempFileName );
}