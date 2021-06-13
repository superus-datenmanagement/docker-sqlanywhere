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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "sacapidll.h"

char * ConnectStr = "";

static void Usage()
/*****************/
{
    fprintf( stderr, "Usage: send_retrieve_full_blob [options] \n" );
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
    SQLAnywhereInterface api;
    a_sqlany_connection *sqlany_conn;
    a_sqlany_stmt 	*sqlany_stmt;
    unsigned int	 i;
    unsigned char	*data;
    unsigned int	 size = 1024*1024; // 1MB blob
    size_t		 my_size;
    int    	 	 code;
    a_sqlany_data_value  value;
    int 		 num_cols;
    unsigned int	 max_api_ver;
    a_sqlany_bind_param  param;
    sacapi_bool		 ok;

    argc = ProcessOptions( argv );
    if( argc < 0 ) {
        return -1;
    }

    if( !sqlany_initialize_interface( &api, NULL ) ) {
	printf( "Could not initialize the interface!\n" );
	exit( 0 );
    }

    ok = api.sqlany_init( "my_php_app", SQLANY_API_VERSION_1, &max_api_ver );
    assert( ok );
    sqlany_conn = api.sqlany_new_connection();

    if( !api.sqlany_connect( sqlany_conn, ConnectStr ) ) {
	char buffer[SACAPI_ERROR_SIZE];
	code = api.sqlany_error( sqlany_conn, buffer, sizeof(buffer) );
	printf( "Could not connection[%d]:%s\n", code, buffer );
	goto clean;
    }

    printf( "Connected successfully!\n" );

    api.sqlany_execute_immediate( sqlany_conn, "drop table my_blob_table" );
    ok = api.sqlany_execute_immediate( sqlany_conn, "create table my_blob_table (size integer, data long binary)" );
    assert( ok );

    sqlany_stmt = api.sqlany_prepare( sqlany_conn, "insert into my_blob_table( size, data ) values( ?, ?)" );
    assert( sqlany_stmt != NULL );

    data = (unsigned char *)malloc( size );
    // initialize the buffer
    for( i = 0; i < size; i++ ) {
	data[i] = i % 256;
    }


    // initialize the parameters
    api.sqlany_describe_bind_param( sqlany_stmt, 0, &param );
    param.value.buffer = (char *)&size;
    param.value.type   = A_VAL32;		// This needs to be set as the server does not 
    						// know what data will be inserting.
    api.sqlany_bind_param( sqlany_stmt, 0, &param );

    my_size = size;
    api.sqlany_describe_bind_param( sqlany_stmt, 1, &param );
    param.value.buffer = (char *)data;
    param.value.length = &my_size;
    param.value.type   = A_BINARY;		// This needs to be set for the same reason as above.
    api.sqlany_bind_param( sqlany_stmt, 1, &param );

    ok = api.sqlany_execute( sqlany_stmt );
    assert( ok );

    api.sqlany_free_stmt( sqlany_stmt );

    api.sqlany_commit( sqlany_conn );

    sqlany_stmt = api.sqlany_execute_direct( sqlany_conn, "select * from my_blob_table" );
    assert( sqlany_stmt != NULL );

    ok = api.sqlany_fetch_next( sqlany_stmt );
    assert( ok );

    num_cols = api.sqlany_num_cols( sqlany_stmt );

    assert( num_cols == 2 );

    api.sqlany_get_column( sqlany_stmt, 0, &value );

    assert( *((unsigned int*)value.buffer) == size );
    assert( value.type == A_VAL32 );

    api.sqlany_get_column( sqlany_stmt, 1, &value );

    assert( value.type == A_BINARY );
    assert( *(value.length) == my_size );

    for( i = 0; i < (*value.length); i++ ) {
	assert( (unsigned char)(value.buffer[i]) == data[i]);
    }

    ok = api.sqlany_fetch_next( sqlany_stmt );
    assert( !ok );
    api.sqlany_free_stmt( sqlany_stmt );

    api.sqlany_disconnect( sqlany_conn );

clean:
    api.sqlany_free_connection( sqlany_conn );

    api.sqlany_fini();

    sqlany_finalize_interface( &api );
    printf( "Success!\n" );
}

