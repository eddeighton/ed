
#include "ed/argument.h"
#include "ed/parser.h"

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
