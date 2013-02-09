;=======================================================================
;
;	NES Sound Driver Libraly		Sound Device Driver
;
;=======================================================================

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

.ifdef	FDS
	sta	FDS_Sweep_Bias
.endif

.ifdef	MMC5
	sta	MMC5_Pulse1_CTRL
	sta	MMC5_Pulse2_CTRL
.endif

.ifdef	VRC6
	sta	VRC6_Frequency
	sta	VRC6_Pulse1_CTRL
	sta	VRC6_Pulse2_CTRL
	sta	VRC6_SAW_CTRL
.endif

.ifdef	FDS
	lda	#$02
	sta	FDS_Control
.endif

.ifdef	MMC5
	lda	#$03
	sta	MMC5_CHANCTRL
.endif

	lda	#$08
	sta	APU_PULSE1RAMP		; Pulse #1 Ramp Control Register (W)
	sta	APU_PULSE2RAMP		; Pulse #2 Ramp Control Register (W)

	lda	#$0F			; 
	sta	APU_CHANCTRL		; Sound/Vertical Clock Signal Register (R)

	lda	#$10
	sta	APU_MODCTRL		; Delta Modulation Control Register (W)

;	lda	#$80
;	sta	APU_PAD2		; SOFTCLK (RW)

.ifdef	N163
;	lda	#$20
;	sta				; namco t163 sound enable
.endif

.ifdef	FDS
	lda	#$80
	sta	FDS_Volume
	sta	FDS_Sweep_Envelope
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
JMPTBL:	.addr	_nsd_ch1_keyon		;BGM ch1 Pulse
	.addr	_nsd_ch2_keyon		;BGM ch2 Pulse
	.addr	_nsd_ch3_keyon		;BGM ch3 Triangle
	.addr	Exit			;BGM ch4 Noize
	.addr	_nsd_dpcm_keyon		;BGM ch5 DPCM
.ifdef	FDS
	.addr	_nsd_fds_keyon
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
.ifdef	OPLL
	.addr	_nsd_vrc7_keyon
	.addr	_nsd_vrc7_keyon
	.addr	_nsd_vrc7_keyon
	.addr	_nsd_vrc7_keyon
	.addr	_nsd_vrc7_keyon
	.addr	_nsd_vrc7_keyon
	.addr	_nsd_opll_keyon_R
	.addr	_nsd_opll_keyon_R
	.addr	_nsd_opll_keyon_R
	.addr	_nsd_opll_BD_keyon
	.addr	_nsd_opll_SD_keyon
	.addr	_nsd_opll_HH_keyon
	.addr	_nsd_opll_Cym_keyon
	.addr	_nsd_opll_Tom_keyon
.endif
.ifdef	MMC5
	.addr	_nsd_mmc5_ch1_keyon		;仕組みは同じ。
	.addr	_nsd_mmc5_ch2_keyon		;
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
.ifdef	NULL
	.addr	Exit
.endif
	.addr	_nsd_se_keyon		;SE  ch1 Pulse
	.addr	Exit			;SE  ch2 Noize

;---------------------------------------
.code
	ldy	JMPTBL,x		;
	sty	__ptr			;
	ldy	JMPTBL + 1,x		;
	sty	__ptr + 1		;
	jmp	(__ptr)			;[5]

;---------------------------------------
_nsd_ch1_keyon:
	lda	#$00
	sta	__apu_frequency1
Exit:
	rts

_nsd_ch2_keyon:
	lda	#$00
	sta	__apu_frequency2
	rts

_nsd_ch3_keyon:
	;Hardware key off for ch3
	lda	#$FF
	sta	APU_TRICTRL1

	lda	#$00
	sta	__apu_frequency3
	rts

_nsd_se_keyon:
	;For hardware Key on
	lda	#$00
	sta	__se_frequency
	rts

;---------------------------------------
_nsd_dpcm_keyon:

	lda	#$0F
	sta	APU_CHANCTRL

	lda	#0
	sta	__tmp + 1
	lda	__note,x
	asl	a
	rol	__tmp + 1
.ifdef	DPCMBank
	sta	__tmp
.endif
	asl	a
	rol	__tmp + 1
.ifdef	DPCMBank
	add	__tmp
.endif
	tay			;y <- address
.ifdef	DPCMBank
	lda	#0
	adc	__tmp + 1
	sta	__tmp + 1
.endif

	lda	__dpcm_info + 0
	sta	__ptr + 0
	lda	__dpcm_info + 1
	add	__tmp + 1
	sta	__ptr + 1

	;Set the DPCM resister
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

.ifdef	DPCMBank

.endif

	rts

;---------------------------------------
.ifdef	FDS
_nsd_fds_keyon:
	lda	__fds_sweepbias
	sta	FDS_Sweep_Bias
	rts
.endif
;---------------------------------------
.ifdef	MMC5

_nsd_mmc5_ch1_keyon:
	lda	#$00
	sta	__mmc5_frequency1
	rts

_nsd_mmc5_ch2_keyon:
	lda	#$00
	sta	__mmc5_frequency2
	rts

.endif
;---------------------------------------
.if	.defined(OPLL)
_nsd_opll_keyon_R:

	;OPLL_Rhythm check
	lda	__opll_ryhthm
	cmp	#$20
	bcs	_nsd_vrc7_keyon_exit
.endif
;---------------------------------------
.if	.defined(VRC7) || .defined(OPLL)
_nsd_vrc7_keyon:

	lda	__chflag,x
	ora	#nsd_chflag::KeyOn
	sta	__chflag,x

_nsd_vrc7_keyon_exit:
	rts
.endif
.endproc


;---------------------------------------
.ifdef	OPLL

.proc	_nsd_opll_BD_keyon
	lda	__opll_ryhthm
	ora	#$10
	sta	__opll_ryhthm
	jmp	_nsd_opll_rhythm_set
.endproc

.proc	_nsd_opll_SD_keyon
	lda	__opll_ryhthm
	ora	#$08
	sta	__opll_ryhthm
	jmp	_nsd_opll_rhythm_set
.endproc

.proc	_nsd_opll_HH_keyon
	lda	__opll_ryhthm
	ora	#$01
	sta	__opll_ryhthm
	jmp	_nsd_opll_rhythm_set
.endproc

.proc	_nsd_opll_Cym_keyon
	lda	__opll_ryhthm
	ora	#$02
	sta	__opll_ryhthm
	jmp	_nsd_opll_rhythm_set
.endproc

.proc	_nsd_opll_Tom_keyon
	lda	__opll_ryhthm
	ora	#$04
	sta	__opll_ryhthm
	jmp	_nsd_opll_rhythm_set
.endproc

.proc	_nsd_opll_BD_keyoff
	lda	__opll_ryhthm
	and	#~$10
	sta	__opll_ryhthm
	jmp	_nsd_opll_rhythm_set
.endproc

.proc	_nsd_opll_SD_keyoff
	lda	__opll_ryhthm
	and	#~$08
	sta	__opll_ryhthm
	jmp	_nsd_opll_rhythm_set
.endproc

.proc	_nsd_opll_HH_keyoff
	lda	__opll_ryhthm
	and	#~$01
	sta	__opll_ryhthm
	jmp	_nsd_opll_rhythm_set
.endproc

.proc	_nsd_opll_Cym_keyoff
	lda	__opll_ryhthm
	and	#~$02
	sta	__opll_ryhthm
	jmp	_nsd_opll_rhythm_set
.endproc

.proc	_nsd_opll_Tom_keyoff
	lda	__opll_ryhthm
	and	#~$04
	sta	__opll_ryhthm
	jmp	_nsd_opll_rhythm_set
.endproc

.proc	_nsd_opll_rhythm_set
	;OPLL_Rhythm check
	ldy	__opll_ryhthm
	cpy	#$20
	bcc	Exit

	ldy	#OPLL_RHYTHM
	sty	OPLL_Resister
	lda	__opll_ryhthm
	tay
	tay
	sta	OPLL_Data
