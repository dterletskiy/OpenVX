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

#include "Resources.hpp"
#include "trace.hpp"



class VXResourceStorage
{
   public:
      template< typename T, typename... Args >
      bool create( const std::string &name, Args... args )
      {
         auto resource_wrapper = T::create( args... );
         if( not resource_wrapper )
         {
            MSG_ERR( "cretae resource '%s' error with name '%s'",
                  T::name, name.c_str( )
               );
            return false;
         }
         auto result = 
            get_map< T >( ).emplace( name, std::move( resource_wrapper ) );
         if( not result.second )
         {
            MSG_ERR( "resource '%s' with name '%s' already exists",
                  T::name, name.c_str( )
               );
         }
         else
         {
            MSG_DBG( "created resource '%s' with name '%s'",
                  T::name, name.c_str( )
               );
            register_type< T >( );
         }

         return result.second;
      }

      template< typename T >
      void remove( )
      {
         get_map< T >( ).clear( );
      }

      template< typename T >
      void remove( const std::string &name )
      {
         return get_map< T >( ).erase( name ) > 0;
      }

      void clear( )
      {
         for( auto &fn : m_clear_functions )
         {
            fn( );
         }
      }

      template< typename T >
      typename T::RESOURCE_TYPE get( const std::string &name ) const
      {
         return get_resource< T >( name );
      }

      template< typename T >
      std::shared_ptr< T > get_wrapper( const std::string &name ) const
      {
         const auto &map = get_map< T >( );
         auto it = map.find( name );
         if( map.end( ) == it  )
         {
            MSG_ERR( "resource '%s' with name '%s' not found",
                  T::name, name.c_str( )
               );
            return nullptr;
         }

         return it->second;
      }

      template< typename T >
      typename T::RESOURCE_TYPE get_resource( const std::string &name ) const
      {
         auto wrapper = get_wrapper< T >( name );
         return wrapper ? wrapper->resource : nullptr;
      }

   private:
      template< typename T >
      void register_type( )
      {
         static bool registered = false;
         if( not registered )
         {
            m_clear_functions.emplace_back(
               [ this ]( )
               {
                  MSG_DBG( "release resource: '%s'", T::name );
                  get_map< T >( ).clear( );
               }
            );
            registered = true;
            MSG_INF( "registered new resource type: '%s'", T::name );
         }
      }

   private:
      template< typename T >
      using Map = std::unordered_map< std::string, std::shared_ptr< T > >;

      template< typename T >
      Map< T > &get_map( ) const
      {
         static Map< T > map;
         return map;
      }

      std::vector< std::function< void( ) > > m_clear_functions;
};
