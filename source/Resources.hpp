#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <functional>

#include <VX/vx.h>
#include <VX/vxu.h>

#include "trace.hpp"



template< typename T >
struct TVXResourceTraits;

template< typename T >
constexpr const char* resource_name( )
{
   return TVXResourceTraits< T >::name;
}



struct IVXResource
{
   virtual ~IVXResource( ) = default;
};

template< typename tType, auto tCreator, auto tReleaser >
struct TVXResource : public IVXResource
{
   using RESOURCE_TYPE = tType;
   using TYPE = TVXResource< tType, tCreator, tReleaser >;
   using tSptr = std::shared_ptr< TYPE >;
   using tUptr = std::unique_ptr< TYPE >;

   static constexpr const char* name = TVXResourceTraits< TYPE >::name;

   template< typename... Args >
   TVXResource( Args... args ) : resource( tCreator( args... ) )
   {
   }

   ~TVXResource( ) override
   {
      tReleaser( &resource );
   }

   template< typename... Args >
   static tSptr create( Args... args )
   {
      auto ptr = std::make_shared< TYPE >( args... );
      vx_status status = vxGetStatus( (vx_reference)(ptr->resource) );
      if( VX_SUCCESS != status )
      {
         MSG_ERR( "error: %d", status );
         return nullptr;
      }
      return ptr;
   }

   RESOURCE_TYPE resource;
};

template< auto tCreator >
struct TVXNode : public TVXResource< vx_node, tCreator, vxReleaseNode  >
{
   using Base = TVXResource< vx_node, tCreator, vxReleaseNode >;

   using RESOURCE_TYPE = typename Base::RESOURCE_TYPE;
   using TYPE = TVXNode< tCreator >;
   using tSptr = std::shared_ptr< TYPE >;
   using tUptr = std::unique_ptr< TYPE >;

   static constexpr const char* name = TVXResourceTraits< TYPE >::name;

   using Base::Base; // inherit constructors

   template< typename... Args >
   static tSptr create( Args... args )
   {
      auto ptr = std::make_shared< TYPE >( args... );

      vx_status status = vxGetStatus(
         reinterpret_cast< vx_reference >( ptr->resource )
      );

      if( VX_SUCCESS != status )
      {
         MSG_ERR( "error: %d", status );
         return nullptr;
      }

      return ptr;
   }
};



#define DEFINE_RESOURCE_TYPE_TRAITS( NAME )              \
   template< >                                           \
   struct TVXResourceTraits< NAME >                      \
   {                                                     \
      static constexpr const char *name = #NAME;         \
   };

#define DEFINE_RESOURCE( NAME, TYPE, CREATOR, RELEASER ) \
   using NAME = TVXResource< TYPE, CREATOR,  RELEASER >; \
   DEFINE_RESOURCE_TYPE_TRAITS( NAME );

#define DEFINE_RESOURCE_NODE( NAME, CREATOR )            \
   using NAME = TVXNode< CREATOR >;                      \
   DEFINE_RESOURCE_TYPE_TRAITS( NAME );



DEFINE_RESOURCE( VXContext,   vx_context, vxCreateContext,  vxReleaseContext  );
DEFINE_RESOURCE( VXGraph,     vx_graph,   vxCreateGraph,    vxReleaseGraph    );
DEFINE_RESOURCE( VXImage,     vx_image,   vxCreateImage,    vxReleaseImage    );
DEFINE_RESOURCE( VXScalar,    vx_scalar,  vxCreateScalar,   vxReleaseScalar   );

DEFINE_RESOURCE_NODE( VXNodeGaussian3x3,  vxGaussian3x3Node    );
DEFINE_RESOURCE_NODE( VXNodeSobel3x3,     vxSobel3x3Node       );
DEFINE_RESOURCE_NODE( VXNodeMagnitude,    vxMagnitudeNode      );
DEFINE_RESOURCE_NODE( VXNodeConvertDepth, vxConvertDepthNode   );



template< typename... Ts >
struct TypeList
{
};

using VXAllResources = TypeList<
      VXContext,
      VXGraph,
      VXImage,
      VXScalar,

      VXNodeGaussian3x3,
      VXNodeSobel3x3,
      VXNodeMagnitude,
      VXNodeConvertDepth
   >;

template< typename List >
struct ForEachType;

template< typename... Ts >
struct ForEachType< TypeList< Ts... > >
{
   template< typename F >
   static void apply( F &&f )
   {
      ( f.template operator( )< Ts >( ), ... );
   }
};

struct PrintResourceNames
{
   template< typename T >
   void operator( )( ) const
   {
      MSG_DBG( "Resource: %s", T::name );
   }
};
