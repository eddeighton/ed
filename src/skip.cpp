#include "ed/skip.hpp"

#include "ed/parser.hpp"

#include "common/assert_verify.hpp"

#include <sstream>

namespace Ed
{

void strip( const std::string& strInput, std::string& strOutput )
{
    std::ostringstream osError;
    ParseResult result = parse_impl< AllGrammar >( strInput, strOutput, osError );
    VERIFY_RTE( result.first );
}

}