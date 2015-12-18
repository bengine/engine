#include <stdlib.h>
#include <stdio.h>
#include "yaml.h"

int main(int argc, const char *const *argv) {
    int number;

    if (argc < 2) {
        printf("Usage: %s file1.yaml ...\n", argv[0]);
        return 0;
    }

    for (number = 1; number < argc; ++number) {
        FILE *file;
        yaml_t yaml;
        //yaml_parser_t parser;
        //yaml_document_t document;
        int done = 0;
        int count = 0;
        int error = 0;

        printf("[%d] Loading '%s': ", number, argv[number]);
        fflush(stdout);

        file = fopen(argv[number], "rb");
        
        /*yaml_parser_initialize(&parser);
        yaml_parser_set_input_file(&parser, file);*/

        /*while (!done)
        {
            if (!yaml_parser_load(&parser, &document)) {
                error = 1;
                break;
            }

            done = (!yaml_document_get_root_node(&document));

            yaml_document_delete(&document);

            if (!done) count ++;
        }

        yaml_parser_delete(&parser);

        assert(!fclose(file));

        printf("%s (%d documents)\n", (error ? "FAILURE" : "SUCCESS"), count);*/
    }

    return EXIT_SUCCESS;
}