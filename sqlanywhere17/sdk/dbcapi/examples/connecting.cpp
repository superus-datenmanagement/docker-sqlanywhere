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
    fprintf( stderr, "Usage: connecting [options] \n" );
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
    a_sqlany_connection   *sqlany_conn;
    unsigned int	  max_api_ver;

    argc = ProcessOptions( argv );
    if( argc < 0 ) {
	return -1;
    }

    if( !sqlany_initialize_interface( &api, NULL ) ) {
	printf( "Could not initialize the interface!\n" );
	exit( 0 );
    }

    if( !api.sqlany_init( "MyAPP", SQLANY_API_VERSION_1, &max_api_ver )) {
	printf( "Failed to initialize the interface! Supported version = %d\n", max_api_ver );
	sqlany_finalize_interface( &api );
	return -1;
    }

    /* A connection object needs to be created first */
    sqlany_conn = api.sqlany_new_connection();

    if( !api.sqlany_connect( sqlany_conn, ConnectStr ) ) { 
        /* failed to connect */
	char buffer[SACAPI_ERROR_SIZE];
	char sqlstate[6];
	int  rc;
	rc = api.sqlany_error( sqlany_conn, buffer, sizeof(buffer) );
	api.sqlany_sqlstate( sqlany_conn, sqlstate, sizeof(sqlstate) );
	printf( "Failed to connect: error code=%d error message=%s, sqlstate=%s\n",
	    rc, buffer, sqlstate );
    } else {
	printf( "Connected successfully!\n" );
	api.sqlany_disconnect( sqlany_conn );
    }

    /* Must free the connection object or there will be a memory leak */
    api.sqlany_free_connection( sqlany_conn );

    api.sqlany_fini();

    sqlany_finalize_interface( &api );

    return 0;
}
