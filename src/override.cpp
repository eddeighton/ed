#include "ed/override.hpp"

#include "ed/file.hpp"
#include "ed/node.hpp"
#include "ed/shorthand.hpp"
#include "ed/tag.hpp"
#include "ed/declarator.hpp"

#include "common/assert_verify.hpp"
#include "common/variant_utils.hpp"

namespace Ed
{

void overrideAndExtend( const Declarator& origin, Declarator& overrider )
{
    //do not want to propogate TypeList for non-type nodes
    if( origin.identifier )
    {
        if( !overrider.identifier )
            overrider.identifier = origin.identifier;
    }
}

void overrideAndExtend( const boost::optional< Shorthand >& origin, boost::optional< Shorthand >& overrider )
{
    if( origin && overrider )
    {
        if( origin.get().children.size() > overrider.get().children.size() )
        {
            for( Shorthand::ShortHandVariantVector::const_iterator 
                i = origin.get().children.begin() + overrider.get().children.size(),
                iEnd = origin.get().children.end(); i!=iEnd; ++i )
                overrider.get().children.push_back( *i );
        }
    }
    else if( origin )
        overrider = origin;
}

void stripTagListOfInheritance( TagList& tags )
{
    TagList copy;
    for( TagList::const_iterator 
        i = tags.begin(),
        iEnd = tags.end(); i!=iEnd; ++i )
    {
        if( !boost::apply_visitor( boost::TypeAccessor< const FileRef >(), *i ) && 
            !boost::apply_visitor( boost::TypeAccessor< const Ref >(), *i ))
            copy.push_back( *i );
    }
    tags = copy;
}

void overrideAndExtend( const boost::optional< TagList >& origin, boost::optional< TagList >& overrider )
{
    if( origin )
    {
        //strip the origins inheritance tags
        TagList originCopy = origin.get();
        stripTagListOfInheritance( originCopy );

        //then copy to the overrider
        if( overrider )
        {
            TagList temp = overrider.get();
            stripTagListOfInheritance( temp );
            overrider.get() = originCopy;
            for( TagList::const_iterator 
                i = temp.begin(),
                iEnd = temp.end(); i!=iEnd; ++i )
            {
                overrider.get().push_back( *i );
            }
        }
        else
            overrider = originCopy;
    }
    else if( overrider )
    {
        stripTagListOfInheritance( overrider.get() );
    }

}

void overrideAndExtend( const Statement& origin, Statement& overrider )
{
    overrideAndExtend( origin.declarator, overrider.declarator );
    overrideAndExtend( origin.tagList,    overrider.tagList );
    overrideAndExtend( origin.shorthand,  overrider.shorthand );
}

struct MatchIdentifier
{
    const EdNode& cmp;
    MatchIdentifier( const EdNode& _cmp ) : cmp( _cmp ) {}
    bool operator()( const EdNode& node ) const
    {
        if( cmp.getStatement().declarator.identifier &&
            node.getStatement().declarator.identifier )
        {
            return cmp.getStatement().declarator.identifier.get() ==
                node.getStatement().declarator.identifier.get();
        }
        else if( !cmp.getStatement().declarator.identifier &&
            !node.getStatement().declarator.identifier )
        {
            return cmp.getStatement().declarator.typeList == 
                node.getStatement().declarator.typeList;
        }
        else
            return false;
    }
    bool operator()( const EdNode* pNode ) const
    {
        return this->operator()( *pNode );
    }
};

void overrideAndExtend( const EdNode& origin, EdNode& overrider )
{
    overrideAndExtend( origin.getStatement(), overrider.getStatement() );
    
    EdNode::PtrVector::iterator insertPos = overrider.getChildren().begin();
    for( EdNode::PtrVector::const_iterator
        i = origin.getChildren().begin(),
        iEnd = origin.getChildren().end();
        i!=iEnd; ++i )
    {
        EdNode::PtrVector::iterator iFind =
            std::find_if( overrider.getChildren().begin(),
                        overrider.getChildren().end(), MatchIdentifier( **i ) );
        if( iFind != overrider.getChildren().end() )
        {
            overrideAndExtend( **i, **iFind );
        }
        else
        {
            insertPos = 
                overrider.getChildren().insert( insertPos, new EdNode( **i, &overrider ) ) + 1U;
        }
    }
}

}
