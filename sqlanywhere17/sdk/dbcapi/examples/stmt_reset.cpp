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
#include <assert.h>

SQLAnywhereInterface  api;

char * ConnectStr = "";

static void Usage()
/*****************/
{
    fprintf( stderr, "Usage: stmt_reset [options] \n" );
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

void
print_error( a_sqlany_connection * sqlany_conn, char * str )
{
    char buffer[SACAPI_ERROR_SIZE];
    int  rc;
    rc = api.sqlany_error( sqlany_conn, buffer, sizeof(buffer));
    printf( "%s: [%d] %s\n", str, rc, buffer );
}

int main( int argc, char * argv[] )
{
    a_sqlany_connection * sqlany_conn;
    a_sqlany_stmt	* sqlany_stmt;
    unsigned int	  max_api_ver;
    sacapi_bool		  ok;

    argc = ProcessOptions( argv );
    if( argc < 0 ) {
        return -1;
    }

    if( !sqlany_initialize_interface( &api, NULL ) ) {
	printf( "Could not initialize the interface!\n" );
	exit( 0 );
    }

    if( !api.sqlany_init( "MyAPP", SQLANY_API_VERSION_1, &max_api_ver )) {
	printf( "Failed to initialize the interface! Supported version=%d\n", max_api_ver );
	sqlany_finalize_interface( &api );
	return -1;
    }

    /* A connection object needs to be created first */
    sqlany_conn = api.sqlany_new_connection();

    if( !api.sqlany_connect( sqlany_conn, ConnectStr ) ) { 
	print_error( sqlany_conn, (char *)"Failed connecting" );
	api.sqlany_free_connection( sqlany_conn );
	api.sqlany_fini();
	sqlany_finalize_interface( &api );
	exit( -1 );
    }
    printf( "Connected successfully!\n" );

    api.sqlany_execute_immediate( sqlany_conn, "drop table foo" );
    api.sqlany_execute_immediate( sqlany_conn, "create table foo ( id integer, name char(20))" );

    sqlany_stmt = api.sqlany_prepare( sqlany_conn, "insert into foo values( ?, ? )" );
    assert( sqlany_stmt );

    int i= 0;
    while( i < 10 ) {

	a_sqlany_bind_param 	param;
	char	   		buffer[256];
	size_t			buffer_length;

	ok = api.sqlany_describe_bind_param( sqlany_stmt, 0, &param );
	assert( ok );
	param.value.buffer  = (char *)&i;
	param.value.is_null = NULL;
	param.value.type    = A_UVAL32;
	ok = api.sqlany_bind_param( sqlany_stmt, 0, &param );
	assert( ok );

	ok = api.sqlany_describe_bind_param( sqlany_stmt, 1, &param );
	assert( ok );
	param.value.buffer = buffer;
	param.value.length = &buffer_length;
	param.value.type   = A_STRING;
	ok = api.sqlany_bind_param( sqlany_stmt, 1, &param );
	assert( ok );
							  
	sprintf( buffer, "Entry %d", i );
	buffer_length = strlen( buffer );

	/* We are not expecting a result set so the result set parameter could be NULL */
	if( !api.sqlany_execute( sqlany_stmt ) ) {
	    print_error( sqlany_conn, (char *)"Execute failed" );
	    break;
	}
	api.sqlany_commit( sqlany_conn );

	/* Free the statement object or there will be a memory leak */
	api.sqlany_reset( sqlany_stmt );
	i++;
    }

    api.sqlany_free_stmt( sqlany_stmt );


    sqlany_stmt = api.sqlany_execute_direct( sqlany_conn, "select * from foo" );
    assert( sqlany_stmt );
    while( api.sqlany_fetch_next( sqlany_stmt ) ) {
	i--;
    }
    assert( i == 0 );
    int  err_code;
    char err_msg[256];
    char sqlstate[6];
    err_code = api.sqlany_error( sqlany_conn, err_msg, sizeof(err_msg) );
    api.sqlany_sqlstate( sqlany_conn, sqlstate, sizeof(sqlstate) );
    assert( err_code == 100 );
    assert( strcmp( sqlstate, "02000" ) == 0 );

    api.sqlany_clear_error( sqlany_conn );
    err_code = api.sqlany_error( sqlany_conn, err_msg, sizeof(err_msg) );
    api.sqlany_sqlstate( sqlany_conn, sqlstate, sizeof(sqlstate) );
    assert( err_code == 0 );
    assert( strcmp( sqlstate, "00000" ) == 0 );

    api.sqlany_free_stmt( sqlany_stmt );


    api.sqlany_disconnect( sqlany_conn );

    /* Must free the connection object or there will be a memory leak */
    api.sqlany_free_connection( sqlany_conn );

    api.sqlany_fini();

    sqlany_finalize_interface( &api );

    return 0;
}