Exit:
	rts
.endproc
.endif

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
	.addr	Exit
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
.ifdef	OPLL
	.addr	_nsd_OPLL_keyoff
	.addr	_nsd_OPLL_keyoff
	.addr	_nsd_OPLL_keyoff
	.addr	_nsd_OPLL_keyoff
	.addr	_nsd_OPLL_keyoff
	.addr	_nsd_OPLL_keyoff
	.addr	_nsd_OPLL_keyoff_R
	.addr	_nsd_OPLL_keyoff_R
	.addr	_nsd_OPLL_keyoff_R
	.addr	_nsd_opll_BD_keyoff
	.addr	_nsd_opll_SD_keyoff
	.addr	_nsd_opll_HH_keyoff
	.addr	_nsd_opll_Cym_keyoff
	.addr	_nsd_opll_Tom_keyoff
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
.ifdef	NULL
	.addr	Exit
.endif
	.addr	Exit			;SE  ch1 Pulse
	.addr	Exit			;SE  ch2 Noize		-- no process --

;---------------------------------------
.code
	ldy	JMPTBL,x		;[]
	sty	__ptr			;[]
	ldy	JMPTBL + 1,x		;[]
	sty	__ptr + 1		;[]
	jmp	(__ptr)			;[5]

;---------------------------------------
_nsd_ch3_keyoff:
	lda	#$80
	sta	APU_TRICTRL1
Exit:
	rts

;---------------------------------------
_nsd_dpcm_keyoff:

	;r- ?
	lda	__chflag,x
	and	#nsd_chflag::KeyOff
	bne	Exit

	lda	#$0F
	sta	APU_CHANCTRL

	rts

;---------------------------------------
.ifdef	VRC7
_nsd_vrc7_keyoff:

	;取りあえず、必ずKeyOffする。
	lda	__chflag,x		;[4]4
	and	#~nsd_chflag::KeyOn	;[2]6
	sta	__chflag,x		;[4]10

	;書かれない可能性があるので、ここでレジスタに書く。
	txa				;[2]12
	sub	#nsd::TR_VRC7		;[]
	shr	a, 1			;[2]
	tay				;[2]
	add	#VRC7_Octave		;[2]
	sta	VRC7_Resister		;レジスターをセット

	lda	__vrc7_frequency,y	;[5]
	and	#$EF			;[2]
	sta	VRC7_Data		;

	rts				;[6]
.endif

;---------------------------------------
.ifdef	OPLL
_nsd_OPLL_keyoff_R:

	;OPLL_Rhythm check
	lda	__opll_ryhthm
	cmp	#$20
	bcs	_nsd_OPLL_keyoff_exit

_nsd_OPLL_keyoff:

	;取りあえず、必ずKeyOffする。
	lda	__chflag,x
	and	#~nsd_chflag::KeyOn
	sta	__chflag,x

	;書かれない可能性があるので、ここでレジスタに書く。
	txa
	sub	#nsd::TR_OPLL
	shr	a, 1
	tay
	add	#OPLL_Octave
	sta	OPLL_Resister		;レジスターをセット

	lda	__opll_frequency,y	;[5]
	and	#$EF			;[2]
	sta	OPLL_Data		;

_nsd_OPLL_keyoff_exit:
	rts
.endif
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
	.addr	_nsd_fds_GainMod
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
.ifdef	OPLL
	.addr	_nes_vrc7_voice
	.addr	_nes_vrc7_voice
	.addr	_nes_vrc7_voice
	.addr	_nes_vrc7_voice
	.addr	_nes_vrc7_voice
	.addr	_nes_vrc7_voice
	.addr	_nes_vrc7_voice
	.addr	_nes_vrc7_voice
	.addr	_nes_vrc7_voice
	.addr	Exit
	.addr	Exit
	.addr	Exit
	.addr	Exit
	.addr	Exit
.endif
.ifdef	MMC5
	.addr	_nsd_nes_voice		;仕組みは同じ
	.addr	_nsd_nes_voice		;
.endif
.ifdef	N163
	.addr	_nsd_n163_ch1_voice
	.addr	_nsd_n163_ch2_voice
	.addr	_nsd_n163_ch3_voice
	.addr	_nsd_n163_ch4_voice
	.addr	_nsd_n163_ch5_voice
	.addr	_nsd_n163_ch6_voice
	.addr	_nsd_n163_ch7_voice
	.addr	_nsd_n163_ch8_voice
.endif
.ifdef	PSG
	.addr	_nsd_psg_ch1_voice
	.addr	_nsd_psg_ch2_voice
	.addr	_nsd_psg_ch3_voice
.endif
.ifdef	NULL
	.addr	Exit
.endif
	.addr	_nsd_nes_voice		;SE  ch1 Pulse
	.addr	_nsd_noise_voice	;SE  ch2 Noize

;---------------------------------------
.code
	ldy	JMPTBL,x
	sty	__ptr
	ldy	JMPTBL + 1,x
	sty	__ptr + 1
	jmp	(__ptr)

;---------------------------------------
_nsd_nes_voice:

	;-------------------------------
	; *** Calculate the voice
	shl	a, 6	;a <<= 6
	and	#$C0	;a &= 0xF0	;for OR to volume(lower 4bit)

	;-------------------------------
	; *** Set the voice to work
	sta	__voice_set,x

	;-------------------------------
	; *** Exit
Exit:
	rts

;---------------------------------------
_nsd_noise_voice:

	;-------------------------------
	; *** Calculate the voice
	shl	a, 7	;a <<= 7
	and	#$80	;a &= 0x80	;for OR to frequency(lower 4bit)

	;-------------------------------
	; *** Set the voice to work
	sta	__voice_set,x

	;-------------------------------
	; *** Exit
	rts

;---------------------------------------
.ifdef	FDS
_nsd_fds_GainMod:
	;-------------------------------
	; *** Calculate the voice
	and	#$3F
	ora	#$80

	;-------------------------------
	; *** Set the voice to work
	sta	FDS_Sweep_Envelope

	;-------------------------------
	; *** Exit
	rts
.endif

;---------------------------------------
.ifdef	VRC6
_nes_vrc6_voice:
	;-------------------------------
	; *** Calculate the voice
	shl	a, 4	;a <<= 6
	and	#$70	;a &= 0xF0	;for OR to volume(lower 4bit)

	;-------------------------------
	; *** Set the voice to work
	sta	__voice_set,x

	;-------------------------------
	; *** Exit
	rts

.endif

;---------------------------------------
.if	.defined(VRC7) || .defined(OPLL)
_nes_vrc7_voice:
	;-------------------------------
	; *** Calculate the voice
	shl	a, 4	;a <<= 6
	and	#$F0	;a &= 0xF0	;for OR to volume(lower 4bit)

	;-------------------------------
	; *** Set the voice to work
	sta	__voice_set,x

	;-------------------------------
	; *** Exit
	rts
.endif

;---------------------------------------
.ifdef	N163
_nsd_n163_ch1_voice:
	ldy	#N163_Waveform - (8 * 0)

_nsd_n163_ch1_voice_set:
	sty	N163_Resister
	shl	a, 2
	sta	N163_Data

_nsd_n163_ch1_voice_exit:
	rts

_nsd_n163_ch2_voice:
	ldy	__n163_num
	cpy	#$10
	bcc	_nsd_n163_ch1_voice_exit	;1未満だったら終了
	ldy	#N163_Waveform - (8 * 1)
	jmp	_nsd_n163_ch1_voice_set

_nsd_n163_ch3_voice:
	ldy	__n163_num
	cpy	#$20
	bcc	_nsd_n163_ch1_voice_exit	;1未満だったら終了
	ldy	#N163_Waveform - (8 * 2)
	jmp	_nsd_n163_ch1_voice_set

