#ifndef YAML_H_
#define YAML_H_

#if defined(HAVE_CONFIG_H)
# include "config.h"
#endif

#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/**********************************************************************
 * PRE_DEFINE MACROS
 */

#if defined(YAML_SHARED) /* Shared library */
# if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN32__)
#   if defined(YAML_EXPORT)
#     define YAML_DECL __declspec(dllexport)
#   else
#     define YAML_DECL __declspec(dllimport)
#   endif
# endif
#else
# define YAML_DECL
#endif

#if !defined(YAML_MALLOC)
# define YAML_MALLOC(size) malloc(size)
#endif

#if !defined(YAML_REALLOC)
# define YAML_REALLOC(ptr, size) realloc(ptr, size)
#endif

#if !defined(YAML_FREE)
# define YAML_FREE(ptr) free(ptr)
#endif

/**********************************************************************
 * ERROR ID
 */

#define YAML_EOK	  	 0
#define YAML_EFAILD	-1
#define YAML_EMEMORY  	-2 /* Cannot allocate or reallocate a block of memory. */
#define YAML_EREADER  	-3 /* Cannot read or decode the input stream. */
#define YAML_ESCANNER 	-4 /* Cannot scan the input stream. */
#define YAML_EPARSER  	-5 /* Cannot parse the input stream. */
#define YAML_ECOMPOSER	-6 /* Cannot compose a YAML document. */
#define YAML_EWRITER  	-7 /* Cannot write to the output stream. */
#define YAML_EEMITTER 	-8 /* Cannot emit a YAML stream. */

/**********************************************************************
 * BUFFER STRUCT AND OPERATOR
 */

#define YAML_BUFFER_STRUCT(type) struct { type *start; type *end; type *pointer; type *last; }

#define YAML_BUFFER_INIT(ret, buffer, type, count) do { \
	assert((count) > 0); \
	\
	(buffer)->start = (type *)YAML_MALLOC((count) * sizeof(type)); \
	if (!(buffer)->start) \
		*(ret) = YAML_EMEMORY; \
	else { \
		(buffer)->last = (buffer)->pointer = (buffer)->start; \
		(buffer)->end = (buffer)->start + (count); \
		*(ret) = YAML_EOK; \
	} \
} while (0)

#define YAML_BUFFER_DESTROY(buffer) do { \
	YAML_FREE((buffer)->start); \
	(buffer)->start = (buffer)->pointer = (buffer)->end = NULL; \
} while (0)

/**********************************************************************
 * STACK STRUCT AND OPERATOR
 */

#define YAML_STACK_STRUCT(type) struct { type *start; type *end; type *top; }

#define YAML_STACK_INIT(ret, stack, type, count) do { \
	assert((count) > 0); \
	\
	(stack)->start = (type *)YAML_MALLOC((count) *sizeof(type)); \
	if (!(stack)->start) \
		*(ret) = YAML_EMEMORY; \
	else { \
		(stack)->top = (stack)->start; \
		(stack)->end = (stack)->start + (count); \
		*(ret) = YAML_EOK; \
	} \
} while (0)

#define YAML_STACK_DESTROY(stack) do { \
	YAML_FREE((stack)->start); \
	(stack)->start = (stack)->top = (stack)->end = NULL; \
} while (0)

#define YAML_STACK_LIMIT(ret, stack, count) do { \
	*(ret) = ((stack)->top - (stack)->start < (count)) ? YAML_EOK : YAML_EMEMORY; \
} while (0)

#define YAML_STACK_EXTEND(ret, stack, type) do { \
	size_t new_count = ((stack)->end - (stack)->start) * 2; \
	ptrdiff_t top_offset = (stack)->top - (stack)->start; \
	\
	type *new_start = (type *)YAML_REALLOC((stack)->start, new_count * sizeof(type)); \
	if (!new_start) \
		*(ret) = YAML_EMEMORY; \
	else { \
		(stack)->top = new_start + top_offset; \
		(stack)->end = new_start + new_count; \
		(stack)->start = new_start; \
		*(ret) = YAML_EOK; \
	} \
} while (0)

