
//
// Utility to generate compressed messages and BASIC keywords
//

#include "common.h"
#include "config_generated_strings.h"

#include <unistd.h>

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <regex>
#include <sstream>
#include <map>
#include <vector>

//
// Command line settings
//

std::string CMD_outFile = "out.s";
std::string CMD_cnfFile = "";

//
// Type definition for strings/keywords to generate
//

enum class ListType
{
    BASIC_KEYWORDS,
	BASIC_STRINGS,
    KERNAL_STRINGS,
    DICTIONARY
};

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

//
// Global variables
//

static std::map<std::string, bool> GLOBAL_ConfigOptions;
static BuildType                   GLOBAL_BuildType;
static std::string                 GLOBAL_BuildTypeName;

const std::list<InputList>  GLOBAL_InputSet =
{
    { LIST_Keywords_V2,       ListType::BASIC_KEYWORDS, "keywords_V2" },
    { LIST_Keywords_01,       ListType::BASIC_KEYWORDS, "keywords_01" },
    { LIST_Keywords_04,       ListType::BASIC_KEYWORDS, "keywords_04" },
    { LIST_Keywords_06,       ListType::BASIC_KEYWORDS, "keywords_06" },
    { LIST_Errors,            ListType::BASIC_STRINGS,  "errors"      },
    { LIST_MiscStrings,       ListType::BASIC_STRINGS,  "misc"        },
    { LIST_Kernal,            ListType::KERNAL_STRINGS, "kernal"      },
};


static bool GLOBAL_Compression_BKw_Freq = false;
static bool GLOBAL_Compression_Msg_Freq = false;
static bool GLOBAL_Compression_Msg_Dict = false;

//
// Common helper functions
//

void parseConfigFile()
{
    GLOBAL_ConfigOptions.clear();
   
    // Open the configuration file
   
    std::ifstream cnfFile;
    cnfFile.open(CMD_cnfFile);
    if (!cnfFile.good()) ERROR("unable to open config file");
   
    // Parse the file
   
    size_t lineNum = 0;
    while (!cnfFile.eof())
    {
        lineNum++;
        std::string workStr;
       
        // Read a single line, remove leading spaces and tabs

        std::getline(cnfFile, workStr);
        if (cnfFile.bad()) ERROR("error reading configuration file");
        workStr = std::regex_replace(workStr, std::regex("^[ \t]+"), "");

        // Split the line into tokens

        std::vector<std::string> tokens;

        std::istringstream workStream(workStr);
        std::string token;
        while(std::getline(workStream, token, ' '))
        {
            if (token.empty()) continue;
            tokens.push_back(token);
        }
        if (tokens.empty()) continue;

        // Only accept lines which are boolean config options set to YES

        if (tokens.size() < 4 ||
            tokens[0].compare(";;")       != 0 ||
            tokens[1].compare("#CONFIG#") != 0 ||
            tokens[3].compare("YES")      != 0)
        {
            continue;
        }

        // Add definitin to config option map

        if (tokens[2].empty()) ERROR(std::string("error parsing config file - line ") + std::to_string(lineNum));

        GLOBAL_ConfigOptions[tokens[2]] = true;
    }
   
    cnfFile.close();

    // Determine build type

    if (GLOBAL_ConfigOptions["PLATFORM_COMMANDER_X16"])
    {
        GLOBAL_BuildType     = BuildType::X16;
        GLOBAL_BuildTypeName = "X16";
    }
    else if (GLOBAL_ConfigOptions["PLATFORM_COMMODORE_64"] && GLOBAL_ConfigOptions["MB_M65"])
    {
        GLOBAL_BuildType     = BuildType::M65;
        GLOBAL_BuildTypeName = "M65";
    }
    else if (GLOBAL_ConfigOptions["PLATFORM_COMMODORE_64"] && GLOBAL_ConfigOptions["ROM_CRT"])
    {
        GLOBAL_BuildType     = BuildType::CRT;
        GLOBAL_BuildTypeName = "CRT";
    }
    else if (GLOBAL_ConfigOptions["PLATFORM_COMMODORE_64"] && GLOBAL_ConfigOptions["MB_U64"])
    {
        GLOBAL_BuildType     = BuildType::U64;
        GLOBAL_BuildTypeName = "U64";
    }
    else if (GLOBAL_ConfigOptions["PLATFORM_COMMODORE_64"])
    {
        GLOBAL_BuildType     = BuildType::STD;
        GLOBAL_BuildTypeName = "STD";
    }
    else
    {
        ERROR("unable to determine build type");
    }

    // Determine string compression type

    if (GLOBAL_ConfigOptions["COMPRESSED_KEYWORDS_FREQ"])
    {
        GLOBAL_Compression_BKw_Freq = true;
    }
    if (GLOBAL_ConfigOptions["COMPRESSED_MSG_FREQ"])
    {
        GLOBAL_Compression_Msg_Freq     = true;
    }
    if (GLOBAL_ConfigOptions["COMPRESSED_MSG_DICT"])
    {
        GLOBAL_Compression_Msg_Dict     = true;
    }
}

void printUsage()
{
    std::cout << "\n" <<
        "usage: generate_strings [-o <out file>] [-c <configuration file>]" << "\n\n";
}

void printBanner()
{
    printBannerLineTop();
    std::cout << "// Generating compressed messages and BASIC tokens\n";
    printBannerLineBottom();
}

void parseCommandLine(int argc, char **argv)
{
    int opt;

    // Retrieve command line options

    while ((opt = getopt(argc, argv, "o:c:")) != -1)
    {
        switch(opt)
        {
            case 'o': CMD_outFile   = optarg; break;
            case 'c': CMD_cnfFile   = optarg; break;
            default: printUsage(); ERROR();
        }
    }
}

