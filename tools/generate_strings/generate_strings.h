
//
// Utility to generate compressed messages and BASIC keywords
//

#ifndef GENERATE_STRINGS_H
#define GENERATE_STRINGS_H

#include "common.h"

#include <unistd.h>

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <regex>
#include <sstream>
#include <string>
#include <list>
#include <map>
#include <vector>

//
// Type definitions
//

enum class ListType
{
    BASIC_KEYWORDS,
    BASIC_ERRORS,
	BASIC_STRINGS,
    KERNAL_STRINGS,
    DICTIONARY
};

typedef struct InputStringEntry
{
    bool        enabledSTD = false; // if enabled for standard build
    bool        enabledCRT = false; // if enabled for standard build with extra ROM cartridge
    bool        enabledM65 = false; // if enabled for Mega 65 build
    bool        enabledU64 = false; // if enabled for Ultimate 64 build
    bool        enabledX16 = false; // if enabled for Commander X16 build
    std::string alias;              // alias, for the assembler
    std::string string;             // string/token
    uint8_t     abbrevLen = 0;      // length of token abbreviation

} InputStringEntry;

typedef struct InputList
{
	const std::list<InputStringEntry> &list;
	const ListType                    listType;
	const std::string                 listName;

} InputList;

enum class BuildType
{
    STD,
    CRT,
    M65,
    U64,
    X16
};


#endif // GENERATE_STRINGS_H
