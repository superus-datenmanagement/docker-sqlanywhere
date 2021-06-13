FROM ubuntu:16.04

RUN mkdir /db

COPY ./entrypoint.sh /entrypoint.sh
COPY ./sqlanywhere16 /opt/sqlanywhere16

ENV LD_LIBRARY_PATH="/opt/sqlanywhere16/lib64"

ENV PATH="/opt/sqlanywhere16/bin64:${PATH}"

ENV DB_FILE="/opt/sqlanywhere16/demo.db"
ENV DB_NAME="database"

CMD ["/bin/bash", "/entrypoint.sh"]