//
// Main class
//


class DataProcessor
{
public:

	DataProcessor();
	
	void process();
	void write();
	
private:

    typedef struct StringEntry
    {
        const InputStringEntry &inputEntry;
        std::vector<uint8_t>   encoded;
        const bool             relevant;

    } StringEntry;

    typedef struct StringList
    {
        std::list<StringEntry> list;

        const ListType         listType;
        const std::string      listName;
        const bool             encDict;
        const bool             encFreq;

        StringList(ListType listType, const std::string &listName, bool encDict, bool encFreq) :
            listType(listType), listName(listName), encDict(encDict), encFreq(encFreq)
        {
        }

    } StringList;

    std::list<StringList> stringSet;

    bool isRelevant(const InputStringEntry &inputStringEntry) const;
    void addInputList(const InputList &inputList);
};

DataProcessor::DataProcessor()
{
    for (const auto &inputList : GLOBAL_InputSet)
    {
        addInputList(inputList);
    }
}

void DataProcessor::process()
{
	// XXX
}

void DataProcessor::write()
{
    // Remove old file

    unlink(CMD_outFile.c_str());

    // Open output file for writing

    std::ofstream outFile(CMD_outFile, std::fstream::out | std::fstream::trunc);
    if (!outFile.good()) ERROR(std::string("can't open oputput file '") + CMD_outFile + "'");

    // Write header

    outFile << ";\n; Generated file - do not edit\n;";

    // Write packed strings

    outFile << std::endl << std::endl;
    outFile << outputString;                    // XXX generate this!
    outFile << std::endl << std::endl;

    // Close the file
  
    outFile.close();

    std::cout << std::string("compressed strings written to: '") + CMD_outFile + "'\n\n";
}

bool DataProcessor::isRelevant(const InputStringEntry &inputStringEntry) const
{
    switch (GLOBAL_BuildType)
    {
        case GLOBAL_BuildType::STD: return inputStringEntry.enabledSTD;
        case GLOBAL_BuildType::CRT: return inputStringEntry.enabledCRT;
        case GLOBAL_BuildType::M65: return inputStringEntry.enabledM65;
        case GLOBAL_BuildType::U64: return inputStringEntry.enabledU64;
        case GLOBAL_BuildType::X16: return inputStringEntry.enabledX16;
        default: ERROR(std::string("please update ") + __FUNCTION__);
    }

    return false;
}

void DataProcessor::addInputList(const InputList &inputList)
{
    // First check if the list contains anything relevant to current build

    bool isListRelevant = false;
    for (auto &inputStringEntry : inputList.list)
    {
        if (isRelevant(inputStringEntry))
        {
            isListRelevant = true;
            break;
        }
    }

    if (!isListRelevant) return;

    // Prepare the list of string entries

    bool encDict = false;
    bool encFreq = false;

    switch (inputList.listType)
    {
        case ListType::BASIC_KEYWORDS:
        {
            encFreq = GLOBAL_Compression_BKw_Freq;
        }
        break;
        case ListType::BASIC_STRINGS:
        {
            if (GLOBAL_Compression_Msg_Dict)
            {
                encDict = true;
            }
            else if (GLOBAL_Compression_Msg_Freq)
            {
                encFreq = true;
            }
        }
        break;
        case ListType::KERNAL_STRINGS: break;
        default: ERROR(std::string("please update ") + __FUNCTION__);
    }

    stringSet.push_back(StringList(inputList.listType, inputList.listName, encDict, encFreq));

    // Now copy the whole list into internal structures, handling irrelevant entries in the mean time

    auto &stringList = stringSet.back();
    for (auto &inputStringEntry : inputList.list)
    {
        if (isRelevant(inputStringEntry))
        {
            stringList.push_back(); // XXX
        }
        else if (inputList.listType)
        {
            // Push empty reference

            // XXX
        }
    }
}




//
// Main function
//

int main(int argc, char **argv)
{
    printBanner();

    parseCommandLine(argc, argv);
    parseConfigFile();

    DataProcessor dataProcessor;
    dataProcessor.process();
    dataProcessor.write();

    return 0;
}















    const std::string &getOutput();

private:

    void process();

    void generateConfigDepStrings();
    void validateLists();
    void calculateFrequencies();
    void encodeStringsDict();
    void encodeStringsFreq();

    void encodeByFreq(const std::string &plain, StringEncoded &encoded) const;

    void prepareOutput();
    void prepareOutput_1n_3n(std::ostringstream &stream);
    void prepareOutput_labels(std::ostringstream &stream,
                              const StringEntryList &stringEntryList,
                              const StringEncodedList &stringEncodedList);
    void prepareOutput_packed(std::ostringstream &stream,
                              const StringEntryList &stringEntryList,
                              const StringEncodedList &stringEncodedList);

    void putCharEncoding(std::ostringstream &stream, uint8_t idx, char character, bool is3n);

    bool isCompressionLvl2(const StringEntryList &list) const;

    virtual bool isRelevant(const StringEntry &entry) const = 0;
    virtual std::string layoutName() const = 0;

    std::vector<StringEntryList>          stringEntryLists;
    std::vector<StringEncodedList>        stringEncodedLists;

    std::vector<char>                     as1n; // list of bytes to be encoded as 1 nibble
    std::vector<char>                     as3n; // list of bytes to be encoded as 3 nibbles

    uint8_t                               tk__packed_as_3n    = 0;
    uint8_t                               tk__max_keyword_len = 0;

    size_t                                maxAliasLen          = 0;
    std::string                           outFileContent;
};