#define YAML_STACK_PUSH(ret, stack, type, value) do { \
	*(ret) = ((stack)->top == (stack)->end) ? YAML_EMEMORY : YAML_EOK; \
	if (*(ret)) \
		YAML_STACK_EXTEND(&ret, stack, type); \
	if (!*(ret)) \
		*((stack)->top++) = (value); \
} while (0)

#define YAML_STACK_POP(stack) (*(--(stack)->top))

/**********************************************************************
 * QUEUE STRUCT AND OPERATOR
 */

#define YAML_QUEUE_STRUCT(type) struct { type *start; type *end; type *head; type *tail; }

#define YAML_QUEUE_INIT(ret, queue, type, count) do { \
	assert((count) > 0); \
	\
	(queue)->start = (type *)YAML_MALLOC((count) * sizeof(type)); \
	if (!(queue)->start) \
		*(ret) = YAML_EMEMORY; \
	else { \
		(queue)->head = (queue)->tail = (queue)->start; \
		(queue)->end = (queue)->start + (count); \
		*(ret) = YAML_EOK; \
	} \
} while (0)

#define YAML_QUEUE_DESTROY(queue) do { \
	YAML_FREE((queue)->start); \
	(queue)->start = (queue)->head = (queue)->tail = (queue)->end = NULL; \
} while (0)

#define YAML_QUEUE_EMPTY(ret, queue) do { \
	*(ret) = ((queue)->head == (queue)->tail) ? YAML_EOK : YAML_EFAILD; \
} while (0)

#define YAML_QUEUE_EXTEND(ret, queue, type) do { \
	ptrdiff_t count = ((queue)->end - (queue)->start); \
	ptrdiff_t new_count = count * 2; \
	ptrdiff_t length = (queue)->tail - (queue)->head; \
	ptrdiff_t head_offset = (queue)->head - (queue)->start; \
	ptrdiff_t tail_offset = (queue)->tail - (queue)->start; \
	\
	/* Need resize the queue */ \
	if (length == count) { \
		type *new_start = (type *)YAML_REALLOC((queue)->start, new_count * sizeof(type)); \
		if (!new_start) \
			*(ret) = YAML_EMEMORY; \
		else { \
			(queue)->head = new_start + head_offset; \
			(queue)->tail = new_start + tail_offset; \
			(queue)->end = new_start + new_count; \
			(queue)->start = new_start; \
		} \
	} \
	\
	/* Move the queue at the beginning of the buffer */ \
	if (length < count) { \
		if (length > 0) \
			memmove((queue)->start, (queue)->head, length * sizeof(type)); \
		(queue)->tail = (queue)->start + length; \
		(queue)->head = (queue)->start; \
	} \
	*(ret) = YAML_EOK; \
} while (0)

#define YAML_QUEUE_ENQUEUE(ret, queue, type, value) do { \
	*(ret) = ((queue)->tail == (queue)->end) ? YAML_EMEMORY : YAML_EOK; \
	if (*(ret)) \
		YAML_QUEUE_EXTEND(&ret, queue, type); \
	if (!*(ret)) \
		*((queue)->tail++) = (value); \
} while (0)

#define YAML_QUEUE_DEQUEUE(queue) (*((queue)->head++))

#define YAML_QUEUE_INSERT(ret, queue, index, elem_type, value) do { \
	type *insert_pos = (queue)->head + (index); \
	ptrdiff_t right_len = (queue)->tail - (queue)->head - (index); \
	\
	*(ret) = ((queue)->tail == (queue)->end) ? YAML_EMEMORY : YAML_EOK; \
	if (*(ret)) \
		YAML_QUEUE_EXTEND(&ret, queue, type); \
	if (!*(ret)) { \
		memmove(insert_pos + 1, insert_pos, right_len * sizeof(type)); \
		*insert_pos = value; \
		(queue)->tail++; \
	} \
} while (0)

/**********************************************************************
 */

typedef unsigned char yaml_byte_t;
typedef unsigned char yaml_char_t;

typedef struct {
	size_t index;
	size_t line;
	size_t column;
} yaml_mark_t;

#define YAML_ENCODING_ANY		0 /* Let the parser choose the encoding. */
#define YAML_ENCODING_UTF8		1 /* The default UTF-8 encoding. */
#define YAML_ENCODING_UTF16LE	3 /* The UTF-16-LE encoding with BOM. */
#define YAML_ENCODING_UTF16BE	4 /* The UTF-16-BE encoding with BOM. */

