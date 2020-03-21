//
// Utility to generate compressed strings and BASIC tokens
//

#include "common.h"

#include <unistd.h>

#include <algorithm>
#include <sstream>
#include <map>
#include <vector>

//
// Type definition for text/tokens to generate
//

typedef struct EntryString
{
	bool        enabledSTD;     // whether enabled for basic build 
	bool        enabledM65;     // whether enabled for Mega65 build
	bool        enabledX16;     // whether enabled for Commander X16 build
	std::string alias;          // alias, for the assembler
	std::string string;         // string/token
	uint8_t     abbrevLen = 0;  // length of token abbreviation
} EntryString;


// http://www.classic-games.com/commodore64/cbmtoken.html
// https://www.c64-wiki.com/wiki/BASIC_token

// BASIC keywords - V2 dialect

const std::vector<EntryString> GLOBAL_Keywords_V2 =
{
	{ true,  true,  true,  "IDX_KV2_END",        "END",        2 }, // $80
	{ true,  true,  true,  "IDX_KV2_FOR",        "FOR",        2 }, // $81
	{ true,  true,  true,  "IDX_KV2_NEXT",       "NEXT",       2 }, // $82
	{ true,  true,  true,  "IDX_KV2_DATA",       "DATA",       2 }, // $83
	{ true,  true,  true,  "IDX_KV2_INPUT_IO",   "INPUT#",     2 }, // $84
	{ true,  true,  true,  "IDX_KV2_INPUT",      "INPUT",      0 }, // $85
	{ true,  true,  true,  "IDX_KV2_DIM",        "DIM",        2 }, // $86
	{ true,  true,  true,  "IDX_KV2_READ",       "READ",       2 }, // $87
	{ true,  true,  true,  "IDX_KV2_LET",        "LET",        2 }, // $88
	{ true,  true,  true,  "IDX_KV2_GOTO",       "GOTO",       2 }, // $89
	{ true,  true,  true,  "IDX_KV2_RUN",        "RUN",        2 }, // $8A
	{ true,  true,  true,  "IDX_KV2_IF",         "IF",         0 }, // $8B
	{ true,  true,  true,  "IDX_KV2_RESTORE",    "RESTORE",    3 }, // $8C
	{ true,  true,  true,  "IDX_KV2_GOSUB",      "GOSUB",      3 }, // $8D
	{ true,  true,  true,  "IDX_KV2_RETURN",     "RETURN",     3 }, // $8E
	{ true,  true,  true,  "IDX_KV2_REM",        "REM",        0 }, // $8F
	{ true,  true,  true,  "IDX_KV2_STOP",       "STOP",       2 }, // $90
	{ true,  true,  true,  "IDX_KV2_ON",         "ON",         0 }, // $91
	{ true,  true,  true,  "IDX_KV2_WAIT",       "WAIT",       2 }, // $92
	{ true,  true,  true,  "IDX_KV2_LOAD",       "LOAD",       2 }, // $93
	{ true,  true,  true,  "IDX_KV2_SAVE",       "SAVE",       2 }, // $94
	{ true,  true,  true,  "IDX_KV2_VERIFY",     "VERIFY",     2 }, // $95
	{ true,  true,  true,  "IDX_KV2_DEF",        "DEF",        2 }, // $96
	{ true,  true,  true,  "IDX_KV2_POKE",       "POKE",       2 }, // $97
	{ true,  true,  true,  "IDX_KV2_PRINT_IO",   "PRINT#",     2 }, // $98
	{ true,  true,  true,  "IDX_KV2_PRINT",      "PRINT",      0 }, // $99
	{ true,  true,  true,  "IDX_KV2_CONT",       "CONT",       2 }, // $9A
	{ true,  true,  true,  "IDX_KV2_LIST",       "LIST",       2 }, // $9B
	{ true,  true,  true,  "IDX_KV2_CLR",        "CLR",        2 }, // $9C
	{ true,  true,  true,  "IDX_KV2_CMD",        "CMD",        2 }, // $9D
	{ true,  true,  true,  "IDX_KV2_SYS",        "SYS",        2 }, // $9E
	{ true,  true,  true,  "IDX_KV2_OPEN",       "OPEN",       2 }, // $9F
	{ true,  true,  true,  "IDX_KV2_CLOSE",      "CLOSE",      3 }, // $A0
	{ true,  true,  true,  "IDX_KV2_GET",        "GET",        2 }, // $A1
	{ true,  true,  true,  "IDX_KV2_NEW",        "NEW",        0 }, // $A2
	{ true,  true,  true,  "IDX_KV2_TAB",        "TAB(",       2 }, // $A3
	{ true,  true,  true,  "IDX_KV2_TO",         "TO",         0 }, // $A4
	{ true,  true,  true,  "IDX_KV2_FN",         "FN",         0 }, // $A5
	{ true,  true,  true,  "IDX_KV2_SPC",        "SPC(",       2 }, // $A6
	{ true,  true,  true,  "IDX_KV2_THEN",       "THEN",       2 }, // $A7
	{ true,  true,  true,  "IDX_KV2_NOT",        "NOT",        2 }, // $A8
	{ true,  true,  true,  "IDX_KV2_STEP",       "STEP",       2 }, // $A9
	{ true,  true,  true,  "IDX_KV2_OP_ADD",     "+",          0 }, // $AA
	{ true,  true,  true,  "IDX_KV2_OP_SUB",     "-",          0 }, // $AB
	{ true,  true,  true,  "IDX_KV2_OP_MUL",     "*",          0 }, // $AC
	{ true,  true,  true,  "IDX_KV2_OP_DIV",     "/",          0 }, // $AD
	{ true,  true,  true,  "IDX_KV2_OP_POW",     "^",          0 }, // $AE
	{ true,  true,  true,  "IDX_KV2_AND",        "AND",        2 }, // $AF
	{ true,  true,  true,  "IDX_KV2_OR",         "OR",         0 }, // $B0
	{ true,  true,  true,  "IDX_KV2_OP_GT",      ">",          0 }, // $B1
	{ true,  true,  true,  "IDX_KV2_OP_EQ",      "=",          0 }, // $B2
	{ true,  true,  true,  "IDX_KV2_OP_LT",      "<",          0 }, // $B3
	{ true,  true,  true,  "IDX_KV2_SGN",        "SGN",        2 }, // $B4
	{ true,  true,  true,  "IDX_KV2_INT",        "INT",        0 }, // $B5
	{ true,  true,  true,  "IDX_KV2_ABS",        "ABS",        2 }, // $B6
	{ true,  true,  true,  "IDX_KV2_USR",        "USR",        2 }, // $B7
	{ true,  true,  true,  "IDX_KV2_FRE",        "FRE",        2 }, // $B8
	{ true,  true,  true,  "IDX_KV2_POS",        "POS",        0 }, // $B9
	{ true,  true,  true,  "IDX_KV2_SQR",        "SQR",        2 }, // $BA
	{ true,  true,  true,  "IDX_KV2_RND",        "RND",        2 }, // $BB
	{ true,  true,  true,  "IDX_KV2_LOG",        "LOG",        0 }, // $BC
	{ true,  true,  true,  "IDX_KV2_EXP",        "EXP",        2 }, // $BD
	{ true,  true,  true,  "IDX_KV2_COS",        "COS",        0 }, // $BE
	{ true,  true,  true,  "IDX_KV2_SIN",        "SIN",        2 }, // $BF
	{ true,  true,  true,  "IDX_KV2_TAN",        "TAN",        0 }, // $C0
	{ true,  true,  true,  "IDX_KV2_ATN",        "ATN",        2 }, // $C1
	{ true,  true,  true,  "IDX_KV2_PEEK",       "PEEK",       2 }, // $C2
	{ true,  true,  true,  "IDX_KV2_LEN",        "LEN",        0 }, // $C3
	{ true,  true,  true,  "IDX_KV2_STR",        "STR$",       3 }, // $C4
	{ true,  true,  true,  "IDX_KV2_VAL",        "VAL",        2 }, // $C5
	{ true,  true,  true,  "IDX_KV2_ASC",        "ASC",        2 }, // $C6
	{ true,  true,  true,  "IDX_KV2_CHR",        "CHR$",       2 }, // $C7
	{ true,  true,  true,  "IDX_KV2_LEFT",       "LEFT$",      3 }, // $C8
	{ true,  true,  true,  "IDX_KV2_RIGHT",      "RIGHT$",     2 }, // $C9
	{ true,  true,  true,  "IDX_KV2_MID",        "MID$",       2 }, // $CA
	{ true,  true,  true,  "IDX_KV2_GO",         "GO",         0 }, // $CB
};

