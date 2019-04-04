#include <malloc.h>


void Test_MacMemorySegment_allocate(void)
{
	void* pMem = malloc(0x11007);

	__asm
	{
		mov     ecx, eax
		lea     esi, [ecx + 8]
		and     esi, 0FFFh
		mov     edx, esi
		neg     edx
		xor     eax, eax
		mov[ecx + edx + 1000h], ecx
		mov     edx, 1000h
		sub     edx, esi
		add     edx, ecx
		add     edx, 8
	}

	free(pMem);
}
