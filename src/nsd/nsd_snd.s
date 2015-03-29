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

	.export		_nsd_dpcm_calc

	.import		_nsd_div192

	.import		nsd_work
	.importzp	nsd_work_zp

.ifdef	DPCMBank
	.import		_nsd_ptr_bank
.endif

.if	.defined(VRC7) || .defined(OPLL)
	.export		_Wait42
.endif

	.include	"nes.inc"
	.include	"nsddef.inc"
	.include	"macro.inc"



.ifdef	DPCMBank
	.import		_nsd_bank_change
.endif

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
	sta	VRC6_Pulse1_CTUNE
	sta	VRC6_Pulse2_CTUNE
	sta	VRC6_SAW_CTUNE
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

	lda	#$40
	sta	APU_PAD2		; SOFTCLK (RW)

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
;	void	__fastcall__	_nsd_dpcm_calc(void);
;-----------------------------------------------------------------------
;<<Contents>>
;	dpcm info
;<<Input>>
;	nothing
;<<Output>>
;	__ptr	dpcm info address
;	y
;=======================================================================
.proc	_nsd_dpcm_calc

	lda	#0
	sta	__tmp + 1
	lda	__note,x

	asl	a
	rol	__tmp + 1

.ifdef	DPCMBank
	;__ptr = a * 2
	sta	__ptr
	lda	__tmp + 1
	sta	__ptr + 1
	lda	__ptr
.endif

	asl	a
	rol	__tmp + 1	; __tmp + 1  <=  a の 上位2bit

.ifdef	DPCMBank
	add	__ptr
	tay			;y <- address
	lda	__tmp + 1
	adc	__ptr + 1
	sta	__tmp + 1
	tya
.endif

	add	__dpcm_info + 0
	sta	__ptr + 0
	lda	__dpcm_info + 1
	adc	__tmp + 1
	sta	__ptr + 1

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
	.addr	_nsd_opll_keyon
	.addr	_nsd_opll_keyon
	.addr	_nsd_opll_keyon
	.addr	_nsd_opll_keyon
	.addr	_nsd_opll_keyon
	.addr	_nsd_opll_keyon
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
	lda	__apu_tri_time
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

	lda	#nsd_flag::Jump
	bit	__flag
	bne	@E

	lda	#$0F
	sta	APU_CHANCTRL

	jsr	_nsd_dpcm_calc

.ifdef	DPCMBank
	jsr	_nsd_ptr_bank
	;bank number
	ldy	#4
	lda	(__ptr),y
	jsr	_nsd_bank_change
.endif

	;Set the DPCM resister
	;CTRL (note etc..)
	ldy	#0
	lda	(__ptr),y
	sta	APU_MODCTRL

	;volume
	iny
	lda	(__ptr),y
	bmi	@L			;if 0x80 >= a then skip
	sta	APU_MODDA
@L:
	;address
	iny
	lda	(__ptr),y
	sta	APU_MODADDR

	;length
	iny
	lda	(__ptr),y
	sta	APU_MODLEN

	lda	#$1F
	sta	APU_CHANCTRL
@E:
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
.ifdef	VRC7
_nsd_vrc7_keyon:

	lda	__chflag,x		;[4]4
	and	#nsd_chflag::NoKeyOff
	beq	@exit

	;KeyOffしていないので、onする前に、offする。
	txa				;[2]12
	sub	#nsd::TR_VRC7		;[]
	shr	a, 1			;[2]
	tay				;[2]
	add	#VRC7_Octave		;[2]
	sta	VRC7_Resister		;レジスターをセット

	lda	__vrc7_freq_old,y	;[5]
	and	#$EF			;[2]
	sta	__vrc7_freq_old,y	;[5]
	sta	VRC7_Data		;


@exit:
	lda	__chflag,x
	ora	#nsd_chflag::KeyOn
	sta	__chflag,x

_nsd_vrc7_keyon_exit:
	rts
.endif
;---------------------------------------
.ifdef	OPLL
_nsd_opll_keyon_R:

	;OPLL_Rhythm check
	lda	__opll_ryhthm
	cmp	#$20
	bcs	_nsd_opll_keyon_exit

_nsd_opll_keyon:

	lda	__chflag,x		;[4]4
	and	#nsd_chflag::NoKeyOff
	beq	@exit

	;KeyOffしていないので、onする前に、offする。
	txa				;[2]12
	sub	#nsd::TR_OPLL		;[]
	shr	a, 1			;[2]
	tay				;[2]
	add	#OPLL_Octave		;[2]
	sta	OPLL_Resister		;レジスターをセット

	lda	__opll_freq_old,y	;[5]
	and	#$EF			;[2]
	sta	__opll_freq_old,y	;[5]
	sta	OPLL_Data		;

@exit:
	lda	__chflag,x
	ora	#nsd_chflag::KeyOn
	sta	__chflag,x

_nsd_opll_keyon_exit:
	rts
.endif
.endproc


;---------------------------------------
.ifdef	OPLL

.proc	_nsd_opll_BD_keyon
	lda	#$10
	bne	_nsd_opll_rhythm_set_or
.endproc

.proc	_nsd_opll_SD_keyon
	lda	#$08
	bne	_nsd_opll_rhythm_set_or
.endproc

.proc	_nsd_opll_HH_keyon
	lda	#$01
	bne	_nsd_opll_rhythm_set_or
.endproc

.proc	_nsd_opll_Cym_keyon
	lda	#$02
	bne	_nsd_opll_rhythm_set_or
.endproc

.proc	_nsd_opll_Tom_keyon
	lda	#$04
;	bne	_nsd_opll_rhythm_set_or
.endproc

.proc	_nsd_opll_rhythm_set_or
	ora	__opll_ryhthm
	bne	_nsd_opll_rhythm_set		;※必ず1以上
.endproc

.proc	_nsd_opll_BD_keyoff
	lda	#~$10
	bne	_nsd_opll_rhythm_set_and
.endproc

.proc	_nsd_opll_SD_keyoff
	lda	#~$08
	bne	_nsd_opll_rhythm_set_and
.endproc

.proc	_nsd_opll_HH_keyoff
	lda	#~$01
	bne	_nsd_opll_rhythm_set_and
.endproc

.proc	_nsd_opll_Cym_keyoff
	lda	#~$02
	bne	_nsd_opll_rhythm_set_and
.endproc

.proc	_nsd_opll_Tom_keyoff
	lda	#~$04
;	bne	_nsd_opll_rhythm_set_and
.endproc

.proc	_nsd_opll_rhythm_set_and
	and	__opll_ryhthm
;	jmp	_nsd_opll_rhythm_set
.endproc

