// ***************************************************************************
// Copyright (c) 2019 SAP SE or an SAP affiliate company. All rights reserved.
// ***************************************************************************

#ifndef CONSTS_H
#define CONSTS_H

#ifdef WINNT
#define EXTENVCALL __stdcall
#else
#define EXTENVCALL
#endif

typedef enum DataAccessApi {
    DBLIB,
    ADONET,
    ODBC,
    PHP,
    JAVASCRIPT,
    PERL
} DataAccessApi;

typedef enum ExtEnvApiVersion {
    EXTENV_API_VERSION_1 = 1
} ExtEnvApiVersion;

typedef int EXTENVRET;

#define EXTENV_OK 0
#define EXTENV_ERROR -1

#define INVALID_RS 0

#endif
