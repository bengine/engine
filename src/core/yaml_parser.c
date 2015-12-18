#include <assert.h>
#include "yaml_private.h"

yaml_errno_t yaml_parser_init(yaml_parser_t *parser) {
    if (!parser) return YAML_ERRNO_FAIL;

    memset(parser, 0, sizeof(yaml_parser_t));

    if (YAML_BUFFER_INIT(parser, parser->raw_buffer, YAML_INPUT_RAW_BUFFER_SIZE))
        goto ERROR;
    if (YAML_BUFFER_INIT(parser, parser->buffer, YAML_INPUT_BUFFER_SIZE))
        goto ERROR;
    if (YAML_QUEUE_INIT(parser, parser->tokens, YAML_INITIAL_QUEUE_SIZE))
        goto ERROR;
    if (YAML_STACK_INIT(parser, parser->indents, YAML_INITIAL_STACK_SIZE))
        goto ERROR;
    if (YAML_STACK_INIT(parser, parser->simple_keys, YAML_INITIAL_STACK_SIZE))
        goto ERROR;
    if (YAML_STACK_INIT(parser, parser->states, YAML_INITIAL_STACK_SIZE))
        goto ERROR;
    if (YAML_STACK_INIT(parser, parser->marks, YAML_INITIAL_STACK_SIZE))
        goto ERROR;
    if (YAML_STACK_INIT(parser, parser->tag_directives, YAML_INITIAL_STACK_SIZE))
        goto ERROR;

    return YAML_ERRNO_OK;

ERROR:
    YAML_BUFFER_DEL(parser, parser->raw_buffer);
    YAML_BUFFER_DEL(parser, parser->buffer);
    YAML_QUEUE_DEL(parser, parser->tokens);
    YAML_STACK_DEL(parser, parser->indents);
    YAML_STACK_DEL(parser, parser->simple_keys);
    YAML_STACK_DEL(parser, parser->states);
    YAML_STACK_DEL(parser, parser->marks);
    YAML_STACK_DEL(parser, parser->tag_directives);

    return YAML_ERRNO_FAIL;
}

void yaml_parser_destroy(yaml_parser_t *parser) {
    assert(parser); /* Non-NULL parser object expected. */

    YAML_BUFFER_DEL(parser, parser->raw_buffer);
    YAML_BUFFER_DEL(parser, parser->buffer);
    while (!YAML_QUEUE_EMPTY(parser, parser->tokens))
        yaml_token_delete(&YAML_QUEUE_DEQUEUE(parser, parser->tokens));

    YAML_QUEUE_DEL(parser, parser->tokens);
    YAML_STACK_DEL(parser, parser->indents);
    YAML_STACK_DEL(parser, parser->simple_keys);
    YAML_STACK_DEL(parser, parser->states);
    YAML_STACK_DEL(parser, parser->marks);
    while (!YAML_STACK_EMPTY(parser, parser->tag_directives)) {
        yaml_tag_directive_t tag_directive = YAML_STACK_POP(parser, parser->tag_directives);
        _yaml->free(tag_directive.handle);
        _yaml->free(tag_directive.prefix);
    }
    YAML_STACK_DEL(parser, parser->tag_directives);

    //memset(parser, 0, sizeof(yaml_parser_t));
}
