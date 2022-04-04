/**
 * first pass functions
 */
#include "headers.h"
#include "consts.h"

/**
 * get_machine_directive_line- the functions gets the machine directive line and parse it to machine directive line object
 * machine directive line contains the machine directive of the line and the 2 operans (if exist)
 * @param current_token - current line after symbol extraction (if exist)
 * @param file_status - file status object
 * @param should_skip - should skip line flag
 * @return
 */
machine_directive_line *get_machine_directive_line(char *current_token, status *file_status, bool *should_skip) {
    machine_directive *md = NULL; /* init null machine directive */
    machine_directive_line *mdl = NULL; /* pointer of machine_directive_line */
    int operand_counter = 0; /* line operand counter */
    char * op; /* operand 1 name pointer */
    char * op2; /* operand 2 name pointer */
    if (!(md = get_machine_directive(current_token))){  /* get current machine_directive */
        log_error_wrapper("unknown machine directive", file_status,should_skip);
        return NULL;
    }

    mdl = (machine_directive_line *)safe_malloc(sizeof(machine_directive_line)); /* allocate new mdl in memory*/
    mdl->md = md; /* set mdl machine_directive to the one we found earlier */
    mdl->destination_operand = NULL;
    mdl->source_operand = NULL;


    current_token = strtok(NULL, " ,\t\n"); /* get first operand */
    if (!current_token) {
        if (operand_counter == md->amount_of_operands){ /* if there is no new operand the md don't receive any operand return the current mdl*/
            return mdl;
        }
        log_error_wrapper("invalid number of operands", file_status,should_skip);
        free(mdl);
        return NULL;
    }

    operand_counter++; /* add new operand to the counter */
    op = (char *) safe_malloc(sizeof(char ) * MAX_LINE_LENGTH); /* allocate new operand in memory*/
    strcpy(op, current_token);  /* set the new operand in mdl */

    current_token = strtok(NULL, " , \t\n"); /* get second operand */
    if (!current_token) {
        if (operand_counter == md->amount_of_operands){ /* if the second operand doesnt exist and the md don't receive second operand return the current mdl*/
            mdl->destination_operand = op;
            return mdl;
        }
        log_error_wrapper("invalid number of operands", file_status,should_skip);
        free(op);
        free(mdl);
        return NULL;
    }

    operand_counter++; /* add new operand to the counter */
    op2 = (char *) safe_malloc(sizeof(char ) * MAX_LINE_LENGTH); /* allocate new operand in memory*/
    strcpy(op2, current_token);  /* set the new operand in mdl */

    current_token = strtok(NULL, " ,\t\n"); /* looking for extra words in line */
    if (!current_token) {
        if (operand_counter == md->amount_of_operands){ /* if the line have exactly 2 operands and the md expecting 2 operand return the current mdl */
            mdl->source_operand = op;
            mdl->destination_operand = op2;
            return mdl;
        }
        log_error_wrapper("invalid number of operands", file_status,should_skip);
        free(op);
        free(op2);
        free(mdl);
        return NULL;
    }else {
        log_error_wrapper("too much operands", file_status,should_skip);
        free(op);
        free(op2);
        free(mdl);
        return NULL;
    }

}

/**
 * is_index_addressing_mode - the function gets an operand and check if the the operand addressing mode is index
 * if so, the function initialize the operand symbol name and the register inside the parentheses
 * @param operand - operand to check
 * @param symbol_name - symbol_name to initialize if the operand addressing mode is index
 * @param register_num - register_num to initialize if the operand addressing mode is index
 * @param file_status - file status object
 * @param should_skip - should skip code line flag
 * @return
 */
