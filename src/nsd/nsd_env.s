
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
;<<Note>>
;	DPCM track do not call this routine.
;=======================================================================
.proc	nsd_envelop

	stx	__channel

	;=======================================
	;Frequency (note)
Frequency:
	;r-��ԁH
	lda	__chflag,x
	and	#nsd_chflag::KeyOff
	beq	@V0
	;�ŏ��̋x���H
	lda	__note,x
	cmp	#$FF
	bne	@L
@V0:	lda	#0			; if (rest with vol=0){
	jmp	_nsd_snd_volume		;	volume = 0
@L:					; } else {


	;-------------------------------
	;Envelop of Note
	lda	__env_note + 1,x
	beq	@NOENV				;envelop is disable?
	sta	__ptr + 1
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
	;Por
	lda	__por_target,x
	beq	F_Env			;�|���^�����g���H

	lda	__por_depth,x
	beq	Por_S			;�^�[�Q�b�g�ɓ��B�H

	dec	__por_ctr,x		;MML�R���p�C���ł�decay�� +1 ���鎖�B
	bne	Por_S

	lda	__por_rate,x
	sta	__por_ctr,x

	ldy	#$00
	lda	__por_depth,x
	bpl	@PL
	dey
@PL:	add	__por_now + 0,x
	sta	__por_now + 0,x
	tya
	adc	__por_now + 1,x
	sta	__por_now + 1,x		;__por_now += __por_depth

	shl	a, 4
	sta	__ptr			;�� __ptr �� __tmp + 2 �Ƃ��Ďg�p���Ă���B
	lda	__por_now + 0,x
	shr	a, 4
	ora	__ptr			;a = (char)(__por_now >> 4);

	cpy	#$00
	beq	Por_PL
Por_MI:	cmp	__por_target,x		;if (a - __por_target < 0) then 
	bcc	Por_O
	bcs	Por_S
Por_PL:	cmp	__por_target,x		;if (a - __por_target > 0) then 
	bcc	Por_S
;	bcs	Por_O

Por_O:	lda	__por_target,x		;�^�[�Q�b�g�ɓ��B�������̏���
	sta	__por_now + 1,x
	shl	a, 4
	sta	__por_now,x
	lda	__por_now + 1,x
	shr	a, 4
	cpy	#00
	beq	@L
	ora	#$F0			;�Z�p�V�t�g�I�Ȃ���
@L:	sta	__por_now + 1,x
	lda	#$0
	sta	__por_rate,x
	sta	__por_depth,x
Por_S:	lda	__por_now,x		;�����Ƀ|���^�����g�l���Z
	add	__tmp
	sta	__tmp
	lda	__por_now + 1,x
	adc	__tmp + 1
	sta	__tmp + 1
Por_E:

	;-------------------------------
	;Envelop of Frequency
F_Env:	lda	__env_frequency + 1,x
	beq	@Freq_Exit
	sta	__ptr + 1
	ENV	__env_frequency, __env_freq_ptr, __env_freq_now, __Envelop_F, 0
	ldy	#$00
	cmp	#$40
	bcc	@L
	dey
	ora	#$80
@L:	add	__tmp
	sta	__tmp
	tya
	adc	__tmp+1
	sta	__tmp+1
@Freq_Exit:



	;-------------------------------
	;Detune of cent unit
Detune:	
	ldy	#$00
	lda	__detune_cent,x
	bpl	@L
	dey				; ay = __detune_cent (sign expand)
@L:	add	__tmp
	sta	__tmp
	tya
	adc	__tmp + 1		;__tmp += (signed int)__detune_cent

	;-----------------------
	;Setting device (APU)
	tax				;
	lda	__tmp			;ax <- frequency�@�i����ōςނ�ˁB�j
	jsr	_nsd_snd_frequency	;nsd_snd_frequency(ax);



	;=======================================
	;Voice & Volume
	ldx	__channel	;�v��H
	cpx	#nsd::TR_BGM3
	bne	Voice
exit:	rts

	;-------------------------------
	;Envelop of Voice
Voice:
	ldy	__env_voice + 1,x	;�������@�œK���@������
	beq	Voice_Exit		;���F�G���x���[�vOff�̏ꍇ�͔�΂�

	lda	__chflag,x		;����(gatemode==1)�ł���΂��B�ݒ��
	and	#$02			;nsd_key_on()�֐��y��
	beq	Voice_Exit		;nsd_key_off()�֐��ł��B

@Envelop:
	lda	__Envelop_V,x
	and	#$F0			;
	beq	@Done			;if(counter != 0){
	lda	__Envelop_V,x		;
	sub	#$10			;  counter--
	sta	__Envelop_V,x		;
	jmp	Voice_Exit		;} else {

@Done:	sty	__ptr + 1
	lda	__env_voice,x
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
	beq	@L3	;mode = 3 ��������A�G���x���[�v��
	cmp	#$01
	beq	@L1		;mode = 1 �ŁA�����[�X����

@L2:	;Release (Envelope)
	lda	__env_volume + 1,x
	bne	@Envelop	;mode = 2 ���A�|�C���^�L��ŁA�G���x���[�v�ցB

.ifdef	VRC7
	;VRC7�́Amode 2�̎��̓����[�X�������Ȃ��B
	cpx	#nsd::TR_VRC7
	bcc	@VRC7L
	cpx	#nsd::TR_VRC7 + 6*2
	bcs	@VRC7L
	lda	__volume,x
	and	#$0F
	jmp	@VRC7_Exit
@VRC7L:
.endif
.ifdef	OPLL
	;OPLL�́Amode 2�̎��̓����[�X�������Ȃ��B
	cpx	#nsd::TR_OPLL
	bcc	@OPLLL
	cpx	#nsd::TR_OPLL + 9*2
	bcs	@OPLLL
	lda	__volume,x
	and	#$0F
	jmp	@OPLL_Exit
@OPLLL:
.endif

@L1:	;Release (Volume)
	lda	__volume,x

.ifdef	VRC6
	cpx	#nsd::TR_VRC6 + 4	;VRC6 SAW ?
	bne	@VRC6
	and	#$F0
	shr	a, 2
	jmp	@EX
@VRC6:
.endif

.ifdef	FDS
	cpx	#nsd::TR_FDS
	bne	@FDS
	and	#$F0
	shr	a, 2
	jmp	@EX
@FDS:
.endif
	shr	a, 4
@VRC7_Exit:
@OPLL_Exit:
@EX:
	jmp	Set_Volume

@L3:	lda	__env_volume + 1,x
	bne	@Envelop

	;Envelope �������̏���
	lda	__volume,x

.ifdef	VRC6
	cpx	#nsd::TR_VRC6 + 4
	bne	@VRC6V
	shl	a, 2
@VRC6V:
.endif

.ifdef	FDS
	cpx	#nsd::TR_FDS
	bne	@FDSV
	shl	a, 2
@FDSV:
.endif

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