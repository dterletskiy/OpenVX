#pragma once

#include <VX/vx.h>
#include <VX/vxu.h>

#include "trace.hpp"



struct VXResource
{
   virtual ~VXResource( ) = default;
   virtual void release( ) = 0;
};

struct VXContext : VXResource
{
   using RESOURCE_TYPE = vx_context;
   using TYPE = VXContext;
   using tSptr = std::shared_ptr< TYPE >;
   using tUptr = std::unique_ptr< TYPE >;

   static constexpr auto &creator  = vxCreateContext;
   static constexpr auto &releaser = vxReleaseContext;

   static constexpr const char* name = "VXContext";

   template< typename... Args >
   VXContext( Args... args ) : resource( creator( args... ) )
   {
   }

   ~VXContext( ) override
   {
      release( );
   }

   template< typename... Args >
   static tSptr create( Args... args )
   {
      auto ptr = std::make_shared< TYPE >( args... );
      vx_status status = vxGetStatus( (vx_reference)(ptr->resource) );
      if( status != VX_SUCCESS )
      {
         MSG_ERR( "error: %d", status );
         return nullptr;
      }
      return ptr;
   }

   void release( ) override
   {
      releaser( &resource );
   }

   RESOURCE_TYPE resource;
};

struct VXGraph : VXResource
{
   using RESOURCE_TYPE = vx_graph;
   using TYPE = VXGraph;
   using tSptr = std::shared_ptr< TYPE >;
   using tUptr = std::unique_ptr< TYPE >;

   static constexpr auto &creator  = vxCreateGraph;
   static constexpr auto &releaser = vxReleaseGraph;

   static constexpr const char* name = "VXGraph";

   template< typename... Args >
   VXGraph( Args... args ) : resource( creator( args... ) )
   {
   }

   ~VXGraph( ) override
   {
      release( );
   }

   template< typename... Args >
   static tSptr create( Args... args )
   {
      auto ptr = std::make_shared< TYPE >( args... );
      vx_status status = vxGetStatus( (vx_reference)(ptr->resource) );
      if( status != VX_SUCCESS )
      {
         MSG_ERR( "error: %d", status );
         return nullptr;
      }
      return ptr;
   }

   void release( ) override
   {
      releaser( &resource );
   }

   RESOURCE_TYPE resource;
};

struct VXImage : VXResource
{
   using RESOURCE_TYPE = vx_image;
   using TYPE = VXImage;
   using tSptr = std::shared_ptr< TYPE >;
   using tUptr = std::unique_ptr< TYPE >;

   static constexpr auto &creator  = vxCreateImage;
   static constexpr auto &releaser = vxReleaseImage;

   static constexpr const char* name = "VXImage";

   template< typename... Args >
   VXImage( Args... args ) : resource( creator( args... ) )
   {
   }

   ~VXImage( ) override
   {
      release( );
   }

   template< typename... Args >
   static tSptr create( Args... args )
   {
      auto ptr = std::make_shared< TYPE >( args... );
      vx_status status = vxGetStatus( (vx_reference)(ptr->resource) );
      if( status != VX_SUCCESS )
      {
         MSG_ERR( "error: %d", status );
         return nullptr;
      }
      return ptr;
   }

   void release( ) override
   {
      releaser( &resource );
   }

   RESOURCE_TYPE resource;
};

struct VXScalar : VXResource
{
   using RESOURCE_TYPE = vx_scalar;
   using TYPE = VXScalar;
   using tSptr = std::shared_ptr< TYPE >;
   using tUptr = std::unique_ptr< TYPE >;

   static constexpr auto &creator  = vxCreateScalar;
   static constexpr auto &releaser = vxReleaseScalar;

   static constexpr const char* name = "VXScalar";

   template< typename... Args >
   VXScalar( Args... args ) : resource( creator( args... ) )
   {
   }

   ~VXScalar( ) override
   {
      release( );
   }

   template< typename... Args >
   static tSptr create( Args... args )
   {
      auto ptr = std::make_shared< TYPE >( args... );
      vx_status status = vxGetStatus( (vx_reference)(ptr->resource) );
      if( status != VX_SUCCESS )
      {
         MSG_ERR( "error: %d", status );
         return nullptr;
      }
      return ptr;
   }

   void release( ) override
   {
      releaser( &resource );
   }

   RESOURCE_TYPE resource;
};

template< auto tCreator >
struct VXNode : VXResource
{
   using RESOURCE_TYPE = vx_node;
   using TYPE = VXNode< tCreator >;
   using tSptr = std::shared_ptr< TYPE >;
   using tUptr = std::unique_ptr< TYPE >;

   // static constexpr auto &creator  = tCreator;
   static constexpr auto &releaser = vxReleaseNode;

   static constexpr const char* name = "VXNode";

   template< typename... Args >
   VXNode( Args... args ) : resource( tCreator( args... ) )
   {
   }

   ~VXNode( ) override
   {
      release( );
   }

   template< typename... Args >
   static tSptr create( Args... args )
   {
      auto ptr = std::make_shared< TYPE >( args... );
      vx_status status = vxGetStatus( (vx_reference)(ptr->resource) );
      if( status != VX_SUCCESS )
      {
         MSG_ERR( "error: %d", status );
         return nullptr;
      }
      return ptr;
   }

   void release( ) override
   {
      releaser( &resource );
   }

   RESOURCE_TYPE resource;
};

template< typename T >
constexpr const char* resource_name( )
{
   return typename T::name;
}
