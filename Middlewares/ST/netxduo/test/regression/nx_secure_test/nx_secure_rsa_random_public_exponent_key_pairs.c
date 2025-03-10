#ifndef NX_CRYPTO_STANDALONE_ENABLE
#include "nx_api.h"
#else
#include "nx_crypto.h"
#endif

/* 2048 bits rsa key pair. */
/* public exponent */
static UCHAR pub_e_2048_0[] = {0x67, 0x25, 0x3b};

/* private exponent */
static UCHAR pri_e_2048_0[] =
{
    0x22, 0x3e, 0xf2, 0x97, 0x2f, 0x57, 0xfc, 0x6e, 0x35, 0x58, 0x7a, 0x3a, 0xbf, 0x4b, 0x48,
    0x70, 0xf4, 0x0f, 0x47, 0xe5, 0x35, 0xf2, 0x22, 0x16, 0x9a, 0x68, 0x9a, 0x38, 0xf4, 0x80,
    0xe6, 0x6f, 0x61, 0x63, 0x41, 0xde, 0xf9, 0x77, 0xee, 0xac, 0xa3, 0xd2, 0xc6, 0x84, 0x90,
    0x2c, 0xf5, 0xe9, 0x44, 0x54, 0xfb, 0x73, 0x0b, 0xaf, 0x34, 0x72, 0xe1, 0x3f, 0x8c, 0xa7,
    0x66, 0x42, 0x45, 0x0c, 0xc9, 0x1c, 0xa0, 0x72, 0x25, 0x97, 0x3f, 0x7a, 0xed, 0x59, 0xf7,
    0x15, 0x63, 0x31, 0x2e, 0xf1, 0xe9, 0x04, 0xb2, 0xbf, 0xe0, 0x71, 0xca, 0x20, 0x70, 0xd9,
    0x21, 0x49, 0x5f, 0xf2, 0xc2, 0x5a, 0xcc, 0x2c, 0x0d, 0xe0, 0xe1, 0xf0, 0xc4, 0xb0, 0xfb,
    0x4e, 0xda, 0x28, 0x92, 0x16, 0x36, 0x3d, 0x4d, 0x34, 0xbe, 0x86, 0x84, 0xdc, 0x67, 0x66,
    0x6f, 0x1d, 0x85, 0x99, 0x10, 0x0a, 0x29, 0x18, 0xe2, 0x34, 0x23, 0xf5, 0xad, 0x3b, 0x3d,
    0x01, 0x04, 0x9b, 0x85, 0x91, 0xb6, 0xc7, 0x9a, 0x54, 0x5b, 0xbc, 0x86, 0xd9, 0xf0, 0x5c,
    0xc6, 0x96, 0x73, 0xdb, 0x48, 0xbe, 0xea, 0xcd, 0x15, 0xe9, 0x37, 0xea, 0x46, 0x26, 0xf3,
    0x3a, 0x2b, 0x70, 0x0e, 0x65, 0xce, 0xc2, 0x8d, 0xef, 0x00, 0xe5, 0x81, 0x52, 0xf5, 0xff,
    0x5f, 0xa3, 0xdc, 0x0b, 0xcc, 0x9d, 0xc6, 0x74, 0x5c, 0xb6, 0xb3, 0xef, 0xa9, 0x06, 0x37,
    0xe8, 0x44, 0x26, 0xcb, 0x0d, 0x91, 0x1a, 0x5c, 0xd0, 0x78, 0x84, 0x61, 0x65, 0x97, 0xf5,
    0x96, 0xdd, 0xa6, 0x92, 0xc6, 0xa8, 0x6a, 0x30, 0x76, 0xfb, 0x61, 0x3d, 0x56, 0x44, 0xbf,
    0x39, 0xfb, 0xf7, 0xfb, 0x2a, 0xbb, 0x5d, 0xf0, 0x94, 0x89, 0xba, 0x2f, 0x88, 0x79, 0x85,
    0x6c, 0x8d, 0x4f, 0xce, 0xa3, 0x3a, 0xc5, 0x21, 0xb1, 0xe1, 0xab, 0x3d, 0xba, 0x59, 0x62,
    0xc3
};

