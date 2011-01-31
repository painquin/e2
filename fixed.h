#pragma once

#include <sys/types.h>

typedef int32_t fix;
#define FIXBITS 8

extern fix int2fix(int32_t a);
extern fix float2fix(float f);
extern fix fixmul(fix a, fix b);
extern fix fixdiv(fix a, fix b);
extern fix fix2float(fix f);
extern fix fix2int(fix f);