// BASIC keywords - V10 dialect, non-prefixed tokens

const std::vector<EntryString> GLOBAL_Keywords_V10 =
{
	{ false, false, false, "IDX_KX0_RGR",        "RGR",        2 }, // $CC
	{ false, false, false, "IDX_KX0_RCLR",       "RCLR",       2 }, // $CD
	{ false, false, false, "IDX_KX0_PREFIX_1",   "",           0 }, // $CE
	{ false, false, false, "IDX_KX0_JOY",        "JOY",        2 }, // $CF
	{ false, false, false, "IDX_KX0_RDOT",       "RDOT",       2 }, // $D0
	{ false, false, false, "IDX_KX0_DEC",        "DEC",        0 }, // $D1
	{ false, false, false, "IDX_KX0_HEX",        "HEX$",       2 }, // $D2
	{ false, false, false, "IDX_KX0_ERR",        "ERR$",       2 }, // $D3
	{ false, false, false, "IDX_KX0_INSTR",      "INSTR",      3 }, // $D4
	{ false, false, false, "IDX_KX0_ELSE",       "ELSE",       2 }, // $D5
	{ false, false, false, "IDX_KX0_RESUME",     "RESUME",     3 }, // $D6
	{ false, false, false, "IDX_KX0_TRAP",       "TRAP",       2 }, // $D7
	{ false, false, false, "IDX_KX0_TRON",       "TRON",       3 }, // $D8
	{ false, false, false, "IDX_KX0_TROFF",      "TROFF",      4 }, // $D9
	{ false, false, false, "IDX_KX0_SOUND",      "SOUND",      2 }, // $DA
	{ false, false, false, "IDX_KX0_VOL",        "VOL",        2 }, // $DB
	{ false, false, false, "IDX_KX0_AUTO",       "AUTO",       2 }, // $DC
	{ false, false, false, "IDX_KX0_PUDEF",      "PUDEF",      2 }, // $DD
	{ false, false, false, "IDX_KX0_GRAPHIC",    "GRAPHIC",    2 }, // $DE
	{ false, false, false, "IDX_KX0_PAINT",      "PAINT",      2 }, // $DF
	{ false, false, false, "IDX_KX0_CHAR",       "CHAR",       3 }, // $E0
	{ false, false, false, "IDX_KX0_BOX",        "BOX",        0 }, // $E1
	{ false, false, false, "IDX_KX0_CIRCLE",     "CIRCLE",     2 }, // $E2
	{ false, false, false, "IDX_KX0_PASTE",      "PASTE",      0 }, // $E3 XXX abbreviation?
	{ false, false, false, "IDX_KX0_CUT",        "CUT",        0 }, // $E4 XXX abbreviation?
	{ false, false, false, "IDX_KX0_LINE",       "LINE",       0 }, // $E5 XXX abbreviation?
	{ false, false, false, "IDX_KX0_LOCATE",     "LOCATE",     3 }, // $E6
	{ false, false, false, "IDX_KX0_COLOR",      "COLOR",      4 }, // $E7
	{ false, false, false, "IDX_KX0_SCNCLR",     "SCNCLR",     2 }, // $E8
	{ false, false, false, "IDX_KX0_SCALE",      "SCALE",      3 }, // $E9
	{ false, false, false, "IDX_KX0_HELP",       "HELP",       3 }, // $EA
	{ false, false, false, "IDX_KX0_DO",         "DO",         0 }, // $EB
	{ false, false, false, "IDX_KX0_LOOP",       "LOOP",       3 }, // $EC
	{ false, false, false, "IDX_KX0_EXIT",       "EXIT",       3 }, // $ED
	{ false, false, false, "IDX_KX0_DIR",        "DIR",        0 }, // $EE XXX abbreviation?
	{ false, false, false, "IDX_KX0_DSAVE",      "DSAVE",      2 }, // $EF
	{ false, false, false, "IDX_KX0_DLOAD",      "DLOAD",      2 }, // $F0
	{ false, false, false, "IDX_KX0_HEADER",     "HEADER",     3 }, // $F1
	{ false, false, false, "IDX_KX0_SCRATCH",    "SCRATCH",    3 }, // $F2
	{ false, false, false, "IDX_KX0_COLLECT",    "COLLECT",    5 }, // $F3
	{ false, false, false, "IDX_KX0_COPY",       "COPY",       3 }, // $F4
	{ false, false, false, "IDX_KX0_RENAME",     "RENAME",     3 }, // $F5
	{ false, false, false, "IDX_KX0_BACKUP",     "BACKUP",     3 }, // $F6
	{ false, false, false, "IDX_KX0_DELETE",     "DELETE",     3 }, // $F7
	{ false, false, false, "IDX_KX0_RENUMBER",   "RENUMBER",   4 }, // $F8
	{ false, false, false, "IDX_KX0_KEY",        "KEY",        2 }, // $F9
	{ false, false, false, "IDX_KX0_MONITOR",    "MONITOR",    3 }, // $FA
	{ false, false, false, "IDX_KX0_USING",      "USING",      0 }, // $FB XXX abbreviation?
	{ false, false, false, "IDX_KX0_UNTIL",      "UNTIL",      0 }, // $FC XXX abbreviation?
	{ false, false, false, "IDX_KX0_WHILE",      "WHILE",      3 }, // $FD
};

