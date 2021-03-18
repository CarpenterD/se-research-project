from jinja2 import Environment, FileSystemLoader
import os
import glob
import argparse

import options

# constants -------------------------------------
SRC_DIR = "../../src"
TEMPLATE_ENV = "templates"
OPTIONS_DIR = f"{SRC_DIR}/markers/options"
UTILS_DIR = f"{SRC_DIR}/utils"
TEST_CONFIG_DIR = f"{UTILS_DIR}/configuration"
FILE_SUFFIX = ".generated"

# parse arguments -------------------------------
parser = argparse.ArgumentParser(description="Generate source files for 'stylemarker'")
parser.add_argument('opts', help="JSON file containing configuration options for stylemarker")
parser.add_argument('-o', '--output', default=SRC_DIR, help="Path to the stylemarker 'src' directory.")
parser.add_argument('-t', '--templateLocation', default=TEMPLATE_ENV, help="The directory containing template files.")
args = parser.parse_args()
OPT_FILE = args.opts
SRC_DIR = args.output
TEMPLATE_ENV = args.templateLocation

# helper functions ------------------------------
def RemoveGeneratedFiles(path):
    searchPath = os.path.join(path, "*"+FILE_SUFFIX+".*")
    files = glob.glob(searchPath, recursive=True)
    for file in files:
        os.remove(file)

# main code -------------------------------------
print(f"Initialising template environment '{TEMPLATE_ENV}'")
try:
    env = Environment( loader=FileSystemLoader(TEMPLATE_ENV) )
    optionHdrTemplate = env.get_template('OptionsTemplate.hpp')
    optionSrcTemplate = env.get_template('OptionsTemplate.cpp')
    testConfigTemplate = env.get_template('TestConfigTemplate.hpp')
    configReaderTemplate = env.get_template('ConfigReaderTemplate.cpp')
except:
    print("error: unable to initialise template environment.")
    parser.print_usage()
    exit()

# get options from file
print(f"Loading options from '{OPT_FILE}'")
try:
    stdOptions, optionSets = options.LoadOptionsFromJson(OPT_FILE)
except:
    print("error: unable to open options file")
    parser.print_usage()
    exit()

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