/* modulus */
static UCHAR m_2048_0[] =
{
    0xab, 0xca, 0x27, 0x6f, 0xfc, 0xc7, 0x78, 0x9d, 0xb4, 0x02, 0x3b, 0x7b, 0x3c, 0xf1,
    0x66, 0xe4, 0x26, 0x02, 0x49, 0xf2, 0x73, 0x7c, 0x2d, 0xfd, 0x3e, 0x7f, 0xa5, 0xa1, 0x15,
    0x2f, 0xae, 0x10, 0x95, 0x3b, 0x8d, 0x28, 0xa3, 0x01, 0xeb, 0xe9, 0x67, 0x10, 0xef, 0x27,
    0x25, 0x9c, 0x38, 0x60, 0x16, 0x4e, 0xfc, 0x10, 0xce, 0x86, 0x25, 0xeb, 0xea, 0x05, 0x89,
    0x1b, 0xdb, 0xf5, 0xab, 0x8d, 0xda, 0x23, 0x5f, 0xd9, 0xcc, 0x01, 0x99, 0xe8, 0xf2, 0x21,
    0x0d, 0xe6, 0xec, 0xb9, 0x75, 0x1b, 0xdf, 0x48, 0x00, 0x9f, 0x0b, 0x40, 0xe6, 0x06, 0x85,
    0xa7, 0xd7, 0xdf, 0x5b, 0xff, 0x8a, 0x93, 0xb7, 0x72, 0x65, 0x30, 0xfd, 0xe8, 0x3f, 0xff,
    0x61, 0x8b, 0x00, 0x9b, 0x23, 0x56, 0xf3, 0x40, 0xfb, 0x47, 0x80, 0x4f, 0x7b, 0x91, 0x41,
    0x30, 0xdd, 0x9f, 0xa3, 0xaf, 0x2e, 0x13, 0xed, 0xf4, 0x36, 0x1a, 0xba, 0x5d, 0x97, 0xbb,
    0x38, 0xcb, 0x72, 0xf0, 0xdf, 0xec, 0x1b, 0x24, 0x6c, 0x8d, 0xe2, 0x3e, 0x8c, 0x1e, 0x23,
    0x73, 0x74, 0x03, 0x7a, 0xfa, 0x83, 0xad, 0x32, 0x78, 0x99, 0x12, 0xcc, 0xd2, 0xd3, 0xac,
    0xee, 0xdb, 0x11, 0xb1, 0xcd, 0xb2, 0x5e, 0x74, 0x86, 0x3b, 0xd8, 0x7a, 0x0a, 0x6c, 0x30,
    0x96, 0x3b, 0xfe, 0xc9, 0x05, 0xee, 0xaf, 0xcc, 0x8d, 0x54, 0x5d, 0x39, 0xef, 0x77, 0xc9,
    0x03, 0x98, 0x0d, 0x5f, 0xd2, 0x15, 0xfa, 0x50, 0x2d, 0xb6, 0x57, 0x40, 0xc5, 0x42, 0xef,
    0xdd, 0x7a, 0x39, 0x39, 0xce, 0xb7, 0x2d, 0x2d, 0xde, 0x62, 0xe4, 0x44, 0xdf, 0xf8, 0xbe,
    0x47, 0xb7, 0xf7, 0x61, 0xaf, 0x33, 0x6f, 0xf8, 0x88, 0xd9, 0x1c, 0x21, 0xe8, 0x30, 0xb9,
    0x57, 0x48, 0x16, 0x49, 0x2b, 0x8f, 0x37, 0x9c, 0x8e, 0xfb, 0x55, 0x92, 0x64, 0xe1, 0x1d,
    0xb8, 0x9f
};

/* 3072 bits rsa key pair. */
/* public exponent */
static UCHAR pub_e_3072_0[] = {0xf6, 0x1a, 0x81};

