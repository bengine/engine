#include <assert.h>
#include "yaml_private.h"

int yaml_parser_init(yaml_parser_t *parser) {
	assert(parser);     /* Non-NULL parser object expected. */

	memset(parser, 0, sizeof(yaml_parser_t));
	if (!BUFFER_INIT(parser, parser->raw_buffer, INPUT_RAW_BUFFER_SIZE))
		goto error;
	if (!BUFFER_INIT(parser, parser->buffer, INPUT_BUFFER_SIZE))
		goto error;
	if (!QUEUE_INIT(parser, parser->tokens, INITIAL_QUEUE_SIZE))
		goto error;
	if (!STACK_INIT(parser, parser->indents, INITIAL_STACK_SIZE))
		goto error;
	if (!STACK_INIT(parser, parser->simple_keys, INITIAL_STACK_SIZE))
		goto error;
	if (!STACK_INIT(parser, parser->states, INITIAL_STACK_SIZE))
		goto error;
	if (!STACK_INIT(parser, parser->marks, INITIAL_STACK_SIZE))
		goto error;
	if (!STACK_INIT(parser, parser->tag_directives, INITIAL_STACK_SIZE))
		goto error;

	return 1;

error:

	BUFFER_DEL(parser, parser->raw_buffer);
	BUFFER_DEL(parser, parser->buffer);
	QUEUE_DEL(parser, parser->tokens);
	STACK_DEL(parser, parser->indents);
	STACK_DEL(parser, parser->simple_keys);
	STACK_DEL(parser, parser->states);
	STACK_DEL(parser, parser->marks);
	STACK_DEL(parser, parser->tag_directives);

	return 0;
}
