/**
 * data list declaration and functions
 */

#include "structs.h"
#include "headers.h"

/* data linked list head pointer */
static data * data_list_head = NULL;

/*
 * print_data_to_file-
 * the function gets ob file and final IC calc
 * and write the data words to the object file
 */
void print_data_to_file(FILE * file_to_write, int IC) {
    data * it = data_list_head;
    while (it){
        unsigned long word_bits = 0;
        word_bits = (( (1 <<2) & 0xFF)<<16) | ((unsigned short int)(it->value) & 0xFFFFF) ;
        write_num_to_file(file_to_write,word_bits,it->dc+IC);
        it = it->next;
    }
}
/**
 * the function get current ic and data value and add it to the data list
 * @param ic - current address
 * @param value - current data value
 * @return the create data object
 */
data *add_value_to_data_list(int ic , int value){
    data *new = NULL;
    new = malloc(sizeof(data));
    if (new == NULL){
        fprintf(stderr,"could not allocate memory ");
        exit(1);
    }
    new->dc = ic;
    new->value = value;
    new->next = NULL;
    if(!data_list_head){
        data_list_head = new;
    } else {
        data * ptr = data_list_head;
        while (ptr->next){
            ptr = ptr->next;
        }
        ptr->next = new;
    }
    return new;
}

void free_data_list() {
    data *it = data_list_head, *tmp;

    while (it != NULL) {
        tmp = it;
        it = it->next;
        free(tmp);
    }
    data_list_head = NULL;
}