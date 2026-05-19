#!/usr/local/bin/bash

CC=/usr/local/opt/gcc-16.1.0/bin/gcc
CFLAGS="-Wall -Wextra -g -O"
INC1=/usr/home/kostik/work/DEV/src

${CC} ${CFLAGS} -I${INC1} -o ntsync ntsync.c -lpthread
