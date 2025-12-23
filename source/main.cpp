#include <string>



#include "ResourceManager.hpp"
#include "pgm.hpp"
#include "trace.hpp"



bool mod_image( vx_image image, ImageSize& image_size )
{
   vx_rectangle_t rect = { 0, 0, image_size.width, image_size.height };
   vx_map_id map_id;
   vx_imagepatch_addressing_t addr;
   vx_uint8* ptr = nullptr;

   vx_status status = vxMapImagePatch(
         image,
         &rect,
         0,
         &map_id,
         &addr,
         (void**)&ptr,
         VX_READ_AND_WRITE,
         VX_MEMORY_TYPE_HOST,
         0
      );
   if( VX_SUCCESS != status )
   {
      MSG_ERR( "'vxMapImagePatch' error: %d", status );
      return false;
   }

   for( vx_uint32 y = 0; y < addr.dim_y; ++y )
   {
      vx_uint8* row = (vx_uint8*)((vx_uint8*)ptr + y * addr.stride_y);
      for( vx_uint32 x = 0; x < addr.dim_x; ++x )
      {
         // Горизонтальный градиент
         // row[x * addr.stride_x] = (vx_uint8)((x * 255) / (addr.dim_x - 1));
         row[x * addr.stride_x] = row[x * addr.stride_x] & ( addr.dim_x - x );
      }
   }

   vxUnmapImagePatch( image, map_id );
   return true;
}



int main( )
{
   MSG_DBG( "Defiend resources:" );
   ForEachType< VXAllResources >::apply( PrintResourceNames{ } );

   const char* input_image_file = "../resources/sample_640×426.pgm";
   const char* output_image_file = "sample_640×426_processed.pgm";

   // ImageSize image_size = { 640, 426 };
   ImageSize image_size( input_image_file );

   VXResourceStorage resources;

   resources.create< VXContext >(
         "context"
      );

   resources.create< VXGraph >(
         "graph",
         resources.get< VXContext >( "context" )
      );

   resources.create< VXImage >(
         "input",
         resources.get< VXContext >( "context" ),
         image_size.width,
         image_size.height,
         VX_DF_IMAGE_U8
      );
   resources.create< VXImage >(
         "blurred",
         resources.get< VXContext >( "context" ),
         image_size.width,
         image_size.height,
         VX_DF_IMAGE_U8
      );
   resources.create< VXImage >(
         "edges_x",
         resources.get< VXContext >( "context" ),
         image_size.width,
         image_size.height,
         VX_DF_IMAGE_S16
      );
   resources.create< VXImage >(
         "edges_y",
         resources.get< VXContext >( "context" ),
         image_size.width,
         image_size.height,
         VX_DF_IMAGE_S16
      );
   resources.create< VXImage >(
         "edges",
         resources.get< VXContext >( "context" ),
         image_size.width,
         image_size.height,
         VX_DF_IMAGE_S16
      );
   resources.create< VXImage >(
         "output",
         resources.get< VXContext >( "context" ),
         image_size.width,
         image_size.height,
         VX_DF_IMAGE_U8
      );

   vx_int32 shift_val = 1;
   resources.create< VXScalar >(
         "shift",
         resources.get< VXContext >( "context" ),
         VX_TYPE_INT32,
         &shift_val
      );


   if( !load_pgm( input_image_file, resources.get< VXImage >( "input" ), image_size ) )
   {
      MSG_ERR( "Failed to load '%s'", input_image_file );
      return -1;
   }

   resources.create< VXNodeGaussian3x3 >(
         "blure",
         resources.get< VXGraph >( "graph" ),
         resources.get< VXImage >( "input" ),
         resources.get< VXImage >( "blurred" )
      );
   resources.create< VXNodeSobel3x3 >(
         "sobel",
         resources.get< VXGraph >( "graph" ),
         resources.get< VXImage >( "blurred" ),
         resources.get< VXImage >( "edges_x" ),
         resources.get< VXImage >( "edges_y" )
      );
   resources.create< VXNodeMagnitude >(
         "magnitude",
         resources.get< VXGraph >( "graph" ),
         resources.get< VXImage >( "edges_x" ),
         resources.get< VXImage >( "edges_y" ),
         resources.get< VXImage >( "edges" )
      );
   resources.create< VXNodeConvertDepth >(
         "convert",
         resources.get< VXGraph >( "graph" ),
         resources.get< VXImage >( "edges" ),
         resources.get< VXImage >( "output" ),
         VX_CONVERT_POLICY_SATURATE,
         resources.get< VXScalar >( "shift" )
      );

   if( auto status = vxVerifyGraph( resources.get< VXGraph >( "graph" ) ); status != VX_SUCCESS )
   {
      MSG_ERR( "Graph verification failed: '%d'", status );
      return -1;
   }

   vxProcessGraph( resources.get< VXGraph >( "graph" ) );

   if( !save_pgm( output_image_file, resources.get< VXImage >( "output" ), image_size ) )
   {
      MSG_ERR( "Failed to save '%s'", output_image_file );
      return -1;
   }

   resources.clear( );

   MSG_INF( "Done" );
   return 0;
}
