#include <stdlib.h>
#include <assert.h>
#include "yaml_private.h"

yaml_t *_yaml = NULL;

yaml_errno_t yaml_init(yaml_t *yaml) {
    //if (!yaml) return YAML_ERRNO_FAIL;

    //_yaml = yaml;
    //if (!_yaml->malloc)
    //    _yaml->malloc = malloc;
    //if (!_yaml->realloc)
    //    _yaml->realloc = realloc;
    //if (!_yaml->free)
    //    _yaml->free = free;

    //_yaml->parser = (yaml_parser_t *)_yaml->malloc(sizeof(yaml_parser_t));
    //yaml_parser_init(_yaml->parser);

    return YAML_ERRNO_OK;
}

void yaml_destroy() {
    //yaml_parser_destroy(_yaml->parser);
    //_yaml->free(_yaml->parser);
    //_yaml = NULL;
}

void CORE_DECL yaml_set_input_file(yaml_t *yaml, FILE *file) {
}