.proc	_nsd_opll_rhythm_set
	sta	__opll_ryhthm

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

	lda	__chflag,x		;[4]4
	and	#nsd_chflag::NoKeyOff
	bne	@exit

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

	lda	__vrc7_freq_old,y	;[5]
	and	#$EF			;[2]
	sta	__vrc7_freq_old,y	;[5]
	sta	VRC7_Data		;

@exit:
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

	lda	__chflag,x		;[4]4
	and	#nsd_chflag::NoKeyOff
	bne	_nsd_OPLL_keyoff_exit

	;取りあえず、必ずKeyOffする。
	lda	__chflag,x		;[4]4
	and	#~nsd_chflag::KeyOn	;[2]6
	sta	__chflag,x		;[4]10

	;書かれない可能性があるので、ここでレジスタに書く。
	txa				;[2]12
	sub	#nsd::TR_OPLL		;[]
	shr	a, 1			;[2]
	tay				;[2]
	add	#OPLL_Octave		;[2]
	sta	OPLL_Resister		;レジスターをセット

	lda	__opll_freq_old,y	;[5]
	and	#$EF			;[2]
	sta	__opll_freq_old,y	;[5]
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
JMPTBL:	.addr	_nsd_ch1_voice		;BGM ch1 Pulse
	.addr	_nsd_ch2_voice		;BGM ch2 Pulse
	.addr	Exit			;BGM ch3 Triangle	-- no process --
	.addr	_nsd_noise_voice	;BGM ch4 Noize
	.addr	Exit			;BGM ch5 DPCM
.ifdef	FDS
	.addr	_nsd_fds_GainMod
.endif
.ifdef	VRC6
	.addr	_nes_vrc6_ch1_voice
	.addr	_nes_vrc6_ch2_voice
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
	.addr	_nes_opll_voice
	.addr	_nes_opll_voice
	.addr	_nes_opll_voice
	.addr	_nes_opll_voice
	.addr	_nes_opll_voice
	.addr	_nes_opll_voice
	.addr	_nes_opll_voice
	.addr	_nes_opll_voice
	.addr	_nes_opll_voice
	.addr	Exit
	.addr	Exit
	.addr	Exit
	.addr	Exit
	.addr	Exit
.endif
.ifdef	MMC5
	.addr	_nsd_mmc5_ch1_voice		;仕組みは同じ
	.addr	_nsd_mmc5_ch2_voice		;
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
	.addr	_nsd_noise_voice_se1	;SE  ch1 Pulse
	.addr	_nsd_noise_voice_se2	;SE  ch2 Noize

;---------------------------------------
.code
	ldy	JMPTBL,x
	sty	__ptr
	ldy	JMPTBL + 1,x
	sty	__ptr + 1
	jmp	(__ptr)

;---------------------------------------
_nsd_ch1_voice:

	;-------------------------------
	; *** Calculate the voice
;	shl	a, 6	;a <<= 6
	ror	a
	ror	a
	ror	a
	and	#$C0	;a &= 0xF0	;for OR to volume(lower 4bit)

	;-------------------------------
	; *** Set the voice to work
	sta	__apu_voice_set1

	;-------------------------------
	; *** Exit
Exit:
	rts

;---------------------------------------
_nsd_ch2_voice:

	;-------------------------------
	; *** Calculate the voice
;	shl	a, 6	;a <<= 6
	ror	a
	ror	a
	ror	a
	and	#$C0	;a &= 0xF0	;for OR to volume(lower 4bit)

	;-------------------------------
	; *** Set the voice to work
	sta	__apu_voice_set2

	rts

;---------------------------------------
_nsd_noise_voice_se1:

	;-------------------------------
	; *** Calculate the voice
;	shl	a, 6	;a <<= 6
	ror	a
	ror	a
	ror	a
	and	#$C0	;a &= 0xF0	;for OR to volume(lower 4bit)

	;-------------------------------
	; *** Set the voice to work
	sta	__se_voice_set1

	rts

;---------------------------------------
_nsd_noise_voice:

	;-------------------------------
	; *** Calculate the voice
;	shl	a, 7	;a <<= 7
	ror	a
	ror	a
	and	#$80	;a &= 0x80	;for OR to frequency(lower 4bit)

	;-------------------------------
	; *** Set the voice to work
	sta	__apu_voice_set4

	;-------------------------------
	; *** Exit

	lda	__frequency,x		;下位8bitだけあればいい。
	jmp	_nsd_nes_ch4_frequency

;	rts

;---------------------------------------
_nsd_noise_voice_se2:

	;-------------------------------
	; *** Calculate the voice
;	shl	a, 7	;a <<= 7
	ror	a
	ror	a
	and	#$80	;a &= 0x80	;for OR to frequency(lower 4bit)

	;-------------------------------
	; *** Set the voice to work
	sta	__se_voice_set2

	;-------------------------------
	; *** Exit

	lda	__frequency,x		;下位8bitだけあればいい。
	jmp	_nsd_nes_se2_frequency

;	rts


;---------------------------------------
.ifdef	MMC5
_nsd_mmc5_ch1_voice:

	;-------------------------------
	; *** Calculate the voice
;	shl	a, 6	;a <<= 6
	ror	a
	ror	a
	ror	a
	and	#$C0	;a &= 0xF0	;for OR to volume(lower 4bit)

	;-------------------------------
	; *** Set the voice to work
	sta	__mmc5_voice_set1

	;-------------------------------
	; *** Exit
	rts

;---------------------------------------
_nsd_mmc5_ch2_voice:

	;-------------------------------
	; *** Calculate the voice
;	shl	a, 6	;a <<= 6
	ror	a
	ror	a
	ror	a
	and	#$C0	;a &= 0xF0	;for OR to volume(lower 4bit)

	;-------------------------------
	; *** Set the voice to work
	sta	__mmc5_voice_set2

	;-------------------------------
	; *** Exit
	rts
.endif

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
_nes_vrc6_ch1_voice:
	;-------------------------------
	; *** Calculate the voice
	shl	a, 4	;a <<= 6
	and	#$70	;a &= 0xF0	;for OR to volume(lower 4bit)

	;-------------------------------
	; *** Set the voice to work
	sta	__vrc6_voice_set1

	;-------------------------------
	; *** Exit
	rts

;---------------------------------------
_nes_vrc6_ch2_voice:
	;-------------------------------
	; *** Calculate the voice
	shl	a, 4	;a <<= 6
	and	#$70	;a &= 0xF0	;for OR to volume(lower 4bit)

	;-------------------------------
	; *** Set the voice to work
	sta	__vrc6_voice_set2

	;-------------------------------
	; *** Exit
	rts

.endif

;---------------------------------------
.ifdef	VRC7
_nes_vrc7_voice:

;	 CSV
;	000x xxxx	val
;	0010 xxxx	ML(M)
;	0011 xxxx	ML(C)
;	01xx xxxx	TL

	ldy	__vrc7_reg
	sty	__ptr
	ldy	__vrc7_reg + 1
	sty	__ptr + 1

