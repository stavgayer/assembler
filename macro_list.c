/**
 * macro list declaration and functions
 */

#include "structs.h"
#include "headers.h"
#include "consts.h"

/* macro linked list head pointer */
static macro * macro_list_head = NULL;

/**
 * add new macro with macro id to macro list and return the macro object
 * @param macro_id - macro name
 * @return generated macro object
 */
macro *add_macro(char *macro_id){
    macro *new = NULL;
    if((new = get_macro_by_id(macro_id))){
        fprintf(stderr, "cannot add new macro %s, macro already exist\n", macro_id);
        return NULL;
    }

    new = malloc(sizeof(macro));
    if (new == NULL){
        fprintf(stderr,"could not allocate memory ");
        exit(1);
    }

    new->macro_id = malloc(sizeof (char )*MAX_LINE_LENGTH);
    strcpy(new->macro_id,macro_id);

    new->macro_code = (char *)malloc(sizeof (char )*MAX_LINE_LENGTH * MACRO_MAX_LINE_NUMBER);
    new->macro_code[0] = '\0';
    new->next = NULL;
    if(!macro_list_head){
        macro_list_head = new;
    } else {
        macro * ptr = macro_list_head;
        while (ptr->next){
            ptr = ptr->next;
        }
        ptr->next = new;
    }
    return new;
}
/**
 * get macro object from linked list by macro id
 * @param macro_id - macro name
 * @return macro from macro list
 */
macro *get_macro_by_id(char *macro_id){
    macro * ptr = macro_list_head;
    while (ptr){
        if (!strcmp(ptr->macro_id,macro_id)){
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

/**
 * print macro list (debugging purpose)
 */
void display_macro_list() {
    macro * ptr;
    for (ptr = macro_list_head; ptr != NULL; ptr = ptr->next)
        printf("macro id: %s\nmacro code: %s \n", ptr->macro_id, ptr->macro_code);
}

/**
 * free macro list from memory
 */
void free_macro_list() {
    macro *it = macro_list_head, *tmp;

    while (it != NULL) {
        tmp = it;
        it = it->next;
        free(tmp->macro_id);
        free(tmp->macro_code);
        free(tmp);
    }
    macro_list_head = NULL;
}