//
// Work class implementation
//

void DictEncoder::addString(const std::string &inString, StringEncoded *outPtr)
{
    // Store the pointer to encoding

    encodings.push_back(outPtr);

    // Check if string is already present in the dictionary

    auto pos = std::find(dictionary.begin(), dictionary.end(), inString);

    // Create initial encoding

    if (pos != dictionary.end())
    {
        encodings.back()->push_back(pos - dictionary.begin());
    }
    else
    {
        if (dictionary.size() > 255)
        {
            ERROR("max 255 strings allowed for dictionary compression");
        }

        dictionary.push_back(inString);
        encodings.back()->push_back(dictionary.size() - 1);
    }
}


void DictEncoder::extractWords(std::vector<std::string> &candidateList)
{
    for (const auto &dictionaryEntry : dictionary)
    {
        // Split the dictionary entry into words
       
        std::istringstream entryStream(dictionaryEntry);
        while (entryStream)
        {
            std::string word_1;
            entryStream >> word_1;

            if (word_1.empty()) continue;
           
            // Create possible variants with spaces

            std::string word_2 = " " + word_1;
            std::string word_3 = word_1 + " ";
            std::string word_4 = word_2 + " ";

            // If necessary, add the word to candidate list
           
            auto addToList = [&dictionaryEntry, &candidateList](std::string &word)
            {
                // Before dding, make sure the word variant exists in the string,
                // is not already on the list, and is long enough it makes sense to try

                if (word.size() < 1) return;
                if (dictionaryEntry.find(word) == std::string::npos) return;
                if (std::find(candidateList.begin(), candidateList.end(), word) != candidateList.end()) return;

                candidateList.push_back(word);
            };
           
            addToList(word_1);
            addToList(word_2);
            addToList(word_3);
            addToList(word_4);
        }
    }
}

int32_t DictEncoder::evaluateCandidate(std::string &candidate)
{
    // XXX it looks like this is not 100% right - debug the algorithm if dictionary compression is reintroduced

    // Build the score - find how much bytes can be spared by extracting this particular candidate
    // Limited size of the dictionary is taken into consideration
   
    int32_t  score      = -(candidate.size() + 1);
    uint32_t targetSize = dictionary.size() + 1;

    // First check for situation when the candidate equals the currently existing dictionary entry

    for (const auto &dictionaryEntry : dictionary)
    {   
        if (dictionaryEntry == candidate)
        {
            // Candidate equals the string

            score += candidate.size() + 1;
            targetSize--;
            continue;
        }
    }

    // Now check the remaining cases

    for (const auto &dictionaryEntry : dictionary)
    {   
        if (dictionaryEntry == candidate) continue; // already handled

        // Find all the occurences of the candidate in the current string

        std::vector<uint8_t> occurences;
        for (auto iter = dictionaryEntry.begin(); iter < (dictionaryEntry.end() - candidate.size() + 1); iter++)
        {
            if (candidate != std::string(iter, iter + candidate.size())) continue;
           
            occurences.push_back(iter - dictionaryEntry.begin());
            iter += candidate.size() - 1; // to make sure we have no occurence overlapping
        }
       
        if (occurences.empty()) continue;
       
        // Now summarize what we are going to gain when doing replacement
       
        if (occurences[0] != 0)
        {
            // For the substring before the first occurence we need to create a separate entry
            // in the dictionary - unless it is already there, it brings some additional cost

            std::string otherStr = std::string(dictionaryEntry.begin(), dictionaryEntry.end() + occurences[0]);

            if (std::find(dictionary.begin(), dictionary.end(), otherStr) != dictionary.end())
            {
                // This extra string is already in the dictionary - that gives extra saving
                score += otherStr.size() - 1;
            }
            else
            {
                // This extra string is not in the dictionary, it has to be added
                score -= 2;
                targetSize++;
            }
        }
       
        for (auto iter = occurences.begin(); iter < occurences.end(); iter++)
        {
            // With a replacement, we should gain some bytes
            score += candidate.size() - 1;

            // If the occurence ends with the last byte of dictionary string - nothing more to do
            if (*iter + candidate.size() == dictionaryEntry.size()) break;

            // If next occurence starts immediately after this one - next iteration
            if (iter + 1 != occurences.end() && (*(iter + 1) - *iter) == (uint8_t) candidate.size()) continue;

            // For the substring between this occurence and the next one (or the end of the string)
            // we need to create a separate entry in the dictionary - unless it is already there,
            // it brings some additional cost

            std::string otherStr;
            if (iter + 1 == occurences.end())
            {
                otherStr = std::string(dictionaryEntry.begin() + *iter + candidate.size(),
                                       dictionaryEntry.end());
            }
            else
            {
                otherStr = std::string(dictionaryEntry.begin() + *iter + candidate.size(),
                                       dictionaryEntry.begin() + *(iter + 1));
            }

            if (std::find(dictionary.begin(), dictionary.end(), otherStr) != dictionary.end())
            {
                // This extra string is already in the dictionary - that gives extra saving
                score += otherStr.size() - 1;
            }
            else
            {
                // This extra string is not in the dictionary, it has to be added
                score -= 2;
                targetSize++;
            }           
        }
       
        // We crossed the maximum number of strings (leave one free for the work buffer),
        // do not consider this candidate
       
        if (targetSize > 254) return -1;
    }
   
    return score;
}

