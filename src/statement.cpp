#include "ed/statement.hpp"

#include "compose.hpp"
#include "common/variant_utils.hpp"

#include <algorithm>

namespace Ed
{

ParseResult parse( const std::string& strInput, Statement& statement, std::ostream& errorStream )
{
    return parse_impl< StatementGrammar >( strInput, statement, errorStream );
}

bool operator==( const Statement& left, const Statement& right )
{
    return
        left.declarator == right.declarator &&
        left.tagList == right.tagList &&
        left.shorthand == right.shorthand;
}

std::string Statement::getIdentifier() const
{
    std::string strResult;

    if( declarator.identifier )
        strResult = declarator.identifier.get();

    return strResult;
}

bool Statement::getIdentifiers( IdentifierSet& identifiers ) const
{
    if( tagList )
    {
        boost::function< void( const Identifier& ) > functor = generics::collect( identifiers );
        boost::TypeDispatcher< Identifier > visitor( functor );
        std::for_each( tagList.get().begin(), tagList.get().end(),
            boost::apply_visitor( visitor ) );
    }
    return !identifiers.empty();
}

bool Statement::getFileRefs( FileRef::Vector& files ) const
{
    if( tagList )
    {
        boost::function< void( const FileRef& ) > functor = generics::collect( files );
        boost::TypeDispatcher< FileRef > visitor( functor );
        std::for_each( tagList.get().begin(), tagList.get().end(),
            boost::apply_visitor( visitor ) );
    }
    return !files.empty();
}

bool Statement::getRefs( Ref::Vector& refs ) const
{
    if( tagList )
    {
        boost::function< void( const Ref& ) > functor = generics::collect( refs );
        boost::TypeDispatcher< Ref > visitor( functor );
        std::for_each( tagList.get().begin(), tagList.get().end(),
            boost::apply_visitor( visitor ) );
    }
    return !refs.empty();
}

}