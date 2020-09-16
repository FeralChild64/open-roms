//
// Utility to build a segment from the provided assembler
// source files, placing routines where they need to live
//

#include "common.h"

#include <dirent.h>
#include <libgen.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <list>
#include <map>
#include <set>
#include <vector>

const std::string LAB_OUT_START = "__routine_START_";
const std::string LAB_OUT_END   = "__routine_END_";
const std::string LAB_IN_START  = "\t" + LAB_OUT_START;
const std::string LAB_IN_END    = "\t" + LAB_OUT_END;

//
// Command line settings
//

std::string CMD_assembler = "./acme";
std::string CMD_outFile   = "OUT.BIN";
std::string CMD_outDir    = "./out";
std::string CMD_segName   = "MAIN";
std::string CMD_segInfo   = "(unnamed)";
std::string CMD_romLayout = "STD";
int         CMD_loAddress = 0xC000;
int         CMD_hiAddress = 0xCFFF;

std::list<std::string> CMD_inList;

//
// Common helper functions
//

void printUsage()
{
    std::cout << "\n" <<
        "usage: build_segment [-a <assembler command>] [-o <out file>] [-d <out dir>]" << "\n" <<
        "                     [-l <start/low address>] [-h <end/high address>]" << "\n" <<
        "                     [-s <segment name>] [-i <segment display info>]" << "\n" <<
        "                     [-r <rom layout>]" << "\n" <<
        "                     <input dir/file list>" << "\n\n";
}

void printBannerCollectAnalyse()
{
    printBannerLineTop();
    std::cout << "// Segment '" << CMD_segInfo << "' - collecting and analysing routines" << "\n";
    printBannerLineBottom();
}

void printBannerBinCompile()
{
    printBannerLineTop();
    std::cout << "// Segment '" << CMD_segInfo << "' - binning and compiling the assembly" << "\n";
    printBannerLineBottom();
}

std::string toLabel(const std::string &fileName)
{
    std::string retVal = fileName;

    std::replace(retVal.begin(), retVal.end(), ',', '_');
    std::replace(retVal.begin(), retVal.end(), '.', '_');

    return retVal;
}

//
// Class definitions
//

class SourceFile
{
public:
    SourceFile(const std::string &fileName, const std::string &dirName);

    void preprocess();
    bool preprocessLine(const std::string &line);
    bool nameMatch(const std::string &token, const std::string &name);

    std::string fileName;
    std::string dirName;

    bool ignore;
    bool floating;
    bool high;
    std::vector<char> content;

    std::string label;
    int startAddr;     // for fixed (non-floating) routines only
    int codeLength;    // for both fixed and floating routines

    int testAddrStart; // start address during test run
    int testAddrEnd;   // end address during test run
};

class BinningProblem
{
public:

    BinningProblem() {};
    BinningProblem(int loAddress, int hiAddress);

    bool isSolved() const;

    void addToProblem(SourceFile *routine);
    void fillGap(DualStream &logOutput, int gapAddress, const std::list<SourceFile *> &routines);
    void placeHighRoutines(DualStream &logOutput);
    void performObviousSteps(DualStream &logOutput);
    void removeUselessGaps(DualStream &logOutput);
    void sortFloatingRoutinesBySize();

    std::map<int, SourceFile *> fixedRoutines;    // routines with location already fixed
    std::map<int, int>          gaps;             // gaps by address
    std::vector<SourceFile *>   floatingRoutines; // routines not allocated to any address yet; always keep them sorted!

    int statSize;
    int statFree;
    int statWasted;
};

class Solver
{
public:
    explicit Solver(BinningProblem &problem) : problem(problem), logOutput(logFile, std::cout) {}

    void run();

    int selectGapToFill();
    void findPartialSolution(int gapSize, std::list<SourceFile *> &partialSolution);

private:

    BinningProblem &problem;
    std::ofstream logFile;

    DualStream logOutput;
};

//
// Global variables
//

std::list<SourceFile> GLOBAL_sourceFiles;
std::list<SourceFile> GLOBAL_sourceFiles_noCode;
size_t                GLOBAL_maxFileNameLen    = 0;
size_t                GLOBAL_totalRoutinesSize = 0;
BinningProblem        GLOBAL_binningProblem;

//
// Top-level functions
//

void parseCommandLine(int argc, char **argv)
{
    int opt;

    // Retrieve command line options

    while ((opt = getopt(argc, argv, "a:o:d:s:i:r:l:h:")) != -1)
    {
        switch(opt)
        {
            case 'a': CMD_assembler = optarg; break;
            case 'o': CMD_outFile   = optarg; break;
            case 'd': CMD_outDir    = optarg; break;
            case 's': CMD_segName   = optarg; break;
            case 'i': CMD_segInfo   = optarg; break;
            case 'r': CMD_romLayout = optarg; break;
            case 'l': CMD_loAddress = strtol(optarg, nullptr, 16); break;
            case 'h': CMD_hiAddress = strtol(optarg, nullptr, 16); break;
            default: printUsage(); ERROR();
        }
    }

    // Retrieve file/directory list

    for (int idx = optind; idx < argc; idx++)
    {
        CMD_inList.push_back(argv[idx]);
    }

    if (CMD_inList.empty()) { printUsage(); ERROR("empty directory/file list"); }
}

