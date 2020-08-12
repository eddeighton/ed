#include "ed/fileRef.hpp"

#include "common/assert_verify.hpp"
#include "common/variant_utils.hpp"

namespace Ed
{

ParseResult parse( const std::string& strInput, FileRef& fileReference, std::ostream& errorStream )
{
    return parse_impl< FileRefGrammar >( strInput, fileReference, errorStream );
}

bool operator==( const FileRef& left, const FileRef& right )
{
    boost::VariantStrictEqualityVisitor visitor;
    return left.size() == right.size() &&
        std::equal( left.begin(), left.end(), right.begin(),
            boost::apply_visitor( visitor ) );
}

}