.ifdef	DPCMBank
	jsr	_nsd_ptr_bank
.endif

	sta	__tmp + 1
	asl	a
	asl	a
	bcs	@TL
	pha
	and	#%11110111	;Reset D
	ora	#%00000100	;Set   I
	pha
	plp
	pla
	bpl	@Voice
	bvs	@MLC

@MLM:
	ldy	#0
	sty	VRC7_Resister
	lda	(__ptr),y
	and	#$F0
	sta	__tmp
	lda	__tmp + 1
	and	#$0F
	ora	__tmp
	sta	VRC7_Data
	jmp	@Exit

@MLC:
	ldy	#1
	sty	VRC7_Resister
	lda	(__ptr),y
	and	#$F0
	sta	__tmp
	lda	__tmp + 1
	and	#$0F
	ora	__tmp
	sta	VRC7_Data
	jmp	@Exit

@TL:
	ldy	#2
	sty	VRC7_Resister
	lda	(__ptr),y
	and	#$C0
	sta	__tmp
	lda	__tmp + 1
	and	#$3F
	ora	__tmp
	sta	VRC7_Data
	jmp	@Exit

@Voice:	
	;-------------------------------
	; *** Set the voice to work
	;チャンネルの計算
	sta	__tmp
	txa				;[2]9
	sub	#nsd::TR_VRC7		;[3]12
	shr	a, 1			;[2]14
	tay				;[2]16

	;-------------------------------
	; *** Calculate the voice
	lda	__tmp
	shl	a, 2	;a <<= 4
	and	#$F0	;a &= 0xF0	;for OR to volume(lower 4bit)

	sta	__vrc7_voice_set,y

	;-------------------------------
	; *** Exit
@Exit:	rts
.endif

;---------------------------------------
.ifdef	OPLL
_nes_opll_voice:

;	 CSV
;	000x xxxx	val
;	0010 xxxx	ML(M)
;	0011 xxxx	ML(C)
;	01xx xxxx	TL

	ldy	__opll_reg
	sty	__ptr
	ldy	__opll_reg + 1
	sty	__ptr + 1

.ifdef	DPCMBank
	jsr	_nsd_ptr_bank
.endif

	sta	__tmp + 1
	asl	a
	asl	a
	bcs	@TL
	pha
	and	#%11110111	;Reset D
	ora	#%00000100	;Set   I
	pha
	plp
	pla
	bpl	@Voice
	bvs	@MLC

@MLM:
	ldy	#0
	sty	OPLL_Resister
	lda	(__ptr),y
	and	#$F0
	sta	__tmp
	lda	__tmp + 1
	and	#$0F
	ora	__tmp
	sta	OPLL_Data
	jmp	@Exit

@MLC:
	ldy	#1
	sty	OPLL_Resister
	lda	(__ptr),y
	and	#$F0
	sta	__tmp
	lda	__tmp + 1
	and	#$0F
	ora	__tmp
	sta	OPLL_Data
	jmp	@Exit

@TL:
	ldy	#2
	sty	OPLL_Resister
	lda	(__ptr),y
	and	#$C0
	sta	__tmp
	lda	__tmp + 1
	and	#$3F
	ora	__tmp
	sta	OPLL_Data
	jmp	@Exit

@Voice:	
	;-------------------------------
	; *** Set the voice to work
	;チャンネルの計算
	sta	__tmp
	txa				;[2]9
	sub	#nsd::TR_OPLL		;[3]12
	shr	a, 1			;[2]14
	tay				;[2]16

	;-------------------------------
	; *** Calculate the voice
	lda	__tmp
	shl	a, 2	;a <<= 4
	and	#$F0	;a &= 0xF0	;for OR to volume(lower 4bit)

	sta	__opll_voice_set,y

	;-------------------------------
	; *** Exit
@Exit:	rts
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
	bne	_nsd_n163_ch1_voice_set

_nsd_n163_ch3_voice:
	ldy	__n163_num
	cpy	#$20
	bcc	_nsd_n163_ch1_voice_exit	;1未満だったら終了
	ldy	#N163_Waveform - (8 * 2)
	bne	_nsd_n163_ch1_voice_set

_nsd_n163_ch4_voice:
	ldy	__n163_num
	cpy	#$30
	bcc	_nsd_n163_ch1_voice_exit	;1未満だったら終了
	ldy	#N163_Waveform - (8 * 3)
	bne	_nsd_n163_ch1_voice_set

_nsd_n163_ch5_voice:
	ldy	__n163_num
	cpy	#$40
	bcc	_nsd_n163_ch1_voice_exit	;1未満だったら終了
	ldy	#N163_Waveform - (8 * 4)
	bne	_nsd_n163_ch1_voice_set

_nsd_n163_ch6_voice:
	ldy	__n163_num
	cpy	#$50
	bcc	_nsd_n163_ch1_voice_exit	;1未満だったら終了
	ldy	#N163_Waveform - (8 * 5)
	bne	_nsd_n163_ch1_voice_set

_nsd_n163_ch7_voice:
	ldy	__n163_num
	cpy	#$60
	bcc	_nsd_n163_ch1_voice_exit	;1未満だったら終了
	ldy	#N163_Waveform - (8 * 6)
	bne	_nsd_n163_ch1_voice_set

_nsd_n163_ch8_voice:
	ldy	__n163_num
	cpy	#$70
	bcc	_nsd_n163_ch1_voice_exit	;1未満だったら終了
	ldy	#N163_Waveform - (8 * 7)
	bne	_nsd_n163_ch1_voice_set

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
;<<Note>>
;	■■■※最終的なレジスタの更新も行うので、毎フレーム必ず呼ぶこと！！
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
	ora	__apu_voice_set1

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

	;-------------------------------
	; *** Mix voice and volume
	;a = (a & 0x0F) | (nsd_word.Voice.voice_set & 0xF0)
	and	#$0F
	ora	#$30	;a |= 0x30	;counter on / hard-envelop off
	ora	__apu_voice_set2

	;-------------------------------
	; *** Output to NES sound device
	;y = x << 1
	sta	APU_PULSE2CTRL

	;-------------------------------
	; *** Exit
	rts

;---------------------------------------
_nsd_se1_volume:

	;-------------------------------
	; *** Mix voice and volume
	;a = (a & 0x0F) | (nsd_word.Voice.voice_set & 0xF0)
	and	#$0F
	ora	#$30	;a |= 0x30	;counter on / hard-envelop off
	ora	__se_voice_set1

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
	ora	__vrc6_voice_set1

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
	ora	__vrc6_voice_set2

	;-------------------------------
	; *** Output to NES sound device
	;y = x << 1
	sta	VRC6_Pulse2_CTRL

	;-------------------------------
	; *** Exit
	rts

