#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define MAX_LINE_LENGTH 80
#define MACRO_MAX_LINE_NUMBER 6


int read_file_with_extension(char *fileName, char *ext, FILE *file_to_read) {
    char *full_file_name = (char *) malloc(strlen(fileName) + strlen(ext));
    strcpy(full_file_name, fileName);
    strcat(full_file_name, ext);
    file_to_read = fopen(full_file_name, "r");
    if (file_to_read == NULL) {
        fprintf(stderr, "couldn't read file %s, please make sure the file exist, skipping.\n", full_file_name);
        free(full_file_name);
        return 0;
    }
    free(full_file_name);
    return 1;
}
void pre_assembler(FILE *file_to_read) {
    char line[MAX_LINE_LENGTH];
    while (fgets(line , MAX_LINE_LENGTH, file_to_read)){
        printf("%s",line);
    }
}
int test(void) {
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            printf("%s\n", dir->d_name);
        }
        closedir(d);
    }
    return(0);
}

int main(int argc, char *argv[]) {
    test();
    FILE *file_to_read;
    int status;
    int i;
    for (i = 1; i < argc; ++i) {
        status = read_file_with_extension(argv[i], ".as", file_to_read);
        if (status) {
            printf("file exist");
            pre_assembler(file_to_read);
            fclose(file_to_read);
        }
    }
    return 0;
}