void readSourceFiles()
{
    struct stat statBuf;
    for (const auto &objName : CMD_inList)
    {
        if (stat(objName.c_str(), &statBuf) < 0)
        {
            ERROR(std::string("can't get information about '") + objName + "'");
        }

        if (S_ISREG(statBuf.st_mode))
        {
            // This is a regular file

            char *tmp1  = strdup(objName.c_str());
            char *tmp2 = strdup(objName.c_str());

            std::string dirName  = dirname(tmp1);
            std::string fileName = basename(tmp2);

            free(tmp1);
            free(tmp2);

            GLOBAL_sourceFiles.push_back(SourceFile(fileName, dirName));
            GLOBAL_maxFileNameLen = std::max(GLOBAL_maxFileNameLen, fileName.length());
            continue;
        }

        // This should be a directory

        DIR *dirHandle = opendir(objName.c_str());
        if (!dirHandle) ERROR(std::string("unable to open directory '") + objName + "'");

        struct dirent *dirEntry;
        while ((dirEntry = readdir(dirHandle)) != nullptr)
        {
            const std::string fileName = dirEntry->d_name;

            // Filter-out files which are not assembler files, temporary, etc.

            if (fileName.length() < 3)   continue;
            if (fileName.front() == '#') continue;
            if (fileName.front() == '~') continue;
            if (fileName.substr(fileName.length() - 2) != ".s") continue;

            // Add file to the global list

            GLOBAL_sourceFiles.push_back(SourceFile(fileName, objName));
            GLOBAL_maxFileNameLen = std::max(GLOBAL_maxFileNameLen, fileName.length());
        }

        closedir(dirHandle);
    }

    // Filter-out files marked as ignored

    GLOBAL_sourceFiles.erase(std::remove_if(GLOBAL_sourceFiles.begin(), GLOBAL_sourceFiles.end(),
        [](const SourceFile &sourceFile) -> bool { return sourceFile.ignore; }), GLOBAL_sourceFiles.end());

    if (GLOBAL_sourceFiles.empty()) ERROR("no source files found");

    // Sort the file list by name, to provide deterministic results

    auto compare = [](const SourceFile &a, const SourceFile &b) -> bool { return a.fileName.compare(b.fileName) < 0; };
    GLOBAL_sourceFiles.sort(compare);
}

void checkInputFileLabels()
{
    std::set<std::string> usedLabels;
    for (const auto &sourceFile : GLOBAL_sourceFiles)
    {
        if (usedLabels.count(sourceFile.label) != 0)
        {
            ERROR(std::string("input file '") + sourceFile.fileName + "' has a name too similar to another one");
        }

        usedLabels.insert(sourceFile.label);
    }
}

