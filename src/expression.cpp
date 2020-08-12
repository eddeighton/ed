#include "ed/expression.hpp"

namespace Ed
{

ParseResult parse( const std::string& strInput, Expression& expression, std::ostream& errorStream )
{
    return parse_impl< ExpressionGrammar >( strInput, expression, errorStream );
}

}