;---------------------------------------
_nsd_vrc6_ch3_volume:
	and	#$3F
	sta	VRC6_SAW_CTRL
	rts

.endif

;---------------------------------------
.ifdef	VRC7
_nsd_vrc7_volume:


	eor	#$FF			;[2]2
	and	#$0F			;[2]4
	sta	__tmp			;[3]7	__tmp <- volume

	;チャンネルの計算
	txa				;[2]9
	sub	#nsd::TR_VRC7		;[3]12
	shr	a, 1			;[2]14
	tay				;[2]16
	pha				;[3]19

	;《音量・音色番号書き込み》
	;────────────────
	add	#VRC7_Volume		;[4]23
	sta	VRC7_Resister		;●Resister Write
	lda	__tmp			;[3]
	ora	__vrc7_voice_set,y	;[4]
	sta	VRC7_Data		;●Data Write

	;《周波数 下位byte 書き込み》
	;────────────────
	ldy	#0					;[2]2
	lda	__detune_fine,x				;[4]6
	bpl	@L					;[3] or [4]9	(Branch) or [4](not Branch)
	dey						;	ay = __detune_fine (sign expand)
@L:	add	__vrc7_frequency - nsd::TR_VRC7 + 0,x	;[6]15
	sta	__tmp					;[3]18
	tya						;[2]20
	adc	__vrc7_frequency - nsd::TR_VRC7 + 1,x	;[4]24
	and	#$0F					;[2]26
	sta	__tmp + 1				;[3]29	__tmp += (signed int)__detune_cent
	pla						;[4]33
	pha						;[3]36
	tay						;[2]38	y <- device channel
	add	#VRC7_Frequency				;[4]42 > 42 clock !! (VRC7のwait)
	sta	VRC7_Resister				;●Resister Write
	pla						;[4]
	lda	__tmp					;[2]6
	sta	VRC7_Data				;●Data Write

	;《フラグ・オクターブ・周波数上位byte書き込み》
	;────────────────
	lda	__chflag,x		;[4]
	and	#$30			;[2]6	 00XX 0000 <2>
	ora	__tmp + 1		;[3]9	flag と octave をマージ
	cmp	__vrc7_freq_old,y	;[4]13
	beq	@freq_exit		;[2]15	同じだったら書かない。
	sta	__vrc7_freq_old,y	;[5]20
	pha				;[3]23	*Wait
	pla				;[4]27	*Wait
	pha				;[3]30	*Wait
	pla				;[4]34	*Wait
	tya				;[2]36	*Wait
	tya				;[2]38
	add	#VRC7_Octave		;[4]42 > 42 clock !! (VRC7のwait)
	sta	VRC7_Resister		;●Resister Write
	tya				;[2]
	lda	__vrc7_freq_old,y	;[4]6 clock
	sta	VRC7_Data		;●Data Write

@freq_exit:

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

	eor	#$FF			;[2]2
	and	#$0F			;[2]4
	sta	__tmp			;[3]7	__tmp <- volume

	;チャンネルの計算
	txa				;[2]9
	sub	#nsd::TR_OPLL		;[3]12
	shr	a, 1			;[2]14
	tay				;[2]16
	pha				;[3]19

	;《音量・音色番号書き込み》
	;────────────────
	add	#OPLL_Volume		;[4]23
	sta	OPLL_Resister		;●Resister Write
	lda	__tmp			;[3]
	ora	__opll_voice_set,y	;[4]
	sta	OPLL_Data		;●Data Write

	;《周波数 下位byte 書き込み》
	;────────────────
	ldy	#0					;[2]2
	lda	__detune_fine,x				;[4]6
	bpl	@L					;[3] or [4]9	(Branch) or [4](not Branch)
	dey						;	ay = __detune_fine (sign expand)
@L:	add	__opll_frequency - nsd::TR_OPLL + 0,x	;[6]15
	sta	__tmp					;[3]18
	tya						;[2]20
	adc	__opll_frequency - nsd::TR_OPLL + 1,x	;[4]24
	and	#$0F					;[2]26
	sta	__tmp + 1				;[3]29	__tmp += (signed int)__detune_cent
	pla						;[4]33
	pha						;[3]36
	tay						;[2]38	y <- device channel
	add	#OPLL_Frequency				;[4]42 > 42 clock !! (OPLLのwait)
	sta	OPLL_Resister				;●Resister Write
	pla						;[4]
	lda	__tmp					;[2]6
	sta	OPLL_Data				;●Data Write

	;《フラグ・オクターブ・周波数上位byte書き込み》
	;────────────────
	lda	__chflag,x		;[4]
	and	#$30			;[2]6	 00XX 0000 <2>
	ora	__tmp + 1		;[3]9	flag と octave をマージ
	cmp	__opll_freq_old,y	;[4]13
	beq	@freq_exit		;[2]15	同じだったら書かない。
	sta	__opll_freq_old,y	;[5]20
	pha				;[3]23	*Wait
	pla				;[4]27	*Wait
	pha				;[3]30	*Wait
	pla				;[4]34	*Wait
	tya				;[2]36	*Wait
	tya				;[2]38
	add	#OPLL_Octave		;[4]42 > 42 clock !! (OPLLのwait)
	sta	OPLL_Resister		;●Resister Write
	tya				;[2]
	lda	__opll_freq_old,y	;[4]6 clock
	sta	OPLL_Data		;●Data Write

@freq_exit:
_nsd_OPLL_volume_exit:

	rts				;[6]
;---------------------------------------
_nsd_opll_BD_volume:
	lda	#$0F
	sta	__tmp
	lda	__volume + nsd::TR_OPLL + 18
	sta	__tmp + 1
	ldy	#OPLL_BD
	bne	_nsd_opll_DR_volume

_nsd_opll_SD_HH_volume:
	lda	__volume + nsd::TR_OPLL + 22
	sta	__tmp
	lda	__volume + nsd::TR_OPLL + 20
	sta	__tmp + 1
	ldy	#OPLL_HH_SD
	bne	_nsd_opll_DR_volume

_nsd_opll_Cym_Tom_volume:
	lda	__volume + nsd::TR_OPLL + 26
	sta	__tmp
	lda	__volume + nsd::TR_OPLL + 24
	sta	__tmp + 1
	ldy	#OPLL_TOM_CYM
;	bne	_nsd_opll_DR_volume

_nsd_opll_DR_volume:
	lda	__opll_ryhthm
	cmp	#$20
	bcc	@Exit

	sty	OPLL_Resister
	lda	__tmp
	shl	a, 4
	sta	__tmp
	lda	__tmp + 1
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
	ora	__mmc5_voice_set1

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
	ora	__mmc5_voice_set2

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
	bne	_nsd_n163_ch1_volume_Set