bool is_index_addressing_mode(char *operand, char *symbol_name ,unsigned int *register_num, status *file_status, bool *should_skip) {
    char *token_cpy = operand; /* operand char pointer copy */
    int i = 0;
    int reg = 0;
    char registry[4];
    char tmp_symbol_name[SYMBOL_MAX_LENGTH]; /* init tmp symbol name variable*/
    while ((*token_cpy) != ' ' && (*token_cpy) != '\0' && (*token_cpy) != '\n' && (*token_cpy) != EOF && (*token_cpy ) != '[' && i<SYMBOL_MAX_LENGTH) { /* extract symbol name */
        tmp_symbol_name[i] = (*token_cpy);
        i++;
        token_cpy++;
    }
    if( (*token_cpy) != '[' ){ /* if in the end of the symbol there is no parentheses, it's not index */
        return false;
    }
    token_cpy++;
    tmp_symbol_name[i] = '\0';
    if (!is_valid_symbol_name(tmp_symbol_name, file_status, false, should_skip)) { /* check if symbol nane is valid */
        return false;
    }
    strcpy(symbol_name,tmp_symbol_name);
    i= 0;
    while ((*token_cpy) != ' ' && (*token_cpy) != '\0' && (*token_cpy) != '\n' && (*token_cpy) != EOF && (*token_cpy ) != ']' && i<3) { /* extract registry */
        registry[i] = (*token_cpy);
        i++;
        token_cpy++;
    }
    registry[i] = '\0';
    if( (*token_cpy) != ']' ){ /*if parentheses not closed in the end of the registry name it's not an index */
        return false;
    }

    if ((reg = is_register(registry)) == -1) { /* check if valid registry */
        log_error_wrapper("invalid registry!", file_status,should_skip);
        return false;
    }
    *register_num = reg;
    token_cpy++;
    if ((*token_cpy) != '\0' && (*token_cpy) != '\n' && (*token_cpy) != EOF) {
        return false;
    } /* if there is more words to the line, its not a valid index */

    return true;

}


/**
 * the function gets operand and returns the operand addressing mode
 * @param operand
 * @param index_symbol - in case of index addressing mode, the variable initialize with the symbol name
 * @param register_num - in case of registry or index, the variable initialize with the registry number
 * @param immediate_num - in case of immediate, the variable initialize with the immediate value
 * @param file_status - file status object
 * @param should_skip - should skip code line flag
 * @return
 */
addressing_mode analyse_addressing_mode(char *operand, char *index_symbol, unsigned int *register_num, int *immediate_num , status *file_status, bool *should_skip){
    int result = 0; /* immediate number initialize */
    int reg = 0; /* registry number initialize */
    if(!operand){ /* if the operand is empty return null */
        return NONE;
    }

    if (operand[0] == '#'){ /* if the operand starts with #, check for immediate addressing mode */
        char *end;
        char *operand_cpy = operand;
        operand_cpy++;
        result = strtol(operand_cpy, &end, 10); /* get immediate number and look for valid number*/
        if (*end != '\0') {
            log_error_wrapper("invalid number", file_status,should_skip);
            return NONE;
        }

        if ((result < INT16_MIN) || (result > INT16_MAX)) { /* if immediate number is out of range (16 signed int) return error */
            log_error_wrapper("out of range", file_status,should_skip);
            return NONE;
        }
        *immediate_num = result; /* set immediate number with the parsed value*/
        return IMMEDIATE;
    }


    if ((reg = is_register(operand)) != -1) { /* check if operand is registry and if so, set registry number*/
        *register_num = reg;
        return REGISTER_DIRECT;
    }

    if(is_index_addressing_mode(operand, index_symbol,register_num, file_status, should_skip)) { /* check if operand is index and if so, set index_symbol and register_num */
        return INDEX;
    } else if (*should_skip){
        return NONE;
    }

    if (is_valid_symbol_name(operand, file_status, true,should_skip)) { /* check if operand is valid symbol name and return direct addressing mode */
        return DIRECT;
    }
    return NONE;
}

/**
 * the function gets the current machine directive line and validate and parsing it to
 * code words, and increasing the current IC accordingly
 * @param current_token current line token
 * @param md - current machine directive
 * @param IC - pointer to current IC count
 * @param file_status - file status object
 * @param should_skip - should skip code line flag
 * @return true if succeed
 */
