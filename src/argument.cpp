#include "ed/argument.hpp"

#include "ed/code.hpp"
#include "ed/serialise.hpp"

#include <sstream>

namespace Ed
{

ParseResult parse( const std::string& strInput, ArgumentVariant& argument, std::ostream& errorStream )
{
    return parse_impl< ArgumentGrammar >( strInput, argument, errorStream );
}

void propagate( Ref& ref )
{
    for( Reference& r : ref )
    {
        r.insert( r.begin(), eRefUp );
    }
}

void propagate( std::string& str )
{
    CodeReferenceVector refs;
    extractCodeReferences( str, refs );
    for( auto i = refs.rbegin(), iEnd = refs.rend(); i != iEnd; ++i )
    {
        CodeReference& r = *i;
        propagate( r.ref );
        std::ostringstream os;
        os << r.ref;
        str.replace(
            str.begin() + static_cast< int >( r.szStart ), str.begin() + static_cast< int >( r.szEnd ), os.str() );
    }
}

struct ArgumentPropagator : boost::static_visitor< void >
{
    void operator()( Ref& ref ) const { propagate( ref ); }
    void operator()( Code& code ) const { propagate( code.code ); }
    void operator()( Expression& exp ) const { propagate( exp ); }
    template < class T >
    void operator()( T& ) const
    {
    }
};

void propagate( ArgumentVariant& argument )
{
    ArgumentPropagator visitor;
    boost::apply_visitor( visitor, argument );
}

} // namespace Ed