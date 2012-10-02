
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
.code
.proc	_nsd_snd_init

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
	lda	#$00
	sta	VRC6_Frequency
	sta	VRC6_Pulse1_CTRL
	sta	VRC6_Pulse2_CTRL
	sta	VRC6_SAW_CTRL
.endif

.ifdef	VRC7
	;初期化は特に無し？
.endif

.ifdef	MMC5
	lda	#$00
	sta	MMC5_Pulse1_CTRL
	sta	MMC5_Pulse2_CTRL
	lda	#$03
	sta	MMC5_CHANCTRL
.endif

.ifdef	N163
	
.endif

.ifdef	PSG
	
.endif

	rts
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
	.addr	Exit
	.addr	Exit
	.addr	Exit
.endif
.ifdef	VRC7
	.addr	_nsd_vrc7_keyon		;取りあえず、処理しない。
	.addr	_nsd_vrc7_keyon
	.addr	_nsd_vrc7_keyon
	.addr	_nsd_vrc7_keyon
	.addr	_nsd_vrc7_keyon
	.addr	_nsd_vrc7_keyon
.endif
.ifdef	MMC5
	.addr	_nsd_nes_keyon		;仕組みは同じ。
	.addr	_nsd_nes_keyon		;
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
	.addr	Exit
	.addr	Exit
	.addr	Exit
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
	.addr	Exit
	.addr	Exit
	.addr	Exit
.endif
.ifdef	VRC7
	.addr	_nsd_vrc7_keyoff	;KeyOffを書き込んでおく。
	.addr	_nsd_vrc7_keyoff
	.addr	_nsd_vrc7_keyoff
	.addr	_nsd_vrc7_keyoff
	.addr	_nsd_vrc7_keyoff
	.addr	_nsd_vrc7_keyoff
.endif
.ifdef	MMC5
	.addr	Exit
	.addr	Exit
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
	.addr	Exit
	.addr	Exit
	.addr	Exit
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
	.addr	_nes_vrc6_voice
	.addr	_nes_vrc6_voice
	.addr	Exit			;Saw
.endif
.ifdef	VRC7
	.addr	_nes_vrc7_voice
	.addr	_nes_vrc7_voice
	.addr	_nes_vrc7_voice
	.addr	_nes_vrc7_voice
	.addr	_nes_vrc7_voice
	.addr	_nes_vrc7_voice
.endif
.ifdef	MMC5
	.addr	_nsd_nes_voice		;仕組みは同じ
	.addr	_nsd_nes_voice		;
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
	.addr	_nsd_vrc6_ch1_volume
	.addr	_nsd_vrc6_ch2_volume
	.addr	_nsd_vrc6_ch3_volume
.endif
.ifdef	VRC7
	.addr	_nsd_vrc7_volume
	.addr	_nsd_vrc7_volume
	.addr	_nsd_vrc7_volume
	.addr	_nsd_vrc7_volume
	.addr	_nsd_vrc7_volume
	.addr	_nsd_vrc7_volume
.endif
.ifdef	MMC5
	.addr	_nsd_mmc5_ch1_volume
	.addr	_nsd_mmc5_ch2_volume
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
	.addr	Exit
.endif
.ifdef	VRC6
	.addr	Exit
	.addr	Exit
	.addr	Exit
.endif
.ifdef	VRC7
	.addr	_nsd_vrc7_sustain
	.addr	_nsd_vrc7_sustain
	.addr	_nsd_vrc7_sustain
	.addr	_nsd_vrc7_sustain
	.addr	_nsd_vrc7_sustain
	.addr	_nsd_vrc7_sustain
.endif
.ifdef	MMC5
	.addr	Exit
	.addr	Exit
.endif
.ifdef	N163
	.addr	Exit
	.addr	Exit
	.addr	Exit
	.addr	Exit
	.addr	Exit
	.addr	Exit
	.addr	Exit
	.addr	Exit
.endif
.ifdef	PSG
	.addr	Exit
	.addr	Exit
	.addr	Exit
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
JMPTBL:	.addr	_nsd_nes_ch1_frequency	;BGM ch1 Pulse
	.addr	_nsd_nes_ch2_frequency	;BGM ch2 Pulse
	.addr	_nsd_nes_ch3_frequency	;BGM ch3 Triangle
	.addr	_nsd_nes_ch4_frequency	;BGM ch4 Noise
	.addr	Exit			;BGM ch5 DPCM