#define YAML_BREAK_ANY		0 /* Let the parser choose the break type. */
#define YAML_BREAK_CR		1 /* Use CR for line breaks (Mac style). */
#define YAML_BREAK_LN		3 /* Use LN for line breaks (Unix style). */
#define YAML_BREAK_CRLN		4 /* Use CR LN for line breaks (DOS style). */

#define YAML_SCALAR_ANY				0 /* Let the emitter choose the style. */
#define YAML_SCALAR_PLAIN			1 /* The plain scalar style. */
#define YAML_SCALAR_SINGLE_QUOTED	2 /* The single-quoted scalar style. */
#define YAML_SCALAR_DOUBLE_QUOTED	3 /* The double-quoted scalar style. */
#define YAML_SCALAR_LITERAL			4 /* The literal scalar style. */
#define YAML_SCALAR_FOLDED			5 /* The folded scalar style. */

#define YAML_SEQUENCE_ANY			0 /* Let the emitter choose the style. */
#define YAML_SEQUENCE_BLOCK			1 /* The block sequence style. */
#define YAML_SEQUENCE_FLOW			2 /* The flow sequence style. */

#define YAML_MAPPING_ANY			0 /* Let the emitter choose the style. */
#define YAML_MAPPING_BLOCK			1 /* The block mapping style. */
#define YAML_MAPPING_FLOW			2 /* The flow mapping style. */

#define YAML_TOKEN_NO					0 /* An empty token. */
#define YAML_TOKEN_STREAM_START			1 /* A STREAM-START token. */
#define YAML_TOKEN_STREAM_END			2 /* A STREAM-END token. */
#define YAML_TOKEN_VERSION_DIRECTIVE	3 /* A VERSION-DIRECTIVE token. */
#define YAML_TOKEN_TAG_DIRECTIVE		4 /* A TAG-DIRECTIVE token. */
#define YAML_TOKEN_DOCUMENT_START		5 /* A DOCUMENT-START token. */
#define YAML_TOKEN_DOCUMENT_END			6 /* A DOCUMENT-END token. */
#define YAML_TOKEN_BLOCK_SEQUENCE_START	7 /* A BLOCK-SEQUENCE-START token. */
#define YAML_TOKEN_BLOCK_MAPPING_START	8 /* A BLOCK-SEQUENCE-END token. */
#define YAML_TOKEN_BLOCK_END			9 /* A BLOCK-END token. */
#define YAML_TOKEN_FLOW_SEQUENCE_START	10 /* A FLOW-SEQUENCE-START token. */
#define YAML_TOKEN_FLOW_SEQUENCE_END	11 /* A FLOW-SEQUENCE-END token. */
#define YAML_TOKEN_FLOW_MAPPING_START	12 /* A FLOW-MAPPING-START token. */
#define YAML_TOKEN_FLOW_MAPPING_END		13 /* A FLOW-MAPPING-END token. */
#define YAML_TOKEN_BLOCK_ENTRY			14 /* A BLOCK-ENTRY token. */
#define YAML_TOKEN_FLOW_ENTRY				15 /* A FLOW-ENTRY token. */
#define YAML_TOKEN_KEY					16 /* A KEY token. */
#define YAML_TOKEN_VALUE					17 /* A VALUE token. */
#define YAML_TOKEN_ALIAS					18 /* An ALIAS token. */
#define YAML_TOKEN_ANCHOR					19 /* An ANCHOR token. */
#define YAML_TOKEN_TAG					20 /* A TAG token. */
#define YAML_TOKEN_SCALAR					21 /* A SCALAR token. */

typedef struct {
	int type;
	union {
		struct {
			int encoding;
		} stream_start;
		struct {
			yaml_char_t *value;
		} alias;
		struct {
			yaml_char_t *value;
		} anchor;
		struct {
			yaml_char_t *handle;
			yaml_char_t *suffix;
		} tag;
		struct {
			yaml_char_t *value;
			size_t length;
			int style;
		} scalar;
		struct {
			int major;
			int minor;
		} version_directive;
		struct {
			yaml_char_t *handle;
			yaml_char_t *prefix;
		} tag_directive;
	} data;

	yaml_mark_t start_mark;
	yaml_mark_t end_mark;
} yaml_token_t;