/* private exponent */
static UCHAR pri_e_3072_0[] =
{
    0x13, 0xfc, 0x53, 0x7c, 0x19, 0x84, 0x45, 0xab, 0xe8, 0x66, 0xc9, 0xe6, 0xd0, 0x04, 0xdc,
    0xd4, 0x9d, 0x20, 0x91, 0x84, 0xb1, 0x79, 0x51, 0xf2, 0x8f, 0x18, 0x92, 0x84, 0x9b, 0xbd,
    0xfd, 0xf9, 0x07, 0xee, 0x9d, 0xed, 0x9d, 0xf6, 0x1e, 0x8e, 0xd7, 0xb2, 0x56, 0x0f, 0xae,
    0xea, 0x2a, 0x05, 0xcd, 0xfc, 0x0b, 0x96, 0xd4, 0x80, 0x5d, 0xd0, 0xe9, 0xef, 0xff, 0xee,
    0x91, 0xb5, 0x4f, 0x18, 0x2e, 0x2a, 0x56, 0xbf, 0xf6, 0x40, 0xf5, 0x09, 0x71, 0x93, 0x6b,
    0x3e, 0x28, 0x25, 0x23, 0x2a, 0x05, 0xcf, 0x5c, 0x1d, 0xae, 0xeb, 0xee, 0xd9, 0x16, 0xab,
    0x75, 0x76, 0x97, 0xc8, 0x1a, 0xfa, 0x3a, 0xa3, 0x1f, 0x48, 0xb6, 0x29, 0xea, 0xa0, 0xd0,
    0x0f, 0x64, 0x2c, 0x12, 0xe7, 0xd5, 0xd3, 0xce, 0xdf, 0xd9, 0xef, 0x2b, 0xe0, 0x0a, 0xc6,
    0x1b, 0x6b, 0xb0, 0xc5, 0xea, 0xfa, 0x76, 0x7a, 0x67, 0x32, 0x46, 0xaa, 0x78, 0x1f, 0x51,
    0x07, 0x7c, 0xcb, 0x7f, 0x66, 0x2a, 0x6c, 0xd1, 0xa2, 0x8e, 0xa3, 0x71, 0x94, 0x3a, 0x97,
    0xde, 0x2a, 0x6e, 0xe2, 0xe3, 0x4f, 0x87, 0x48, 0x13, 0x86, 0x10, 0x2d, 0xba, 0xc0, 0x93,
    0x95, 0xb0, 0xb8, 0x78, 0xd0, 0x4c, 0x08, 0x7f, 0x97, 0x4f, 0xc3, 0x9a, 0x34, 0xfc, 0xbd,
    0x93, 0x65, 0x7b, 0x04, 0xa1, 0x7e, 0x03, 0x28, 0x18, 0x7a, 0x04, 0x78, 0xec, 0x05, 0xdd,
    0x83, 0xda, 0x25, 0xc5, 0x12, 0x3f, 0xfa, 0xbb, 0xe9, 0x58, 0x7c, 0x88, 0x2e, 0x9c, 0xd6,
    0xd0, 0x22, 0x8e, 0xd3, 0x5e, 0x99, 0x67, 0x5a, 0xe9, 0xd6, 0x57, 0xa7, 0x09, 0x3a, 0x83,
    0xc4, 0x9d, 0xc4, 0x07, 0x93, 0xc3, 0xb5, 0xfa, 0x97, 0x83, 0x8f, 0x9b, 0x79, 0x67, 0x70,
    0x50, 0xd2, 0x7a, 0xaa, 0x82, 0x7b, 0x3d, 0x71, 0xf5, 0x04, 0x50, 0xbb, 0x1e, 0x2a, 0x29,
    0x8f, 0xb4, 0x5a, 0x06, 0x49, 0x00, 0x92, 0x60, 0xc7, 0x1d, 0x83, 0x3b, 0x72, 0x9b, 0xb9,
    0x09, 0x3d, 0x22, 0xbf, 0xc4, 0x04, 0x8d, 0x9a, 0x6a, 0x92, 0x81, 0x4b, 0x32, 0x74, 0x91,
    0x34, 0xba, 0xa4, 0xd8, 0x1a, 0x1d, 0x24, 0xec, 0x77, 0x18, 0xa3, 0xd7, 0xf4, 0xab, 0x31,
    0x0b, 0x5a, 0xf0, 0x9e, 0x08, 0x4e, 0xcb, 0x55, 0xa6, 0xe1, 0xeb, 0xc5, 0xd8, 0x79, 0x78,
    0xce, 0x16, 0x14, 0x07, 0xde, 0x58, 0xea, 0x36, 0x2c, 0x0a, 0x9a, 0x04, 0x8e, 0xda, 0xa4,
    0x65, 0xea, 0x52, 0x86, 0x55, 0xf8, 0xda, 0x9d, 0xa6, 0x89, 0x0d, 0x28, 0xd1, 0xd3, 0xb3,
    0xe7, 0xbd, 0xac, 0x35, 0x52, 0xc5, 0xc4, 0xbe, 0x10, 0x6a, 0x7f, 0x7b, 0xe7, 0x79, 0xf9,
    0x37, 0x5e, 0x62, 0x13, 0xd6, 0xd8, 0xe9, 0x02, 0xe2, 0xb6, 0x28, 0xe8, 0x07, 0x55, 0xe7,
    0x72, 0x59, 0x8f, 0xb2, 0xb5, 0x40, 0x98, 0x4e, 0xc1
};

