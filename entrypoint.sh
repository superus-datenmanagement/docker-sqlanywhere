#!/bin/bash
SERVER_NAME="${DB_NAME}_16_${RANDOM}${RANDOM}"
dbsrv16 -n "${SERVER_NAME}" "${DB_FILE}" -n "${DB_NAME}"