#define YAML_EVENT_NO				0 /* An empty event. */
#define YAML_EVENT_STREAM_START		1 /* A STREAM-START event. */
#define YAML_EVENT_STREAM_END		2 /* A STREAM-END event. */
#define YAML_EVENT_DOCUMENT_START	3 /* A DOCUMENT-START event. */
#define YAML_EVENT_DOCUMENT_END		4 /* A DOCUMENT-END event. */
#define YAML_EVENT_ALIAS			5 /* An ALIAS event. */
#define YAML_EVENT_SCALAR			6 /* A SCALAR event. */
#define YAML_EVENT_SEQUENCE_START	7 /* A SEQUENCE-START event. */
#define YAML_EVENT_SEQUENCE_END		8 /* A SEQUENCE-END event. */
#define YAML_EVENT_MAPPING_START	9 /* A MAPPING-START event. */
#define YAML_EVENT_MAPPING_END		10 /* A MAPPING-END event. */

typedef struct {
	yaml_char_t *handle;
	yaml_char_t *prefix;
} yaml_tag_directive_t;

typedef struct {
	int type;
	union {
		struct {
			int encoding;
		} stream_start;
		struct {
			struct {
				int major;
				int minor;
			} version_directive;
			struct {
				yaml_tag_directive_t *start;
				yaml_tag_directive_t *end;
			} tag_directives;
			int implicit;
		} document_start;
		struct {
			int implicit;
		} document_end;
		struct {
			yaml_char_t *anchor;
		} alias;
		struct {
			yaml_char_t *anchor;
			yaml_char_t *tag;
			yaml_char_t *value;
			size_t length;
			int plain_implicit;
			int quoted_implicit;
			int style;
		} scalar;
		struct {
			yaml_char_t *anchor;
			yaml_char_t *tag;
			int implicit;
			int style;
		} sequence_start;
		struct {
			yaml_char_t *anchor;
			yaml_char_t *tag;
			int implicit;
			int style;
		} mapping_start;
	} data;

	yaml_mark_t start_mark;
	yaml_mark_t end_mark;
} yaml_event_t;

/* The tag !!null with the only possible value: null. */
#define YAML_TAG_NULL		"tag:yaml.org,2002:null"
/* The tag !!bool with the values: true and falce. */
#define YAML_TAG_BOOL       "tag:yaml.org,2002:bool"
/* The tag !!str for string values. */
#define YAML_TAG_STR        "tag:yaml.org,2002:str"
/* The tag !!int for integer values. */
#define YAML_TAG_INT        "tag:yaml.org,2002:int"
/* The tag !!float for float values. */
#define YAML_TAG_FLOAT      "tag:yaml.org,2002:float"
/* The tag !!timestamp for date and time values. */
#define YAML_TAG_TIMESTAMP  "tag:yaml.org,2002:timestamp"
/* The tag !!seq is used to denote sequences. */
#define YAML_TAG_SEQ        "tag:yaml.org,2002:seq"
/* The tag !!map is used to denote mapping. */
#define YAML_TAG_MAP        "tag:yaml.org,2002:map"

/* The default scalar tag is !!str. */
#define YAML_TAG_DEFAULT_SCALAR     YAML_TAG_STR
/* The default sequence tag is !!seq. */
#define YAML_TAG_DEFAULT_SEQUENCE   YAML_TAG_SEQ
/* The default mapping tag is !!map. */
#define YAML_TAG_DEFAULT_MAPPING    YAML_TAG_MAP

#define	YAML_NSTYLE_NO			0 /* An empty node style. */
#define YAML_NSTYLE_SCALAR		1 /* A scalar node style. */
#define YAML_NSTYLE_SEQUENCE	2 /* A sequence node style. */
#define YAML_NSTYLE_MAPPING		3 /* A mapping node style. */

typedef int yaml_node_item_t;

typedef struct {
	int key;
	int value;
} yaml_node_pair_t;

