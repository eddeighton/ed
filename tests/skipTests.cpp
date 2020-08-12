

#include "ed/skip.hpp"
#include "ed/serialise.hpp"

#include <gtest/gtest.h>

#include <string>


TEST( EdGrammarTests, Skip_1 )
{
    const std::string strInput =
            "THE QUICK BROWN FOX JUMPED OVER THE LAZY DOG 0123456789"\
            "the quick brown fox jumped over the lazy dog";
    const std::string strExpected =
            "THEQUICKBROWNFOXJUMPEDOVERTHELAZYDOG0123456789"\
            "thequickbrownfoxjumpedoverthelazydog";
    std::string strOutput;
    Ed::strip( strInput, strOutput );
    ASSERT_EQ( strExpected, strOutput );
}

TEST( EdGrammarTests, Skip_2 )
{
    std::string strInput    = "//comment\nstuff//comment\n";
    std::string strExpected = "stuff";
    std::string strOutput;
    Ed::strip( strInput, strOutput );
    ASSERT_EQ( strExpected, strOutput );
}

TEST( EdGrammarTests, Skip_3 )
{
    std::string strInput    = "/* This is a multiline\n comment \n and ends here */STRING/* This has "\
                               " some nested /* comment tags */STRING";
    std::string strExpected = "STRINGSTRING";
    std::string strOutput;
    Ed::strip( strInput, strOutput );
    ASSERT_EQ( strExpected, strOutput );
}