// BASIC keywords - V10 dialect, tokens prefixed with $CE

const std::vector<EntryString> GLOBAL_Keywords_V10_CE =
{
	{ false, false, false, "IDX_KX1_POT",        "POT",        2 }, // $CE $02
	{ false, false, false, "IDX_KX1_BUMP",       "BUMP",       2 }, // $CE $03
	{ false, false, false, "IDX_KX1_PEN",        "PEN",        2 }, // $CE $04
	{ false, false, false, "IDX_KX1_RSPOS",      "RSPPOS",     2 }, // $CE $05
	{ false, false, false, "IDX_KX1_RSPRITE",    "RSPRITE",    4 }, // $CE $06
	{ false, false, false, "IDX_KX1_RSPCOLOR",   "RSPCOLOR",   4 }, // $CE $07
	{ false, false, false, "IDX_KX1_XOR",        "XOR",        2 }, // $CE $08
	{ false, false, false, "IDX_KX1_RWINDOW",    "RWINDOW",    2 }, // $CE $09
	{ false, false, false, "IDX_KX1_POINTER",    "POINTER",    3 }, // $CE $0A
};

// BASIC keywords - V10 dialect, tokens prefixed with $FE (note: DMA has different code, to prevent conflict with V7)

const std::vector<EntryString> GLOBAL_Keywords_V10_FE =
{
	{ false, false, false, "IDX_KX2_BANK",       "BANK",       2 }, // $FE $02
	{ false, false, false, "IDX_KX2_FILTER",     "FILTER",     2 }, // $FE $03
	{ false, false, false, "IDX_KX2_PLAY",       "PLAY",       2 }, // $FE $04
	{ false, false, false, "IDX_KX2_TEMPO",      "TEMPO",      2 }, // $FE $05
	{ false, false, false, "IDX_KX2_MOVSPR",     "MOVSPR",     0 }, // $FE $06
	{ false, false, false, "IDX_KX2_SPRITE",     "SPRITE",     2 }, // $FE $07
	{ false, false, false, "IDX_KX2_SPRCOLOR",   "SPRCOLOR",   4 }, // $FE $08
	{ false, false, false, "IDX_KX2_RREG",       "RREG",       2 }, // $FE $09
	{ false, false, false, "IDX_KX2_ENVELOPE",   "ENVELOPE",   2 }, // $FE $0A
	{ false, false, false, "IDX_KX2_SLEEP",      "SLEEP",      2 }, // $FE $0B
	{ false, false, false, "IDX_KX2_CATALOG",    "CATALOG",    2 }, // $FE $0C
	{ false, false, false, "IDX_KX2_DOPEN",      "DOPEN",      2 }, // $FE $0D
	{ false, false, false, "IDX_KX2_APPEND",     "APPEND",     2 }, // $FE $0E
	{ false, false, false, "IDX_KX2_DCLOSE",     "DCLOSE",     2 }, // $FE $0F
	{ false, false, false, "IDX_KX2_BSAVE",      "BSAVE",      2 }, // $FE $10
	{ false, false, false, "IDX_KX2_BLOAD",      "BLOAD",      2 }, // $FE $11
	{ false, false, false, "IDX_KX2_RECORD",     "RECORD",     2 }, // $FE $12
	{ false, false, false, "IDX_KX2_CONCAT",     "CONCAT",     2 }, // $FE $13
	{ false, false, false, "IDX_KX2_DVERIFY",    "DVERIFY",    2 }, // $FE $14
	{ false, false, false, "IDX_KX2_DCLEAR",     "DCLEAR",     4 }, // $FE $15
	{ false, false, false, "IDX_KX2_SPRSAV",     "SPRSAV",     4 }, // $FE $16
	{ false, false, false, "IDX_KX2_COLLISION",  "COLLISION",  4 }, // $FE $17
	{ false, false, false, "IDX_KX2_BEGIN",      "BEGIN",      2 }, // $FE $18
	{ false, false, false, "IDX_KX2_BEND",       "BEND",       3 }, // $FE $19
	{ false, false, false, "IDX_KX2_WINDOW",     "WINDOW",     0 }, // $FE $1A XXX abbreviation?
	{ false, false, false, "IDX_KX2_BOOT",       "BOOT",       2 }, // $FE $1B
	{ false, false, false, "IDX_KX2_WIDTH",      "WIDTH",      2 }, // $FE $1C
	{ false, false, false, "IDX_KX2_SPRDEF",     "SPRDEF",     4 }, // $FE $1D
	{ false, false, false, "IDX_KX2_QUIT",       "QUIT",       0 }, // $FE $1E XXX abbreviation?
	{ false, false, false, "IDX_KX2_STASH",      "STASH",      0 }, // $FE $1F XXX abbreviation?
	{ false, false, false, "IDX_KX2_DMA",        "DMA",        0 }, // $FE $20 XXX abbreviation?
	{ false, false, false, "IDX_KX2_FETCH",      "FETCH",      0 }, // $FE $21 XXX abbreviation?
	{ false, false, false, "",                   "",           0 }, // $FE $22
	{ false, false, false, "IDX_KX2_SWAP",       "SWAP",       0 }, // $FE $23 XXX abbreviation?
	{ false, false, false, "IDX_KX2_OFF",        "OFF",        0 }, // $FE $24 XXX abbreviation?
	{ false, false, false, "IDX_KX2_FAST",       "FAST",       0 }, // $FE $25
	{ false, false, false, "IDX_KX2_SLOW",       "SLOW",       0 }, // $FE $26
	{ false, false, false, "IDX_KX2_TYPE",       "TYPE",       0 }, // $FE $27 XXX abbreviation?
	{ false, false, false, "IDX_KX2_BVERIFY",    "BVERIFY",    0 }, // $FE $28 XXX abbreviation?
	{ false, false, false, "IDX_KX2_DIRECTORY",  "DIRECTORY",  3 }, // $FE $29
	{ false, false, false, "IDX_KX2_ERASE",      "ERASE",      0 }, // $FE $2A XXX abbreviation?
	{ false, false, false, "IDX_KX2_FIND",       "FIND",       0 }, // $FE $2B XXX abbreviation?
	{ false, false, false, "IDX_KX2_CHANGE",     "CHANGE",     0 }, // $FE $2C XXX abbreviation?
	{ false, false, false, "IDX_KX2_SET",        "SET",        0 }, // $FE $2D XXX abbreviation?
	{ false, false, false, "IDX_KX2_SCREEN",     "SCREEN",     0 }, // $FE $2E XXX abbreviation?
	{ false, false, false, "IDX_KX2_POLYGON",    "POLYGON",    0 }, // $FE $2F XXX abbreviation?
	{ false, false, false, "IDX_KX2_ELLIPSE",    "ELLIPSE",    0 }, // $FE $30 XXX abbreviation?
	{ false, false, false, "IDX_KX2_VIEWPORT",   "VIEWPORT",   0 }, // $FE $31 XXX abbreviation?
	{ false, false, false, "IDX_KX2_GCOPY",      "GCOPY",      0 }, // $FE $32 XXX abbreviation?
	{ false, false, false, "IDX_KX2_PEN",        "PEN",        0 }, // $FE $33 XXX abbreviation?
	{ false, false, false, "IDX_KX2_PALETTE",    "PALETTE",    0 }, // $FE $34 XXX abbreviation?
	{ false, false, false, "IDX_KX2_DMODE",      "DMODE",      0 }, // $FE $35 XXX abbreviation?
	{ false, false, false, "IDX_KX2_DPAT",       "DPAT",       0 }, // $FE $36 XXX abbreviation?
	{ false, false, false, "IDX_KX2_PIC",        "PIC",        0 }, // $FE $37 XXX abbreviation?
	{ false, false, false, "IDX_KX2_GENLOCK",    "GENLOCK",    0 }, // $FE $38 XXX abbreviation?
	{ false, false, false, "IDX_KX2_FOREGROUND", "FOREGROUND", 0 }, // $FE $39 XXX abbreviation?
	{ false, false, false, "",                   "",           0 }, // $FE $3A
	{ false, false, false, "IDX_KX2_BACKGROUND", "BACKGROUND", 0 }, // $FE $3B XXX abbreviation?
	{ false, false, false, "IDX_KX2_BORDER",     "BORDER",     0 }, // $FE $3C XXX abbreviation?
	{ false, false, false, "IDX_KX2_HIGHLIGHT",  "HIGHLIGHT",  0 }, // $FE $3D XXX abbreviation?
};


