
#include "common/stl.h"
#include "common/compose.h"

#include <gtest/gtest.h>
#include <utility>
#include <list>
#include <vector>
#include <set>
#include <map>

const int DATA1_SIZE = 9;
const int _data1[ DATA1_SIZE ] = { 0,1,4,7,9,10,13,15,17 };
const std::vector< int > g_data1( _data1, _data1 + DATA1_SIZE );

const int DATA2_SIZE = 10;
const int _data2[ DATA2_SIZE ] = { 0,4,6,7,8,9,10,11,14,15 };
const std::vector< int > g_data2( _data2, _data2 + DATA2_SIZE );

const int adds[ 4 ] = { 6,8,11,14 };
const std::vector< int > g_adds( adds, adds + 4 );

const int rems[3] = { 1,13,17 };
const std::vector< int > g_rems( rems, rems + 3 );


TEST( Generics, match_1 )
{
    std::vector< int > s1( g_data1.begin(), g_data1.end() );
    std::set< int > s2( g_data2.begin(), g_data2.end() );
    typedef std::vector< int >::const_iterator IterType1;
    typedef std::set< int >::const_iterator IterType2;

    std::list< int > add;
    std::vector< int > remove;
    generics::match( s1.begin(), s1.end(), s2.begin(), s2.end(),
            generics::lessthan( generics::deref< IterType1 >(),
                                generics::deref< IterType2 >() ),
            generics::collect( remove, generics::deref< IterType1 >() ),
            generics::collect( add, generics::deref< IterType2 >() ) );

    ASSERT_EQ( std::list< int >( g_adds.begin(), g_adds.end() ), add );
    ASSERT_EQ( std::vector< int >( g_rems.begin(), g_rems.end() ), remove );
}

TEST( Generics, match_2 )
{
    std::vector< int > s1( g_data1.begin(), g_data1.end() );
    std::vector< int > s2( g_data2.begin(), g_data2.end() );
    typedef std::vector< int >::const_iterator IterType;

    std::list< int > add, remove;
    generics::match( s1.begin(), s1.end(), s2.begin(), s2.end(),
            generics::lessthan( generics::deref< IterType >(),
                                generics::deref< IterType >() ),
            generics::collect( remove, generics::deref< IterType >() ),
            generics::collect( add, generics::deref< IterType >() ) );

    ASSERT_EQ( std::list< int >( g_adds.begin(), g_adds.end() ), add );
    ASSERT_EQ( std::list< int >( g_rems.begin(), g_rems.end() ), remove );
}

TEST( Generics, match_3 )
{
    std::list< int > s1( g_data1.begin(), g_data1.end() );
    std::list< int > s2( g_data2.begin(), g_data2.end() );
    typedef std::list< int >::const_iterator IterType;

    std::set< int > add, remove;
    generics::match( s1.begin(), s1.end(), s2.begin(), s2.end(),
            generics::lessthan( generics::deref< IterType >(),
                                generics::deref< IterType >() ),
            generics::collect( remove, generics::deref< IterType >() ),
            generics::collect( add, generics::deref< IterType >() ) );

    ASSERT_EQ( std::set< int >( g_adds.begin(), g_adds.end() ), add );
    ASSERT_EQ( std::set< int >( g_rems.begin(), g_rems.end() ), remove );
}

TEST( Generics, match_4 )
{
    std::map< int, int > m1;
    for( std::vector< int >::const_iterator i = g_data1.begin(), iEnd = g_data1.end(); i!=iEnd; ++i )
        m1.insert( std::make_pair( *i, 0 ) );
    std::list< int > s2( g_data2.begin(), g_data2.end() );
    typedef std::map< int, int >::const_iterator Iter1Type;
    typedef std::list< int >::const_iterator Iter2Type;

    std::list< int > add;
    std::list< int > remove;
    generics::match( m1.begin(), m1.end(), s2.begin(), s2.end(),
            generics::lessthan( generics::first< Iter1Type >(),
                                 generics::deref< Iter2Type >() ),
            generics::collect( remove, generics::first< Iter1Type >() ),
            generics::collect( add, generics::deref< Iter2Type >() ) );

    ASSERT_EQ( std::list< int >( g_adds.begin(), g_adds.end() ), add );
    ASSERT_EQ( std::list< int >( g_rems.begin(), g_rems.end() ), remove );

}

TEST( Generics, match_5 )
{
    std::list< int > l( g_data1.begin(), g_data1.end() );
    typedef std::list< int >::const_iterator Iter1Type;

    std::map< int, int > m;
    for( std::vector< int >::const_iterator i = g_data2.begin(), iEnd = g_data2.end(); i!=iEnd; ++i )
        m.insert( std::make_pair( *i, 0 ) );
    typedef std::map< int, int >::const_iterator Iter2Type;

    std::list< int > add;
    std::vector< int > remove;
    generics::match( l.begin(), l.end(), m.begin(), m.end(),
            generics::lessthan( generics::deref< Iter1Type >(),
                                 generics::first< Iter2Type >() ),
            generics::collect( remove, generics::deref< Iter1Type >() ),
            generics::collect( add, generics::first< Iter2Type >() ) );

    ASSERT_EQ( std::list< int >( g_adds.begin(), g_adds.end() ), add );
    ASSERT_EQ( std::vector< int >( g_rems.begin(), g_rems.end() ), remove );

}

static const int someData[]     = { 0,1,2,3,4,5,6,7,8,9};
static const int someResult2[]  = { 0,1,2,7,3,4,5,6,8,9};
static const int someResult1[]  = { 0,1,2,4,5,6,7,3,8,9};
TEST( Generics, slide_1 )
{
    {
        std::vector< int > v( someData, someData + 10 );
        generics::slide( v.begin(), 3, 7 );
        ASSERT_EQ( v, std::vector< int >( someResult1, someResult1 + 10 ) );
    }

    {
        std::vector< int > v( someData, someData + 10 );
        generics::slide( v.begin(), 7, 3 );
        ASSERT_EQ( v, std::vector< int >( someResult2, someResult2 + 10 ) );
    }

}