.ifdef	FDS
;	.addr	
.endif
.ifdef	VRC6
	.addr	_nsd_vrc6_ch1_frequency
	.addr	_nsd_vrc6_ch2_frequency
	.addr	_nsd_vrc6_ch3_frequency
.endif
.ifdef	VRC7
	.addr	_nsd_vrc7_frequency
	.addr	_nsd_vrc7_frequency
	.addr	_nsd_vrc7_frequency
	.addr	_nsd_vrc7_frequency
	.addr	_nsd_vrc7_frequency
	.addr	_nsd_vrc7_frequency
.endif
.ifdef	MMC5
	.addr	_nsd_mmc5_ch1_frequency
	.addr	_nsd_mmc5_ch2_frequency
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
	.addr	_nsd_nes_ch2_frequency	;SE  ch1 Pulse
	.addr	_nsd_nes_ch4_frequency	;SE  ch2 Noise
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
	lda	__frequency,y		;ax ← __frequency

	jmp	(__ptr)
Exit:
	ldx	__channel
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
	lda	#$00
	sta	__frequency_set,x
	sta	__frequency_set + 1,x

	rts
.endproc

;---------------------------------------
.proc	_nsd_ch3_keyon

	;For hardware Key on
	lda	#$00
	sta	__frequency_set,x
	sta	__frequency_set + 1,x

	lda	#$FF
	sta	APU_TRICTRL1

	rts
.endproc

;---------------------------------------
.proc	_nsd_ch3_keyoff

	lda	#$80
	sta	APU_TRICTRL1

	rts
.endproc

;---------------------------------------
.proc	_nsd_dpcm_keyon

	lda	#$0F
	sta	APU_CHANCTRL

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

;---------------------------------------
.proc	_nsd_dpcm_keyoff

	;r- ?
	lda	__chflag,x
	and	#nsd_chflag::KeyOff
	bne	Exit

	lda	#$0F
	sta	APU_CHANCTRL

	lda	#$00
	sta	APU_MODDA

	lda	#$00
	sta	APU_MODLEN
Exit:
	rts
.endproc

;---------------------------------------
.ifdef	VRC7
.proc	_nsd_vrc7_keyon

	lda	__chflag,x
	ora	#nsd_chflag::KeyOn
	sta	__chflag,x

	rts
.endproc

;---------------------------------------
.proc	_nsd_vrc7_keyoff

;	取りあえず、必ずKeyOffする。
	lda	__chflag,x
	and	#~nsd_chflag::KeyOn
	sta	__chflag,x

	;書かれない可能性があるので、ここで書く。
	txa
	sub	#nsd::TR_VRC7
	shr	a, 1
	add	#VRC7_Octave
	sta	VRC7_Resister		;レジスターをセット

	lda	__frequency_set + 1,x	;[5]
	and	#$EF			;[2]
	sta	VRC7_Data		;

;Exit:
	rts
.endproc
.endif
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

;---------------------------------------
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

;---------------------------------------
.ifdef	VRC6
.proc	_nes_vrc6_voice
	;-------------------------------
	; *** Calculate the voice
	shl	a, 4	;a <<= 6
	and	#$70	;a &= 0xF0	;for OR to volume(lower 4bit)

	;-------------------------------
	; *** Set the voice to work
	sta	__voice_set,x

	;-------------------------------
	; *** Exit
exit:
	rts
.endproc
.endif

;---------------------------------------
.ifdef	VRC7
.proc	_nes_vrc7_voice
	;-------------------------------
	; *** Calculate the voice
	shl	a, 4	;a <<= 6
	and	#$F0	;a &= 0xF0	;for OR to volume(lower 4bit)

	;-------------------------------
	; *** Set the voice to work
	sta	__voice_set,x

	;-------------------------------
	; *** Exit
exit:
	rts
.endproc
.endif
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

;---------------------------------------
.proc	_nsd_ch2_volume
.code

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

;---------------------------------------
.proc	_nsd_ch4_volume
.code

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



;---------------------------------------
.ifdef	VRC6
.proc	_nsd_vrc6_ch1_volume

	;-------------------------------
	; *** Mix voice and volume
	;a = (a & 0x0F) | (nsd_word.Voice.voice_set & 0xF0)
	and	#$0F
	ora	__voice_set,x

	;-------------------------------
	; *** Output to NES sound device
	;y = x << 1
	sta	VRC6_Pulse1_CTRL

	;-------------------------------
	; *** Exit
