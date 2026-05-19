#!/usr/bin/bash

CC=cc
CFLAGS="-Wall -Wextra -g -O"

${CC} ${CFLAGS} -static -o ntsync ntsync.c -lpthread
