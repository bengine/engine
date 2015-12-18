#ifndef YAML_H_
#define YAML_H_

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "export.h"

#define YAML_NULL_TAG       "tag:yaml.org,2002:null" /* The tag !!null with the only possible value: null */
#define YAML_BOOL_TAG       "tag:yaml.org,2002:bool" /* The tag !!bool with the values: true and falce */
#define YAML_STR_TAG        "tag:yaml.org,2002:str" /* The tag !!str for string values */
#define YAML_INT_TAG        "tag:yaml.org,2002:int" /* The tag !!int for integer values */
#define YAML_FLOAT_TAG      "tag:yaml.org,2002:float" /* The tag !!float for float values */
#define YAML_TIMESTAMP_TAG  "tag:yaml.org,2002:timestamp" /* The tag !!timestamp for date and time values */
#define YAML_SEQ_TAG        "tag:yaml.org,2002:seq" /* The tag !!seq is used to denote sequences */
#define YAML_MAP_TAG        "tag:yaml.org,2002:map" /* The tag !!map is used to denote mapping */

#define YAML_DEFAULT_SCALAR_TAG     YAML_STR_TAG /* The default scalar tag is !!str */
#define YAML_DEFAULT_SEQUENCE_TAG   YAML_SEQ_TAG /* The default sequence tag is !!seq */
#define YAML_DEFAULT_MAPPING_TAG    YAML_MAP_TAG /* The default mapping tag is !!map */

typedef enum {
    YAML_ERRNO_OK = 0, /* Succeeded */
    YAML_ERRNO_FAIL = -1, /* Fail */
    YAML_ERRNO_MEMORY = -2, /* Cannot allocate or reallocate a block of memory */
    YAML_ERRNO_READER = -3, /* Cannot read or decode the input stream */
    YAML_ERRNO_SCANNER = -4, /* Cannot scan the input stream */
    YAML_ERRNO_PARSER = -5, /* Cannot parse the input stream */
    YAML_ERRNO_COMPOSER = -6, /* Cannot compose a YAML document */
    YAML_ERRNO_WRITER = -7, /* Cannot write to the output stream */
    YAML_ERRNO_EMITTER = -8 /* Cannot emit a YAML stream */
} yaml_errno_t;

typedef enum {
    YAML_ANY_ENCODING, /* Let the parser choose the encoding */
    YAML_UTF8_ENCODING, /* The default UTF-8 encoding */
    YAML_UTF16LE_ENCODING, /* The UTF-16-LE encoding with BOM */
    YAML_UTF16BE_ENCODING /* The UTF-16-BE encoding with BOM */
} yaml_encoding_t;

typedef enum {
    YAML_ANY_BREAK,
    YAML_CR_BREAK, /* Use CR for line breaks (Mac style) */
    YAML_LN_BREAK, /* Use LN for line breaks (Unix style) */
    YAML_CRLN_BREAK /* Use CR LN for line breaks (DOS style) */
} yaml_break_t;

typedef enum {
    YAML_ANY_SCALAR_STYLE, /* Let the emitter choose the style */
    YAML_PLAIN_SCALAR_STYLE, /* The plain scalar style */
    YAML_SINGLE_QUOTED_SCALAR_STYLE, /* The single-quoted scalar style */
    YAML_DOUBLE_QUOTED_SCALAR_STYLE, /* The double-quoted scalar style */
    YAML_LITERAL_SCALAR_STYLE, /* The literal scalar style */
    YAML_FOLDED_SCALAR_STYLE /* The folded scalar style */
} yaml_scalar_style_t;

typedef enum {
    YAML_ANY_SEQUENCE_STYLE, /* Let the emitter choose the style */
    YAML_BLOCK_SEQUENCE_STYLE, /* The block sequence style */
    YAML_FLOW_SEQUENCE_STYLE /* The flow sequence style */
} yaml_sequence_style_t;

typedef enum {
    YAML_ANY_MAPPING_STYLE, /* Let the emitter choose the style */
    YAML_BLOCK_MAPPING_STYLE, /* The block mapping style */
    YAML_FLOW_MAPPING_STYLE /* The flow mapping style */
    /*YAML_FLOW_SET_MAPPING_STYLE*/
} yaml_mapping_style_t;

