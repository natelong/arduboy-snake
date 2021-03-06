#include "rnd.h"
#include "tinymt32.h"


//---------------------------------------------------------------------------
tinymt32_t MathTinyMt;


//---------------------------------------------------------------------------
void RndInit(void)
{
	tinymt32_init(&MathTinyMt, 0xF67C129B);
}
//---------------------------------------------------------------------------
void RndInitSeed(u32 num)
{
	tinymt32_init(&MathTinyMt, num);
}
//---------------------------------------------------------------------------
u32 Rnd32(void)
{
	return tinymt32_generate_uint32(&MathTinyMt);
}
//---------------------------------------------------------------------------
u32 Rnd(u32 num)
{
	s32 ret = Rnd32() % num;

	if(ret < 0)
	{
		ret *= -1;
	}

	return ret;
}
