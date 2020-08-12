#ifndef NODEIO_13_04_2014
#define NODEIO_13_04_2014

#include "node.hpp"
#include "shorthandio.hpp"

#include <map>
#include <string>

namespace Ed
{

template< class TNodeType >
class NodeArchive
{
public:
    typedef std::multimap< Identifier, TNodeType*, typename Identifier::Compare > NodeMap;
    typedef std::map< Identifier, int, typename Identifier::Compare > IterMap;
    typedef typename NodeMap::const_iterator Iter;
    typedef std::pair< Iter, Iter > Range;

public:
    NodeArchive( TNodeType& node )
    :   m_node( node )
    {
        for( auto i = m_node.children.begin(),
            iEnd = m_node.children.end(); i!=iEnd; ++i )
        {
            if( i->statement.declarator.identifier )
            {
                m_nodeMap.insert( std::make_pair( i->statement.declarator.identifier.get(), &*i ) );
            }
        }
    }

    boost::optional< TNodeType& > getNext( const Identifier& identifier )
    {
        boost::optional< TNodeType& > result;
        typename NodeMap::const_iterator iFirst = m_nodeMap.lower_bound( identifier );
        typename NodeMap::const_iterator iLast = m_nodeMap.upper_bound( identifier );
        if( iFirst != iLast )
        {
            const int iIndex = m_iterMap[ identifier ]++;
            if( std::distance( iFirst, iLast ) > iIndex )
            {
                std::advance( iFirst, iIndex );
                result = *iFirst->second;
            }
        }
        return result;
    }

    boost::optional< TNodeType& > get( const Identifier& identifier ) const
    {
        boost::optional< TNodeType& > result;
        typename NodeMap::const_iterator iFirst = m_nodeMap.lower_bound( identifier );
        typename NodeMap::const_iterator iLast = m_nodeMap.upper_bound( identifier );
        if( iFirst != iLast )
        {
            VERIFY_RTE( std::distance( iFirst, iLast ) == 1 );
            result = *iFirst->second;
        }
        return result;
    }

    Range getAll( const Identifier& identifier ) const
    {
        return std::make_pair( m_nodeMap.lower_bound( identifier ),
                m_nodeMap.upper_bound( identifier ) );
    }

protected:
    TNodeType& m_node;
    NodeMap m_nodeMap;
    IterMap m_iterMap;
};

template< class T >
class MutableNodeArchive : public NodeArchive< T >
{
public:
    MutableNodeArchive( T& node ) : NodeArchive< T >( node ) { }

    T& add( const Identifier& identifier )
    {
        NodeArchive< T >::m_node.children.push_back( T( ( Statement( ( Declarator( identifier ) ) ) ) ) );
        NodeArchive< T >::m_nodeMap.insert( std::make_pair( identifier, &(NodeArchive< T >::m_node.children.back()) ) );
        return NodeArchive< T >::m_node.children.back();
    }

    T& getOrAdd( const Identifier& identifier )
    {
        boost::optional< T& > result = NodeArchive< T >::get( identifier );
        if( !result )
            result = add( identifier );
        VERIFY_RTE( result );
        return result.get();
    }

