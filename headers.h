/**
 * functions headers
 */
#ifndef C_ASSEMBLER_HEADERS_H
#define C_ASSEMBLER_HEADERS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "structs.h"


/* first pass function */
void first_pass(FILE *file_to_read,int *IC, int *DC, status *file_status);
bool insert_num(char *current_num, int i, status *file_status, int *DC);
bool data_directive_parsing(char *current_token, status *file_status, bool *should_skip, int *DC);
bool string_directive_parsing (char *current_token, status *file_status, bool *should_skip, int *DC);
bool is_directive(char *current_token, status *file_status, bool *should_skip, char *new_symbol, int *DC);
machine_directive_line *get_machine_directive_line(char *current_token, status *file_status, bool *should_skip);
bool is_index_addressing_mode(char *operand, char *symbol_name ,unsigned int *register_num, status *file_status, bool *should_skip);
addressing_mode analyse_addressing_mode(char *operand, char *index_symbol, unsigned int *register_num, int *immediate_num , status *file_status, bool *should_skip);
bool analyse_operands_structure_of_machine_directive(char *current_token, int *IC, status *file_status, bool *should_skip);
/* second pass functions */
void second_pass(status *file_status);

/* files utils functions */
char * get_full_file_name(char  *file_name, char *ext);
FILE * read_file_with_extension(char *file_name, char *ext, char *modes);
void create_ob_file(char *file_name,int IC, int DC);

/* pre assembler function */
void pre_assembler(FILE *file_to_read, char *file_name,status *file_status);

/* macro's linked list functions*/
macro *add_macro(char *macro_id);
macro *get_macro_by_id(char *macro_id);
void free_macro_list();

/* symbol table functions*/
symbol *add_symbol_to_symbol_table(char *symbol_id, int value, bool is_external , bool is_code, bool is_entry , bool is_data,
                                   status *file_status);
symbol *get_symbol_by_id(char *symbol_id);
void free_symbol_table();
void display_symbol_list();
void add_ic_to_data_symbols( int IC);

/* data linked list functions */
data *add_value_to_data_list(int ic , int value);
void print_data_to_file(FILE * file_to_write, int IC);
void free_data_list();

/* entries linked list function */
entry *get_entry_by_id(char *symbol_id);
entry *add_entry(char *symbol_id);
void free_entry_list();
entry *get_next_unchecked_entry();
entry *get_entries_list_head();
void print_entries_to_file(FILE * file_to_write);
void create_ent_file(char *file_name);

/* word linked list functions*/
word *new_word();
data_word *new_data_word(unsigned int *are , int *value , char *init_missing_symbol, data_word_type t);
funct_details_word *new_funct_details_word(int funct);
bool add_word_to_word_list(word *new);
word *get_next_missing_symbol_word();
void push_word(word *first_word, word *word_to_add);
void print_words_to_file(FILE * file_to_write);
void free_funct_details_word(funct_details_word *word);
void free_op_code_word( op_code_word *word);
void free_data_word( data_word *word);
void free_word_chain(word * head);
void free_word_list();

/* utils functions */
void log_error_wrapper(char *err, status *file_status,bool *should_skip);
void log_error_without_line_wrapper(char *err, status *file_status,bool *should_skip);
void log_panic_wrapper(char *err ,status *file_status);
void *safe_malloc(size_t n);
bool is_valid_line(char *line, FILE *fp, status *file_status);
char *concat(const char *s1, const char *s2);
unsigned int extract_4_bits_from_pos(long number, int p);
status *new_status(char *file_name, char *ext);
void free_status(status *current_status);
void free_linked_lists();

/*common*/
machine_directive *get_machine_directive(char *current_token);
int is_register(char *current_token);
bool is_reserved_word(char *current_token);
bool is_valid_symbol_name(char *current_token, status *file_status,bool should_log, bool *should_skip);
bool is_new_symbol(char *current_token, status *file_status, bool *should_skip, char *new_symbol);
void write_num_to_file(FILE * file_to_write, unsigned long word_bits, int address);

/* external linked list functions */
external *add_to_external_list(char *symbol_id , int base, int offset);
external *get_head_externals();
void print_externals_to_file(FILE * file_to_write);
void create_ext_file(char *file_name);
void free_externals_list();

#endif /*C_ASSEMBLER_HEADERS_H*/