bool analyse_operands_structure_of_machine_directive(char *current_token, int *IC, status *file_status, bool *should_skip) {
    int add_to_ic = 0; /* count words counter init */
    word *first_word; /* pointer to first word */
    word *second_word = NULL; /* pointer to second word */
    word *direct_word_base , *direct_word_offset, *immediate_word; /* pointer to words */
    unsigned int absolute_t = ((1 << 2) & 0xFF);

    machine_directive_line *mdl;
    if(!(mdl = get_machine_directive_line(current_token,file_status,should_skip))){ /* get current mdl */
        return false;
    }
    first_word = new_word(); /* init first word */
    first_word->ocw = (op_code_word *) safe_malloc( sizeof(op_code_word));/* init first word as op code word */
    first_word->ocw->op_code = (1 << mdl->md->opcode); /* set op code*/
    first_word->address = *IC ; /* set word address*/
    add_to_ic++; /* increase current word count */

    if(mdl->md->amount_of_operands > 0){ /* if machine directive gets operands, initialize second word, set funct and chain it to first word*/
        second_word = new_word();
        second_word->fdw = new_funct_details_word(mdl->md->funct);
        second_word->fdw->funct = mdl->md->funct;
        first_word->next = second_word;
        second_word->address = *IC + add_to_ic;
        add_to_ic++;/* increase current word count */
    }

    if (mdl->source_operand != NULL) { /* if source operand is not null*/
        unsigned int register_num;
        int immediate_num;
        char index_symbol[MAX_LINE_LENGTH];
        /* get source operand addressing mode*/
        addressing_mode source_addressing_mode = analyse_addressing_mode(mdl->source_operand, index_symbol, &register_num, &immediate_num, file_status, should_skip);


        if (source_addressing_mode == NONE) { /* if couldn't get source operand addressing mode, return false */
            free_word_chain(first_word);
            return false;
        } else if (source_addressing_mode == REGISTER_DIRECT){ /* if registry, set second word source addressing mode and registry number in second word */
            second_word->fdw->source_addressing_mode = REGISTER_DIRECT-1;
            second_word->fdw->source_registry = register_num;
        } else if (source_addressing_mode == INDEX){ /* if index, set second word source addressing mode and registry number in second word, and add 2 data words to be filled*/
            second_word->fdw->source_addressing_mode = INDEX-1;
            second_word->fdw->source_registry = register_num;

            direct_word_base = new_word();
            direct_word_base->address = *IC +add_to_ic;

            direct_word_offset = new_word();
            direct_word_offset->address = *IC +add_to_ic+1;

            direct_word_base->dlw = new_data_word(NULL, NULL, index_symbol, BASE);
            direct_word_offset->dlw = new_data_word(NULL, NULL, index_symbol, OFFSET);

            push_word(second_word, direct_word_base);
            push_word(second_word, direct_word_offset);
            add_to_ic = add_to_ic+2; /* increase word counter by 2 */

        } else if (source_addressing_mode == IMMEDIATE) { /* if immediate, set second word source addressing mode in second word, and add 1 data word with immediate value */
            second_word->fdw->source_addressing_mode = IMMEDIATE-1;
            second_word->fdw->source_registry = 0;

            immediate_word = new_word();
            immediate_word->dlw = new_data_word(&absolute_t,&immediate_num,NULL, OFFSET);

            push_word(first_word, immediate_word);
            immediate_word->address = *IC +add_to_ic;
            add_to_ic++; /* increase word counter by 1 */
        } else { /* if direct, set second word source addressing mode in second word, and add 2 data words to be filled  */
            second_word->fdw->source_addressing_mode = DIRECT-1;
            second_word->fdw->source_registry = 0;

            direct_word_base = new_word();
            direct_word_base->address = *IC +add_to_ic;

            direct_word_offset = new_word();
            direct_word_offset->address = *IC +add_to_ic+1;

            direct_word_base->dlw = new_data_word(NULL,NULL,mdl->source_operand, BASE);
            direct_word_offset->dlw = new_data_word(NULL,NULL,mdl->source_operand, OFFSET);

            push_word(second_word, direct_word_base);
            push_word(second_word, direct_word_offset);
            add_to_ic = add_to_ic+2; /* increase word counter by 2 */
        }
    }

    if (mdl->destination_operand != NULL) { /* same check as the source operand but for the destination operand*/
        unsigned int register_num;
        int immediate_num;
        char index_symbol[MAX_LINE_LENGTH];
        addressing_mode dest_addressing_mode = analyse_addressing_mode(mdl->destination_operand, index_symbol, &register_num, &immediate_num, file_status, should_skip);

        if (dest_addressing_mode== NONE) {
            free_word_chain(first_word);
            return false;
        } else if (dest_addressing_mode == REGISTER_DIRECT){
            second_word->fdw->destination_addressing_mode = REGISTER_DIRECT-1;
            second_word->fdw->destination_registry = register_num;
        } else if (dest_addressing_mode == INDEX){
            second_word->fdw->destination_addressing_mode = INDEX-1;
            second_word->fdw->destination_registry = register_num;

            direct_word_base = new_word();
            direct_word_base->address = *IC +add_to_ic;

            direct_word_offset = new_word();
            direct_word_offset->address = *IC +add_to_ic+1;

            direct_word_base->dlw = new_data_word(NULL, NULL, index_symbol, BASE);
            direct_word_offset->dlw = new_data_word(NULL, NULL, index_symbol, OFFSET);

            push_word(first_word, direct_word_base);
            push_word(first_word, direct_word_offset);
            add_to_ic = add_to_ic+2;

        } else if (dest_addressing_mode == IMMEDIATE) {
            second_word->fdw->destination_addressing_mode = IMMEDIATE-1;
            second_word->fdw->destination_registry = 0;

            immediate_word = new_word();
            immediate_word->dlw = new_data_word(&absolute_t,&immediate_num,NULL, OFFSET);

            push_word(first_word, immediate_word);
            immediate_word->address = *IC +add_to_ic;
            add_to_ic++;
        } else {
            second_word->fdw->destination_addressing_mode = DIRECT-1;
            second_word->fdw->destination_registry = 0;

            direct_word_base = new_word();
            direct_word_base->address = *IC +add_to_ic;

            direct_word_offset = new_word();
            direct_word_offset->address = *IC +add_to_ic+1;

            direct_word_base->dlw = new_data_word(NULL,NULL,mdl->destination_operand, BASE);
            direct_word_offset->dlw = new_data_word(NULL,NULL,mdl->destination_operand, OFFSET);

            push_word(first_word, direct_word_base);
            push_word(first_word, direct_word_offset);
            add_to_ic = add_to_ic+2;
        }

    }

    if (!add_word_to_word_list(first_word)) { /* add current word chain to word linked list*/
        log_panic_wrapper("cannot add word to to word list",file_status);
    }
    (*IC) = (*IC)+add_to_ic; /* increase IC with current ic counter*/
    return true;
}