typedef struct {
	int type;
	yaml_char_t *tag;

	union {
		struct {
			yaml_char_t *value;
			size_t length;
			int style;
		} scalar;

		struct {
			YAML_STACK_STRUCT(yaml_node_item_t) items;
			int style;
		} sequence;

		struct {
			YAML_STACK_STRUCT(yaml_node_pair_t) pairs;
			int style;
		} mapping;
	} data;

	yaml_mark_t start_mark;
	yaml_mark_t end_mark;
} yaml_node_t;

typedef struct {
	struct {
		int major;
		int minor;
	} version_directive;
	struct {
		yaml_tag_directive_t *start;
		yaml_tag_directive_t *end;
	} tag_directives;

	YAML_STACK_STRUCT(yaml_node_t) nodes;
	
	int start_implicit;
	int end_implicit;

	yaml_mark_t start_mark;
	yaml_mark_t end_mark;
} yaml_document_t;

/* The prototype of a read handler.
*/
typedef int yaml_read_handler_t(void *data, unsigned char *buffer, size_t size, size_t *size_read);

typedef struct {
	int possible;
	int required;
	size_t token_number;
	yaml_mark_t mark;
} yaml_simple_key_t;

#define YAML_PST_STREAM_START							0 /* Expect STREAM-START. */
#define YAML_PST_IMPLICIT_DOCUMENT_START				1 /* Expect the beginning of an implicit document. */
#define YAML_PST_DOCUMENT_START							2 /* Expect DOCUMENT-START. */
#define YAML_PARSE_DOCUMENT_CONTENT						3 /* Expect the content of a document. */
#define YAML_PARSE_DOCUMENT_END							4 /* Expect DOCUMENT-END. */
#define YAML_PARSE_BLOCK_NODE							5 /* Expect a block node. */
#define YAML_PARSE_BLOCK_NODE_OR_INDENTLESS_SEQUENCE	6 /* Expect a block node or indentless sequence. */
#define YAML_PARSE_FLOW_NODE							7 /* Expect a flow node. */
#define YAML_PARSE_BLOCK_SEQUENCE_FIRST_ENTRY			9 /* Expect the first entry of a block sequence. */
#define YAML_PARSE_BLOCK_SEQUENCE_ENTRY					10 /* Expect an entry of a block sequence. */
#define YAML_PARSE_INDENTLESS_SEQUENCE_ENTRY			11 /* Expect an entry of an indentless sequence. */
#define YAML_PARSE_BLOCK_MAPPING_FIRST_KEY				12 /* Expect the first key of a block mapping. */
#define YAML_PARSE_BLOCK_MAPPING_KEY					13 /* Expect a block mapping key. */
#define YAML_PARSE_BLOCK_MAPPING_VALUE					14 /* Expect a block mapping value. */
#define YAML_PARSE_FLOW_SEQUENCE_FIRST_ENTRY			15 /* Expect the first entry of a flow sequence. */
#define YAML_PARSE_FLOW_SEQUENCE_ENTRY					16 /* Expect an entry of a flow sequence. */
#define YAML_PARSE_FLOW_SEQUENCE_ENTRY_MAPPING_KEY		17 /* Expect a key of an ordered mapping. */
#define YAML_PARSE_FLOW_SEQUENCE_ENTRY_MAPPING_VALUE	18 /* Expect a value of an ordered mapping. */
#define YAML_PARSE_FLOW_SEQUENCE_ENTRY_MAPPING_END		19 /* Expect the and of an ordered mapping entry. */
#define YAML_PARSE_FLOW_MAPPING_FIRST_KEY				20 /* Expect the first key of a flow mapping. */
#define YAML_PARSE_FLOW_MAPPING_KEY						21 /* Expect a key of a flow mapping. */
#define YAML_PARSE_FLOW_MAPPING_VALUE					22 /* Expect a value of a flow mapping. */
#define YAML_PARSE_FLOW_MAPPING_EMPTY_VALUE				23 /* Expect an empty value of a flow mapping. */
#define YAML_PARSE_END									24 /* Expect nothing. */

typedef struct {
	yaml_char_t *anchor;
	int index;
	yaml_mark_t mark;
} yaml_alias_data_t;

