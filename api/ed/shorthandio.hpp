#ifndef SHORTHAND_IO_12_04_2014
#define SHORTHAND_IO_12_04_2014

#include "shorthand.hpp"
#include "serialise.hpp"

#include "common/assert_verify.hpp"
#include "common/variant_utils.hpp"
#include "compose.hpp"

#include <vector>
#include <type_traits>

namespace Ed
{

enum StackOperation
{
    ePush,
    ePop
};

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
class OShorthandStream
{
    typedef std::vector< Shorthand* > Stack;

public:
    OShorthandStream( Shorthand& shorthand )
    {
        m_stack.push_back( &shorthand );
    }
    ~OShorthandStream()
    {
        TERMINATE_IF_NOT( m_stack.size() == 1U, "OShorthandStream incorrectly used. Remaining stack: " << m_stack.size() );
    }

    OShorthandStream& push()
    {
        VERIFY_RTE( !m_stack.empty() );
        Shorthand* pShorthand = m_stack.back();
        pShorthand->children.push_back( ( ShorthandVariant( ( Shorthand() ) ) ) );

        boost::optional< Shorthand& > opt = 
            boost::apply_visitor( boost::TypeAccessor< Shorthand >(), pShorthand->children.back() );

        m_stack.push_back( &opt.get() );

        return *this;
    }
    
    OShorthandStream& pop()
    {
        m_stack.pop_back();
        VERIFY_RTE( !m_stack.empty() );
        return *this;
    }
    
    OShorthandStream& push_back( const ArgumentVariant& data )
    {
        VERIFY_RTE( !m_stack.empty() );
        m_stack.back()->children.push_back( ( ShorthandVariant( data ) ) );
        return *this;
    }
    
    OShorthandStream& push_back( const Shorthand& data )
    {
        VERIFY_RTE( !m_stack.empty() );
        m_stack.back()->children.push_back( ( ShorthandVariant( data ) ) );
        return *this;
    }

private:
    Stack m_stack;
};

inline OShorthandStream& operator<<( OShorthandStream& os, StackOperation so )
{
    switch( so )
    {
        case ePush:   return os.push();
        case ePop:    return os.pop();
        default:
            THROW_RTE( "Invalid stack operation for OShorthandStream: " << so );
            break;
    }
}

template< class T >
inline OShorthandStream& operator<<( OShorthandStream& os, const T& data )
{
    return os.push_back( ed_cast( data )  );
}

template<>
inline OShorthandStream& operator<<( OShorthandStream& os, const Shorthand& data )
{
    return os.push_back( data );
}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
class IShorthandStream
{
    typedef Shorthand::ShortHandVariantVector::const_iterator Iter;
    typedef std::pair< Iter, Iter > IterRange;
    typedef std::vector< IterRange > Stack;

public:
    IShorthandStream( const Shorthand& shorthand )
    {
        m_stack.push_back( std::make_pair( shorthand.children.begin(), shorthand.children.end() ) );
    }

    ~IShorthandStream()
    {
        TERMINATE_IF_NOT( m_stack.size() == 1U, "OShorthandStream incorrectly used. Remaining stack: " << m_stack.size() );
    }

    IShorthandStream& push()
    {
        boost::optional< const Shorthand& > opt = 
                boost::apply_visitor( boost::TypeAccessor< const Shorthand >(), *m_stack.back().first );
        VERIFY_RTE( opt );
        m_stack.push_back( std::make_pair( opt.get().children.begin(), opt.get().children.end() ) );
        return *this;
    }
    
    IShorthandStream& pop()
    {
        VERIFY_RTE( !m_stack.empty() );
        m_stack.pop_back();
        m_stack.back().first++;
        return *this;
    }

    operator const void*() const
    {
        VERIFY_RTE( !m_stack.empty() );
        return ( m_stack.back().first != m_stack.back().second ) ? this : nullptr;
    }
    
    const ArgumentVariant& read()
    {
        VERIFY_RTE( !m_stack.empty() );
        VERIFY_RTE( m_stack.back().first != m_stack.back().second );
        boost::optional< const ArgumentVariant& > opt = 
                boost::apply_visitor( boost::TypeAccessor< const ArgumentVariant >(), *m_stack.back().first );
        ++m_stack.back().first;
        VERIFY_RTE( opt );
        return opt.get();
    }
    
private:
    Stack m_stack;
};

inline IShorthandStream& operator>>( IShorthandStream& os, StackOperation so )
{
    switch( so )
    {
        case ePush:   return os.push();
        case ePop:    return os.pop();
        default:
            THROW_RTE( "Invalid stack operation for OShorthandStream: " << so );
            break;
    }
}

template< class T >
inline IShorthandStream& operator>>( IShorthandStream& os, T& data )
{
    const ArgumentVariant& arg = os.read();
    if( boost::optional< const T& > opt = 
            boost::apply_visitor( boost::TypeAccessor< const T >(), arg ) )
    {
        data = opt.get();
    }
    else
    {
        //attempt conversion
        ed_convert( arg, data );
    }
    return os;
}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

template< class OutStreamType, class T >
OutStreamType& serialiseOut( OutStreamType& os, const T& container )
{
    for( auto i = container.cbegin(), iEnd = container.cend(); i!=iEnd; ++i )
        os << *i;
    return os;
}

template< class InStreamType, class T >
InStreamType& serialiseIn( InStreamType& is, T& container )
{
    container.clear();
    while( is )
    {
        typename T::value_type value;
        is >> value;
        generics::push_back( container, value );
    }
    return is;
}

}

#endif //SHORTHAND_IO_12_04_2014