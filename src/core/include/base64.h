#ifndef BASE64_H_
#define BASE64_H_

#include <stddef.h>
#include "export.h"

typedef unsigned char base64_char_t;

void CORE_DECL base64_encode1(base64_char_t *const dest, const base64_char_t *const src);
void CORE_DECL base64_encode2(base64_char_t *const dest, const base64_char_t *const src);
void CORE_DECL base64_encode3(base64_char_t *const dest, const base64_char_t *const src);
size_t CORE_DECL base64_encode(base64_char_t *const output, const base64_char_t *const input, size_t inlen);
size_t CORE_DECL base64_decode3(base64_char_t *const dest, const base64_char_t *const src);
size_t CORE_DECL base64_decode(base64_char_t *const output, const base64_char_t *const input, size_t inlen);

#endif /* !BASE64_H_ */
