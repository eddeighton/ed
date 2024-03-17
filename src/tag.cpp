#include "ed/tag.hpp"

namespace Ed
{

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//Tag
ParseResult parse( const std::string& strInput, Tag& tag, std::ostream& errorStream )
{
    return parse_impl< TagGrammar >( strInput, tag, errorStream );
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//TagList
TagList::TagList( const Tag& type1 )
{
    push_back( type1 );
}

TagList::TagList( const Tag& type1, const Tag& type2 )
{
    push_back( type1 );
    push_back( type2 );
}

TagList::TagList( const Tag& type1, const Tag& type2, const Tag& type3 )
{
    push_back( type1 );
    push_back( type2 );
    push_back( type3 );
}

struct TagConvertVisitorIndex : boost::static_visitor< boost::optional< unsigned int > >
{
    boost::optional< unsigned int > operator()( unsigned int uiIndex ) const
    {
        return uiIndex;
    }
    template< class T >
    boost::optional< unsigned int > operator()( const T& ) const
    {
        return boost::optional< unsigned int >();
    }
};

unsigned int TagList::getShorthandIndex() const
{
    unsigned int uiIndex = NO_INDEX;
    for( TagList::const_iterator i = begin(),
        iEnd = end(); i!=iEnd; ++i )
    {
        if( boost::optional< unsigned int > ui =
            boost::apply_visitor( TagConvertVisitorIndex(), *i ) )
            uiIndex = ui.get();
    }
    return uiIndex;
}

ParseResult parse( const std::string& strInput, TagList& tagList, std::ostream& errorStream )
{
    return parse_impl< TagListGrammar >( strInput, tagList, errorStream );
}
}