exit:
	rts
.endproc

;---------------------------------------
.proc	_nsd_vrc6_ch2_volume

	;-------------------------------
	; *** Mix voice and volume
	;a = (a & 0x0F) | (nsd_word.Voice.voice_set & 0xF0)
	and	#$0F
	ora	__voice_set,x

	;-------------------------------
	; *** Output to NES sound device
	;y = x << 1
	sta	VRC6_Pulse2_CTRL

	;-------------------------------
	; *** Exit
exit:
	rts
.endproc

;---------------------------------------
.proc	_nsd_vrc6_ch3_volume
	sta	VRC6_SAW_CTRL
	rts
.endproc

.endif



;---------------------------------------
.ifdef	VRC7
.proc	_nsd_vrc7_volume

	eor	#$FF
	tay

	txa
	sub	#nsd::TR_VRC7
	shr	a, 1
	add	#VRC7_Volume
	sta	VRC7_Resister		;

	tya				;[2]
	and	#$0F			;[2]
	ora	__voice_set,x		;[4]

	sta	VRC7_Data

	rts
.endproc
.endif



;---------------------------------------
.ifdef	MMC5
.proc	_nsd_mmc5_ch1_volume

	;-------------------------------
	; *** Mix voice and volume
	;a = (a & 0x0F) | (nsd_word.Voice.voice_set & 0xF0)
	and	#$0F
	ora	#$30	;a |= 0x30	;counter on / hard-envelop off
	ora	__voice_set,x

	;-------------------------------
	; *** Output to NES sound device
	;y = x << 1
	sta	MMC5_Pulse1_CTRL

	;-------------------------------
	; *** Exit
exit:
	rts
.endproc

;---------------------------------------
.proc	_nsd_mmc5_ch2_volume

	;-------------------------------
	; *** Mix voice and volume
	;a = (a & 0x0F) | (nsd_word.Voice.voice_set & 0xF0)
	and	#$0F
	ora	#$30	;a |= 0x30	;counter on / hard-envelop off
	ora	__voice_set,x

	;-------------------------------
	; *** Output to NES sound device
	;y = x << 1
	sta	MMC5_Pulse2_CTRL

	;-------------------------------
	; *** Exit
exit:
	rts
.endproc
.endif

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

;---------------------------------------
.proc	_nsd_ch2_sweep
.code
	sta	APU_PULSE2RAMP
	sta	__sweep_ch2
	rts
.endproc

;---------------------------------------
.proc	_nsd_se2_sweep
.code
	sta	APU_PULSE2RAMP
	rts
.endproc

;---------------------------------------
.ifdef	VRC7
.proc	_nsd_vrc7_sustain

	tay
	lda	__chflag,x
	cpy	#0
	bne	@L
	and	#~nsd_chflag::Sustain
	jmp	@Set
@L:	ora	#nsd_chflag::Sustain
@Set:	sta	__chflag,x

	rts
.endproc
.endif
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
	.byte	$AD
	.byte	$AE
	.byte	$AF
	.byte	$B1
	.byte	$B2
	.byte	$B3
	.byte	$B4
	.byte	$B6
	.byte	$B7
	.byte	$B8
	.byte	$BA
	.byte	$BB
	.byte	$BC
	.byte	$BE
	.byte	$BF
	.byte	$C1
	.byte	$C2
	.byte	$C3
	.byte	$C5
	.byte	$C6
	.byte	$C8
	.byte	$C9
	.byte	$CB
	.byte	$CC
	.byte	$CD
	.byte	$CF
	.byte	$D0
	.byte	$D2
	.byte	$D4
	.byte	$D5
	.byte	$D7
	.byte	$D8
	.byte	$DA
	.byte	$DB
	.byte	$DD
	.byte	$DF
	.byte	$E0
	.byte	$E2
	.byte	$E3
	.byte	$E5
	.byte	$E7
	.byte	$E8
	.byte	$EA
	.byte	$EC
	.byte	$ED
	.byte	$EF
	.byte	$F1
	.byte	$F3
	.byte	$F4
	.byte	$F6
	.byte	$F8
	.byte	$FA
	.byte	$FC
	.byte	$FD
	.byte	$FF
	.byte	$01
	.byte	$03
	.byte	$05
	.byte	$07
	.byte	$09
	.byte	$0B
	.byte	$0D
	.byte	$0F
	.byte	$10
	.byte	$12
	.byte	$14
	.byte	$16
	.byte	$18
	.byte	$1B
	.byte	$1D
	.byte	$1F
	.byte	$21
	.byte	$23
	.byte	$25
	.byte	$27
	.byte	$29
	.byte	$2B
	.byte	$2E
	.byte	$30
	.byte	$32
	.byte	$34
	.byte	$36
	.byte	$39
	.byte	$3B
	.byte	$3D
	.byte	$3F
	.byte	$42
	.byte	$44
	.byte	$46
	.byte	$49
	.byte	$4B
	.byte	$4E
	.byte	$50
	.byte	$53
	.byte	$55
	.byte	$57
