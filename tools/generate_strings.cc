//
// Utility to generate compressed strings and BASIC tokens
//

// XXX this is a next-generation replacement of 'compress_text.c', still under development!
// XXX use dictionary compression using https://stackoverflow.com/questions/9195676/finding-the-smallest-number-of-substrings-to-represent-a-set-of-strings

#include "common.h"

#include <unistd.h>

#include <algorithm>
#include <sstream>
#include <map>
#include <vector>

//
// Command line settings
//

std::string CMD_outFile = "out.s";

//
// Type definition for text/tokens to generate
//

typedef struct StringEntry
{
	bool        enabledSTD;     // whether enabled for basic build 
	bool        enabledM65;     // whether enabled for Mega65 build
	bool        enabledX16;     // whether enabled for Commander X16 build
	std::string alias;          // alias, for the assembler
	std::string string;         // string/token
	uint8_t     abbrevLen = 0;  // length of token abbreviation
} StringEntry;

typedef struct StringEntryList
{
	std::string              name;
	std::vector<StringEntry> list;
} StringEntryList;

typedef std::vector<uint8_t>       StringEncoded;
typedef std::vector<StringEncoded> StringEncodedList;

// http://www.classic-games.com/commodore64/cbmtoken.html
// https://www.c64-wiki.com/wiki/BASIC_token

// BASIC keywords - V2 dialect

