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

char * ConnectStr = "";

static void Usage()
/*****************/
{
    fprintf( stderr, "Usage: preparing_statements [options] \n" );
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
	api.sqlany_free_connection( sqlany_conn );
	api.sqlany_fini();
	sqlany_finalize_interface( &api );
	exit( -1 );
    }
			
    printf( "Connected successfully!\n" );

    api.sqlany_execute_immediate( sqlany_conn, "drop procedure foo" );
    api.sqlany_execute_immediate( sqlany_conn, 
	    "create procedure foo ( IN prefix char(10), 	\n"
	    "                       INOUT buffer varchar(256),	\n"
	    "                       OUT str_len int, 		\n"
	    "                       IN suffix char(10) ) \n"
	    "begin					 \n"
	    "    set buffer = prefix || buffer || suffix;\n"
	    "    select length( buffer ) into str_len;	 \n"
	    "end					 \n" );
    sqlany_stmt = api.sqlany_prepare( sqlany_conn, "call foo( ?, ?, ?, ? )" );

    if( sqlany_stmt ) {

	a_sqlany_bind_param 	param;
	char	   		buffer[256] = "-some_string-";
	int			str_len;
	size_t			buffer_size = strlen(buffer);
	size_t			prefix_length = 6;
	size_t			suffix_length = 6;

	ok = api.sqlany_describe_bind_param( sqlany_stmt, 0, &param );
	assert( ok );
	param.value.buffer = (char *)"PREFIX";
	param.value.length = &prefix_length;
	ok = api.sqlany_bind_param( sqlany_stmt, 0, &param );
	assert( ok );

	ok = api.sqlany_describe_bind_param( sqlany_stmt, 1, &param );
	assert( ok );
	param.value.buffer = buffer;
	param.value.length = &buffer_size;
	//params[1].value.type	    = A_STRING;		  // already set by sqlany_describe_bind_param()
	//params[1].direction	    = INPUT_OUTPUT;	  // already set by sqlany_describe_bind_param()
	param.value.buffer_size = sizeof(buffer); 	  // IMPORTANT: this field must be set for 
							  // OUTPUT and INPUT_OUTPUT parameters so that 
							  // the library knows how much data can be written
							  // into the buffer
	ok = api.sqlany_bind_param( sqlany_stmt, 1, &param );
	assert( ok );
							  

	ok = api.sqlany_describe_bind_param( sqlany_stmt, 2, &param );
	assert( ok );
	param.value.buffer	= (char *)&str_len;		  
	param.value.is_null     = NULL; 		  // use NULL if not interested in nullability
	//param.value.type	    = A_VAL32;		  // already set by sqlany_describe_bind_param()
	//param.direction	    = OUTPUT_ONLY; 	  // already set by sqlany_describe_bind_param()
	//param.value.buffer_size = sizeof(str_len);	  // for non string or binary buffers, buffer_size is not needed
	ok = api.sqlany_bind_param( sqlany_stmt, 2, &param );
	assert( ok );

	ok = api.sqlany_describe_bind_param( sqlany_stmt, 3, &param );
	assert( ok );
	param.value.buffer      = (char *)"SUFFIX";
	param.value.length      = &suffix_length;
	//params.value.type	= A_STRING; 	  	  // already set by sqlany_describe_bind_param()
	ok = api.sqlany_bind_param( sqlany_stmt, 3, &param );
	assert( ok );

	/* We are not expecting a result set so the result set parameter could be NULL */
	if( api.sqlany_execute( sqlany_stmt ) ) {
	    printf( "Complete string is %s and is %d chars long \n", buffer, str_len );
	    assert( str_len == (6+13+6) );

	    buffer_size = str_len;
	    api.sqlany_execute( sqlany_stmt );
	    printf( "Complete string is %s and is %d chars long \n", buffer, str_len );
	    assert( str_len == 6+(6+13+6)+6 );
	} else {
	    char buffer[SACAPI_ERROR_SIZE];
	    int  rc;
	    rc = api.sqlany_error( sqlany_conn, buffer, sizeof(buffer));
	    printf( "Failed to execute! [%d] %s\n", rc, buffer );
	}

	/* Free the statement object or there will be a memory leak */
	api.sqlany_free_stmt( sqlany_stmt );
    }

    api.sqlany_disconnect( sqlany_conn );

    /* Must free the connection object or there will be a memory leak */
    api.sqlany_free_connection( sqlany_conn );

    api.sqlany_fini();

    sqlany_finalize_interface( &api );

    return 0;
}
