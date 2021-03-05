import json
import string

# Class Definitions -----------------------------
class OptionSet:
    def __init__(self, name):
        self.name = name
        self.description = None
        self.options = []

    def GetClassName(self):
        return CapitaliseFirstLetter(self.name)

    def GetDefineName(self):
        return "OPTSET_" + self.name.upper()

    def GetClassDocumentation(self):
        docString = "Automatically generated config/options file."
        if self.description != None:
            docString = self.description + "\n\n" + docString
        return "/**\n" + docString + "\n*/"

    def __repr__(self):
        outString = f"OptionSet '{self.name}':\n"
        for option in self.options:
            outString += "\t" + repr(option) + "\n"
        return outString

class Option:
    def __init__(self, name, datatype):
        self.name = name
        self.datatype = datatype
        self.description = None
        self.defaultValue = None
        self.optional = False

    def GetPropertyName(self):
        return LowerFirstLetter(self.name)

    def GetDefaultPropertyName(self):
        return "default" + CapitaliseFirstLetter(self.name)

    def GetPropertyType(self):
        typeString = self.datatype
        if self.datatype == "string":
            typeString = "std::string"
        return typeString
    
    def GetDefaultLiteral(self):
        if self.datatype == "string":
            return "\"" + self.defaultValue + "\""
        else:
            return self.defaultValue

    def GetDocString(self):
        return self.description

    def __repr__(self):
        return ("[optional] " if self.optional else "") + self.datatype + " " + self.name + ("" if self.description == None else (" - \"" + self.description + "\""))

# Helper Functions  -----------------------------
def CapitaliseFirstLetter(string):
    if string==None or len(string) < 1:
        return string
    elif len(string) < 2:
        return string.upper()
    else:
        return string[0].upper() + string[1:]

def LowerFirstLetter(string):
    if string==None or len(string) < 1:
        return string
    elif len(string) < 2:
        return string.lower()
    else:
        return string[0].lower() + string[1:]

_identifierCharacters = list(string.ascii_letters) + list(string.digits)
def IsSafeCxxIdentifier(name):
    """
        Check if the name meets conditions to be a C++ identifier. Does not check for reserved words.
    """
    if name == None or len(name) == 0:
        return False
    # check valid characters
    for letter in name:
        if not letter in _identifierCharacters:
            return False
    # check first character is not a digit
    if name[0] in list(string.digits):
        return False
    return True

def IsConvertableCxxType(datatype):
    if datatype == None:
        return False
    return True

def _isConvertableToType(value, datatype):
    return True;

def _parseOption(optDict):
    """
        Parses a dictionary into an instance of an 'Option'
    """
    if not IsSafeCxxIdentifier(optDict.get("name")):
        raise Exception("Invalid option - 'name' is not present or is not an acceptable c++ identifier")

    if not IsConvertableCxxType(optDict.get("type")):
        raise Exception("Invalid option - 'type' is not present, not recognised or not supported")

    # initialise with basic properties
    opt = Option(optDict["name"], optDict["type"])
    opt.description = optDict.get("description")
    
    # determine if optional and check validity of default value
    opt.optional = optDict.get("defaultValue") != None 
    if opt.optional:
        if _isConvertableToType(optDict["defaultValue"], opt.datatype):
            opt.defaultValue = optDict["defaultValue"]
        else:
            raise Exception("Invalid option - default value cannot be converted to the options type")
    return opt

# Driver Function -------------------------------
def LoadOptionsFromJson(file):
    """
        Loads option information from a JSON file
    """
    stdOptions, optionSets = [], []

    with open(file) as jsonFile:
        allOptions = json.load(jsonFile)

        if allOptions["stdOptions"] != None:
            stdOptions = [_parseOption(opt) for opt in allOptions["stdOptions"]]

        if allOptions["optionSets"] != None:
            for obj in allOptions["optionSets"]:
                optSet = OptionSet( obj["name"] if obj.get("name") != None and IsSafeCxxIdentifier(obj["name"]) else "OptionSet"+str(len(optionSets)) )
                optSet.description = obj.get("description")
                optSet.options = [_parseOption(opt) for opt in obj["options"]]
                optionSets.append(optSet)

    # TODO remove duplicate options and option sets (by name)
    return stdOptions, optionSets
