#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "yaml_private.h"

void yaml_token_destroy(yaml_token_t *token) {
	assert(token);

	switch (token->type) {
	case YAML_TOKEN_TAG_DIRECTIVE:
		YAML_FREE(token->data.tag_directive.handle);
		YAML_FREE(token->data.tag_directive.prefix);
		break;
	case YAML_TOKEN_ALIAS:
		YAML_FREE(token->data.alias.value);
		break;
	case YAML_TOKEN_ANCHOR:
		YAML_FREE(token->data.anchor.value);
		break;
	case YAML_TOKEN_TAG:
		YAML_FREE(token->data.tag.handle);
		YAML_FREE(token->data.tag.suffix);
		break;
	case YAML_TOKEN_SCALAR:
		YAML_FREE(token->data.scalar.value);
		break;
	default:
		break;
	}
	
	memset(token, 0, sizeof(yaml_token_t));
}

void yaml_event_destroy(yaml_event_t *event) {
	assert(event);

	yaml_tag_directive_t *tag_directive;
	switch (event->type) {
	case YAML_EVENT_DOCUMENT_START:
		for (tag_directive = event->data.document_start.tag_directives.start;
			tag_directive != event->data.document_start.tag_directives.end;
			tag_directive++) {
			YAML_FREE(tag_directive->handle);
			YAML_FREE(tag_directive->prefix);
		}
		YAML_FREE(event->data.document_start.tag_directives.start);
		break;
	case YAML_EVENT_ALIAS:
		YAML_FREE(event->data.alias.anchor);
		break;
	case YAML_EVENT_SCALAR:
		YAML_FREE(event->data.scalar.anchor);
		YAML_FREE(event->data.scalar.tag);
		YAML_FREE(event->data.scalar.value);
		break;
	case YAML_EVENT_SEQUENCE_START:
		YAML_FREE(event->data.sequence_start.anchor);
		YAML_FREE(event->data.sequence_start.tag);
		break;
	case YAML_EVENT_MAPPING_START:
		YAML_FREE(event->data.mapping_start.anchor);
		YAML_FREE(event->data.mapping_start.tag);
		break;
	default:
		break;
	}

	memset(event, 0, sizeof(yaml_event_t));
}

int yaml_stream_start_event_init(yaml_event_t *event, int encoding) {
	assert(event);
	yaml_mark_t mark = { 0, 0, 0 };
	YAML_EVENT_STREAM_START_INIT(event, encoding, mark, mark);
	return 1;
}

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

	return YAML_EOK;

ERROR:
	YAML_BUFFER_DESTROY(&parser->raw_buffer);
	YAML_BUFFER_DESTROY(&parser->buffer);
	YAML_QUEUE_DESTROY(&parser->tokens);
	YAML_STACK_DESTROY(&parser->indents);
	YAML_STACK_DESTROY(&parser->simple_keys);
	YAML_STACK_DESTROY(&parser->states);
	YAML_STACK_DESTROY(&parser->marks);
	YAML_STACK_DESTROY(&parser->tag_directives);
	
	return YAML_EFAILD;
}

/* File read handler.
 */
static int yaml_file_read_handler(void *data, yaml_byte_t *buffer, size_t count, size_t *size_read) {
	yaml_parser_t *parser = (yaml_parser_t *)data;
	
	*size_read = fread(buffer, sizeof(yaml_byte_t), count, parser->input.file);
	return ferror(parser->input.file) ? YAML_EFAILD : YAML_EOK;
}

void yaml_parser_set_input_file(yaml_parser_t *parser, FILE *file) {
	assert(parser && !parser->read_handler && file);

	parser->read_handler = yaml_file_read_handler;
	parser->read_handler_data = parser;
	parser->input.file = file;
}

//yaml_stream_start_event_init(yaml_event_t *event, int encoding) {
//	yaml_mark_t mark = { 0, 0, 0 };
//
//	assert(event);  /* Non-NULL event object is expected. */
//
//	STREAM_START_EVENT_INIT(*event, encoding, mark, mark);
//
//	return 1;
//}