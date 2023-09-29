#! /bin/sh
valgrind ./restoration /comp/40/bin/images/corruption/$1-corrupt.pgm > $1.pgm 2> $1.err