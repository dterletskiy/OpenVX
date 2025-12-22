#pragma once



#include <stdio.h>
#include <string.h>

#include "Colors.hpp"



#ifdef __GNUC__
   #define __FILENAME__ \
      ( __builtin_strrchr( __FILE__, '/' ) ? __builtin_strrchr( __FILE__, '/' ) + 1 : __FILE__ )
#else
   #define __FILENAME__ \
      ( strrchr( __FILE__, '/' ) ? strrchr( __FILE__, '/' ) + 1 : __FILE__ )
#endif

#define PREFIX_FORMAT "[%s:%s:%d] -> "

#if 0

   #define DBG_COMMON( LOG_LEVEL, USER_FORMAT, ... ) \
      do \
      { \
         printf( LOG_LEVEL PREFIX_FORMAT USER_FORMAT, \
               __FILENAME__, __func__, __LINE__, ##__VA_ARGS__ \
            ); \
         printf( "\n" ); \
      } while( false )

   #define MSG_INF( USER_FORMAT, ... ) \
      DBG_COMMON( "[INF] ", USER_FORMAT, ##__VA_ARGS__ )

   #define MSG_DBG( USER_FORMAT, ... ) \
      DBG_COMMON( "[DBG] ", USER_FORMAT, ##__VA_ARGS__ )

   #define MSG_WRN( USER_FORMAT, ... ) \
      DBG_COMMON( "[WRN] ", USER_FORMAT, ##__VA_ARGS__ )

   #define MSG_ERR( USER_FORMAT, ... ) \
      DBG_COMMON( "[ERR] ", USER_FORMAT, ##__VA_ARGS__ )

#else

#define MSG( LOG_LEVEL, ... ) \
   do \
   { \
      if( strcmp( LOG_LEVEL, "NO_LOG_LEVEL" ) != 0 ) \
         printf( LOG_LEVEL PREFIX_FORMAT, __FILENAME__, __func__, __LINE__ ); \
      printf( " " __VA_ARGS__ ); \
      printf( "\n" ); \
   } while( false )

#define MSG_INF( ... ) \
   MSG( FG_WHITE "[INF] " RESET, " " __VA_ARGS__ )

#define MSG_DBG( ... ) \
   MSG( FG_LIGHT_GREEN "[DBG] " RESET, " " __VA_ARGS__ )

#define MSG_WRN( ... ) \
   MSG( FG_LIGHT_BLUE "[WRN] " RESET, " " __VA_ARGS__ )

#define MSG_ERR( ... ) \
   MSG( FG_LIGHT_RED "[ERR] " RESET, " " __VA_ARGS__ )

#define MSG_FTL( ... ) \
   MSG( FG_RED "[ERR] " RESET, " " __VA_ARGS__ )

#define MSG_MSG( ... ) \
   MSG( "NO_LOG_LEVEL", " " __VA_ARGS__ )

#endif



// printf( "@TDA: [%s:%s:%d] -> app = %p\n", __FILE__, __func__, __LINE__, (void*)app );
