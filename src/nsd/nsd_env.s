
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
;	void	nsd_sequence(void);
;-----------------------------------------------------------------------
;<<Contents>>
;		
;<<Input>>
;	x	Channel * 2
;<<Output>>
;	nothing
;=======================================================================
.proc	nsd_envelop

	stx	__channel

	;-------------------------------
	;Frequency (note)
Frequency:
	lda	__chflag,x
	and	#nsd_chflag::KeyOff
	bne	@L
	lda	#0			; if (rest with vol=0){
	jmp	_nsd_snd_volume		;	volume = 0
@L:					; } else {
	lda	__note,x
	add	__trans,x
	sta	__tmp + 1
	shl	a, 4
	sta	__tmp
	lda	__tmp + 1
	shr	a, 4
	sta	__tmp + 1		;__tmp = (int)(__note << 4)
	jmp	debug

	;-----------
	;Detune
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

	;-----------
	;Envelop Frequency
F_Env:	lda	__env_frequency + 1,x
	beq	@Freq_Exit
	sta	__ptr + 1
	lda	__env_frequency,x
	sta	__ptr
	ldy	__env_freq_ptr,x
	;
	;¡¡	to do envelop process
	;
	tya
	sta	__env_freq_ptr,x

@Freq_Exit:

	;-----------
	;Envelop Note
	lda	__env_note + 1,x
	beq	@Note_Exit
	sta	__ptr + 1
	lda	__env_note,x
	sta	__ptr
	ldy	__env_note_ptr,x
	;
	;¡¡	to do envelop process
	;
	tya
	sta	__env_note_ptr,x

@Note_Exit:

	;-----------
	;Setting device (APU)
debug:
	lda	__tmp
	ldx	__tmp + 1
	jsr	_nsd_snd_frequency	;nsd_snd_frequency(ax);
	ldx	__channel

	;-------------------------------
	;Voice & Volume
	cpx	#nsd::TR_BGM3
	beq	exit
	cpx	#nsd::TR_BGM5
	beq	exit

	;-------------------------------
	;Voice
Voice:
	lda	__chflag,x
	and	#$02
	bne	@Envelop
	lda	__voice,x
	shr	a, 4			; a = release voice
	jmp	Set_Voice
@Envelop:
	lda	__env_voice + 1,x
	bne	@L
	lda	#$02
	bne	Set_Voice
@L:	sta	__ptr + 1
	lda	__env_voice,x
	sta	__ptr
	ldy	__env_voi_ptr,x
	;
	;¡¡	to do envelop process
	;
	tya
	sta	__env_voi_ptr,x

Set_Voice:
	jsr	_nsd_snd_voice		;nsd_snd_voice(a);
Exit_Voice:

	;-------------------------------
	;Volume
Volume:
	lda	__chflag,x
	and	#$02
	bne	@Envelop
	lda	__volume,x
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
	;¡¡	to do envelop process
	;
	tya
	sta	__env_vol_ptr,x

Set_Volume:
	jsr	_nsd_snd_volume		;nsd_snd_volume(a);

exit:
	rts
.endproc