void calcRoutineSizes()
{
    const std::string nameBase = CMD_segName + "_sizetest";
    const std::string filePath = CMD_outDir + DIR_SEPARATOR;

    const std::string outFileNameBare = nameBase + ".s";
    const std::string outFileNamePath = filePath + outFileNameBare;
    const std::string symFileNamePath = filePath + nameBase + ".sym";

    // Remove old files

    unlink(outFileNamePath.c_str());
    unlink(symFileNamePath.c_str());

    // Write test file to determine routine sizes

    std::ofstream outFile(outFileNamePath, std::fstream::out | std::fstream::trunc);
    if (!outFile.good()) ERROR(std::string("can't open temporary file '") + outFileNamePath + "'");

    // Start at $100, so that no local data gets accesses using ZP addressing modes
    // during this pass, which would otherwise upset things later

    outFile << "\n" << "*=$100" << "\n";
    outFile << "!set SEGMENT_" << CMD_segName << " = 1\n";
    outFile << "!set ROM_LAYOUT_" << CMD_romLayout << " = 1\n";

    for (const auto &sourceFile : GLOBAL_sourceFiles)
    {
        outFile << "\n\n\n\n";
        outFile << ";--- Source file " << sourceFile.fileName << "\n\n";
        outFile << "!zone " << toLabel(sourceFile.fileName) << "\n\n";
        outFile << LAB_OUT_START << sourceFile.label << ":" << "\n\n";

        outFile << std::string(sourceFile.content.begin(), sourceFile.content.end());

        outFile << "\n\n";
        outFile << LAB_OUT_END << sourceFile.label << ":" << "\n";
    }

    if (!outFile.good()) ERROR(std::string("error writing temporary file '") + outFileNamePath + "'");
    outFile.close();

    // All written - now launch the assembler

    const std::string cmd = "cd " + filePath + " && " + CMD_assembler +
                            " --color --outfile /dev/null --symbollist " + CMD_segName + "_sizetest.sym " +
                            outFileNameBare;
    std::cout << "command: " << cmd << "\n" << std::flush;
    if (0 != system(cmd.c_str()))
    {
        ERROR("assembler running failed");
    }

    // Read addresses

    std::ifstream symFile;
    symFile.open(symFileNamePath);
    if (!symFile.good()) ERROR(std::string("unable to open results file '") + symFileNamePath + "'");

    std::string line;
    while (std::getline(symFile, line))
    {
        bool startLabel;

        if (0 == line.compare(0, LAB_IN_START.size(), LAB_IN_START))
        {
            line.erase(0, LAB_IN_START.length());
            startLabel = true;
        }
        else if (0 == line.compare(0, LAB_IN_END.size(), LAB_IN_END))
        {
            line.erase(0, LAB_IN_END.length());
            startLabel = false;
        }
        else continue;

        // Decode address, write it into the object

        auto eqPos = line.rfind('=');

        auto address         = strtol(line.substr(eqPos + 3).c_str(), nullptr , 16);
        std::string refLabel = line.substr(0, eqPos - 1);

        for (auto &sourceFile : GLOBAL_sourceFiles)
        {
            if (refLabel.compare(sourceFile.label) != 0) continue;
            if (startLabel)
            {
                sourceFile.testAddrStart = address;
            }
            else
            {
                sourceFile.testAddrEnd = address;
            }
        }
    }

    symFile.close();

    // Calculate size of each and every routine

    for (auto &sourceFile : GLOBAL_sourceFiles)
    {
        if (sourceFile.testAddrStart <= 0 || sourceFile.testAddrEnd <= 0 ||
            sourceFile.testAddrStart > sourceFile.testAddrEnd)
        {
            ERROR(std::string("unable to determine code length in '") + sourceFile.fileName + "'");
        }

        sourceFile.codeLength = sourceFile.testAddrEnd - sourceFile.testAddrStart;
        GLOBAL_totalRoutinesSize += sourceFile.codeLength;
    }

    // Check whether total code size is sane

    if (0 == GLOBAL_totalRoutinesSize) ERROR("total code size is 0");
    if (signed(GLOBAL_totalRoutinesSize) > CMD_hiAddress - CMD_loAddress)
    {
        ERROR(std::string("total code size is ") + std::to_string(GLOBAL_totalRoutinesSize) + ", too much for this segment!");
    }

    // Sort the file list by code size, starting from the smallest

    auto compare = [](const SourceFile &a, const SourceFile &b) -> bool { return a.codeLength < b.codeLength; };
    GLOBAL_sourceFiles.sort(compare);

    // Move zero-sized element to a separate list

    while (GLOBAL_sourceFiles.front().codeLength == 0)
    {
        GLOBAL_sourceFiles_noCode.push_back(GLOBAL_sourceFiles.front());
        GLOBAL_sourceFiles.pop_front();
    }
}

void prepareBinningProblem()
{
    // Prepare the log file

    const std::string logFileNamePath = CMD_outDir + DIR_SEPARATOR + CMD_segName + "_binproblem.log";
    unlink(logFileNamePath.c_str());
    std::ofstream logFile(logFileNamePath, std::fstream::out | std::fstream::trunc);
    DualStream logOutput(logFile, std::cout);

    // Print out code length information

    for (const auto &sourceFile : GLOBAL_sourceFiles)
    {
        std::string spacing;
        spacing.resize(GLOBAL_maxFileNameLen + 4 - sourceFile.fileName.length(), ' ');
        std::string floating_high;

        if (sourceFile.high)
        {
            floating_high = "(floating, high)  ";
        }
        else if (sourceFile.floating)
        {
            floating_high = "(floating)        ";
        }
        else
        {
            floating_high = "                  ";
        }

        logOutput << "file:    " << floating_high << sourceFile.fileName << spacing << "size: " << std::to_string(sourceFile.codeLength) << "\n";
    }

    // Create the binning problem

    GLOBAL_binningProblem = BinningProblem(CMD_loAddress, CMD_hiAddress);

    for (auto &sourceFile : GLOBAL_sourceFiles)
    {
        GLOBAL_binningProblem.addToProblem(&sourceFile);
    }

    // Print out some more statistics

    logOutput << "\n" <<
                 "free space (after floating routines are placed):    " <<
                 std::to_string(CMD_hiAddress - CMD_loAddress + 1 - GLOBAL_totalRoutinesSize) << "\n" <<
                 "number of floating routines:                        " <<
                 std::to_string(GLOBAL_binningProblem.floatingRoutines.size()) << "\n" <<
                 "number of gaps for the floating routines:           " <<
                 std::to_string(GLOBAL_binningProblem.gaps.size()) << "\n" <<
                 "\n";

    // Print out the available gaps

    for (auto& gap : GLOBAL_binningProblem.gaps)
    {
        logOutput << "gap address: $" << std::uppercase << std::hex << gap.first <<
                     "    size: " << std::to_string(gap.second) << "\n";
    }

    logOutput << "\n";

    // Close the log file

    if (!logFile.good()) ERROR(std::string("error writing log file '") + logFileNamePath + "'");
    logFile.close();
}