/**
 * validate and insert number variable to data list
 * @param current_num - number to check
 * @param i - end of number index
 * @param file_status - file status object
 * @param DC - pointer to current dc counter
 * @return if insert succeed
 */
bool insert_num(char *current_num, int i, status *file_status, int *DC) {
    char *end; /* end pointer */
    data *inserted_data =NULL; /* inserted data pointer*/
    int result ; /* int result from string to int check */
    current_num[i] = '\0'; /* set end of number*/

    if (strlen(current_num) == 0) { /*check if number is empty*/
        log_error_wrapper("empty number", file_status,NULL);
        return false;
    }

    result = strtol(current_num, &end, 10); /* covert num to int and check if valid*/
    if (*end != '\0') {
        log_error_wrapper("invalid number", file_status,NULL);
        return false;
    }

    if ((result < INT16_MIN) || (result > INT16_MAX)) { /* check if number is out of range (16 signed int). if so, return error */
        log_error_wrapper("out of range", file_status,NULL);
        return false;
    }
    inserted_data = add_value_to_data_list(*DC,result); /* insert number to data list */
    if (!inserted_data) {
        log_error_wrapper("error inserting new data to data list", file_status,NULL);
        return false;
    }
    (*DC)++; /*increase current dc count*/
    return true;
}

/**
 * validate and parse data directive, if valid insert numbers to data list
 * @param current_token - data directive line
 * @param file_status - file status object
 * @param should_skip - should skip line flag
 * @param DC - pointer to current dc count
 * @return if parsing succeed
 */