typedef enum {
    YAML_NO_NODE, /* An empty node */
    YAML_SCALAR_NODE, /* A scalar node */
    YAML_SEQUENCE_NODE, /* A sequence node */
    YAML_MAPPING_NODE /* A mapping node */
} yaml_node_type_t;

typedef enum {
    YAML_NO_TOKEN, /* An empty token */
    YAML_STREAM_START_TOKEN, /* A STREAM-START token */
    YAML_STREAM_END_TOKEN, /* A STREAM-END token */
    YAML_VERSION_DIRECTIVE_TOKEN, /* A VERSION-DIRECTIVE token */
    YAML_TAG_DIRECTIVE_TOKEN, /* A TAG-DIRECTIVE token */
    YAML_DOCUMENT_START_TOKEN, /* A DOCUMENT-START token */
    YAML_DOCUMENT_END_TOKEN, /* A DOCUMENT-END token */
    YAML_BLOCK_SEQUENCE_START_TOKEN, /* A BLOCK-SEQUENCE-START token */
    YAML_BLOCK_MAPPING_START_TOKEN, /* A BLOCK-SEQUENCE-END token */
    YAML_BLOCK_END_TOKEN, /* A BLOCK-END token */
    YAML_FLOW_SEQUENCE_START_TOKEN, /* A FLOW-SEQUENCE-START token */
    YAML_FLOW_SEQUENCE_END_TOKEN, /* A FLOW-SEQUENCE-END token */
    YAML_FLOW_MAPPING_START_TOKEN, /* A FLOW-MAPPING-START token */
    YAML_FLOW_MAPPING_END_TOKEN, /* A FLOW-MAPPING-END token */
    YAML_BLOCK_ENTRY_TOKEN, /* A BLOCK-ENTRY token */
    YAML_FLOW_ENTRY_TOKEN, /* A FLOW-ENTRY token */
    YAML_KEY_TOKEN, /* A KEY token */
    YAML_VALUE_TOKEN, /* A VALUE token */
    YAML_ALIAS_TOKEN, /* An ALIAS token */
    YAML_ANCHOR_TOKEN, /* An ANCHOR token */
    YAML_TAG_TOKEN, /* A TAG token */
    YAML_SCALAR_TOKEN /* A SCALAR token */
} yaml_token_type_t;

typedef enum {
    YAML_NO_EVENT, /* An empty event */
    YAML_STREAM_START_EVENT, /* A STREAM-START event */
    YAML_STREAM_END_EVENT, /* A STREAM-END event */
    YAML_DOCUMENT_START_EVENT, /* A DOCUMENT-START event */
    YAML_DOCUMENT_END_EVENT, /* A DOCUMENT-END event */
    YAML_ALIAS_EVENT, /* An ALIAS event */
    YAML_SCALAR_EVENT, /* A SCALAR event */
    YAML_SEQUENCE_START_EVENT, /* A SEQUENCE-START event */
    YAML_SEQUENCE_END_EVENT, /* A SEQUENCE-END event */
    YAML_MAPPING_START_EVENT, /* A MAPPING-START event */
    YAML_MAPPING_END_EVENT /* A MAPPING-END event */
} yaml_event_type_t;

