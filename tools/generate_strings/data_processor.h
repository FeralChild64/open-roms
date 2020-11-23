
//
// Utility to generate compressed messages and BASIC keywords
//

#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

#include "generate_strings.h"



class DataProcessor
{
public:

	DataProcessor();
	
	void process();
	
private:

    typedef struct StringEntry
    {
        const std::string alias;        // alias, for the assembler
        std::string string;             // original string/token
        const uint8_t abbrevLen = 0;    // length of token abbreviation

        std::vector<uint8_t>   encoded;
        bool                   relevant = false;

        StringEntry(const InputStringEntry &inputEntry) :
            alias(inputEntry.alias),
            string(inputEntry.string),
            abbrevLen(inputEntry.abbrevLen),
            relevant(true)
        {
        };

        StringEntry(std::string &string) : string(string), relevant(true) { };
        StringEntry() { };

    } StringEntry;

    typedef struct StringList
    {
        std::vector<StringEntry> list;

        const ListType           listType;
        const std::string        listName;
        const bool               encDict = false;
        const bool               encFreq = false;

        StringList(ListType listType, const std::string &listName, bool encDict, bool encFreq) :
            listType(listType), listName(listName), encDict(encDict), encFreq(encFreq)
        {
        }

    } StringList;

    std::list<StringList> stringSet;
    StringList            stringDict;

    bool isRelevant(const InputStringEntry &inputStringEntry) const;
    void addInputList(const InputList &inputList);

    void createDictionary();
    void optimizeDictionaryEncoding();

    uint8_t getAllocDictEntry(const std::string &string);
    uint8_t allocDictEntry(const std::string &string);

    void getDictCandidates(std::vector<std::string> &candidateList);
    uint32_t evaluateDictCandidate(const std::string &candidate) const;
    uint32_t evaluateDictCompression() const;
    void applyDictReplacement(const std::string &newString);

    void writeResults();
};



#endif // DATA_PROCESSOR_H