void solveBinningProblem()
{
    std::cout << "trying to solve the routine binning problem" << "\n\n";

    // Do some routine actions on the binning problem object

    Solver solver(GLOBAL_binningProblem);
    solver.run();

    if (!GLOBAL_binningProblem.isSolved())
    {
        ERROR("unable to solve the routine binning problem");
    }

    std::cout << "\n";
}

void compileSegment()
{
    // First combine everything into one assembler file

    const std::string outFileNameBare = CMD_segName + "_combined.s";
    const std::string vlfFileNamePath = CMD_segName + "_combined.vs";
    const std::string repFileNamePath = CMD_segName + "_combined.rep";
    const std::string symFileNamePath = CMD_segName + "_combined.sym";
    const std::string filePath        = CMD_outDir + DIR_SEPARATOR;
    const std::string outFileNamePath = filePath + outFileNameBare;
    unlink(outFileNamePath.c_str());
    unlink(vlfFileNamePath.c_str());
    unlink(repFileNamePath.c_str());
    unlink(symFileNamePath.c_str());
    std::ofstream outFile(outFileNamePath, std::fstream::out | std::fstream::trunc);
    if (!outFile.good()) ERROR(std::string("can't open temporary file '") + outFileNamePath + "'");

    // Write the header

    outFile << "!set SEGMENT_" << CMD_segName << " = 1\n";
    outFile << "!set ROM_LAYOUT_" << CMD_romLayout << " = 1\n\n";
    // XXX outFile << "\t* = $" << std::hex << CMD_loAddress << "\n";
    // XXX outFile << "\t!fill $" << std::hex << (CMD_hiAddress + 1 - CMD_loAddress) << "\n";
    // XXX outFile << "\t* = $" << std::hex << CMD_loAddress << "\n\n";

    // Write files which only contain definitions (no routines)

    for (const auto &sourceFile : GLOBAL_sourceFiles_noCode)
    {
        outFile << "\n\n\n\n";
        outFile << ";--- Source file " << sourceFile.fileName << "\n\n";
        outFile << "!zone " << toLabel(sourceFile.fileName) << "\n\n";
        outFile << std::string(sourceFile.content.begin(), sourceFile.content.end());
        outFile << "\n";
    }

    // Write remaining files, these should be placed under their proper locations

    for (const auto &routine : GLOBAL_binningProblem.fixedRoutines)
    {
        outFile << "\n\n\n\n";
        outFile << ";--- Source file " << routine.second->fileName << "\n\n";
        outFile << "!zone " << toLabel(routine.second->fileName) << "\n\n";
        outFile << "\t* = $" << std::hex << routine.first << "\n\n";
        outFile << std::string(routine.second->content.begin(), routine.second->content.end());
        outFile << "\n";
    }

    outFile << "\n\n";

    if (!outFile.good()) ERROR(std::string("error writing temporary file '") + outFileNamePath + "'");
    outFile.close();

    // All written - now launch the assembler

    const std::string cmd = "cd " + filePath + " && " + CMD_assembler +
                            " --strict-segments --color" +
                            " --outfile "     + CMD_outFile +
                            " --symbollist "  + symFileNamePath +
                            " --vicelabels "  + vlfFileNamePath +
                            " --report "      + repFileNamePath +
                            " " + outFileNameBare;
    std::cout << "command: " << cmd << "\n" << std::flush;
    if (0 != system(cmd.c_str()))
    {
        ERROR("assembler running failed");
    }
}

//
// Main function
//

int main(int argc, char **argv)
{
    parseCommandLine(argc, argv);

    printBannerCollectAnalyse();

    readSourceFiles();
    checkInputFileLabels();
    calcRoutineSizes();

    printBannerBinCompile();

    prepareBinningProblem();
    solveBinningProblem();
    compileSegment();

    return 0;
}

//
// Class 'SourceFile'
//

