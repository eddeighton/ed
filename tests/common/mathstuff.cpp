
#include "common/mathGenerics.h"

#include <gtest/gtest.h>

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//skip
TEST( BasicMath, IndexTest_X )
{
    ASSERT_EQ( 0, Util::getXFrom2dIndex( 4, 0 ) );
    ASSERT_EQ( 1, Util::getXFrom2dIndex( 4, 1 ) );
    ASSERT_EQ( 2, Util::getXFrom2dIndex( 4, 2 ) );
    ASSERT_EQ( 3, Util::getXFrom2dIndex( 4, 3 ) );
    ASSERT_EQ( 0, Util::getXFrom2dIndex( 4, 4 ) );
    ASSERT_EQ( 1, Util::getXFrom2dIndex( 4, 5 ) );
    ASSERT_EQ( 2, Util::getXFrom2dIndex( 4, 6 ) );
    ASSERT_EQ( 3, Util::getXFrom2dIndex( 4, 7 ) );
    ASSERT_EQ( 0, Util::getXFrom2dIndex( 4, 8 ) );
}

TEST( BasicMath, IndexTest_Y )
{
    ASSERT_EQ( 0, Util::getYFrom2dIndex( 4, 0 ) );
    ASSERT_EQ( 0, Util::getYFrom2dIndex( 4, 1 ) );
    ASSERT_EQ( 0, Util::getYFrom2dIndex( 4, 2 ) );
    ASSERT_EQ( 0, Util::getYFrom2dIndex( 4, 3 ) );
    ASSERT_EQ( 1, Util::getYFrom2dIndex( 4, 4 ) );
    ASSERT_EQ( 1, Util::getYFrom2dIndex( 4, 5 ) );
    ASSERT_EQ( 1, Util::getYFrom2dIndex( 4, 6 ) );
    ASSERT_EQ( 1, Util::getYFrom2dIndex( 4, 7 ) );
    ASSERT_EQ( 2, Util::getYFrom2dIndex( 4, 8 ) );

}

TEST( BasicMath, IndexTest3d )
{
    ASSERT_EQ( 0, Util::getXFrom3dIndex( 2, 3, 0 ) );
    ASSERT_EQ( 0, Util::getYFrom3dIndex( 2, 3, 0 ) );
    ASSERT_EQ( 0, Util::getZFrom3dIndex( 2, 3, 0 ) );
    
    ASSERT_EQ( 1, Util::getXFrom3dIndex( 2, 3, 1 ) );
    ASSERT_EQ( 0, Util::getYFrom3dIndex( 2, 3, 1 ) );
    ASSERT_EQ( 0, Util::getZFrom3dIndex( 2, 3, 1 ) );
    
    ASSERT_EQ( 0, Util::getXFrom3dIndex( 2, 3, 2 ) );
    ASSERT_EQ( 1, Util::getYFrom3dIndex( 2, 3, 2 ) );
    ASSERT_EQ( 0, Util::getZFrom3dIndex( 2, 3, 2 ) );
    
    ASSERT_EQ( 1, Util::getXFrom3dIndex( 2, 3, 3 ) );
    ASSERT_EQ( 1, Util::getYFrom3dIndex( 2, 3, 3 ) );
    ASSERT_EQ( 0, Util::getZFrom3dIndex( 2, 3, 3 ) );
    
    ASSERT_EQ( 0, Util::getXFrom3dIndex( 2, 3, 4 ) );
    ASSERT_EQ( 2, Util::getYFrom3dIndex( 2, 3, 4 ) );
    ASSERT_EQ( 0, Util::getZFrom3dIndex( 2, 3, 4 ) );
    
    ASSERT_EQ( 1, Util::getXFrom3dIndex( 2, 3, 5 ) );
    ASSERT_EQ( 2, Util::getYFrom3dIndex( 2, 3, 5 ) );
    ASSERT_EQ( 0, Util::getZFrom3dIndex( 2, 3, 5 ) );
    
    ASSERT_EQ( 0, Util::getXFrom3dIndex( 2, 3, 6 ) );
    ASSERT_EQ( 0, Util::getYFrom3dIndex( 2, 3, 6 ) );
    ASSERT_EQ( 1, Util::getZFrom3dIndex( 2, 3, 6 ) );
    
    ASSERT_EQ( 1, Util::getXFrom3dIndex( 2, 3, 7 ) );
    ASSERT_EQ( 0, Util::getYFrom3dIndex( 2, 3, 7 ) );
    ASSERT_EQ( 1, Util::getZFrom3dIndex( 2, 3, 7 ) );
    
    ASSERT_EQ( 0, Util::getXFrom3dIndex( 2, 3, 8 ) );
    ASSERT_EQ( 1, Util::getYFrom3dIndex( 2, 3, 8 ) );
    ASSERT_EQ( 1, Util::getZFrom3dIndex( 2, 3, 8 ) );
    
    ASSERT_EQ( 1, Util::getXFrom3dIndex( 2, 3, 9 ) );
    ASSERT_EQ( 1, Util::getYFrom3dIndex( 2, 3, 9 ) );
    ASSERT_EQ( 1, Util::getZFrom3dIndex( 2, 3, 9 ) );

}
