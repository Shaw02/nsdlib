
	.setcpu		"6502"

	.export		_nsd_snd_init
	.export		_nsd_snd_voice
	.export		_nsd_snd_volume
	.export		_nsd_snd_sweep
	.export		_nsd_snd_frequency
	.export		_nsd_snd_keyon
	.export		_nsd_snd_keyoff

	.import		_nsd_div192

	.import		nsd_work
	.importzp	nsd_work_zp

	.include	"nes.inc"
	.include	"nsd.inc"

;=======================================================================
;	void	__fastcall__	nsd_snd_init(void);
;-----------------------------------------------------------------------
;<<Contents>>
;	Initaraize of the sound device
;<<Input>>
;	nothing
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_snd_init
.code
	jmp	_nsd_nes_init

Exit:
.endproc

;=======================================================================
;	void	__fastcall__	_nsd_snd_keyon(void);
;-----------------------------------------------------------------------
;<<Contents>>
;	Key on the device
;<<Input>>
;	nothing
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_snd_keyon
.rodata
JMPTBL:	.addr	_nsd_nes_keyon		;BGM ch1 Pulse
	.addr	_nsd_nes_keyon		;BGM ch2 Pulse
	.addr	_nsd_ch3_keyon		;BGM ch3 Triangle
	.addr	_nsd_nes_keyon		;BGM ch4 Noize
	.addr	_nsd_dpcm_keyon		;BGM ch5 DPCM
.ifdef	FDS
;	.addr	
.endif
.ifdef	VRC6
;	.addr	
;	.addr	
;	.addr	
.endif
.ifdef	VRC7
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
.endif
.ifdef	MMC5
;	.addr	
;	.addr	
.endif
.ifdef	N163
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
.endif
.ifdef	PSG
;	.addr	
;	.addr	
;	.addr	
.endif
	.addr	_nsd_nes_keyon		;SE  ch1 Pulse
	.addr	_nsd_nes_keyon		;SE  ch2 Noize

.code
	ldx	__channel