void DictEncoder::cleanupDictionary()
{
    // Get rid of dictionary entries which are not needed anymore
   
    bool isClean = false;
   
    while (!isClean)
    {
        isClean = true;
       
        for (auto iter = dictionary.begin(); iter < dictionary.end(); iter++)
        {
            if (!iter->empty()) continue;
           
            // Found an empty string in the dictionary - remove it and adapt encoding
           
            isClean = false;
            uint8_t obsoleteStrIdx = iter - dictionary.begin();
           
            dictionary.erase(iter);
           
            for (auto &encoding : encodings) for (auto &byte : *encoding)
            {
                if (byte >= obsoleteStrIdx) byte--;
            }
           
            break;
        }
    }
}

bool DictEncoder::optimizeSplit()
{
    // XXX - The code here is not the best one - redesign and algorithm improvements
    // (like - consider optimizing as a game and apply alpha-beta scheme to select the best
    // candidate, incorporate frequency encoding into candidate evaluation) could result in
    // a slightly better compression. But currently we have very few strings, and the dictionary
    // compression is disabled by default - so right now this is not worth the effort.
   
    if (dictionary.size() >= 255) return false; // up to 255 entries in the dictionary are possible
   
    // First select words, which can potentially be extracted to new substrings
   
    std::vector<std::string> candidateList;
    extractWords(candidateList);

    // XXX possible future improvement: find a couple of largest common substrings and add them as candidates too
   
    // Now find the best word to be extracted
   
    auto bestCandidate         = candidateList.end();
    int32_t bestCandidateScore = 0;
   
    for (auto iter = candidateList.begin(); iter < candidateList.end(); iter++)
    {
        int32_t candidateScore = evaluateCandidate(*iter);

        if (candidateScore > bestCandidateScore)
        {
            bestCandidate      = iter;
            bestCandidateScore = candidateScore;
        }
    }
   
    // Only allow for replacements that brings some size benefit
   
    if (bestCandidateScore < 1) return false;
   
    // Extract the best candidate to a separate string
   
    // First add our selected candidate to the dictionary and replace all strings which are equal to it
   
    auto    &selectedStr   = *bestCandidate;
    uint8_t selectedStrIdx = dictionary.size();
   
    dictionary.push_back(selectedStr);

    for (auto iter = dictionary.begin(); iter < dictionary.end(); iter++)
    {
        uint8_t currentStrIdx = iter - dictionary.begin();
       
        if (*iter != selectedStr || selectedStrIdx == currentStrIdx) continue;
       
        // Replace the current string with the new one
       
        for (auto &encoding : encodings) for (auto &byte : *encoding)
        {
            if (byte == currentStrIdx) byte = selectedStrIdx;
        }
       
        // Mark obsolete dictionary entry as free for removal
       
        iter->clear();
    }

    cleanupDictionary();

    // Now handle normal cases, when the string needs to be split
    // XXX - this is not time-effective, optimize this in the future

    bool optimizeAgain = true;
    bool nexIteration  = true;
    while (nexIteration)
    {
        nexIteration = false;

        for (auto iter = dictionary.begin(); iter < dictionary.end(); iter++)
        {
            auto    &currentStr     = *iter;
            uint8_t currentStrIdx   = iter - dictionary.begin();   
            auto    selectedStrIter = std::find(dictionary.begin(), dictionary.end(), selectedStr);
            selectedStrIdx          = selectedStrIter - dictionary.begin();

            if (currentStrIdx == selectedStrIdx) continue;
   
            // Check if the current string contains the selected string
           
            auto pos = currentStr.find(selectedStr);
            if (pos == std::string::npos) continue;
               
            nexIteration  = true;
            optimizeAgain = true;
   
            // Prepare replacement sequence for 'selectedStrIdx'
               
            std::vector<uint8_t> replacement;
           
            if (pos == 0)
            {
                // The current dictionary string starts with our selected substring
               
                replacement.push_back(selectedStrIdx);
                currentStr = currentStr.substr(selectedStr.size(), currentStr.size() - selectedStr.size());
               
                if (!currentStr.empty())
                {
                    // Check if the remaining string is present somewhere else in the dictionary; if so, reuse it
               
                    uint8_t pos2 = currentStrIdx;
                    for (auto iter2 = dictionary.begin(); iter2 < dictionary.end(); iter2++)
                    {
                        if (iter2 == iter) continue;
                        if (currentStr == *iter2)
                        {
                            dictionary[pos2].clear();
                            pos2 = iter2 - dictionary.begin();
                            break;
                        }
                    }
                   
                    replacement.push_back(pos2);
                }
            }
            else
            {
                // The current dictionary string does not start with our selected substring
               
                std::string newStr = currentStr.substr(0, pos);
                currentStr         = currentStr.substr(pos, currentStr.size() - pos);
               
                // Check if the newStr is present somewhere else in the dictionary; if so, reuse it
               
                bool    found = false;
                uint8_t pos2  = 0;
                for (auto iter2 = dictionary.begin(); iter2 < dictionary.end(); iter2++)
                {
                    if (*iter2 == newStr)
                    {
                        found = true;
                        pos2  = iter2 - dictionary.begin();

                        break;
                    }
                }
           
                if (found)
                {
                    replacement.push_back(pos2);                   
                }
                else
                {
                    dictionary.push_back(newStr);
                    replacement.push_back(dictionary.size() - 1);                   
                }

                replacement.push_back(currentStrIdx);
            }

            // Now replace all the occurences of 'selectedStrIdx' with a 'replacement' vector

            for (auto &encoding : encodings)
            {
                for (uint8_t idx = 0; idx < encoding->size(); idx++)
                {
                    if ((*encoding)[idx] != currentStrIdx) continue;
                   
                    // Perform replacement
                   
                    encoding->erase(encoding->begin() + idx);
                   
                    for (const auto replacementIdx : replacement)
                    {
                        encoding->insert(encoding->begin() + idx, replacementIdx);
                        idx++;
                    }
                   
                    idx--;
                }
            }
           
            cleanupDictionary();
        }
    }

    return optimizeAgain;
}

