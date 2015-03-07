#!/usr/bin/python2.7

from operator import itemgetter

import os, sys

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print "Please entire at least one file to analyze"

    data = {}
    for fname in sys.argv[1:]:
        print fname
        with open(fname, "r") as fin:
            for line in fin:
                td = line.split(", ")
                for i in xrange(0, len(td) - 3, 4):
                    if int(td[i]) not in data:
                        data[int(td[i])] = {"i": [], "q": [], "r": [], "f": []}
                    data[int(td[i])]["i"] += [float(td[i + 1])]
                    data[int(td[i])]["q"] += [float(td[i + 2])]
                    data[int(td[i])]["r"] += [float(td[i + 3])]
                    data[int(td[i])]["f"] += [fname]

    sortedSizes = sorted(data.keys())

    print "%18s  |  %16s  |  %16s  |  %16s  |  %s" % ("number of points", "insert time (s)", "query time (s)", "delete time (s)", "result file")
    print "=" * (18 + 5 + 16 + 5 + 16 + 5 + 16 + 5 + 30)
    for size in sortedSizes:
        print "%18d  |  %16s  |  %16s  |  %16s  |  %s" % (size, "%.8f" % data[size]["i"][0], "%.8f" % data[size]["q"][0], "%.8f" % data[size]["r"][0], data[size]["f"][0])
        if len(data[size]["i"]) > 1:
            for i in xrange(1, len(data[size]["i"])):
                print "%18s  |  %16s  |  %16s  |  %16s  |  %16s" % ("", "%.8f" % data[size]["i"][i], "%.8f" % data[size]["q"][i], "%.8f" % data[size]["r"][i], data[size]["f"][i])
