<?php
    #*********************************************************************
    # Copyright 2019 SAP SE or an SAP affiliate company. All rights reserved.
    #
    # This sample code is provided AS IS, without warranty or liability
    # of any kind.
    # 
    # You may use, reproduce, modify and distribute this sample code
    # without limitation, on the condition that you retain the foregoing
    # copyright notice and disclaimer as to the original code.
    # 
    #*********************************************************************
    #
    # This sample program contains a hard-coded userid and password
    # to connect to the demo database. This is done to simplify the
    # sample program. The use of hard-coded passwords is strongly
    # discouraged in production code.  A best practice for production
    # code would be to prompt the user for the userid and password.
    #

    $conn = sasql_connect( "UID=DBA;PWD=sql" ) or die("Can't connect to db");
 
    // Retriving image from database ...
    $ret_blob = "";
    $query = "SELECT image FROM my_images WHERE id = 5";
    $result = sasql_query( $conn, $query  );
    if( ! $result ) {
        echo "FAILED executing query<br>";
        return 0;
    } else {
        $row = sasql_fetch_row( $result );
        if( ! $row ) {
	    echo "FAILED fetching data<br>";
	    return 0;
	} else {
	    $ret_blob = $row[0];
	}
    }
    sasql_free_result( $result );

    header("Content-type: image/gif");
    echo $ret_blob;

    sasql_close( $conn );
?>
