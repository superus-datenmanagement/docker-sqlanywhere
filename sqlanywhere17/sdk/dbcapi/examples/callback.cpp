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
    fprintf( stderr, "Usage: callback [options] \n" );
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
print_error( a_sqlany_connection *sqlany_conn, const char *str )
{
    char buffer[SACAPI_ERROR_SIZE];
    int  rc;
    rc = api.sqlany_error( sqlany_conn, buffer, sizeof(buffer));
    printf( "%s: [%d] %s\n", str, rc, buffer );
}

#if defined(_WIN32)
    #define CLIENT_FILE "c:\\temp\\customers.dat"
#else
    #define CLIENT_FILE "/tmp/customers.dat"
#endif

#define ansi_min(a,b)    (((a) < (b)) ? (a) : (b))

void SQLANY_CALLBACK messages( a_sqlany_connection *sqlany_conn, a_sqlany_message_type msg_type, int sqlcode, unsigned short length, char *msg )
{
    size_t  mlen;
    char    mbuffer[80];

    switch( msg_type )
    {
	case MESSAGE_TYPE_INFO:
	    printf( "The message type is INFO.\n" );
	    break;
	case MESSAGE_TYPE_WARNING:
	    printf( "The message type is WARNING.\n" );
	    break;
	case MESSAGE_TYPE_ACTION:
	    printf( "The message type is ACTION.\n" );
	    break;
	case MESSAGE_TYPE_STATUS:
	    printf( "The message type is STATUS.\n" );
	    break;
	case MESSAGE_TYPE_PROGRESS:
	    printf( "The message type is PROGRESS.\n" );
	    break;
    }
    mlen = ansi_min( length, sizeof(mbuffer) );
    strncpy( mbuffer, msg, mlen );
    mbuffer[mlen] = '\0';
    printf( "Message is \"%s\".\n", mbuffer );
    api.sqlany_sqlstate( sqlany_conn, mbuffer, sizeof( mbuffer ) );
    printf( "SQLCode(%d) SQLState(\"%s\")\n\n", sqlcode, mbuffer );
}

int SQLANY_CALLBACK client_download( a_sqlany_connection *sqlany_conn, char * file_name, int is_write )
{
    printf( "%s file \"%s\"\n\n", is_write ? "Writing":"Reading", file_name );
    /* Always allow the transfer to proceed */
    return( 1 );
}

int SQLANY_CALLBACK client_upload( a_sqlany_connection *sqlany_conn, char * file_name, int is_write )
{
    printf( "%s file \"%s\"\n\n", is_write ? "Writing":"Reading", file_name );
    /* Always allow the transfer to proceed */
    return( 1 );
}

