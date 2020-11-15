
//
// Utility to generate compressed messages and BASIC keywords
//

#include "data_processor.h"



DataProcessor::DataProcessor()
{
    for (const auto &inputList : GLOBAL_InputSet)
    {
        addInputList(inputList);
    }

    if (stringSet.empty()) ERROR(std::string("no valid strings in layout '") + GLOBAL_BuildTypeName + "'");
}

void DataProcessor::process()
{
    createDictionary();

	// XXX

    writeResults();
}

void DataProcessor::writeResults() // XXX move it down
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
        case ListType::BASIC_ERRORS:
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
            stringList.push_back(StringEntry(inputStringEntry));
        }
        else if (inputList.listType == ListType::BASIC_KEYWORDS || inputList.listType == ListType::BASIC_ERRORS)
        {
            // Push empty item, - for certain lists we want to keep indices constant

            stringList.push_back(StringEntry());
        }
    }
}

void DataProcessor::createDictionary()
{
    if (!GLOBAL_Compression_Msg_Dict) return;

    stringDict = StringList(ListType::DICTIONARY, "dict", false, GLOBAL_Compression_Msg_Freq);

    for (auto &stringList : stringSet)
    {
        if (!stringList.listType.encDict) continue;

        for (auto &stringEntry : stringList.list)
        {
            if (!stringEntry.relevant || stringEntry.inputEntry.string.empty()) continue;

            // Replace the string with dictionary entry reference

            uint8_t dictIdx = getAllocDictEntry(stringEntry.inputEntry.string);



            // XXX

        }
    }

    // If dictionary is empty (unlikely) - disable dictionary compression

    if (stringDict.list.empty()) GLOBAL_Compression_Msg_Dict = false;
}

uint8_t DataProcessor::getAllocDictEntry(const std::string &entryString)
{
	// XXX
}

uint8_t DataProcessor::allocDictEntry(const std::string &entryString)
{
	if (stringDict.list.size() >= 255)
	{
	    ERROR("max 255 strings allowed for dictionary compression");
	}
	
	// XXX
}