bool data_directive_parsing(char *current_token, status *file_status, bool *should_skip, int *DC) {
    bool expecting_comma = false; /* expecting comma flag */
    bool status; /* insert number status */
    while (current_token) {
        char *token_cpy = current_token;
        char current_num[MAX_LINE_LENGTH];
        int i = 0;
        while ((*token_cpy) != ' ' && (*token_cpy) != '\0' && (*token_cpy) != '\n' && (*token_cpy) != EOF) { /* validate data directive line */
            if ((*token_cpy) == ',') {
                if (!expecting_comma) {
                    log_error_wrapper("syntax error: unexpected comma", file_status,should_skip);
                    return false;
                } else {
                    expecting_comma = false;
                    current_num[i] = '\0';
                    status = insert_num(current_num, i, file_status, DC); /* insert parsed variable to data list*/
                    if (!status) {
                        *should_skip = true;
                        return false;
                    }
                    strcpy(current_num, "");
                    i = 0;
                }
            } else {
                expecting_comma = true;
                current_num[i] = (*token_cpy);
                i++;
            }
            token_cpy++;
        }
        current_num[i] = '\0';
        if (strlen(current_num) > 0) {
            expecting_comma = true;
            status = insert_num(current_num, i, file_status, DC);  /* insert parsed variable to data list*/
            if (!status){
                *should_skip = true;
                return false;
            }
            strcpy(current_num, "");
        }

        current_token = strtok(NULL, " \t\n");
    }
    return true;
}

/**
 * validate and parse string directive line, if valid insert string chars to data list
 * @param current_token - string directive line
 * @param file_status - file status object
 * @param should_skip - should skip flag
 * @param DC - pointer to current dc count
 * @return if parsing succeed
 */
bool string_directive_parsing (char *current_token, status *file_status, bool *should_skip, int *DC) {
    char *token_cpy ; /* current token copy pointer */
    data *inserted_data; /* inserted data pointer */
    int token_len = strlen(current_token); /* get current token length */
    if (token_len <2  || (*current_token) != '"' || current_token[token_len-1] != '"' ){ /* if sting len is less than 2 or string doesn't end or starts with ", it's not a valid string*/
        log_error_wrapper("illegal string", file_status,should_skip);
        return false;
    }

    token_cpy = current_token; /*  copy token to copy token*/
    current_token[token_len-1] = '\0';
    do {
        token_cpy++;
        inserted_data = add_value_to_data_list(*DC,*token_cpy); /* add each char ascii code to the data list */
        if (!inserted_data) {
            log_error_wrapper("error inserting new data to data list", file_status,should_skip);
            return false;
        }
        (*DC)++; /*  increase dc counter*/
    } while ((*token_cpy) != '\0');
    return true;
}

/**
 * the function get the next line word and check if its a directive line
 * if so, the line gets validate and parsed according to the directive type
 *
 * @param current_token - current word
 * @param file_status - file status object
 * @param should_skip - should skip line flag
 * @param new_symbol - if directive is set with symbol, symbol passed to the function and parsed accordingly
 * @param DC - pointer to current dc counter
 * @return returns if directive
 */
bool is_directive(char *current_token, status *file_status, bool *should_skip, char *new_symbol, int *DC) {
    if (current_token[0] != '.') { /* if current word doesn't start with ., it's not a directive*/
        return false;
    }
    if (!strcmp(current_token, DATA_CODE_DIRECTIVE)) { /* check if data directive and if so, validate and parse*/
        if (new_symbol) {
            if (!add_symbol_to_symbol_table(new_symbol, *DC, false, false, false, true, file_status)){
                *should_skip = true;
                return false;
            }
        }
        current_token = strtok(NULL, " \t\n");
        if (!current_token) {
            log_error_wrapper("data directive cannot be empty!", file_status,should_skip);
            return false;
        }
        return data_directive_parsing(current_token, file_status, should_skip, DC);
    } else if (!strcmp(current_token, STRING_CODE_DIRECTIVE)) { /* check if string directive and if so, validate and parse*/
        if (new_symbol) {
            if (!add_symbol_to_symbol_table(new_symbol, *DC, false, false, false, true, file_status)){
                *should_skip = true;
                return false;
            }
        }
        current_token = strtok(NULL, "\n");
        if (!current_token) {
            log_error_wrapper("sting directive cannot be empty!", file_status,should_skip);
            return false;
        }
        return string_directive_parsing(current_token, file_status, should_skip, DC);
    } else if (!strcmp(current_token, EXTERN_CODE_DIRECTIVE)) { /* check if data directive and if so, validate and parse */
        current_token = strtok(NULL, "\n");
        if (!current_token) {
            log_error_wrapper("extern directive cannot be empty!", file_status,should_skip);
            return false;
        }
        if (!is_valid_symbol_name(current_token,file_status,true,should_skip)) {
            return false;
        }
        if (!add_symbol_to_symbol_table(current_token, 0, true, false, false, false, file_status)){
            *should_skip = true;
            return false;
        }
        return true;
    } else if (!strcmp(current_token, ENTRY_CODE_DIRECTIVE)) { /* check if entry directive and if so, add to entry list */
        /* instead of reading the file lines again and look for entries in the second pass
         * the entries saved to entries linked list and then, in the second pass the code iterate through this list
         * instead of reading the file again.
         * */
        current_token = strtok(NULL, "\n");
        if (!current_token) {
            log_error_wrapper("entry directive cannot be empty!", file_status,should_skip);
            return false;
        }
        if (!is_valid_symbol_name(current_token,file_status,true,should_skip)) { /* validate entry symbol name*/
            return false;
        }
        add_entry(current_token);
        return true;
    } else {
        log_error_wrapper("this code directive is not supported", file_status,should_skip); /* invalid directive*/
        return false;
    }
}




