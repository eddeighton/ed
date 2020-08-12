
#include "common/edsMath.h"

#include <gtest/gtest.h>

TEST( EdsMath, clamp_floats )
{
    ASSERT_FLOAT_EQ( 0.0f, EdsMath::clamp( 0.0f, 1.0f, -2.0f ) );
    ASSERT_FLOAT_EQ( 1.0f, EdsMath::clamp( 0.0f, 1.0f, 2.0f ) );
    ASSERT_FLOAT_EQ( 0.0f, EdsMath::clamp( 0.0f, 1.0f, -1.0f ) );
    ASSERT_FLOAT_EQ( 1.0f, EdsMath::clamp( 0.0f, 1.0f, 1.0f ) );
    ASSERT_FLOAT_EQ( 0.5f, EdsMath::clamp( 0.0f, 1.0f, 0.5f ) );
    ASSERT_FLOAT_EQ( -5.0f, EdsMath::clamp( -12.5f, -5.0f, 0.0f ) );
    ASSERT_FLOAT_EQ( -12.5f, EdsMath::clamp( -12.5f, -5.0f, -13.0f ) );
    ASSERT_FLOAT_EQ( -6.0f, EdsMath::clamp( -12.5f, -5.0f, -6.0f ) );
}
TEST( EdsMath, clamp_ints )
{
    ASSERT_EQ(  0, EdsMath::clamp( 0, 1, -2 ) );
    ASSERT_EQ(  1, EdsMath::clamp( 0, 1,  2 ) );
    ASSERT_EQ(  0, EdsMath::clamp( 0, 1, -1 ) );
    ASSERT_EQ(  1, EdsMath::clamp( 0, 1,  1 ) );
    ASSERT_EQ(  2, EdsMath::clamp( 0, 3,  2 ) );
    ASSERT_EQ( -5, EdsMath::clamp( -12, -5, 0 ) );
    ASSERT_EQ(-12, EdsMath::clamp( -12, -5, -13 ) );
    ASSERT_EQ( -6, EdsMath::clamp( -12, -5, -6 ) );
}

TEST( EdsMath, mapToRange_uint )
{
    ASSERT_EQ( 0u, EdsMath::mapToRange( 0u, 1u ) );
    ASSERT_EQ( 0u, EdsMath::mapToRange( 1u, 1u ) );
    ASSERT_EQ( 1u, EdsMath::mapToRange( 1u, 2u ) );
}

TEST( EdsMath, mapToRange_int )
{
    ASSERT_EQ( 0, EdsMath::mapToRange( 0, 1 ) );
    ASSERT_EQ( 0, EdsMath::mapToRange( 1, 1 ) );
    ASSERT_EQ( 1, EdsMath::mapToRange( 1, 2 ) );
    ASSERT_EQ( 0, EdsMath::mapToRange( -1, 1 ) );
    ASSERT_EQ( 2, EdsMath::mapToRange( -2, 4 ) );
    ASSERT_EQ( 2, EdsMath::mapToRange( 2, 4 ) );
}

TEST( EdsMath, mapToRange_floats )
{
    ASSERT_FLOAT_EQ( 0.0f, EdsMath::mapToRange( -2.0f, 1.0f ) );
    ASSERT_FLOAT_EQ( 0.0f, EdsMath::mapToRange( -1.0f, 1.0f ) );
    ASSERT_FLOAT_EQ( 0.0f, EdsMath::mapToRange( 0.0f, 1.0f ) );
    ASSERT_FLOAT_EQ( 0.0f, EdsMath::mapToRange( 1.0f, 1.0f ) );
    ASSERT_FLOAT_EQ( 0.0f, EdsMath::mapToRange( 2.0f, 1.0f ) );

    ASSERT_FLOAT_EQ( 0.5f, EdsMath::mapToRange( -2.5f, 1.0f ) );
    ASSERT_FLOAT_EQ( 0.5f, EdsMath::mapToRange( -1.5f, 1.0f ) );
    ASSERT_FLOAT_EQ( 0.5f, EdsMath::mapToRange( 0.5f, 1.0f ) );
    ASSERT_FLOAT_EQ( 0.5f, EdsMath::mapToRange( 1.5f, 1.0f ) );
    ASSERT_FLOAT_EQ( 0.5f, EdsMath::mapToRange( 2.5f, 1.0f ) );
}
TEST( EdsMath, mapToRange_double )
{
    ASSERT_DOUBLE_EQ( 0.0, EdsMath::mapToRange( -2.0, 1.0 ) );
    ASSERT_DOUBLE_EQ( 0.0, EdsMath::mapToRange( -1.0, 1.0 ) );
    ASSERT_DOUBLE_EQ( 0.0, EdsMath::mapToRange( 0.0, 1.0 ) );
    ASSERT_DOUBLE_EQ( 0.0, EdsMath::mapToRange( 1.0, 1.0 ) );
    ASSERT_DOUBLE_EQ( 0.0, EdsMath::mapToRange( 2.0, 1.0 ) );

    ASSERT_DOUBLE_EQ( 0.5, EdsMath::mapToRange( -2.5, 1.0 ) );
    ASSERT_DOUBLE_EQ( 0.5, EdsMath::mapToRange( -1.5, 1.0 ) );
    ASSERT_DOUBLE_EQ( 0.5, EdsMath::mapToRange( 0.5, 1.0 ) );
    ASSERT_DOUBLE_EQ( 0.5, EdsMath::mapToRange( 1.5, 1.0 ) );
    ASSERT_DOUBLE_EQ( 0.5, EdsMath::mapToRange( 2.5, 1.0 ) );
}


