#include "base64.h"

static const uint8_t _encode_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const uint8_t _decode_table[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0x3E, 0xFF, 0xFF, 0xFF, 0x3F,
    0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B,
    0x3C, 0x3D, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF,
    0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
    0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
    0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20,
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30,
    0x31, 0x32, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,

    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

static uint8_t _placeholder = '=';

static uint8_t _encode(uint8_t byte)
{
    return _encode_table[byte];
}

static uint8_t _decode(uint8_t byte)
{
    return _decode_table[byte];
}

void base64_encode1(uint8_t *const dest, const uint8_t *const src)
{
    dest[0] = _encode((src[0] >> 2) & 0x3F);
    dest[1] = _encode(((src[0] << 4) & 0x30));
    dest[2] = _placeholder;
    dest[3] = _placeholder;
}

void base64_encode2(uint8_t *const dest, const uint8_t *const src)
{
    dest[0] = _encode((src[0] >> 2) & 0x3F);
    dest[1] = _encode(((src[0] << 4) & 0x30) | ((src[1] >> 4) & 0x0F));
    dest[2] = _encode(((src[1] << 2) & 0x3C));
    dest[3] = _placeholder;
}

void base64_encode3(uint8_t *const dest, const uint8_t *const src)
{
    dest[0] = _encode((src[0] >> 2) & 0x3F);
    dest[1] = _encode(((src[0] << 4) & 0x30) | ((src[1] >> 4) & 0x0F));
    dest[2] = _encode(((src[1] << 2) & 0x3C) | ((src[2] >> 6) & 0x03));
    dest[3] = _encode(src[2] & 0x3F);
}

size_t base64_encode(uint8_t *const output, const uint8_t *const input, size_t inlen)
{
    size_t floor = (inlen / 3) * 3, len = inlen - floor;
    const uint8_t *ptr = input;
    const uint8_t *const end = input + floor;
    uint8_t *optr = output;

    /* encode complete triplet */
    for (; ptr < end; ptr += 3) {
        base64_encode3(optr, ptr);
        optr += 4;
    }

    /* encodes a 2-byte ending into 3 bytes and 1 pad byte and writes */
    if (len == 2) {
        base64_encode2(optr, ptr);
        optr += 4;
        return optr - output;
    }

    /* encodes a 1-byte ending into 2 bytes and 2 pad bytes */
    if (len == 1) {
        base64_encode1(optr, ptr);
        optr += 4;
        return optr - output;
    }

    return optr - output;
}

size_t base64_decode3(uint8_t *const dest, const uint8_t *const src)
{
    uint8_t b0 = _decode(src[0]);
    uint8_t b1 = _decode(src[1]);
    uint8_t b2 = _decode(src[2]);
    uint8_t b3 = _decode(src[3]);

    /* make sure all characters were valid */
    if (b0 == 0xFF || b1 == 0xFF || b2 == 0xFF || b3 == 0xFF)
        return 0;

    /* decode and return the number of bytes actually decoded */
    dest[0] = ((b0 << 2) & 0xFC) | ((b1 >> 4) & 0x03);
    if (src[2] == _placeholder)
        return 1;

    dest[1] = ((b1 << 4) & 0xF0) | ((b2 >> 2) & 0x0F);
    if (src[3] == _placeholder)
        return 2;

    dest[2] = ((b2 << 6) & 0xC0) | ((b3 >> 0) & 0x3F);
    return 3;
}

size_t base64_decode(uint8_t *const output, const uint8_t *const input, size_t inlen)
{
    const uint8_t *const end = input + inlen;
    uint8_t *optr = output;
    const uint8_t *ptr = input;

    /* decode complete triplet */
    for (; ptr < end; ptr += 4)
        optr += base64_decode3(optr, ptr);
    return optr - output;
}