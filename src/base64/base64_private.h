#ifndef BASE64_PRIVATE_H_
#define BASE64_PRIVATE_H_

#include "base64.h"

void base64_encode1(base64_char_t *dest, const base64_char_t *src);
void base64_encode2(base64_char_t *dest, const base64_char_t *src);
void base64_encode3(base64_char_t *dest, const base64_char_t *src);
size_t base64_decode3(base64_char_t *dest, const base64_char_t *src);

#endif /* !BASE64_PRIVATE_H_ */