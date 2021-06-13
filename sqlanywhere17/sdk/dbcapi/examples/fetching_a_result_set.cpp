// ***************************************************************************
// Copyright (c) 2019 SAP SE or an SAP affiliate company. All rights reserved.
// ***************************************************************************
// This sample code is provided AS IS, without warranty or liability
// of any kind.
// 
// You may use, reproduce, modify and distribute this sample code
// without limitation, on the condition that you retain the foregoing
// copyright notice and disclaimer as to the original code.
// 
// *********************************************************************
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sacapidll.h"

char * ConnectStr = "";

static void Usage()
/*****************/
{
    fprintf( stderr, "Usage: fetching_a_result_set [options] \n" );
    fprintf( stderr, "Options:\n" );
    fprintf( stderr, "   -c conn_str     : database connection string (required)\n" );
}

static int ArgumentIsASwitch( char * arg )
/****************************************/
{
#if defined( UNIX )
    return ( arg[0] == '-' );
#else
    return ( arg[0] == '-' ) || ( arg[0] == '/' );
#endif
}

static int ProcessOptions( char * argv[] )
/****************************************/
{
    int             argc;
    char *          arg;
    char            opt;

#define _get_arg_param()                                                \
            arg += 2;                                                   \
            if( !arg[0] ) arg = argv[++argc];                           \
            if( arg == NULL ) {                                         \
                fprintf( stderr, "Missing argument parameter\n" );      \
                return( -1 );                                           \
            }

    for( argc = 1; (arg = argv[argc]) != NULL; ++ argc ) {
        if( !ArgumentIsASwitch( arg ) ) break;
        opt = arg[1];
        switch( opt ) {
        case 'c':
            _get_arg_param();
            ConnectStr = arg;
            break;
        default:
            fprintf( stderr, "**** Unknown option: -%c\n", opt );
            Usage();
            return( -1 );
        }
    }
    if( ConnectStr[0] == '\0' ) {
        fprintf( stderr, "A database connection string is required.\n" );
        Usage();
        return( -1 );
    }
    return( argc );
}

int main( int argc, char * argv[] )
{
    SQLAnywhereInterface  api;
    a_sqlany_connection * sqlany_conn;
    a_sqlany_stmt	* sqlany_stmt;
    unsigned int	  max_api_ver;

    argc = ProcessOptions( argv );
    if( argc < 0 ) {
        return -1;
    }

    if( !sqlany_initialize_interface( &api, NULL ) ) {
	printf( "Could not initialize the interface!\n" );
	exit( 0 );
    } 

    if( !api.sqlany_init( "MyAPP", SQLANY_API_VERSION_1, &max_api_ver ) ) {
	printf( "Failed to initialize the interface! Supported version=%d\n", max_api_ver );
	sqlany_finalize_interface( &api );
	return -1;
    }

    /* A connection object needs to be created first */
    sqlany_conn = api.sqlany_new_connection();

    if( !api.sqlany_connect( sqlany_conn, ConnectStr ) ) { 
	api.sqlany_free_connection( sqlany_conn );
	api.sqlany_fini();
	sqlany_finalize_interface( &api );
	exit( -1 );
    }
			
    printf( "Connected successfully!\n" );


    if( (sqlany_stmt = api.sqlany_execute_direct( sqlany_conn, "select * from systable" )) != NULL ) {
	int 			num_rows = 0;
	a_sqlany_data_value	value;

	while( api.sqlany_fetch_next( sqlany_stmt ) ) {
	    
	    num_rows++;
	    printf( "\nRow [%d] data .......\n", num_rows );
	    for( int i = 0; i < api.sqlany_num_cols( sqlany_stmt ); i++ ) {
		
		if( !api.sqlany_get_column( sqlany_stmt, i, &value ) ) {
		    printf( "Getting column data failed!\n" );
		    break;
		}

		if( *(value.is_null) ) {
		    printf( "Received a NULL value\n" );
		    continue;
		}

		switch( value.type ) {
		    case A_BINARY:
			printf( "Binary value of length %llu.\n",
                                (unsigned long long)*(value.length) );
			break;
		    case A_STRING:
			printf( "String value [%.*s] of length %llu.\n", 
				(int)*(value.length), (char *)value.buffer,
                                (unsigned long long)*(value.length) );
			break;
		    case A_VAL64:
			printf( "A_VAL64 value [%lld].\n", *(long long *)value.buffer );
			break;
		    case A_UVAL64:
			printf( "A_UVAL64 value [%lld].\n", *(unsigned long long *)value.buffer );
			break;
		    case A_VAL32:
			printf( "A_VAL32 value [%d].\n", *(int*)value.buffer );
			break;
		    case A_UVAL32:
			printf( "A_UVAL32 value [%d].\n", *(unsigned int*)value.buffer );
			break;
		    case A_VAL16:
			printf( "A_VAL16 value [%d].\n", *(short*)value.buffer );
			break;
		    case A_UVAL16:
			printf( "A_UVAL16 value [%d].\n", *(unsigned short*)value.buffer );
			break;
		    case A_VAL8:
			printf( "A_VAL8 value [%d].\n", *(char *)value.buffer );
			break;
		    case A_UVAL8:
			printf( "A_UVAL8 value [%d].\n", *(unsigned char *)value.buffer );
			break;
		    case A_DOUBLE:
			printf( "A_DOUBLE value [%f].\n", *(double *)value.buffer );
			break;
                default: break;
		}
		/* do some processing with the data ... */
	    }
	}

	/* Must free the result set object when done with it */
	api.sqlany_free_stmt( sqlany_stmt );
    }	    
    api.sqlany_disconnect( sqlany_conn );

    /* Must free the connection object or there will be a memory leak */
    api.sqlany_free_connection( sqlany_conn );

    api.sqlany_fini();

    sqlany_finalize_interface( &api );

    return 0;
}