.endif



;-----------------------
;N163 Frequency table
.ifdef	N163
Freq_N163:
	.word	$4168
	.word	$437F
	.word	$4597
	.word	$47B2
	.word	$49CE
	.word	$4BEC
	.word	$4E0D
	.word	$502F
	.word	$5253
	.word	$547A
	.word	$56A2
	.word	$58CC
	.word	$5AF8
	.word	$5D27
	.word	$5F57
	.word	$6189
	.word	$63BE
	.word	$65F4
	.word	$682D
	.word	$6A67
	.word	$6CA4
	.word	$6EE2
	.word	$7123
	.word	$7366
	.word	$75AB
	.word	$77F2
	.word	$7A3B
	.word	$7C86
	.word	$7ED3
	.word	$8123
	.word	$8374
	.word	$85C8
	.word	$881E
	.word	$8A76
	.word	$8CD0
	.word	$8F2D
	.word	$918B
	.word	$93EC
	.word	$964F
	.word	$98B4
	.word	$9B1C
	.word	$9D85
	.word	$9FF1
	.word	$A25F
	.word	$A4D0
	.word	$A742
	.word	$A9B7
	.word	$AC2F
	.word	$AEA8
	.word	$B124
	.word	$B3A2
	.word	$B622
	.word	$B8A5
	.word	$BB2A
	.word	$BDB1
	.word	$C03B
	.word	$C2C7
	.word	$C555
	.word	$C7E6
	.word	$CA79
	.word	$CD0F
	.word	$CFA7
	.word	$D241
	.word	$D4DE
	.word	$D77D
	.word	$DA1E
	.word	$DCC2
	.word	$DF69
	.word	$E212
	.word	$E4BD
	.word	$E76B
	.word	$EA1B
	.word	$ECCE
	.word	$EF83
	.word	$F23B
	.word	$F4F5
	.word	$F7B2
	.word	$FA71
	.word	$FD33
	.word	$FFF8
	.word	$02BF		;$50
	.word	$0589
	.word	$0855
	.word	$0B23
	.word	$0DF5
	.word	$10C9
	.word	$139F
	.word	$1679
	.word	$1955
	.word	$1C33
	.word	$1F14
	.word	$21F8
	.word	$24DF
	.word	$27C8
	.word	$2AB4
	.word	$2DA2
	.word	$3094
	.word	$3388
	.word	$367E
	.word	$3978
	.word	$3C74
	.word	$3F73
	.word	$4275
	.word	$457A
	.word	$4881
	.word	$4B8B
	.word	$4E98
	.word	$51A8
	.word	$54BB
	.word	$57D0
	.word	$5AE9
	.word	$5E04
	.word	$6122
	.word	$6443
	.word	$6767
	.word	$6A8E
	.word	$6DB7
	.word	$70E4
	.word	$7414
	.word	$7746
	.word	$7A7C
	.word	$7DB4
	.word	$80F0
	.word	$842E
	.word	$8770
	.word	$8AB4
	.word	$8DFC
	.word	$9146
	.word	$9494
	.word	$97E4
	.word	$9B38
	.word	$9E8F
	.word	$A1E9
	.word	$A546
	.word	$A8A6
	.word	$AC09
	.word	$AF6F
	.word	$B2D9
	.word	$B645
	.word	$B9B5
	.word	$BD28
	.word	$C09E
	.word	$C418
	.word	$C794
	.word	$CB14
	.word	$CE97
	.word	$D21E
	.word	$D5A7
	.word	$D934
	.word	$DCC4
	.word	$E058
	.word	$E3EF
	.word	$E789
	.word	$EB26
	.word	$EEC7
	.word	$F26B
	.word	$F613
	.word	$F9BE
	.word	$FD6C
	.word	$011E		;$9F
	.word	$04D3
	.word	$088B
	.word	$0C47
	.word	$1007
	.word	$13CA
	.word	$1790
	.word	$1B5A
	.word	$1F27
	.word	$22F8
	.word	$26CD
	.word	$2AA5
	.word	$2E80
	.word	$325F
	.word	$3642
	.word	$3A28
	.word	$3E12
	.word	$4200
	.word	$45F1
	.word	$49E6
	.word	$4DDF
	.word	$51DB
	.word	$55DB
	.word	$59DE
	.word	$5DE5
	.word	$61F0
	.word	$65FF
	.word	$6A12
	.word	$6E28
	.word	$7242
	.word	$7660
	.word	$7A82
	.word	$7EA7