typedef struct {
	int error;
	const char *problem;
	size_t problem_offset;
	int problem_value;
	yaml_mark_t problem_mark;
	const char *context;
	yaml_mark_t context_mark;

	yaml_read_handler_t *read_handler;
	void *read_handler_data;
	union {
		struct {
			unsigned char *start;
			unsigned char *end;
			unsigned char *current;
		} string;

		FILE *file;
	} input;

	int eof;
	
	YAML_BUFFER_STRUCT(yaml_char_t) buffer;
	
	size_t unread;

	YAML_BUFFER_STRUCT(yaml_byte_t) raw_buffer;

	int encoding;
	size_t offset;
	yaml_mark_t mark;
	
	int stream_start_produced;
	int stream_end_produced;
	int flow_level;

	YAML_QUEUE_STRUCT(yaml_token_t) tokens;

	size_t tokens_parsed;
	int token_available;

	YAML_STACK_STRUCT(int) indents;

	int indent;
	int simple_key_allowed;

	YAML_STACK_STRUCT(yaml_simple_key_t) simple_keys;

	YAML_STACK_STRUCT(int) states;
	
	int state;

	YAML_STACK_STRUCT(yaml_mark_t) marks;

	YAML_STACK_STRUCT(yaml_tag_directive_t) tag_directives;

	YAML_STACK_STRUCT(yaml_alias_data_t) aliases;

	yaml_document_t *document;
} yaml_parser_t;

/* The prototype of a write handler. */
typedef int yaml_write_handler_t(void *data, unsigned char *buffer, size_t size);

#define YAML_ES_STREAM_START				0 /* Expect STREAM-START. */
#define YAML_ES_FIRST_DOCUMENT_START		1 /* Expect the first DOCUMENT-START or STREAM-END. */
#define YAML_ES_DOCUMENT_START				2 /* Expect DOCUMENT-START or STREAM-END. */
#define YAML_ES_DOCUMENT_CONTENT			3 /* Expect the content of a document. */
#define YAML_ES_DOCUMENT_END				4 /* Expect DOCUMENT-END. */
#define YAML_ES_FLOW_SEQUENCE_FIRST_ITEM	5 /* Expect the first item of a flow sequence. */
#define YAML_ES_FLOW_SEQUENCE_ITEM			6 /* Expect an item of a flow sequence. */
#define YAML_ES_FLOW_MAPPING_FIRST_KEY		7 /* Expect the first key of a flow mapping. */
#define YAML_ES_FLOW_MAPPING_KEY		 	8 /* Expect a key of a flow mapping. */
#define YAML_ES_FLOW_MAPPING_SIMPLE_VALUE	9 /* Expect a value for a simple key of a flow mapping. */
#define YAML_ES_FLOW_MAPPING_VALUE			10 /* Expect a value of a flow mapping. */
#define YAML_ES_BLOCK_SEQUENCE_FIRST_ITEM	11 /* Expect the first item of a block sequence. */
#define YAML_ES_BLOCK_SEQUENCE_ITEM			12 /* Expect an item of a block sequence. */
#define YAML_ES_BLOCK_MAPPING_FIRST_KEY		13 /* Expect the first key of a block mapping. */
#define YAML_ES_BLOCK_MAPPING_KEY			14 /* Expect the key of a block mapping. */
#define YAML_ES_BLOCK_MAPPING_SIMPLE_VALUE	15 /* Expect a value for a simple key of a block mapping. */
#define YAML_ES_BLOCK_MAPPING_VALUE			16 /* Expect a value of a block mapping. */
#define YAML_ES_END_STATE					17 /* Expect nothing. */

typedef struct {
	int error;
	const char *problem;
	
	yaml_write_handler_t *write_handler;
	void *write_handler_data;

	union {
		struct {
			unsigned char *buffer;
			size_t size;
			size_t *size_written;
		} string;

		FILE *file;
	} output;

	YAML_BUFFER_STRUCT(yaml_char_t) buffer;
	YAML_BUFFER_STRUCT(yaml_byte_t) raw_buffer;

	int encoding;
	int canonical;
	int best_indent;
	int best_width;
	int unicode;
	int line_break;

	YAML_STACK_STRUCT(int) states;

	int state;

	YAML_QUEUE_STRUCT(yaml_event_t) events;
	YAML_STACK_STRUCT(int) indents;
	YAML_STACK_STRUCT(yaml_tag_directive_t) tag_directives;

	int indent;
	int flow_level;
	int root_context;
	int sequence_context;
	int mapping_context;
	int simple_key_context;

	int line;
	int column;
	int whitespace;
	int indention;
	int open_ended;

	struct {
		yaml_char_t *anchor;
		size_t anchor_length;
		int alias;
	} anchor_data;

	struct {
		yaml_char_t *handle;
		size_t handle_length;
		yaml_char_t *suffix;
		size_t suffix_length;
	} tag_data;

	struct {
		yaml_char_t *value;
		size_t length;
		int multiline;
		int flow_plain_allowed;
		int block_plain_allowed;
		int single_quoted_allowed;
		int block_allowed;
		int style;
	} scalar_data;

	int opened;
	int closed;

	struct {
		int references;
		int anchor;
		int serialized;
	} *anchors;

	int last_anchor_id;
	yaml_document_t *document;
} yaml_emitter_t;

