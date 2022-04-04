/**
 * common functions
 */

#include "headers.h"
#include "consts.h"
#include <ctype.h>

/**
 * static global registers array
 */
static char *registers[NO_OF_REGISTERS] = {
        "r0", "r1", "r2", "r3",
        "r4", "r5", "r6", "r7",
        "r8", "r9", "r10", "r11",
        "r12", "r13", "r14", "r15"
};

/**
 * static global machine directive (op codes) array
 */
static machine_directive machine_directives[NO_OF_MACHINE_DIRECTIVES] = {
        /* group 1- 2 operands machine directives */
        {"mov",  0,  0,  2},
        {"cmp",  1,  0,  2},
        {"add",  2,  10,  2},
        {"sub",  2,  11,  2},
        {"lea",  4,  0,  2},

        /* group 2- 1 operand machine directives */
        {"clr",  5,  10, 1},
        {"not",  5,  11, 1},
        {"inc",  5,  12, 1},
        {"dec",  5,  13, 1},
        {"jmp",  9,  10, 1},
        {"bne",  9,  11, 1},
        {"jsr",  9,  12, 1},
        {"red",  12, 0,  1},
        {"prn",  13, 0,  1},

        /* group 3- no operand machine directives */
        {"rts",  14, 0,  0},
        {"stop", 15, 0,  0},

};
/**
 * get machine directive by op code name
 * @param current_token -  op code
 * @return machine directive object
 */
machine_directive *get_machine_directive(char *current_token) {
    int i;
    for (i = 0; i < NO_OF_MACHINE_DIRECTIVES; i++) {
        if (!strcmp(current_token, machine_directives[i].name)) {
            return &machine_directives[i];
        }
    }
    return NULL;
}

/**
 * check if current token is a valid register and if so return register number
 * @param current_token - register name
 * @return register index in register array
 */
int is_register(char *current_token) {
    int i;
    for (i = 0; i < NO_OF_REGISTERS; i++) {
        if (!strcmp(current_token, registers[i])) {
            return i;
        }
    }
    return -1;
}
/**
 * check if current token is register or op code
 * @param current_token - current word
 * @return if is reserverd word
 */
bool is_reserved_word(char *current_token) {
    return (get_machine_directive(current_token) || (is_register(current_token) != -1));
}

/**
 * check if current word is a valid symbol name
 * @param current_token - current word
 * @param file_status - file status object
 * @param should_log - should log error flag
 * @param should_skip - should skip line flag
 * @return if its a valid symbol name
 */
bool is_valid_symbol_name(char *current_token, status *file_status,bool should_log, bool *should_skip){
    char *token_cpy;
    if (!isalpha(*current_token)) {
        if (should_log){
            log_error_wrapper("symbol must start with alphabetic letter", file_status,should_skip);
        }
        return false;
    }

    token_cpy = current_token;
    while ((*token_cpy) != '\0') {
        if (!isalnum(*token_cpy)) {
            if (should_log){
                log_error_wrapper("new_symbol must contain only alphanumeric characters", file_status,should_skip);
            }
            return false;
        }
        token_cpy++;
    }
    if (is_reserved_word(current_token)) {
        if (should_log) {
            log_error_wrapper("cannot use reserved word as a symbol name", file_status,should_skip);
        }
        return false;
    }
    return true;
}

/**
 * check if the current word is a valid new symbol declaration
 * @param current_token
 * @param file_status
 * @param should_skip
 * @param new_symbol
 * @return if its a valid new symbol name
 */
bool is_new_symbol(char *current_token, status *file_status, bool *should_skip, char *new_symbol) {
    int token_len;
    token_len = strlen(current_token);
    if (current_token[token_len - 1] != ':') {
        new_symbol = NULL;
        return false;
    }
    current_token[token_len - 1] = '\0';
    if (!is_valid_symbol_name(current_token,file_status,true,should_skip)){
        return false;
    }
    new_symbol = malloc(strlen(current_token));
    strcpy(new_symbol, current_token);
    return true;
}

/**
 * the function gets word value and address and write it to the file in address - hex format
 * @param file_to_write - file to write the line to
 * @param word_bits - the word value
 * @param address - the word address
 */
void write_num_to_file(FILE * file_to_write, unsigned long word_bits, int address){
    word_bits = ((word_bits) & 0xFFFFF);

    fprintf(file_to_write, "%04d A%hx-B%hx-C%hx-D%hx-E%hx\n" ,address,extract_4_bits_from_pos(word_bits,16),extract_4_bits_from_pos(word_bits,12),
            extract_4_bits_from_pos(word_bits,8),
            extract_4_bits_from_pos(word_bits,4),
            extract_4_bits_from_pos(word_bits,0));
}