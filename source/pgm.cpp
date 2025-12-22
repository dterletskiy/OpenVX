#include "pgm.hpp"
#include "trace.hpp"



bool load_pgm(
      const char* filename, vx_image image, ImageSize& image_size
   )
{
   FILE* f = fopen( filename, "rb" );
   if( !f )
   {
      MSG_ERR( "'fopen' error: %s", filename );
      return false;
   }

   char magic[3] = {0};
   if( 1 != fscanf( f, "%2s", magic ) )
   {
      MSG_ERR( "'fscanf' error: %s", filename );
      fclose( f );
      return false;
   }
   if( 0 != strcmp( magic, "P5" ) )
   {
      MSG_ERR( "'strcmp' error: %s", filename );
      fclose( f );
      return false;
   }

   ImageSize detected_image_size;
   int maxval = 0;
   fscanf( f, "%d %d %d", &detected_image_size.width, &detected_image_size.height, &maxval );
   fgetc( f ); // skip single whitespace

   image_size.print( );
   detected_image_size.print( );

   if( image_size.is_zero( ) )
   {
      MSG_WRN( "updating image size" );
      image_size = detected_image_size;
   }

   if( image_size != detected_image_size || 255 != maxval )
   {
      MSG_ERR( "image size mismatch" );
      fclose( f );
      return false;
   }

   vx_rectangle_t rect = { 0, 0, image_size.width, image_size.height };
   vx_map_id map_id;
   vx_imagepatch_addressing_t addr;
   vx_uint8* ptr = nullptr;

   if( VX_SUCCESS != vxMapImagePatch(
         image,
         &rect,
         0,
         &map_id,
         &addr,
         (void**)&ptr,
         VX_WRITE_ONLY,
         VX_MEMORY_TYPE_HOST,
         0
      ))
   {
      MSG_ERR( "'vxMapImagePatch' error: %s", filename );
      fclose( f );
      return false;
   }

   for( vx_uint32 y = 0; y < image_size.height; ++y )
   {
      vx_uint8* row = (vx_uint8*)((vx_uint8*)ptr + y * addr.stride_y);
      fread( row, 1, image_size.width, f );
   }

   vxUnmapImagePatch( image, map_id );
   fclose( f );
   return true;
}

bool save_pgm(
      const char* filename, vx_image image, const ImageSize& image_size
   )
{
   FILE* f = fopen( filename, "wb" );
   if( !f )
   {
      MSG_ERR( "'fopen' error: %s", filename );
      return false;
   }

   fprintf( f, "P5\n%u %u\n255\n", image_size.width, image_size.height );

   vx_rectangle_t rect = { 0, 0, image_size.width, image_size.height };
   vx_map_id map_id;
   vx_imagepatch_addressing_t addr;
   vx_uint8* ptr = nullptr;

   if( VX_SUCCESS != vxMapImagePatch(
         image,
         &rect,
         0,
         &map_id,
         &addr,
         (void**)&ptr,
         VX_READ_ONLY,
         VX_MEMORY_TYPE_HOST,
         0
      ) )
   {
      fclose( f );
      return false;
   }

   for( vx_uint32 y = 0; y < image_size.height; ++y )
   {
      vx_uint8* row = (vx_uint8*)( (vx_uint8*)ptr + y * addr.stride_y );
      fwrite( row, 1, image_size.width, f );
   }

   vxUnmapImagePatch( image, map_id );
   fclose( f );
   return true;
}
