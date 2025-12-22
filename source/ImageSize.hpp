#pragma once

#include <cstdint>
#include <limits>



struct ImageSize
{
   using VALUE_TYPE = uint32_t;

   static const ImageSize zero;
   static const ImageSize invalid;

   ImageSize( ) = default;
   ImageSize( const VALUE_TYPE& w, const VALUE_TYPE& h );
   ImageSize( const char* filename );

   void print( ) const;

   bool operator==( const ImageSize & other ) const;
   bool operator!=( const ImageSize & other ) const;

   bool is_zero( ) const;
   bool is_invalid( ) const;

   VALUE_TYPE width = 0;
   VALUE_TYPE height = 0;
};