    void addTags( const TagList& tags )
    {
        if( NodeArchive< T >::m_node.statement.tagList )
            std::copy( tags.begin(), tags.end(), std::back_inserter( NodeArchive< T >::m_node.statement.tagList.get() ) );
        else
            NodeArchive< T >::m_node.statement.tagList = tags;
    }
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
class ONodeStream : public MutableNodeArchive< Node >
{
public:
    ONodeStream( Node& node ) : MutableNodeArchive< Node >( node )
    {}
};

template< class T, class TIdent >
void subtreeOpt( ONodeStream& os, const TIdent& ident, const T& data )
{
    if( boost::optional< Node& > n = os.get( ident ) )
    {
        ONodeStream o( n.get() );
        o << data;
    }
}

template< class T, class TIdent >
void subtree( ONodeStream& os, const TIdent& ident, const T& data )
{
    ONodeStream o( os.getOrAdd( ident ) );
    o << data;
}

template< class T, class TIdent >
void subtree( ONodeStream& os, const TIdent& ident, const T& data, const TagList& tags )
{
    ONodeStream o( os.getOrAdd( ident ) );
    o.addTags( tags );
    o << data;
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
class INodeStream : public NodeArchive< const Node >
{
public:
    INodeStream( const Node& node ) : NodeArchive< const Node >( node )
    {}
};

template< class T, class TIdent >
void subtreeOpt( INodeStream& is, const TIdent& ident, T& data )
{
    if( boost::optional< const Node& > n = is.get( ident ) )
    {
        INodeStream i( n.get() );
        i >> data;
    }
}

template< class T, class TIdent >
void subtree( INodeStream& is, const TIdent& ident, T& data )
{
    boost::optional< const Node& > n = is.get( ident );
    VERIFY_RTE_MSG( n, "Failed to locate node: " << ident << "\n" )
    INodeStream i( n.get() );
    i >> data;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
template< class T >
class Entry
{
public:
    template< class Ident >
    Entry( const Ident& identity, T& value )
        :   m_identity( identity ),
            m_value( value )
    {}
    const Identifier m_identity;
    T& m_value;
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
template< class T >
class EntryUnique
{
public:
    template< class Ident >
    EntryUnique( const Ident& identity, T& value )
        :   m_identity( identity ),
            m_value( value )
    {}
    const Identifier m_identity;
    T& m_value;
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/**
 * \class EntryOpt
 *
 * For optional entries where if not present the value remains unmodified
 * Example:
 *
        is >> Ed::EntryOpt< Ed::Expression >( "pre", expPrefix );

 */
template< class T >
class EntryOpt
{
public:
    template< class Ident >
    EntryOpt( const Ident& identity, T& value )
        :   m_identity( identity ),
            m_value( value )
    {}
    const Identifier m_identity;
    T& m_value;
};

template< class T, class IdentType >
inline EntryOpt< T > makeEntryOpt( const IdentType& ident, T& value )
{
    return EntryOpt< T >( ident, value );
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
template< class T >
inline ONodeStream& operator<<( ONodeStream& os, const Entry< const T >& entry )
{
    Node& n = os.add( entry.m_identity );
    OShorthandStream o( n.getShorty() );
    o << entry.m_value;
    return os;
}

template< class T >
inline ONodeStream& operator<<( ONodeStream& os, const EntryUnique< const T >& entry )
{
    Node& n = os.getOrAdd( entry.m_identity );
    //reset the shorthand to ensure unique
    n.statement.shorthand = (Ed::Shorthand());
    OShorthandStream o( n.getShorty() );
    o << entry.m_value;
    return os;
}

template< class T >
inline ONodeStream& operator<<( ONodeStream& os, const EntryOpt< const T >& entry )
{
    if( boost::optional< Node& > n = os.get( entry.m_identity ) )
    {
        OShorthandStream o( n.get().getShorty() );
        o << entry.m_value;
    }
    return os;
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

template< class T >
inline INodeStream& operator>>( INodeStream& is, const Entry< T >& entry )
{
    boost::optional< const Node& > n = is.getNext( entry.m_identity );
    VERIFY_RTE_MSG( n, "Failed to locate node: " << entry.m_identity );
    boost::optional< const Shorthand& > shorthand = n.get().getShorty();
    VERIFY_RTE_MSG( shorthand, "Failed to find shorthand in node: " << entry.m_identity );
    IShorthandStream s( shorthand.get() );
    s >> entry.m_value;
    return is;
}

template< class T >
inline INodeStream& operator>>( INodeStream& is, const EntryUnique< T >& entry )
{
    boost::optional< const Node& > n = is.get( entry.m_identity );
    VERIFY_RTE_MSG( n, "Failed to locate node: " << entry.m_identity );
    boost::optional< const Shorthand& > shorthand = n.get().getShorty();
    VERIFY_RTE_MSG( shorthand, "Failed to find shorthand in node: " << entry.m_identity );
    IShorthandStream s( shorthand.get() );
    s >> entry.m_value;
    return is;
}

template< class T >
inline INodeStream& operator>>( INodeStream& is, const EntryOpt< T >& entry )
{
    if( boost::optional< const Node& > n = is.get( entry.m_identity ) )
    {
        if( boost::optional< const Shorthand& > shorthand = n.get().getShorty() )
        {
            IShorthandStream s( shorthand.get() );
            s >> entry.m_value;
        }
    }
    return is;
}

}

#endif //NODEIO_13_04_2014