SourceFile::SourceFile(const std::string &fileName, const std::string &dirName) :
    fileName(fileName),
    dirName(dirName),
    ignore(false),
    floating(false),
    high(false),
    startAddr(-1),
    codeLength(-1),
    testAddrStart(-1),
    testAddrEnd(-1)
{
    const std::string fileNameWithPath = dirName + DIR_SEPARATOR + fileName;
    std::cout << "reading file: " << fileNameWithPath << "\n";

    // Open the file

    std::ifstream inFile;
    inFile.open(fileNameWithPath);
    if (!inFile.good()) ERROR("unable to open file");

    // Read the content

    inFile.seekg(0, inFile.end);
    auto fileLength = inFile.tellg();
    if (fileLength == 0) ERROR("file is empty");
    content.resize(fileLength);

    inFile.seekg(0);
    inFile.read(&content[0], fileLength);
    if (!inFile.good()) ERROR("error reading file content");
    if (content.back() != '\n') content.push_back('\n');

    inFile.close();

    // Determine if the file content is floating or fixed position one,
    // retrieve start address

    auto isHexDigit = [](char digit) -> bool
    {
        return (digit >= '0' && digit <= '9') ||
               (digit >= 'a' && digit <= 'f') ||
               (digit >= 'A' && digit <= 'F');;
    };

    floating = !(fileName.length() >= 6 && fileName[4] == '.' &&
                 isHexDigit(fileName[0]) &&
                 isHexDigit(fileName[1]) &&
                 isHexDigit(fileName[2]) &&
                 isHexDigit(fileName[3]));

    if (!floating)
    {
        startAddr = strtol(fileName.substr(0, 4).c_str(), nullptr ,16);
    }

    // Preprocess the file (apply settings from the content)

    preprocess();

    // Generate assembler compatible label from the file name

    label = toLabel(fileName);
}

void SourceFile::preprocess()
{
    std::string contentStr(content.begin(), content.end());
    std::istringstream stream(contentStr);
    std::string line;

    while (std::getline(stream, line))
    {
        if (line.empty()) continue;
        std::replace(line.begin(), line.end(), '\t', ' ');
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
        if (!preprocessLine(line)) break;
    }
}

bool SourceFile::preprocessLine(const std::string &line)
{
    // Split the line into tokens

    std::list<std::string> tokens;

    std::istringstream stream(line);
    std::string token;
    while(std::getline(stream, token, ' '))
    {
        if (token.empty()) continue;
        tokens.push_back(token);
    }
    if (tokens.empty()) return true;

    // Now preprocess line using the token list

    auto iter = tokens.begin();

    // Injest the comment token

    if ((iter++)->compare(";;") != 0 || iter == tokens.end()) return true;

    // Check if supported directive

    if ((iter++)->compare("#LAYOUT#") != 0) return true;

    // Check if segment name and rom layout match

    if (iter == tokens.end()) ERROR("syntax error, missing rom layout in '#LAYOUT#'");
    if (!nameMatch(*(iter++), CMD_romLayout)) return true;

    if (iter == tokens.end()) ERROR("syntax error, missing segment name in '#LAYOUT#'");
    if (!nameMatch(*(iter++), CMD_segName)) return true;

    // Retrieve and apply action

    if (iter == tokens.end()) ERROR("syntax error, missing action in '#LAYOUT#'");

    if (iter->compare("#IGNORE") == 0)
    {
        ignore   = true;
    }
    else if (iter->compare("#TAKE-FLOAT") == 0)
    {
        floating = true;
    }
    else if (iter->compare("#TAKE-HIGH") == 0)
    {
        floating = true;
        high     = true;
    }
    else if (iter->compare("#TAKE-OFFSET") == 0)
    {
        if (++iter == tokens.end()) ERROR("syntax error, missing parameter for '#TAKE-OFFSET'");
        startAddr += strtol(iter->c_str(), nullptr, 16);
    }
    else if (iter->compare("#TAKE") != 0)
    {
        ERROR(std::string("syntax error, unsupported action '") + token + "' in '#LAYOUT#'");
    }

    // Tell the caller to finish processing the file

    return false;
}

bool SourceFile::nameMatch(const std::string &token, const std::string &name)
{
    return (token.compare("*") == 0) || (token.compare(name) == 0);
}

//
// Class 'BinningProblem'
//

BinningProblem::BinningProblem(int loAddress, int hiAddress)
{
    if (hiAddress < loAddress || hiAddress < 0 || loAddress < 0 || hiAddress > 0xFFFF || loAddress > 0xFFFF)
    {
        ERROR("invalid lo/hi address");
    }

    // Create one large gap (initial) to represent the assembly

    gaps[loAddress] = hiAddress - loAddress + 1;

    // Initial value of statistics

    statSize = gaps[loAddress];
    statFree = gaps[loAddress];
    statWasted = 0;
}

bool BinningProblem::isSolved() const
{
    return floatingRoutines.empty();
}

