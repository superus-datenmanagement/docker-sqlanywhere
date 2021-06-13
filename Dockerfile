FROM ubuntu:16.04

RUN mkdir /db

COPY ./entrypoint.sh /entrypoint.sh
COPY ./sqlanywhere17 /opt/sqlanywhere17

ENV LD_LIBRARY_PATH="/opt/sqlanywhere17/lib64"

ENV PATH="/opt/sqlanywhere17/bin64:${PATH}"

ENV NODE_PATH="/opt/sqlanywhere17/node:${PATH}"

ENV SQLANY17="/opt/sqlanywhere17"

ENV DB_FILE="/opt/sqlanywhere17/demo.db"
ENV DB_NAME="database"

CMD ["/bin/bash", "/entrypoint.sh"]