_nsd_n163_ch4_voice:
	ldy	__n163_num
	cpy	#$30
	bcc	_nsd_n163_ch1_voice_exit	;1未満だったら終了
	ldy	#N163_Waveform - (8 * 3)
	jmp	_nsd_n163_ch1_voice_set

_nsd_n163_ch5_voice:
	ldy	__n163_num
	cpy	#$40
	bcc	_nsd_n163_ch1_voice_exit	;1未満だったら終了
	ldy	#N163_Waveform - (8 * 4)
	jmp	_nsd_n163_ch1_voice_set

_nsd_n163_ch6_voice:
	ldy	__n163_num
	cpy	#$50
	bcc	_nsd_n163_ch1_voice_exit	;1未満だったら終了
	ldy	#N163_Waveform - (8 * 5)
	jmp	_nsd_n163_ch1_voice_set

_nsd_n163_ch7_voice:
	ldy	__n163_num
	cpy	#$60
	bcc	_nsd_n163_ch1_voice_exit	;1未満だったら終了
	ldy	#N163_Waveform - (8 * 6)
	jmp	_nsd_n163_ch1_voice_set

_nsd_n163_ch8_voice:
	ldy	__n163_num
	cpy	#$70
	bcc	_nsd_n163_ch1_voice_exit	;1未満だったら終了
	ldy	#N163_Waveform - (8 * 7)
	jmp	_nsd_n163_ch1_voice_set

.endif

;---------------------------------------
.ifdef	PSG
;0ppn-nnnn
;	n nnnn	frequency
;	pp	00 noise	0-15
;		01 tone + noise	16-31
;		10 tone		32
;		11 none
_nsd_psg_ch1_voice:
	tay

	lda	__psg_switch
	and	#%00110110	;mask

	cpy	#$40
	bcs	@L40

	pha
	lda	#PSG_Noise_Frequency
	sta	PSG_Register
	tya
	and	#$1F
	sta	PSG_Data
	pla

	cpy	#$20
	bcc	@L60
	bcs	_nsd_psg_ch1_voice_SET

@L40:
	ora	#%00001000	;noise off
	cpy	#$60
	bcc	_nsd_psg_ch1_voice_SET
@L60:
	ora	#%00000001	;tone off

_nsd_psg_ch1_voice_SET:
	ldy	#PSG_Switch
	sty	PSG_Register
	sta	__psg_switch
	sta	PSG_Data
	rts

;---------------------------------------
_nsd_psg_ch2_voice:
	tay

	lda	__psg_switch
	and	#%00101101	;mask

	cpy	#$40
	bcs	@L40

	pha
	lda	#PSG_Noise_Frequency
	sta	PSG_Register
	tya
	and	#$1F
	sta	PSG_Data
	pla

	cpy	#$20
	bcc	@L60
	bcs	_nsd_psg_ch1_voice_SET

@L40:
	ora	#%00010000	;noise off
	cpy	#$60
	bcc	_nsd_psg_ch1_voice_SET
@L60:
	ora	#%00000010	;tone off
	jmp	_nsd_psg_ch1_voice_SET

;---------------------------------------
_nsd_psg_ch3_voice:
	tay

	lda	__psg_switch
	and	#%00011011	;mask

	cpy	#$40
	bcs	@L40

	pha
	lda	#PSG_Noise_Frequency
	sta	PSG_Register
	tya
	and	#$1F
	sta	PSG_Data
	pla

	cpy	#$20
	bcc	@L60
	bcs	_nsd_psg_ch1_voice_SET

@L40:
	ora	#%00100000	;noise off
	cpy	#$60
	bcc	_nsd_psg_ch1_voice_SET
@L60:
	ora	#%00000100	;tone off
	jmp	_nsd_psg_ch1_voice_SET

.endif

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
	.addr	_nsd_fds_volume
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
.ifdef	OPLL
	.addr	_nsd_OPLL_volume
	.addr	_nsd_OPLL_volume
	.addr	_nsd_OPLL_volume
	.addr	_nsd_OPLL_volume
	.addr	_nsd_OPLL_volume
	.addr	_nsd_OPLL_volume
	.addr	_nsd_OPLL_volume_R
	.addr	_nsd_OPLL_volume_R
	.addr	_nsd_OPLL_volume_R
	.addr	_nsd_opll_BD_volume
	.addr	_nsd_opll_SD_HH_volume
	.addr	_nsd_opll_SD_HH_volume
	.addr	_nsd_opll_Cym_Tom_volume
	.addr	_nsd_opll_Cym_Tom_volume
.endif
.ifdef	MMC5
	.addr	_nsd_mmc5_ch1_volume
	.addr	_nsd_mmc5_ch2_volume
.endif
.ifdef	N163
	.addr	_nsd_n163_ch1_volume
	.addr	_nsd_n163_ch2_volume
	.addr	_nsd_n163_ch3_volume
	.addr	_nsd_n163_ch4_volume
	.addr	_nsd_n163_ch5_volume
	.addr	_nsd_n163_ch6_volume
	.addr	_nsd_n163_ch7_volume
	.addr	_nsd_n163_ch8_volume
.endif
.ifdef	PSG
	.addr	_nsd_psg_ch1_volume
	.addr	_nsd_psg_ch2_volume
	.addr	_nsd_psg_ch3_volume
.endif
.ifdef	NULL
	.addr	Exit
.endif
	.addr	_nsd_se1_volume		;SE  ch1 Pulse
	.addr	_nsd_se2_volume		;SE  ch2 Noize

;---------------------------------------
.code
	ldy	JMPTBL,x		;
	sty	__ptr			;
	ldy	JMPTBL + 1,x		;
	sty	__ptr + 1		;
	jmp	(__ptr)			;[5]

;---------------------------------------
_nsd_ch1_volume:

	;-------------------------------
	; *** Mix voice and volume
	;a = (a & 0x0F) | (nsd_word.Voice.voice_set & 0xF0)
	and	#$0F
	ora	#$30	;a |= 0x30	;counter on / hard-envelop off
;	ora	__voice_set,x
	ora	__voice_set + nsd::TR_BGM1

	;-------------------------------
	; *** Output to NES sound device
	;y = x << 1
	sta	APU_PULSE1CTRL

	;-------------------------------
	; *** Exit
Exit:
	rts

;---------------------------------------
_nsd_ch2_volume:
	;-------------------------------
	;SE check
	ldy	__Sequence_ptr + nsd::TR_SE1 + 1
	bne	Exit

_nsd_se1_volume:

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
	rts

;---------------------------------------
_nsd_ch4_volume:
	;-------------------------------
	;SE check
	ldy	__Sequence_ptr + nsd::TR_SE2 + 1
	bne	Exit

_nsd_se2_volume:

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
	rts

;---------------------------------------
.ifdef	FDS
_nsd_fds_volume:
	;-------------------------------
	; *** Calculate the voice
	and	#$3F
	ora	#$80

	;-------------------------------
	; *** Set the voice to work
	sta	FDS_Volume

	;-------------------------------
	; *** Exit
	rts
.endif

;---------------------------------------
.ifdef	VRC6
_nsd_vrc6_ch1_volume:

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
	rts

;---------------------------------------
_nsd_vrc6_ch2_volume:

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
	rts

;---------------------------------------
_nsd_vrc6_ch3_volume:
	sta	VRC6_SAW_CTRL
	rts

.endif

;---------------------------------------
.ifdef	VRC7
_nsd_vrc7_volume:

	eor	#$FF			;[2]2
	tay				;[2]4

	txa				;[2]6
	sub	#nsd::TR_VRC7		;[]
	shr	a, 1			;[2]
	add	#VRC7_Volume		;[2]
	sta	VRC7_Resister		;

	tya				;[2]
	and	#$0F			;[2]4
	ora	__voice_set,x		;[4]8

	sta	VRC7_Data

	rts				;[6]

