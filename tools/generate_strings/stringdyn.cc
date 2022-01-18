
#include "stringdyn.h"


StringInputDynamic::StringInputDynamic()
{
    // Generate string to show the build features
   
    featureStr;
    featureStrM65 = "\r";

    // Tape support features
   
    if (GLOBAL_ConfigOptions["TAPE_NORMAL"] && GLOBAL_ConfigOptions["TAPE_TURBO"])
    {
        featureStr    += "TAPE LOAD NORMAL TURBO\r";
        featureStrM65 += "TAPE     : LOAD NORMAL TURBO\r";
    }
    else if (GLOBAL_ConfigOptions["TAPE_NORMAL"])
    {
        featureStr    += "TAPE LOAD NORMAL\r";
        featureStrM65 += "TAPE     : LOAD NORMAL\r";
    }
    else if (GLOBAL_ConfigOptions["TAPE_TURBO"])
    {
        featureStr    += "TAPE LOAD TURBO\r";
        featureStrM65 += "TAPE     : LOAD TURBO\r";
    }
   
    // IEC support features
   
    if (GLOBAL_ConfigOptions["IEC"])
    {
        featureStr    += "IEC";
        featureStrM65 += "IEC      :";
       
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
        featureStrM65 += "IEC      : -\r";
    }

    // RS-232 support features
   
    if (GLOBAL_ConfigOptions["RS232_ACIA"])   featureStr += "ACIA 6551\r";
    if (GLOBAL_ConfigOptions["RS232_UP2400"]) featureStr += "UP2400\r";
    if (GLOBAL_ConfigOptions["RS232_UP9600"]) featureStr += "UP9600\r";
   
    featureStrM65 += "RS-232   : -\r";

    // CBDOS features

    featureStrM65 += "\rSD CARD  : ";

    // Keyboard support features
   
    if (GLOBAL_ConfigOptions["KEYBOARD_C128"]) featureStr += "KBD 128\r";
}

void StringInputDynamic::addToInput(std::vector<StringInputList> &stringInputLists)
{
    // Add strings to appropriate list
   
    for (auto &stringInputList : stringInputLists)
    {
        if (stringInputList.name != std::string("misc")) continue;

        // List found

        if (GLOBAL_ConfigOptions["SHOW_FEATURES"] || GLOBAL_ConfigOptions["MB_M65"])
        {
            StringInputEntry newEntry1 = { true, true, true, true, true, "STR_PAL",      "PAL\r"    };
            StringInputEntry newEntry2 = { true, true, true, true, true, "STR_NTSC",     "NTSC\r"   };

            stringInputList.list.push_back(newEntry1);
            stringInputList.list.push_back(newEntry2);
        }

        if (GLOBAL_ConfigOptions["SHOW_FEATURES"])
        {
            StringInputEntry newEntry = { true, true, true, true, true, "STR_FEATURES", featureStr };
            stringInputList.list.push_back(newEntry);
        }

        if (GLOBAL_ConfigOptions["MB_M65"])
        {
            StringInputEntry newEntry = { false, true, true, false, false, "STR_SI_FEATURES", featureStrM65 };
            stringInputList.list.push_back(newEntry);
        }

        if (!GLOBAL_ConfigOptions["BRAND_CUSTOM_BUILD"] || GLOBAL_ConfigOptions["MB_M65"])
        {
            StringInputEntry newEntry = { true, true, true, true, true, "STR_PRE_REV", "RELEASE " };
            stringInputList.list.push_back(newEntry);
        }

        break;
    }
}
