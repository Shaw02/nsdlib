
	.setcpu		"6502"

	.export		nsd_envelop

	.import		_nsd_snd_voice
	.import		_nsd_snd_volume
	.import		_nsd_snd_frequency
	.import		_nsd_mul
	.import		nsd_work
	.importzp	nsd_work_zp

	.include	"nes.inc"
	.include	"nsd.inc"

.code

;=======================================================================
;	void	nsd_envelop_proc(void);
;-----------------------------------------------------------------------
;<<Contents>>
;	Process of envelop
;<<Input>>
;	__ptr	Pointer of envelop pattern
;	y	Address
;<<Output>>
;	a	Next Address
;	__ptr	Value
;=======================================================================
.macro	ENV	Address, Counter


	lda	Counter,x
	beq	Loop

	dec	Counter,x
;	lda	現在値
	jmp	Exit


Loop:
	ldy	Address,x		;   y  = 

	lda	(__ptr),y
	bpl	COM

	iny
	jmp	Exit
COM:

	;
	;■■	to do envelop process
	;


	jmp	Exit



Exit:
	pha
	tya
	sta	Address,x
	pla
.endmacro
;=======================================================================
;	void	nsd_envelop(void);
;-----------------------------------------------------------------------
;<<Contents>>
;	Envelop & LFO main routine
;<<Input>>
;	x	Channel * 2
;<<Output>>
;	nothing
;=======================================================================
.proc	nsd_envelop

	stx	__channel

	;=======================================
	;Frequency (note)
Frequency:
	lda	__chflag,x
	and	#nsd_chflag::KeyOff
	bne	@L
	lda	#0			; if (rest with vol=0){
	jmp	_nsd_snd_volume		;	volume = 0
@L:					; } else {

	;-------------------------------
	;Envelop of Note
	lda	__env_note + 1,x
	beq	@Note_Exit
	sta	__ptr + 1
	lda	__env_note,x
	sta	__ptr
	ldy	__env_note_ptr,x
	;
	;■■	to do envelop process
	;
	sta	__env_note_ptr,x

@Note_Exit:

	lda	__note,x
	add	__trans,x
	sta	__tmp + 1
	shl	a, 4
	sta	__tmp
	lda	__tmp + 1
	shr	a, 4
	sta	__tmp + 1		;__tmp = (int)(__note << 4)

	;-------------------------------
	;Envelop of Frequency
F_Env:	lda	__env_frequency + 1,x
	beq	@Freq_Exit
	sta	__ptr + 1
	lda	__env_frequency,x
	sta	__ptr
	ldy	__env_freq_ptr,x
	;
	;■■	to do envelop process
	;
	sta	__env_freq_ptr,x

@Freq_Exit:

	;-------------------------------
	;Detune of cent unit
Detune:	lda	__detune_cent,x
	bmi	@L
	ldy	#$00
	jmp	@E
@L:	ldy	#$FF			; ay = __detune_cent (sign expand)
@E:	add	__tmp
	sta	__tmp
	tya
	adc	__tmp + 1
	sta	__tmp + 1		;__tmp += (signed int)__detune_cent

	;-----------------------
	;Setting device (APU)
	lda	__tmp
	ldx	__tmp + 1
	jsr	_nsd_snd_frequency	;nsd_snd_frequency(ax);



	;=======================================
	;Voice & Volume
	ldx	__channel
	cpx	#nsd::TR_BGM3
	beq	exit
	cpx	#nsd::TR_BGM5
	beq	exit

	;-------------------------------
	;Envelop of Voice
Voice:
	lda	__chflag,x
	lda	__chflag,x
	and	#$03
	cmp	#$03
	beq	@Envelop	;mode = 3 だったら、エンベロープへ
	cmp	#$01
	beq	@R		;mode = 1 で、リリース音色
	lda	__env_volume + 1,x
	bne	@Envelop	;mode = 2 且つ、ポインタ有りで、エンベロープへ。
@R:	lda	__voice,x
	shr	a, 4			; a = release voice
	jmp	Set_Voice
@Envelop:
	lda	__env_voice + 1,x
	bne	@L
	lda	__env_voice,x
	jmp	Set_Voice
@L:	sta	__ptr + 1
	lda	__env_voice,x
	sta	__ptr			;__ptr = pointer of envelop pattern
	ldy	__env_voi_ptr,x		;   y  = 
	;
	;■■	to do envelop process
	;
	sta	__env_voi_ptr,x

	;-----------------------
	;Setting device (APU)
Set_Voice:
	jsr	_nsd_snd_voice		;nsd_snd_voice(a);

	;-------------------------------
	;Envelop of Volume
Volume:
	lda	__chflag,x
	and	#$03
	cmp	#$03
	beq	@Envelop	;mode = 3 だったら、エンベロープへ
	cmp	#$01
	beq	@R		;mode = 1 で、リリース音量
	lda	__env_volume + 1,x
	bne	@Envelop	;mode = 2 且つ、ポインタ有りで、エンベロープへ。
@R:	lda	__volume,x
	and	#$F0
	jmp	Set_Volume
@Envelop:
	lda	__env_volume + 1,x
	bne	@L
	lda	__volume,x
	shl	a, 4
	jmp	Set_Volume
@L:	sta	__ptr + 1
	lda	__env_volume,x
	sta	__ptr
	ldy	__env_vol_ptr,x
	;
	;■■	to do envelop process
	;
	sta	__env_vol_ptr,x

	;-----------------------
	;Setting device (APU)
Set_Volume:
	jsr	_nsd_snd_volume		;nsd_snd_volume(a);



exit:
	rts
.endproc