bool DictEncoder::optimizeJoin()
{
    // Try to optimize by joining two substrings
   
    // XXX - possible future improvement
   
    return false;
}

void DictEncoder::optimizeOrder()
{
    // XXX - it would be better to do this after the dictionary is compressed

    // Try to optimize order of strings in the dictionary for fastest display
    // - put the shorter substrings first
    // - for strings with roughly the same length put more frequently used first
   
    typedef struct Entry
    {
        uint32_t    penalty;
        std::string word;
    } Entry;
   
    // Create copy of the dictionary with optimization info
   
    std::vector<Entry> optimizedOrder;
    for (auto iter = dictionary.begin(); iter < dictionary.end(); iter++)
    {
        const auto &dictionaryStr = *iter;
   
        uint8_t  sizePenalty = dictionaryStr.size() / 3;
        uint16_t freqPenalty = 65535;
       
        // Calculate penalty for low occurence frequency
       
        for (auto &encoding : encodings)
        {
            for (auto &byte : *encoding)
            {
                if (byte == (iter - dictionary.begin())) freqPenalty--;
            }
        }

        // Create new entry
       
        Entry newEntry;
        newEntry.word    = dictionaryStr;
        newEntry.penalty = sizePenalty * 65536 + freqPenalty;

        optimizedOrder.push_back(newEntry);
    }
   

    // Determine ordering - sort from smallest penalty to largest
   
    std::sort(optimizedOrder.begin(), optimizedOrder.end(),
              [](const Entry &e1, const Entry &e2) { return e1.penalty < e2.penalty; });

    // Create helper table for reordering
   
    std::vector<uint8_t> reorderTable;
   
    for (uint8_t idx1 = 0; idx1 < dictionary.size(); idx1++)
    {
        uint8_t idx2 = 0;
        while (dictionary[idx1] != optimizedOrder[idx2].word) idx2++;
       
        reorderTable.push_back(idx2);       
    }

    // Perform the reordering
   
    for (auto &encoding : encodings) for (auto &byte : *encoding)
    {
        byte = reorderTable[byte];
    }
   
    for (uint8_t idx = 0; idx < dictionary.size(); idx++) dictionary[idx] = optimizedOrder[idx].word;
}

void DictEncoder::process(StringEntryList &outDictionary)
{
    if (dictionary.empty()) return;
    // Optimize as long as it brings any improvement

    while (optimizeSplit() || optimizeJoin()) ;
    optimizeOrder();

    // Export the dictionary to external format

    outDictionary.type = ListType::DICTIONARY;
    outDictionary.name = "DICTIONARY";

    for (const auto &dictionaryStr : dictionary)
    {
        StringEntry newEntry = { true, true, true, true, "", dictionaryStr };
        outDictionary.list.push_back(newEntry);
    }

    // Adapt the encoding to external format (0 = end of string)

    for (auto &encoding : encodings)
    {
        for (auto &byte : *encoding) byte++;
        encoding->push_back(0);
    }
}

bool DataSet::isCompressionLvl2(const StringEntryList &list) const
{
    return (GLOBAL_ConfigOptions["COMPRESSION_LVL_2"] && list.type == ListType::STRINGS_BASIC);
}

void DataSet::addStrings(const StringEntryList &stringList)
{
    // Import the new list of strings

    stringEntryLists.push_back(stringList);
    stringEncodedLists.emplace_back();

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
   
    // Clear the output content - make sure it is not valid anymore

    outFileContent.clear();
}

void DataSet::process()
{
    std::cout << "processing file '" << CMD_cnfFile << "', layout '" << layoutName() << "'" << std::endl;

    generateConfigDepStrings();
    validateLists();
    encodeStringsDict();
    calculateFrequencies();
    encodeStringsFreq();   
    prepareOutput();
}

const std::string &DataSet::getOutput()
{
    if (outFileContent.empty())
    {
        process();
    }

    return outFileContent;
}