void BinningProblem::addToProblem(SourceFile *routine)
{
    if (routine->floating)
    {
        floatingRoutines.push_back(routine);
    }
    else
    {
        // For the fixed-address routines, we have to find the matching place

        bool gapFound = false;
        for (auto &gap: gaps)
        {
            if (gap.first > routine->startAddr || gap.first + gap.second - 1 < routine->startAddr)
            {
                continue; // not a suitable gap
            }

            // We have to put the routine into the current gap

            if (gap.first + gap.second < routine->startAddr + routine->codeLength)
            {
                ERROR(std::string("fixed address file '") + routine->fileName + "' won't fit in the available gap");
            }

            gapFound = true;

            // Put the routine into the gap, possibly removing it or splitting into two

            fixedRoutines[routine->startAddr] = routine;
            statFree -= routine->codeLength;

            // Calculate possible new gap after the routine

            int newGapSize  = (gap.first + gap.second) - (routine->startAddr + routine->codeLength);
            int newGapStart = (newGapSize <= 0) ? -1 : routine->startAddr + routine->codeLength;

            // Remove or shrink the current gap

            if (gap.first == routine->startAddr)
            {
                gaps.erase(gap.first);
            }
            else
            {
                gap.second = routine->startAddr - gap.first;
            }

            // Add a new gap

            if (newGapStart > 0) gaps[newGapStart] = newGapSize;

            break; // iterator is not valid enymore, we have to stop the llop
        }

        if (!gapFound)
        {
            ERROR(std::string("start address of fixed address file '") +
                  routine->fileName + "' (" + std::to_string(routine->startAddr) +
                  ") already occupied or out of range");
        }
    }
}

void BinningProblem::fillGap(DualStream &logOutput, int gapAddress, const std::list<SourceFile *> &routines)
{
    int offset = 0;
    std::string spacing;

    for (auto &routine : routines)
    {
        int targetAddr = gapAddress + offset;

        spacing.resize(GLOBAL_maxFileNameLen + 4 - routine->fileName.length(), ' ');
        logOutput << "    $" << std::hex << targetAddr << std::dec << ": " <<
                     routine->fileName << spacing << "size: " << routine->codeLength << "\n";

        fixedRoutines[targetAddr] = routine;
        offset   += routine->codeLength;
        statFree -= routine->codeLength;

        if (offset > gaps[gapAddress]) ERROR(std::string("internal error line ") + std::to_string(__LINE__));

        floatingRoutines.erase(std::remove(floatingRoutines.begin(), floatingRoutines.end(), routine), floatingRoutines.end());
    }

    // Get rid of the gap, it's useless now

    if (offset == gaps[gapAddress])
    {
        logOutput << "filled to the last byte" << "\n";
    }
    else if (!isSolved())
    {
        logOutput << "filled in - dropped bytes: " << gaps[gapAddress] - offset << "\n";
        statWasted += gaps[gapAddress] - offset;
    }
    else
    {
        logOutput << "out of routines" << "\n";
    }

    gaps.erase(gapAddress);
}

void BinningProblem::placeHighRoutines(DualStream &logOutput)
{
    // Place routines which has to be stored in the high ROM area
    // It is expected there will be very few of them, so no complicated algorithms here

    std::string spacing;

    while (!floatingRoutines.empty())
    {
        // Find the largest high routine

        auto iterRoutine = floatingRoutines.end();

        for (auto iter = floatingRoutines.begin(); iter < floatingRoutines.end(); iter++)
        {
            if (!(*iter)->high) continue;

            if (iterRoutine == floatingRoutines.end() ||
                (*iterRoutine)->codeLength < (*iter)->codeLength)
            {
                iterRoutine = iter;
                continue;
            }

            if ((*iterRoutine)->codeLength < (*iter)->codeLength)
            {
                iterRoutine = iter;   
            }
        }

        if (iterRoutine == floatingRoutines.end()) break;

        // Now we need to find a space to put it - just take the largest gap available,
        // which is located in the high ROM area

        int routineSize = (*iterRoutine)->codeLength;
        int gapAddress  = -1;

        for (auto &gap : gaps)
        {
            if (gap.second < routineSize || gap.first < 0xE000) continue;

            if (gap.second == routineSize)
            {
                gapAddress = gap.first;
                break;
            }

            if (gapAddress < 0 || gap.second > gaps[gapAddress])
            {
                gapAddress = gap.first;
            }
        }

        if (gapAddress < 0) ERROR("no suitable space for a high routine");

        // Place routine in the gap found

        gaps[gapAddress] -= routineSize;

        int targetAddr = gapAddress + gaps[gapAddress];
        fixedRoutines[targetAddr] = *iterRoutine;
        statFree   -= routineSize;

        logOutput << "reducing gap $" << std::hex << gapAddress << std::dec <<
                    " to size " << gaps[gapAddress] << "\n";

        spacing.resize(GLOBAL_maxFileNameLen + 4 - (*iterRoutine)->fileName.length(), ' ');
        logOutput << "    $" << std::hex << targetAddr << std::dec << ": " <<
                     (*iterRoutine)->fileName << spacing << "size: " <<
                     (*iterRoutine)->codeLength << "\n";

        floatingRoutines.erase(iterRoutine);
        if (gaps[gapAddress] == 0) gaps.erase(gapAddress);
    }
}