/**
 * first pass-
 * the first pass is the core of the program
 * the function loop thorough the lines of the code file and validating and parsing each line
 * first the function check of each line is valid (not too long) and then looking for symbol
 * after that, the function is determining if the line is a directive or machine directive line
 * and parsing the line accordingly
 *
 * @param file_to_read - the generated file from the pre assembler
 * @param IC - pointer to the initialized IC counter
 * @param DC - pointer to the initialized DC counter
 * @param file_status - file status object - object used by the program to save line number and validation status
 */
void first_pass(FILE *file_to_read,int *IC, int *DC, status *file_status) {
    char line[MAX_LINE_LENGTH]; /* line string */
    char *current_token = NULL; /* current word of the line */
    char *new_symbol = NULL; /* symbol pointer */
    bool should_skip = false; /* should skip current line flag */
    char line_copy[MAX_LINE_LENGTH]; /* declaration of line copy */

    while (fgets(line, MAX_LINE_LENGTH, file_to_read)) { /* read new line from file */
        file_status->line_number++; /* increase line number in file status */
        should_skip = false; /* reset should skip line status */

        strcpy(line_copy, line);
        if (!is_valid_line(line_copy, file_to_read, file_status)) { /* check if line is valid*/
            continue;
        }
        new_symbol = NULL;
        current_token = strtok(line_copy, " \t\n");
        if (!current_token || !strcmp(current_token, ";")) { /* if comment or empty line, skip current line */
            continue;
        }

        if (is_new_symbol(current_token, file_status, &should_skip, new_symbol)) { /* look for new symbol, and if exist set current symbol pointer to  parsed symbol*/
            new_symbol = current_token;
            current_token = strtok(NULL, " \t\n"); /* move to next word*/
            if (!current_token) { /* if there is no word after symbol, return error */
                log_error_wrapper("symbol must be assigned with value", file_status,NULL);
                continue;
            }
        } else if (should_skip) { /*if should skip current line due to error, continue */
            continue;
        }

        if (is_directive(current_token, file_status, &should_skip, new_symbol, DC) || should_skip) { /* check if its a directive line and if so and the directive parsed correctly or error found, continue */
            continue;
        }

        if (new_symbol) { /* if it's not a directive, its must be machine directive, so first check if there is symbol and if so save it to symbol list*/
            if (!add_symbol_to_symbol_table(new_symbol, *IC, false, true, false, false, file_status)){
                continue;
            }
        }

        if (!analyse_operands_structure_of_machine_directive(current_token,IC,file_status,&should_skip)){ /* parse machine directive line and continue if there is error, continue */
            continue;
        }
    }
    if (file_status->errors_flag) { /* if errors found in the first pass, log that error found and skip file */
        fprintf(stderr,"error found in first pass, skipping file!\n" );
        return;
    }
    add_ic_to_data_symbols(*IC); /* update data symbol addresses with final ic */
    if((*IC)+(*DC) > MEMORY_MAX_SIZE) { /* check if code memory reached limit */
        log_error_wrapper("out of memory!" , file_status,&should_skip);
    }
    fprintf(stdout, "first pass of %s finished successfully\n", file_status->file_name);
}