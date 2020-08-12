#include "ed/identifier.hpp"

namespace Ed
{

ParseResult parse( const std::string& strInput, Identifier& identifier, std::ostream& errorStream )
{
    return parse_impl< IdentifierGrammar >( strInput, identifier, errorStream );
}

}