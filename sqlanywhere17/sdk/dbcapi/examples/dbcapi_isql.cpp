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
#include <ctype.h>
#include <assert.h>
#include <signal.h>

#if defined( USE_READLINE )
#include <readline/readline.h>
#include <readline/history.h>
#endif

#if defined( UNIX ) 
#define strnicmp   strncasecmp
#define get_time() 0
#define my_snprintf snprintf
#else
#include <windows.h>
#define get_time() GetTickCount()
#define my_snprintf _snprintf
#endif

#include "sacapi.h"
#include "sacapidll.h"

SQLAnywhereInterface api;
a_sqlany_connection *sqlany_conn = NULL;
int max_history = 0;


void print_blob( char * buffer, size_t length )
/**********************************************/
{
    size_t i;

    if( length == 0 ) {
	return;
    }
    printf( "0x" );
    i = 0;
    while( i < length ) {
	printf( "%.2X", (unsigned char)buffer[i] );
	i++;
    }
}

void execute( const char * query )
/***************************/
{
    a_sqlany_stmt * sqlany_stmt;
    int		    err_code;
    char	    err_mesg[SACAPI_ERROR_SIZE];
    int		    i;
    int		    num_rows;
    int		    length;

    sqlany_stmt = api.sqlany_execute_direct( sqlany_conn, query );
    if( sqlany_stmt == NULL ) {
	err_code = api.sqlany_error( sqlany_conn, err_mesg, sizeof(err_mesg) );
	printf( "Failed: [%d] '%s'\n", err_code, err_mesg );
	return;
    }
    if( api.sqlany_error( sqlany_conn, NULL, 0 ) > 0 ) {
	err_code = api.sqlany_error( sqlany_conn, err_mesg, sizeof(err_mesg) );
	printf( "Warning: [%d] '%s'\n", err_code, err_mesg );
    }
    if( api.sqlany_num_cols( sqlany_stmt ) == 0 ) {
	//printf( "Executed successfully.\n" );
	if( api.sqlany_affected_rows( sqlany_stmt ) > 0 ) {
	    printf( "%d affected rows.\n", api.sqlany_affected_rows( sqlany_stmt ) );
	}
	api.sqlany_free_stmt( sqlany_stmt );
	return;
    }

    for( ;; ) {
	printf( "Estimated number of rows: %d\n", api.sqlany_num_rows( sqlany_stmt ) );
	// first output column header
	length = 0;
	for( i = 0; i < api.sqlany_num_cols( sqlany_stmt ); i++ ) {
	    a_sqlany_column_info	column_info;

	    if( i > 0 ) {
		printf( "," );
		length += 1;
	    }
	    api.sqlany_get_column_info( sqlany_stmt, i, &column_info );
	    printf( "%s", column_info.name );
	    length += (int)strlen( column_info.name );
	}
	printf( "\n" );
	for( i = 0; i < length; i++ ) {
	    printf( "-" );
	}
	printf( "\n" );
	num_rows = 0;
	while( api.sqlany_fetch_next( sqlany_stmt ) ) {
	    num_rows++;
	    for( i = 0; i < api.sqlany_num_cols( sqlany_stmt ); i++ ) {
		a_sqlany_data_value dvalue;
		sacapi_bool	    ok;

		ok = api.sqlany_get_column( sqlany_stmt, i, &dvalue );
		err_code = api.sqlany_error( sqlany_conn, err_mesg, sizeof(err_mesg) );
		
		if( !ok ) { 
		    printf( "Error: [%d] '%s'\n", err_code, err_mesg );
		}
		if( err_code ) {
		    printf( "Warning: [%d] '%s'\n", err_code, err_mesg );
		}

		if( i > 0 ) {
		    printf( "," );
		}
		if( *(dvalue.is_null) ) {
		    printf( "(NULL)" );
		    continue;
		}
		switch( dvalue.type ) {
		    case A_BINARY:
			print_blob( dvalue.buffer, *(dvalue.length) );
			break;
		    case A_STRING:
			printf( "'%.*s'", (int)*(dvalue.length), (char *)dvalue.buffer );
			break;
		    case A_VAL64:
			printf( "%lld", *(long long*)dvalue.buffer);
			break;
		    case A_UVAL64:
			printf( "%lld", *(unsigned long long*)dvalue.buffer);
			break;
		    case A_VAL32:
			printf( "%d", *(int*)dvalue.buffer );
			break;
		    case A_UVAL32:
			printf( "%u", *(unsigned int*)dvalue.buffer );
			break;
		    case A_VAL16:
			printf( "%d", *(short*)dvalue.buffer );
			break;
		    case A_UVAL16:
			printf( "%u", *(unsigned short*)dvalue.buffer );
			break;
		    case A_VAL8:
			printf( "%d", *(char*)dvalue.buffer );
			break;
		    case A_UVAL8:
			printf( "%d", *(unsigned char*)dvalue.buffer );
			break;
		    case A_DOUBLE:
			printf( "%f", *(double*)dvalue.buffer );
			break;
                    default: break;
		}
	    }
	    printf( "\n" ); 
	}
	for( i = 0; i < length; i++ ) {
	    printf( "-" );
	}
	printf( "\n" );

	printf( "%d rows returned\n", num_rows );
	if( api.sqlany_error( sqlany_conn, NULL, 0 ) != 100 ) {
	    char buffer[256];
	    int  code = api.sqlany_error( sqlany_conn, buffer, sizeof(buffer) );
	    printf( "Failed: [%d] '%s'\n", code, buffer );
	}
	printf( "\n" );

	sacapi_bool	    ok;
	ok = api.sqlany_get_next_result( sqlany_stmt );
	err_code = api.sqlany_error( sqlany_conn, err_mesg, sizeof(err_mesg));

	if( !ok ) {
	    printf( "%s: [%d] '%s'\n", (err_code > 0 ? "Warning" : "Error" ), err_code, err_mesg );
	    break;
	}
    }

    fflush( stdout );
    api.sqlany_free_stmt( sqlany_stmt );
}

