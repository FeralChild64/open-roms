
#include "global.h"

// This class handles dynamicallly gererated (configuration dependend) strings
class StringInputDynamic
{
public:
	StringInputDynamic();
    void addToInput(std::vector<StringInputList> &stringInputLists);

private:
	std::string featureStr;
    std::string featureStrM65;
};
