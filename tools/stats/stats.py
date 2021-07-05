import os
import json
import statistics

import numpy as np

JSON_DIR = "output"

_, _, filenames = next(os.walk(JSON_DIR))

def myhist(data, bins):
    hist, bin_edges = np.histogram(data, bins)
    print(hist)
    print(bin_edges)
    print("ymax = ", max(hist))
    data = ""
    for i in range(len(hist)):
        data += f"({bin_edges[i]}, {hist[i]}) "
    print(f"{data} ({bin_edges[-1]}, 0)")

errors = {}

for filename in filenames:
    name = os.path.splitext(filename)[0]

    with open(JSON_DIR + "/" + filename) as f:
        try:
            data = json.load(f)
        except:
            print( "ERR: Unable to read ", filename)
            continue

    results = data["results"]

    if results != None:
        for result in results:
            if not result["marker"] in errors:
                errors[ result["marker"] ] = { "fileCount": 0, "total": 0, "perFile": []}
            if len(result["feedback"]) > 0:
                errors[ result["marker"] ]["fileCount"] += 1
                errors[ result["marker"] ]["total"] += len(result["feedback"])
                errors[ result["marker"] ]["perFile"].append(len(result["feedback"]))
            

for errorName in errors:
    # print (errors[errorName])

    errors[errorName]["perFile"] += [0] * (60-len( errors[errorName]["perFile"] ))
    mean, stddev = statistics.mean( errors[errorName]["perFile"] ), statistics.stdev( errors[errorName]["perFile"] )

    print(errorName, f"Total Count: {errors[errorName]['total']}, Number of files: {errors[errorName]['fileCount']}, Mean: {mean} (s.d.={stddev})")
    print("\t", errors[errorName]["perFile"])

    if errorName == "FunctionLengthMarker":
        myhist(errors[errorName]["perFile"], [0, 1, 2, 3, 4, 5])
    elif errorName == "IndentationCheck":
        myhist(errors[errorName]["perFile"], [0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100])
    elif errorName == "IdentifierMarker":
        myhist(errors[errorName]["perFile"], [0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30])
    