typedef enum {
    YAML_PARSE_STREAM_START_STATE, /* Expect STREAM-START */
    YAML_PARSE_IMPLICIT_DOCUMENT_START_STATE, /* Expect the beginning of an implicit document */
    YAML_PARSE_DOCUMENT_START_STATE, /* Expect DOCUMENT-START */
    YAML_PARSE_DOCUMENT_CONTENT_STATE, /* Expect the content of a document */
    YAML_PARSE_DOCUMENT_END_STATE, /* Expect DOCUMENT-END */
    YAML_PARSE_BLOCK_NODE_STATE, /* Expect a block node */
    YAML_PARSE_BLOCK_NODE_OR_INDENTLESS_SEQUENCE_STATE, /* Expect a block node or indentless sequence */
    YAML_PARSE_FLOW_NODE_STATE, /* Expect a flow node */
    YAML_PARSE_BLOCK_SEQUENCE_FIRST_ENTRY_STATE, /* Expect the first entry of a block sequence */
    YAML_PARSE_BLOCK_SEQUENCE_ENTRY_STATE, /* Expect an entry of a block sequence */
    YAML_PARSE_INDENTLESS_SEQUENCE_ENTRY_STATE, /* Expect an entry of an indentless sequence */
    YAML_PARSE_BLOCK_MAPPING_FIRST_KEY_STATE, /* Expect the first key of a block mapping */
    YAML_PARSE_BLOCK_MAPPING_KEY_STATE, /* Expect a block mapping key */
    YAML_PARSE_BLOCK_MAPPING_VALUE_STATE, /* Expect a block mapping value */
    YAML_PARSE_FLOW_SEQUENCE_FIRST_ENTRY_STATE, /* Expect the first entry of a flow sequence */
    YAML_PARSE_FLOW_SEQUENCE_ENTRY_STATE, /* Expect an entry of a flow sequence */
    YAML_PARSE_FLOW_SEQUENCE_ENTRY_MAPPING_KEY_STATE, /* Expect a key of an ordered mapping */
    YAML_PARSE_FLOW_SEQUENCE_ENTRY_MAPPING_VALUE_STATE, /* Expect a value of an ordered mapping */
    YAML_PARSE_FLOW_SEQUENCE_ENTRY_MAPPING_END_STATE, /* Expect the and of an ordered mapping entry */
    YAML_PARSE_FLOW_MAPPING_FIRST_KEY_STATE, /* Expect the first key of a flow mapping */
    YAML_PARSE_FLOW_MAPPING_KEY_STATE, /* Expect a key of a flow mapping */
    YAML_PARSE_FLOW_MAPPING_VALUE_STATE, /* Expect a value of a flow mapping */
    YAML_PARSE_FLOW_MAPPING_EMPTY_VALUE_STATE, /* Expect an empty value of a flow mapping */
    YAML_PARSE_END_STATE /* Expect nothing */
} yaml_parser_state_t;

typedef enum {
    YAML_EMIT_STREAM_START_STATE, /* Expect STREAM-START */
    YAML_EMIT_FIRST_DOCUMENT_START_STATE, /* Expect the first DOCUMENT-START or STREAM-END */
    YAML_EMIT_DOCUMENT_START_STATE, /* Expect DOCUMENT-START or STREAM-END */
    YAML_EMIT_DOCUMENT_CONTENT_STATE, /* Expect the content of a document */
    YAML_EMIT_DOCUMENT_END_STATE, /* Expect DOCUMENT-END */
    YAML_EMIT_FLOW_SEQUENCE_FIRST_ITEM_STATE, /* Expect the first item of a flow sequence */
    YAML_EMIT_FLOW_SEQUENCE_ITEM_STATE, /* Expect an item of a flow sequence */
    YAML_EMIT_FLOW_MAPPING_FIRST_KEY_STATE, /* Expect the first key of a flow mapping */
    YAML_EMIT_FLOW_MAPPING_KEY_STATE, /* Expect a key of a flow mapping */
    YAML_EMIT_FLOW_MAPPING_SIMPLE_VALUE_STATE, /* Expect a value for a simple key of a flow mapping */
    YAML_EMIT_FLOW_MAPPING_VALUE_STATE, /* Expect a value of a flow mapping */
    YAML_EMIT_BLOCK_SEQUENCE_FIRST_ITEM_STATE, /* Expect the first item of a block sequence */
    YAML_EMIT_BLOCK_SEQUENCE_ITEM_STATE, /* Expect an item of a block sequence */
    YAML_EMIT_BLOCK_MAPPING_FIRST_KEY_STATE, /* Expect the first key of a block mapping */
    YAML_EMIT_BLOCK_MAPPING_KEY_STATE, /* Expect the key of a block mapping */
    YAML_EMIT_BLOCK_MAPPING_SIMPLE_VALUE_STATE, /* Expect a value for a simple key of a block mapping */
    YAML_EMIT_BLOCK_MAPPING_VALUE_STATE, /* Expect a value of a block mapping */
    YAML_EMIT_END_STATE /* Expect nothing */
} yaml_emitter_state_t;

typedef struct {
    int major, minor;
} yaml_version_directive_t;

