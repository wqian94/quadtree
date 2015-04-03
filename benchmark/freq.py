#!/usr/bin/python2.7

import sys

def freqplot(fname):
    data = {}
    with open(fname, "r") as fin:
        for line in fin:
            val = float(line)
            if val not in data:
                data[val] = 0
            data[val] += 1
            if len(data) % 1e6 == 0:
                print "Read %d lines" % len(data)
    print data

if __name__ == "__main__":
    for fname in sys.argv[1:]:
        freqplot(fname)
