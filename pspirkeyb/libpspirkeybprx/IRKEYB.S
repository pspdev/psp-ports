	.set noreorder

#include "pspstub.s"

	STUB_START "IRKEYB",0x00090000,0x00050005
	STUB_FUNC  0xAE6E6E8D,pspIrKeybInitPrx
	STUB_FUNC  0xDB966CAE,pspIrKeybOutputModePrx
	STUB_FUNC  0x6A2B703A,pspIrKeybGetOutputModePrx
	STUB_FUNC  0x644C7B28,pspIrKeybReadinputPrx
	STUB_FUNC  0xEA6AFE9E,pspIrKeybFinishPrx
	STUB_END