void DataSet::generateConfigDepStrings()
{
    // Generate string to show the build features
   
    std::string featureStr;
    std::string featureStrM65 = "\r";

    // Tape support features
   
    if (GLOBAL_ConfigOptions["TAPE_NORMAL"] && GLOBAL_ConfigOptions["TAPE_TURBO"])
    {
        featureStr    += "TAPE LOAD NORMAL TURBO\r";
        featureStrM65 += "TAPE   : LOAD NORMAL TURBO\r";
    }
    else if (GLOBAL_ConfigOptions["TAPE_NORMAL"])
    {
        featureStr    += "TAPE LOAD NORMAL\r";
        featureStrM65 += "TAPE   : LOAD NORMAL\r";
    }
    else if (GLOBAL_ConfigOptions["TAPE_TURBO"])
    {
        featureStr    += "TAPE LOAD TURBO\r";
        featureStrM65 += "TAPE   : LOAD TURBO\r";
    }
   
    // IEC support features
   
    if (GLOBAL_ConfigOptions["IEC"])
    {
        featureStr    += "IEC";
        featureStrM65 += "IEC    :";
       
        bool extendedIEC = false;
       
        if (GLOBAL_ConfigOptions["IEC_BURST_CIA1"])
        {
            featureStr    += " BURST1";
            extendedIEC    = true;
        }
        if (GLOBAL_ConfigOptions["IEC_BURST_CIA2"])
        {
            featureStr    += " BURST2";
            extendedIEC    = true;
        }
        if (GLOBAL_ConfigOptions["IEC_BURST_MEGA65"])
        {
            featureStr    += " BURST";
            featureStrM65 += " BURST";
            extendedIEC    = true;
        }
       
        if (GLOBAL_ConfigOptions["IEC_DOLPHINDOS"])
        {
            featureStr    += " DOLPHIN";
            featureStrM65 += " DOLPHIN";
            extendedIEC    = true;
        }
       
        if (GLOBAL_ConfigOptions["IEC_JIFFYDOS"])
        {
            featureStr    += " JIFFY";
            featureStrM65 += " JIFFY";
            extendedIEC    = true;
        }
       
        if (!extendedIEC)
        {
            featureStr    += " NORMAL ONLY";
            featureStrM65 += " NORMAL ONLY";
        }

        featureStr    += "\r";
        featureStrM65 += "\r";
    }
    else
    {
        featureStrM65 += "IEC    : NO\r";
    }

    // RS-232 support features
   
    if (GLOBAL_ConfigOptions["RS232_ACIA"])   featureStr += "ACIA 6551\r";
    if (GLOBAL_ConfigOptions["RS232_UP2400"]) featureStr += "UP2400\r";
    if (GLOBAL_ConfigOptions["RS232_UP9600"]) featureStr += "UP9600\r";
   
    featureStrM65 += "RS-232 : NO\r";

    // CBDOS features

    featureStrM65 += "CBDOS  : NO FDD/SD/RAM SUPPORT\r";

    // Keyboard support features
   
    if (GLOBAL_ConfigOptions["KEYBOARD_C128"]) featureStr += "KBD 128\r";
    if (GLOBAL_ConfigOptions["KEYBOARD_C65"])  featureStr += "KBD 65\r";

    // Add strings to appropriate list
   
    for (auto &stringEntryList : stringEntryLists)
    {
        if (stringEntryList.name != std::string("misc")) continue;

        // List found

        if (GLOBAL_ConfigOptions["SHOW_FEATURES"] || GLOBAL_ConfigOptions["MB_M65"])
        {
            StringEntry newEntry1 = { true, true, true, true, true, "STR_PAL",      "PAL\r"    };
            StringEntry newEntry2 = { true, true, true, true, true, "STR_NTSC",     "NTSC\r"   };

            stringEntryList.list.push_back(newEntry1);
            stringEntryList.list.push_back(newEntry2);
        }

        if (GLOBAL_ConfigOptions["SHOW_FEATURES"])
        {
            StringEntry newEntry = { true, true, true, true, true, "STR_FEATURES", featureStr };
            stringEntryList.list.push_back(newEntry);
        }

        if (GLOBAL_ConfigOptions["MB_M65"])
        {
            StringEntry newEntry = { false, true, true, false, false, "STR_SI_FEATURES", featureStrM65 };
            stringEntryList.list.push_back(newEntry);
        }

        if (!GLOBAL_ConfigOptions["BRAND_CUSTOM_BUILD"] || GLOBAL_ConfigOptions["MB_M65"])
        {
            StringEntry newEntry = { true, true, true, true, true, "STR_PRE_REV", "RELEASE " };
            stringEntryList.list.push_back(newEntry);
        }

        break;
    }
}

void DataSet::validateLists()
{
    for (const auto &stringEntryList : stringEntryLists)
    {
        for (const auto &stringEntry : stringEntryList.list)
        {
            // Check for maximum allowed string length
            if (stringEntry.string.length() > 255) ERROR("string cannot be longer than 255 characters");

            // Check mor maximum keyword length
            if (stringEntryList.type == ListType::KEYWORDS)
            {
                if (stringEntry.string.length() > 16) ERROR("keyword cannot be longer than 16 characters");
                tk__max_keyword_len = std::max(tk__max_keyword_len, (uint8_t) stringEntry.string.length());
            }

            // Update maximum alias length too
            maxAliasLen = std::max(maxAliasLen, stringEntry.alias.length());

            for (const auto &character : stringEntry.string)
            {
                if ((unsigned char) character >= 0x80)
                {
                    ERROR(std::string("character above 0x80 in string '") + stringEntry.string + "'");
                }
            }           
        }
    }
}

void DataSet::encodeStringsDict()
{
    DictEncoder dictEncoder;

    // Add strings for dictionary compresssion

    for (uint8_t idx = 0; idx < stringEntryLists.size(); idx++)
    {
        const auto &stringEntryList = stringEntryLists[idx];
        auto &stringEncodedList     = stringEncodedLists[idx];

        // Skip lists not to be encoded using the dictionary
        if (!isCompressionLvl2(stringEntryList)) continue;

        stringEncodedList.resize(stringEntryList.list.size());
        for (uint8_t idxEntry = 0; idxEntry < stringEntryList.list.size(); idxEntry++)
        {
            dictEncoder.addString(stringEntryList.list[idxEntry].string,
                                  &stringEncodedList[idxEntry]);
        }
    }

    // Perform the compression

    StringEntryList dictionary;
    dictEncoder.process(dictionary);

    // Add new lists

    stringEntryLists.push_back(dictionary);
    stringEncodedLists.emplace_back();
}

