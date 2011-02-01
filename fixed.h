#pragma once

#include <sys/types.h>

typedef int32_t fix;
#define FIXBITS 8

static inline fix int2fix(int32_t a)
{
	return (a << FIXBITS);
}

static inline fix float2fix(float f)
{
	return (fix)(f * (1<<FIXBITS) + ( f >= 0 ? 0.5f : -0.5f ));
}

static inline fix fixmul(fix a, fix b)
{
	return ((int64_t)a * b) >> FIXBITS;
}

static inline fix fixdiv(fix a, fix b)
{
	return ((int64_t)a << FIXBITS) / b;
}

static inline float fix2float(fix f)
{
	return ((float)f / (1 << FIXBITS));
}

static inline fix fix2int(fix f)
{
	return f >> FIXBITS;
}