TEST( EdsMath, Quantize_float )
{
    ASSERT_FLOAT_EQ( 100.0f, EdsMath::quantize( 101.1111f, 10.0f ) );
    ASSERT_FLOAT_EQ( 101.0f, EdsMath::quantize( 101.1111f, 1.0f ) );
    ASSERT_FLOAT_EQ( 101.1f, EdsMath::quantize( 101.1111f, 0.1f ) );

    ASSERT_FLOAT_EQ( -110.0f, EdsMath::quantize( -101.1111f, 10.0f ) );
    ASSERT_FLOAT_EQ( -102.0f, EdsMath::quantize( -101.1111f, 1.0f ) );
    ASSERT_FLOAT_EQ( -101.2f, EdsMath::quantize( -101.1111f, 0.1f ) );
    ASSERT_FLOAT_EQ( -101.12f, EdsMath::quantize( -101.1111f, 0.01f ) );
}

TEST( EdsMath, Quantize_uint )
{
    ASSERT_EQ( 3u, EdsMath::quantize( 5u, 3u ) );
    ASSERT_EQ( 100u, EdsMath::quantize( 123u, 100u ) );
}

TEST( EdsMath, Quantize_int )
{
    ASSERT_EQ( 3, EdsMath::quantize( 5, 3 ) );
    ASSERT_EQ( 100, EdsMath::quantize( 123, 100 ) );
    ASSERT_EQ( -6, EdsMath::quantize( -5, 3 ) );
    ASSERT_EQ( -200, EdsMath::quantize( -123, 100 ) );
}

TEST( EdsMath, roundRealOutToInt )
{
    ASSERT_EQ( 0, EdsMath::roundRealOutToInt( 0.0f ) );
    ASSERT_EQ( 1, EdsMath::roundRealOutToInt( 0.001f ) );
    ASSERT_EQ( -1, EdsMath::roundRealOutToInt( -0.001f ) );
    ASSERT_EQ( 124, EdsMath::roundRealOutToInt( 123.123f ) );
    ASSERT_EQ( -124, EdsMath::roundRealOutToInt( -123.123f ) );
}

TEST( EdsMath, roundPositiveRealToUInt )
{
    ASSERT_EQ( 0u, EdsMath::roundPositiveRealToUInt( 0.0f ) );
    ASSERT_EQ( 0u, EdsMath::roundPositiveRealToUInt( 0.499f ) );
    ASSERT_EQ( 1u, EdsMath::roundPositiveRealToUInt( 0.5f ) );
    ASSERT_EQ( 123u, EdsMath::roundPositiveRealToUInt( 123.123f ) );
    ASSERT_EQ( 124u, EdsMath::roundPositiveRealToUInt( 123.523f ) );
}

TEST( EdsMath, POW10 )
{
    ASSERT_DOUBLE_EQ( 1.0, EdsMath::pow10( 0 ) );
    ASSERT_DOUBLE_EQ( 1000.0, EdsMath::pow10( 3 ) );
    ASSERT_DOUBLE_EQ( 1000000.0, EdsMath::pow10( 6 ) );
    ASSERT_DOUBLE_EQ( 0.001, EdsMath::pow10( -3 ) );
    ASSERT_DOUBLE_EQ( 0.000001, EdsMath::pow10( -6 ) );
}


