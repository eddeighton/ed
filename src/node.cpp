#include "ed/node.hpp"

#include "common/file.hpp"

namespace Ed
{

ParseResult parse( const std::string& strInput, Node& ed, std::ostream& errorStream )
{
	std::vector< Node > nodes;
	const ParseResult result = parse_impl< EDGrammar >( strInput, nodes, errorStream );
	VERIFY_RTE( nodes.size() == 1U );
	ed = nodes.back();
	return result;
}

ParseResult parse( const std::string& strInput, std::vector< Node >& ed, std::ostream& errorStream )
{
    return parse_impl< EDGrammar >( strInput, ed, errorStream );
}

bool operator==( const Node& left, const Node& right )
{
    return left.statement == right.statement &&
        left.children == right.children;
}

}