.endif

;---------------------------------------
.ifdef	OPLL
_nsd_OPLL_volume_R:

	;OPLL_Rhythm check
	ldy	__opll_ryhthm
	cpy	#$20
	bcs	_nsd_OPLL_volume_exit

_nsd_OPLL_volume:

	eor	#$FF
	tay

	txa
	sub	#nsd::TR_OPLL
	shr	a, 1
	add	#OPLL_Volume
	sta	OPLL_Resister		;

	tya				;[2]
	and	#$0F			;[2]
	ora	__voice_set,x		;[4]

	sta	OPLL_Data

_nsd_OPLL_volume_exit:
	rts

;---------------------------------------
_nsd_opll_BD_volume:
	;OPLL_Rhythm check
	lda	__opll_ryhthm
	cmp	#$20
	bcc	@Exit

	ldy	#OPLL_BD
	sty	OPLL_Resister
	lda	__volume + nsd::TR_OPLL + 18
	eor	#$FF
	and	#$0F
	sta	OPLL_Data
@Exit:
	rts

;---------------------------------------
_nsd_opll_SD_HH_volume:
	;OPLL_Rhythm check
	lda	__opll_ryhthm
	cmp	#$20
	bcc	@Exit

	ldy	#OPLL_HH_SD
	sty	OPLL_Resister
	lda	__volume + nsd::TR_OPLL + 22
	shl	a, 4
	sta	__tmp
	lda	__volume + nsd::TR_OPLL + 20
	and	#$0F
	ora	__tmp
	eor	#$FF
	sta	OPLL_Data
@Exit:
	rts

;---------------------------------------
_nsd_opll_Cym_Tom_volume:
	;OPLL_Rhythm check
	lda	__opll_ryhthm
	cmp	#$20
	bcc	@Exit

	ldy	#OPLL_TOM_CYM
	sty	OPLL_Resister
	lda	__volume + nsd::TR_OPLL + 26
	shl	a, 4
	sta	__tmp
	lda	__volume + nsd::TR_OPLL + 24
	and	#$0F
	ora	__tmp
	eor	#$FF
	sta	OPLL_Data
@Exit:
	rts

.endif

;---------------------------------------
.ifdef	MMC5
_nsd_mmc5_ch1_volume:

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
	rts

;---------------------------------------
_nsd_mmc5_ch2_volume:

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
	rts
.endif

;---------------------------------------
.ifdef	N163
_nsd_n163_ch1_volume:
	ldy	#N163_Volume - (8 * 0)
	sty	N163_Resister
	and	#$0F
	ora	__n163_num
	sta	N163_Data
	rts

;---------------------------------------
_nsd_n163_ch2_volume:
	ldy	__n163_num
	cpy	#$10
	bcc	_nsd_n163_ch1_volume_Exit	;1未満だったら終了
	ldy	#N163_Volume - (8 * 1)

_nsd_n163_ch1_volume_Set:
	sty	N163_Resister
	and	#$0F
	sta	N163_Data

_nsd_n163_ch1_volume_Exit:
	rts

;---------------------------------------
_nsd_n163_ch3_volume:
	ldy	__n163_num
	cpy	#$20
	bcc	_nsd_n163_ch1_volume_Exit	;1未満だったら終了
	ldy	#N163_Volume - (8 * 2)
	jmp	_nsd_n163_ch1_volume_Set

;---------------------------------------
_nsd_n163_ch4_volume:
	ldy	__n163_num
	cpy	#$30
	bcc	_nsd_n163_ch1_volume_Exit	;1未満だったら終了
	ldy	#N163_Volume - (8 * 3)
	jmp	_nsd_n163_ch1_volume_Set

;---------------------------------------
_nsd_n163_ch5_volume:
	ldy	__n163_num
	cpy	#$40
	bcc	_nsd_n163_ch1_volume_Exit	;1未満だったら終了
	ldy	#N163_Volume - (8 * 4)
	jmp	_nsd_n163_ch1_volume_Set

;---------------------------------------
_nsd_n163_ch6_volume:
	ldy	__n163_num
	cpy	#$50
	bcc	_nsd_n163_ch1_volume_Exit	;1未満だったら終了
	ldy	#N163_Volume - (8 * 5)
	jmp	_nsd_n163_ch1_volume_Set

;---------------------------------------
_nsd_n163_ch7_volume:
	ldy	__n163_num
	cpy	#$60
	bcc	_nsd_n163_ch1_volume_Exit	;1未満だったら終了
	ldy	#N163_Volume - (8 * 6)
	jmp	_nsd_n163_ch1_volume_Set

;---------------------------------------
_nsd_n163_ch8_volume:
	ldy	__n163_num
	cpy	#$70
	bcc	_nsd_n163_ch1_volume_Exit	;1未満だったら終了
	ldy	#N163_Volume - (8 * 7)
	jmp	_nsd_n163_ch1_volume_Set

.endif

;---------------------------------------
.ifdef	PSG
_nsd_psg_ch1_volume:
	ldy	#PSG_1_Volume
	sty	PSG_Register

_nsd_psg_set_volume:
	and	#$0F
	sta	__tmp
	lda	__chflag,x
	and	#nsd_chflag::Envelop
	ora	__tmp
	sta	PSG_Data
	rts

_nsd_psg_ch2_volume:
	ldy	#PSG_2_Volume
	sty	PSG_Register
	jmp	_nsd_psg_set_volume

_nsd_psg_ch3_volume:
	ldy	#PSG_3_Volume
	sty	PSG_Register
	jmp	_nsd_psg_set_volume

.endif

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
	.addr	_nsd_fds_sweep_bias
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
.ifdef	OPLL
	.addr	_nsd_vrc7_sustain
	.addr	_nsd_vrc7_sustain
	.addr	_nsd_vrc7_sustain
	.addr	_nsd_vrc7_sustain
	.addr	_nsd_vrc7_sustain
	.addr	_nsd_vrc7_sustain
	.addr	_nsd_vrc7_sustain
	.addr	_nsd_vrc7_sustain
	.addr	_nsd_vrc7_sustain
	.addr	_nsd_OPLL_ryhthm
	.addr	_nsd_OPLL_ryhthm
	.addr	_nsd_OPLL_ryhthm
	.addr	_nsd_OPLL_ryhthm
	.addr	_nsd_OPLL_ryhthm
.endif
.ifdef	MMC5
	.addr	Exit
	.addr	Exit
.endif
.ifdef	N163
	.addr	_nsd_n163_sample_length
	.addr	_nsd_n163_sample_length
	.addr	_nsd_n163_sample_length
	.addr	_nsd_n163_sample_length
	.addr	_nsd_n163_sample_length
	.addr	_nsd_n163_sample_length
	.addr	_nsd_n163_sample_length
	.addr	_nsd_n163_sample_length
.endif
.ifdef	PSG
	.addr	_nsd_psg_envelop
	.addr	_nsd_psg_envelop
	.addr	_nsd_psg_envelop
.endif
.ifdef	NULL
	.addr	Exit
.endif
	.addr	_nsd_se2_sweep		;SE  ch1 Pulse
	.addr	Exit			;SE  ch2 Noize		-- no process --

;---------------------------------------
.code
	ldy	JMPTBL,x
	sty	__ptr
	ldy	JMPTBL + 1,x
	sty	__ptr + 1
	jmp	(__ptr)

;---------------------------------------
_nsd_ch1_sweep:
	sta	__sweep_ch1
	sta	APU_PULSE1RAMP
Exit:
	rts

;---------------------------------------
_nsd_ch2_sweep:

	sta	__sweep_ch2

	;-------------------------------
	;SE check
	ldy	__Sequence_ptr + nsd::TR_SE1 + 1
	bne	Exit