// BASIC errors - V2 dialect

const std::vector<EntryString> GLOBAL_Errors_V2 =
{
	{ true,  true,  true,  "IDX_EV2_01", "TOO MANY FILES"        },
	{ true,  true,  true,  "IDX_EV2_02", "FILE OPEN"             },
	{ true,  true,  true,  "IDX_EV2_03", "FILE NOT OPEN"         },
	{ true,  true,  true,  "IDX_EV2_04", "FILE NOT FOUND"        },
	{ true,  true,  true,  "IDX_EV2_05", "DEVICE NOT PRESENT"    },
	{ true,  true,  true,  "IDX_EV2_06", "NOT INPUT FILE"        },
	{ true,  true,  true,  "IDX_EV2_07", "NOT OUTPUT FILE"       },
	{ true,  true,  true,  "IDX_EV2_08", "MISSING FILENAME"      },
	{ true,  true,  true,  "IDX_EV2_09", "ILLEGAL DEVICE NUMBER" },
	{ true,  true,  true,  "IDX_EV2_0A", "NEXT WITHOUT FOR"      },
	{ true,  true,  true,  "IDX_EV2_0B", "SYNTAX"                },
	{ true,  true,  true,  "IDX_EV2_0C", "RETURN WITHOUT GOSUB"  },
	{ true,  true,  true,  "IDX_EV2_0D", "OUT OF DATA"           },
	{ true,  true,  true,  "IDX_EV2_0E", "ILLEGAL QUANTITY"      },
	{ true,  true,  true,  "IDX_EV2_0F", "OVERFLOW"              },
	{ true,  true,  true,  "IDX_EV2_10", "OUT OF MEMORY"         },
	{ true,  true,  true,  "IDX_EV2_11", "UNDEF\'D STATEMENT"    },
	{ true,  true,  true,  "IDX_EV2_12", "BAD SUBSCRIPT"         },
	{ true,  true,  true,  "IDX_EV2_13", "REDIM\'D ARRAY"        },
	{ true,  true,  true,  "IDX_EV2_14", "DIVISION BY ZERO"      },
	{ true,  true,  true,  "IDX_EV2_15", "ILLEGAL DIRECT"        },
	{ true,  true,  true,  "IDX_EV2_16", "TYPE MISMATCH"         },
	{ true,  true,  true,  "IDX_EV2_17", "STRING TOO LONG"       },
	{ true,  true,  true,  "IDX_EV2_18", "FILE DATA"             },
	{ true,  true,  true,  "IDX_EV2_19", "FORMULA TOO COMPLEX"   },
	{ true,  true,  true,  "IDX_EV2_1A", "CAN\'T CONTINUE"       },
	{ true,  true,  true,  "IDX_EV2_1B", "UNDEF\'D FUNCTION"     },
	{ true,  true,  true,  "IDX_EV2_1C", "VERIFY"                },
	{ true,  true,  true,  "IDX_EV2_1D", "LOAD"                  },
	{ true,  true,  true,  "IDX_EV2_1E", "BREAK"                 },
};
	