;---------------------------------------
_nsd_n163_ch4_volume:
	ldy	__n163_num
	cpy	#$30
	bcc	_nsd_n163_ch1_volume_Exit	;1未満だったら終了
	ldy	#N163_Volume - (8 * 3)
	bne	_nsd_n163_ch1_volume_Set

;---------------------------------------
_nsd_n163_ch5_volume:
	ldy	__n163_num
	cpy	#$40
	bcc	_nsd_n163_ch1_volume_Exit	;1未満だったら終了
	ldy	#N163_Volume - (8 * 4)
	bne	_nsd_n163_ch1_volume_Set

;---------------------------------------
_nsd_n163_ch6_volume:
	ldy	__n163_num
	cpy	#$50
	bcc	_nsd_n163_ch1_volume_Exit	;1未満だったら終了
	ldy	#N163_Volume - (8 * 5)
	bne	_nsd_n163_ch1_volume_Set

;---------------------------------------
_nsd_n163_ch7_volume:
	ldy	__n163_num
	cpy	#$60
	bcc	_nsd_n163_ch1_volume_Exit	;1未満だったら終了
	ldy	#N163_Volume - (8 * 6)
	bne	_nsd_n163_ch1_volume_Set

;---------------------------------------
_nsd_n163_ch8_volume:
	ldy	__n163_num
	cpy	#$70
	bcc	_nsd_n163_ch1_volume_Exit	;1未満だったら終了
	ldy	#N163_Volume - (8 * 7)
	bne	_nsd_n163_ch1_volume_Set

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
	bne	_nsd_psg_set_volume

_nsd_psg_ch3_volume:
	ldy	#PSG_3_Volume
	sty	PSG_Register
	bne	_nsd_psg_set_volume

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
	.addr	_nsd_ch3_time		;BGM ch3 Triangle	-- no process --
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

;---------------------------------------
_nsd_ch3_time:
	sta	__apu_tri_time
	rts
.endproc
;---------------------------------------
.ifdef	FDS
.proc	_nsd_fds_sweep_bias
	and	#$7F
	sta	FDS_Sweep_Bias
	sta	__fds_sweepbias
	rts
.endproc
.endif

;---------------------------------------
.if	.defined(VRC7) || .defined(OPLL)
.proc	_nsd_vrc7_sustain

	ror	a		;c <= 0bit
	tay			;
	lda	__chflag,x

	bcc	@Sustain
	ora	#nsd_chflag::Sustain
	bne	@Sustain_E
@Sustain:
	and	#~nsd_chflag::Sustain
@Sustain_E:

	cpy	#0
	beq	@NoKeyOff
	ora	#nsd_chflag::NoKeyOff
	bne	@NoKeyOff_E
@NoKeyOff:
	and	#~nsd_chflag::NoKeyOff
@NoKeyOff_E:

;	tay
;	lda	__chflag,x
;	cpy	#0
;	bne	@L
;	and	#~nsd_chflag::Sustain
;	jmp	@Set
;@L:	ora	#nsd_chflag::Sustain

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
	nop
	nop
	sta	OPLL_Data
	jsr	_Wait42

L02:
	ldy	#$17
	sty	OPLL_Resister
	lda	#$50
	nop
	nop
	sta	OPLL_Data
	jsr	_Wait42

L03:
	ldy	#$18
	sty	OPLL_Resister
	lda	#$C0
	nop
	nop
	sta	OPLL_Data
	jsr	_Wait42

L04:
	ldy	#$26
	sty	OPLL_Resister
	lda	#$05
	nop
	nop
	sta	OPLL_Data
	jsr	_Wait42

L05:
	ldy	#$27
	sty	OPLL_Resister
	lda	#$05
	nop
	nop
	sta	OPLL_Data
	jsr	_Wait42

L06:
	ldy	#$28
	sty	OPLL_Resister
	lda	#$01
	nop
	nop
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
	cmp	#$10
	bcc	@L
	ldy	#PSG_Envelope_Form
	sty	PSG_Register
	tay
	and	#$0F
	sta	PSG_Data
	tya
@L:
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
	.word	$0D35
	.word	$0D1D
	.word	$0D04
	.word	$0CEC
	.word	$0CD5
	.word	$0CBD
	.word	$0CA6
	.word	$0C8E	;Cis / Ds
	.word	$0C77
	.word	$0C60
	.word	$0C49
	.word	$0C33
	.word	$0C1C
	.word	$0C06
	.word	$0BF0
	.word	$0BDA	;D
	.word	$0BC4
	.word	$0BAE
	.word	$0B99
	.word	$0B84
	.word	$0B6E
	.word	$0B59
	.word	$0B44
	.word	$0B30	;Dis / Es
	.word	$0B1B
	.word	$0B07
	.word	$0AF2
	.word	$0ADE
	.word	$0ACA
	.word	$0AB6
	.word	$0AA2
	.word	$0A8F	;E
	.word	$0A7B
	.word	$0A68
	.word	$0A55
	.word	$0A42
	.word	$0A2F
	.word	$0A1C
	.word	$0A0A
	.word	$09F7	;F
	.word	$09E5
	.word	$09D3
	.word	$09C1
	.word	$09AF
	.word	$099D
	.word	$098B
	.word	$0979
	.word	$0968	;Fis / Ges
	.word	$0957
	.word	$0945
	.word	$0934
	.word	$0923
	.word	$0913
	.word	$0902
	.word	$08F1
	.word	$08E1	;G
	.word	$08D1
	.word	$08C0
	.word	$08B0
	.word	$08A0
	.word	$0890
	.word	$0880
	.word	$0871
	.word	$0861	;Gis / As
	.word	$0852
	.word	$0843
	.word	$0833
	.word	$0824
	.word	$0815
	.word	$0806
	.word	$07F8
	.word	$07E9	;A
	.word	$07DA
	.word	$07CC
	.word	$07BD
	.word	$07AF
	.word	$07A1
	.word	$0793
	.word	$0785
	.word	$0777	;B
	.word	$0769
	.word	$075C
	.word	$074E
	.word	$0741
	.word	$0733
	.word	$0726
	.word	$0719
	.word	$070C	;H
	.word	$06FF
	.word	$06F2
	.word	$06E5
	.word	$06D9
	.word	$06CC
	.word	$06BF
	.word	$06B3
	.word	$06A7	;`C （線形補完用）


;---------------------------------------
;FDS Frequency table
.ifdef	FDS
Freq_FDS:
	.word	$09A0	;C
	.word	$09B2
	.word	$09C4
	.word	$09D6
	.word	$09E8
	.word	$09FA
	.word	$0A0D
	.word	$0A1F
	.word	$0A32	;Cis / Ds
	.word	$0A45
	.word	$0A58
	.word	$0A6B
	.word	$0A7F
	.word	$0A92
	.word	$0AA6
	.word	$0ABA
	.word	$0ACD	;D
	.word	$0AE1
	.word	$0AF6
	.word	$0B0A
	.word	$0B1E
	.word	$0B33
	.word	$0B48
	.word	$0B5D
	.word	$0B72	;Dis / Es
	.word	$0B87
	.word	$0B9C
	.word	$0BB2
	.word	$0BC8
	.word	$0BDE
	.word	$0BF4
	.word	$0C0A
	.word	$0C20	;E
	.word	$0C37
	.word	$0C4D
	.word	$0C64
	.word	$0C7B
	.word	$0C92
	.word	$0CA9
	.word	$0CC1
	.word	$0CD9	;F
	.word	$0CF0
	.word	$0D08
	.word	$0D21
	.word	$0D39
	.word	$0D51
	.word	$0D6A
	.word	$0D83
	.word	$0D9C	;Fis / Ges
	.word	$0DB5
	.word	$0DCF
	.word	$0DE8
	.word	$0E02
	.word	$0E1C
	.word	$0E36
	.word	$0E51
	.word	$0E6B	;G
	.word	$0E86
	.word	$0EA1
	.word	$0EBC
	.word	$0ED7
	.word	$0EF3
	.word	$0F0F
	.word	$0F2B
	.word	$0F47	;Gis / As
	.word	$0F63
	.word	$0F80
	.word	$0F9C
	.word	$0FB9
	.word	$0FD6
	.word	$0FF4
	.word	$1011
	.word	$102F	;A
	.word	$104D
	.word	$106C
	.word	$108A
	.word	$10A9
	.word	$10C8
	.word	$10E7
	.word	$1106
	.word	$1126	;B
	.word	$1145
	.word	$1165
	.word	$1186
	.word	$11A6
	.word	$11C7
	.word	$11E8
	.word	$1209
	.word	$122B	;H
	.word	$124C
	.word	$126E
	.word	$1290
	.word	$12B3
	.word	$12D6
	.word	$12F8
	.word	$131C
	.word	$133F	;`C （線形補完用）
