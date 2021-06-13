
function mainloop()
/******************/
{
    var jsextenv_addon = require( 'sqlanywhere_jsextenv' );
    var call_back = function call_back_fcn( method_sig ) {
        try{
	    method_sig = method_sig.replace( /\s+/g, '' );
	    method_sig = method_sig.replace( '|', '><' );
	    method_sig = method_sig.substring( method_sig.indexOf('<') );
	    var conn_ptr = jsextenv_addon.get_conn();
	    if( conn_ptr == "Can't Get Ptr" ) {
		jsextenv_addon.set_error( -7 );
		return;
	    }
	    // Parse Method Signature
            var has_return = false, file_name, args, argc, func_name, func_string, params = '';
	    var match = method_sig.match( '<args=(.*)><(.)><(file|filename)=(.*)>(.*)' );

	    if( !match || !match[2] || !match[4] || !match[5] ) {
		match = method_sig.match( '(<)(.)><(file|filename)=(.*)>(.*)' );
		if( match ) {
		    match[1] = "";
		}
	    }

	    if ( !match || !match[2] || !match[4] || !match[5] ) {
		match = method_sig.match( '<args=(.*)><(file|filename)=(.*)>(.*)' );

		if( !match || !match[3] || !match[4] ) {
		    match = method_sig.match( '(<)(file|filename)=(.*)>(.*)' );
		    if( match ) {
		        match[1] = "";
		    }
		}

		if ( !match || !match[3] || !match[4] ) {
	            jsextenv_addon.set_error( -1 );
		    return;
		}
		args = match[1];
		argc = args.length;
		file_name = match[3];
		func_string = match[4];
		has_return = false;
	    } else {
		args = match[1];
		argc = args.length;
		args = args + match[2];
		file_name = match[4];
		func_string = match[5];
		has_return = true;
	    }

	    // Get method Parameters
	    var start, end, param_count;
	    start = func_string.indexOf( "(" );
	    end = func_string.indexOf( ")" );
	    if( start == -1 || end == -1 ) {
		jsextenv_addon.set_error( -8 );
		return;
	    }
	    func_name = func_string.substring( 0, start );
	    params = func_string.substring( start + 1, end );
	    param_count = params.replace(/\[/g,'');

	    if( param_count.length != argc ) {
		jsextenv_addon.set_error( -7 );
		return;
	    }

	    // Getting Code and Function call
	    var func_call = jsextenv_addon.get_func_call( args, argc, has_return, func_name, params );
	    if ( !( isNaN( func_call ) ) ) {
		jsextenv_addon.set_error( func_call );
		return;
	    }
	    var code = jsextenv_addon.get_code( file_name );
	    if ( code == -5 ) {
		jsextenv_addon.set_error( -5 );
		return;
	    }

	    // Executing
	    var handle = jsextenv_addon.get_conn();
	    if ( has_return ) {
		var ret;
		try {
		    ret = execute( func_call, code, handle);
		} catch( err ) {
		    jsextenv_addon.set_error( err );
		    return;
		}
		if( ret[argc] !== undefined ) {
		    var is_error = jsextenv_addon.set_output( ret, argc, params, has_return );
		    if ( !( isNaN( is_error ) ) ) {
			jsextenv_addon.set_error( is_error );
			return;
		    }
		} else {
		// return is Undefined
		jsextenv_addon.set_error( -3 );
		return;
		}
	    } else {
		// Executing ( No Return Value )
		var ret;
		try {
		    ret = execute( func_call, code, handle );
		} catch( err ) {
		    // Error with user code
		    jsextenv_addon.set_error( err );
		    return;
		}
		var is_error = jsextenv_addon.set_output( ret, argc, params, has_return );
		if ( !( isNaN( is_error ) ) ) {
		    jsextenv_addon.set_error( is_error );
		    return;
		}
	    }
	    return;
	} catch( err ) {
	    jsextenv_addon.set_error( err );
	    return;
	}
    };

    jsextenv_addon.start( process.argv[2], process.argv[3], process.argv[4], process.argv[5], call_back );
}

function execute( sa_ee_func_call, sa_ee_code, sa_dbcapi_handle )
/*************************************************************/
{

    eval( sa_ee_code );

    function exec( func_call ) {
        var args = new Array();
	sa_ee_func_call = null;
        eval( func_call );
	return args;
    }

    sa_ee_code = null;
    return exec( sa_ee_func_call );


}

if( process.argv[2] === undefined || process.argv[3] === undefined || process.argv[4] === undefined || process.argv[5] === undefined ) {
    console.log( "Invalid Arguments passed to Node.js Executable!" );
    process.exit(1);
}
mainloop();