const std::vector<EntryString> GLOBAL_Errors_V10 =
{
	{ false, false, true,  "IDX_EX0_1F", "CAN'T RESUME"          },
	{ false, false, true,  "IDX_EX0_20", "LOOP NOT FOUND"        },
	{ false, false, true,  "IDX_EX0_21", "LOOP WITHOUT DO"       },
	{ false, false, true,  "IDX_EX0_22", "DIRECT MODE ONLY"      },
	{ false, false, true,  "IDX_EX0_23", "NO GRAPHICS AREA"      },
	{ false, false, true,  "IDX_EX0_24", "BAD DISK"              },
	{ false, false, true,  "IDX_EX0_25", "BEND NOT FOUND"        },
	{ false, false, true,  "IDX_EX0_26", "LINE NUMBER TOO LARGE" },
	{ false, false, true,  "IDX_EX0_27", "UNRESOLVED REFERENCE"  },
	{ false, false, true,  "IDX_EX0_28", "UNIMPLEMENTED"         },
	{ false, false, true,  "IDX_EX0_29", "FILE READ"             },
};

const std::vector<EntryString> GLOBAL_MiscStrings =
{
	{ true,  true,  true,  "IDX_STR_01", " BASIC BYTES FREE"     },
	{ true,  true,  true,  "IDX_STR_02", "READY.\r"              },
	{ true,  true,  true,  "IDX_STR_03", "ERROR"                 },
	{ true,  true,  true,  "IDX_STR_04", " IN "                  },
	{ true,  true,  true,  "IDX_STR_05", "BRK AT $"              },
	{ true,  true,  true,  "IDX_STR_06", "MEMORY CORRUPT"        },
};