/* modulus */
static UCHAR m_3072_0[] =
{
    0xab, 0x77, 0x10, 0xc4, 0x43, 0xec, 0x7e, 0x29, 0x4d, 0x9d, 0x86, 0x7e, 0x00, 0x00,
    0x1f, 0x6a, 0x90, 0x44, 0x6f, 0x3c, 0xcc, 0x9b, 0xe6, 0x82, 0xf2, 0x05, 0x6d, 0xe5, 0x8b,
    0x63, 0xc4, 0x44, 0x39, 0xca, 0xaf, 0xfb, 0x2e, 0x3e, 0xaf, 0x1d, 0xf0, 0xff, 0x7c, 0xce,
    0x8b, 0xd6, 0xdc, 0x00, 0xd6, 0x53, 0xaf, 0x9c, 0xd7, 0x7c, 0x07, 0xb2, 0x51, 0x6f, 0x6e,
    0xc8, 0xbe, 0x0c, 0x78, 0xb2, 0x0b, 0xa3, 0xe1, 0x1f, 0x9d, 0x78, 0x95, 0xa2, 0x6b, 0x5b,
    0x27, 0x4f, 0xce, 0x48, 0x51, 0x3a, 0x3f, 0x81, 0xfe, 0xc2, 0xda, 0x5a, 0x5a, 0xac, 0x28,
    0x63, 0x18, 0x6a, 0xed, 0x11, 0xdc, 0x55, 0x30, 0x4a, 0xd3, 0x98, 0xa4, 0x51, 0x9d, 0xd7,
    0xc4, 0x5b, 0x0d, 0x3f, 0xb2, 0xf4, 0x3b, 0x74, 0xb0, 0xdc, 0x19, 0xa0, 0x1d, 0x4f, 0x58,
    0xa2, 0xdf, 0x10, 0x8b, 0x4f, 0x57, 0xe6, 0xb8, 0x31, 0x6e, 0x22, 0x9f, 0x73, 0x81, 0x4d,
    0x65, 0x48, 0x39, 0x1d, 0x7f, 0x5d, 0x7d, 0x6f, 0xfb, 0x50, 0x67, 0xf6, 0x77, 0x1d, 0x84,
    0x08, 0xaa, 0xd3, 0xc5, 0xdf, 0x7f, 0x2a, 0x64, 0xb7, 0x04, 0x9b, 0xde, 0xb9, 0x3d, 0x4b,
    0x39, 0x7b, 0x5b, 0x93, 0x9f, 0xb9, 0xab, 0xdd, 0x8d, 0x4f, 0x1d, 0x3b, 0x3b, 0x7c, 0xb4,
    0x97, 0x99, 0x7a, 0x8d, 0x75, 0x79, 0x10, 0xc8, 0x09, 0x27, 0x2d, 0xd1, 0xd3, 0x6a, 0xef,
    0xe0, 0x01, 0xac, 0x0f, 0xc9, 0xdc, 0xcb, 0xe1, 0x22, 0x30, 0xf5, 0x7c, 0x07, 0xc1, 0xfb,
    0x81, 0xbc, 0x3b, 0x54, 0xd2, 0xab, 0x04, 0x52, 0x84, 0xbe, 0xb7, 0x74, 0xbb, 0xb0, 0x71,
    0x32, 0x7f, 0x1c, 0x2a, 0xc5, 0x5b, 0x02, 0x54, 0xe6, 0x31, 0xae, 0x29, 0x1f, 0xf9, 0x4b,
    0xca, 0x72, 0x46, 0xb6, 0x25, 0xc5, 0x85, 0xfb, 0xc2, 0x38, 0x8d, 0x87, 0x60, 0xda, 0xc8,
    0x62, 0xca, 0x95, 0x79, 0x05, 0xb2, 0xa1, 0x33, 0x3d, 0xe1, 0xe8, 0x64, 0x61, 0xe7, 0x92,
    0x5d, 0x06, 0x62, 0xa1, 0x8d, 0x56, 0x81, 0x91, 0x04, 0xe0, 0xb3, 0x72, 0xa2, 0x07, 0x66,
    0xf0, 0xe5, 0x3d, 0x6d, 0xed, 0xdc, 0xfa, 0x4a, 0x54, 0x08, 0xc4, 0x3d, 0xc3, 0x9d, 0x9b,
    0x33, 0x38, 0x67, 0xd9, 0xdd, 0x26, 0x29, 0xa1, 0xe8, 0x9f, 0xa3, 0x57, 0xc4, 0x8d, 0x14,
    0x89, 0xe5, 0x52, 0xe6, 0x06, 0xee, 0xa4, 0xaa, 0x93, 0xa3, 0xb3, 0x46, 0x19, 0x35, 0x13,
    0x7b, 0x32, 0xcd, 0xce, 0x90, 0x72, 0xe3, 0xce, 0x51, 0x10, 0x39, 0xce, 0x53, 0x4a, 0xc6,
    0x0e, 0xe1, 0x5a, 0x4a, 0xe3, 0x07, 0xfc, 0x8a, 0xe0, 0xb9, 0x56, 0xa5, 0x5a, 0x4f, 0x6e,
    0x26, 0x07, 0xc2, 0xdf, 0xf3, 0x45, 0x5c, 0x7d, 0x40, 0x42, 0xd5, 0xcf, 0xfc, 0x75, 0x09,
    0x62, 0xb2, 0x6b, 0x19, 0xd6, 0x73, 0x18, 0xed, 0x68, 0x29
};

