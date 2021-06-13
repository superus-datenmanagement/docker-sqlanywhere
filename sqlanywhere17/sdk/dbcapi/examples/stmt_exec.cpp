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
    fprintf( stderr, "Usage: stmt_exec [options] \n" );
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

struct a_value {
    int		id;
    char 	name[25];
};

struct a_value values[] = 
{
    { 1, "First" },
    { 2, "First" },
    { 2, "Second" },
    { 3, "First" },
    { 3, "Second" },
    { 3, "Third" },
    { 4, "" },
    { 5, "" }
};

int main( int argc, char * argv[] )
{
    a_sqlany_connection * sqlany_conn;
    a_sqlany_stmt	* sqlany_stmt;
    unsigned int	  max_api_ver;
    int			  ok;

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

    api.sqlany_execute_immediate( sqlany_conn, "drop table if exists foo" );
    ok = api.sqlany_execute_immediate( sqlany_conn, "create table foo ( id integer, name char(20), null_field char(50))" );
    assert( ok );
    sqlany_stmt = api.sqlany_prepare( sqlany_conn, "insert into foo values( ?, ?, ? )" );
    assert( sqlany_stmt );
    {
	a_sqlany_bind_param 	param;
	int 			id = 0;
	char			name[25];
	size_t			name_len;
	sacapi_bool		is_null;
	sacapi_bool		null_field = sacapi_true;

	ok = api.sqlany_describe_bind_param( sqlany_stmt, 0, &param );
	assert( ok );
	param.value.buffer  = (char *)&id;
	param.value.is_null = NULL;
	param.value.type    = A_UVAL32;
	ok = api.sqlany_bind_param( sqlany_stmt, 0, &param );
	assert( ok );

	ok = api.sqlany_describe_bind_param( sqlany_stmt, 1, &param );
	assert( ok );
	param.value.buffer  	= name;
	param.value.buffer_size = sizeof(name);
	param.value.length  = &name_len;
	param.value.is_null = &is_null;
	param.value.type    = A_STRING;
	ok = api.sqlany_bind_param( sqlany_stmt, 1, &param );
	assert( ok );

	ok = api.sqlany_describe_bind_param( sqlany_stmt, 2, &param );
	assert( ok );
	//param.value.buffer  	= NULL;
	//param.value.buffer_size = 0;
	//param.value.length  = NULL;
	param.value.is_null = &null_field;
	//param.value.type    = A_S;
	ok = api.sqlany_bind_param( sqlany_stmt, 2, &param );
	assert( ok );

	for( unsigned int i = 0; i < sizeof(values)/sizeof(struct a_value); i++ ) {
	    id = values[i].id;
	    if( values[i].name[0] == '\0' ) {
		is_null = sacapi_true;
	    } else {
		is_null = sacapi_false;
		strcpy( name, values[i].name );
		name_len = strlen( name );
	    }
	    ok = api.sqlany_execute( sqlany_stmt );
	    assert( ok );
	}
	ok = api.sqlany_commit( sqlany_conn );
	assert( ok );
    }

    sqlany_stmt = api.sqlany_prepare( sqlany_conn, "select * from foo where id = ?" );
    assert( sqlany_stmt );
    {
	a_sqlany_bind_param 	param;
	int 			value;

	ok = api.sqlany_describe_bind_param( sqlany_stmt, 0, &param );
	assert( ok );
	param.value.buffer  = (char *)&value;
	param.value.is_null = NULL;
	param.value.type    = A_UVAL32;
	ok = api.sqlany_bind_param( sqlany_stmt, 0, &param );
	assert( ok );

	for( unsigned int i = 0; i < sizeof(values)/sizeof(struct a_value); i++ ) {

	    value = values[i].id;
	    /* We are not expecting a result set so the result set parameter could be NULL */
	    if( !api.sqlany_execute( sqlany_stmt ) ) {
		print_error( sqlany_conn, (char *)"Execute failed" );
		break;
	    }
	    {
		char		err_mesg[256];
		int		err_code;
		int		row_count = 0;

		assert( api.sqlany_num_cols( sqlany_stmt ) == 3 );

		while( api.sqlany_fetch_next( sqlany_stmt ) ) {
		    a_sqlany_data_value dvalue;
		    int			found = 0;
		    int			id;

		    row_count++;
		    ok = api.sqlany_get_column( sqlany_stmt, 0, &dvalue );
		    assert( ok );
		    assert( dvalue.type == A_VAL32 );
		    id = *(int*)dvalue.buffer;
		    ok = api.sqlany_get_column( sqlany_stmt, 1, &dvalue );
		    assert( ok );
		    assert( dvalue.type == A_STRING );

		    for( unsigned int i = 0; i < sizeof(values)/sizeof(struct a_value); i++ ) {
			if( values[i].id == id ) {
			    if( values[i].name[0] == '\0' ) {
				assert( *(dvalue.is_null) == 1 );
				found = 1;
				break;
			    } else if( strncmp( values[i].name, dvalue.buffer, strlen(values[i].name) ) == 0 ) { 
				found = 1;
				break;
			    }
			}
		    }
		    assert( found );

		}
		assert( row_count > 0 );
		err_code = api.sqlany_error( sqlany_conn, err_mesg, sizeof(err_mesg) );

		ok = api.sqlany_get_next_result( sqlany_stmt );
		assert( !ok );
		err_code = api.sqlany_error( sqlany_conn, err_mesg, sizeof(err_mesg));
		assert( err_code == 105 );
		char sqlstate[6];
		api.sqlany_sqlstate( sqlany_conn, sqlstate, sizeof(sqlstate) );
		assert( strcmp( sqlstate, "01W05" ) == 0 );
	    }
	}
    }
    ok = api.sqlany_commit( sqlany_conn );
    assert( ok );
    api.sqlany_free_stmt( sqlany_stmt );

    api.sqlany_disconnect( sqlany_conn );

    /* Must free the connection object or there will be a memory leak */
    api.sqlany_free_connection( sqlany_conn );

    api.sqlany_fini();

    sqlany_finalize_interface( &api );
 
    return 0;
}
