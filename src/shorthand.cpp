#include "ed/shorthand.hpp"

#include <iomanip>

namespace Ed
{

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Shorthand
Shorthand::Shorthand( const ArgumentVariant& value )
{
    children.push_back( ( ShorthandVariant( value ) ) );
}
Shorthand::Shorthand( const ArgumentVariant& v1, const ArgumentVariant& v2 )
{
    children.push_back( ( ShorthandVariant( v1 ) ) );
    children.push_back( ( ShorthandVariant( v2 ) ) );
}
Shorthand::Shorthand( const ArgumentVariant& v1, const ArgumentVariant& v2, const ArgumentVariant& v3 )
{
    children.push_back( ( ShorthandVariant( v1 ) ) );
    children.push_back( ( ShorthandVariant( v2 ) ) );
    children.push_back( ( ShorthandVariant( v3 ) ) );
}
Shorthand::Shorthand( const ArgumentVariant& v1, const ArgumentVariant& v2, const ArgumentVariant& v3,
                      const ArgumentVariant& v4 )
{
    children.push_back( ( ShorthandVariant( v1 ) ) );
    children.push_back( ( ShorthandVariant( v2 ) ) );
    children.push_back( ( ShorthandVariant( v3 ) ) );
    children.push_back( ( ShorthandVariant( v4 ) ) );
}

Shorthand::Shorthand( const std::vector< ShorthandVariant >& _children )
    : children( _children )
{
}

Shorthand::Shorthand( const Ed::Ref& ref )
{
    children.push_back( ( ShorthandVariant( ( ArgumentVariant( ref ) ) ) ) );
}

Shorthand::Shorthand( const Ed::FileRef& ref )
{
    children.push_back( ( ShorthandVariant( ( ArgumentVariant( ref ) ) ) ) );
}

Shorthand::Shorthand( const std::vector< ArgumentVariant >& args )
{
    std::copy( args.begin(), args.end(), std::back_inserter( children ) );
}

void Shorthand::push_back( const ArgumentVariant& av )
{
    children.push_back( ( ShorthandVariant( av ) ) );
}

ParseResult parse( const std::string& strInput, Shorthand& shortHand, std::ostream& errorStream )
{
    return parse_impl< ShorthandGrammar >( strInput, shortHand, errorStream );
}

struct ShorthandVariantComparisonVisitor : boost::static_visitor< bool >
{
    bool operator()( const Shorthand& left, const Shorthand& right ) const { return left == right; }
    bool operator()( const ArgumentVariant& left, const ArgumentVariant& right ) const
    {
        boost::VariantStrictEqualityVisitor visitor;
        return boost::apply_visitor( visitor )( left, right );
    }
    bool operator()( const Shorthand&, const ArgumentVariant& ) const { return false; }
    bool operator()( const ArgumentVariant&, const Shorthand& ) const { return false; }
};

bool operator==( const Shorthand& left, const Shorthand& right )
{
    ShorthandVariantComparisonVisitor visitor;
    return left.children.size() == right.children.size()
           && std::equal(
               left.children.begin(), left.children.end(), right.children.begin(), boost::apply_visitor( visitor ) );
}

struct ShortHandVariantArgVariantCompareVisitor : boost::static_visitor< bool >
{
    const ArgumentVariant& av;
    ShortHandVariantArgVariantCompareVisitor( const ArgumentVariant& _av )
        : av( _av )
    {
    }
    bool operator()( const Shorthand& ) const { return false; }
    bool operator()( const ArgumentVariant& a ) const { return av == a; }
};

struct ShortHandComparisonVisitor : boost::static_visitor< bool >
{
    const Shorthand& s;
    ShortHandComparisonVisitor( const Shorthand& _sh )
        : s( _sh )
    {
    }
    bool operator()( const Shorthand& sh ) const { return s == sh; }
    bool operator()( const ArgumentVariant& av ) const
    {
        if( s.children.size() == 1u )
            return boost::apply_visitor( ShortHandVariantArgVariantCompareVisitor( av ), s.children[ 0u ] );
        else
            return false;
    }
};

bool compareShortHand( const Shorthand& sh, const ShorthandVariant& v )
{
    return boost::apply_visitor( ShortHandComparisonVisitor( sh ), v );
}

struct ShortHandPropagator : boost::static_visitor< void >
{
    void operator()( Shorthand& sh ) const
    {
        std::for_each( sh.children.begin(), sh.children.end(), boost::apply_visitor( *this ) );
    }
    void operator()( ArgumentVariant& a ) const { propagate( a ); }
};

void propagate( ShorthandVariant& shorthand )
{
    ShortHandPropagator visitor;
    boost::apply_visitor( visitor, shorthand );
}

} // namespace Ed