/* 4096 bits rsa key pair. */
static UCHAR pub_e_4096_0[] = {0x3d, 0x14, 0x4d};

static UCHAR pri_e_4096_0[] =
{
    0x63, 0x1f, 0x3c, 0xea, 0xae, 0x86, 0xcb, 0x97, 0xc3, 0x98, 0x20, 0xaa, 0x37, 0xcf, 0x3d,
    0xee, 0x25, 0x0c, 0x4e, 0xf0, 0x38, 0xcb, 0x04, 0x3b, 0xa5, 0x84, 0x81, 0x53, 0x3f, 0x9c,
    0xec, 0x4a, 0xac, 0x94, 0x85, 0xd0, 0x33, 0x19, 0x56, 0xc4, 0x35, 0xe7, 0xe0, 0x97, 0x92,
    0x8f, 0x5d, 0x8b, 0x4c, 0x2f, 0xe2, 0xd8, 0xcb, 0x9e, 0x23, 0x9f, 0x36, 0x2f, 0x76, 0x46,
    0x18, 0xcf, 0xe7, 0xe7, 0x70, 0x75, 0x16, 0x2c, 0x6a, 0x33, 0x77, 0x68, 0xb5, 0x48, 0x13,
    0x70, 0x6f, 0x01, 0xbf, 0x03, 0x03, 0xbf, 0x91, 0xc7, 0xe3, 0x3c, 0x3b, 0x56, 0x7d, 0x03,
    0x27, 0x4d, 0x32, 0xe2, 0x41, 0xd2, 0x0f, 0x7b, 0x16, 0xb1, 0x67, 0x76, 0x2d, 0xc0, 0x77,
    0xdd, 0x3e, 0xc1, 0x04, 0x90, 0xeb, 0x4e, 0xff, 0x45, 0xbb, 0x93, 0x31, 0x7a, 0x04, 0xcd,
    0x15, 0x4b, 0x42, 0xe1, 0x41, 0xbb, 0xe9, 0x0f, 0xa2, 0x21, 0x90, 0xe2, 0x3b, 0xdf, 0x5a,
    0xbd, 0x00, 0x7d, 0x4b, 0x17, 0x6a, 0xd4, 0xb0, 0xa2, 0xcb, 0xc8, 0x60, 0x23, 0x38, 0x0f,
    0xaa, 0xe0, 0xbb, 0x17, 0xb4, 0xf7, 0x24, 0x4c, 0x31, 0xe8, 0x88, 0xbe, 0x92, 0x37, 0x15,
    0xd6, 0x63, 0x86, 0x66, 0xc1, 0x3d, 0x48, 0xdb, 0x11, 0xe0, 0x55, 0x0a, 0xe8, 0x10, 0x2b,
    0x36, 0x12, 0x6f, 0x86, 0x77, 0x51, 0x9d, 0xbf, 0x2f, 0x11, 0x76, 0x26, 0xe0, 0x45, 0x3c,
    0x96, 0x43, 0x67, 0x85, 0x05, 0xc8, 0x95, 0xa5, 0xfc, 0x00, 0xb0, 0xec, 0xff, 0xb8, 0xff,
    0x5a, 0x46, 0xb2, 0x77, 0x3a, 0xcb, 0xd1, 0x2a, 0xcd, 0xa4, 0x36, 0x79, 0xc2, 0xc8, 0xcf,
    0xa2, 0xf1, 0xae, 0xa2, 0x28, 0x4c, 0xbb, 0x85, 0xde, 0x5c, 0xba, 0xbe, 0x6a, 0x45, 0xe4,
    0x38, 0x27, 0x53, 0x9f, 0xdb, 0x56, 0xbc, 0x05, 0x68, 0x4f, 0x84, 0x97, 0x6a, 0x8f, 0x79,
    0xd3, 0x82, 0xa7, 0x1c, 0x9d, 0x7b, 0xde, 0x3c, 0x55, 0xbe, 0x39, 0xec, 0x65, 0xbd, 0xd3,
    0x8f, 0xc7, 0xc4, 0x2e, 0x22, 0x90, 0xad, 0x25, 0x48, 0xdc, 0x41, 0xf9, 0xc9, 0xb4, 0x42,
    0x62, 0x04, 0x58, 0xc2, 0x2f, 0x02, 0xf4, 0x5d, 0x0b, 0xb5, 0x7c, 0x8d, 0xf4, 0xee, 0x43,
    0xf5, 0x26, 0x34, 0xcc, 0x88, 0xe3, 0x4b, 0x31, 0x9d, 0x33, 0xda, 0x2b, 0x44, 0x6d, 0x0c,
    0x7b, 0x65, 0x0b, 0x93, 0x38, 0x65, 0xb7, 0xec, 0x2c, 0x03, 0xf5, 0xf1, 0x75, 0x93, 0xec,
    0x4d, 0x6d, 0xe0, 0xe5, 0x21, 0x8d, 0x20, 0x3b, 0xf8, 0x46, 0x2c, 0x8e, 0x4c, 0x96, 0x8d,
    0x96, 0x1d, 0xd5, 0xe6, 0x58, 0x72, 0xcb, 0x2a, 0xd2, 0x49, 0x4c, 0xce, 0xe5, 0xe5, 0x2a,
    0x6a, 0xd6, 0xf4, 0xf8, 0x82, 0xc6, 0xa0, 0x7d, 0x43, 0xf4, 0xf0, 0xde, 0x1b, 0x35, 0x25,
    0x08, 0x27, 0x7c, 0x9c, 0x9e, 0xbc, 0x91, 0x98, 0x7c, 0xd6, 0x9d, 0x19, 0xf1, 0xa6, 0x85,
    0x1c, 0x30, 0x01, 0xee, 0xbb, 0xe7, 0xe0, 0x71, 0xb7, 0x9d, 0xa3, 0x91, 0x8e, 0x05, 0x2c,
    0xa7, 0x46, 0x19, 0xc6, 0x47, 0xa7, 0xa0, 0xa8, 0x13, 0x42, 0xbd, 0xd1, 0x4c, 0x4f, 0xa0,
    0x83, 0xc4, 0xc7, 0xc4, 0x7f, 0x63, 0x84, 0x88, 0x9c, 0x64, 0x35, 0xf0, 0xd1, 0xc2, 0x88,
    0xb2, 0xfd, 0x0d, 0x62, 0xcc, 0x37, 0xfc, 0x83, 0x87, 0x80, 0x94, 0xe3, 0xf9, 0x2d, 0x5d,
    0x0b, 0x5d, 0x73, 0x1a, 0xb7, 0x73, 0xcc, 0x65, 0x50, 0x12, 0xdc, 0x55, 0xda, 0xe1, 0xd5,
    0xe7, 0x3b, 0x16, 0x11, 0xed, 0xa7, 0xba, 0x21, 0x50, 0xac, 0x86, 0x59, 0xf9, 0x85, 0x4c,
    0x58, 0xec, 0x5f, 0xf5, 0x09, 0x7a, 0x40, 0x0f, 0x72, 0xc8, 0xf0, 0xde, 0xcf, 0x98, 0x58,
    0xb1, 0xa7, 0xb0, 0xe7, 0x04, 0xe8, 0xc8, 0xee, 0xe5, 0x04, 0xb2, 0x9d, 0x96, 0xd0, 0x82,
    0xe4, 0x85
};

