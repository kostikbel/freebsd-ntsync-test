#!/usr/bin/bash

CC=cc
CFLAGS="-Wall -Wextra -g -O"

${CC} ${CFLAGS} -o ntsync ntsync.c -lpthread
