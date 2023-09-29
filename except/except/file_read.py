#!/bin/env python

import sys



fname = sys.argv[1]

#
#     process_file
#
def process_file(name):
    f = open(fname, 'rb')
    for l in f.readlines():
        print l
    
#
#     main
#
try:
    process_file(fname)
except IOError:
    print "Could not read file:", fname
    sys.exit()

