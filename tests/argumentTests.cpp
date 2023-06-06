
#include "ed/argument.hpp"
#include "ed/parser.hpp"

#include <gtest/gtest.h>

#include <string>
#include <sstream>


TEST( EdGrammarTests, Argument_1 )
{
    using namespace Ed;
    ArgumentVariant arg;
    ParseResult r = parse( "Hello World;", arg );
    Expression e;
    ed_convert( arg, e );

    ASSERT_EQ( true, r.first );
    ASSERT_EQ( "Hello World", e );
}

TEST( EdGrammarTests, Argument_2 )
{
    using namespace Ed;
    ArgumentVariant arg;
    ParseResult r = parse( "123;", arg );
    Expression e;
    ed_convert( arg, e );

    ASSERT_EQ( true, r.first );
    ASSERT_EQ( "123", e );
}
