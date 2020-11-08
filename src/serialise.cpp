#include "ed/serialise.hpp"

#include "ed/identifier.hpp"
#include "ed/type.hpp"
#include "ed/fileRef.hpp"
#include "ed/ref.hpp"
#include "ed/expression.hpp"
#include "ed/code.hpp"
#include "ed/declarator.hpp"
#include "ed/shorthand.hpp"
#include "ed/statement.hpp"
#include "ed/node.hpp"
#include "ed/file.hpp"

#include <boost/tokenizer.hpp>

#include <string>
#include <iterator>

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
namespace Ed
{

OutStream& operator<<( OutStream& os, const Identifier& id )
{
	return os << std::string( id );
}

OutStream& operator<<( OutStream& os, const Type& type )
{
    return os << std::string( type );
}
OutStream& operator<<( OutStream& os, const TypeList& ns )
{
    for( TypeList::const_iterator j = ns.begin(),
        jNext = ns.begin(),
        jEnd = ns.end(); j!=jEnd; ++j )
    {
        ++jNext;
        os << *j;
        if( jNext != jEnd )
            os << TYPELIST_DELIMITER;
    }
    return os;
}


struct FileVariantPrinter : boost::static_visitor< void >
{
    OutStream& os;
    FileVariantPrinter( OutStream& _os ) : os( _os ) {}
    void operator()( const std::string& str ) const
    {
        os << str;
    }
    void operator()( const FileActionType& type ) const
    {
        switch( type )
        {
        case eFileUp:
            os << PATH_UP;
            break;
        default:
            ASSERT( false );
            break;
        }
    }
};

OutStream& operator<<( OutStream& os, const FileRef& fileRef )
{
    FileVariantPrinter printer( os );
    for( std::vector< FileVariant >::const_iterator
        i = fileRef.begin(),
        iEnd = fileRef.end(); i!=iEnd; ++i )
    {
        if( i == fileRef.begin() && i->which() == 0u )
            os << PATH_UP;
        else
        {
            os << PATH_DELIMITER;
            boost::apply_visitor( printer, *i );
        }
    }
    return os;
}


struct RefVariantPrinter : boost::static_visitor< void >
{
    OutStream& os;
    mutable bool bLastWasName;
    RefVariantPrinter( OutStream& _os ) : os( _os ), bLastWasName( false ) {}

    void operator()( const std::string& str ) const
    {
        if( bLastWasName )
            os << REF_DELIMITER;
        os << str;
        bLastWasName = true;
    }

    void operator()( const RefActionType& type ) const
    {
        switch( type )
        {
        case eRefUp:
            os << REF_DELIMITER;
            bLastWasName = false;
            break;
        default:
            ASSERT( false );
            break;
        }
    }

    void operator()( const Ref& ref ) const
    {
        if( bLastWasName )
            os << REF_DELIMITER;
        bLastWasName = true;

        os << REF_BRANCH_OPEN; 
        for( auto j = ref.begin(), jEnd = ref.end(); j!=jEnd; ++j )
        {
            if( j != ref.begin() ) os << REF_BRANCH_DELIMITER;
            
            //recurse without the dot at the start
            RefVariantPrinter printer( os );
            for( std::vector< RefVariant >::const_iterator
                i = j->begin(),
                iEnd = j->end(); i!=iEnd; ++i )
            {
                boost::apply_visitor( printer, *i );
            }
        }
        os << REF_BRANCH_CLOSE;
    }
};

OutStream& operator<<( OutStream& os, const Reference& ref )
{
    RefVariantPrinter printer( os );
    os << REF_DELIMITER;
    for( std::vector< RefVariant >::const_iterator
        i = ref.begin(),
        iEnd = ref.end(); i!=iEnd; ++i )
    {
        boost::apply_visitor( printer, *i );
    }
    return os;
}

OutStream& operator<<( OutStream& os, const Ref& ref )
{
    for( Ref::const_iterator j = ref.begin(),
        jEnd = ref.end(); j!=jEnd; ++j )
    {
        if( j != ref.begin() ) os << ASSOCIATION_DELIMITER;
        os << *j;
    }
    return os;
}

OutStream& operator<<( OutStream& os, const Expression& expression )
{
    return os << static_cast< std::string >( expression ) << EXPRESSION_DELIMITER;
}

OutStream& operator<<( OutStream& os, const Code& code )
{
    typedef boost::tokenizer< boost::char_separator< char > > TokenizerType;
    const boost::char_separator< char > sep( "\n" );
    TokenizerType tok( code.code, sep );
    for( TokenizerType::const_iterator tok_iter = tok.begin();
        tok_iter != tok.end(); ++tok_iter )
    {
        os << Indent() << *tok_iter << EndLine();
    }
    return os;// << code.code;
}

OutStream& operator<<( OutStream& os, const Declarator& declarator )
{
    if( declarator.identifier && !declarator.typeList.empty() )
        os << declarator.identifier.get() << " " << declarator.typeList;
    else if( declarator.identifier )
        os << declarator.identifier.get();
    else if( !declarator.typeList.empty() )
        os << declarator.typeList;
    return os;
}

struct ArgumentVariantPrinter : boost::static_visitor< OutStream& >
{
    OutStream& os;
    ArgumentVariantPrinter( OutStream& _os );
    OutStream& operator()( const FileRef& fileRef ) const;
    OutStream& operator()( const Ref& ref ) const;
    OutStream& operator()( const Code& code ) const;
    OutStream& operator()( const Expression& expression ) const;
    OutStream& operator()( const double& d ) const;
    OutStream& operator()( const bool& b ) const;
};

ArgumentVariantPrinter::ArgumentVariantPrinter( OutStream& _os )
    : os( _os )
{
}
OutStream& ArgumentVariantPrinter::operator()( const FileRef& fileRef ) const
{
    return os << fileRef;
}
OutStream& ArgumentVariantPrinter::operator()( const Ref& ref ) const
{
    return os << ref;
}
OutStream& ArgumentVariantPrinter::operator()( const Code& code ) const
{
    if( std::find( code.code.begin(), code.code.end(), '\n' ) == code.code.end() )
        return os << CODE_OPEN << code.code << CODE_CLOSE;
    else
        return os << EndLine() << Indent() << CODE_OPEN << EndLine() << code << EndLine() << Indent() << CODE_CLOSE;
}
OutStream& ArgumentVariantPrinter::operator()( const Expression& expression ) const
{
    return os << expression;
}
OutStream& ArgumentVariantPrinter::operator()( const double& d ) const
{
    return os << d;
}
OutStream& ArgumentVariantPrinter::operator()( const bool& b ) const
{
    return os << std::boolalpha << b;
}

OutStream& operator<<( OutStream& os, const ArgumentVariant& arg )
{
    ArgumentVariantPrinter visitor( os );
    boost::apply_visitor( visitor, arg );
    return os;
}

struct ShortHandPrinter : public boost::static_visitor< OutStream& >
{
    OutStream& os;
    ShortHandPrinter( OutStream& _os)
        : os( _os )
    {
    }

