#include "ed/node.hpp"

#include "common/file.hpp"
#include "common/variant_utils.hpp"

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


struct ReferenceVisitor : boost::static_visitor< const Ed::Identifier& >
{
    std::string m_str;
    ReferenceVisitor( const std::string& strReference )
        :   m_str( strReference )
    {}
    
    const Ed::Identifier& operator()( const Ed::RefActionType& ) const
    {
        THROW_RTE( "Reference contains Ed::RefActionType: " << m_str );
    }
    const Ed::Identifier& operator()( const Ed::Ref& ) const
    {
        THROW_RTE( "Reference contains Ed::Ref: " << m_str );
    }
    const Ed::Identifier& operator()( const Ed::Identifier& id ) const
    {
        return id;
    }
};

boost::optional< const Node& > find( const Node& node, const std::string& strReference )
{
    boost::optional< const Node& > result;
    
    std::ostringstream osError;
    Ed::Reference ref;
    const ParseResult parseResult = Ed::parse( strReference, ref, osError );
    VERIFY_RTE_MSG( parseResult.first, "Failed to parse config reference: " << strReference << " " << osError.str() );
    VERIFY_RTE_MSG( parseResult.second.base() == strReference.end(), "Failed to parse config reference: " << strReference );
    
    const Node* pNodeIter = &node;
    
    ReferenceVisitor visitor( strReference );
    for( const RefVariant& refVar : ref )
    {
        const Ed::Identifier& id = boost::apply_visitor( visitor, refVar );
        
        bool bFound = false;
        for( const Node& childNode : pNodeIter->children )
        {
            if( childNode.statement.getIdentifier() == id )
            {
                pNodeIter = &childNode;
                bFound = true;
                break;
            }
        }
        
        VERIFY_RTE_MSG( bFound, "Failed to locate reference: " << strReference );
    }
    
    const Node& resultNode = *pNodeIter;
    result = resultNode;

    return result;
}

}