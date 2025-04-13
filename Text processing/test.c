#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rand_malloc.h"

int error = 0;

// Function to read a line dynamically
char* getLine() {
    int capacity = 32;  // Initial capacity
    int size = 0;
    char* line = malloc(capacity * sizeof(char));
    int c;

    if (line == NULL) {
        error = 1;
        return NULL;
    }

    while ((c = getchar()) != EOF) {
        if (c == '\n') {
            line[size] = '\0';  // Null-terminate the string
            return line;
        }

        line[size++] = c;
        if (size >= capacity) {
            capacity *= 2;
            char* new_line = realloc(line, capacity * sizeof(char));
            if (new_line == NULL) {
                fprintf(stderr, "Memory allocation error getLine()\n");
                free(line);
                error = 1;
                return NULL;
            }
            line = new_line;
        }
    }

    // EOF encountered after reading characters
    line[size] = '\0';
    return line;
}
 
int main() {
    printf("Enter lines of text\n");

    char* line;  // handles memory for a single line
    char** lines = NULL;  // handles array of lines
    char** temp_lines = NULL;
    int num_lines = 0;

    while ((line = getLine()) != NULL) {
        temp_lines = realloc(lines, (num_lines + 1) * sizeof(char*));
        if (temp_lines == NULL) {
            for (int i = 0; i < num_lines; i++){
                free(lines[i]);
            }
            free(lines);
            free(line);

            fprintf(stderr, "Memory allocation error main 1\n");
            exit(EXIT_FAILURE);
        }

        lines = temp_lines;
        lines[num_lines++] = line;
    }


    if (error == 1){
        for (int i = 0; i < num_lines; i++){
            free(lines[i]);
        }
        free(line);
        free(lines);
        fprintf(stderr, "Memory allocation error main 2\n");
        exit(EXIT_FAILURE);
    }

    printf("Reversed text:\n");

    for (int i = num_lines - 1; i >= 0; --i) {
        char* current_line = lines[i];
        char* token;
        char** words = NULL;
        int num_words = 0;

        // Line gets tokenized into words
        token = strtok(current_line, " ");
        while (token != NULL) {
            words = realloc(words, (num_words + 1) * sizeof(char*));
            if (words == NULL) {
                fprintf(stderr, "Memory allocation error main 3\n");
                free(line);
                free(lines);
                free(temp_lines);

                free(token);
                free(current_line);
                exit(EXIT_FAILURE);
            }
            words[num_words++] = token;
            token = strtok(NULL, " ");
        }

        // Print words in reverse order
        for (int j = num_words - 1; j >= 0; --j) {
            printf("%s", words[j]);
            if (j > 0) printf(" ");
        }
        printf("\n");

        // Free the tokenized words array and current line
        free(words);
        free(current_line);
    }

    free(lines);

    return 0;
}

/* Test functions

void test_malloc() {
    printf("Calling malloc multiple times:\n");

    for (int i = 0; i < 10; ++i) {
        int* array = malloc(5 * sizeof(int));
        if (array != NULL) {
            printf("Good allocation\n");
            free(array);
        } else {
            printf("Bad allocation, null pointer returned\n");
        }
    }
}

void test_realloc() {
    printf("Calling realloc multiple times:\n");

    int* array = NULL;
    for (int i = 0; i < 10; ++i) {
        int* new_array = realloc(array, (i + 1) * sizeof(int));
        if (new_array != NULL) {
            printf("Good allocation\n");
            array = new_array;
        } else {
            printf("Bad allocation, null pointer returned\n");
        }
    }
    free(array);
}

*/
