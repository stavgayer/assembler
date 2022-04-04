/**
 * utils functions
 */
#include "headers.h"
#include "consts.h"

/**
 * log error and exist
 * @param err
 * @param file_status
 */
void log_panic_wrapper(char *err, status *file_status) {
    char error_msg[MAX_LOG_SIZE];
    sprintf(error_msg, ERROR_WRAPPER, file_status->file_name, file_status->line_number, err);
    fprintf(stderr, "%s", error_msg);
    exit(1);
}

/**
 * log error and update file status. set should skip if exist
 * @param err
 * @param file_status
 * @param should_skip
 */
void log_error_wrapper(char *err, status *file_status,bool *should_skip) {
    char error_msg[MAX_LOG_SIZE];
    if (should_skip != NULL) {
        *should_skip = true;
    }
    file_status->errors_flag = true;
    sprintf(error_msg, ERROR_WRAPPER, file_status->file_name, file_status->line_number, err);
    fprintf(stderr,"%s" ,error_msg);
}


/**
 * log error without line and update file status. set should skip if exist
 * @param err
 * @param file_status
 * @param should_skip
 */
void log_error_without_line_wrapper(char *err, status *file_status,bool *should_skip) {
    char error_msg[MAX_LOG_SIZE];
    if (should_skip != NULL) {
        *should_skip = true;
    }
    file_status->errors_flag = true;
    sprintf(error_msg, ERROR_WRAPPER_NO_LINE, file_status->file_name, err);
    fprintf(stderr,"%s" ,error_msg);
}

/**
 * safe malloc
 * @param n - size of object
 * @return pointer to created object
 */
void *safe_malloc(size_t n) {
    void *p = malloc(n);
    if (p == NULL) {
        fprintf(stderr, "Fatal: failed to allocate memory.\n");
        abort();
    }
    return p;
}

/**
 * check if line is valid
 * @param line - line to check
 * @param fp - the file
 * @param file_status - file status object
 * @return if the line is valid
 */
bool is_valid_line(char *line, FILE *fp, status *file_status) {
    if (strchr(line, '\n') == NULL && !feof(fp)) {
        char err_msg[MAX_LINE_LENGTH];
        sprintf(err_msg, "line maximum length is %d", MAX_LINE_LENGTH);
        log_error_wrapper(err_msg, file_status,NULL);
        return false;
    }
    return true;
}
/**
 * concat 2 strings
 * @param s1
 * @param s2
 * @return concat string
 */
char *concat(const char *s1, const char *s2) {
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}


/**
 * extract the 4 bits from position p
 * @param number - number to extract bits from
 * @param p - position
 * @return - extracted bits
 */
unsigned int extract_4_bits_from_pos(long number, int p) {
    number = number >> p;
    return ((number) & 0xF);
}

/**
 * create new file status
 * @param file_name - file name
 * @param ext - file extension
 * @return new file status
 */
status *new_status(char *file_name, char *ext) {
    status *file_status = NULL;
    file_status = (status *) safe_malloc(sizeof(status));
    file_status->file_name = concat(file_name, ext);
    file_status->line_number = 0;
    file_status->errors_flag = false;
    return file_status;
}

/**
 * free file status from memory
 * @param current_status
 */
void free_status(status *current_status) {
    free(current_status->file_name);
    free(current_status);
}

/**
 * free all linked lists in project
 */
void free_linked_lists(){
    free_symbol_table();
    free_word_list();
    free_macro_list();
    free_entry_list();
    free_data_list();
    free_externals_list();
}