.endif

;---------------------------------------
;SAW Frequency table
.ifdef	VRC6
Freq_SAW:
	.word	$0F34	;C
	.word	$0F18
	.word	$0EFD
	.word	$0EE1
	.word	$0EC6
	.word	$0EAA
	.word	$0E8F
	.word	$0E75
	.word	$0E5A
	.word	$0E40
	.word	$0E25
	.word	$0E0B
	.word	$0DF1
	.word	$0DD8
	.word	$0DBE
	.word	$0DA5
	.word	$0D8C	;D
	.word	$0D73
	.word	$0D5A
	.word	$0D42
	.word	$0D29
	.word	$0D11
	.word	$0CF9
	.word	$0CE1
	.word	$0CC9
	.word	$0CB2
	.word	$0C9A
	.word	$0C83
	.word	$0C6C
	.word	$0C55
	.word	$0C3E
	.word	$0C28
	.word	$0C12	;E
	.word	$0BFB
	.word	$0BE5
	.word	$0BCF
	.word	$0BBA
	.word	$0BA4
	.word	$0B8F
	.word	$0B79
	.word	$0B64	;F
	.word	$0B4F
	.word	$0B3A
	.word	$0B26
	.word	$0B11
	.word	$0AFD
	.word	$0AE9
	.word	$0AD4
	.word	$0AC1
	.word	$0AAD
	.word	$0A99
	.word	$0A86
	.word	$0A72
	.word	$0A5F
	.word	$0A4C
	.word	$0A39
	.word	$0A26	;G
	.word	$0A13
	.word	$0A01
	.word	$09EE
	.word	$09DC
	.word	$09CA
	.word	$09B8
	.word	$09A6
	.word	$0994
	.word	$0983
	.word	$0971
	.word	$0960
	.word	$094E
	.word	$093D
	.word	$092C
	.word	$091B
	.word	$090B	;A
	.word	$08FA
	.word	$08E9
	.word	$08D9
	.word	$08C9
	.word	$08B9
	.word	$08A9
	.word	$0899
	.word	$0889	;B
	.word	$0879
	.word	$0869
	.word	$085A
	.word	$084B
	.word	$083B
	.word	$082C
	.word	$081D
	.word	$080E	;H
	.word	$07FF
	.word	$07F1
	.word	$07E2
	.word	$07D3
	.word	$07C5
	.word	$07B7
	.word	$07A9
	.word	$079A	;`C （線形補完用）
.endif

;---------------------------------------
;VRC7 Frequency table
.if	.defined(VRC7) || .defined(OPLL)
Freq_VRC7:
	.byte	$AD	;C
	.byte	$AE
	.byte	$B0
	.byte	$B1
	.byte	$B2
	.byte	$B4
	.byte	$B5
	.byte	$B6
	.byte	$B8
	.byte	$B9
	.byte	$BA
	.byte	$BC
	.byte	$BD
	.byte	$BE
	.byte	$C0
	.byte	$C1
	.byte	$C2	;D
	.byte	$C4
	.byte	$C5
	.byte	$C7
	.byte	$C8
	.byte	$CA
	.byte	$CB
	.byte	$CD
	.byte	$CE
	.byte	$CF
	.byte	$D1
	.byte	$D3
	.byte	$D4
	.byte	$D6
	.byte	$D7
	.byte	$D9
	.byte	$DA	;E
	.byte	$DC
	.byte	$DD
	.byte	$DF
	.byte	$E1
	.byte	$E2
	.byte	$E4
	.byte	$E6
	.byte	$E7	;F
	.byte	$E9
	.byte	$EB
	.byte	$EC
	.byte	$EE
	.byte	$F0
	.byte	$F1
	.byte	$F3
	.byte	$F5
	.byte	$F7
	.byte	$F9
	.byte	$FA
	.byte	$FC
	.byte	$FE
	.byte	$00
	.byte	$02
	.byte	$04	;G
	.byte	$05
	.byte	$07
	.byte	$09
	.byte	$0B
	.byte	$0D
	.byte	$0F
	.byte	$11
	.byte	$13
	.byte	$15
	.byte	$17
	.byte	$19
	.byte	$1B
	.byte	$1D
	.byte	$1F
	.byte	$21
	.byte	$23	;A
	.byte	$25
	.byte	$28
	.byte	$2A
	.byte	$2C
	.byte	$2E
	.byte	$30
	.byte	$32
	.byte	$35	;B
	.byte	$37
	.byte	$39
	.byte	$3B
	.byte	$3E
	.byte	$40
	.byte	$42
	.byte	$45
	.byte	$47	;H
	.byte	$49
	.byte	$4C
	.byte	$4E
	.byte	$51
	.byte	$53
	.byte	$55
	.byte	$58
	.byte	$5A	;`C （線形補完用）
