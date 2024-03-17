#ifndef ED_SERIALISE_29_03_2014
#define ED_SERIALISE_29_03_2014

#include "argument.hpp"

#include "common/assert_verify.hpp"

#include <ostream>
#include <vector>
#include <iterator>
#include <algorithm>

namespace Ed
{

class Identifier;
class Type;
class TypeList;
class FileRef;
class Reference;
class Ref;
class Expression;
class Code;
class Declarator;
class Shorthand;
class TagList;
class Statement;
class Node;
class EdNode;
class File;

class OutStream
{
public:
    OutStream( std::ostream& os, int iIndentSize = 4, int iIndent = 0 )
        : m_ostream( os )
        , m_iIndent( iIndent )
        , m_iIndentSize( iIndentSize )
    {
    }

    std::ostream& get() { return m_ostream; }

    void inc() { ++m_iIndent; }
    void dec() { --m_iIndent; }

    OutStream& indent()
    {
        std::fill_n( std::ostream_iterator< char >( m_ostream, nullptr ), m_iIndent * m_iIndentSize, ' ' );
        return *this;
    }

    OutStream& carriageReturn()
    {
        m_ostream << '\n';
        return *this;
    }

private:
    std::ostream& m_ostream;
    int           m_iIndent;
    int           m_iIndentSize;
};

struct EndLine
{
};
inline OutStream& operator<<( OutStream& os, const EndLine& )
{
    return os.carriageReturn();
}

struct Indent
{
};
inline OutStream& operator<<( OutStream& os, const Indent& )
{
    return os.indent();
}

struct Push
{
    char c;
    Push( char _c ) { c = _c; }
};
inline OutStream& operator<<( OutStream& os, const Push& push )
{
    os.indent();
    os.get() << push.c;
    os.inc();
    return os.carriageReturn();
}

struct Pop
{
    char c;
    Pop( char _c ) { c = _c; }
};
inline OutStream& operator<<( OutStream& os, const Pop& pop )
{
    os.dec();
    os.indent();
    os.get() << pop.c;
    return os.carriageReturn();
}

OutStream& operator<<( OutStream& os, const Identifier& id );
OutStream& operator<<( OutStream& os, const Type& ns );
OutStream& operator<<( OutStream& os, const TypeList& ns );
OutStream& operator<<( OutStream& os, const FileRef& fileRef );
OutStream& operator<<( OutStream& os, const Reference& reference );
OutStream& operator<<( OutStream& os, const Ref& fileRef );
OutStream& operator<<( OutStream& os, const Expression& code );
OutStream& operator<<( OutStream& os, const Code& code );
OutStream& operator<<( OutStream& os, const Declarator& declarator );
OutStream& operator<<( OutStream& os, const ArgumentVariant& arg );
OutStream& operator<<( OutStream& os, const Shorthand& shortHand );
OutStream& operator<<( OutStream& os, const TagList& ns );
OutStream& operator<<( OutStream& os, const Statement& statement );
OutStream& operator<<( OutStream& os, const Node& ed );
OutStream& operator<<( OutStream& os, const EdNode& ed );
OutStream& operator<<( OutStream& os, const File& ed );

template < class T >
inline OutStream& operator<<( OutStream& os, const T& data )
{
    os.get() << data;
    return os;
}

inline std::ostream& operator<<( std::ostream& os, const Identifier& v )
{
    OutStream o( os );
    o << v;
    return os;
}
inline std::ostream& operator<<( std::ostream& os, const Type& v )
{
    OutStream o( os );
    o << v;
    return os;
}
inline std::ostream& operator<<( std::ostream& os, const TypeList& v )
{
    OutStream o( os );
    o << v;
    return os;
}
inline std::ostream& operator<<( std::ostream& os, const FileRef& v )
{
    OutStream o( os );
    o << v;
    return os;
}
inline std::ostream& operator<<( std::ostream& os, const Reference& v )
{
    OutStream o( os );
    o << v;
    return os;
}
inline std::ostream& operator<<( std::ostream& os, const Ref& v )
{
    OutStream o( os );
    o << v;
    return os;
}
inline std::ostream& operator<<( std::ostream& os, const Expression& v )
{
    OutStream o( os );
    o << v;
    return os;
}
inline std::ostream& operator<<( std::ostream& os, const Code& v )
{
    OutStream o( os );
    o << v;
    return os;
}
inline std::ostream& operator<<( std::ostream& os, const Declarator& v )
{
    OutStream o( os );
    o << v;
    return os;
}
inline std::ostream& operator<<( std::ostream& os, const ArgumentVariant& v )
{
    OutStream o( os );
    o << v;
    return os;
}
inline std::ostream& operator<<( std::ostream& os, const Shorthand& v )
{
    OutStream o( os );
    o << v;
    return os;
}
inline std::ostream& operator<<( std::ostream& os, const TagList& v )
{
    OutStream o( os );
    o << v;
    return os;
}
inline std::ostream& operator<<( std::ostream& os, const Statement& v )
{
    OutStream o( os );
    o << v;
    return os;
}
inline std::ostream& operator<<( std::ostream& os, const Node& v )
{
    OutStream o( os );
    o << v;
    return os;
}
inline std::ostream& operator<<( std::ostream& os, const EdNode& v )
{
    OutStream o( os );
    o << v;
    return os;
}
inline std::ostream& operator<<( std::ostream& os, const File& v )
{
    OutStream o( os );
    o << v;
    return os;
}
} // namespace Ed

#endif // ED_SERIALISE_29_03_2014