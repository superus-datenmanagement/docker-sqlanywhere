// ***************************************************************************
// Copyright (c) 2019 SAP SE or an SAP affiliate company. All rights reserved.
// ***************************************************************************

//***************************************************************************
//  Version number
//***************************************************************************

// The version number will be increased for each maintenance release.
// This allows room to add new features to EBFs.
//
// Note: only version numbers for the current major release need to be
// defined here.

#define DB_TOOLS_VERSION_17_0_0    17000
#define DB_TOOLS_VERSION_17_0_4    17004
#define DB_TOOLS_VERSION_17_0_8    17008
#define DB_TOOLS_VERSION_NUMBER	   DB_TOOLS_VERSION_17_0_8

// changes in this version requiring a version number change:
// - added inmem_mode and live_type to a_validate_db
// - added min_pwd_len in a_create_db
// - added starting_offset, current_relative_offset and end_offset 
//   to a_change_log
// - added online_rebuild, online_rebuild_from_backup, etc to an_unload_db

#define DB_TOOLS_VERSION_IQSP09	   16001
#define DB_TOOLS_VERSION_16_0_0    16000

#define DB_TOOLS_VERSION_12_0_0    12000

#define DB_TOOLS_VERSION_11_0_0    11000
// Changes in this version requiring a version number change:
// - added encrypted_stream_opts field to a_sync_db


#define DB_TOOLS_VERSION_MIN_SUPPORTED	DB_TOOLS_VERSION_16_0_0

#define DB_TOOLS_VERSION_10_0_0	   10000

