#ifndef BASE64_H_
#define BASE64_H_

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stddef.h>

#if defined(BASE64_SHARED) /* Shared library */
# if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN32__)
#   if defined(BASE64_EXPORT)
#     define BASE64_DECL __declspec(dllexport)
#   else
#     define BASE64_DECL __declspec(dllimport)
#   endif
# endif
#else
# define BASE64_DECL
#endif

typedef unsigned char base64_char_t;

BASE64_DECL size_t base64_encode(base64_char_t *output, const base64_char_t *input, size_t input_len);
BASE64_DECL size_t base64_decode(base64_char_t *output, const base64_char_t *input, size_t input_len);

#endif /* !BASE64_H_ */
