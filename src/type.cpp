#include "ed/type.hpp"

namespace Ed
{
TypeList::TypeList( const Type& type1 )
{
    push_back( type1 );
}
TypeList::TypeList( const Type& type1, const Type& type2 )
{
    push_back( type1 );
    push_back( type2 );
}
TypeList::TypeList( const Type& type1, const Type& type2, const Type& type3 )
{
    push_back( type1 );
    push_back( type2 );
    push_back( type3 );
}

ParseResult parse( const std::string& strInput, Type& type, std::ostream& errorStream )
{
    return parse_impl< TypeGrammar >( strInput, type, errorStream );
}

ParseResult parse( const std::string& strInput, TypeList& typeList, std::ostream& errorStream )
{
    return parse_impl< TypeListGrammar >( strInput, typeList, errorStream );
}

}