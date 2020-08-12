

#include "ed/node.h"
#include "ed/parser.h"
#include "meta/file.h"

#include <gtest/gtest.h>

#include <string>
#include <sstream>


TEST( MetaParserTests, MetaFileTest_1 )
{
    const std::string strConfig = "config{ pre( \"prefix_\";) post( \"_postfix\"; ) dir( \"/ed2/somewhereElse\"; ) ext( \"blah\"; ) }";

    Ed::Node config = Ed::parse< Ed::Node >( strConfig );

    boost::filesystem::path pathResult =
        meta::determineGeneratedFilename( boost::filesystem::path( "/ed1/someMetaProgram.mcpp" ), config );

    ASSERT_EQ( "/ed2/somewhereElse/prefix_someMetaProgram_postfix.blah", pathResult.generic_string() );
}

TEST( MetaParserTests, MetaFileTest_2 )
{
    const std::string strConfig = "config{ pre( \"prefix_\";) post( \"_postfix\"; ) dir( \"/ed2/somewhereElse\"; )  }";

    Ed::Node config = Ed::parse< Ed::Node >( strConfig );

    boost::filesystem::path pathResult =
        meta::determineGeneratedFilename( boost::filesystem::path( "/ed1/someMetaProgram.mcpp" ), config );

    ASSERT_EQ( "/ed2/somewhereElse/prefix_someMetaProgram_postfix.mcpp", pathResult.generic_string() );
}

TEST( MetaParserTests, MetaFileTest_3 )
{
    const std::string strConfig = "config{ pre( \"prefix_\";) dir( \"/ed2/somewhereElse\"; )  }";

    Ed::Node config = Ed::parse< Ed::Node >( strConfig );

    boost::filesystem::path pathResult =
        meta::determineGeneratedFilename( boost::filesystem::path( "/ed1/someMetaProgram.mcpp" ), config );

    ASSERT_EQ( "/ed2/somewhereElse/prefix_someMetaProgram.mcpp", pathResult.generic_string() );
}

TEST( MetaParserTests, MetaFileTest_4 )
{
    const std::string strConfig = "config{ pre( \"prefix_\";)   }";

    Ed::Node config = Ed::parse< Ed::Node >( strConfig );

    boost::filesystem::path pathResult =
        meta::determineGeneratedFilename( boost::filesystem::path( "/ed1/someMetaProgram.mcpp" ), config );

    ASSERT_EQ( "/ed1/prefix_someMetaProgram.mcpp", pathResult.generic_string() );
}

TEST( MetaParserTests, MetaFileTest_5 )
{
    const std::string strConfig = "config{  }";

    Ed::Node config = Ed::parse< Ed::Node >( strConfig );

    boost::filesystem::path pathResult =
        meta::determineGeneratedFilename( boost::filesystem::path( "/ed1/someMetaProgram.mcpp" ), config );

    ASSERT_EQ( "/ed1/someMetaProgram.mcpp", pathResult.generic_string() );
}