_nsd_se2_sweep:
	sta	APU_PULSE2RAMP
	rts

.endproc

;---------------------------------------
.ifdef	FDS
_nsd_fds_sweep_bias:
	and	#$7F
	sta	FDS_Sweep_Bias
	sta	__fds_sweepbias
	rts
.endif

;---------------------------------------
.if	.defined(VRC7) || .defined(OPLL)
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

.ifdef	OPLL
.proc	_nsd_OPLL_ryhthm

	shl	a, 5
	and	#$20
	sta	__tmp

	ldy	#OPLL_RHYTHM
	sty	OPLL_Resister

	lda	__opll_ryhthm
	and	#$1F
	ora	__tmp
	sta	__opll_ryhthm
	sta	OPLL_Data

	cmp	#$20
	bcc	Exit

L01:
	ldy	#$16
	sty	OPLL_Resister
	lda	#$20
	tay
	ldy	#10
	sta	OPLL_Data
@L:	dey
	bne	@L

L02:
	ldy	#$17
	sty	OPLL_Resister
	lda	#$50
	tay
	ldy	#10
	sta	OPLL_Data
@L:	dey
	bne	@L

L03:
	ldy	#$18
	sty	OPLL_Resister
	lda	#$C0
	tay
	ldy	#10
	sta	OPLL_Data
@L:	dey
	bne	@L

L04:
	ldy	#$26
	sty	OPLL_Resister
	lda	#$05
	tay
	ldy	#10
	sta	OPLL_Data
@L:	dey
	bne	@L

L05:
	ldy	#$27
	sty	OPLL_Resister
	lda	#$05
	tay
	ldy	#10
	sta	OPLL_Data
@L:	dey
	bne	@L

L06:
	ldy	#$28
	sty	OPLL_Resister
	lda	#$01
	tay
	tay
	sta	OPLL_Data

Exit:
	rts
.endproc
.endif

;---------------------------------------
.ifdef	N163
.proc	_nsd_n163_sample_length
	shl	a, 2
;	ora	#$E0
	sta	__tmp
	txa
	sub	#nsd::TR_N163
	shr	a, 1
	tay
	lda	__tmp
	sta	__n163_frequency,y
	rts
.endproc
.endif

;---------------------------------------
.ifdef	PSG
.proc	_nsd_psg_envelop
	ldy	#PSG_Envelope_Form
	sty	PSG_Register
	tay
	and	#$0F
	sta	PSG_Data
	tya
	and	#nsd_chflag::Envelop
	sta	__tmp
	lda	__chflag,x
	and	#~nsd_chflag::Envelop
	ora	__tmp
	sta	__chflag,x
	rts
.endproc
.endif
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
.rodata

;---------------------------------------
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



;---------------------------------------
;FDS Frequency table
.ifdef	FDS
Freq_FDS:
	.word	$99F
	.word	$9B1
	.word	$9C3
	.word	$9D5
	.word	$9E7
	.word	$9F9
	.word	$A0C
	.word	$A1E
	.word	$A31
	.word	$A44
	.word	$A57
	.word	$A6A
	.word	$A7E
	.word	$A91
	.word	$AA5
	.word	$AB9
	.word	$ACC
	.word	$AE0
	.word	$AF5
	.word	$B09
	.word	$B1D
	.word	$B32
	.word	$B47
	.word	$B5C
	.word	$B71
	.word	$B86
	.word	$B9B
	.word	$BB1
	.word	$BC7
	.word	$BDD
	.word	$BF3
	.word	$C09
	.word	$C1F
	.word	$C36
	.word	$C4C
	.word	$C63
	.word	$C7A
	.word	$C91
	.word	$CA8
	.word	$CC0
	.word	$CD8
	.word	$CEF
	.word	$D07
	.word	$D20
	.word	$D38
	.word	$D50
	.word	$D69
	.word	$D82
	.word	$D9B
	.word	$DB4
	.word	$DCE
	.word	$DE7
	.word	$E01
	.word	$E1B
	.word	$E35
	.word	$E50
	.word	$E6A
	.word	$E85
	.word	$EA0
	.word	$EBB
	.word	$ED6
	.word	$EF2
	.word	$F0E
	.word	$F2A
	.word	$F46
	.word	$F62
	.word	$F7F
	.word	$F9B
	.word	$FB8
	.word	$FD5
	.word	$FF3
	.word	$1010
	.word	$102E
	.word	$104C
	.word	$106B
	.word	$1089
	.word	$10A8
	.word	$10C7
	.word	$10E6
	.word	$1105
	.word	$1125
	.word	$1144
	.word	$1164
	.word	$1185
	.word	$11A5
	.word	$11C6
	.word	$11E7
	.word	$1208
	.word	$122A
	.word	$124B
	.word	$126D
	.word	$128F
	.word	$12B2
	.word	$12D5
	.word	$12F7
	.word	$131B
.endif

;---------------------------------------
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

;---------------------------------------
;VRC7 Frequency table
.if	.defined(VRC7) || .defined(OPLL)
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

;---------------------------------------
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
Freq_N163_50:
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

.code

.proc	_nsd_snd_frequency
.rodata

JMPTBL:	.addr	_nsd_nes_ch1_frequency	;BGM ch1 Pulse
	.addr	_nsd_nes_ch2_frequency	;BGM ch2 Pulse
	.addr	_nsd_nes_ch3_frequency	;BGM ch3 Triangle
	.addr	_nsd_nes_ch4_frequency	;BGM ch4 Noise
	.addr	Exit			;BGM ch5 DPCM
.ifdef	FDS
	.addr	_nsd_fds_frequency
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
.ifdef	OPLL
	.addr	_nsd_OPLL_frequency
	.addr	_nsd_OPLL_frequency
	.addr	_nsd_OPLL_frequency
	.addr	_nsd_OPLL_frequency
	.addr	_nsd_OPLL_frequency
	.addr	_nsd_OPLL_frequency
	.addr	_nsd_OPLL_frequency_R
	.addr	_nsd_OPLL_frequency_R
	.addr	_nsd_OPLL_frequency_R
	.addr	_nsd_OPLL_frequency_BD
	.addr	_nsd_OPLL_frequency_HH_SD
	.addr	_nsd_OPLL_frequency_HH_SD
	.addr	_nsd_OPLL_frequency_TOM_CYM
	.addr	_nsd_OPLL_frequency_TOM_CYM
.endif
.ifdef	MMC5
	.addr	_nsd_mmc5_ch1_frequency
	.addr	_nsd_mmc5_ch2_frequency
.endif
.ifdef	N163
	.addr	_nsd_n163_ch1_frequency
	.addr	_nsd_n163_ch2_frequency
	.addr	_nsd_n163_ch3_frequency
	.addr	_nsd_n163_ch4_frequency
	.addr	_nsd_n163_ch5_frequency
	.addr	_nsd_n163_ch6_frequency
	.addr	_nsd_n163_ch7_frequency
	.addr	_nsd_n163_ch8_frequency
.endif
.ifdef	PSG
	.addr	_nsd_psg_ch1_frequency
	.addr	_nsd_psg_ch2_frequency
	.addr	_nsd_psg_ch3_frequency
.endif
.ifdef	NULL
	.addr	Exit
.endif
	.addr	_nsd_nes_se1_frequency	;SE  ch1 Pulse
	.addr	_nsd_nes_se2_frequency	;SE  ch2 Noise

;---------------------------------------
.code
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

	;-----------
	;Jump
Set_Frequency:
	lda	JMPTBL,y
	sta	__ptr
	lda	JMPTBL + 1,y
	sta	__ptr + 1

	lda	__frequency,y		;ax ← __frequency
	jmp	(__ptr)
Exit:
	ldx	__channel
	rts
.endproc