/* Free any memory allocated for a token object.
 */
YAML_DECL void yaml_token_destroy(yaml_token_t *token);

/* Create the STREAM-START event.
 */
YAML_DECL int yaml_stream_start_event_init(yaml_event_t *event, int encoding);

/* Create the STREAM-END event.
 */
YAML_DECL int yaml_stream_end_event_init(yaml_event_t *event);

/* Create the DOCUMENT-START event.
 * Returns YAML_NO_ERROR if the function succeeded.
 */
YAML_DECL int yaml_document_start_event_init(yaml_event_t *event,
											 int ma, int min,
											 yaml_tag_directive_t *tag_directives_start,
											 yaml_tag_directive_t *tag_directives_end,
											 int implicit);

/* Create the DOCUMENT-END event.
 */
YAML_DECL int yaml_document_end_event_init(yaml_event_t *event, int implicit);

/* Create an ALIAS event.
 */
YAML_DECL int yaml_alias_event_init(yaml_event_t *event, yaml_char_t *anchor);

/* Create a SCALAR event.
 */
YAML_DECL int yaml_scalar_event_init(yaml_event_t *event,
									 yaml_char_t *anchor, yaml_char_t *tag,
									 yaml_char_t *value, int length,
									 int plain_implicit, int quoted_implicit,
									 int style);

/* Create a SEQUENCE-START event.
 */
YAML_DECL int yaml_sequence_start_event_init(yaml_event_t *event,
											 yaml_char_t *anchor, yaml_char_t *tag, int implicit,
											 int style);

/* Create a SEQUENCE-END event.
 */
YAML_DECL int yaml_sequence_end_event_init(yaml_event_t *event);

/* Create a MAPPING-START event.
 */
YAML_DECL int yaml_mapping_start_event_init(yaml_event_t *event,
											yaml_char_t *anchor, yaml_char_t *tag, int implicit,
											int style);

/* Create a MAPPING-END event.
 */
YAML_DECL int yaml_mapping_end_event_init(yaml_event_t *event);

/* Free any memory allocated for an event object.
 */
YAML_DECL void yaml_event_destroy(yaml_event_t *event);

/* Create a YAML document.
 */
YAML_DECL int yaml_document_init(yaml_document_t *document, int major, int minor,
								 yaml_tag_directive_t *tag_directives_start,
								 yaml_tag_directive_t *tag_directives_end,
								 int start_implicit, int end_implicit);

/* Delete a YAML document and all its nodes.
 */
YAML_DECL void yaml_document_destroy(yaml_document_t *document);

/* Get a node of a YAML document.
 */
YAML_DECL yaml_node_t *yaml_document_get_node(yaml_document_t *document, int index);

/* Get the root of a YAML document node.
 */
YAML_DECL yaml_node_t *yaml_document_get_root_node(yaml_document_t *document);

/* Create a SCALAR node and attach it to the document.
 */
YAML_DECL int yaml_document_add_scalar(yaml_document_t *document,
									   yaml_char_t *tag, yaml_char_t *value, int length,
									   int style);

/* Create a SCALAR node and attach it to the document.
 */
YAML_DECL int yaml_document_add_scalar(yaml_document_t *document,
									   yaml_char_t *tag, yaml_char_t *value, int length,
									   int style);

/* Create a SEQUENCE node and attach it to the document.
 */
YAML_DECL int yaml_document_add_sequence(yaml_document_t *document, yaml_char_t *tag, int style);