//
// Work class definitions
//

class DataSet
{
public:

	void addTokens(const std::vector<EntryString> &stringList);
	void addStrings(const std::vector<EntryString> &stringList);

	const std::string &getOutput();

private:

	void process();

	void removeTrailingStrings();
	void buildDictionary();
	void calculateFrequencies();
	void encodeWordsTokens();
	void encodeStrings();
	void prepareOutput();

	virtual bool isRelevant(const EntryString &entry) const;
	
	std::vector<std::vector<EntryString>>          tokens;
	std::vector<EntryString>                       strings;
	
	std::vector<std::string>                       words;
	
	std::vector<char>                              asNibble;
	std::vector<char>                              asByte;

	std::vector<std::vector<std::vector<uint8_t>>> tokensEncoded;
	std::vector<std::vector<uint8_t>>              wordsEncoded;	
	std::vector<std::vector<uint8_t>>              stringsEncoded;
	
	std::string outputString;
	bool        outputStringValid = false;
};

class DataSetSTD : public DataSet
{
	bool isRelevant(const EntryString &entry) const { return entry.enabledSTD; }
};

class DataSetM65 : public DataSet
{
	bool isRelevant(const EntryString &entry) const { return entry.enabledM65; }
};

//
// Work class implementation
//

void DataSet::addTokens(const std::vector<EntryString> &stringList)
{
	tokens.push_back(stringList);
	
	// Clear tokens not relevant for the current configuration
	
	for (auto &entry : tokens.back())
	{
		if (isRelevant(entry)) continue;
		entry.string.clear();
	}
	
	outputStringValid = false;
}

