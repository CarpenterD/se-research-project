from jinja2 import Environment, FileSystemLoader
import os
import glob
import argparse

import options

# constants -------------------------------------
SRC_DIR = "../../src"
OPTIONS_DIR = f"{SRC_DIR}/markers/options/"
TEST_CONFIG_DIR = f"{SRC_DIR}/utils/configuration/"
FILE_SUFFIX = ".generated"

# helper functions ------------------------------
def RemoveGeneratedFiles(path):
    searchPath = os.path.join(path, "*"+FILE_SUFFIX+".*")
    files = glob.glob(searchPath, recursive=True)
    for file in files:
        os.remove(file)

# main code -------------------------------------
print("Initialising templates")
env = Environment( loader=FileSystemLoader('templates') )
optionsTemplate = env.get_template('OptionsTemplate.hpp')
testConfigTemplate = env.get_template('TestConfigTemplate.hpp')

# get options from file
inputFile = "markerOptions.json"
print(f"Loading options from '{inputFile}'")
stdOptions, optionSets = options.LoadOptionsFromJson(inputFile)

stdOptionSet = options.OptionSet("StdOptions")
stdOptionSet.options = stdOptions
optionSets.append(stdOptionSet)

# remove existing files
# in order to delete files for removed option sets etc
print("Removing existing generated files")
RemoveGeneratedFiles(OPTIONS_DIR)
# we do not want to delete TestConfig since it is always required

# generate option files
print(f"Generating option sets")
for optSet in optionSets:
    outputPath = OPTIONS_DIR + optSet.name + FILE_SUFFIX + ".hpp"
    print(f"\t{outputPath}")
    with open(outputPath, mode='w') as outfile:
        templateStream = optionsTemplate.stream(set = optSet)
        templateStream.dump(outfile)

# generate test config struct
print(f"Generating 'TestConfig.hpp'")
outputPath = TEST_CONFIG_DIR + "TestConfig.hpp" #no '.generated' to prevent against deletion
with open(outputPath, mode='w') as outfile:
        templateStream = testConfigTemplate.stream(options = stdOptions)
        templateStream.dump(outfile)
print("Done.")