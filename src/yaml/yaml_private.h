#ifndef YAML_PRIVATE_H_
#define YAML_PRIVATE_H_

#include "yaml.h"

/* The size of the input raw buffer.
 */
#define YAML_INPUT_RAW_BUFFER_SIZE	16384

 /* The size of the input buffer.
  * It should be possible to decode the whole raw buffer.
  */
#define YAML_INPUT_BUFFER_SIZE	(YAML_INPUT_RAW_BUFFER_SIZE * 3)

  /* The size of the output buffer.
   */
#define YAML_OUTPUT_BUFFER_SIZE	16384

   /* The size of the output raw buffer.
	* It should be possible to encode the whole output buffer.
	*/
#define YAML_OUTPUT_RAW_BUFFER_SIZE	(YAML_OUTPUT_BUFFER_SIZE * 2 + 2)

	/* The size of other stacks and queues.
	 */
#define YAML_INITIAL_STACK_SIZE		16
#define YAML_INITIAL_QUEUE_SIZE		16
#define YAML_INITIAL_STRING_SIZE	16

	 /* Token initializers.
	  */

#define YAML_TOKEN_INIT(token, type, start_mark, end_mark) do { \
	(memset(token, 0, sizeof(yaml_token_t)); \
	(token)->type = (type); \
	(token)->start_mark = (start_mark); \
	(token)->end_mark = (end_mark); \
} while (0)

#define YAML_TOKEN_STREAM_START_INIT(token, encoding, start_mark, end_mark) do { \
    YAML_TOKEN_INIT((token), YAML_TOKEN_STREAM_START, (start_mark), (end_mark)); \
	(token)->data.stream_start.encoding = (encoding); \
} while (0)

#define YAML_TOKEN_STREAM_END_INIT(token, start_mark, end_mark) do { \
    YAML_TOKEN_INIT((token), YAML_TOKEN_STREAM_END, (start_mark), (end_mark)); \
} while (0)

#define YAML_TOKEN_ALIAS_INIT(token, value, start_mark, end_mark) do { \
    YAML_TOKEN_INIT((token), YAML_TOKEN_ALIAS, (start_mark), (end_mark)); \
	(token)->data.alias.value = (value); \
} while (0)

#define YAML_TOKEN_ANCHOR_INIT(token, value, start_mark, end_mark) do { \
	YAML_TOKEN_INIT((token), YAML_TOKEN_ANCHOR, (start_mark), (end_mark)); \
	(token)->data.anchor.value = (value); \
} while (0)

#define YAML_TOKEN_TAG_INIT(token, handle, suffix, start_mark, end_mark) do { \
	YAML_TOKEN_INIT((token), YAML_TOKEN_TAG, (start_mark), (end_mark)); \
	(token)->data.tag.handle = (handle); \
	(token)->data.tag.suffix = (suffix); \
} while (0)

#define YAML_TOKEN_SCALAR_INIT(token, value, length, style, start_mark, end_mark) do { \
	YAML_TOKEN_INIT((token), YAML_TOKEN_SCALAR, (start_mark), (end_mark)); \
	(token)->data.scalar.value = (value); \
    (token)->data.scalar.length = (length); \
	(token)->data.scalar.style = (style); \
} while (0)

#define YAML_TOKEN_VERSION_DIRECTIVE_INIT(token, major, minor, start_mark, end_mark) do { \
    YAML_TOKEN_INIT((token), YAML_TOKEN_VERSION_DIRECTIVE, (start_mark), (end_mark)); \
	(token)->data.version_directive.major = (major); \
	(token)->data.version_directive.minor = (minor); \
} while (0)

#define YAML_TOKEN_TAG_DIRECTIVE_INIT(token, handle, prefix, start_mark, end_mark) do { \
	YAML_TOKEN_INIT((token), YAML_TOKEN_TAG_DIRECTIVE, (start_mark), (end_mark)); \
	(token)->data.tag_directive.handle = (handle); \
	(token)->data.tag_directive.prefix = (prefix); \
} while (0)

	  /* Event initializers.
	   */

