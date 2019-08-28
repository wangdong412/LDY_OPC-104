//////////////////////////////////////////////////////////////////////////
///		项目: 
///		作者: 栾述强
///		版本: V0.1
///     日期: 2010-5-10
//////////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif		///_MSC_VER > 1000

#ifndef __MYMD5_H__
#define __MYMD5_H__
///=======================================================================
///MD5算法的封装类
///=======================================================================

#define UINT4 unsigned int

/* Constants for MD5Transform routine. */
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

/* F, G, H and I are basic MD5 functions.*/
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits.*/
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
	Rotation is separate from addition to prevent recomputation.*/
#define FF(a, b, c, d, x, s, ac) { \
	(a) += F ((b), (c), (d)) + (x) + (UINT4)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
	}
#define GG(a, b, c, d, x, s, ac) { \
	(a) += G ((b), (c), (d)) + (x) + (UINT4)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
	}
#define HH(a, b, c, d, x, s, ac) { \
	(a) += H ((b), (c), (d)) + (x) + (UINT4)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
	}
#define II(a, b, c, d, x, s, ac) { \
	(a) += I ((b), (c), (d)) + (x) + (UINT4)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
	}

static void Encode(unsigned char* output, UINT4* input, UINT4 len);
static void Decode(UINT4* output, const unsigned char* input, unsigned int len);

static unsigned char PADDING[64] = { 
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	

typedef struct { unsigned int state[4];                                  /* state (ABCD) */
				 unsigned int count[2];        /* number of bits, modulo 2^64 (lsb first) */
				 unsigned char buffer[64];                         /* input buffer */
				 } MD5_CTX;

class CMyMD5
{
public:
	CMyMD5(void);
	~CMyMD5(void);
	
	//输入srcBuf及其长度nLen, 输出MD5码
	static bool GetMD5Code(unsigned char* MD5DigestBuf, const unsigned char* srcBuf, unsigned int nLen);
	
private:
	static void MD5Init(MD5_CTX *context);
	static void MD5Update(MD5_CTX *context, const unsigned char *input, unsigned int inputLen);
	static void MD5Final(unsigned char* digest, MD5_CTX* context);
	static void MD5Transform(UINT4* state, const unsigned char* block);  //4uint数组, uc数组64
};

///=======================================================================
#endif		///#define __MYMD5_H__
