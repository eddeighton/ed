#include "ed/ref.hpp"

#include "common/math.hpp"
#include "common/variant_utils.hpp"

namespace Ed
{

ParseResult parse( const std::string& strInput, Reference& reference, std::ostream& errorStream )
{
    return parse_impl< ReferenceGrammar >( strInput, reference, errorStream );
}

bool operator==( const Reference& left, const Reference& right )
{
    boost::VariantStrictEqualityVisitor visitor;
    return left.size() == right.size() &&
        std::equal( left.begin(), left.end(), right.begin(),
            boost::apply_visitor( visitor ) );
}


ParseResult parse( const std::string& strInput, Ref& ref, std::ostream& errorStream )
{
    return parse_impl< RefGrammar >( strInput, ref, errorStream );
}

bool operator==( const Ref& left, const Ref& right )
{
    return left.size() == right.size() &&
        std::equal( left.begin(), left.end(), right.begin() );
}

}