void DataSet::calculateFrequencies()
{
    as1n.clear();
    as3n.clear();
   
    std::map<char, uint16_t> freqMapGeneral;  // general character frequency map
    std::map<char, uint16_t> freqMapKeywords; // frequency map for keywords

    // Calculate frequencies of characters in the strings

    for (const auto &stringEntryList : stringEntryLists)
    {
        // Skip lists encoded by the dictionary
        if (isCompressionLvl2(stringEntryList)) continue;

        for (const auto &stringEntry : stringEntryList.list)
        {
            if (!isRelevant(stringEntry)) continue;

            for (const auto &character : stringEntry.string)
            {               
                freqMapGeneral[character]++;
                if (stringEntryList.type == ListType::KEYWORDS) freqMapKeywords[character]++;
            }
        }
    }
   
    // Sort characters by frequency
   
    std::vector<char> freqVector1;
   
    for (auto iter = freqMapGeneral.begin(); iter != freqMapGeneral.end(); ++iter)
    {
        freqVector1.push_back(iter->first);
    }
   
    std::sort(freqVector1.begin(), freqVector1.end(), [&freqMapGeneral](char e1, char e2)
              { return freqMapGeneral[e2] < freqMapGeneral[e1]; });

    // Check if minimal amount of characters needed, below 15 is not supported by the 6502 side code

    if (freqVector1.size() < 15) ERROR(std::string("not enough distinct characters in layout '") + layoutName() + "', at least 15 needed");

    // Extract 14 most frequent characters to be encoded as 1 nibble
   
    for (uint8_t idx = 0; idx < 14; idx++)
    {
        as1n.push_back(freqVector1[idx]);
    }
   
    // Now sort them by frequency in keywords, in descending order - this will speed up the tokenizer a little
   
    std::sort(as1n.begin(), as1n.end(), [&freqMapKeywords](char e1, char e2) { return freqMapKeywords[e2] > freqMapKeywords[e1]; });

    // Extract characters to be encoded as 3 nibbles, which actually exist in keywords

    std::vector<char> freqVector2;

    tk__packed_as_3n = 0;
    for (uint8_t idx = 14; idx < freqVector1.size(); idx++)
    {
        const auto &character = freqVector1[idx];
       
        if (freqMapKeywords[character] > 0)
        {
            as3n.push_back(character);
            tk__packed_as_3n++;
        }
        else
        {
            freqVector2.push_back(character);
        }
    }
    tk__packed_as_3n = std::max(tk__packed_as_3n, (uint8_t) 1);

    // Again, sort them by frequency in keywords, in descending order - this will speed up the tokenizer a little
   
    std::sort(as3n.begin(), as3n.end(), [&freqMapKeywords](char e1, char e2) { return freqMapKeywords[e2] > freqMapKeywords[e1]; });

    // Finally extract the remaining characters to be encoded as 3 nibbles

    for (const auto &character : freqVector2)
    {
        as3n.push_back(character);
    }
}

void DataSet::encodeByFreq(const std::string &plain, StringEncoded &encoded) const
{
    bool fullByte = true;

    auto push1n = [&](uint8_t val) // push 1 nibble - encoded character or 0xF mark
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

    auto push2n = [&](uint8_t val) // push the remaining nibbles for 3-niobble encoded characters
    {
        if (fullByte)
        {
            encoded.push_back(val);
        }
        else
        {
            // Encode byte in a way to be easily decoded by 6502
            encoded.back() += (val / 0x10) * 0x10;
            encoded.push_back(val % 0x10);           
        }
    };

    // Encode every single character by frequency, put them in the output vector

    for (const char &character : plain)
    {
        auto iterEncoding1 = std::find(as1n.begin(), as1n.end(), character);
        if (iterEncoding1 != as1n.end())
        {
            push1n(std::distance(as1n.begin(), iterEncoding1) + 1);
        }
        else
        {
            push1n(0x0F);

            auto iterEncoding3 = std::find(as3n.begin(), as3n.end(), character);
            if (iterEncoding3 == as3n.end())
            {
                ERROR("internal error in 'encodeByFreq'");
            }
            push2n(std::distance(as3n.begin(), iterEncoding3) + 1);
        }
    }

    // Make sure the last byte of encoded stream is 0

    if (encoded.size() == 0 || encoded.back() != 0) encoded.push_back(0);
}

void DataSet::encodeStringsFreq()
{
    // Encode every relevant string from every list - by character frequency

    for (uint8_t idx = 0; idx < stringEntryLists.size(); idx++)
    {
        const auto &stringEntryList = stringEntryLists[idx];
        auto &stringEncodedList = stringEncodedLists[idx];

        // Skip lists encoded by the dictionary
        if (isCompressionLvl2(stringEntryList)) continue;

        // Perform frequency encoding of the list

        for (const auto &stringEntry : stringEntryList.list)
        {
            stringEncodedList.emplace_back();
            auto &stringEncoded = stringEncodedList.back();

            if (isRelevant(stringEntry))
            {
                encodeByFreq(stringEntry.string, stringEncoded);
            }
        }
    }
}

void DataSet::putCharEncoding(std::ostringstream &stream, uint8_t idx, char character, bool is3n)
{
    stream << "\t!byte $" << std::uppercase << std::hex <<
              std::setfill('0') << std::setw(2) << +character <<
              "    ; " << std::setfill(is3n ? '0' : ' ') << std::setw(2) << +idx;

    std::string petscii;

    if (character == 0x20)
    {
        petscii = " = SPACE";
    }
    else if (character == 0x27)
    {
        petscii = " = APOSTROPHE";
    }
    else if (character == 0x0D)
    {
        petscii = " = RETURN";
    }
    else if (character > ' ' && character < 'a')
    {
        petscii = std::string(" = '") + character + "'";
    }

    stream << petscii << std::endl;
}

