
	;Zeropage works
	.exportzp	__r0
	.exportzp	__r1
	.exportzp	__r2
	.exportzp	__r3
	.exportzp	__r4
	.exportzp	__r5
	.exportzp	__r6
	.exportzp	__r7
	
	.exportzp	__n0
	.exportzp	__n1
	.exportzp	__n2
	.exportzp	__n3
	
	.exportzp	__i0
	.exportzp	__i1
	.exportzp	__i2
	.exportzp	__i3
	
	.exportzp	__Flag_2000
	.exportzp	__Flag_2001
		
	.exportzp	__cc
	.exportzp	__ss
	.exportzp	__mm
	.exportzp	__hh
	
	.exportzp	__PAD0
	.exportzp	__PAD1
	.exportzp	__PAD0_OLD
	.exportzp	__PAD1_OLD
	.exportzp	__PAD0_REL
	.exportzp	__PAD1_REL
	.exportzp	__PAD0_TRG
	.exportzp	__PAD1_TRG
	
	.exportzp	__MMC3_Pbank0
	.exportzp	__MMC3_Pbank1

	.exportzp	__MusBank
	.exportzp	__SeBank

	;Non Zeropage works
	.export		_NMI_CallBack
	.export		_IRQ_CallBack
	.export		_JMP_Address

;=======================================================================
;	Zeropage works
;-----------------------------------------------------------------------
.zeropage

__r0:		.byte	0		;= $00	;
__r1:		.byte	0		;= $01	;
__r2:		.byte	0		;= $02	;
__r3:		.byte	0		;= $03	;
__r4:		.byte	0		;= $04	;※関数規約：呼び出され側で保持
__r5:		.byte	0		;= $05	;※関数規約：呼び出され側で保持
__r6:		.byte	0		;= $06	;※関数規約：呼び出され側で保持
__r7:		.byte	0		;= $07	;※関数規約：呼び出され側で保持

;汎用レジスタ（NMI割り込み用）
__n0:		.byte	0		;= $08	;
__n1:		.byte	0		;= $09	;
__n2:		.byte	0		;= $0A	;
__n3:		.byte	0		;= $0B	;

;汎用レジスタ（IRQ割り込み用）
__i0:		.byte	0		;= $0C	;
__i1:		.byte	0		;= $0D	;
__i2:		.byte	0		;= $0E	;
__i3:		.byte	0		;= $0F	;

;-----------------------------------------------------------------------
;システム関連
__Flag_2000:	.byte	0		;= $10	;
__Flag_2001:	.byte	0		;= $11	;

;-----------------------------------------------------------------------
;マッパー関連
__MMC3_Pbank0:	.byte	0		;= $12	;
__MMC3_Pbank1:	.byte	0		;= $13	;

;-----------------------------------------------------------------------
;時間関連
__cc:		.byte	0		;= $14	; 1/60単位 (HEX)
__ss:		.byte	0		;= $15	; 秒【BCD】
__mm:		.byte	0		;= $16	; 分【BCD】
__hh:		.byte	0		;= $17	; 時 (HEX)

;-----------------------------------------------------------------------
;キー関連
__PAD0:		.byte	0		;= $18	;
__PAD1:		.byte	0		;= $19	;
__PAD0_OLD:	.byte	0		;= $1A	;
__PAD1_OLD:	.byte	0		;= $1B	;
__PAD0_REL:	.byte	0		;= $1C	;
__PAD1_REL:	.byte	0		;= $1D	;
__PAD0_TRG:	.byte	0		;= $1E	;
__PAD1_TRG:	.byte	0		;= $1F	;

;-----------------------------------------------------------------------
;サウンド関連

__MusBank:	.byte	0		;= $20
__SeBank:	.byte	0		;= $21

;
;	$20 - $4F … nsd.lib sound work
;


;=======================================================================
;	Non Zeropage works
;-----------------------------------------------------------------------
.bss

_NMI_CallBack:	.word	0		;= $0300
_IRQ_CallBack:	.word	0		;= $0302
_JMP_Address:	.word	0		;= $0304

