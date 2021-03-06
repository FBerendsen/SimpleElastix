#include "SimpleITKTestHarness.h"
#include "sitkCastImageFilter.h"
#include "sitkSimpleElastix.h"
#include "sitkSimpleTransformix.h"

namespace sitk = itk::simple;

class SimpleTransformixTest : public ::testing::Test
{
protected:

    virtual void SetUp()
    {
        fixedImage = sitk::Cast( sitk::ReadImage( dataFinder.GetFile( "Input/BrainProtonDensitySliceBorder20.png" ) ), sitk::sitkFloat32 );
        movingImage = sitk::Cast( sitk::ReadImage( dataFinder.GetFile( "Input/BrainProtonDensitySliceShifted13x17y.png" ) ), sitk::sitkFloat32 );

        elastix.SetFixedImage( fixedImage );
        elastix.SetMovingImage( movingImage );
        elastix.SetParameterMap( sitk::GetDefaultParameterMap( "translation" ) );
        elastix.Execute();

        outputFolder = std::string( "." );
    }

    typedef sitk::SimpleElastix                         SimpleElastixType;
    typedef sitk::SimpleTransformix                     SimpleTransformixType;
    typedef SimpleTransformixType::ParameterMapType     ParameterMapType;
    typedef SimpleTransformixType::ParameterMapListType ParameterMapListType;
    typedef SimpleTransformixType::ParameterValuesType  ParameterValuesType;

    SimpleElastixType elastix;
    sitk::Image fixedImage;
    sitk::Image movingImage;
    std::string outputFolder;

    bool IsEmpty( const sitk::Image image )
    {
      return ( image.GetWidth() == 0 && image.GetHeight() == 0 );
    }
};

TEST_F( SimpleTransformixTest, ObjectOrientedInterface )
{
    SimpleTransformixType transformix;

    EXPECT_EQ( transformix.GetName(), "SimpleTransformix" );
    EXPECT_EQ( transformix.GetTransformParameterMap().size(), 0u );

    ASSERT_THROW( transformix.Execute(), sitk::GenericException );
    EXPECT_NO_THROW( transformix.SetInputImage( movingImage ) );
    ASSERT_THROW( transformix.Execute(), sitk::GenericException );

    EXPECT_NO_THROW( transformix.SetTransformParameterMap( elastix.GetTransformParameterMap() ) );
    EXPECT_NO_THROW( transformix.Execute() );
    EXPECT_FALSE( this->IsEmpty( transformix.GetResultImage() ) );

    EXPECT_NO_THROW( transformix.Execute() );
    EXPECT_FALSE( this->IsEmpty( transformix.GetResultImage() ) );

    EXPECT_NO_THROW( transformix.Execute() );
    EXPECT_FALSE( this->IsEmpty( transformix.GetResultImage() ) );
}


TEST_F( SimpleTransformixTest, ProceduralInterface )
{
    sitk::Image resultImage;

    EXPECT_NO_THROW( resultImage = Transformix( movingImage, elastix.GetTransformParameterMap()[0] ) );
    EXPECT_FALSE( this->IsEmpty( resultImage ) );
    EXPECT_NO_THROW( resultImage = Transformix( movingImage, elastix.GetTransformParameterMap()[0], true ) );
    EXPECT_FALSE( this->IsEmpty( resultImage ) );
    EXPECT_NO_THROW( resultImage = Transformix( movingImage, elastix.GetTransformParameterMap()[0], true, outputFolder ) );
    EXPECT_FALSE( this->IsEmpty( resultImage ) );
    EXPECT_NO_THROW( resultImage = Transformix( movingImage, elastix.GetTransformParameterMap()[0], false, outputFolder ) );
    EXPECT_FALSE( this->IsEmpty( resultImage ) );

    EXPECT_NO_THROW( resultImage = Transformix( movingImage, elastix.GetTransformParameterMap() ) );
    EXPECT_FALSE( this->IsEmpty( resultImage ) );
    EXPECT_NO_THROW( resultImage = Transformix( movingImage, elastix.GetTransformParameterMap(), true ) );
    EXPECT_FALSE( this->IsEmpty( resultImage ) );
    EXPECT_NO_THROW( resultImage = Transformix( movingImage, elastix.GetTransformParameterMap(), true, outputFolder ) );
    EXPECT_FALSE( this->IsEmpty( resultImage ) );
    EXPECT_NO_THROW( resultImage = Transformix( movingImage, elastix.GetTransformParameterMap(), false, outputFolder ) );
    EXPECT_FALSE( this->IsEmpty( resultImage ) );

    ParameterMapListType parameterMapList = elastix.GetTransformParameterMap();
    parameterMapList[ parameterMapList.size()-1 ][ "WriteResultImage" ] = ParameterValuesType( 1, "false" );
    EXPECT_NO_THROW( resultImage = Transformix( movingImage, elastix.GetTransformParameterMap() ) );
    EXPECT_FALSE( this->IsEmpty( resultImage ) );
}