void
execute_sql( const char * line )
/*******************************/
{
    unsigned int  start_time;
    start_time = get_time();
    execute( line );
    unsigned int elapsed = get_time() - start_time;
    printf( "Total elapsed time = %dms\n", elapsed );
}

size_t read_file( char * file_name, char ** buffer )
/***************************************************/
{
    FILE * file = fopen( file_name, "rb" );
    size_t bytes_read;
    size_t size;

    if( file == NULL ) {
	return 0;
    }
    fseek( file, 0, SEEK_END );
    size = ftell( file );
    fseek( file, 0, SEEK_SET );
    (*buffer) = (char *)malloc( size + 1 );
    bytes_read = fread( (*buffer), 1, size, file );
    (*buffer)[bytes_read] = '\0';
    fclose( file );
    return bytes_read;
}

void 
print_help()
/***********/
{
    printf( "Commands:\n" );
    printf( "  help                       Print this help screen\n" );
    printf( "  quit                       Exit\n" );
    printf( "  connect <connect_string>   Connect\n" );
#if defined( USE_READLINE )
    printf( "  !<range>[,<range]          Replay a range of history. Range is <start>[-<end]\n" );
    printf( "  history                    Show history\n" );
#endif
}

int print_history( const char * history );
int play_history( const char * history );

int 
execute_line( const char * buffer )
/**********************************/
{
    if( buffer[0] == '\0' ||
	(buffer[0] == '-' && buffer[1] == '-') ) {
    } else if( strcmp( buffer, "quit" ) == 0 ) {
	return 0;
    } else if( strcmp( buffer, "help" ) == 0 || strcmp( buffer, "?" ) == 0 ) {
	print_help();
#if defined( USE_READLINE )	
    } else if( buffer[0] == '!' ) {
	play_history( &buffer[1] );
    } else if( strnicmp( buffer, "history", 7 ) == 0 ) {
	print_history( &buffer[7] );
#endif
    } else if( strnicmp( buffer, "read ", 5 ) == 0 ) {
	char * file_name = strdup( &buffer[5] );
	char * sql;

	size_t bytes_read = read_file( file_name, &sql );
	if( bytes_read == 0 ) {
	    printf( "Could not read file %s\n", file_name );
	    free( file_name );
	} else {
	    execute_line( sql );
	    free( file_name );
	    free( sql );
	}
    } else if( strnicmp( buffer, "connect ", 8 ) == 0 ) {
	char error_buffer[256];
	if( !api.sqlany_connect( sqlany_conn, &buffer[8] ) ) {
	    int code = api.sqlany_error( sqlany_conn, error_buffer, sizeof(error_buffer) );
	    printf( "Failed to connect: [%d] %s\n", code, error_buffer );
	} else {
	    printf( "Connected successfully!\n" );
	}
    } else if( strnicmp( buffer, "disconnect ", 8 ) == 0 ) {
	api.sqlany_disconnect( sqlany_conn );
	printf( "Disconnected!\n" );
    } else {
	execute_sql( buffer );
    }
    return 1;
}

#if defined( WIN32 ) 
void __cdecl cancel_function( int )
#else
void cancel_function( int )
#endif
/*********************************/
{
    api.sqlany_cancel( sqlany_conn );
}


#if defined( USE_READLINE )
size_t get_line( const char * prompt, char * buffer, size_t size )
/*************************************************************/
{
    char * input = readline( prompt );
    size_t s;
    if( input == NULL ) {
	buffer[0] = '\0';
        s = (size_t)-1;
    } else if( input[0] == '\0' ) {
	s = 0;
	buffer[s] = '\0';
    } else {
	HIST_ENTRY * entry = history_get( max_history );
	if( entry == NULL || 
	    strcmp( entry->line, input ) != 0 ) {
	    add_history( input );
	    max_history++;
	} 
	size_t l = strlen( input );
	s = ( l < (size-1) ? l : (size-1) );
	memcpy( buffer, input, s );
	buffer[s] = '\0';
    }
    return s;
}

