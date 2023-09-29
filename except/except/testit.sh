#!/bin/bash

set -e
trap 'echo "Testit caught signal $? from failing executable"; exit 0' ERR
echo "******************************************"
echo "           Testing: $1"
echo "******************************************"
echo
./$1

