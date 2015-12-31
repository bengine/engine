#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "yaml_private.h"

int yaml_parser_init(yaml_parser_t *parser) {
	assert(parser); /* Non-NULL parser object expected. */
	memset(parser, 0, sizeof(yaml_parser_t));

	YAML_BUFFER_INIT(&parser->error, &parser->raw_buffer, yaml_byte_t, YAML_INPUT_RAW_BUFFER_SIZE);
	if (parser->error)
		goto ERROR;
	YAML_BUFFER_INIT(&parser->error, &parser->buffer, yaml_byte_t, YAML_INPUT_BUFFER_SIZE);
	if (parser->error)
		goto ERROR;
	YAML_QUEUE_INIT(&parser->error, &parser->tokens, yaml_token_t, YAML_INITIAL_QUEUE_SIZE);
	if (parser->error)
		goto ERROR;
	YAML_STACK_INIT(&parser->error, &parser->indents, int, YAML_INITIAL_STACK_SIZE);
	if (parser->error)
		goto ERROR;
	YAML_STACK_INIT(&parser->error, &parser->simple_keys, yaml_simple_key_t, YAML_INITIAL_STACK_SIZE);
	if (parser->error)
		goto ERROR;
	YAML_STACK_INIT(&parser->error, &parser->states, int, YAML_INITIAL_STACK_SIZE);
	if (parser->error)
		goto ERROR;
	YAML_STACK_INIT(&parser->error, &parser->marks, yaml_mark_t, YAML_INITIAL_STACK_SIZE);
	if (parser->error)
		goto ERROR;
	YAML_STACK_INIT(&parser->error, &parser->tag_directives, yaml_tag_directive_t, YAML_INITIAL_STACK_SIZE);
	if (parser->error)
		goto ERROR;

	return YAML_E_OK;

ERROR:
	YAML_BUFFER_DESTROY(&parser->raw_buffer);
	YAML_BUFFER_DESTROY(&parser->buffer);
	YAML_QUEUE_DESTROY(&parser->tokens);
	YAML_STACK_DESTROY(&parser->indents);
	YAML_STACK_DESTROY(&parser->simple_keys);
	YAML_STACK_DESTROY(&parser->states);
	YAML_STACK_DESTROY(&parser->marks);
	YAML_STACK_DESTROY(&parser->tag_directives);
	
	return YAML_E_FAILD;
}
