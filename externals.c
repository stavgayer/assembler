/**
* externals list declaration and functions
*/
#include "headers.h"
#include "consts.h"

static external *external_list = NULL;

/**
 * add external to external list
 * @param symbol_id - external symbol name
 * @param base - base value
 * @param offset - off set value
 * @return external object
 */
external *add_to_external_list(char *symbol_id , int base, int offset){
    external *new = NULL;
    new = malloc(sizeof(external));
    if (new == NULL){
        fprintf(stderr,"could not allocate memory ");
        exit(1);
    }
    strcpy(new->symbol_name, symbol_id);
    new->base = base;
    new->offset =offset;
    new->next = NULL;
    if(!external_list){
        external_list = new;
    } else {
        external * ptr = external_list;
        while (ptr->next){
            ptr = ptr->next;
        }
        ptr->next = new;
    }
    return new;
}

/**
 * get the head object of the externals list
 * @return external head object
 */
external *get_head_externals() {
    external * it = external_list;
    return it;
}

/**
 * write externals list to ext file
 * @param file_to_write - externals file
 */
void print_externals_to_file(FILE * file_to_write) {
    external * it = external_list;
    while (it){ /* for each external */
        fprintf(file_to_write, "%s BASE %d \n%s OFFSET %d \n\n",  it->symbol_name, it->base, it->symbol_name,it->offset);
        it = it->next;
    }
}


/**
 * free externals list from memory
 */
void free_externals_list() {
    external *it = external_list, *tmp;
    while (it != NULL) {
        tmp = it;
        it = it->next;
        free(tmp);
    }
    external_list = NULL;
}