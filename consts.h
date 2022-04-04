/**
 * consts file
 */
/* consts */
#define MEMORY_MAX_SIZE 8192 /* max memory size */
#define MEMORY_START_ADDRESS 100 /* memory start address */
#define MAX_LOG_SIZE 512 /* max log length*/
#define NO_OF_MACHINE_DIRECTIVES 16 /* number of op code */
#define NO_OF_REGISTERS 16 /* number of registers */
#define ERROR_WRAPPER "error in %s line %d: %s\n" /* error wrapper */
#define ERROR_WRAPPER_NO_LINE "error in %s: %s\n" /* error wrapper */

#define MAX_LINE_LENGTH 81 /* max line length */
#define MACRO_MAX_LINE_NUMBER 6 /* max line in macro */
#define MACRO_KEYWORD "macro" /* macro keyword */
#define EOF_MACRO_KEYWORD "endm" /* end of macro keyword */

#define DATA_CODE_DIRECTIVE ".data" /* data directive keyword */
#define STRING_CODE_DIRECTIVE ".string"  /* string directive keyword */
#define EXTERN_CODE_DIRECTIVE ".extern"  /* extern directive keyword */
#define ENTRY_CODE_DIRECTIVE ".entry"  /* entry directive keyword */


#define INT16_MAX 32767 /* max value number */
#define INT16_MIN -32768 /* max value number */

#define SYMBOL_MAX_LENGTH 31 /*max symbol name length */
