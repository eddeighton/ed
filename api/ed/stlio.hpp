#ifndef STLIO_13_04_2014
#define STLIO_13_04_2014

#include "ed/shorthandio.hpp"

#include <string>
#include <bitset>
#include <vector>
#include <tuple>

namespace Ed
{

template< class T1, class T2 >
inline OShorthandStream& operator<<( OShorthandStream& os, const std::tuple< T1, T2 >& data )
{
    return os << std::get< 0 >( data ) << std::get< 1 >( data );
}

template< class T1, class T2 >
inline IShorthandStream& operator>>( IShorthandStream& is, std::tuple< T1, T2 >& data )
{  
    return is >> std::get< 0 >( data ) >> std::get< 1 >( data );
}

template< class T1, class T2, class T3 >
inline OShorthandStream& operator<<( OShorthandStream& os, const std::tuple< T1, T2, T3 >& data )
{
    return os << std::get< 0 >( data ) << std::get< 1 >( data ) << std::get< 2 >( data );
}

template< class T1, class T2, class T3 >
inline IShorthandStream& operator>>( IShorthandStream& is, std::tuple< T1, T2, T3 >& data )
{  
    return is >> std::get< 0 >( data ) >> std::get< 1 >( data ) >> std::get< 2 >( data );
}

template< class T1, class T2, class T3, class T4 >
inline OShorthandStream& operator<<( OShorthandStream& os, const std::tuple< T1, T2, T3, T4 >& data )
{
    return os << std::get< 0 >( data ) << std::get< 1 >( data ) << std::get< 2 >( data ) << std::get< 3 >( data );
}

template< class T1, class T2, class T3, class T4 >
inline IShorthandStream& operator>>( IShorthandStream& is, std::tuple< T1, T2, T3, T4 >& data )
{  
    return is >> std::get< 0 >( data ) >> std::get< 1 >( data ) >> std::get< 2 >( data ) >> std::get< 3 >( data );
}

template< class T1, class T2, class T3, class T4 >
inline IShorthandStream& operator>>( IShorthandStream& is, std::tuple< T1&, T2&, T3&, T4& >& data )
{  
    return is >> std::get< 0 >( data ) >> std::get< 1 >( data ) >> std::get< 2 >( data ) >> std::get< 3 >( data );
}

inline OShorthandStream& operator<<( OShorthandStream& os, const std::vector< float >& data )
{
    return serialiseOut( os, data );
}

inline IShorthandStream& operator>>( IShorthandStream& is, std::vector< float >& data )
{
    return serialiseIn( is, data );
}

template< uint32_t SIZE >
inline OShorthandStream& operator<<( OShorthandStream& os, const std::bitset< SIZE >& data )
{
    return os << data.to_string();
}

template< uint32_t SIZE >
inline IShorthandStream& operator>>( IShorthandStream& is, std::bitset< SIZE >& data )
{
    std::string str;
    is >> str;
    data = std::bitset< SIZE >( str );
    return is;
}

}

#endif //STLIO_13_04_2014