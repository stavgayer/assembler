/**
 * word list declaration and functions
 */
#include "headers.h"
#include "consts.h"
/* word list head pointer */
static word *word_list_head = NULL;

/**
 * create new empty word
 * @return new word
 */
word *new_word() {
    word *new_word = (word *) safe_malloc(sizeof(word ) );
    new_word->fdw = NULL;
    new_word->ocw = NULL;
    new_word->dlw = NULL;
    new_word->next = NULL;
    new_word->address = 0;
    return new_word;
}

/**
 * write word list to object file
 * @param file_to_write - object file
 */
void print_words_to_file(FILE * file_to_write) {
    word * it = word_list_head;
    while (it){ /* for each word */
        unsigned long word_bits = 0;
        /*create word by type */
        if(it->ocw){
            word_bits = (( (1 <<2) & 0xFF)<<16) | (it->ocw->op_code) ;
        } else if (it->fdw){
            word_bits = (( (1 <<2) & 0xFF)<<16) | (it->fdw->funct <<12) |
                    (it->fdw->source_registry << 8) | (it->fdw->source_addressing_mode << 6) |
                    (it->fdw->destination_registry << 2) | (it->fdw->destination_addressing_mode);

        } else if (it->dlw){
            word_bits = ( it->dlw->are<<16) | (unsigned short int)( it->dlw->data);
        }
        /* write word generated number to file */
        write_num_to_file(file_to_write,word_bits,it->address);
        it = it->next; /* move to next word*/
    }
}

/**
 * creates new data word with initialize values
 * @param are - are value of the data word
 * @param value - values of the data word
 * @param init_missing_symbol - missing symbol to complete later
 * @param t - is base or offset (only if missing symbol)
 * @return new data word
 */
data_word *new_data_word(unsigned int *are , int *value , char *init_missing_symbol, data_word_type t){
    data_word *new_data_word = (data_word *) safe_malloc(sizeof(data_word ) );
    if (init_missing_symbol != NULL) {
        new_data_word->symbol_need_to_be_filled = (char *) safe_malloc(sizeof(char ) * SYMBOL_MAX_LENGTH);
        strcpy(new_data_word->symbol_need_to_be_filled , init_missing_symbol);
        if (t == BASE) {
            new_data_word->is_base = true;
        }
        if (t == OFFSET) {
            new_data_word->is_offset = true;
        }
        new_data_word->data =0;
        new_data_word->are = 0;
    } else if (are != NULL && value != NULL) {
        new_data_word->are = *are;
        new_data_word->data = *value;
        new_data_word->symbol_need_to_be_filled = NULL;
    }else {
        return NULL;
    }
    return new_data_word;
}

/**
 * new funct details word
 * @param funct - funct value
 * @return  new funct details word
 */
funct_details_word *new_funct_details_word(int funct){
    funct_details_word *new = (funct_details_word *) safe_malloc( sizeof(funct_details_word));
    new->funct = funct;
    new->source_registry = 0,new->source_addressing_mode=0;
    new->destination_registry = 0,new->destination_addressing_mode=0;
    return new;
}

/**
 * add word to the general word list
 * @param new - word to add
 * @return if succeed
 */
bool add_word_to_word_list(word *new){
    if (new ==NULL) {
        return false;
    }
    if(!word_list_head){
        word_list_head = new;
    } else {
        word * ptr = word_list_head;
        while (ptr->next){
            ptr = ptr->next;
        }
        ptr->next = new;
    }
    return true;

}

/**
 * push new word to another word chain
 * @param first_word - word chain
 * @param word_to_add - new word to end to the end of the word chain!
 */
void push_word(word *first_word, word *word_to_add) {
    if(!first_word->next) {
        first_word->next = word_to_add;
    } else {
        word * ptr = first_word;
        while (ptr->next){
            ptr = ptr->next;
        }
        ptr->next = word_to_add;
    }
}

/**
 * get the word list length
 * @return length
 */
int word_list_length(){
    int count = 0;
    word *it = word_list_head;
    while (it != NULL) {
        count++;
        it = it->next;
    }
    return count;
}

 /**
  * get the next word from list with missing symbol
  * @return word with missing symbol
  */
word *get_next_missing_symbol_word(){
    word * it = word_list_head;
    while (it){
        if (it->dlw && it->dlw->symbol_need_to_be_filled) return it;
        it = it->next;
    }
    return NULL;

}

/**
 * free the general word list from memory
 */
void free_word_list() {
    word *it = word_list_head, *current_word;
    while (it != NULL) {
        current_word = it;
        it = it->next;
        free_op_code_word(current_word->ocw);
        free_funct_details_word(current_word->fdw);
        free_data_word(current_word->dlw);
    }
    word_list_head = NULL;
}

/**
 * free the passed word chain from memory
 * @param head - word chain
 */
void free_word_chain(word * head) {
    word *it = head, *current_word;
    while (it != NULL) {
        current_word = it;
        it = it->next;
        free_op_code_word(current_word->ocw);
        free_funct_details_word(current_word->fdw);
        free_data_word(current_word->dlw);
    }
    head = NULL;
}

/**
 * free funct_details_word from memory
 * @param word - funct_details_word
 */
void free_funct_details_word(funct_details_word *word) {
    if (word == NULL){
        return;
    }
    free(word);
}

/**
 * free op code word from memory
 * @param word - op code word
 */
void free_op_code_word( op_code_word *word) {
    if (word == NULL){
        return;
    }
    free(word);
}

/**
 * free data word from memory
 * @param word
 */
void free_data_word( data_word *word) {
    if (word == NULL){
        return;
    }
    if (word->symbol_need_to_be_filled) free(word->symbol_need_to_be_filled);
    free(word);
}