#include "ed/file.hpp"

#include "ed/node.hpp"
#include "ed/shorthand.hpp"
#include "ed/tag.hpp"
#include "ed/fileRef.hpp"
#include "ed/declarator.hpp"
#include "ed/override.hpp"
#include "ed/serialise.hpp"

#include "common/assert_verify.hpp"
#include "common/file.hpp"
#include "common/stl.hpp"

#include <fstream>
#include <iterator>
#include <regex>

namespace Ed
{

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
EdNode::EdNode( const EdNode& n, EdNode* _pParent )
    : m_statement( n.getStatement() )
    , m_pParent( _pParent )
{
    for( auto i = n.m_children.begin(), iEnd = n.m_children.end(); i != iEnd; ++i )
    {
        m_children.push_back( new EdNode( **i, this ) );
    }
}

EdNode::EdNode( const Node& n, EdNode* _pParent )
    : m_statement( n.statement )
    , m_pParent( _pParent )
{
    for( auto i = n.children.begin(), iEnd = n.children.end(); i != iEnd; ++i )
    {
        m_children.push_back( new EdNode( *i, this ) );
    }
}

EdNode::~EdNode()
{
    generics::deleteAndClear( m_children );
}

bool EdNode::match( const std::regex& reg ) const
{
    return std::regex_match( getStatement().getIdentifier(), reg );
}

void EdNode::toNode( Node& n ) const
{
    n.statement = getStatement();
    for( auto i = m_children.begin(), iEnd = m_children.end(); i != iEnd; ++i )
    {
        Ed::Node childNode;
        n.children.push_back( childNode );
        ( *i )->toNode( n.children.back() );
    }
}

void EdNode::toNode( Node& n, const std::regex& reg ) const
{
    n.statement = getStatement();
    for( auto i = m_children.begin(), iEnd = m_children.end(); i != iEnd; ++i )
    {
        if( ( *i )->match( reg ) )
        {
            Ed::Node childNode;
            n.children.push_back( childNode );
            ( *i )->toNode( n.children.back(), reg );
        }
    }
}

bool operator==( const EdNode& left, const EdNode& right )
{
    bool bEqual = false;
    if( left.getStatement() == right.getStatement() && left.getChildren().size() == right.getChildren().size() )
    {
        auto i = left.getChildren().begin(), iEnd = left.getChildren().end(), j = right.getChildren().begin();
        for( ; i != iEnd && **i == **j; ++i, ++j )
            ;
        bEqual = ( i == iEnd ) ? true : false;
    }
    return bEqual;
}

struct TagConvertVisitorIndex : boost::static_visitor< boost::optional< unsigned int > >
{
    boost::optional< unsigned int > operator()( unsigned int uiIndex ) const { return uiIndex; }
    template < class T >
    boost::optional< unsigned int > operator()( const T& other ) const
    {
        return {};
    }
};

void EdNode::expandShorthand()
{
    bool bUsedIndexZero = false;

    for( auto i = m_children.begin(), iEnd = m_children.end(); i != iEnd; ++i )
    {
        EdNode* pNode = *i;

        if( pNode->m_statement.tagList )
        {
            const unsigned int uiIndex = pNode->m_statement.tagList.get().getShorthandIndex();
            if( uiIndex != TagList::NO_INDEX )
            {
                // we have a shorthand index...
                if( getStatement().shorthand )
                {
                    if( boost::optional< const Ed::ShorthandVariant& > shOpt
                        = getStatement().shorthand.get().get( uiIndex ) )
                    {
                        ShorthandVariant newShorthandVar( shOpt.get() ); // copy the variant
                        propagate( newShorthandVar ); // propagate it - ( this propagates all parts recursively )

                        Shorthand newShorthand; // create new shorthand with the variant
                        newShorthand.children.push_back( newShorthandVar );
                        boost::optional< Shorthand > newShorthandOpt( newShorthand );

                        overrideAndExtend(
                            pNode->m_statement.shorthand,
                            newShorthandOpt ); // now override and extend the original child shorthand with it

                        pNode->m_statement.shorthand = newShorthandOpt; // finally set the child shorthand

                        if( uiIndex == 0U )
                            bUsedIndexZero = true;
                    }
                }
            }
        }

        pNode->expandShorthand();
    }

    if( bUsedIndexZero )
        m_statement.shorthand.reset();
}

void EdNode::removeTypes()
{
    PtrVector temp;
    temp.swap( m_children );
    for( auto i = temp.begin(), iEnd = temp.end(); i != iEnd; ++i )
    {
        if( ( *i )->m_statement.declarator.identifier )
        {
            m_children.push_back( *i );
            ( *i )->removeTypes();
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
boost::filesystem::path BasicFileSystem::load( const std::string& strRelativePath, std::string& strContents )
{
    const boost::filesystem::path filePath
        = boost::filesystem::edsCannonicalise( boost::filesystem::absolute( strRelativePath ) );

    VERIFY_RTE_MSG( boost::filesystem::exists( filePath ), "Could not locate file: " << filePath );
    std::string str;
    boost::filesystem::loadAsciiFile( filePath, strContents );

    return filePath;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
File::File( FileSystem& filesystem, const std::string& strPath )
    : m_filesystem( filesystem )
{
    std::string str;
    m_filePath = m_filesystem.load( strPath, str );

    // parse the file
    std::ostringstream osError;
    ParseResult        result = parse( str, m_originals, osError );
    if( !result.first )
    {
        THROW_RTE( "Error during parsing: " << m_filePath << " : " << osError.str() );
    }
    else if( result.second.base() != str.end() )
    {
        THROW_RTE( "Parsing failed to process entire file: " << m_filePath << "\n" << osError.str() );
    }

    for( auto i = m_originals.begin(), iEnd = m_originals.end(); i != iEnd; ++i )
        m_children.push_back( new EdNode( *i, this ) );

    // process inheritance
    File* pThisFile = this;
    std::for_each(
        m_children.begin(), m_children.end(), [ pThisFile ]( EdNode* pNode ) { pThisFile->inheritNode( pNode ); } );
}

void File::inheritNode( EdNode* pNode )
{
    inheritStatement( pNode );
    File* pThisFile = this;
    std::for_each( pNode->m_children.begin(), pNode->m_children.end(),
                   [ pThisFile ]( EdNode* pChildNode ) { pThisFile->inheritNode( pChildNode ); } );
}

boost::optional< const EdNode& >
File::lookupType( const EdNode* pNode, const TypeList::const_iterator iter, const TypeList::const_iterator iterEnd )
{
    VERIFY_RTE( pNode );

    boost::optional< const EdNode& > result;

    if( iter == iterEnd )
    {
        result = *pNode;
    }
    else
    {
        for( auto i = pNode->m_children.begin(), iEnd = pNode->m_children.end(); i != iEnd; ++i )
        {
            if( !( *i )->getStatement().declarator.identifier && !( *i )->getStatement().declarator.typeList.empty() )
            {
                if( ( *i )->getStatement().declarator.typeList.back() == *iter )
                {
                    if( ( result = lookupType( *i, iter + 1, iterEnd ) ) )
                        break;
                }
            }
        }

        if( !result && pNode->getParent() )
            result = lookupType( pNode->getParent(), iter, iterEnd );

        if( !result )
        {
            // attempt searching inherited file
            auto iFind = m_inheritedFiles.find( pNode );
            if( iFind != m_inheritedFiles.end() )
                result = lookupType( iFind->second.get(), iter, iterEnd );
        }
    }

    return result;
}

void File::inheritStatement( EdNode* pNode )
{
    if( pNode->m_statement.declarator.identifier )
    {
        if( pNode->m_statement.declarator.typeList.empty() )
        {
            // do regular inheritance using directive if it exists
            if( boost::optional< const EdNode& > n = locateNodeToInherit( pNode ) )
                overrideAndExtend( n.get(), *pNode );
        }
        else
        {
            FileRef::Vector files;
            VERIFY_RTE_MSG( !pNode->m_statement.getFileRefs( files ),
                            "Node cannot have both type and inheritance directive: " << pNode->m_statement );
            // inherit using the type
            VERIFY_RTE( pNode->getParent() );
            TypeList&                        typeList = pNode->m_statement.declarator.typeList;
            boost::optional< const EdNode& > n = lookupType( pNode->getParent(), typeList.begin(), typeList.end() );
            VERIFY_RTE_MSG( n, "Failed to lookup type for: " << pNode->m_statement );
            overrideAndExtend( n.get(), *pNode );
            typeList.clear(); // strip the typelist
        }
    }
    else
    {
        if( !pNode->m_statement.declarator.typeList.empty() )
        {
            // do regular inheritance using directive if it exists
            if( boost::optional< const EdNode& > n = locateNodeToInherit( pNode ) )
                overrideAndExtend( n.get(), *pNode );
        }
        else
        {
            THROW_RTE( "Node must have either identifier or type or both: " << pNode->m_statement );
        }
    }
}

struct PathConstructorVisitor : boost::static_visitor< std::string >
{
    std::string operator()( const std::string& str ) const { return str; }
    std::string operator()( FileActionType actionType ) const
    {
        std::string strResult;
        switch( actionType )
        {
            case eFileUp:
                strResult = "..";
                break;
            case TOTAL_FILEREF_ACTION_TYPES:
            default:
                THROW_RTE( "Invalid file action type" );
                break;
        }
        return strResult;
    }
};

void getFileRefPath( const FileRef& fileRef, const boost::filesystem::path& current, boost::filesystem::path& result )
{
    result = current;
    result.remove_filename();
    for( auto i = fileRef.begin(), iEnd = fileRef.end(); i != iEnd; ++i )
    {
        std::string s = boost::apply_visitor( PathConstructorVisitor(), *i );
        result.append( s.begin(), s.end() );
    }
    result = result.replace_extension( ".ed" );
}

struct LocateRefFromRootVisitor : boost::static_visitor< boost::optional< const EdNode& > >
{
    const EdNode::PtrVector& nodes;
    LocateRefFromRootVisitor( const EdNode::PtrVector& _nodes )
        : nodes( _nodes )
    {
    }

    boost::optional< const EdNode& > operator()( const Identifier& identifier ) const
    {
        boost::optional< const EdNode& > result;
        // return the first match
        for( auto i = nodes.begin(), iEnd = nodes.end(); i != iEnd; ++i )
        {
            if( ( *i )->getStatement().declarator.identifier
                && ( *i )->getStatement().declarator.identifier.get() == identifier )
            {
                result = **i;
                break;
            }
        }
        return result;
    }

    boost::optional< const EdNode& > operator()( const RefActionType& ) const
    {
        THROW_RTE( "Invalid use of ref . in inheritance directive" );
        return {};
    }

    boost::optional< const EdNode& > operator()( const Ref& ) const
    {
        THROW_RTE( "Invalid use of ref . in inheritance directive" );
        return {};
    }
};

boost::optional< const EdNode& > getRefNode( const EdNode::PtrVector& nodes, Reference::const_iterator i,
                                             Reference::const_iterator iEnd )
{
    boost::optional< const EdNode& > result;
    if( i != iEnd )
    {
        if( ( result = boost::apply_visitor( LocateRefFromRootVisitor( nodes ), *i ) ) )
        {
            ++i;
            if( i != iEnd )
            {
                result = getRefNode( result.get().getChildren(), i, iEnd );
            }
        }
    }
    return result;
}

boost::optional< const EdNode& > File::locateNodeToInherit( EdNode* pNode )
{
    boost::optional< const EdNode& > nodeToInherit;

    FileRef::Vector files;
    if( pNode->m_statement.getFileRefs( files ) )
    {
        boost::filesystem::path targetFile;
        getFileRefPath( files.back(), m_filePath, targetFile );

        // load the file and recursively process it
        File::Ptr pFile( new File( m_filesystem, targetFile.string() ) );
        m_inheritedFiles.insert( std::make_pair( pNode, pFile ) );
        Ref::Vector refs;
        if( pNode->m_statement.getRefs( refs ) )
        {
            const Ref& nodeRef = refs.back();
            if( !nodeRef.empty() )
                nodeToInherit = getRefNode( pFile->getChildren(), nodeRef.front().begin(), nodeRef.front().end() );
            VERIFY_RTE_MSG( nodeToInherit, "Failed to resolve inheritance reference for: " << pNode->m_statement );
        }
        else
        {
            nodeToInherit = *( pFile.get() );
        }
    }

    return nodeToInherit;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
std::string saveNodeToFile( const std::string& strFile, const Node& node )
{
    const boost::filesystem::path p = boost::filesystem::edsCannonicalise( boost::filesystem::absolute( strFile ) );
    const std::string             strActualFilePath = p.generic_string().c_str();

    std::ofstream of( strActualFilePath.c_str(), std::ofstream::out | std::ofstream::trunc );
    if( !of.good() )
    {
        THROW_RTE( "Failed to create file: " << strActualFilePath );
    }

    for( auto i = node.children.begin(), iEnd = node.children.end(); i != iEnd; ++i )
        of << *i;

    return strActualFilePath;
}

bool operator==( const File& left, const File& right )
{
    Node n1, n2;
    left.toNode( n1 );
    right.toNode( n2 );
    return n1 == n2;
}

} // namespace Ed
