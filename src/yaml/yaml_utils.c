#include "yaml_private.h"

yaml_errno_t yaml_stack_extend(void **start, void **top, void **end) {
    size_t size = (char *)*end - (char *)*start;
    size_t offset = (char *)*top - (char *)*start;

    void *new_start = _yaml->realloc(*start, size * 2);

    if (!new_start) return YAML_ERRNO_MEMORY;

    *top = (char *)new_start + offset;
    *end = (char *)new_start + size * 2;
    *start = new_start;

    return YAML_ERRNO_OK;
}

yaml_errno_t yaml_queue_extend(void **start, void **head, void **tail, void **end) {
    size_t size = (char *)*end - (char *)*start;
    size_t offset = (char *)*head - (char *)*start;
    size_t toffset = (char *)*tail - (char *)*start;

    /* Check if we need to resize the queue. */
    if (*start == *head && *tail == *end) {
        void *new_start = _yaml->realloc(*start, size * 2);

        if (!new_start) return YAML_ERRNO_MEMORY;

        *head = (char *)new_start + offset;
        *tail = (char *)new_start + toffset;
        *end = (char *)new_start + size * 2;
        *start = new_start;
    }

    /* Check if we need to move the queue at the beginning of the buffer. */
    if (*tail == *end) {
        if (*head != *tail)
            memmove(*start, *head, (char *)*tail - (char *)*head);

        *tail = (char *)*tail - (char *)*head + (char *)*start;
        *head = *start;
    }

    return YAML_ERRNO_OK;
}

//yaml_string_t *yaml_strdup(const yaml_string_t *str) {
//    if (!str) return NULL;
//
//    yaml_string_t *new_str = YAML_MALLOC(sizeof(yaml_string_t));
//    size_t len = YAML_STRLEN(str);
//
//    new_str->start = YAML_MALLOC(len);
//    new_str->end = new_str->pointer = new_str->start + len;
//
//    return new_str;
//}