const StringEntryList GLOBAL_Keywords_V2 = { "keywords_V2",
{
    // STD    M65    X16 
	{ true,  true,  true,  "IDX_KV2_80",   "END",        2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_81",   "FOR",        2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_82",   "NEXT",       2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_83",   "DATA",       2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_84",   "INPUT#",     2 }, // https://www.atariarchives.org/creativeatari/Using_Disks_With_Atari_Basic.php
	{ true,  true,  true,  "IDX_KV2_85",   "INPUT",      0 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_86",   "DIM",        2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_87",   "READ",       2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_88",   "LET",        2 }, // https://en.wikipedia.org/wiki/Atari_BASIC
	{ true,  true,  true,  "IDX_KV2_89",   "GOTO",       2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_8A",   "RUN",        2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_8B",   "IF",         0 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_8C",   "RESTORE",    3 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_8D",   "GOSUB",      3 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_8E",   "RETURN",     3 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_8F",   "REM",        0 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_90",   "STOP",       2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_91",   "ON",         0 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_92",   "WAIT",       2 }, // http://www.picaxe.com/BASIC-Commands/Time-Delays/wait/
	{ true,  true,  true,  "IDX_KV2_93",   "LOAD",       2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_94",   "SAVE",       2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_95",   "VERIFY",     2 }, // https://en.wikipedia.org/wiki/Sinclair_BASIC
	{ true,  true,  true,  "IDX_KV2_96",   "DEF",        2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_97",   "POKE",       2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_98",   "PRINT#",     2 }, // https://www.atariarchives.org/creativeatari/Using_Disks_With_Atari_Basic.php
	{ true,  true,  true,  "IDX_KV2_99",   "PRINT",      0 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_9A",   "CONT",       2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_9B",   "LIST",       2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_9C",   "CLR",        2 }, // Apple I Replica Creation: Back to the Garage, p125
	{ true,  true,  true,  "IDX_KV2_9D",   "CMD",        2 }, // https://en.wikipedia.org/wiki/List_of_DOS_commands
	{ true,  true,  true,  "IDX_KV2_9E",   "SYS",        2 }, // https://www.lifewire.com/dos-commands-4070427
	{ true,  true,  true,  "IDX_KV2_9F",   "OPEN",       2 }, // https://www.atariarchives.org/creativeatari/Using_Disks_With_Atari_Basic.php
	// STD    M65    X16 
	{ true,  true,  true,  "IDX_KV2_A0",   "CLOSE",      3 }, // https://www.atariarchives.org/creativeatari/Using_Disks_With_Atari_Basic.php
	{ true,  true,  true,  "IDX_KV2_A1",   "GET",        2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_A2",   "NEW",        0 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_A3",   "TAB(",       2 }, // http://www.antonis.de/qbebooks/gwbasman/tab.html
	{ true,  true,  true,  "IDX_KV2_A4",   "TO",         0 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_A5",   "FN",         0 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_A6",   "SPC(",       2 }, // http://www.antonis.de/qbebooks/gwbasman/spc.html
	{ true,  true,  true,  "IDX_KV2_A7",   "THEN",       2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_A8",   "NOT",        2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_A9",   "STEP",       2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_AA",   "+",          0 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_AB",   "-",          0 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_AC",   "*",          0 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_AD",   "/",          0 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_AE",   "^",          0 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_AF",   "AND",        2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_B0",   "OR",         0 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_B1",   ">",          0 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_B2",   "=",          0 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_B3",   "<",          0 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_B4",   "SGN",        2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_B5",   "INT",        0 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_B6",   "ABS",        2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_B7",   "USR",        2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_B8",   "FRE",        2 }, // http://www.antonis.de/qbebooks/gwbasman/fre.html
	{ true,  true,  true,  "IDX_KV2_B9",   "POS",        0 }, // http://www.antonis.de/qbebooks/gwbasman/pos.html
	{ true,  true,  true,  "IDX_KV2_BA",   "SQR",        2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_BB",   "RND",        2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_BC",   "LOG",        0 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_BD",   "EXP",        2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_BE",   "COS",        0 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_BF",   "SIN",        2 }, // https://www.landsnail.com/a2ref.htm
	// STD    M65    X16 
	{ true,  true,  true,  "IDX_KV2_C0",   "TAN",        0 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_C1",   "ATN",        2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_C2",   "PEEK",       2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_C3",   "LEN",        0 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_C4",   "STR$",       3 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_C5",   "VAL",        2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_C6",   "ASC",        2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_C7",   "CHR$",       2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_C8",   "LEFT$",      3 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_C9",   "RIGHT$",     2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_CA",   "MID$",       2 }, // https://www.landsnail.com/a2ref.htm
	{ true,  true,  true,  "IDX_KV2_CB",   "GO",         0 }, // https://en.wikipedia.org/wiki/Goto
} };

// BASIC keywords - extended keywords list #1, for now just for testing

const StringEntryList GLOBAL_Keywords_X1 =  { "keywords_X1",
{
    // STD    M65    X16 
	{ false, true,  false, "IDX_KX1_01",   "TEST",         },
} };

// BASIC errors - all dialects

const StringEntryList GLOBAL_Errors =  { "errors",
{
	// STD    M65    X16   --- error strings compatible with CBM BASIC V2
	{ true,  true,  true,  "IDX_EV2_01", "TOO MANY FILES"           },
	{ true,  true,  true,  "IDX_EV2_02", "FILE OPEN"                },
	{ true,  true,  true,  "IDX_EV2_03", "FILE NOT OPEN"            },
	{ true,  true,  true,  "IDX_EV2_04", "FILE NOT FOUND"           },
	{ true,  true,  true,  "IDX_EV2_05", "DEVICE NOT PRESENT"       },
	{ true,  true,  true,  "IDX_EV2_06", "NOT INPUT FILE"           },
	{ true,  true,  true,  "IDX_EV2_07", "NOT OUTPUT FILE"          },
	{ true,  true,  true,  "IDX_EV2_08", "MISSING FILENAME"         },
	{ true,  true,  true,  "IDX_EV2_09", "ILLEGAL DEVICE NUMBER"    },
	{ true,  true,  true,  "IDX_EV2_0A", "NEXT WITHOUT FOR"         },
	{ true,  true,  true,  "IDX_EV2_0B", "SYNTAX"                   },
	{ true,  true,  true,  "IDX_EV2_0C", "RETURN WITHOUT GOSUB"     },
	{ true,  true,  true,  "IDX_EV2_0D", "OUT OF DATA"              },
	{ true,  true,  true,  "IDX_EV2_0E", "ILLEGAL QUANTITY"         },
	{ true,  true,  true,  "IDX_EV2_0F", "OVERFLOW"                 },
	{ true,  true,  true,  "IDX_EV2_10", "OUT OF MEMORY"            },
	{ true,  true,  true,  "IDX_EV2_11", "UNDEF\'D STATEMENT"       },
	{ true,  true,  true,  "IDX_EV2_12", "BAD SUBSCRIPT"            },
	{ true,  true,  true,  "IDX_EV2_13", "REDIM\'D ARRAY"           },
	{ true,  true,  true,  "IDX_EV2_14", "DIVISION BY ZERO"         },
	{ true,  true,  true,  "IDX_EV2_15", "ILLEGAL DIRECT"           },
	{ true,  true,  true,  "IDX_EV2_16", "TYPE MISMATCH"            },
	{ true,  true,  true,  "IDX_EV2_17", "STRING TOO LONG"          },
	{ true,  true,  true,  "IDX_EV2_18", "FILE DATA"                },
	{ true,  true,  true,  "IDX_EV2_19", "FORMULA TOO COMPLEX"      },
	{ true,  true,  true,  "IDX_EV2_1A", "CAN\'T CONTINUE"          },
	{ true,  true,  true,  "IDX_EV2_1B", "UNDEF\'D FUNCTION"        },
	{ true,  true,  true,  "IDX_EV2_1C", "VERIFY"                   },
	{ true,  true,  true,  "IDX_EV2_1D", "LOAD"                     },
	{ true,  true,  true,  "IDX_EV2_1E", "BREAK"                    },
	// STD    M65    X16   --- error strings compatible with CBM BASIC V7
	{ false, false, true,  "IDX_EV7_1F", "CAN'T RESUME"             },
	{ false, false, true,  "IDX_EV7_20", "LOOP NOT FOUND"           },
	{ false, false, true,  "IDX_EV7_21", "LOOP WITHOUT DO"          },
	{ false, false, true,  "IDX_EV7_22", "DIRECT MODE ONLY"         },
	{ false, false, true,  "IDX_EV7_23", "NO GRAPHICS AREA"         },
	{ false, false, true,  "IDX_EV7_24", "BAD DISK"                 },
	{ false, false, true,  "IDX_EV7_25", "BEND NOT FOUND"           },
	{ false, false, true,  "IDX_EV7_26", "LINE NUMBER TOO LARGE"    },
	{ false, false, true,  "IDX_EV7_27", "UNRESOLVED REFERENCE"     },
	{ true,   true, true,  "IDX_EV7_28", "NOT IMPLEMENTED"          }, // this is actually different message than V7 dialect prints
	{ false, false, true,  "IDX_EV7_29", "FILE READ"                },
	// STD    M65    X16   --- error strings specific to OpenROMs
	{ true,  true,  true,  "IDX_EOR_2A", "MEMORY CORRUPT"           },	
} };

// BASIC errors - miscelaneous strings

const StringEntryList GLOBAL_MiscStrings =  { "misc",
{
	// STD    M65    X16   --- misc strings as on CBM machines
	{ true,  true,  true,  "IDX_STR_BYTES",   " BASIC BYTES FREE"   },
	{ true,  true,  true,  "IDX_STR_READY",   "READY.\r"            },
	{ true,  true,  true,  "IDX_STR_ERROR",   " ERROR"              },
	{ true,  true,  true,  "IDX_STR_IN",      " IN "                },
	// STD    M65    X16   --- misc strings specific to OpenROMs
	{ true,  true,  true,  "IDX_STR_BRK",     "BRK AT $"            },
} };

//
// Work class definitions
//

class DataSet
{
public:

	void addStrings(const StringEntryList &stringList);

	const std::string &getOutput();

private:

	void process();

	void calculateFrequencies();
	void encodeStrings();

	void encodeByFreq(const std::string &plain, StringEncoded &encoded) const;

	void prepareOutput();

	virtual bool isRelevant(const StringEntry &entry) const;
	virtual std::string layoutName() const;

	std::vector<StringEntryList>          stringEntryLists;
	std::vector<StringEncodedList>        stringEncodedLists;

	std::vector<char>                     asNibble;
	std::vector<char>                     asByte;
	
	std::string outFileContent;
	bool        outFileContentValid = false;
};

class DataSetSTD : public DataSet
{
	bool isRelevant(const StringEntry &entry) const { return entry.enabledSTD; }
	std::string layoutName() const { return "STD"; }
};

class DataSetM65 : public DataSet
{
	bool isRelevant(const StringEntry &entry) const { return entry.enabledM65; }
	std::string layoutName() const { return "M65"; }
};

class DataSetX16 : public DataSet
{
	bool isRelevant(const StringEntry &entry) const { return entry.enabledX16; }
	std::string layoutName() const { return "X16"; }
};

//
// Work class implementation
//

void DataSet::addStrings(const StringEntryList &stringList)
{
	// Import the new list of strings

	stringEntryLists.push_back(stringList);

	// Clear strings not relevant for the current configuration
	
	while (1)
	{
		if (isRelevant(stringEntryLists.back().list.back())) break;

		stringEntryLists.back().list.pop_back();
		if (stringEntryLists.back().list.empty())
		{
			ERROR(std::string("no valid strings in layout '") + layoutName() + "', list'" + stringEntryLists.back().name + "'");
		}
	}
	
	outputStringValid = false;
}

void DataSet::process()
{
	calculateFrequencies();
	encodeStrings();
	prepareOutput();
}

const std::string &DataSet::getOutput()
{
	if (!outputStringValid)
	{
		process();
	}
	
	return outputString;
}

void DataSet::calculateFrequencies()
{
	asNibble.clear();
	asByte.clear();
	
	std::map<char, uint16_t> freqMap;
	
	// Calculate frequencies

	for (const auto &stringEntryList : stringEntryLists)
	{
		for (const auto &stringEntry : stringEntryList.list)
		{
			if (!isRelevant(stringEntry)) continue;

			for (const auto &character : stringEntry.string)
			{
				if (character >= 0x80)
				{
					ERROR(std::string("character above 0x80 in string '") + stringEntry.string + "'");
				}
				
				freqMap[character]++;
			}
		}
	}
	
	// Sort characters
	
	std::vector<std::pair<char, uint16_t>> freqVector;
	
	for (auto iter = freqMap.begin(); iter != freqMap.end(); ++iter)
	{
		freqVector.push_back(*iter);
	}
	
	std::sort(freqVector.begin(), freqVector.end(),
	          [](std::pair<char, uint16_t> e1, std::pair<char, uint16_t> e2)
			  { return e2.second < e1.second; });

	// Extract 14 most frequent characters to be encoded as nibble, remaining as byte

	uint8_t counter = 0;
	
	for (const auto &freqPair : freqVector)
	{
		if (counter < 14)
		{
			asNibble.push_back(freqPair.first);
		}
		else
		{
			asByte.push_back(freqPair.first);
		}
		
		counter++;
	}	
}

void DataSet::encodeByFreq(const std::string &plain, StringEncoded &encoded) const
{
	bool fullByte = true;

	auto putNibble = [&](uint8_t val)
	{
		if (fullByte)
		{
			encoded.push_back(val);
			fullByte = false;
		}
		else
		{
			encoded.back() += val * 0x10;
			fullByte = true;
		}
	};

	auto putByte = [&](uint8_t val)
	{
		if (fullByte)
		{
			encoded.push_back(val);
		}
		else
		{
			encoded.back() += (val % 0x10) * 0x10;
			encoded.push_back(val / 0x10);			
		}
	};

	// Encode every single character by frequency, put them in the output vector

	for (const char &character : plain)
	{
		auto iterNibble = std::find(asNibble.begin(), asNibble.end(), character);
		if (iterNibble != asNibble.end())
		{
			putNibble(std::distance(asNibble.begin(), iterNibble) + 1);
		}
		else
		{
			putNibble(0x0F);

			auto iterByte = std::find(asByte.begin(), asByte.end(), character);
			if (iterByte == asByte.end())
			{
				ERROR("internal error in 'encodeByFreq'");
			}
			putByte(std::distance(asByte.begin(), iterByte) + 1);
		}
	}

	// Make sure the last byte of encoded stream is 0

	if (encoded.size() == 0 || encoded.back() != 0) encoded.push_back(0);
}

void DataSet::encodeStrings()
{
	stringsEncoded.clear();

	// Encode every relevant string from every list by frequency

	// XXX
}

void DataSet::prepareOutput()
{
	// XXX
	
	outputStringValid = true;
}


//
// Common helper functions
//

void printUsage()
{
    std::cout << "\n" <<
        "usage: generate_strings [-o <out file>]" << "\n\n";
}

void printBanner()
{
    printBannerLineTop();
    std::cout << "// Generating compressed strings and BASIC tokens" << "\n";
    printBannerLineBottom();
}

void parseCommandLine(int argc, char **argv)
{
    int opt;

    // Retrieve command line options

    while ((opt = getopt(argc, argv, "o:")) != -1)
    {
        switch(opt)
        {
            case 'o': CMD_outFile   = optarg; break;
            default: printUsage(); ERROR();
        }
    }
}

void writeStrings()
{
	DataSetSTD dataSetSTD;
	DataSetM65 dataSetM65;
	DataSetX16 dataSetX16;

	dataSetSTD.addStrings(GLOBAL_Keywords_V2);
	dataSetSTD.addStrings(GLOBAL_Errors);
	dataSetSTD.addStrings(GLOBAL_MiscStrings);

	dataSetM65.addStrings(GLOBAL_Keywords_V2);
	dataSetM65.addStrings(GLOBAL_Keywords_X1);
	dataSetM65.addStrings(GLOBAL_Errors);
	dataSetM65.addStrings(GLOBAL_MiscStrings);

	DataSetX16.addStrings(GLOBAL_Keywords_V2);
	DataSetX16.addStrings(GLOBAL_Errors);
	DataSetX16.addStrings(GLOBAL_MiscStrings);

	// XXX
}

//
// Main function
//

int main(int argc, char **argv)
{
    printBanner();
    parseCommandLine(argc, argv);

    writeStrings();

    return 0;
}
