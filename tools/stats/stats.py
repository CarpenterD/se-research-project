import os
import json

JSON_DIR = "output"

_, _, filenames = next(os.walk(JSON_DIR))

errors = {}

for filename in filenames:
    name = os.path.splitext(filename)[0]

    with open(JSON_DIR + "/" + filename) as f:
        try:
            data = json.load(f)
        except:
            continue

    results = data["results"]

    if results != None:
        for result in results:
            if not result["marker"] in errors:
                errors[ result["marker"] ] = { "fileCount": 0, "total": 0 }
            if len(result["feedback"]) > 0:
                errors[ result["marker"] ]["fileCount"] += 1
                errors[ result["marker"] ]["total"] += len(result["feedback"])
            
print(errors)