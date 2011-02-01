
#include "fixed.h"

fix int2fix(int32_t a)
{
	return (a << FIXBITS);
}

fix float2fix(float f)
{
	return (fix)(f * (1<<FIXBITS) + ( f >= 0 ? 0.5f : -0.5f ));
}

fix fixmul(fix a, fix b)
{
	return ((int64_t)a * b) >> FIXBITS;
}

fix fixdiv(fix a, fix b)
{
	return ((int64_t)a << FIXBITS) / b;
}

float fix2float(fix f)
{
	return ((float)f / (1 << FIXBITS));
}

fix fix2int(fix f)
{
	return f >> FIXBITS;
}

