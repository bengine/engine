#ifndef YAML_PRIVATE_H_
#define YAML_PRIVATE_H_

#include <string.h>
#include "yaml.h"

extern yaml_t *_yaml;

#define YAML_INPUT_RAW_BUFFER_SIZE 16384 /* The size of the input raw buffer */

/* The size of the input buffer.
 It should be possible to decode the whole raw buffer.
 */
#define YAML_INPUT_BUFFER_SIZE (YAML_INPUT_RAW_BUFFER_SIZE * 3)

#define YAML_OUTPUT_BUFFER_SIZE 16384 /* The size of the output buffer */

 /* The size of the output raw buffer.
  It should be possible to encode the whole output buffer.
  */
#define YAML_OUTPUT_RAW_BUFFER_SIZE (YAML_OUTPUT_BUFFER_SIZE * 2 + 2)

  /* The size of other stacks and queues. */
#define YAML_INITIAL_STACK_SIZE 16
#define YAML_INITIAL_QUEUE_SIZE 16
#define YAML_INITIAL_STRING_SIZE 16

#define YAML_BUFFER_INIT(context, buffer, size) \
            (((buffer).start = _yaml->malloc(size)) ? \
                ((buffer).last = (buffer).pointer = (buffer).start, (buffer).end = (buffer).start + (size), YAML_ERRNO_OK) : \
                ((context)->error = YAML_ERRNO_MEMORY, YAML_ERRNO_FAIL))

#define YAML_BUFFER_DEL(context, buffer) \
            (_yaml->free((buffer).start), (buffer).start = (buffer).pointer = (buffer).end = NULL)

#define YAML_STACK_INIT(context, stack, size) \
            (((stack).start = _yaml->malloc((size) * sizeof(*(stack).start))) ? \
                ((stack).top = (stack).start, (stack).end = (stack).start + (size), YAML_ERRNO_OK) : \
                ((context)->error = YAML_ERRNO_MEMORY, YAML_ERRNO_FAIL))

#define YAML_STACK_DEL(context, stack) \
            (_yaml->free((stack).start), (stack).start = (stack).top = (stack).end = NULL)

#define YAML_STACK_EMPTY(context, stack) \
            ((stack).start == (stack).top)

#define YAML_STACK_LIMIT(context, stack, size) \
            ((stack).top - (stack).start < (size) ? \
                YAML_ERRNO_OK : \
                ((context)->error = YAML_ERRNO_MEMORY, YAML_ERRNO_FAIL))

#define YAML_STACK_PUSH(context, stack, value) \
            (((stack).top != (stack).end || yaml_stack_extend((void **)&(stack).start, (void **)&(stack).top, (void **)&(stack).end)) ? \
                (*((stack).top++) = value, YAML_ERRNO_OK) : \
                ((context)->error = YAML_ERRNO_MEMORY, YAML_ERRNO_FAIL))

#define YAML_STACK_POP(context, stack) \
            (*(--(stack).top))

#define YAML_QUEUE_INIT(context, queue, size) \
            (((queue).start = _yaml->malloc((size) * sizeof(*(queue).start))) ? \
                ((queue).head = (queue).tail = (queue).start, (queue).end = (queue).start + (size), YAML_ERRNO_OK) : \
                ((context)->error = YAML_ERRNO_MEMORY, YAML_ERRNO_FAIL))

#define YAML_QUEUE_DEL(context, queue) \
            (_yaml->free((queue).start), (queue).start = (queue).head = (queue).tail = (queue).end = NULL)

#define YAML_QUEUE_EMPTY(context, queue) \
                ((queue).head == (queue).tail)

#define YAML_QUEUE_ENQUEUE(context, queue, value) \
            (((queue).tail != (queue).end || yaml_queue_extend((void **)&(queue).start, (void **)&(queue).head, (void **)&(queue).tail, (void **)&(queue).end)) ? \
                (*((queue).tail++) = value, YAML_ERRNO_OK) : \
                ((context)->error = YAML_ERRNO_MEMORY, YAML_ERRNO_FAIL))

#define YAML_QUEUE_DEQUEUE(context, queue) \
            (*((queue).head++))

#define YAML_QUEUE_INSERT(context, queue, index, value) \
            (((queue).tail != (queue).end || yaml_queue_extend((void **)&(queue).start, (void **)&(queue).head, (void **)&(queue).tail, (void **)&(queue).end)) ? \
                (memmove((queue).head + (index) + 1, (queue).head + (index), ((queue).tail - (queue).head - (index)) * sizeof(*(queue).start)), *((queue).head + (index)) = value, (queue).tail++, YAML_ERRNO_OK) : \
                ((context)->error = YAML_ERRNO_MEMORY, YAML_ERRNO_FAIL))