void BinningProblem::performObviousSteps(DualStream &logOutput)
{
    // Get the size of the biggest routine; if there is just one gap which
    // can handle it - put the routine exactly there

    std::string spacing;

    bool repeat  = true;
    bool lastGap = false;
    while (repeat && !floatingRoutines.empty() && !gaps.empty())
    {
        repeat = false;

        int routineSize = floatingRoutines.back()->codeLength;

        int gapAddress;
        int matchingGaps = 0;
        for (auto &gap : gaps)
        {
            if (gap.second >= routineSize)
            {
                matchingGaps++;
                gapAddress = gap.first;
            }
        }

        if (!lastGap && gaps.size() == 1)
        {
            lastGap = true;
            logOutput << "selected gap: $" << std::hex << gapAddress << std::dec <<
                         " (size: " << gaps[gapAddress] << ") - the last remaining" << "\n";
        }

        if (matchingGaps == 1)
        {
            // Routine can only be placed in this particular gap - do so

            gaps[gapAddress] -= routineSize;
            int targetAddr = gapAddress + gaps[gapAddress];
            fixedRoutines[targetAddr] = floatingRoutines.back();
            statFree   -= routineSize;

            if (gaps.size() > 1)
            {
                logOutput << "forced reducing gap $" << std::hex << gapAddress << std::dec <<
                             " to size " << gaps[gapAddress] << "\n";
            }

            spacing.resize(GLOBAL_maxFileNameLen + 4 - floatingRoutines.back()->fileName.length(), ' ');
            logOutput << "    $" << std::hex << targetAddr << std::dec << ": " <<
                         floatingRoutines.back()->fileName << spacing << "size: " <<
                         floatingRoutines.back()->codeLength << "\n";

            floatingRoutines.pop_back();
            if (gaps[gapAddress] == 0) gaps.erase(gapAddress);

            repeat = true;
        }
    }
}

void BinningProblem::removeUselessGaps(DualStream &logOutput)
{
    // Get the size of the smallest floating routine,
    // remove all the gaps which are smaller in size

    int minUsefulSize = floatingRoutines[0]->codeLength;

    bool repeat = true;
    while (repeat)
    {
        repeat = false;
        for (auto &gap : gaps)
        {
            if (gap.second < minUsefulSize)
            {
                logOutput << "dropping gap: $" << std::hex << gap.first << std::dec << " (size: " << gap.second << ")" << "\n";
                statWasted += gap.second;
                gaps.erase(gap.first);
                repeat = true;
                break;
            }
        }
    }
}

void BinningProblem::sortFloatingRoutinesBySize()
{
    // Sort the unallocated routines by size, starting from the smallest one

    auto compare = [](const SourceFile *a, const SourceFile *b) -> bool { return a->codeLength < b->codeLength; };
    std::sort (floatingRoutines.begin(), floatingRoutines.end(), compare);
}

//
// Class 'Solver'
//

void Solver::run()
{
    // Prepare the log file

    const std::string logFileNamePath = CMD_outDir + DIR_SEPARATOR + CMD_segName + "_binsolution.log";
    unlink(logFileNamePath.c_str());
    logFile.open(logFileNamePath, std::fstream::out | std::fstream::trunc);

    // Sort the floating routines, just to be extra sure

    problem.sortFloatingRoutinesBySize();

    // Place routines which should be stored in high-ROM

    problem.placeHighRoutines(logOutput);

    // Run the solver until all is done

    while (!problem.gaps.empty() && !problem.floatingRoutines.empty())
    {
        problem.performObviousSteps(logOutput);
        problem.removeUselessGaps(logOutput);

        if (problem.gaps.empty() || problem.floatingRoutines.empty()) break;

        int gapAddr = selectGapToFill();

        logOutput << "selected gap: $" << std::hex << gapAddr << std::dec << " (size: " << problem.gaps[gapAddr] << ")" << "\n";

        std::list<SourceFile *> partialSolution;
        findPartialSolution(problem.gaps[gapAddr], partialSolution);
        problem.fillGap(logOutput, gapAddr, partialSolution);
    }

    // Print out the result

    if (problem.isSolved())
    {
        logOutput << "\n" << "all the routines sucessfully placed" << "\n\n";

        logOutput << "segment statistics:" << "\n";
        // logOutput << "    - total size:   " << problem.statSize << "\n"; - for BASIC contains filling gap too
        logOutput << "    - wasted bytes: " << problem.statWasted << "\n";
        logOutput << "    - still free:   " << problem.statFree - problem.statWasted << "\n\n";
    }

    // Close the log file

    if (!logFile.good()) ERROR(std::string("error writing log file '") + logFileNamePath + "'");
    logFile.close();
}

