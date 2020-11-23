
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
    optimizeDictionaryEncoding();

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
            if (!stringEntry.relevant || stringEntry.string.empty()) continue;

            // Set initial encoding as index of the inly string in the dictionary

            stringEntry.encoded.push_back(getAllocDictEntry(stringEntry.string));
        }
    }

    // If dictionary is empty (unlikely) - disable dictionary compression

    if (stringDict.list.empty()) GLOBAL_Compression_Msg_Dict = false;
}

uint8_t DataProcessor::getAllocDictEntry(const std::string &string)
{
	// Check if dictionary already contains given string

	for (uint8_t idx = 0; idx < stringDict.list.size(); idx++)
	{
		if (string.compare(stringDict.list[idx].string) == 0)
		{
			stringDict.list[idx].relevant = true;
			return idx;
		}
	}

	// String not present, allocate new entry

	return allocDictEntry(string);
}

uint8_t DataProcessor::allocDictEntry(const std::string &string)
{
	// First try to reuse irrelevant (effectively empty) entry

	for (uint8_t idx = 0; idx < stringDict.list.size(); idx++)
	{
		if (!stringDict.list[idx].relevant)
		{
			stringDict.list[idx] = StringEntry(string);
			return idx;
		}
	}

	// Not free entry found - add a new one

	if (stringDict.list.size() >= 255)
	{
	    ERROR("max 255 strings allowed for dictionary compression");
	}

	stringDict.list.insert(StringEntry(string));

	return stringDict.list.size() - 1;
}

void DataProcessor::optimizeDictionaryEncoding()
{
	while (true)
	{
		// If maximum size of dictionary reached - nothing can be done

		if (dictionary.size() >= 255) return;

    	// Select words, which can potentially be extracted to new substrings
   
    	std::vector<std::string> candidateList;
    	getDictCandidates(candidateList);

	    // Now find the best candidate for a new dictionary entry
	   
	    auto bestCandidate = candidateList.end();
	    uint32_t bestScore = 0;

	    for (auto iter = candidateList.begin(); iter < candidateList.end(); iter++)
	    {
	        uint32_t score = evaluateDictCandidate(*iter);

	        if (score > bestScore)
	        {
	            bestCandidate = iter;
	            bestScore     = score;
	        }
	    }

	    // If no candidates were evaluated - nothing to do

	    if (bestScore < 1) return;

	    // Only allow for replacements that brings some size benefit
	   
	    if (bestScore <= evaluateDictCompression()) return;

	    // Replace substrings with the new dictionary entry

	    applyDictReplacement(bestCandidate);
	}
}

void DataProcessor::getDictCandidates(std::vector<std::string> &candidateList)
{
	// XXX consider checking for substrings of spaces only too

    for (const auto &dictEntry : stringDict.list)
    {
        // Split the dictionary entry into words

        std::istringstream entryStream(dictEntry.string);
        while (entryStream)
        {
	        std::istringstream entryStream(dictEntry.string);
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
	           
	            auto addToList = [&dictEntry, &candidateList](std::string &word)
	            {
	                // Before adding, make sure the word exists in the string,
	                // is not already on the list, and it's size makes it worth trying

	                if (word.size() < 1) return;
	                if (dictEntry.string.size() - word.size() > 1) return;
	                if (dictEntry.string.find(word) == std::string::npos) return;
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
}

uint32_t DataProcessor::evaluateDictCandidate(const std::string &candidate) const
{
	DataProcessor testProcessor = *this;
	testProcessor.applyDictReplacement(candidate);
	return testProcessor.evaluateDictCompression();
}

uint32_t DataProcessor::evaluateDictCompression() const
{
	uint32_t score = 0;

	// First count the number of bytes needed for the dictionary

    for (const auto &dictEntry : stringDict.list)
    {
    	if (!dictEntry.relevant) continue;

    	// XXX for freq compressed dictionary, add compressed size instead
    	score += dictEntry.string.size() + 1;
    }

    // Count bytes needed for the dictionary compressed strings

    for (const auto &stringList : stringSet)
    {
    	if (!stringList.encDict) continue;

    	for (const auto &stringEntry : stringList.list)
    	{
    		score += dictEntry.encoded.size() + 1;
    	}
	}

	// Return results as score (higher value = better)

	return 65536 - score;
}

void DataProcessor::applyDictReplacement(const std::string &newString)
{
    // First add our selected candidate to the dictionary

	uint8_t newIdx = getAllocDictEntry(newString);

	// Replace all strings which are equal to the new one

	for (uint8_t idx = 0; idx < dictionary.list.size(); idx++)
	{
		if (!dictionary.list[idx].relevant || idx == newIdx) continue;

		// XXX

        // Mark obsolete dictionary entry as free for removal

		dictionary.list[idx].relevant = false;
	}

	// XXX
}







       
        if (*iter != selectedStr || selectedStrIdx == currentStrIdx) continue;
       
        // Replace the current string with the new one
       
        for (auto &encoding : encodings) for (auto &byte : *encoding)
        {
            if (byte == currentStrIdx) byte = selectedStrIdx;
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