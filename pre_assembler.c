/**
 * parse macro's process
 */
#include "headers.h"
#include "consts.h"

/**
 * pre assembler get the assembler file and look for macros
 * after finding all the macros, the pre assembler parse each macro call to the declared macro
 * @param file_to_read - assembler file to read
 * @param file_name - generated .am file name
 */
void pre_assembler(FILE *file_to_read, char *file_name,status *file_status) {
    FILE *file_to_write; /* file to read pointer */
    char line[MAX_LINE_LENGTH]; /* line declaration */
    char line_copy[MAX_LINE_LENGTH]; /* line copy declaration*/
    char * current_word = NULL; /* current word pointer */
    macro *current_macro = NULL; /* macro pointer */
    int reading_new_macro_flag = 0; /* inside new macro flag*/

    file_to_write = read_file_with_extension(file_name , ".am", "w"); /* create .am file*/
    if (!file_to_write){ /* if couldn't create am file exit !*/
        fprintf(stderr,"could not create %s.am file",file_name);
        exit(1);
    }

    while (fgets(line , MAX_LINE_LENGTH, file_to_read)){ /* read new line */
        file_status->line_number++; /* increase line number in file status */
        strcpy(line_copy,line); /* copy line */
        current_word = strtok(line_copy, " \t\n"); /* get first word */
        if(reading_new_macro_flag){ /* if in the middle of reading new file */
            if(strcmp(current_word, EOF_MACRO_KEYWORD)){ /* check if end of macro */
                strcat(current_macro->macro_code, line); /* if not concat line to macro*/
                continue;
            }else {
                reading_new_macro_flag= 0; /* end macro */
                current_macro = NULL;
                continue;
            }
        }
        if (!strcmp(current_word, MACRO_KEYWORD)){ /* if the word is a macro declaration */
            current_word = strtok(NULL, " \t\n"); /* get macro name*/
            if (!current_word){
                log_error_wrapper("macro must have a name, skipping file !", file_status,NULL);
                return;
            } else if  (!(current_macro = add_macro(current_word))){ /* add new macro to list and get macro object */
                log_error_wrapper("cannot add new macro, skipping file!", file_status,NULL);
                return; /* macro errors not expected ! (according to task) */
            } else {
                reading_new_macro_flag = 1; /* mark reading new macro flag*/
            }
        }else{
            macro *lookup_macro = NULL;
            if (!(lookup_macro = get_macro_by_id(current_word))){ /*look for a macro */
                fputs(line,file_to_write); /* if not macro write the line to file */
            } else {
                fputs(lookup_macro->macro_code,  file_to_write); /* if macro write macro instead of the line*/
            }
        }
    }
    fclose(file_to_write); /* close am file */
    free_macro_list(); /* free macro list */
}