int Solver::selectGapToFill()
{
    // Find the smallest gap to fill-in

    int gapAddr = -1;
    int gapSize = -1;

    for (auto &gap : problem.gaps)
    {
        if (gapSize < 0 || gap.second < gapSize)
        {
            gapAddr = gap.first;
            gapSize = gap.second;
        }
    }

    return gapAddr;
}

int KS(const std::vector<SourceFile *> &routines,
    std::vector<std::vector<int>> &cacheV,             // value cache
    std::vector<std::vector<std::list<bool>>> &cacheS, // partial solution cache
    int n, int C,
    std::list<bool> &solution) // sequence of decisions for each routine, has to be empty when calling
{
    // This routine solves a knapsack problem using a dynamic programming.
    //
    // In short: 'routines' contains a list of routines that can potentially be placed in the gap of size C.
    // We start from index n (the highest one), referencing the last routine from the list (the largest one,
    // as the list is sorted). Our goal: by recursively searching the tree of all possible solutions
    // (solution = a sequence of decisions, each telling whether to take the routine or not) find the one
    // that fills the gap in the most complete way. Additional cache greatly speeds up the algorithm.
    // See the comments below to fully understand how it works.

    // XXX Possible efficiency improvement for the future: try to consider more than one gap at once,
    //     this will need a serious rework of the dynamic cache handling and will probably hurt the performance

    if (n == 0)
    {
        // No more routines to take the decision...
        return 0;
    }

    if (C == 0)
    {
        // We have no capacity left - decisions for all the routines left should be not to take them
        solution.resize(n, false);
        return 0;
    }

    auto &cachedV = cacheV[n - 1][C - 1];
    auto &cachedS = cacheS[n - 1][C - 1];
    if (cachedV >= 0)
    {
        // We already hold the solution in our cache - return it
        solution = cachedS;
        return cachedV;
    }

    auto &codeSize = routines[n - 1]->codeLength;
    if (codeSize > C)
    {
        // We can't take this routine, it's too large

        cachedV = KS(routines, cacheV, cacheS, n - 1, C, solution);
        solution.push_back(false);
        cachedS = solution;

        return cachedV;
    }

    // Note: it it seems equally good to take this routine, or not - take it!
    // Evaluation starts from the largest routines, and we should prefer to leave
    // a larger number of smaller routines - as this gives more possibilities
    // while optimizing the usage of further gaps

    std::list<bool> solution1;
    int val1 = KS(routines, cacheV, cacheS, n - 1, C - codeSize, solution1) + codeSize; // take

    if (val1 == C)
    {
        // By taking this routine we are filling all the space - contrary to the classic
        // knapsack problem, this is an optimal solution for us, don't waste time calculating
        // the others

        cachedV = val1;
        solution = solution1;
        solution.push_back(true);
        cachedS = solution;
        return cachedV;
    }

    std::list<bool> solution2;
    int val2 = KS(routines, cacheV, cacheS, n - 1, C, solution2); // don't take

    if (val1 >= val2)
    {
        // It's better to take this particular routine

        cachedV = val1;
        solution = solution1;
        solution.push_back(true);
    }
    else
    {
        // It's better not to take this particular routine

        cachedV = val2;
        solution = solution2;
        solution.push_back(false);
    }

    cachedS = solution;
    return cachedV;
};

void Solver::findPartialSolution(int gapSize, std::list<SourceFile *> &partialSolution)
{
    // First filter out available routines, take only the ones not larger than our gap

    std::vector<SourceFile *> routines;
    routines.clear();
    for (auto &routine : problem.floatingRoutines)
    {
        if (routine->codeLength <= gapSize)
        {
            routines.push_back(routine);
        }
        else break; // 'floatingRoutines' should be kept sorted
    }

    // Prepare value cache

    std::vector<std::vector<int>> cacheV;
    cacheV.resize(routines.size());
    for (auto &cacheRow : cacheV) cacheRow.resize(gapSize, -1);

    // Prepare partial solution cache
    std::vector<std::vector<std::list<bool>>> cacheS;
    cacheS.resize(routines.size());
    for (auto &cacheRow : cacheS) cacheRow.resize(gapSize);

    // Calculate solution

    std::list<bool> solution;
    KS(routines, cacheV, cacheS, routines.size(), gapSize, solution);

    if (routines.size() != solution.size()) ERROR(std::string("internal error line ") + std::to_string(__LINE__));

    // Return the solution upstream

    while (!routines.empty())
    {
        if (solution.back() != false)
        {
            partialSolution.push_front(routines.back());
        }

        routines.pop_back();
        solution.pop_back();
    }

    return;
}