typedef unsigned char yaml_char_t;

typedef struct {
    yaml_char_t *handle, *prefix;
} yaml_tag_directive_t;

typedef struct {
    size_t index; /* The position index */
    size_t line; /* The position line */
    size_t column; /* The position column */
} yaml_mark_t;

typedef struct {
    yaml_token_type_t type;
    yaml_mark_t start_mark; /* The beginning of the token */
    yaml_mark_t end_mark; /* The end of the token */
    union {
        struct {
            yaml_encoding_t encoding;
        } stream_start;
        struct {
            yaml_char_t *value;
        } alias;
        struct {
            yaml_char_t *value;
        } anchor;
        struct {
            yaml_char_t *handle, *suffix;
        } tag;
        struct {
            yaml_char_t *value;
            size_t length; /* The length of the scalar value */
            yaml_scalar_style_t style;
        } scalar;
        struct {
            int major, minor;
        } version_directive;
        struct {
            yaml_char_t *handle, *prefix;
        } tag_directive;
    } data;
} yaml_token_t;

typedef struct {
    yaml_event_type_t type;
    yaml_mark_t start_mark; /* The beginning of the event */
    yaml_mark_t end_mark; /* The end of the event */
    union {
        struct {
            yaml_encoding_t encoding;
        } stream_start;
        struct {
            yaml_version_directive_t *version_directive;
            struct {
                yaml_tag_directive_t *start; /* The beginning of the tag directives list */
                yaml_tag_directive_t *end; /* The end of the tag directives list */
            } tag_directives;
            int implicit; /* Is the document indicator implicit? */
        } document_start;
        struct {
            int implicit; /* Is the document end indicator implicit? */
        } document_end;
        struct {
            yaml_char_t *anchor;
        } alias;
        struct {
            yaml_char_t *anchor;
            yaml_char_t *tag;
            yaml_char_t *value;
            size_t length; /* The length of the scalar value */
            int plain_implicit; /* Is the tag optional for the plain style? */
            int quoted_implicit; /* Is the tag optional for any non-plain style? */
            yaml_scalar_style_t style;
        } scalar;
        struct {
            yaml_char_t *anchor;
            yaml_char_t *tag;
            int implicit; /* Is the tag optional? */
            yaml_sequence_style_t style;
        } sequence_start;
        struct {
            yaml_char_t *anchor;
            yaml_char_t *tag;
            int implicit; /* Is the tag optional? */
            yaml_mapping_style_t style;
        } mapping_start;
    } data;
} yaml_event_t;

typedef struct {
    int key, value;
} yaml_node_pair_t;

typedef int yaml_node_item_t;

typedef struct {
    yaml_node_type_t type;
    yaml_char_t *tag;
    yaml_mark_t start_mark; /* The beginning of the node */
    yaml_mark_t end_mark; /* The end of the node */
    union {
        struct {
            yaml_char_t *value;
            size_t length; /* The length of the scalar value */
            yaml_scalar_style_t style;
        } scalar;
        struct {
            struct {
                yaml_node_item_t *start; /* The beginning of the stack */
                yaml_node_item_t *end; /* The end of the stack */
                yaml_node_item_t *top; /* The top of the stack */
            } items;
            yaml_sequence_style_t style;
        } sequence;
        struct {
            struct {
                yaml_node_pair_t *start; /* The beginning of the stack */
                yaml_node_pair_t *end; /* The end of the stack */
                yaml_node_pair_t *top; /* The top of the stack */
            } pairs;
            yaml_mapping_style_t style;
        } mapping;
    } data;
} yaml_node_t;

typedef struct {
    yaml_version_directive_t *version_directive;
    int start_implicit; /* Is the document start indicator implicit? */
    int end_implicit; /* Is the document end indicator implicit? */
    yaml_mark_t start_mark; /* The beginning of the document */
    yaml_mark_t end_mark; /* The end of the document */
    struct {
        yaml_node_t *start; /* The beginning of the stack */
        yaml_node_t *end; /* The end of the stack */
        yaml_node_t *top; /* The top of the stack */
    } nodes;
    struct {
        yaml_tag_directive_t *start; /* The beginning of the tag directives list */
        yaml_tag_directive_t *end; /* The end of the tag directives list */
    } tag_directives;
} yaml_document_t;