void DataSet::prepareOutput_1n_3n(std::ostringstream &stream)
{
    uint8_t idx;

    // Export all nibble-encoded characters

    stream << std::endl << "!macro PUT_PACKED_AS_1N { ; characters encoded as 1 nibble" << std::endl << std::endl;
   
    idx = 0;
    for (const auto& encoding : as1n)
    {
        putCharEncoding(stream, ++idx, encoding, false);
    }
   
    stream << "}" << std::endl;

    // Export all byte-encoded characters

    stream << std::endl << "!macro PUT_PACKED_AS_3N { ; characters encoded as 3 nibbles" << std::endl << std::endl;

    idx = 0;
    for (const auto& encoding : as3n)
    {
        if (idx == tk__packed_as_3n) stream << std::endl <<
            "\t; Characters below are not used by any BASIC keyword" << std::endl << std::endl;

        putCharEncoding(stream, ++idx, encoding, true);
    }

    stream << "}" << std::endl;
}

void DataSet::prepareOutput_labels(std::ostringstream &stream,
                                   const StringEntryList &stringEntryList,
                                   const StringEncodedList &stringEncodedList)
{
    // For the dictionary we do not have any labels
    if (stringEntryList.type == ListType::DICTIONARY) return;

    stream << std::endl;
    for (uint8_t idx = 0; idx < stringEncodedList.size(); idx++)
    {
        const auto &stringEntry   = stringEntryList.list[idx];
        const auto &stringEncoded = stringEncodedList[idx];

        if (!stringEncoded.empty())
        {
            stream << "!set IDX__" << stringEntry.alias <<
                      std::string(maxAliasLen - stringEntry.alias.length(), ' ') << " = $" <<
                      std::uppercase << std::hex << std::setfill('0') << std::setw(2) << +idx << std::endl;
        }
    }
}

void DataSet::prepareOutput_packed(std::ostringstream &stream,
                                   const StringEntryList &stringEntryList,
                                   const StringEncodedList &stringEncodedList)
{
    if (isCompressionLvl2(stringEntryList))
    {
        stream << std::endl << "!macro PUT_PACKED_DICT_";
    }
    else
    {
        stream << std::endl << "!macro PUT_PACKED_FREQ_";           
    }

    stream << stringEntryList.name << " {" << std::endl << std::endl;

    enum LastStr { NONE, SKIPPED, WRITTEN } lastStr = LastStr::NONE;
    for (uint8_t idxString = 0; idxString < stringEncodedList.size(); idxString++)
    {
        const auto &stringEncoded = stringEncodedList[idxString];

        if (stringEncoded.empty())
        {
            if (stringEntryList.type == ListType::DICTIONARY) ERROR("internal error"); // should never happen

            if (lastStr == LastStr::WRITTEN) stream << std::endl;
            stream << "\t!byte $00    ; skipped " << stringEntryList.list[idxString].alias << std::endl;
            lastStr = LastStr::SKIPPED;
        }
        else
        {
            if (lastStr != LastStr::NONE) stream << std::endl;

            // Output the label - as a comment

            if (stringEntryList.type != ListType::DICTIONARY)
            {
                stream << "\t; IDX__" << stringEntryList.list[idxString].alias << std::endl;
            }

            // Output the source string - as a comment

            stream << "\t; '";
            for (auto &character : stringEntryList.list[idxString].string)
            {
                if (character >= 32 && character <= 132 && character != 39 && character != 34)
                {
                    stream << character;
                }
                else if (character == 13)
                {
                    stream << "<return>";
                }
                else
                {
                    stream << "_";
                }
            }
            stream << "'" << std::endl;

            // Output the encoding

            stream << "\t!byte ";

            bool first = true;
            for (const auto &charEncoded : stringEncoded)
            {
                if (first)
                {
                    first = false;
                }
                else
                {
                    stream << ", ";
                }

                stream << "$" << std::uppercase << std::hex << std::setfill('0') << std::setw(2) << +charEncoded;
            }

            stream << std::endl;
            lastStr = LastStr::WRITTEN;
        }
    }

    // For the token list - put the 'end of keywords' mark

    if (stringEntryList.type == ListType::KEYWORDS)
    {
        stream << std::endl << "\t; Marker - end of the keyword list" << std::endl;
        stream << "\t!byte $FF, $FF" << std::endl;
    }

    stream << "}" << std::endl;
}

void DataSet::prepareOutput()
{
    // Convert our encoded strings to a KickAssembler source

    std::ostringstream stream;

    // Export 1-nibble and 3-nibble encoding data

    prepareOutput_1n_3n(stream);

    // Export additional data for the tokenizer

    stream << std::endl << "!set TK__PACKED_AS_3N    = $" << std::hex << +tk__packed_as_3n <<
              std::endl << "!set TK__MAX_KEYWORD_LEN = "  << std::dec << +tk__max_keyword_len << std::endl;

    // Export encoded strings

    for (uint8_t idx = 0; idx < stringEntryLists.size(); idx++)
    {
        const auto &stringEntryList   = stringEntryLists[idx];
        const auto &stringEncodedList = stringEncodedLists[idx];

        if (stringEncodedList.empty()) continue;

        // Export labels for the current list

        prepareOutput_labels(stream, stringEntryList, stringEncodedList);

        // For the token list - put the number of tokens available

        if (stringEntryList.type == ListType::KEYWORDS)
        {
            stream << std::endl << "!set TK__MAXTOKEN_" << stringEntryList.name << " = " <<
                      std::dec << stringEncodedList.size() << std::endl;
        }

        // Export the packed data

        prepareOutput_packed(stream, stringEntryList, stringEncodedList);
    }

    // Finalize the file stream

    stream << std::endl;
    outFileContent = stream.str();
}

