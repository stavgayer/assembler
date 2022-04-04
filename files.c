/**
 * files functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers.h"


/* concat file name with file extension */
char * get_full_file_name(char  *file_name, char *ext){
    char *full_file_name = (char *) safe_malloc(strlen(file_name) + strlen(ext)); /* allocate memory for new string*/
    strcpy(full_file_name, file_name); /* copy file name to new string */
    strcat(full_file_name, ext); /* concat extension */
    return full_file_name;
}

/**
 * read function utility function
 * @param file_name - file name to read
 * @param ext - file extension
 * @param modes - file mode
 * @return file
 */
FILE * read_file_with_extension(char *file_name, char *ext, char *modes) {
    FILE * file_to_read = NULL;
    char *full_file_name = get_full_file_name(file_name, ext);
    if (!(file_to_read = fopen(full_file_name, modes) )) {
        fprintf(stderr, "couldn't read file %s, please make sure the file exist, skipping.\n", full_file_name);
        free(full_file_name);
        return 0;
    }
    free(full_file_name);
    return file_to_read;
}
/**
 * the function generate the object file and calls the functions that write the data to file with the final DC and IC
 * @param file_name - object file name
 * @param IC - final IC
 * @param DC - final DC
 */
void create_ob_file(char *file_name,int IC, int DC) {
    FILE * file_to_write = NULL;
    fprintf(stdout,"creating %s.ob\n", file_name);
    if (!(file_to_write = read_file_with_extension(file_name,".ob", "w"))){
        return;
    }
    fprintf(file_to_write, "%d %d\n",IC- MEMORY_START_ADDRESS , DC);
    print_words_to_file(file_to_write);
    print_data_to_file(file_to_write,IC);
    fclose(file_to_write);
}


/**
 * the function generates the externals file if there is externals and calls to the function that writes them to the file
 * @param file_name - externals file name
 */
void create_ext_file(char *file_name) {
    FILE * file_to_write = NULL;
    external *ext = get_head_externals();
    if (ext) {
        fprintf(stdout,"creating %s.ext\n", file_name);
        if (!(file_to_write = read_file_with_extension(file_name,".ext", "w"))){
            return;
        }
        print_externals_to_file(file_to_write);
        fclose(file_to_write);
    }
}

/**
 * the functions creates the entries file if there is entries and calls the function that writes them to the file
 * @param file_name - entries file name
 */
void create_ent_file(char *file_name) {
    FILE * file_to_write = NULL;
    entry *ent = get_entries_list_head();
    if(ent){
        fprintf(stdout,"creating %s.ent\n", file_name);
        if (!(file_to_write = read_file_with_extension(file_name,".ent", "w"))){
            return;
        }
        print_entries_to_file(file_to_write);
        fclose(file_to_write);
    }
}