;---------------------------------------
.proc	_nsd_nes_ch1_frequency

	jsr	Normal_frequency

	lda	__tmp
	sta	APU_PULSE1FTUNE
	lda	__tmp + 1
	ora	#$08
	cmp	__apu_frequency1
	beq	Exit
	sta	APU_PULSE1CTUNE
	sta	__apu_frequency1

Exit:
	rts
.endproc

;---------------------------------------
.proc	_nsd_nes_ch2_frequency
	;-------------------------------
	;SE check
	ldy	__Sequence_ptr + nsd::TR_SE1 + 1
	bne	Exit

	jsr	Normal_frequency

	lda	__tmp
	sta	APU_PULSE2FTUNE
	lda	__tmp + 1
	ora	#$08
	cmp	__apu_frequency2
	beq	Exit
	sta	APU_PULSE2STUNE		;nes.inc 間違えてる。
	sta	__apu_frequency2
Exit:
	rts
.endproc

.proc	_nsd_nes_se1_frequency

	jsr	Normal_frequency

	lda	__tmp
	sta	APU_PULSE2FTUNE
	lda	__tmp + 1
	ora	#$08
	cmp	__se_frequency
	beq	Exit
	sta	APU_PULSE2STUNE		;nes.inc 間違えてる。
	sta	__se_frequency

Exit:
	rts
.endproc

;---------------------------------------
.proc	_nsd_nes_ch3_frequency

	jsr	Normal_frequency

	lda	__tmp
	sta	APU_TRIFREQ1
	lda	__tmp + 1
	ora	#$08
	cmp	__apu_frequency3
	beq	Exit
	sta	APU_TRIFREQ2
	sta	__apu_frequency3

Exit:
	rts
.endproc

;---------------------------------------
.proc	_nsd_nes_ch4_frequency
	;-------------------------------
	;SE check
	ldy	__Sequence_ptr + nsd::TR_SE2 + 1
	beq	_nsd_nes_se2_frequency
	rts
.endproc

.proc	_nsd_nes_se2_frequency

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
	sta	APU_NOISEFREQ2		;Length counter load (L) 

	;-------------------------------
	; *** Exit
Exit:
	rts
.endproc

;---------------------------------------
.ifdef	FDS
.proc	_nsd_fds_frequency

	jsr	_nsd_div192		; 
	and	#$FE			; x =  ax  /  192
	tay				; y = (ax mod 192) & 0xFE

	;-------------------------------
	; *** Get frequency from table
	; nsd_work_zp._tmp <- frequency
	lda	Freq_FDS + 1,y
	sta	__tmp + 1
	lda	Freq_FDS,y

	;-------------------------------
	; *** Octave caluclate  and  overflow check
Octave_Proc:
	;if (octave == 0) {
	cpx	#7
	bcc	Octave_Loop
	bne	@Over
	sta	__tmp
	lda	__tmp + 1
	cmp	#$10				;if (frequency >= 0x1000) {
	bcc	Detune
@Over:	lda	#$0F
	sta	__tmp + 1
	lda	#$FF				;	frequency = 0x0FFF
	jmp	Octave_Exit			; } else {
	; } } else { while (octave > 0) {
Octave_Loop:
	lsr	__tmp + 1	; frequency >>= 1
	ror	a
	inx			; octave--;
	cpx	#7
	bne	Octave_Loop
	; } }
Octave_Exit:
	sta	__tmp

Detune:	
	ldx	__channel
	ldy	#$00
	lda	__detune_fine,x
	bpl	@L
	dey				; ay = __detune_fine (sign expand)
@L:	add	__tmp
	sta	FDS_FTUNE
	tya
	adc	__tmp + 1
	and	#$0F
	sta	FDS_CTUNE		;__tmp += (signed int)__detune_cent

	rts
.endproc
.endif

;---------------------------------------
.ifdef	VRC6
.proc	_nsd_vrc6_ch1_frequency
	jsr	Normal12_frequency

	lda	__tmp
	sta	VRC6_Pulse1_FTUNE
	lda	__tmp + 1
	ora	#$80
	sta	VRC6_Pulse1_CTUNE

	rts
.endproc

;---------------------------------------
.proc	_nsd_vrc6_ch2_frequency
	jsr	Normal12_frequency

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

	;-------------------------------
	; *** Octave caluclate  and  overflow check