    OutStream& operator()( const Shorthand& shortHand ) const
    {
        return os << shortHand;
    }

    OutStream& operator()( const ArgumentVariant& var ) const
    {
        return os << var;
    }
};

OutStream& operator<<( OutStream& os, const Shorthand& shortHand )
{
    os << SH_OPEN;
    ShortHandPrinter visitor( os );
    for( std::vector< ShorthandVariant >::const_iterator
        i = shortHand.children.begin(),
        iEnd = shortHand.children.end(); i!=iEnd; ++i )
    {
        if( i != shortHand.children.begin() ) os << SH_DELIMITER;
        boost::apply_visitor( visitor, *i );
    }
    os << SH_CLOSE;
    return os;
}

OutStream& operator<<( OutStream& os, const TagList& ns )
{
    if( !ns.empty() )
    {
        os << TAGS_OPEN;
        for( TagList::const_iterator j = ns.begin(),
            jNext = ns.begin(),
            jEnd = ns.end(); j!=jEnd; ++j )
        {
            ++jNext;
            os << *j;
            if( jNext != jEnd )
                os << TAGS_DELIMITER;
        }
        os << TAGS_CLOSE;
    }
    return os;
}

OutStream& operator<<( OutStream& os, const Statement& statement )
{
    os << statement.declarator;
    if( statement.tagList ) os << statement.tagList.get();
    if( statement.shorthand ) os << statement.shorthand.get();
    return os;
}

void structuredPrint( OutStream& os, const Node& value )
{
    os << Indent() << getStatement( value ) << EndLine();
    if( !isEmpty( value ) )
    {
        os << Push( BODY_OPEN );
        std::for_each( begin( value ), end( value ),
            [ &os ]( const Node& n ) { structuredPrint( os, n ); } );
        os << Pop( BODY_CLOSE );
    }
}

OutStream& operator<<( OutStream& os, const Node& node )
{
    structuredPrint( os, node );
    return os;
}

void structuredPrintEdNode( OutStream& os, const EdNode* pValue )
{
    os << Indent() << getStatement( *pValue ) << EndLine();
    if( !isEmpty( *pValue ) )
    {
        os << Push( BODY_OPEN );
        std::for_each( begin( *pValue ), end( *pValue ),
            [ &os ]( const EdNode* pNode ) { structuredPrintEdNode( os, pNode ); } );
        os << Pop( BODY_CLOSE );
    }
}

OutStream& operator<<( OutStream& os, const EdNode& node )
{
    structuredPrintEdNode( os, &node );
    return os;
}

OutStream& operator<<( OutStream& os, const File& ed )
{
    for( EdNode::PtrVector::const_iterator i = ed.getChildren().begin(),
        iEnd = ed.getChildren().end(); i!=iEnd; ++i )
        os << **i;
    return os;
}

}