.endif

;-----------------------
;Code
.code
;---------------------------------------
.proc	_nsd_nes_ch1_frequency

	jsr	Normal_frequency

	lda	__tmp
	sta	__frequency_set,x
	sta	APU_PULSE1FTUNE
	lda	__tmp + 1
	ora	#$08
	cmp	__frequency_set + 1,x
	beq	Exit
	sta	APU_PULSE1CTUNE
	sta	__frequency_set + 1,x

Exit:
	rts
.endproc

;---------------------------------------
.proc	_nsd_nes_ch2_frequency

	jsr	Normal_frequency

	lda	__tmp
	sta	__frequency_set,x
	sta	APU_PULSE2FTUNE
	lda	__tmp + 1
	ora	#$08
	cmp	__frequency_set + 1,x
	beq	Exit
	sta	APU_PULSE2STUNE		;nes.inc 間違えてる。
	sta	__frequency_set + 1,x

Exit:
	rts
.endproc

;---------------------------------------
.proc	_nsd_nes_ch3_frequency

	jsr	Normal_frequency

	lda	__tmp
	sta	__frequency_set,x
	sta	APU_TRIFREQ1
	lda	__tmp + 1
	ora	#$08
	cmp	__frequency_set + 1,x
	beq	Exit
	sta	APU_TRIFREQ2
	sta	__frequency_set + 1,x

Exit:
	rts
.endproc

;---------------------------------------
.proc	_nsd_nes_ch4_frequency
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
;	cmp	__frequency_set + 1,x	;for flag
;	beq	Exit
	sta	APU_NOISEFREQ2		;Length counter load (L) 
;	sta	__frequency_set + 1,x

	;-------------------------------
	; *** Exit
Exit:
	rts
.endproc

;---------------------------------------
.ifdef	VRC6
.proc	_nsd_vrc6_ch1_frequency
	jsr	Normal_frequency

	lda	__tmp
	sta	VRC6_Pulse1_FTUNE
	lda	__tmp + 1
	ora	#$80
	sta	VRC6_Pulse1_CTUNE

	rts
.endproc

;---------------------------------------
.proc	_nsd_vrc6_ch2_frequency
	jsr	Normal_frequency

	lda	__tmp
	sta	VRC6_Pulse2_FTUNE
	lda	__tmp + 1
	ora	#$80
	sta	VRC6_Pulse2_CTUNE

	rts
.endproc

;---------------------------------------
.proc	_nsd_vrc6_ch3_frequency

	jsr	_nsd_div192		; 
	and	#$FE			; x =  ax  /  192
	tay				; y = (ax mod 192) & 0xFE

	;-------------------------------
	; *** Get frequency from table
	lda	Freq_SAW + 1,y
	sta	__tmp + 1
	lda	Freq_SAW,y
	cpx	#0
	beq	DEC_Freq
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
	sta	__tmp
	tya
	adc	__tmp + 1
	sta	__tmp + 1		;__tmp += (signed int)__detune_cent

	lda	__tmp
	sta	VRC6_SAW_FTUNE
	lda	__tmp + 1
	ora	#$80
	sta	VRC6_SAW_CTUNE

	rts
.endproc
.endif



