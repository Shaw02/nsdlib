
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
.macro	ENV	Address, Pointer, Now, Counter, n

	lda	Counter,x
	.if	(n = 0)
		and	#$0F			;
		beq	@Done			;
		lda	Counter,x		;
		sub	#$01			;	
	.else
		and	#$F0			;
		beq	@Done			;if(counter != 0){
		lda	Counter,x		;
		sub	#$10			;  counter--
	.endif
	sta	Counter,x		;
	jmp	@Decay			;} else {

@Done:	lda	Address,x
	sta	__ptr			;  __ptr = (table address of envelop)
	ldy	Pointer,x		;  y     = (_envelop pointer)
@Loop:	lda	(__ptr),y		;  do{
	bpl	@Value			;  a = __ptr[y]
	cmp	#$C0			;    if( a >= 0x80 ){
	bcc	@SetDecay		;      if( a >= 0xc0){
	and	#$3F			;        y = a & 0x3F;  //continue
	tay				;      } else {
	jmp	@Loop			;        (decay counter) = a<<4 | (decay counter);

@SetDecay:				;        break;
	.if	(n = 4)
		shl	a, n		;      }
	.else
		and	#$0F
	.endif
	ora	Counter,x		;    } else { 
	sta	Counter,x		;      (now value) = a;
	jmp	@SetY			;      break;
					;    }
@Value:	sta	Now,x			;  } while(1);
@SetY:	iny				;  (_envelop pointer) = y + 1;
	tya				;}
	sta	Pointer,x		;a = (now value)
@Decay:	lda	Now,x			;
@Set:	

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
	sta	__ptr + 1
	beq	@NOENV				;envelop is disable?
	ENV	__env_note, __env_note_ptr, __env_note_now, __Envelop_F, 4
	cmp	#$40
	bcc	@Sigh
	ora	#$80
@Sigh:	add	__note,x
	jmp	@Note_Exit
@NOENV:
	lda	__note,x
@Note_Exit:

	;-------------------------------
	; __tmp = (__note,x + __trans,x) << 4;
	add	__trans,x
	sta	__tmp + 1
	shl	a, 4
	sta	__tmp
	lda	__tmp + 1
	shr	a, 4
	sta	__tmp + 1

	;-------------------------------
	;Envelop of Frequency
F_Env:	lda	__env_frequency + 1,x
	sta	__ptr + 1
	beq	@Freq_Exit
	ENV	__env_frequency, __env_freq_ptr, __env_freq_now, __Envelop_F, 0
	cmp	#$40
	bcc	@Sigh
	ora	#$80
	add	__tmp
	sta	__tmp
	lda	#$FF
	bne	@SetH
@Sigh:	add	__tmp
	sta	__tmp
	lda	#$0
@SetH:	adc	__tmp+1
	sta	__tmp+1
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
	bne	Voice
exit:	rts

	;-------------------------------
	;Envelop of Voice
Voice:
	lda	__chflag,x
	lda	__chflag,x
	and	#$02
	bne	@L

	lda	__voice,x
	shr	a, 4			; a = release voice
	jmp	Set_Voice

@L:	lda	__env_voice + 1,x
	bne	@Envelop
	lda	__env_voice,x
	jmp	Set_Voice

@Envelop:
	sta	__ptr + 1
	lda	__Envelop_V,x
	and	#$F0			;
	beq	@Done			;if(counter != 0){
	lda	__Envelop_V,x		;
	sub	#$10			;  counter--
	sta	__Envelop_V,x		;
	jmp	Voice_Exit		;} else {

@Done:	lda	__env_voice,x
	sta	__ptr			;  __ptr = (table address of envelop)
	ldy	__env_voi_ptr,x		;  y     = (_envelop pointer)
@Loop:	lda	(__ptr),y		;  do{
	bpl	@Value			;  a = __ptr[y]
	cmp	#$C0			;    if( a >= 0x80 ){
	bcc	@SetDecay		;      if( a >= 0xc0){
	and	#$3F			;        y = a & 0x3F;  //continue
	tay				;      } else {
	jmp	@Loop			;        (decay counter) = a<<4 | (decay counter);

@SetDecay:				;
	shl	a, 4			;
	ora	__Envelop_V,x		;
	sta	__Envelop_V,x		;
	iny				;
	tya				;
	sta	__env_voi_ptr,x		;
	jmp	Voice_Exit

@Value:
	pha
	iny
	tya
	sta	__env_voi_ptr,x
	pla
	;-----------------------
	;Setting device (APU)
Set_Voice:
	jsr	_nsd_snd_voice		;nsd_snd_voice(a);
Voice_Exit:

	;-------------------------------
	;Envelop of Volume
Volume:
	lda	__chflag,x
	and	#$03
	cmp	#$03
	beq	@L3	;mode = 3 だったら、エンベロープへ
	cmp	#$01
	beq	@L1		;mode = 1 で、リリース音量

@L2:	lda	__env_volume + 1,x
	bne	@Envelop	;mode = 2 且つ、ポインタ有りで、エンベロープへ。
@L1:	lda	__volume,x
	and	#$F0
	jmp	Set_Volume

@L3:	lda	__env_volume + 1,x
	bne	@Envelop
	lda	__volume,x
	shl	a, 4
	jmp	Set_Volume

@Envelop:
	sta	__ptr + 1
	ENV	__env_volume, __env_vol_ptr, __env_vol_now, __Envelop_V, 0

	sta	__tmp
	lda	__volume,x
	ldx	__tmp
	jsr	_nsd_mul
	ldx	__channel

	;-----------------------
	;Setting device (APU)
Set_Volume:
	jmp	_nsd_snd_volume		;nsd_snd_volume(a);

.endproc

