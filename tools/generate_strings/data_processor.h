
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
        const InputStringEntry &inputEntry;
        std::vector<uint8_t>   encoded;
        const bool             relevant;

        StringEntry(const InputStringEntry &inputEntry) : inputEntry(inputEntry), relevant(true)
        {
        }

        StringEntry() : inputEntry(GLOBAL_DummyInputString), relevant(false)
        {
        }

    } StringEntry;

    typedef struct StringList
    {
        std::list<StringEntry> list;

        const ListType         listType;
        const std::string      listName;
        const bool             encDict = false;
        const bool             encFreq = false;

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
    uint8_t getAllocDictEntry(const std::string &entryString);
    uint8_t allocDictEntry(const std::string &entryString);

    void writeResults();
};



#endif // DATA_PROCESSOR_H
