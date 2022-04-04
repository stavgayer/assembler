/**
 * Stav Gayer 208714154
 * main project file
 */
#include "headers.h"
#include "consts.h"


/**
 * assembler- Systems programming lab in Clang - The Open University of Israel
 * assembler get list of names of assembler files (.as files ) and parse the code macros to .am files
 * the code to machine code files (.ob)
 * parse the entries to entries files (.ent),
 * and the externals to externals variables file (.ext)
 * usage - ./assembler [FILE_NAME1] [FILE_NAME2]
 */
int main(int argc, char *argv[]) {
    int i;
    for (i = 1; i < argc; ++i) { /* iterate through the arguments*/
        FILE *file_to_read = NULL; /* file pointer */
        status *file_status = NULL; /* file status */

        int IC = MEMORY_START_ADDRESS; /* initialize the ic and dc for each file*/
        int DC = 0;

        if ((file_to_read = read_file_with_extension(argv[i], ".as", "r"))) { /* check if .as file exist and if so pass the assembly file to the pre assembler */
            file_status = new_status(argv[i], ".as"); /*init new file status object*/
            pre_assembler(file_to_read, argv[i],file_status); /* send file to pre assembler */
            if (file_status->errors_flag) { /* if error found during the macro parsing, free lists close file and continue !*/
                fclose(file_to_read);
                free_status(file_status);
                free_linked_lists();
                continue;
            }
            fclose(file_to_read); /* close file */
            free_status(file_status); /* free status object */
        } else {
            continue; /* if file not exist, continue */
        }

        if ((file_to_read = read_file_with_extension(argv[i], ".am", "r"))) { /* check if .am file exist and if so pass the file to the first pass */
            file_status = new_status(argv[i], ".am"); /*init new file status object*/
            first_pass(file_to_read, &IC, &DC, file_status); /* send file to first pass */
            if (file_status->errors_flag) { /* if error found in first pass, free lists close file and continue! */
                fclose(file_to_read);
                free_status(file_status);
                free_linked_lists();
                continue;
            }
            free_status(file_status); /* reset file status */
            file_status = new_status(argv[i], ".am");
            second_pass(file_status);  /* send file to second pass */
            if (file_status->errors_flag) { /* if errors found in second pass, close file, free lists and continue */
                fclose(file_to_read);
                free_status(file_status);
                free_linked_lists();
                continue;
            }
            free_status(file_status);
            fclose(file_to_read);
        } else {
            continue;
        }
        create_ob_file(argv[i],IC,DC); /* create object file */
        create_ext_file(argv[i]); /* create externals file */
        create_ent_file(argv[i]); /* create entries file */



        free_linked_lists(); /* free lists */
    }
    return 0;
}