void DataSet::addStrings(const std::vector<EntryString> &stringList)
{
	strings.insert(strings.end(), stringList.begin(), stringList.end());

	// Clear strings not relevant for the current configuration
	
	for (auto &entry : strings)
	{
		if (isRelevant(entry)) continue;
		entry.string.clear();
	}
	
	outputStringValid = false;
}

void DataSet::process()
{
	removeTrailingStrings();
	
	buildDictionary();
	calculateFrequencies();
	encodeWordsTokens();
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

void DataSet::removeTrailingStrings()
{
	// XXX
}

void DataSet::buildDictionary()
{
	words.clear();
	
	// Extract all the words from the list of strings

	for (const auto &current : strings)
	{
		std::istringstream stream(current.string);
		std::string word;
		while (std::getline(stream, word, ' '))
		{
			if (word.length() > 0) words.push_back(word);
		}
	}

	// Remove duplicates

	std::sort(words.begin(), words.end());
	words.erase(std::unique(words.begin(), words.end()), words.end());
}

void DataSet::calculateFrequencies()
{
	asNibble.clear();
	asByte.clear();
	
	std::map<char, uint16_t> freqMap;
	
	// Calculate frequencies - tokens

	for (const auto &tokenList : tokens)
	{
		for (const auto &token : tokenList)
		{
			for (const auto &character : token.string)
			{
				if (character >= 0x80)
				{
					// XXX error
				}
				
				freqMap[character]++;
			}
		}
	}
	
	// Calculate frequencies - words
	
	for (const auto &string : words)
	{
		for (const auto &character : string)
		{	
			if (character >= 0x80)
			{
				// XXX error
			}
			
			freqMap[character]++;
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

void DataSet::encodeWordsTokens()
{
	tokensEncoded.clear();
	wordsEncoded.clear();
	
	// XXX
}

void DataSet::encodeStrings()
{
	stringsEncoded.clear();

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

void printBanner()
{
    printBannerLineTop();
    std::cout << "// Generating compressed strings and BASIC tokens" << "\n";
    printBannerLineBottom();
}

//
// Main function
//

int main(int argc, char **argv)
{
    printBanner();

    return 0;
}