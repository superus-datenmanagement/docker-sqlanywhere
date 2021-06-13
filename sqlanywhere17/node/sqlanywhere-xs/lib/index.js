// ***************************************************************************
// Copyright (c) 2019 SAP SE or an SAP affiliate company. All rights reserved.
// ***************************************************************************
var db = null;
var driver_file = "sqlanywhere_xs"

var v = process.version;
var match = v.match( 'v([0-9]+)\.([0-9]+)\.[0-9]+' );
driver_file += '_v' + match[1];
if( match[1]+0 == 0 ) {
    driver_file += '_' + match[2];
}

try {
    if( process.platform == "win32" && process.arch == "x64" ) {
	db = require( "./../bin64/" + driver_file );
    
    } else if( process.platform == "win32" && process.arch == "ia32" ) {
	db = require( "./../bin32/" + driver_file );
    
    } else if( process.platform == "linux" && process.arch == "x64" ) {
	db = require( "./../bin64/" + driver_file ); 
    
    } else if( process.platform == "linux" && process.arch == "ia32" ) {
	db = require( "./../bin32/" + driver_file ); 
    
    } else {
	throw new Error( "Platform Not Supported" );
    }
} catch( err ) {
    try {
	// Try natively compiled binaries
	db = require( "./../build/Release/sqlanywhere_xs.node" ); 
    } catch( err ) {
	throw new Error( "Could not load modules for Platform: '" + process.platform + "', Process Arch: '" + process.arch
			+ "', and Version: '" + process.version +"'" ); 
    }
}
module.exports = db;
