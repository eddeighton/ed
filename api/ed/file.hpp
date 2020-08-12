#ifndef EDFILE_11_05_2013
#define EDFILE_11_05_2013

#include "node.hpp"

#include <boost/filesystem.hpp>
#include <boost/optional.hpp>

#include <map>
#include <list>
#include <vector>
#include <string>
#include <regex>

namespace Ed
{

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
class EdNode
{
    friend class File;
public:
    typedef std::vector< EdNode* > PtrVector;

    EdNode() : m_pParent( 0U ) {}
    virtual ~EdNode();
    EdNode( const Node& n, EdNode* _pParent = 0U );
    EdNode( const EdNode& n, EdNode* _pParent = 0U );

    const PtrVector& getChildren()  const { return m_children; }
    const Statement& getStatement() const { return m_statement; }
    const EdNode* getParent()       const { return m_pParent; }

    Statement& getStatement() { return m_statement; }
    PtrVector& getChildren()  { return m_children; }

    bool match( const std::regex& reg ) const;
    void toNode( Node& n ) const;
    void toNode( Node& n, const std::regex& reg ) const;

    void expandShorthand();
    void removeTypes();

private:
    void print( std::ostream& os, int iIndent ) const;

protected:
    PtrVector m_children;
    Statement m_statement;
    EdNode* m_pParent;
};

bool operator==( const EdNode& left, const EdNode& right );
inline const Statement& getStatement( const EdNode& node )              { return node.getStatement(); }
inline EdNode::PtrVector::const_iterator begin( const EdNode& node )    { return node.getChildren().begin(); }
inline EdNode::PtrVector::const_iterator end( const EdNode& node )      { return node.getChildren().end(); }
inline bool isEmpty( const EdNode& node )                               { return node.getChildren().empty(); }


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
class FileSystem
{
public:
    virtual boost::filesystem::path load( const std::string& strRelativePath, std::string& strContents )=0;
};

class BasicFileSystem : public FileSystem
{
public:
    virtual boost::filesystem::path load( const std::string& strRelativePath, std::string& strContents );
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
class File : public EdNode
{
public:
    typedef boost::shared_ptr< File > Ptr;
    typedef std::map< const EdNode*, Ptr > PtrMap;

    File( FileSystem& filesystem, const std::string& strPath );

    const boost::filesystem::path& getPath() const { return m_filePath; }

private:
    void inheritNode( EdNode* pNode );
    void inheritStatement( EdNode* pNode );
    boost::optional< const EdNode& > lookupType( const EdNode* pNode,
        const TypeList::const_iterator i, const TypeList::const_iterator iEnd );
    boost::optional< const EdNode& > locateNodeToInherit( EdNode* pNode );

protected:
    FileSystem& m_filesystem;
    Node::Vector m_originals;
    boost::filesystem::path m_filePath;
    PtrMap m_inheritedFiles;
};

std::string saveNodeToFile( const std::string& strFile, const Node& node );

bool operator==( const File& left, const File& right );

}


#endif //EDFILE_11_05_2013