typedef int yaml_read_handler_t(void *data, unsigned char *buffer, size_t size, size_t *size_read);

typedef struct {
    int possible; /* Is a simple key possible? */
    int required; /* Is a simple key required? */
    size_t token_number; /* The number of the token */
    yaml_mark_t mark; /* The position mark */
} yaml_simple_key_t;

typedef struct {
    yaml_char_t *anchor;
    int index; /* The node id */
    yaml_mark_t mark; /* The anchor mark */
} yaml_alias_data_t;

typedef struct {
    yaml_errno_t error;
    const char *problem; /* Error description */
    size_t problem_offset; /* The byte about which the problem occured */
    int problem_value; /* The problematic value (-1 is none) */
    yaml_mark_t problem_mark; /* The problem position */
    const char *context; /* The error context */
    yaml_mark_t context_mark; /* The context position */

    yaml_read_handler_t *read_handler;
    void *read_handler_data; /* A pointer for passing to the read handler */
    int eof;
    union {
        struct {
            const unsigned char *start; /* The string start pointer */
            const unsigned char *end; /* The string end pointer */
            const unsigned char *current; /* The string current position */
        } string;
        FILE *file;
    } input;
    struct {
        yaml_char_t *start; /* The beginning of the buffer */
        yaml_char_t *end; /* The end of the buffer */
        yaml_char_t *pointer; /* The current position of the buffer */
        yaml_char_t *last; /* The last filled position of the buffer */
    } buffer;
    size_t unread; /* The number of unread characters in the buffer */
    struct {
        unsigned char *start; /* The beginning of the buffer */
        unsigned char *end; /* The end of the buffer */
        unsigned char *pointer; /* The current position of the buffer */
        unsigned char *last; /* The last filled position of the buffer */
    } raw_buffer;
    yaml_encoding_t encoding;
    size_t offset; /* The offset of the current position (in bytes) */
    yaml_mark_t mark; /* The mark of the current position */

    int stream_start_produced; /* Have we started to scan the input stream? */
    int stream_end_produced; /* Have we reached the end of the input stream? */
    int flow_level; /* The number of unclosed '[' and '{' indicators. */
    struct {
        yaml_token_t *start; /* The beginning of the tokens queue */
        yaml_token_t *end; /* The end of the tokens queue */
        yaml_token_t *head; /* The head of the tokens queue */
        yaml_token_t *tail; /* The tail of the tokens queue */
    } tokens;
    size_t tokens_parsed; /* The number of tokens fetched from the queue */
    int token_available; /* Does the tokens queue contain a token ready for dequeueing */
    struct {
        int *start; /* The beginning of the stack */
        int *end; /* The end of the stack */
        int *top; /* The top of the stack */
    } indents;
    int indent; /* The current indentation level */
    int simple_key_allowed; /* May a simple key occur at the current position? */
    struct {
        yaml_simple_key_t *start; /* The beginning of the stack */
        yaml_simple_key_t *end; /* The end of the stack */
        yaml_simple_key_t *top; /* The top of the stack */
    } simple_keys;

    struct {
        yaml_parser_state_t *start; /* The beginning of the stack */
        yaml_parser_state_t *end; /* The end of the stack */
        yaml_parser_state_t *top; /* The top of the stack */
    } states;
    yaml_parser_state_t state; /* The current parser state */
    struct {
        yaml_mark_t *start; /* The beginning of the stack */
        yaml_mark_t *end; /* The end of the stack */
        yaml_mark_t *top; /* The top of the stack */
    } marks;
    struct {
        yaml_tag_directive_t *start; /* The beginning of the list */
        yaml_tag_directive_t *end; /* The end of the list */
        yaml_tag_directive_t *top; /* The top of the list */
    } tag_directives;

    struct {
        yaml_alias_data_t *start; /* The beginning of the list */
        yaml_alias_data_t *end; /* The end of the list */
        yaml_alias_data_t *top; /* The top of the list */
    } aliases;
    yaml_document_t *document; /* The currently parsed document */
} yaml_parser_t;

typedef int yaml_write_handler_t(void *data, unsigned char *buffer, size_t size);