;---------------------------------------
.ifdef	VRC7
.proc	_nsd_vrc7_frequency

	;除算
	jsr	_nsd_div192		;Wait変わりに使える？
	stx	__tmp + 1
	cmp	#$6E
	rol	__tmp + 1		;__tmp + 1 = (Octave << 1) + Note_MSB

	shr	a, 1			;
	tay				;y = (ax mod 192) >> 1
	lda	Freq_VRC7,y		;
	sta	__tmp			;__tmp + 0 = Note_LSB

	;チャンネルの計算
	ldx	__channel
	txa
	sub	#nsd::TR_VRC7
	shr	a, 1
	pha				;push VRC7でのチャンネル番号
	;オクターブ
	add	#VRC7_Frequency
	sta	VRC7_Resister		;●Resister Write

Detune:	
	ldy	#$00			;[2]
	lda	__detune_fine,x		;[4]6
	bpl	@L			;[2]8
	ldy	#$FF			;	ay = __detune_fine (sign expand)
@L:	add	__tmp			;[5]13	clock > 6 clock
	sta	VRC7_Data		;●Data Write
	sta	__frequency_set + 0,x	;[5]
	tya				;[2]7
	adc	__tmp + 1		;[3]10
	and	#$0F			;[2]12
	sta	__tmp + 1		;[3]15	__tmp += (signed int)__detune_cent
	lda	__chflag,x		;[4]19
	and	#$30			;[2]21	 00XX 0000 <2>
	ora	__tmp + 1		;[3]24	flag と octave をマージ
	sta	__tmp + 1		;[3]27

	pha				;[3]30
	pla				;[4]34	

	pla				;[4]38	a ← VRC7でのチャンネル番号
	add	#VRC7_Octave		;[4]42 clock !! (VRC7のwait)

	sta	VRC7_Resister		;●Resister Write
	lda	__tmp + 1		;[3]3
	sta	__frequency_set + 1,x	;[5]8 clock > 6 clock
	sta	VRC7_Data		;●Data Write

	rts
.endproc
.endif



;---------------------------------------
.ifdef	MMC5
.proc	_nsd_mmc5_ch1_frequency
	jsr	MMC5_frequency

	lda	__tmp
	sta	__frequency_set,x
	sta	MMC5_Pulse1_FTUNE
	lda	__tmp + 1
	ora	#$08
	cmp	__frequency_set + 1,x
	beq	Exit
	sta	MMC5_Pulse1_CTUNE
	sta	__frequency_set + 1,x

Exit:
	rts
.endproc

;---------------------------------------
.proc	_nsd_mmc5_ch2_frequency
	jsr	MMC5_frequency

	lda	__tmp
	sta	__frequency_set,x
	sta	MMC5_Pulse2_FTUNE
	lda	__tmp + 1
	ora	#$08
	cmp	__frequency_set + 1,x
	beq	Exit
	sta	MMC5_Pulse2_CTUNE
	sta	__frequency_set + 1,x

Exit:
	rts
.endproc
.endif



;=======================================================================
;	void	__fastcall__	Normal_frequency(int freq);
;-----------------------------------------------------------------------
;<<Input>>
;	ax	frequency (Range : 0x008E - 0x07FF )	(16 = 100 cent)
;<<Output>>
;	__frequency_set		frequency lower 8bit
;	__tmp + 1		frequency upper 8bit
;=======================================================================
.proc	Normal_frequency

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
	sta	__tmp
	tya
	adc	__tmp + 1
	sta	__tmp + 1		;__tmp += (signed int)__detune_cent

	;-------------------------------
	; *** Exit
Exit:
	rts
.endproc



.ifdef	MMC5
;=======================================================================
;	void	__fastcall__	MMC5_frequency(int freq);
;-----------------------------------------------------------------------
;<<Input>>
;	ax	frequency (Range : 0x008E - 0x07FF )	(16 = 100 cent)
;<<Output>>
;	__frequency_set		frequency lower 8bit
;	__tmp + 1		frequency upper 8bit
;=======================================================================
.proc	MMC5_frequency

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
	cpx	#0
	beq	DEC_Freq
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
	sta	__tmp
	tya
	adc	__tmp + 1
	sta	__tmp + 1		;__tmp += (signed int)__detune_cent

	;-------------------------------
	; *** Exit
Exit:
	rts
.endproc
.endif

