/**
 * project structs
 */
#ifndef C_ASSEMBLER_STRUCTS_H
#define C_ASSEMBLER_STRUCTS_H
#include "consts.h"

/**
 * boolean enum
 * (for readability)
 */
typedef enum { false, true } bool; /* boolean */

/**
 * file status
 * contains the name of the file, current line processed
 * and errors flag
 */
typedef struct status { /* file status */
    char *file_name;
    int line_number;
    bool errors_flag;
} status;

/**
 * macro declaration
 */
typedef struct macro {
    char * macro_id; /* macro id */
    char * macro_code; /* macro content */

    /* pointer to the next macro */
    struct macro * next;
} macro;

/**
 * machine directive (op code func and etc..)
 */
typedef struct machine_directive {
    char * name; /* op code name */
    int opcode; /* op code */
    int funct; /* funct */
    int amount_of_operands; /*amount of needed operands*/
} machine_directive;

/**
 * symbol declaration
 * the symbol contains the name of the symbol, its value
 * and the attributes as flags
 * also, it contains pointer to the next symbol on the list
 */
typedef struct symbol {
    char symbol_id[SYMBOL_MAX_LENGTH];
    int value;

    /* attributes flags */
    bool is_external;
    bool is_code;
    bool is_entry;
    bool is_data;

    /* pointer to the next symbol */
    struct symbol * next;
} symbol;

/**
 * data declaration
 * data contains the data dc (then updated to final address)
 * the data value and pointer to the next data object in the list
 */
typedef struct data {
    int dc;
    int value: 16;
    struct data *next;
} data;

/**
 * addressing mode enum
 */
typedef enum addressing_mode {
    NONE,
    IMMEDIATE,
    DIRECT,
    INDEX,
    REGISTER_DIRECT
} addressing_mode;

/**
 * machine directive line contains the md line parsed for
 * easier usage
 */
typedef struct machine_directive_line {
    machine_directive *md;
    char *source_operand;
    char *destination_operand;
} machine_directive_line;

/**
 * op code word
 */
typedef struct op_code_word {
    unsigned int op_code: 16;
} op_code_word;

/**
 * second word of line with operand
 * holds the addressing mode of the operands and registries
 * (if provided)
 */
typedef struct funct_details_word {
    unsigned int funct : 4;
    unsigned int source_registry :4;
    unsigned int source_addressing_mode :2;
    unsigned int destination_registry :4;
    unsigned int destination_addressing_mode :2;
} funct_details_word;

typedef struct data_word {
    unsigned int are : 3;
    signed int data: 16;
    char *symbol_need_to_be_filled;
    bool is_base;
    bool is_offset;
} data_word;

typedef struct word {
    int address;
    op_code_word *ocw;
    funct_details_word *fdw;
    data_word *dlw;
    struct word *next;
} word;

typedef enum data_word_type {
    BASE = 0,
    OFFSET = 1
} data_word_type;

typedef struct entry {
    char symbol_id[SYMBOL_MAX_LENGTH];
    bool checked;
    unsigned int base;
    unsigned int offset;
    struct entry *next;
} entry;

typedef struct external {
    char symbol_name[SYMBOL_MAX_LENGTH];
    unsigned int base;
    unsigned int offset;
    struct external * next;
} external;



#endif /*C_ASSEMBLER_STRUCTS_H */