typedef struct {
    yaml_errno_t error;
    const char *problem; /* Error description */

    yaml_write_handler_t *write_handler;
    void *write_handler_data; /* A pointer for passing to the white handler */
    union {
        struct {
            unsigned char *buffer; /* The buffer pointer */
            size_t size;
            size_t *size_written; /* The number of written bytes */
        } string;
        FILE *file;
    } output;
    struct {
        yaml_char_t *start; /* The beginning of the buffer */
        yaml_char_t *end; /* The end of the buffer */
        yaml_char_t *pointer; /* The current position of the buffer */
        yaml_char_t *last; /* The last filled position of the buffer */
    } buffer;
    struct {
        unsigned char *start; /* The beginning of the buffer */
        unsigned char *end; /* The end of the buffer */
        unsigned char *pointer; /* The current position of the buffer */
        unsigned char *last; /* The last filled position of the buffer */
    } raw_buffer;
    yaml_encoding_t encoding;

    int canonical; /* If the output is in the canonical style? */
    int best_indent; /* The number of indentation spaces. */
    int best_width; /* The preferred width of the output lines. */
    int unicode; /* Allow unescaped non-ASCII characters? */
    yaml_break_t line_break; /* The preferred line break. */
    struct {
        yaml_emitter_state_t *start; /* The beginning of the stack */
        yaml_emitter_state_t *end; /* The end of the stack */
        yaml_emitter_state_t *top; /* The top of the stack */
    } states;
    yaml_emitter_state_t state;
    struct {
        yaml_event_t *start; /* The beginning of the event queue. */
        yaml_event_t *end; /* The end of the event queue */
        yaml_event_t *head; /* The head of the event queue */
        yaml_event_t *tail; /* The tail of the event queue */
    } events;
    struct {
        int *start; /* The beginning of the stack */
        int *end; /* The end of the stack */
        int *top; /* The top of the stack */
    } indents;
    struct {
        yaml_tag_directive_t *start; /* The beginning of the list */
        yaml_tag_directive_t *end; /* The end of the list */
        yaml_tag_directive_t *top; /* The top of the list */
    } tag_directives;
    int indent; /* The current indentation level */
    int flow_level; /* The current flow level */
    int root_context; /* Is it the document root context? */
    int sequence_context; /* Is it a sequence context? */
    int mapping_context; /* Is it a mapping context? */
    int simple_key_context; /* Is it a simple mapping key context? */
    int line; /* The current line */
    int column; /* The current column */
    int whitespace; /* If the last character was a whitespace? */
    int indention; /* If the last character was an indentation character (' ', '-', '?', ':')? */
    int open_ended; /* If an explicit document end is required? */
    struct {
        yaml_char_t *anchor;
        size_t anchor_length;
        int alias; /* Is it an alias? */
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
        int multiline; /* Does the scalar contain line breaks? */
        int flow_plain_allowed; /* Can the scalar be expessed in the flow plain style? */
        int block_plain_allowed; /* Can the scalar be expressed in the block plain style? */
        int single_quoted_allowed; /* Can the scalar be expressed in the single quoted style? */
        int block_allowed; /* Can the scalar be expressed in the literal or folded styles? */
        yaml_scalar_style_t style;
    } scalar_data;

    int opened; /* If the stream was already opened? */
    int closed; /* If the stream was already closed? */
    struct {
        int references; /* The number of references */
        int anchor; /* The anchor id */
        int serialized; /* If the node has been emitted? */
    } *anchors;
    int last_anchor_id; /* The last assigned anchor id */
    yaml_document_t *document; /* The currently emitted document */
} yaml_emitter_t;

typedef void *(*yaml_malloc_t)(size_t size);
typedef void *(*yaml_realloc_t)(void *ptr, size_t size);
typedef void(*yaml_free_t)(void *ptr);

typedef struct {
    yaml_malloc_t malloc;
    yaml_realloc_t realloc;
    yaml_free_t free;

    yaml_parser_t *parser;
} yaml_t;

yaml_errno_t CORE_DECL yaml_init(yaml_t *yaml);
void CORE_DECL yaml_destroy();
void CORE_DECL yaml_set_input_file(yaml_t *yaml, FILE *file);

#endif /* !YAML_H_ */
