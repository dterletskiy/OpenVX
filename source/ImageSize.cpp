#include "ImageSize.hpp"
#include "trace.hpp"



const ImageSize ImageSize::zero { 0, 0 };
const ImageSize ImageSize::invalid {
      std::numeric_limits< VALUE_TYPE >::max( ),
      std::numeric_limits< VALUE_TYPE >::max( )
   };



ImageSize::ImageSize( const VALUE_TYPE& w, const VALUE_TYPE& h )
   : width( w )
   , height( h )
{
}

ImageSize::ImageSize( const char* filename )
   : width( 0 )
   , height( 0 )

{
   FILE* f = fopen( filename, "rb" );
   if( !f )
   {
      MSG_ERR( "'fopen' error: %s", filename );
      return;
   }

   char magic[3] = {0};
   if( 1 != fscanf( f, "%2s", magic ) )
   {
      MSG_ERR( "'fscanf' error: %s", filename );
      fclose( f );
      return;
   }
   if( 0 != strcmp( magic, "P5" ) )
   {
      MSG_ERR( "'strcmp' error: %s", filename );
      fclose( f );
      return ;
   }

   int maxval = 0;
   fscanf( f, "%d %d %d", &width, &height, &maxval );
   fgetc( f ); // skip single whitespace

   if( 255 != maxval )
   {
      MSG_ERR( "maxval error: %d", maxval );
      width = 0;
      height = 0;
      fclose( f );
      return;
   }

   fclose( f );
}

void ImageSize::print( ) const
{
   MSG_INF( "image size: width = %u / height = %u",
         width, height
      );
}

bool ImageSize::operator==( const ImageSize & other ) const
{
   return ( other.width == width ) && ( other.height == height );
}

bool ImageSize::operator!=( const ImageSize & other ) const
{
   return !( *this == other );
}

bool ImageSize::is_zero( ) const
{
   return *this == zero;
}

bool ImageSize::is_invalid( ) const
{
   return *this == invalid;
}
