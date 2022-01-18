
#ifndef GLOBAL_H
#define GLOBAL_H

#include <map>
#include <vector>

#include "common.h"


//
// Command line settings
//

extern std::string CMD_outFile;
extern std::string CMD_cnfFile;

//
// Options from the configuration file
//

extern std::map<std::string, bool> GLOBAL_ConfigOptions;

//
// Type definitions for input strings/keywords
//

typedef struct StringInputEntry
{
    bool        enabledSTD;     // whether enabled for standard build
    bool        enabledCRT;     // whether enabled for standard build with extra ROM cartridge
    bool        enabledM65;     // whether enabled for Mega 65 build
    bool        enabledU64;     // whether enabled for Ultimate 64 build
    bool        enabledX16;     // whether enabled for Commander X16 build
    std::string alias;          // alias, for the assembler
    std::string string;         // string/token
    uint8_t     abbrevLen = 0;  // length of token abbreviation
} StringEntry;

enum class ListType
{
    KEYWORDS,
    STRINGS_BASIC,
    DICTIONARY                  // for internal use only
};

typedef struct StringInputList
{
    ListType                      type;
    std::string                   name;
    std::vector<StringInputEntry> list;
} StringInputList;

//
// Type definitions for imported/encoded strings/keywords
//

typedef std::vector<uint8_t>       StringEncoded;
typedef std::vector<StringEncoded> StringEncodedList;

#endif // GLOBAL_H
