/**
 *  symbol table declaration and functions
 */

#include "structs.h"
#include "headers.h"
#include "consts.h"

/* symbol table head pointer */
static symbol *symbol_table_head = NULL;

/**
 * add new symbol to table
 * @param symbol_id - symbol name
 * @param value - tmp address
 * @param is_external - is symbol is external symbol
 * @param is_code - is symbol is code symbol
 * @param is_entry - is symbol is entry symbol
 * @param is_data - is symbol is data symbol
 * @param file_status - file status object
 * @return generated symbol
 */
symbol *add_symbol_to_symbol_table(char *symbol_id, int value, bool is_external , bool is_code, bool is_entry , bool is_data,
                                   status *file_status){
    symbol *new = NULL; /* new symbol pointer */
    char err_msg[MAX_LINE_LENGTH]; /* error message */

    if((new = get_symbol_by_id(symbol_id))){ /* look if sybol already exist*/
        if (is_external && new->is_external) { /* if so and external, skip!*/
            return new;
        }
        sprintf(err_msg, "cannot add new symbol %s, symbol already exist", symbol_id);
        log_error_wrapper(err_msg,file_status,NULL);
        return NULL; /* else return already exist !*/
    }

    new = malloc(sizeof(symbol)); /*allocate new symbol in memory */
    if (new == NULL){
        log_panic_wrapper("could not allocate memory",file_status);
    }
    strcpy(new->symbol_id,symbol_id); /* put symbo; content */

    new->value = value;

    new->is_external = is_external;
    new->is_code = is_code;
    new->is_entry = is_entry;
    new->is_data = is_data;
    new->next = NULL;

    if(!symbol_table_head){ /* and add to table */
        symbol_table_head = new;
    } else {
        symbol * ptr = symbol_table_head;
        while (ptr->next){
            ptr = ptr->next;
        }
        ptr->next = new;
    }
    return new;

}

/**
 * get symbol from table by id
 * @param symbol_id - symbok name
 * @return symbol
 */
symbol *get_symbol_by_id(char *symbol_id){
    symbol * ptr = symbol_table_head;
    while (ptr){
        if (!strcmp(ptr->symbol_id,symbol_id)){
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

/**
 * update symbol table with final IC
 * @param IC - final IC
 */
void add_ic_to_data_symbols( int IC) {
    symbol * it = symbol_table_head;
    while (it != NULL) {
        if (it->is_data) {
            it->value = it->value + IC;
        }
        it = it->next;
    }
}

/**
 * free symbol table
 */
void free_symbol_table() {
    symbol *it = symbol_table_head, *current_symbol;
    while (it != NULL) {
        current_symbol = it;
        it = it->next;
        free(current_symbol);
    }
    symbol_table_head = NULL;
}