#define YAML_EVENT_INIT(event, type, start_mark, end_mark) do { \
	memset(&(event), 0, sizeof(yaml_event_t)); \
	(event)->type = (type); \
    (event)->start_mark = (start_mark); \
    (event)->end_mark = (end_mark); \
} while (0)

#define YAML_EVENT_STREAM_START_INIT(event, encoding, start_mark, end_mark) do { \
	YAML_EVENT_INIT((event), YAML_EVENT_STREAM_START, (start_mark), (end_mark)); \
	(event)->data.stream_start.encoding = (encoding); \
} while (0)

#define YAML_EVENT_STREAM_END_INIT(event, start_mark, end_mark) do { \
	YAML_EVENT_INIT((event), YAML_EVENT_STREAM_END, (start_mark), (end_mark)); \
} while (0)

#define YAML_EVENT_DOCUMENT_START_INIT(event, version_directive, tag_directives_start, \
										tag_directives_end, implicit, start_mark, end_mark) do { \
	YAML_EVENT_INIT((event), YAML_EVENT_DOCUMENT_START, (start_mark), (end_mark)); \
	(event)->data.document_start.version_directive = (version_directive); \
	(event)->data.document_start.tag_directives.start = (tag_directives_start); \
	(event)->data.document_start.tag_directives.end = (tag_directives_end); \
	(event)->data.document_start.implicit = (implicit); \
} while (0)

#define YAML_EVENT_DOCUMENT_END_INIT(event, implicit, start_mark, end_mark) do { \
	YAML_EVENT_INIT((event), YAML_EVENT_DOCUMENT_END, (start_mark), (end_mark)); \
	(event)->data.document_end.implicit = (implicit); \
} while (0)

#define YAML_EVENT_ALIAS_INIT(event, anchor, start_mark, end_mark) do { \
	YAML_EVENT_INIT((event), YAML_EVENT_ALIAS, (start_mark), (end_mark)); \
	(event)->data.alias.anchor = (anchor); \
} while (0)

#define YAML_EVENT_SCALAR_INIT(event, anchor, tag, value, length, plain_implicit, \
								quoted_implicit, style, start_mark, end_mark) do { \
	YAML_EVENT_INIT((event), YAML_EVENT_SCALAR, (start_mark), (end_mark)); \
	(event)->data.scalar.anchor = (anchor); \
	(event)->data.scalar.tag = (tag); \
	(event)->data.scalar.value = (value); \
	(event)->data.scalar.length = (length); \
	(event)->data.scalar.plain_implicit = (plain_implicit); \
	(event)->data.scalar.quoted_implicit = (quoted_implicit); \
	(event)->data.scalar.style = (style); \
} while (0)

#define YAML_EVENT_SEQUENCE_START_INIT(event, anchor, tag, implicit, style, start_mark, end_mark) do { \
	YAML_EVENT_INIT((event), YAML_EVENT_SEQUENCE_START, (start_mark), (end_mark)); \
	(event)->data.sequence_start.anchor = (anchor); \
	(event)->data.sequence_start.tag = (tag); \
	(event)->data.sequence_start.implicit = (implicit); \
	(event)->data.sequence_start.style = (style); \
} while (0)

#define YAML_EVENT_SEQUENCE_END_INIT(event, start_mark, end_mark) do { \
	YAML_EVENT_INIT((event), YAML_EVENT_SEQUENCE_END, (start_mark), (end_mark)); \
} while (0)

#define YAML_EVENT_MAPPING_START_INIT(event, anchor, tag, implicit, style, start_mark, end_mark) do { \
	YAML_EVENT_INIT((event), YAML_EVENT_MAPPING_START, (start_mark), (end_mark)); \
	(event)->data.mapping_start.anchor = (anchor); \
	(event)->data.mapping_start.tag = (tag); \
	(event)->data.mapping_start.implicit = (implicit); \
	(event)->data.mapping_start.style = (style); \
} while (0)

#define YAML_EVENT_MAPPING_END_INIT(event, start_mark, end_mark) do { \
	YAML_EVENT_INIT((event), YAML_EVENT_MAPPING_END, (start_mark), (end_mark)); \
} while (0)

#endif /* !YAML_PRIVATE_H_ */