int main( int argc, char * argv[] )
{
    a_sqlany_connection * sqlany_conn1;
    a_sqlany_connection * sqlany_conn2;
    sacapi_bool		  registered;
    unsigned int	  max_api_ver;
    int			  rc;
    char		  buffer[SACAPI_ERROR_SIZE];

    argc = ProcessOptions( argv );
    if( argc < 0 ) {
        return -1;
    }

    if( !sqlany_initialize_interface( &api, NULL ) ) {
	printf( "Could not initialize the interface!\n" );
	exit( 0 );
    }

    if( !api.sqlany_init( "MyAPP", SQLANY_API_VERSION_3, &max_api_ver )) {
	printf( "Failed to initialize the interface! Supported version=%d\n", max_api_ver );
	sqlany_finalize_interface( &api );
	return -1;
    }

    /* A connection object needs to be created first */
    sqlany_conn1 = api.sqlany_new_connection();

    if( !api.sqlany_connect( sqlany_conn1, ConnectStr ) ) { 
	print_error( sqlany_conn1, "Failed connecting" );
	api.sqlany_free_connection( sqlany_conn1 );
	api.sqlany_fini();
	sqlany_finalize_interface( &api );
	exit( -1 );
    }
    printf( "Connected 1 successfully!\n" );

    /* A connection object needs to be created first */
    sqlany_conn2 = api.sqlany_new_connection();

    if( !api.sqlany_connect( sqlany_conn2, ConnectStr ) ) { 
	print_error( sqlany_conn2, "Failed connecting" );
	api.sqlany_free_connection( sqlany_conn2 );
	api.sqlany_fini();
	sqlany_finalize_interface( &api );
	exit( -1 );
    }
    printf( "Connected 2 successfully!\n\n" );
    
    // Sharing a callback (for demonstration purposes)
    registered = api.sqlany_register_callback( sqlany_conn1, CALLBACK_MESSAGE, (SQLANY_CALLBACK_PARM)messages );
    if( !registered ) printf( "Connection 1 callback registration failed!\n" );
    registered = api.sqlany_register_callback( sqlany_conn2, CALLBACK_MESSAGE, (SQLANY_CALLBACK_PARM)messages );
    if( !registered ) printf( "Connection 2 callback registration failed!\n" );
    
    // Not sharing a callback (for demonstration purposes)
    registered = api.sqlany_register_callback( sqlany_conn1, CALLBACK_VALIDATE_FILE_TRANSFER, (SQLANY_CALLBACK_PARM)client_download );
    if( !registered ) printf( "Connection 1 callback registration failed!\n" );
    registered = api.sqlany_register_callback( sqlany_conn2, CALLBACK_VALIDATE_FILE_TRANSFER, (SQLANY_CALLBACK_PARM)client_upload );
    if( !registered ) printf( "Connection 2 callback registration failed!\n" );
    
    registered = api.sqlany_register_callback( sqlany_conn2, (a_sqlany_callback_type)-1, (SQLANY_CALLBACK_PARM)client_upload );
    if( registered ) printf( "Connection 2 callback registration should have failed!\n" );
    
    rc = api.sqlany_execute_immediate( sqlany_conn1, "SET TEMPORARY OPTION allow_write_client_file='on'" );
    rc = api.sqlany_execute_immediate( sqlany_conn2, "SET TEMPORARY OPTION allow_read_client_file='on'" );
    
    // assume we are connected to the sample database
    rc = api.sqlany_execute_immediate( sqlany_conn2, "CREATE OR REPLACE PROCEDURE MESSAGEPROC() "
		    "BEGIN "
		    "  MESSAGE 'Testing callbacks with multiple connections' TYPE STATUS TO CLIENT;"
		    "END" );

    rc = api.sqlany_execute_immediate( sqlany_conn1, "CREATE OR REPLACE PROCEDURE UNLOADCUST() "
		    "BEGIN "
		    "  MESSAGE 'About to unload table' TYPE ACTION TO CLIENT; "
		    "  UNLOAD TABLE Customers INTO CLIENT FILE '" CLIENT_FILE "'; "
		    "  MESSAGE 'Finished unload table' TYPE INFO TO CLIENT; "
		    "END" );

    rc = api.sqlany_execute_immediate( sqlany_conn2, "CALL MESSAGEPROC" );
    
    rc = api.sqlany_execute_immediate( sqlany_conn1, "CALL UNLOADCUST" );
    if( rc == 0 )
    {
	rc = api.sqlany_error( sqlany_conn1, buffer, sizeof(buffer));
	printf( "Failed to execute! [%d]\n%s\n", rc, buffer );
    }

    rc = api.sqlany_execute_immediate( sqlany_conn2, "CREATE TABLE IF NOT EXISTS NewCustomers"
	"("
	"        ID                    integer NOT NULL default autoincrement,"
	"        Surname               person_name_t NOT NULL,"
	"        GivenName             person_name_t NOT NULL,"
	"        Street                street_t NOT NULL,"
	"        City                  city_t NOT NULL,"
	"        State                 state_t NULL,"
	"        Country               country_t NULL,"
	"        PostalCode            postal_code_t NULL,"
	"        Phone                 phone_number_t NOT NULL,"
	"        CompanyName           company_name_t NULL,"
	"        CONSTRAINT CustomersKey PRIMARY KEY (ID)"
	")" );
    rc = api.sqlany_execute_immediate( sqlany_conn2, "TRUNCATE TABLE NewCustomers" );
    rc = api.sqlany_execute_immediate( sqlany_conn2, "CREATE OR REPLACE PROCEDURE LOADCUST() "
		    "BEGIN "
		    "  MESSAGE 'About to load table' TYPE ACTION TO CLIENT; "
		    "  LOAD TABLE NewCustomers USING CLIENT FILE '" CLIENT_FILE "'; "
		    "  MESSAGE 'Finished load table' TYPE INFO TO CLIENT; "
		    "END" );

    rc = api.sqlany_execute_immediate( sqlany_conn2, "CALL LOADCUST" );
    if( rc == 0 )
    {
	rc = api.sqlany_error( sqlany_conn2, buffer, sizeof(buffer));
	printf( "Failed to execute! [%d]\n%s\n", rc, buffer );
    }

    api.sqlany_disconnect( sqlany_conn1 );
    api.sqlany_disconnect( sqlany_conn2 );

    /* Must free the connection object or there will be a memory leak */
    api.sqlany_free_connection( sqlany_conn1 );
    api.sqlany_free_connection( sqlany_conn2 );

    api.sqlany_fini();

    sqlany_finalize_interface( &api );
 
    return 0;
}
