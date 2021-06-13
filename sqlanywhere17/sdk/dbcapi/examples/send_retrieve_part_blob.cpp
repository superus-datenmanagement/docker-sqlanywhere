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
    fprintf( stderr, "Usage: send_retrieve_part_blob [options] \n" );
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
    a_sqlany_connection *sqlany_conn;
    a_sqlany_stmt 	*sqlany_stmt;
    unsigned int	 i;
    unsigned char	 buffer[4096]; 
    unsigned int	 size;
    int    	 	 code;
    int			 num_cols;
    unsigned char	 row_pattern[2] = { 'a', 'b' };
    unsigned int	 row_sizes[2] = { 1024*1024, 512*1024 };
    int			 bytes_read;
    size_t  		 total_bytes_read;
    unsigned int	 max_api_ver;
    sacapi_bool		 ok;
    int 		 row_num;

    argc = ProcessOptions( argv );
    if( argc < 0 ) {
        return -1;
    }

    if( !sqlany_initialize_interface( &api, NULL ) ) {
	printf( "Could not initialize the interface!\n" );
	exit( 0 );
    }

    ok = api.sqlany_init( "my_php_app", SQLANY_API_VERSION_5, &max_api_ver );
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

    // 1. Starting to insert blob operation
    sqlany_stmt = api.sqlany_prepare( sqlany_conn, "insert into my_blob_table( size, data) values( ?, ? )" ); 
    assert( sqlany_stmt != NULL );


    // 1.1 We must first bind the parameters
    a_sqlany_bind_param param;

    api.sqlany_describe_bind_param( sqlany_stmt, 0, &param );
    param.value.buffer = (char *)&size;
    param.value.type   = A_VAL32;
    param.value.is_null= NULL;
    param.direction    = DD_INPUT;
    api.sqlany_bind_param( sqlany_stmt, 0, &param );

    api.sqlany_describe_bind_param( sqlany_stmt, 1, &param );
    param.value.buffer = NULL;
    param.value.type   = A_BINARY;
    param.value.is_null= NULL;
    param.direction    = DD_INPUT;
    api.sqlany_bind_param( sqlany_stmt, 1, &param );

    for( row_num = 0; row_num < 2; row_num++ ) {
	// 1.2 upload the blob data to the server in chunks
	size = row_sizes[row_num];
	for( i = 0; i < sizeof(buffer); i++ ) {
	    buffer[i] = row_pattern[row_num];
	}
	api.sqlany_reset_param_data( sqlany_stmt, 1 );
	for( i = 0; i < size; i += 4096 ) {
	    if( !api.sqlany_send_param_data( sqlany_stmt, 1, (char *)buffer, 4096 )) {
		char msg[SACAPI_ERROR_SIZE];
		code = api.sqlany_error( sqlany_conn, msg, sizeof(msg) );
		printf( "Could not send param[%d]:%s\n", code, msg );
	    }
	}

	// 1.3 actually do the row insert operation
	ok = api.sqlany_execute( sqlany_stmt );
	assert( ok );
    }

    api.sqlany_commit( sqlany_conn );

    api.sqlany_free_stmt( sqlany_stmt );

    // 2. Now let's retrieve the blob
    sqlany_stmt = api.sqlany_execute_direct( sqlany_conn, "select * from my_blob_table" );
    assert( sqlany_stmt != NULL );

    num_cols = api.sqlany_num_cols( sqlany_stmt );
    assert( num_cols == 2 );

    row_num = 0;
    while( true ) {
	ok = api.sqlany_fetch_next( sqlany_stmt );
	if( !ok ) {
	    break;
	}

	a_sqlany_data_value	 value;
	api.sqlany_get_column( sqlany_stmt, 0, &value );

	assert( value.type == A_VAL32 );
	assert( (*(unsigned int *)value.buffer) == row_sizes[row_num] );

	a_sqlany_data_info 	 dinfo;
	api.sqlany_get_data_info( sqlany_stmt, 1, &dinfo );

	assert( dinfo.type == A_BINARY );
	assert( dinfo.data_size == row_sizes[row_num] );
	assert( dinfo.is_null == 0 );

	// 2.1 Retrieve data in 4096 byte chunks
	total_bytes_read = 0;
	while( 1 ) {
	    bytes_read = api.sqlany_get_data( sqlany_stmt, 1, total_bytes_read, buffer, sizeof(buffer) );
	    if( bytes_read <= 0 ) {
		break;
	    }
	    // verify the buffer contents
	    for( i = 0; i < (unsigned int)bytes_read; i++ ) {
		assert( buffer[i] == row_pattern[row_num] );
	    }
	    total_bytes_read += bytes_read;
	}
	assert( total_bytes_read == row_sizes[row_num] );
	row_num++;
    }
    assert( row_num == 2 );

    api.sqlany_free_stmt( sqlany_stmt );

    api.sqlany_disconnect( sqlany_conn );

clean:
    api.sqlany_free_connection( sqlany_conn );

    api.sqlany_fini();

    sqlany_finalize_interface( &api );

    printf( "Success!\n" );
}