static UCHAR m_4096_0[] =
{
    0xa8, 0xb3, 0x5e, 0xbf, 0xb5, 0xd9, 0x12, 0x38, 0x62, 0x47, 0x32, 0xe3, 0xee, 0x72,
    0x3e, 0x38, 0x7c, 0x41, 0x74, 0xb4, 0xb0, 0x30, 0x51, 0xbf, 0x4a, 0x0a, 0xf0, 0x46, 0x0e,
    0x85, 0xa8, 0x1b, 0x75, 0xa8, 0xb5, 0x51, 0xd9, 0x3d, 0x2b, 0xca, 0x2e, 0x57, 0x5e, 0xe9,
    0xfb, 0xa4, 0x11, 0x32, 0xcd, 0xe9, 0xf6, 0xb2, 0x7d, 0x1f, 0x1f, 0x9b, 0x83, 0xdf, 0x66,
    0x3a, 0x79, 0x26, 0x49, 0x1c, 0x8c, 0x4d, 0xca, 0xc0, 0x19, 0x97, 0x64, 0xc2, 0xec, 0x6c,
    0x93, 0x89, 0x5c, 0x68, 0x4f, 0x3f, 0xf1, 0x2f, 0x8a, 0xa7, 0xd9, 0x34, 0x02, 0x20, 0x7c,
    0x25, 0x03, 0xb2, 0xb9, 0xfc, 0x8a, 0xbf, 0xc3, 0xa6, 0xc7, 0x3b, 0xee, 0x7a, 0xfa, 0xa9,
    0x72, 0x78, 0x9b, 0x7a, 0x3f, 0xa2, 0x3e, 0x38, 0x81, 0x59, 0xe1, 0x89, 0x07, 0x5f, 0x90,
    0x04, 0x82, 0xfe, 0xb3, 0x41, 0x8e, 0x22, 0x3e, 0x6f, 0x1f, 0xf7, 0xf0, 0x0d, 0x20, 0xd4,
    0x68, 0x2f, 0xb5, 0x11, 0x83, 0x5c, 0x17, 0xb2, 0xba, 0x8e, 0x76, 0x29, 0x82, 0xdc, 0x6b,
    0xdd, 0xf0, 0x1a, 0x00, 0x89, 0xfe, 0x07, 0x70, 0x5b, 0xc4, 0xd5, 0x4e, 0x6c, 0x83, 0x6d,
    0x71, 0x03, 0xbe, 0x7e, 0x73, 0x0e, 0x14, 0xee, 0xbb, 0x26, 0xf9, 0x4f, 0x3d, 0x5a, 0xd4,
    0x07, 0xfe, 0x24, 0x38, 0x4f, 0xb3, 0x6e, 0x3c, 0x42, 0xc5, 0x49, 0xe3, 0xab, 0xef, 0x81,
    0x04, 0xe0, 0xdd, 0x0f, 0x71, 0x1e, 0x47, 0x09, 0xb2, 0xc3, 0x5e, 0x1d, 0xda, 0x28, 0xe2,
    0xf2, 0x86, 0xf0, 0xf5, 0x45, 0x55, 0x0b, 0x7a, 0x10, 0xbf, 0x14, 0x6f, 0xf4, 0xe1, 0x58,
    0x68, 0x04, 0xcf, 0xfd, 0xae, 0xde, 0x69, 0x9f, 0xcf, 0xa4, 0xc9, 0xcd, 0x28, 0xf6, 0xc9,
    0x90, 0x5e, 0xbb, 0xbe, 0x61, 0xc6, 0xc0, 0xf8, 0x98, 0x81, 0xc2, 0x68, 0x7f, 0xfb, 0xae,
    0xbb, 0x40, 0x04, 0xf4, 0x74, 0xca, 0xfc, 0xd0, 0x7c, 0xd0, 0x78, 0xe7, 0x99, 0x26, 0x01,
    0x5c, 0x7b, 0x9f, 0x6b, 0xde, 0x89, 0xf3, 0x4d, 0x43, 0xc1, 0xc4, 0xa9, 0x25, 0xbd, 0x01,
    0x39, 0x19, 0xb3, 0xba, 0x78, 0xc0, 0x64, 0x16, 0xa1, 0x32, 0x50, 0xb9, 0xd0, 0xd2, 0x1b,
    0xd9, 0x7e, 0xe9, 0x1a, 0x98, 0xd7, 0xcc, 0xae, 0xef, 0x57, 0xb9, 0x80, 0x3e, 0xf2, 0x17,
    0xb2, 0x13, 0xba, 0xee, 0x1b, 0x9c, 0xca, 0xea, 0x0f, 0xf6, 0x13, 0x0e, 0xab, 0x7f, 0x4e,
    0x46, 0x41, 0xba, 0x40, 0xf0, 0xb3, 0xd7, 0x10, 0xda, 0x00, 0xeb, 0x3e, 0xc5, 0xc9, 0x9f,
    0xeb, 0xc2, 0x2c, 0xeb, 0x0d, 0x37, 0x1f, 0xd4, 0x1c, 0xd7, 0x73, 0x37, 0xfc, 0x75, 0x31,
    0x21, 0x50, 0x66, 0x5e, 0x7a, 0x83, 0x67, 0x19, 0x7d, 0xa7, 0x18, 0x02, 0x29, 0x84, 0xe7,
    0x25, 0x22, 0x83, 0x90, 0xd9, 0x34, 0x11, 0x5d, 0xf6, 0x67, 0x99, 0x72, 0x5d, 0x5f, 0xa0,
    0x29, 0x93, 0x96, 0x61, 0x9f, 0xea, 0xe2, 0xa6, 0xaa, 0x8b, 0x6e, 0x68, 0xf7, 0xd7, 0x3b,
    0x36, 0xbc, 0xc3, 0x83, 0xa5, 0x23, 0xd1, 0x01, 0x75, 0x74, 0x0e, 0xcb, 0xdf, 0x73, 0xd8,
    0x02, 0x6b, 0xf6, 0x62, 0xae, 0xbd, 0x91, 0xab, 0x30, 0xfb, 0xe8, 0x0b, 0xf9, 0x4a, 0x46,
    0xd3, 0x79, 0xbc, 0x75, 0xd5, 0xec, 0x18, 0xf5, 0xd0, 0xc2, 0xa1, 0xa2, 0x57, 0xac, 0xab,
    0x2f, 0x23, 0x8c, 0xeb, 0xce, 0x62, 0x68, 0xe9, 0x60, 0x96, 0x35, 0x84, 0xed, 0x60, 0x47,
    0xef, 0xe1, 0x34, 0x60, 0x78, 0x34, 0xac, 0x1f, 0xfe, 0xa1, 0x4b, 0x87, 0xec, 0x54, 0xff,
    0x2b, 0xb4, 0x11, 0x6b, 0xae, 0x85, 0xf8, 0x65, 0x04, 0x07, 0x4b, 0x8e, 0xc6, 0x40, 0x6e,
    0x09, 0x2c, 0x43, 0x51, 0xea, 0x09, 0x9b, 0x7e, 0x6f, 0x40, 0x9e, 0x33, 0x8a, 0x03, 0x7a,
    0x5e, 0x58, 0x71
};