Octave_Proc:
	;if (octave == 0) {
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
	sta	__tmp

Detune:	
	ldx	__channel
	ldy	#$00
	lda	__detune_fine,x
	bpl	@L
	dey				; ay = __detune_fine (sign expand)
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
	dey				;	ay = __detune_fine (sign expand)
@L:	add	__tmp			;[5]13	clock > 6 clock
	sta	VRC7_Data		;●Data Write
	tya				;[2]
	adc	__tmp + 1		;[3]5
	and	#$0F			;[2]7
	sta	__tmp + 1		;[3]10	__tmp += (signed int)__detune_cent
	lda	__chflag,x		;[4]14
	and	#$30			;[2]16	 00XX 0000 <2>
	ora	__tmp + 1		;[3]19	flag と octave をマージ
	sta	__tmp + 1		;[3]22

	lda	(__ptr,x)		;[6]28
	lda	__ptr,x			;[4]32

	pla				;[4]36	a ← VRC7でのチャンネル番号
	tay				;[2]40
	add	#VRC7_Octave		;[4]42 clock !! (VRC7のwait)

	sta	VRC7_Resister		;●Resister Write
	lda	__tmp + 1		;[3]3
	sta	__vrc7_frequency,y	;[5]8 clock > 6 clock
	sta	VRC7_Data		;●Data Write

	rts
.endproc
.endif

;---------------------------------------
.ifdef	OPLL
.proc	_nsd_OPLL_frequency

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
	sub	#nsd::TR_OPLL
	shr	a, 1
	pha				;push OPLLでのチャンネル番号
	;オクターブ
	add	#OPLL_Frequency
	sta	OPLL_Resister		;●Resister Write

Detune:	
	ldy	#$00			;[2]
	lda	__detune_fine,x		;[4]6
	bpl	@L			;[2]8
	dey			;	ay = __detune_fine (sign expand)
@L:	add	__tmp			;[5]13	clock > 6 clock
	sta	OPLL_Data		;●Data Write
	tya				;[2]
	adc	__tmp + 1		;[3]5
	and	#$0F			;[2]7
	sta	__tmp + 1		;[3]10	__tmp += (signed int)__detune_cent
	lda	__chflag,x		;[4]14
	and	#$30			;[2]16	 00XX 0000 <2>
	ora	__tmp + 1		;[3]19	flag と octave をマージ
	sta	__tmp + 1		;[3]22

	lda	(__ptr,x)		;[6]28
	lda	__ptr,x			;[4]32

	pla				;[4]36	a ← VRC7でのチャンネル番号
	tay				;[2]40
	add	#OPLL_Octave		;[4]42 clock !! (OPLLのwait)

	sta	OPLL_Resister		;●Resister Write
	lda	__tmp + 1		;[3]3
	sta	__opll_frequency,y	;[5]8 clock > 6 clock
	sta	OPLL_Data		;●Data Write

	rts
.endproc

.proc	_nsd_OPLL_frequency_R

	;OPLL_Rhythm check
	ldy	__opll_ryhthm
	cpy	#$20
	bcc	_nsd_OPLL_frequency
	rts
.endproc


.proc	_nsd_OPLL_frequency_BD

	;除算
	jsr	_nsd_div192		;Wait変わりに使える？
	stx	__tmp + 1
	cmp	#$6E
	rol	__tmp + 1		;__tmp + 1 = (Octave << 1) + Note_MSB

	shr	a, 1			;
	tay				;y = (ax mod 192) >> 1
	lda	Freq_VRC7,y		;
	sta	__tmp			;__tmp + 0 = Note_LSB

	ldx	__channel
	;オクターブ
	lda	#OPLL_Frequency_BD
	sta	OPLL_Resister		;●Resister Write

Detune:	
	ldy	#$00			;[2]
	lda	__detune_fine,x		;[4]6
	bpl	@L			;[2]8
	dey				;	ay = __detune_fine (sign expand)
@L:	add	__tmp			;[5]13	clock > 6 clock
	sta	OPLL_Data		;●Data Write
	tya				;[2]
	adc	__tmp + 1		;[3]5
	and	#$0F			;[2]7
	sta	__tmp + 1		;[3]10	__tmp += (signed int)__detune_cent
	lda	__chflag,x		;[4]14
	and	#$30			;[2]16	 00XX 0000 <2>
	ora	__tmp + 1		;[3]19	flag と octave をマージ
	sta	__tmp + 1		;[3]22

	lda	(__ptr,x)		;[6]28
	lda	(__ptr,x)		;[6]34
	lda	(__ptr,x)		;[6]40

	lda	#OPLL_Octave_BD		;[2]42

	sta	OPLL_Resister		;●Resister Write
	lda	__tmp + 1		;[3]3
	lda	__tmp + 1		;[3]3
	sta	OPLL_Data		;●Data Write

	rts
.endproc

.proc	_nsd_OPLL_frequency_HH_SD

	;除算
	jsr	_nsd_div192		;Wait変わりに使える？
	stx	__tmp + 1
	cmp	#$6E
	rol	__tmp + 1		;__tmp + 1 = (Octave << 1) + Note_MSB

	shr	a, 1			;
	tay				;y = (ax mod 192) >> 1
	lda	Freq_VRC7,y		;
	sta	__tmp			;__tmp + 0 = Note_LSB

	ldx	__channel
	lda	__chflag,x
	and	#nsd_chflag::KeyOff
	cmp	#$03
	bne	Exit

	;オクターブ
	lda	#OPLL_Frequency_HH_SD
	sta	OPLL_Resister		;●Resister Write

Detune:	
	ldy	#$00			;[2]
	lda	__detune_fine,x		;[4]6
	bpl	@L			;[2]8
	dey				;	ay = __detune_fine (sign expand)
@L:	add	__tmp			;[5]13	clock > 6 clock
	sta	OPLL_Data		;●Data Write
	tya				;[2]
	adc	__tmp + 1		;[3]5
	and	#$0F			;[2]7
	sta	__tmp + 1		;[3]10	__tmp += (signed int)__detune_cent
	lda	__chflag,x		;[4]14
	and	#$30			;[2]16	 00XX 0000 <2>
	ora	__tmp + 1		;[3]19	flag と octave をマージ
	sta	__tmp + 1		;[3]22

	lda	(__ptr,x)		;[6]28
	lda	(__ptr,x)		;[6]34
	lda	(__ptr,x)		;[6]40

	lda	#OPLL_Octave_HH_SD	;[2]42

	sta	OPLL_Resister		;●Resister Write
	lda	__tmp + 1		;[3]3
	lda	__tmp + 1		;[3]3
	sta	OPLL_Data		;●Data Write
Exit:
	rts
.endproc

.proc	_nsd_OPLL_frequency_TOM_CYM

	;除算
	jsr	_nsd_div192		;Wait変わりに使える？
	stx	__tmp + 1
	cmp	#$6E
	rol	__tmp + 1		;__tmp + 1 = (Octave << 1) + Note_MSB

	shr	a, 1			;
	tay				;y = (ax mod 192) >> 1
	lda	Freq_VRC7,y		;
	sta	__tmp			;__tmp + 0 = Note_LSB

	ldx	__channel
	lda	__chflag,x
	and	#nsd_chflag::KeyOff
	cmp	#$03
	bne	Exit

	;オクターブ
	lda	#OPLL_Frequency_TOM_CYM
	sta	OPLL_Resister		;●Resister Write

Detune:	
	ldy	#$00			;[2]
	lda	__detune_fine,x		;[4]6
	bpl	@L			;[2]8
	dey				;	ay = __detune_fine (sign expand)
@L:	add	__tmp			;[5]13	clock > 6 clock
	sta	OPLL_Data		;●Data Write
	tya				;[2]
	adc	__tmp + 1		;[3]5
	and	#$0F			;[2]7
	sta	__tmp + 1		;[3]10	__tmp += (signed int)__detune_cent
	lda	__chflag,x		;[4]14
	and	#$30			;[2]16	 00XX 0000 <2>
	ora	__tmp + 1		;[3]19	flag と octave をマージ
	sta	__tmp + 1		;[3]22

	lda	(__ptr,x)		;[6]28
	lda	(__ptr,x)		;[6]34
	lda	(__ptr,x)		;[6]40

	lda	#OPLL_Octave_TOM_CYM	;[2]42

	sta	OPLL_Resister		;●Resister Write
	lda	__tmp + 1		;[3]3
	lda	__tmp + 1		;[3]3
	sta	OPLL_Data		;●Data Write
Exit:
	rts
.endproc


.endif

;---------------------------------------
.ifdef	MMC5
.proc	_nsd_mmc5_ch1_frequency
	jsr	Normal_frequency

	lda	__tmp
	sta	MMC5_Pulse1_FTUNE
	lda	__tmp + 1
	ora	#$08
	cmp	__mmc5_frequency1
	beq	Exit
	sta	MMC5_Pulse1_CTUNE
	sta	__mmc5_frequency1

Exit:
	rts
.endproc

;---------------------------------------
.proc	_nsd_mmc5_ch2_frequency
	jsr	Normal_frequency

	lda	__tmp
	sta	MMC5_Pulse2_FTUNE
	lda	__tmp + 1
	ora	#$08
	cmp	__mmc5_frequency2
	beq	Exit
	sta	MMC5_Pulse2_CTUNE
	sta	__mmc5_frequency2

Exit:
	rts
.endproc
.endif

;---------------------------------------
.ifdef	N163
.proc	_nsd_n163_ch1_frequency

	jsr	N163_frequency

	lda	#N163_Frequency_Low - (8 * 0)
	sta	N163_Resister
	lda	__tmp
	sta	N163_Data

	lda	#N163_Frequency_Middle - (8 * 0)
	sta	N163_Resister
	lda	__tmp + 1
	sta	N163_Data

	lda	#N163_Frequency_High - (8 * 0)
	sta	N163_Resister
	lda	__ptr
	ora	__n163_frequency + 0
	sta	N163_Data
Exit:
	rts
.endproc

.proc	_nsd_n163_ch2_frequency
	ldy	__n163_num
	cpy	#$10
	bcc	Exit			;1未満だったら終了

	jsr	N163_frequency

	lda	#N163_Frequency_Low - (8 * 1)
	sta	N163_Resister
	lda	__tmp
	sta	N163_Data

	lda	#N163_Frequency_Middle - (8 * 1)
	sta	N163_Resister
	lda	__tmp + 1
	sta	N163_Data

	lda	#N163_Frequency_High - (8 * 1)
	sta	N163_Resister
	lda	__ptr
	ora	__n163_frequency + 1
	sta	N163_Data
Exit:
	rts
.endproc

.proc	_nsd_n163_ch3_frequency
	ldy	__n163_num
	cpy	#$20
	bcc	Exit			;1未満だったら終了

	jsr	N163_frequency

	lda	#N163_Frequency_Low - (8 * 2)
	sta	N163_Resister
	lda	__tmp
	sta	N163_Data

	lda	#N163_Frequency_Middle - (8 * 2)
	sta	N163_Resister
	lda	__tmp + 1
	sta	N163_Data

	lda	#N163_Frequency_High - (8 * 2)
	sta	N163_Resister
	lda	__ptr
	ora	__n163_frequency + 2
	sta	N163_Data
Exit:
	rts
.endproc

.proc	_nsd_n163_ch4_frequency
	ldy	__n163_num
	cpy	#$30
	bcc	Exit			;1未満だったら終了

	jsr	N163_frequency

	lda	#N163_Frequency_Low - (8 * 3)
	sta	N163_Resister
	lda	__tmp
	sta	N163_Data

	lda	#N163_Frequency_Middle - (8 * 3)
	sta	N163_Resister
	lda	__tmp + 1
	sta	N163_Data

	lda	#N163_Frequency_High - (8 * 3)
	sta	N163_Resister
	lda	__ptr
	ora	__n163_frequency + 3
	sta	N163_Data
Exit:
	rts
.endproc

.proc	_nsd_n163_ch5_frequency
	ldy	__n163_num
	cpy	#$40
	bcc	Exit			;1未満だったら終了

	jsr	N163_frequency

	lda	#N163_Frequency_Low - (8 * 4)
	sta	N163_Resister
	lda	__tmp
	sta	N163_Data

	lda	#N163_Frequency_Middle - (8 * 4)
	sta	N163_Resister
	lda	__tmp + 1
	sta	N163_Data

	lda	#N163_Frequency_High - (8 * 4)
	sta	N163_Resister
	lda	__ptr
	ora	__n163_frequency + 4
	sta	N163_Data
Exit:
	rts
.endproc

.proc	_nsd_n163_ch6_frequency
	ldy	__n163_num
	cpy	#$50
	bcc	Exit			;1未満だったら終了

	jsr	N163_frequency

	lda	#N163_Frequency_Low - (8 * 5)
	sta	N163_Resister
	lda	__tmp
	sta	N163_Data

	lda	#N163_Frequency_Middle - (8 * 5)
	sta	N163_Resister
	lda	__tmp + 1
	sta	N163_Data

	lda	#N163_Frequency_High - (8 * 5)
	sta	N163_Resister
	lda	__ptr
	ora	__n163_frequency + 5
	sta	N163_Data
Exit:
	rts
.endproc

.proc	_nsd_n163_ch7_frequency
	ldy	__n163_num
	cpy	#$60
	bcc	Exit			;1未満だったら終了

	jsr	N163_frequency

	lda	#N163_Frequency_Low - (8 * 6)
	sta	N163_Resister
	lda	__tmp
	sta	N163_Data

	lda	#N163_Frequency_Middle - (8 * 6)
	sta	N163_Resister
	lda	__tmp + 1
	sta	N163_Data

	lda	#N163_Frequency_High - (8 * 6)
	sta	N163_Resister
	lda	__ptr
	ora	__n163_frequency + 6
	sta	N163_Data
Exit:
	rts
.endproc

.proc	_nsd_n163_ch8_frequency
	ldy	__n163_num
	cpy	#$70
	bcc	Exit			;1未満だったら終了

	jsr	N163_frequency

	lda	#N163_Frequency_Low - (8 * 7)
	sta	N163_Resister
	lda	__tmp
	sta	N163_Data

	lda	#N163_Frequency_Middle - (8 * 7)
	sta	N163_Resister
	lda	__tmp + 1
	sta	N163_Data

	lda	#N163_Frequency_High - (8 * 7)
	sta	N163_Resister
	lda	__ptr
	ora	__n163_frequency + 7
	sta	N163_Data
Exit:
	rts
.endproc


.endif

;---------------------------------------
.ifdef	PSG
.proc	_nsd_psg_ch1_frequency
	jsr	Normal12_frequency
	ldy	#PSG_1_Frequency_Low
	sty	PSG_Register
	lda	__tmp
	sta	PSG_Data
	iny
	sty	PSG_Register
	lda	__tmp + 1
	sta	PSG_Data
Exit:
	rts
.endproc

.proc	_nsd_psg_ch2_frequency
	jsr	Normal12_frequency
	ldy	#PSG_2_Frequency_Low
	sty	PSG_Register
	lda	__tmp
	sta	PSG_Data
	iny
	sty	PSG_Register
	lda	__tmp + 1
	sta	PSG_Data
Exit:
	rts
.endproc

.proc	_nsd_psg_ch3_frequency
	jsr	Normal12_frequency
	ldy	#PSG_3_Frequency_Low
	sty	PSG_Register
	lda	__tmp
	sta	PSG_Data
	iny
	sty	PSG_Register
	lda	__tmp + 1
	sta	PSG_Data
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
;	__tmp + 1	frequency upper 3bit
;	__tmp		frequency lower 8bit	(total = 11bit)
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
	bcc	Detune
@Over:	lda	#$07
	sta	__tmp + 1
	lda	#$FF				;	frequency = 0x07FF
	jmp	Octave_Exit			; } else {
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
	sta	__tmp

Detune:	
	ldx	__channel
	ldy	#$00
	lda	__detune_fine,x
	bpl	@L
	dey				; ay = __detune_fine (sign expand)
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



.if	.defined(VRC6) || .defined(PSG)
;=======================================================================
;	void	__fastcall__	Normal12_frequency(int freq);
;-----------------------------------------------------------------------
;<<Input>>
;	ax	frequency (Range : 0x0000 - 0x07FF )	(16 = 100 cent)
;<<Output>>
;	__tmp + 1	frequency upper 4bit
;	__tmp		frequency lower 8bit	(total = 12bit)
;=======================================================================
.proc	Normal12_frequency

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
	sta	__tmp

Detune:	
	ldx	__channel
	ldy	#$00
	lda	__detune_fine,x
	bpl	@L
	dey				; ay = __detune_fine (sign expand)
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



.ifdef	N163
;=======================================================================
;	void	__fastcall__	N163_frequency(int freq);
;-----------------------------------------------------------------------
;<<Input>>
;	ax	frequency (Range : 0x008E - 0x07FF )	(16 = 100 cent)
;<<Output>>
;	__ptr		frequency HSB 8bit
;	__tmp + 1	frequency MSB 8bit
;	__tmp		frequency LSB 8bit
;=======================================================================
.proc	N163_frequency

	jsr	_nsd_div192		; 
	cmp	#$50
	bcc	@L50
	cmp	#$9F
	bcc	@L9F
	ldy	#4
	bne	@L
@L9F:	ldy	#3
@L:	sty	__ptr		;frequency HSB
	sub	#$50
	shl	a, 1
	tay
	lda	Freq_N163_50 + 1,y
	sta	__tmp + 1	;frequency MSB
	lda	Freq_N163_50,y	;frequency LSB
	jmp	@L2

@L50:	ldy	#2
	sty	__ptr		;frequency HSB
	shl	a, 1
	tay
	lda	Freq_N163 + 1,y
	sta	__tmp + 1	;frequency MSB
	lda	Freq_N163,y	;frequency LSB
@L2:

	;-------------------------------
	; *** Octave caluclate  and  overflow check
Octave_Proc:
	;if (octave == 0) {
	cpx	#$08
	bcc	Octave_Loop
	bne	@Over
	sta	__tmp
	lda	__ptr
	cmp	#4		;if (frequency >= 0x040000) {
	bcc	Detune
@Over:	lda	#$3
	sta	__ptr
	lda	#$FF
	sta	__tmp + 1	;	frequency = 0x03FFFF;1
	bne	Octave_Exit	;
Octave_Loop:
	lsr	__ptr		;高速化のため、
	ror	__tmp + 1	;ゼロページとaレジスターで
	ror	a		;シフト演算する。
	inx
	cpx	#$08
	bne	Octave_Loop
Octave_Exit:
	sta	__tmp

Detune:	
	ldy	#$00
	ldx	__channel
	lda	__detune_fine,x
	bpl	@L
	dey			; ay = __detune_fine (sign expand)
@L:	sty	__ptr + 1
	add	__tmp
	sta	__tmp
	tya
	adc	__tmp + 1
	sta	__tmp + 1
	lda	__ptr + 1
	adc	__ptr
	sta	__ptr

	;-------------------------------
	; *** Exit
Exit:
	rts
.endproc
.endif

