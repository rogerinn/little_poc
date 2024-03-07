FROM ubuntu:20.04

RUN apt-get update && apt-get install -y valgrind

COPY bin/program .

CMD valgrind ./bin/program