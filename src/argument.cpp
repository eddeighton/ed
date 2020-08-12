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
    for( Ref::iterator i = ref.begin(),
        iEnd = ref.end(); i!=iEnd; ++i )
    {
        Reference& r = *i;
        r.insert( r.begin(), eRefUp );
    }
}

void propagate( std::string& str )
{
    CodeReferenceVector refs;
    extractCodeReferences( str, refs );
    for( CodeReferenceVector::reverse_iterator i = refs.rbegin(),
        iEnd = refs.rend(); i!=iEnd; ++i )
    {
        CodeReference& r = *i;
        propagate( r.ref );
        std::ostringstream os;
        os << r.ref;
        str.replace( str.begin() + r.szStart, str.begin() + r.szEnd, os.str() );
    }
}

struct ArgumentPropagator : boost::static_visitor< void >
{
    void operator()( Ref& ref ) const
    {
        propagate( ref );
    }
    void operator()( Code& code ) const
    {
        propagate( code.code );
    }
    void operator()( Expression& exp ) const
    {
        propagate( exp );
    }
    template< class T >
    void operator()( T& other ) const
    {
    }
};

void propagate( ArgumentVariant& argument )
{
    ArgumentPropagator visitor;
    boost::apply_visitor( visitor, argument );
}

}