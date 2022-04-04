/**
 * second pass functions
 */
#include "headers.h"
#include "consts.h"



/**
 * the second pass called if the first pass is passed without errors
 * first the function checking if all entries filled correctly
 * and than the function filling all the missing data words from the symbol table
 *
 * @param file_status - file status object
 */
void second_pass(status *file_status) {
    entry *entry_it =NULL;
    word *word_it =NULL;
    symbol *s_pointer = NULL;
    char error_msg[MAX_LOG_SIZE]; /* format err*/

    while ((entry_it = get_next_unchecked_entry())) { /* get the next unchecked entry*/
        if(!(s_pointer = get_symbol_by_id(entry_it->symbol_id))){ /* if entry not exist in symbol table, log error */
            sprintf(error_msg, "entry symbol %s not found in code",entry_it->symbol_id);

            log_error_without_line_wrapper(error_msg,file_status,NULL );
            entry_it->checked = true;/* mark as checked*/
        }else { /* else */
            int offset = s_pointer->value %16;
            s_pointer->is_entry = true; /* set  symbol as entry */
            entry_it->checked = true; /* mark as checked*/
            entry_it->offset = offset;
            entry_it->base = s_pointer->value - offset;
        }
    }

    while ((word_it = get_next_missing_symbol_word())){ /* get the next symbol with missing symbol*/

        if(!(s_pointer = get_symbol_by_id(word_it->dlw->symbol_need_to_be_filled))){ /* look for the missing symbol in symbol table*/
            sprintf(error_msg, "cannot find missing symbol %s", word_it->dlw->symbol_need_to_be_filled);
            log_error_without_line_wrapper(error_msg ,file_status,NULL ); /* if not exist, move to the next one*/
            free(word_it->dlw->symbol_need_to_be_filled);
            word_it->dlw->symbol_need_to_be_filled = NULL;
            free(word_it->next->dlw->symbol_need_to_be_filled);
            word_it->next->dlw->symbol_need_to_be_filled = NULL;
        } else {
            /*fill the missing label data */
            if (s_pointer->is_external){
                add_to_external_list(s_pointer->symbol_id,word_it->address,word_it->next->address );
                word_it->dlw->are = 1;
                word_it->dlw->data= 0;
                word_it->next->dlw->are = 1;
                word_it->next->dlw->data = 0;
            } else {
                int offset = s_pointer->value %16;
                word_it->dlw->are = ((1<<1) &0xFF);
                word_it->dlw->data= s_pointer->value - offset;
                word_it->next->dlw->are = ((1<<1) &0xFF);
                word_it->next->dlw->data = offset;
            }
            free(word_it->dlw->symbol_need_to_be_filled);
            word_it->dlw->symbol_need_to_be_filled = NULL;
            free(word_it->next->dlw->symbol_need_to_be_filled);
            word_it->next->dlw->symbol_need_to_be_filled = NULL;
        }
    }
    if (file_status->errors_flag) { /* if errors found in the second pass, log that error found and skip file */
        fprintf(stderr,"error found in second pass, skipping file!\n" );
        return;
    }
    fprintf(stdout, "second pass of %s finished successfully\n", file_status->file_name);

}