int execute_range( const char * range )
/***************************************/
{
    char * dup_range = strdup( range );
    char * begin = dup_range;
    char * end = NULL;
    size_t len = strlen( dup_range );
    for( int i = 0; i < len; i++ ) {
	if( dup_range[i] != '-' ) {
	    continue;
	}
	if( end != NULL ) {
	    // error in command
	   printf( "Error in command: %s\n", range );
	   free( dup_range );
	   return 0; 
	}
	dup_range[i] = '\0';
	end = &dup_range[i+1];
    }
    int begin_num = atoi( begin );
    int end_num = ( end == NULL ? 0 : atoi( end ) );
    int i = begin_num;
    do { 
	HIST_ENTRY * ent = history_get( i );
	if( ent != NULL ) {
	    printf( "Replaying => '%s'\n", ent->line );
	    execute_line( ent->line );
	}
	i++;
    } while( i <= end_num );
    free( dup_range );
    return 0;
}

int play_history( const char * history )
/***************************************/
{
    if( strcmp( history, "" ) == 0 ) {
	return 0;
    }
    char * dup_hist = strdup( history );
    size_t len = strlen( dup_hist );
    char * str = dup_hist;
    for( int i = 0; i < len; i++ ) {
	if( dup_hist[i] != ',' ) {
	    continue;
	}
	dup_hist[i] = '\0';
	execute_range( str );
	str = &dup_hist[i+1];
    }
    if( str[0] != '\0' ) {
	execute_range( str );
    }
    free( dup_hist );
    return 0;
}

int print_history( const char * spec )
/************************************/
{
    int first = 0;
    int last = max_history;
    // skip white space
    while( *spec == ' ' || *spec == '\t' ) {
	spec++;
    }
    if( *spec == '\0' ) {
	// print all history
    } else {
	int count = atoi( spec );
	if( count < max_history ) {
	    first = max_history - count;
	    last = max_history;
	}
    }

    HIST_ENTRY ** list = history_list();
    if( list == NULL ) {
	printf( " No history is available\n" ); 
	return 0;
    }
    for( int i = first; i < last; i++ ) {
	printf( "%4d  %s\n", i+1, list[i]->line );
    }
    printf( "\n" );
    return 0;
}
#else
size_t get_line( const char * prompt, char * buffer, size_t size )
{
    printf( "\n%s", prompt );
    fflush( stdout );

    size_t len = 0;
    int ch = 0;
    while( len < size ) {
	ch = fgetc( stdin );
	if( ch == '\0' || ch == '\n' || ch == '\r' || ch == -1 ) {
	    break;
	}
	buffer[len] = (char)ch;
	len++;
    }
    buffer[len] = '\0';
    max_history++;
    return ( ch == -1 && len == 0 ) ? (size_t)-1 : len;
}
#endif

int main( int argc, char * argv[] )
/**********************************/
{
    unsigned int  max_api_ver;
    char 	  buffer[256];

    if( argc > 1 && 
	(strcmp( argv[1], "-c" ) != 0 || argc < 3) ) {
	printf( "Usage: %s [-c <connection_string>]\n", argv[0] );
	exit( 0 );
    }
    if( !sqlany_initialize_interface( &api, NULL ) ) {
	printf( "Failed to initialize the interface!\n" );
	exit( 0 );
    }
    if( !api.sqlany_init( "isql", SQLANY_API_VERSION_2, &max_api_ver )) {
	printf( "Failed to initialize the interface! Supported version = %d\n", max_api_ver );
	sqlany_finalize_interface( &api );
	return -1;
    }
    sqlany_conn = api.sqlany_new_connection();

    signal( SIGINT, cancel_function );

    if( argc > 1 ) {
	if( !api.sqlany_connect( sqlany_conn, argv[2] ) ) {
	    int code = api.sqlany_error( sqlany_conn, buffer, sizeof(buffer) );
	    printf( "Could not connect: [%d] %s\n", code, buffer );
	} else {
	    printf( "Connected successfully!\n" );
	}
    }
    char prompt[256];
    while( true ) {
	my_snprintf( prompt, sizeof(prompt), "[%d] %s> ", max_history, argv[0] );
	if( get_line( prompt, buffer, sizeof(buffer) ) == (size_t)-1 ) {
	    break;
	}

	if( buffer[0] != '\0' ) {
	    if( execute_line( buffer) ) {
		printf( "\n" );
	    } else {
		break;
	    }
	}
    }

    signal( SIGINT, SIG_DFL );
    api.sqlany_disconnect( sqlany_conn );

    api.sqlany_free_connection( sqlany_conn );
    api.sqlany_fini();
    sqlany_finalize_interface( &api );
}

