#ifndef BASE64_H_
#define BASE64_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h> /* For size_t */
#include <stdint.h> /* For uint8_t */

/** Encode 1 bytes into a 4 byte string.
 */
void base64_encode1(uint8_t *const dest, const uint8_t *const src);

/** Encode 2 bytes into a 4 byte string.
 */
void base64_encode2(uint8_t *const dest, const uint8_t *const src);

/** Encode 3 bytes into a 4 byte string.
 */
void base64_encode3(uint8_t *const dest, const uint8_t *const src);

/** Encode 'length' bytes from the input buffer and store the encoded stream into the output buffer.
 *
 * Return the length of the encoded buffer (output).
 * Note that the output buffer must be allocated by the caller (length * 1.5 should be a safe estimate).
 */
size_t base64_encode(uint8_t *const output, const uint8_t *const input, size_t inlen);

/** Decode 4 bytes into a 3 byte string.
 *
 * Returns the number of bytes actually decoded.
 */
size_t base64_decode3(uint8_t *const dest, const uint8_t *const src);

/** Decode bytes from the input buffer and store the decoded stream into the output buffer
 * until 'length' bytes have been decoded.
 *
 * Return the real length of the decoded stream (which should be equal to 'length').
 * Note that the output buffer must be allocated by the caller.
 */
size_t base64_decode(uint8_t *const output, const uint8_t *const input, size_t inlen);

#ifdef __cplusplus
}
#endif

#endif /* !BASE64_H_ */
