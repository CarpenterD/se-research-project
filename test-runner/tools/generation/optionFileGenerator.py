from jinja2 import Environment, FileSystemLoader
import os
import glob
import argparse

import options

# constants -------------------------------------
OUTPUT_DIR = "../../src/markers/options/"
FILE_SUFFIX = ".generated"

# helper functions ------------------------------
def RemoveGeneratedFiles(path):
    searchPath = os.path.join(path, "*"+FILE_SUFFIX+".*")
    files = glob.glob(searchPath, recursive=True)
    for file in files:
        os.remove(file)

# main code -------------------------------------
env = Environment( loader=FileSystemLoader('templates') )
template = env.get_template('OptionsTemplate.hpp')

# get options from file
stdOptions, optionSets = options.LoadOptionsFromJson("markerOptions.json")

stdOptionSet = options.OptionSet("StdOptions")
stdOptionSet.options = stdOptions
optionSets.append(stdOptionSet)

# generate test/marker option files
for optSet in optionSets:
    with open(OUTPUT_DIR + optSet.name + FILE_SUFFIX + ".hpp", mode='w') as outfile:
        templateStream = template.stream(set = optSet)
        templateStream.dump(outfile)