#define YAML_NULL_STRING { NULL, NULL, NULL }

#define YAML_STRING(string, length)   { (string), (string) + (length), (string) }

#define YAML_STRING_ASSIGN(value, string, length) \
            ((value).start = (string), (value).end = (string) + (length), (value).pointer = (string))

#define YAML_STRING_INIT(context, string, size) \
            (((string).start = _yaml->malloc(size)) ? \
                ((string).pointer = (string).start, (string).end = (string).start+(size), memset((string).start, 0, (size)), YAML_ERRNO_OK) :                                                                   \
                ((context)->error = YAML_ERRNO_MEMORY, YAML_ERRNO_FAIL))

#define YAML_STRING_DESTROY(context, string) \
            (_yaml->free((string).start), (string).start = (string).pointer = (string).end = NULL)

#define YAML_STRING_EXTEND(context, string) \
            (((string).pointer + 5 < (string).end) || yaml_string_extend(&(string).start, &(string).pointer, &(string).end))

#define YAML_STRING_CLEAR(context, string) \
            ((string).pointer = (string).start, memset((string).start, 0, (string).end - (string).start))

#define YAML_STRING_JOIN(context, dst_string, src_string) \
            ((yaml_string_join(&(dst_string).start, &(dst_string).pointer, &(dst_string).end, &(src_string).start, &(src_string).pointer, &(src_string).end)) ? \
                ((src_string).pointer = (src_string).start, YAML_ERRNO_OK) : \
                ((context)->error = YAML_MEMORY_ERROR, YAML_ERRNO_FAIL))

/* Check the octet at the specified position */
#define YAML_STRING_CHECK_AT(string, octet, offset) \
            ((string).pointer[offset] == (yaml_char_t)(octet))

/* Check the current octet in the buffer */
#define YAML_STRING_CHECK(string, octet) YAML_STRING_CHECK_AT((string), (octet), 0)

/* Check if the character at the specified position is an alphabetical character, a digit, '_', or '-' */
#define YAML_STRING_IS_ALPHA_AT(string, offset) \
            (((string).pointer[offset] >= (yaml_char_t)'0' && (string).pointer[offset] <= (yaml_char_t)'9') || \
            ((string).pointer[offset] >= (yaml_char_t)'A' && (string).pointer[offset] <= (yaml_char_t)'Z') || \
            ((string).pointer[offset] >= (yaml_char_t)'a' && (string).pointer[offset] <= (yaml_char_t)'z') || \
            (string).pointer[offset] == '_' || (string).pointer[offset] == '-')

#define YAML_STRING_IS_ALPHA(string) YAML_STRING_IS_ALPHA_AT((string), 0)

/* Check if the character at the specified position is a digit */
#define YAML_STRING_IS_DIGIT_AT(string, offset) \
            (((string).pointer[offset] >= (yaml_char_t)'0' && (string).pointer[offset] <= (yaml_char_t)'9'))

#define YAML_STRING_IS_DIGIT(string)    YAML_STRING_IS_DIGIT_AT((string),0)


typedef struct {
    yaml_char_t *start;
    yaml_char_t *end;
    yaml_char_t *pointer;
} yaml_string_t;

yaml_errno_t yaml_string_extend(yaml_char_t **start, yaml_char_t **pointer, yaml_char_t **end);
yaml_errno_t yaml_string_join(yaml_char_t **dst_start, yaml_char_t **dst_pointer, yaml_char_t **dst_end,
                              yaml_char_t **src_start, yaml_char_t **src_pointer, yaml_char_t **src_end);
yaml_errno_t yaml_stack_extend(void **start, void **top, void **end);
yaml_errno_t yaml_queue_extend(void **start, void **head, void **tail, void **end);

void yaml_token_delete(yaml_token_t *token);

yaml_errno_t yaml_parser_init(yaml_parser_t *parser);
void yaml_parser_destroy(yaml_parser_t *parser);

yaml_errno_t yaml_parser_update_buffer(yaml_parser_t *parser, size_t length);
yaml_errno_t yaml_parser_fetch_more_tokens(yaml_parser_t *parser);

#endif /* !YAML_PRIVATE_H_ */
