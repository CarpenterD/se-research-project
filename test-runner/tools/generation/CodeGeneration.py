from jinja2 import Environment, FileSystemLoader
import os
import glob
import argparse

import options

# constants -------------------------------------
SRC_DIR = "../../src"
OPTIONS_DIR = f"{SRC_DIR}/markers/options"
UTILS_DIR = f"{SRC_DIR}/utils"
TEST_CONFIG_DIR = f"{UTILS_DIR}/configuration"
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
optionHdrTemplate = env.get_template('OptionsTemplate.hpp')
optionSrcTemplate = env.get_template('OptionsTemplate.cpp')
testConfigTemplate = env.get_template('TestConfigTemplate.hpp')
configReaderTemplate = env.get_template('ConfigReaderTemplate.cpp')

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
    # header
    headerName = optSet.name + FILE_SUFFIX + ".hpp"
    outputPath = f"{OPTIONS_DIR}/{headerName}"
    print(f"\t{outputPath}")
    with open(outputPath, mode='w') as outfile:
        templateStream = optionHdrTemplate.stream(set = optSet)
        templateStream.dump(outfile)

    # src
    outputPath = f"{OPTIONS_DIR}/{optSet.name}{FILE_SUFFIX}.cpp"
    print(f"\t{outputPath}")
    with open(outputPath, mode='w') as outfile:
        templateStream = optionSrcTemplate.stream(set = optSet, headerName = headerName)
        templateStream.dump(outfile)

# generate test config struct
print(f"Generating 'TestConfig.hpp'")
outputPath = f"{TEST_CONFIG_DIR}/TestConfig.hpp" #no '.generated' to prevent against deletion
with open(outputPath, mode='w') as outfile:
        templateStream = testConfigTemplate.stream(options = stdOptions)
        templateStream.dump(outfile)

# generate config reader file to parse test std options into one 
print(f"Generating 'ConfigReader" + FILE_SUFFIX + ".cpp'")
outputPath = f"{UTILS_DIR}/ConfigReader{FILE_SUFFIX}.cpp"
with open(outputPath, mode='w') as outfile:
        templateStream = configReaderTemplate.stream(options = stdOptions)
        templateStream.dump(outfile)
print("Done.")