.endif

;---------------------------------------
;N163 Frequency table
.ifdef	N163
Freq_N163:
	.word	$4169
	.word	$4380
	.word	$4598
	.word	$47B3
	.word	$49CF
	.word	$4BED
	.word	$4E0E
	.word	$5030
	.word	$5254
	.word	$547B
	.word	$56A3
	.word	$58CD
	.word	$5AF9
	.word	$5D28
	.word	$5F58
	.word	$618A
	.word	$63BF
	.word	$65F5
	.word	$682E
	.word	$6A68
	.word	$6CA5
	.word	$6EE3
	.word	$7124
	.word	$7367
	.word	$75AC
	.word	$77F3
	.word	$7A3C
	.word	$7C87
	.word	$7ED4
	.word	$8124
	.word	$8375
	.word	$85C9
	.word	$881F
	.word	$8A77
	.word	$8CD1
	.word	$8F2E
	.word	$918C
	.word	$93ED
	.word	$9650
	.word	$98B5
	.word	$9B1D
	.word	$9D86
	.word	$9FF2
	.word	$A260
	.word	$A4D1
	.word	$A743
	.word	$A9B8
	.word	$AC30
	.word	$AEA9
	.word	$B125
	.word	$B3A3
	.word	$B623
	.word	$B8A6
	.word	$BB2B
	.word	$BDB2
	.word	$C03C
	.word	$C2C8
	.word	$C556
	.word	$C7E7
	.word	$CA7A
	.word	$CD10
	.word	$CFA8
	.word	$D242
	.word	$D4DF
	.word	$D77E
	.word	$DA1F
	.word	$DCC3
	.word	$DF6A
	.word	$E213
	.word	$E4BE
	.word	$E76C
	.word	$EA1C
	.word	$ECCF
	.word	$EF84
	.word	$F23C
	.word	$F4F6
	.word	$F7B3
	.word	$FA72
	.word	$FD34
	.word	$FFF9
Freq_N163_50:
	.word	$02C0
	.word	$058A
	.word	$0856
	.word	$0B24
	.word	$0DF6
	.word	$10CA
	.word	$13A0
	.word	$167A
	.word	$1956
	.word	$1C34
	.word	$1F15
	.word	$21F9
	.word	$24E0
	.word	$27C9
	.word	$2AB5
	.word	$2DA3
	.word	$3095
	.word	$3389
	.word	$367F
	.word	$3979
	.word	$3C75
	.word	$3F74
	.word	$4276
	.word	$457B
	.word	$4882
	.word	$4B8C
	.word	$4E99
	.word	$51A9
	.word	$54BC
	.word	$57D1
	.word	$5AEA
	.word	$5E05
	.word	$6123
	.word	$6444
	.word	$6768
	.word	$6A8F
	.word	$6DB8
	.word	$70E5
	.word	$7415
	.word	$7747
	.word	$7A7D
	.word	$7DB5
	.word	$80F1
	.word	$842F
	.word	$8771
	.word	$8AB5
	.word	$8DFD
	.word	$9147
	.word	$9495
	.word	$97E5
	.word	$9B39
	.word	$9E90
	.word	$A1EA
	.word	$A547
	.word	$A8A7
	.word	$AC0A
	.word	$AF70
	.word	$B2DA
	.word	$B646
	.word	$B9B6
	.word	$BD29
	.word	$C09F
	.word	$C419
	.word	$C795
	.word	$CB15
	.word	$CE98
	.word	$D21F
	.word	$D5A8
	.word	$D935
	.word	$DCC5
	.word	$E059
	.word	$E3F0
	.word	$E78A
	.word	$EB27
	.word	$EEC8
	.word	$F26C
	.word	$F614
	.word	$F9BF
	.word	$FD6D
	.word	$011F
	.word	$04D4
	.word	$088C
	.word	$0C48
	.word	$1008
	.word	$13CB
	.word	$1791
	.word	$1B5B
	.word	$1F28
	.word	$22F9
	.word	$26CE
	.word	$2AA6
	.word	$2E81
	.word	$3260
	.word	$3643
	.word	$3A29
	.word	$3E13
	.word	$4201
	.word	$45F2
	.word	$49E7
	.word	$4DE0
	.word	$51DC
	.word	$55DC
	.word	$59DF
	.word	$5DE6
	.word	$61F1
	.word	$6600
	.word	$6A13
	.word	$6E29
	.word	$7243
	.word	$7661
	.word	$7A83
	.word	$7EA8
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
	.addr	_nsd_OPLL_frequency	;変数に書くだけは書いていい。
	.addr	_nsd_OPLL_frequency	;
	.addr	_nsd_OPLL_frequency	;
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
	bne	Exit

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
	ora	__apu_voice_set4
	
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

.proc	_nsd_nes_se2_frequency
;---------------------------------------

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
	ora	__se_voice_set2
	
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
;	and	#$FE			; x =  ax  /  192
	lsr	a
	bcc	@L
	ora	#$80
@L:	asl	a
	tay				; y = (ax mod 192) & 0xFE

	;-------------------------------
	; *** Get frequency from table
	; nsd_work_zp._tmp <- frequency
	lda	Freq_FDS + 1,y
	sta	__tmp + 1
	lda	Freq_FDS,y

	bcc	Octave_Proc

	;線形補完
	add	Freq_FDS + 2,y
	sta	__tmp
	lda	__tmp + 1
	adc	Freq_FDS + 3,y
	lsr	a
	sta	__tmp + 1
	lda	__tmp
	ror	a

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
;	and	#$FE			; x =  ax  /  192
	lsr	a
	bcc	@L
	ora	#$80
@L:	asl	a
	tay				; y = (ax mod 192) & 0xFE

	;-------------------------------
	; *** Get frequency from table
	lda	Freq_SAW + 1,y
	sta	__tmp + 1
	lda	Freq_SAW,y

	bcc	Octave_Proc

	;線形補完
	add	Freq_SAW + 2,y
	sta	__tmp
	lda	__tmp + 1
	adc	Freq_SAW + 3,y
	lsr	a
	sta	__tmp + 1
	lda	__tmp
	ror	a

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
	cmp	#$6C
	rol	__tmp + 1		;__tmp + 1 = (Octave << 1) + Note_MSB

	shr	a, 1			;
	tay				;y = (ax mod 192) >> 1
	lda	Freq_VRC7,y		;
	adc	#0			;補完

	;チャンネルの計算
	ldx	__channel
	sta	__vrc7_frequency - nsd::TR_VRC7 + 0,x
	lda	__tmp + 1
	sta	__vrc7_frequency - nsd::TR_VRC7 + 1,x

	rts
.endproc
.endif