@Set:	;Set	(Don't need SE check)
	ldy	JMPTBL,x
	sty	__ptr
	ldy	JMPTBL + 1,x
	sty	__ptr + 1
	jmp	(__ptr)
Exit:
	rts
.endproc

;=======================================================================
;	void	__fastcall__	_nsd_snd_keyoff(void);
;-----------------------------------------------------------------------
;<<Contents>>
;	Key on the device
;<<Input>>
;	nothing
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_snd_keyoff
.rodata
JMPTBL:	.addr	Exit			;BGM ch1 Pulse
	.addr	Exit			;BGM ch2 Pulse
	.addr	_nsd_ch3_keyoff		;BGM ch3 Triangle	-- no process --
	.addr	Exit			;BGM ch4 Noize		-- no process --
	.addr	_nsd_dpcm_keyoff	;BGM ch5 DPCM		-- no process --
.ifdef	FDS
;	.addr	
.endif
.ifdef	VRC6
;	.addr	
;	.addr	
;	.addr	
.endif
.ifdef	VRC7
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
.endif
.ifdef	MMC5
;	.addr	
;	.addr	
.endif
.ifdef	N163
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
.endif
.ifdef	PSG
;	.addr	
;	.addr	
;	.addr	
.endif
	.addr	Exit			;SE  ch1 Pulse
	.addr	Exit			;SE  ch2 Noize		-- no process --

.code
	ldx	__channel
@Set:	;Set	(Don't need SE check)
	ldy	JMPTBL,x
	sty	__ptr
	ldy	JMPTBL + 1,x
	sty	__ptr + 1
	jmp	(__ptr)
Exit:
	rts
.endproc

;=======================================================================
;	void	__fastcall__	nsd_snd_voice(char voi);
;-----------------------------------------------------------------------
;<<Contents>>
;	Ser the voice (instruction)
;<<Input>>
;	a	voice number
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_snd_voice
.rodata
JMPTBL:	.addr	_nsd_nes_voice		;BGM ch1 Pulse
	.addr	_nsd_nes_voice		;BGM ch2 Pulse
	.addr	Exit			;BGM ch3 Triangle	-- no process --
	.addr	_nsd_noise_voice	;BGM ch4 Noize
	.addr	Exit			;BGM ch5 DPCM
.ifdef	FDS
;	.addr	
.endif
.ifdef	VRC6
;	.addr	
;	.addr	
;	.addr	
.endif
.ifdef	VRC7
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
.endif
.ifdef	MMC5
;	.addr	
;	.addr	
.endif
.ifdef	N163
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
.endif
.ifdef	PSG
;	.addr	
;	.addr	
;	.addr	
.endif
	.addr	_nsd_nes_voice		;SE  ch1 Pulse
	.addr	_nsd_noise_voice	;SE  ch2 Noize

.code
	ldx	__channel
@Set:	;Set	(Don't need SE check)
	ldy	JMPTBL,x
	sty	__ptr
	ldy	JMPTBL + 1,x
	sty	__ptr + 1
	jmp	(__ptr)
Exit:
	rts
.endproc
;=======================================================================
;	void	__fastcall__	nsd_snd_volume(char vol);
;-----------------------------------------------------------------------
;<<Contents>>
;	Ser the volume
;<<Input>>
;	a	volume ( 0 to 255 )
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_snd_volume
.rodata
JMPTBL:	.addr	_nsd_ch1_volume		;BGM ch1 Pulse
	.addr	_nsd_ch2_volume		;BGM ch2 Pulse
	.addr	Exit			;BGM ch3 Triangle	-- no process --
	.addr	_nsd_ch4_volume		;BGM ch4 Noize
	.addr	Exit			;BGM ch5 DPCM		-- no process --
.ifdef	FDS
;	.addr	
.endif
.ifdef	VRC6
;	.addr	
;	.addr	
;	.addr	
.endif
.ifdef	VRC7
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
.endif
.ifdef	MMC5
;	.addr	
;	.addr	
.endif
.ifdef	N163
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
.endif
.ifdef	PSG
;	.addr	
;	.addr	
;	.addr	
.endif
	.addr	_nsd_ch2_volume		;SE  ch1 Pulse
	.addr	_nsd_ch4_volume		;SE  ch2 Noize
.code
	;always jmp except This ch's SE is enabale
	ldx	__channel
	;SE check
@C2:	cpx	#nsd::TR_BGM2
	bne	@C6
	ldy	__Sequence_ptr + nsd::TR_SE1 + 1
	bne	Exit
	jmp	@Set
@C6:	cpx	#nsd::TR_BGM4
	bne	@Set
	ldy	__Sequence_ptr + nsd::TR_SE2 + 1
	bne	Exit
@Set:
	ldy	JMPTBL,x
	sty	__ptr
	ldy	JMPTBL + 1,x
	sty	__ptr + 1
	jmp	(__ptr)
Exit:
	rts
.endproc
;=======================================================================
;	void	__fastcall__	nsd_snd_sweep(char vol);
;-----------------------------------------------------------------------
;<<Contents>>
;	Ser the volume
;<<Input>>
;	a	volume ( 0 to 255 )
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_snd_sweep
.rodata
JMPTBL:	.addr	_nsd_ch1_sweep		;BGM ch1 Pulse
	.addr	_nsd_ch2_sweep		;BGM ch2 Pulse
	.addr	Exit			;BGM ch3 Triangle	-- no process --
	.addr	Exit			;BGM ch4 Noize		-- no process --
	.addr	Exit			;BGM ch5 DPCM		-- no process --
.ifdef	FDS
;	.addr	
.endif
.ifdef	VRC6
;	.addr	
;	.addr	
;	.addr	
.endif
.ifdef	VRC7
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
.endif
.ifdef	MMC5
;	.addr	
;	.addr	
.endif
.ifdef	N163
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
.endif
.ifdef	PSG
;	.addr	
;	.addr	
;	.addr	
.endif
	.addr	_nsd_se2_sweep		;SE  ch1 Pulse
	.addr	Exit			;SE  ch2 Noize		-- no process --
.code
	;always jmp except This ch's SE is enabale
	ldx	__channel
	;SE check
@C2:	cpx	#nsd::TR_BGM2
	bne	@C6
	ldy	__Sequence_ptr + nsd::TR_SE1 + 1
	bne	Exit
	jmp	@Set
@C6:	cpx	#nsd::TR_BGM4
	bne	@Set
	ldy	__Sequence_ptr + nsd::TR_SE2 + 1
	bne	Exit
@Set:
	ldy	JMPTBL,x
	sty	__ptr
	ldy	JMPTBL + 1,x
	sty	__ptr + 1
	jmp	(__ptr)
Exit:
	rts
.endproc
;=======================================================================
;	void	__fastcall__	nsd_snd_frequency(int freq);
;-----------------------------------------------------------------------
;<<Contents>>
;	Ser the voice (instruction)
;<<Input>>
;	ax	frequency (16 = 100 cent, o0c = 0x0000)
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_snd_frequency
.rodata
JMPTBL:	.addr	_nsd_nes_frequency	;BGM ch1 Pulse
	.addr	_nsd_nes_frequency	;BGM ch2 Pulse
	.addr	_nsd_nes_frequency	;BGM ch3 Triangle
	.addr	_nsd_noise_frequency	;BGM ch4 Noise
	.addr	Exit			;BGM ch5 DPCM
.ifdef	FDS
;	.addr	
.endif
.ifdef	VRC6
;	.addr	
;	.addr	
;	.addr	
.endif
.ifdef	VRC7
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
.endif
.ifdef	MMC5
;	.addr	
;	.addr	
.endif
.ifdef	N163
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
;	.addr	
.endif
.ifdef	PSG
;	.addr	
;	.addr	
;	.addr	
.endif
	.addr	_nsd_nes_frequency	;SE  ch1 Pulse
	.addr	_nsd_noise_frequency	;SE  ch2 Noise
.code
	stx	__tmp
	ldx	__channel
	;SE check
@C2:	cpx	#nsd::TR_BGM2
	bne	@C6
	ldy	__Sequence_ptr + nsd::TR_SE1 + 1
	bne	Exit
	jmp	@Set
@C6:	cpx	#nsd::TR_BGM4
	bne	@Set
	ldy	__Sequence_ptr + nsd::TR_SE2 + 1
	bne	Exit
@Set:
	ldy	JMPTBL,x
	sty	__ptr
	ldy	JMPTBL + 1,x
	sty	__ptr + 1
	ldx	__tmp
	jmp	(__ptr)
Exit:
	rts
.endproc

;=======================================================================
;	void	__fastcall__	nsd_nes_init(void);
;-----------------------------------------------------------------------
;<<Contents>>
;	NES Sound device:	Initaraize
;<<Input>>
;	nothing
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_nes_init
.code
	lda	#$00
	sta	APU_PULSE1CTRL		; Pulse #1 Control Register (W)
;	sta	APU_PULSE1FTUNE		; Pulse #1 Fine Tune (FT) Register (W)
;	sta	APU_PULSE1CTUNE		; Pulse #1 Coarse Tune (CT) Register (W)

	sta	APU_PULSE2CTRL		; Pulse #2 Control Register (W)
;	sta	APU_PULSE2FTUNE		; Pulse #2 Fine Tune Register (W)
;	sta	APU_PULSE2STUNE		; Pulse #2 Coarse Tune Register (W)

	sta	APU_TRICTRL1		; Triangle Control Register (W)
;	sta	APU_TRIFREQ1		; Triangle Frequency Register #1 (W)
;	sta	APU_TRIFREQ2		; Triangle Frequency Register #2 (W)

	sta	APU_NOISECTRL		; Noise Control Register #1 (W)
;	sta	APU_NOISEFREQ1		; Noise Frequency Register #1 (W)
;	sta	APU_NOISEFREQ2		; Noise Frequency Register #2 (W)

;	sta	APU_MODDA		; Delta Modulation D/A Register (W)
;	sta	APU_MODADDR		; Delta Modulation Address Register (W)
;	sta	APU_MODLEN		; Delta Modulation Data Length Register (W)

	lda	#$10
	sta	APU_MODCTRL		; Delta Modulation Control Register (W)

	lda	#$08
	sta	APU_PULSE1RAMP		; Pulse #1 Ramp Control Register (W)
	sta	APU_PULSE2RAMP		; Pulse #2 Ramp Control Register (W)

	lda	#$0F			; 
	sta	APU_CHANCTRL		; Sound/Vertical Clock Signal Register (R)

;	lda	#$80
;	sta	APU_PAD2		; SOFTCLK (RW)

.ifdef	FDS
	
.endif

.ifdef	VRC6
	
.endif

.ifdef	VRC7
	
.endif

.ifdef	MMC5
	
.endif

.ifdef	N163
	
.endif

.ifdef	PSG
	
.endif

	rts
.endproc

;=======================================================================
;	void	__fastcall__	_nsd_nes_keyon(void);
;-----------------------------------------------------------------------
;<<Contents>>
;	Key on the device
;<<Input>>
;	nothing
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_nes_keyon

	;For hardware Key on
	lda	#$FF
	sta	__frequency,x
	sta	__frequency + 1,x
	lda	#$00
	sta	__frequency_set,x
	sta	__frequency_set + 1,x

	rts
.endproc

.proc	_nsd_ch3_keyon

	;For hardware Key on
	lda	#$FF
	sta	__frequency,x
	sta	__frequency + 1,x
	lda	#$00
	sta	__frequency_set,x
	sta	__frequency_set + 1,x

	lda	#$FF
	sta	APU_TRICTRL1

	rts
.endproc

.proc	_nsd_ch3_keyoff
	lda	#$80
	sta	APU_TRICTRL1

	rts
.endproc

.proc	_nsd_dpcm_keyon

	lda	__dpcm_info + 0
	sta	__ptr + 0
	lda	__dpcm_info + 1
	sta	__ptr + 1

	lda	__note,x
	shl	a, 2

	tay
	lda	(__ptr),y
	sta	APU_MODCTRL

	iny
	lda	(__ptr),y
	bmi	@L			;if 0x80 >= a then skip
	sta	APU_MODDA
@L:

	iny
	lda	(__ptr),y
	sta	APU_MODADDR

	iny
	lda	(__ptr),y
	sta	APU_MODLEN

	lda	#$1F
	sta	APU_CHANCTRL

	rts
.endproc

.proc	_nsd_dpcm_keyoff

	;r- ?
	lda	__chflag,x
	and	#nsd_chflag::KeyOff
	beq	Exit

	lda	#$0F
	sta	APU_CHANCTRL
Exit:
	rts
.endproc

;=======================================================================
;	void	__fastcall__	nsd_nes_voice(char voice);
;-----------------------------------------------------------------------
;<<Contents>>
;	NES Sound device:	Set voice
;<<Input>>
;	a	voice	(0:12.5% / 1:25% / 2:50% / 3:75%)
;	x	channel
;<<Output>>
;	nsd_work.Voice.voice_set  =  (voice << 6) | 0x30
;=======================================================================
.proc	_nsd_nes_voice
.code
	;-------------------------------
	; *** Calculate the voice
	shl	a, 6	;a <<= 6
	and	#$C0	;a &= 0xF0	;for OR to volume(lower 4bit)

	;-------------------------------
	; *** Set the voice to work
	sta	__voice_set,x

	;-------------------------------
	; *** Exit
exit:
	rts
.endproc

.proc	_nsd_noise_voice
.code
	;-------------------------------
	; *** Calculate the voice
	shl	a, 7	;a <<= 7
	and	#$80	;a &= 0x80	;for OR to frequency(lower 4bit)

	;-------------------------------
	; *** Set the voice to work
	sta	__voice_set,x

	;-------------------------------
	; *** Exit
exit:
	rts
.endproc

;=======================================================================
;	void	__fastcall__	nsd_nes_volume(char volume);
;-----------------------------------------------------------------------
;<<Contents>>
;	NES Sound device:	Set volume
;<<Input>>
;	a	volume	(0 to 255)	(volume is upper 4bit)
;	x	channel
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_ch1_volume
.code
	;-------------------------------
	; *** Calculate the volume
	;a >>= 4
	shr	a, 4

	;-------------------------------
	; *** Mix voice and volume
	;a = (a & 0x0F) | (nsd_word.Voice.voice_set & 0xF0)
	and	#$0F
	ora	#$30	;a |= 0x30	;counter on / hard-envelop off
	ora	__voice_set,x

	;-------------------------------
	; *** Output to NES sound device
	;y = x << 1
	sta	APU_PULSE1CTRL

	;-------------------------------
	; *** Exit
exit:
	rts
.endproc

.proc	_nsd_ch2_volume
.code
	;-------------------------------
	; *** Calculate the volume
	;a >>= 4
	shr	a, 4

	;-------------------------------
	; *** Mix voice and volume
	;a = (a & 0x0F) | (nsd_word.Voice.voice_set & 0xF0)
	and	#$0F
	ora	#$30	;a |= 0x30	;counter on / hard-envelop off
	ora	__voice_set,x

	;-------------------------------
	; *** Output to NES sound device
	;y = x << 1
	sta	APU_PULSE2CTRL

	;-------------------------------
	; *** Exit
exit:
	rts
.endproc

.proc	_nsd_ch4_volume
.code
	;-------------------------------
	; *** Calculate the volume
	;a >>= 4
	shr	a, 4

	;-------------------------------
	; *** Mix voice and volume
	;a = (a & 0x0F) | (nsd_word.Voice.voice_set & 0xF0)
	and	#$0F
	ora	#$30	;a |= 0x30	;counter on / hard-envelop off

	;-------------------------------
	; *** Output to NES sound device
	;y = x << 1
	sta	APU_NOISECTRL

	;-------------------------------
	; *** Exit
exit:
	rts
.endproc
;=======================================================================
;	void	__fastcall__	nsd_nes_volume(char volume);
;-----------------------------------------------------------------------
;<<Contents>>
;	NES Sound device:	Set volume
;<<Input>>
;	a	volume	(0 to 255)	(volume is upper 4bit)
;	x	channel
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_ch1_sweep
.code
	sta	APU_PULSE1RAMP
	sta	__sweep_ch1
	rts
.endproc

.proc	_nsd_ch2_sweep
.code
	sta	APU_PULSE2RAMP
	sta	__sweep_ch2
	rts
.endproc

.proc	_nsd_se2_sweep
.code
	sta	APU_PULSE2RAMP
	rts
.endproc

;=======================================================================
;	void	__fastcall__	nsd_nes_frequency(int freq);
;-----------------------------------------------------------------------
;<<Contents>>
;	NES Sound device:	Set frequency
;<<Input>>
;	ax	frequency (Range : 0x008E - 0x07FF )	(16 = 100 cent)
;<<Output>>
;	nothing
;=======================================================================
.rodata

;APU, MMC5, VRC6, FME7 Frequency table
Freq:
	.word	$0D4D	;C
	.word	$0D34
	.word	$0D1C
	.word	$0D04
	.word	$0CEC
	.word	$0CD4
	.word	$0CBD
	.word	$0CA5
	.word	$0C8E	;Cis
	.word	$0C77
	.word	$0C60
	.word	$0C49
	.word	$0C32
	.word	$0C1C
	.word	$0C06
	.word	$0BEF
	.word	$0BD9
	.word	$0BC4
	.word	$0BAE
	.word	$0B98
	.word	$0B83
	.word	$0B6E
	.word	$0B59
	.word	$0B44
	.word	$0B2F
	.word	$0B1B
	.word	$0B06
	.word	$0AF2
	.word	$0ADE
	.word	$0ACA
	.word	$0AB6
	.word	$0AA2
	.word	$0A8E
	.word	$0A7B
	.word	$0A68
	.word	$0A54
	.word	$0A41
	.word	$0A2F
	.word	$0A1C
	.word	$0A09
	.word	$09F7
	.word	$09E4
	.word	$09D2
	.word	$09C0
	.word	$09AE
	.word	$099C
	.word	$098B
	.word	$0979
	.word	$0968
	.word	$0956
	.word	$0945
	.word	$0934
	.word	$0923
	.word	$0912
	.word	$0901
	.word	$08F1
	.word	$08E0
	.word	$08D0
	.word	$08C0
	.word	$08B0
	.word	$08A0
	.word	$0890
	.word	$0880
	.word	$0870
	.word	$0861
	.word	$0851
	.word	$0842
	.word	$0833
	.word	$0824
	.word	$0815
	.word	$0806
	.word	$07F7
	.word	$07E8	;A
	.word	$07DA
	.word	$07CB
	.word	$07BD
	.word	$07AF
	.word	$07A1
	.word	$0793
	.word	$0785
	.word	$0777	;B (Bs / Ais)
	.word	$0769
	.word	$075B
	.word	$074E
	.word	$0740
	.word	$0733
	.word	$0726
	.word	$0719
	.word	$070B	;H (B)
	.word	$06FE
	.word	$06F2
	.word	$06E5
	.word	$06D8
	.word	$06CC
	.word	$06BF
	.word	$06B3

;-----------------------
;FDS Frequency table
.ifdef	FDS
Freq_FDS:

.endif

;-----------------------
;SAW Frequency table
.ifdef	VRC6
Freq_SAW:
	.word	$0F33
	.word	$0F17
	.word	$0EFC
	.word	$0EE0
	.word	$0EC5
	.word	$0EA9
	.word	$0E8E
	.word	$0E74
	.word	$0E59
	.word	$0E3F
	.word	$0E24
	.word	$0E0A
	.word	$0DF0
	.word	$0DD7
	.word	$0DBD
	.word	$0DA4
	.word	$0D8B
	.word	$0D72
	.word	$0D59
	.word	$0D41
	.word	$0D28
	.word	$0D10
	.word	$0CF8
	.word	$0CE0
	.word	$0CC8
	.word	$0CB1
	.word	$0C99
	.word	$0C82
	.word	$0C6B
	.word	$0C54
	.word	$0C3D
	.word	$0C27
	.word	$0C11
	.word	$0BFA
	.word	$0BE4
	.word	$0BCE
	.word	$0BB9
	.word	$0BA3
	.word	$0B8E
	.word	$0B78
	.word	$0B63
	.word	$0B4E
	.word	$0B39
	.word	$0B25
	.word	$0B10
	.word	$0AFC
	.word	$0AE8
	.word	$0AD3
	.word	$0AC0
	.word	$0AAC
	.word	$0A98
	.word	$0A85
	.word	$0A71
	.word	$0A5E
	.word	$0A4B
	.word	$0A38
	.word	$0A25
	.word	$0A12
	.word	$0A00
	.word	$09ED
	.word	$09DB
	.word	$09C9
	.word	$09B7
	.word	$09A5
	.word	$0993
	.word	$0982
	.word	$0970
	.word	$095F
	.word	$094D
	.word	$093C
	.word	$092B
	.word	$091A
	.word	$090A
	.word	$08F9
	.word	$08E8
	.word	$08D8
	.word	$08C8
	.word	$08B8
	.word	$08A8
	.word	$0898
	.word	$0888
	.word	$0878
	.word	$0868
	.word	$0859
	.word	$084A
	.word	$083A
	.word	$082B
	.word	$081C
	.word	$080D
	.word	$07FE
	.word	$07F0
	.word	$07E1
	.word	$07D2
	.word	$07C4
	.word	$07B6
	.word	$07A8
.endif


;-----------------------
;VRC7 Frequency table
.ifdef	VRC7
Freq_VRC7:

.endif


;-----------------------
;N168 Frequency table
.ifdef	N163
Freq_N163:

.endif

;-----------------------
;Code
.code
.proc	_nsd_nes_frequency
	;-----------
	;check the old frequency
	ldy	__channel
	cmp	__frequency,y
	beq	@L
	sta	__frequency,y
	txa
	sta	__frequency + 1,y
	jmp	Set_Frequency
@L:	txa
	cmp	__frequency + 1,y
	beq	Exit
	sta	__frequency + 1,y
	tax
Set_Frequency:
	lda	__frequency,y
	jsr	_nsd_div192		; 
	and	#$FE			; x =  ax  /  192
	tay				; y = (ax mod 192) & 0xFE

	;-------------------------------
	; *** Get frequency from table
	; nsd_work_zp._tmp <- frequency
	lda	Freq + 1,y
	sta	__tmp + 1
	lda	Freq,y

	;-------------------------------
	; *** Octave caluclate  and  overflow check
Octave_Proc:
	;if (octave == 0) {
	cpx	#0
	bne	Octave_Loop
	sta	__tmp
	lda	__tmp + 1
	cmp	#$08				;if (frequency >= 0x0800) {
	bcc	@E
	lda	#$07
	sta	__tmp + 1
	lda	#$FF				;	frequency = 0x07FF
	jmp	Octave_Exit			; } else {
@E:	lda	__tmp
	jmp	DEC_Freq
	; } } else { while (octave > 0) {
Octave_Loop:
	lsr	__tmp + 1	; frequency >>= 1
	ror	a
	dex			; octave--;
	bne	Octave_Loop
	; } }
DEC_Freq:
	sub	#1
	bcs	Octave_Exit
	dec	__tmp + 1	; frequency -= 1
Octave_Exit:

Detune:	
	ldx	__channel
	sta	__tmp
	ldy	#$00
	lda	__detune_fine,x
	bpl	@L
	ldy	#$FF			; ay = __detune_fine (sign expand)
@L:	add	__tmp
	sta	__frequency_set,x
	tya
	adc	__tmp + 1
	sta	__tmp + 1		;__tmp += (signed int)__detune_cent

	;-------------------------------
	; *** Output to NES sound device
Set_Device:
	txa
	cmp	#nsd::TR_SE1
	beq	@S1
	cmp	#nsd::TR_SE2
	bne	@E
@S2:	lda	#nsd::TR_BGM4	;SE (Noise ch)
	jmp	@E
@S1:	lda	#nsd::TR_BGM2	;SE (Pulse ch)
@E:	asl	a
	tay

	lda	__frequency_set,x
	sta	APU_PULSE1FTUNE,y
	lda	__tmp + 1
	ora	#$08
	cmp	__frequency_set + 1,x
	beq	Exit
	sta	APU_PULSE1CTUNE,y
	sta	__frequency_set + 1,x

	;-------------------------------
	; *** Exit
Exit:
	rts
.endproc

.proc	_nsd_noise_frequency
.code
	;-------------------------------
	; *** Get the note number lower 4bit
	;a >>= 4
	eor	#$FF
	shr	a,4

	;-------------------------------
	; *** Mix voice and frequency
	;a = (a & 0x0F) | (nsd_word.Voice.voice_set & 0xF0)
	ldx	__channel
	and	#$0F
	ora	__voice_set,x
	
	;-------------------------------
	; *** Output to NES sound device
	sta	APU_NOISEFREQ1
	; to do note on?
	lda	#$08
	cmp	__frequency_set + 1,x	;for flag
	beq	Exit
	sta	APU_NOISEFREQ2
	sta	__frequency_set + 1,x

	;-------------------------------
	; *** Exit
Exit:
	rts
.endproc
