
#include "ed/node.h"
#include "ed/serialise.h"

#include <gtest/gtest.h>

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//Ed Grammar Tests

static const char pszEd1[] =
"                                                                                     \
    groundBody BTBody( 1,2,3,4,5,0.0f; )                                              \
    {                                                                                 \
        shape BTPlane< tag1 >( btStaticPlaneShape( btVector3( 0,0,1 ), 0 ); )         \
        t1 OSGTransform( .....osgRoot )                                               \
        {                                                                             \
            transform Matrix( osg::Matrix::translate( osg::Vec3( 0,0, -0.1 ) ); )     \
            cube OSGCube( ....t1, osg::Vec3( 100,100,0.1 ); )                         \
            {                                                                         \
                colour Vec4( osg::Vec4( 0.5,0.5,0.5,1.0); )                           \
            }                                                                         \
        }                                                                             \
    }                                                                                 \
";

TEST( EdGrammarTests, Ed_Succeed1 )
{
    std::vector< Ed::Node > edNodes;
    std::string strInput( pszEd1 );
    std::ostringstream osError;
    const Ed::ParseResult result = Ed::parse( strInput, edNodes, osError );
    ASSERT_TRUE( result.first );
    ASSERT_EQ( result.second.base(), strInput.end() );
}

static const char pszEdBad1[] =
"                                                                                     \
    groundBody BTBody( 0.0f; )                                                        \
    {                                                                                 \
        shape BTPlane< tag1 >( btStaticPlaneShape( btVector3( 0,0,1 ), 0 ); )         \
        t1 OSGTransform( .....osgRoot.                                               \
        {                                                                             \
            transform Matrix( osg::Matrix::translate( osg::Vec3( 0,0, -0.1 ) ); )     \
            cube OSGCube( ....t1, osg::Vec3( 100,100,0.1 ); )                         \
            {                                                                         \
                colour Vec4( osg::Vec4( 0.5,0.5,0.5,1.0); )                           \
            }                                                                         \
        }                                                                             \
    }                                                                                 \
";

TEST( EdGrammarTests, Ed_Fail1 )
{
    std::vector< Ed::Node > edNodes;
    std::string strInput( pszEdBad1 );

    std::ostringstream osError;
    const Ed::ParseResult result = Ed::parse( strInput, edNodes, osError );
    ASSERT_NE( result.second.base(), strInput.end() );
}