/* Create a MAPPING node and attach it to the document.
 */
YAML_DECL int yaml_document_add_mapping(yaml_document_t *document, yaml_char_t *tag, int style);

/* Add an item to a SEQUENCE node.
 */
YAML_DECL int yaml_document_append_sequence_item(yaml_document_t *document, int sequence, int item);

/* Add a pair of a key and a value to a MAPPING node.
 */
YAML_DECL int yaml_document_append_mapping_pair(yaml_document_t *document, int mapping, int key, int value);

/* Initialize a parser.
 * Returns YAML_E_OK if the function succeeded.
 */
YAML_DECL int yaml_parser_init(yaml_parser_t *parser);

/* Destroy a parser.
 */
YAML_DECL void yaml_parser_destroy(yaml_parser_t *parser);

/* Set a string input.
 */
YAML_DECL void yaml_parser_set_input_string(yaml_parser_t *parser, const unsigned char *input, size_t size);

/* Set a file input.
 */
YAML_DECL void yaml_parser_set_input_file(yaml_parser_t *parser, FILE *file);

/* Set a generic input handler.
 */
YAML_DECL void yaml_parser_set_input(yaml_parser_t *parser, yaml_read_handler_t *handler, void *data);

/* Set the source encoding.
 */
YAML_DECL void yaml_parser_set_encoding(yaml_parser_t *parser, int encoding);

/* Scan the input stream and produce the next token.
 */
YAML_DECL int yaml_parser_scan(yaml_parser_t *parser, yaml_token_t *token);

/* Parse the input stream and produce the next parsing event.
 */
YAML_DECL int yaml_parser_parse(yaml_parser_t *parser, yaml_event_t *event);

/* Parse the input stream and produce the next YAML document.
 */
YAML_DECL int yaml_parser_load(yaml_parser_t *parser, yaml_document_t *document);

/* Initialize an emitter.
 */
YAML_DECL int yaml_emitter_init(yaml_emitter_t *emitter);

/* Destroy an emitter. */
YAML_DECL void yaml_emitter_destroy(yaml_emitter_t *emitter);

/* Set a string output.
 */
YAML_DECL void yaml_emitter_set_output_string(yaml_emitter_t *emitter,
											  unsigned char *output, size_t size, size_t *size_written);

/* Set a file output.
 */
YAML_DECL void yaml_emitter_set_output_file(yaml_emitter_t *emitter, FILE *file);

/* Set a generic output handler.
 */
YAML_DECL void yaml_emitter_set_output(yaml_emitter_t *emitter,
									   yaml_write_handler_t *handler, void *data);

/* Set the output encoding.
 */
YAML_DECL void yaml_emitter_set_encoding(yaml_emitter_t *emitter, int encoding);

/* Set if the output should be in the "canonical" format as in the YAML specification.
 */
YAML_DECL void yaml_emitter_set_canonical(yaml_emitter_t *emitter, int canonical);

/* Set the intendation increment.
 */
YAML_DECL void yaml_emitter_set_indent(yaml_emitter_t *emitter, int indent);

/* Set the preferred line width. -1 means unlimited.
 */
YAML_DECL void yaml_emitter_set_width(yaml_emitter_t *emitter, int width);

/* Set if unescaped non-ASCII characters are allowed.
 */
YAML_DECL void yaml_emitter_set_unicode(yaml_emitter_t *emitter, int unicode);

/* Set the preferred line break.
 */
YAML_DECL void yaml_emitter_set_break(yaml_emitter_t *emitter, int line_break);

/* Emit an event.
 */
YAML_DECL int yaml_emitter_emit(yaml_emitter_t *emitter, yaml_event_t *event);

/* Start a YAML stream.
 */
YAML_DECL int yaml_emitter_open(yaml_emitter_t *emitter);

/* Finish a YAML stream.
 */
YAML_DECL int yaml_emitter_close(yaml_emitter_t *emitter);

/* Emit a YAML document.
 */
YAML_DECL int yaml_emitter_dump(yaml_emitter_t *emitter, yaml_document_t *document);

/* Flush the accumulated characters to the output.
 */
YAML_DECL int yaml_emitter_flush(yaml_emitter_t *emitter);

#endif /* !YAML_H_ */