;---------------------------------------
.ifdef	OPLL
.proc	_nsd_OPLL_frequency

	;除算
	jsr	_nsd_div192		;Wait変わりに使える？
	stx	__tmp + 1
	cmp	#$6D
	rol	__tmp + 1		;__tmp + 1 = (Octave << 1) + Note_MSB

	shr	a, 1			;
	tay				;y = (ax mod 192) >> 1
	lda	Freq_VRC7,y		;
	adc	#0			;補完

	;チャンネルの計算
	ldx	__channel
	sta	__opll_frequency - nsd::TR_OPLL + 0,x
	lda	__tmp + 1
	sta	__opll_frequency - nsd::TR_OPLL + 1,x

	rts
.endproc

.proc	_nsd_OPLL_frequency_BD

	;除算
	jsr	_nsd_div192		;Wait変わりに使える？
	stx	__tmp + 1
	cmp	#$6D
	rol	__tmp + 1		;__tmp + 1 = (Octave << 1) + Note_MSB

	shr	a, 1			;
	tay				;y = (ax mod 192) >> 1
	lda	Freq_VRC7,y		;
	adc	#0			;補完
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
	cmp	#$6D
	rol	__tmp + 1		;__tmp + 1 = (Octave << 1) + Note_MSB

	shr	a, 1			;
	tay				;y = (ax mod 192) >> 1
	lda	Freq_VRC7,y		;
	adc	#0			;補完
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
	cmp	#$6D
	rol	__tmp + 1		;__tmp + 1 = (Octave << 1) + Note_MSB

	shr	a, 1			;
	tay				;y = (ax mod 192) >> 1
	lda	Freq_VRC7,y		;
	adc	#0			;補完
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
	ldy	#N163_Frequency_Low - (8 * 0)
	bne	_nsd_n163_frequency
.endproc

.proc	_nsd_n163_ch2_frequency
	ldy	__n163_num
	cpy	#$10
	bcc	_nsd_n163_frequency_Exit	;1未満だったら終了

	jsr	N163_frequency

	ldy	#N163_Frequency_Low - (8 * 1)
	bne	_nsd_n163_frequency
.endproc

.proc	_nsd_n163_ch3_frequency
	ldy	__n163_num
	cpy	#$20
	bcc	_nsd_n163_frequency_Exit	;1未満だったら終了

	jsr	N163_frequency

	ldy	#N163_Frequency_Low - (8 * 2)
	bne	_nsd_n163_frequency
.endproc

.proc	_nsd_n163_ch4_frequency
	ldy	__n163_num
	cpy	#$30
	bcc	_nsd_n163_frequency_Exit	;1未満だったら終了

	jsr	N163_frequency

	ldy	#N163_Frequency_Low - (8 * 3)
	bne	_nsd_n163_frequency
.endproc

.proc	_nsd_n163_ch5_frequency
	ldy	__n163_num
	cpy	#$40
	bcc	_nsd_n163_frequency_Exit	;1未満だったら終了

	jsr	N163_frequency

	ldy	#N163_Frequency_Low - (8 * 4)
	bne	_nsd_n163_frequency
.endproc

.proc	_nsd_n163_ch6_frequency
	ldy	__n163_num
	cpy	#$50
	bcc	_nsd_n163_frequency_Exit	;1未満だったら終了

	jsr	N163_frequency

	ldy	#N163_Frequency_Low - (8 * 5)
	bne	_nsd_n163_frequency
.endproc

.proc	_nsd_n163_ch7_frequency
	ldy	__n163_num
	cpy	#$60
	bcc	_nsd_n163_frequency_Exit	;1未満だったら終了

	jsr	N163_frequency

	ldy	#N163_Frequency_Low - (8 * 6)
	bne	_nsd_n163_frequency
.endproc

.proc	_nsd_n163_ch8_frequency
	ldy	__n163_num
	cpy	#$70
	bcc	_nsd_n163_frequency_Exit	;1未満だったら終了

	jsr	N163_frequency

	ldy	#N163_Frequency_Low - (8 * 7)
;	bne	_nsd_n163_frequency
.endproc

.proc	_nsd_n163_frequency
	sty	N163_Resister
	lda	__tmp
	sta	N163_Data

	iny
	iny
	sty	N163_Resister
	lda	__tmp + 1
	sta	N163_Data

	iny
	iny
	sty	N163_Resister

	txa
	sub	#nsd::TR_N163
	shr	a, 1
	tay

	lda	__ptr
	ora	__n163_frequency,y
	sta	N163_Data
.endproc

.proc	_nsd_n163_frequency_Exit
	rts
.endproc

.endif

;---------------------------------------
.ifdef	PSG
.proc	_nsd_psg_ch1_frequency
	jsr	Normal12_frequency
	ldy	#PSG_1_Frequency_Low
	beq	_nsd_psg_frequency	;これは、0
.endproc

.proc	_nsd_psg_ch2_frequency
	jsr	Normal12_frequency
	ldy	#PSG_2_Frequency_Low
	bne	_nsd_psg_frequency
.endproc

.proc	_nsd_psg_ch3_frequency
	jsr	Normal12_frequency
	ldy	#PSG_3_Frequency_Low
;	bne	_nsd_psg_frequency
.endproc

.proc	_nsd_psg_frequency
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
;	and	#$FE			; x =  ax  /  192
	lsr	a
	bcc	@L
	ora	#$80
@L:	asl	a
	tay				; y = (ax mod 192) & 0xFE

	;-------------------------------
	; *** Get frequency from table
	; nsd_work_zp._tmp <- frequency
	lda	Freq + 1,y
	sta	__tmp + 1
	lda	Freq,y

	bcc	Octave_Proc

	;線形補完（次の音程を足して２で割る）
	add	Freq + 2,y
	sta	__tmp
	lda	__tmp + 1
	adc	Freq + 3,y
	lsr	a
	sta	__tmp + 1
	lda	__tmp
	ror	a

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
;	and	#$FE			; x =  ax  /  192
	lsr	a
	bcc	@L
	ora	#$80
@L:	asl	a
	tay				; y = (ax mod 192) & 0xFE

	;-------------------------------
	; *** Get frequency from table
	; nsd_work_zp._tmp <- frequency
	lda	Freq + 1,y
	sta	__tmp + 1
	lda	Freq,y

	bcc	Octave_Proc

	;線形補完（次の音程を足して２で割る）
	add	Freq + 2,y
	sta	__tmp
	lda	__tmp + 1
	adc	Freq + 3,y
	lsr	a
	sta	__tmp + 1
	lda	__tmp
	ror	a

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

.if	.defined(VRC7) || .defined(OPLL)
.proc	_Wait42
				;[6]

	pha			;[4]
	pla			;[4]
	pha			;[4]
	pla			;[4]
	pha			;[4]
	pla			;[4]	24

	rts			;[6]	14+24	=38

.endproc
.endif
