

.ifndef	__NDK_WORK_INC__

.define	__NDK_WORK_INC__

	;Zeropage works
	.importzp	__r0
	.importzp	__r1
	.importzp	__r2
	.importzp	__r3
	.importzp	__r4
	.importzp	__r5
	.importzp	__r6
	.importzp	__r7
	
	.importzp	__n0
	.importzp	__n1
	.importzp	__n2
	.importzp	__n3
	
	.importzp	__i0
	.importzp	__i1
	.importzp	__i2
	.importzp	__i3
	
	.importzp	__Flag_2000
	.importzp	__Flag_2001
		
	.importzp	__cc
	.importzp	__ss
	.importzp	__mm
	.importzp	__hh
	
	.importzp	__PAD0
	.importzp	__PAD1
	.importzp	__PAD0_OLD
	.importzp	__PAD1_OLD
	.importzp	__PAD0_REL
	.importzp	__PAD1_REL
	.importzp	__PAD0_TRG
	.importzp	__PAD1_TRG
	
	;Non Zeropage works
	.import		_NMI_CallBack
	.import		_IRQ_CallBack
	.import		_JMP_Address

	;Sprite
	.import		_ppu_sprite_buff
.endif
