#!/bin/bash
SERVER_NAME="${DB_NAME}_17_${RANDOM}${RANDOM}"
dbsrv17 -n "${SERVER_NAME}" "${DB_FILE}" -n "${DB_NAME}"
