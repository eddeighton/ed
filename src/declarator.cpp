#include "ed/declarator.hpp"

namespace Ed
{

ParseResult parse( const std::string& strInput, Declarator& declarator, std::ostream& errorStream )
{
    return parse_impl< DeclaratorGrammar >( strInput, declarator, errorStream );
}

bool operator==( const Declarator& left, const Declarator& right )
{
    return left.identifier == right.identifier &&
        left.typeList == right.typeList;
}

}