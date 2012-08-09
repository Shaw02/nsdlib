
	.setcpu		"6502"

	.export		nsd_sequence

	.import		_nsd_snd_keyon
	.import		_nsd_snd_keyoff
	.import		_nsd_snd_sweep
	.import		nsd_work
	.importzp	nsd_work_zp

	.include	"nes.inc"
	.include	"nsd.inc"

;=======================================================================
;		nsd_keyon
;-----------------------------------------------------------------------
;<<Contents>>
;	Key On
;<<Input>>
;	nothing
;<<Output>>
;	nothing
;=======================================================================
.proc	nsd_keyon

	;tai check
	lda	__tai,x
	and	#$02
	bne	exit

	;Hardware key on
	jsr	_nsd_snd_keyon

	;Software envelop Key on
	lda	__chflag,x
	and	#~nsd_chflag::KeyOff
	ora	#nsd_chflag::KeyOff
	sta	__chflag,x

	sta	__Envelop_F,x
	sta	__env_freq_ptr,x
	sta	__env_note_ptr,x

	cpx	#nsd::TR_BGM3
	beq	exit
	cpx	#nsd::TR_BGM5
	beq	exit
	sta	__Envelop_V,x
	sta	__env_voi_ptr,x
	sta	__env_vol_ptr,x

exit:	rts
.endproc

;=======================================================================
;		nsd_keyoff
;-----------------------------------------------------------------------
;<<Contents>>
;	Key Off
;<<Input>>
;	nothing
;<<Output>>
;	nothing
;=======================================================================
.proc	nsd_keyoff

	; to do	tai check
	lda	__tai,x
	and	#$01
	bne	exit

	;Hardware key off
	jsr	_nsd_snd_keyoff

	;Software key Off
	lda	__chflag,x
	and	#~nsd_chflag::KeyOff
	ora	__gatemode,x
	sta	__chflag,x

	lda	#$00
	tay
	sta	__por_target,x		;Por off

	;Frequency Envelop keyoff
	lda	__env_frequency,x
	sta	__ptr
	lda	__env_frequency + 1,x
	sta	__ptr + 1
	lda	(__ptr),y
	beq	Freq_End
	sta	__env_freq_ptr,x
	lda	__Envelop_F,x
	and	#$F0
	sta	__Envelop_F,x
Freq_End:

	;Note Envelop keyoff
	lda	__env_note,x
	sta	__ptr
	lda	__env_note + 1,x
	sta	__ptr + 1
	lda	(__ptr),y
	beq	Note_End
	sta	__env_note_ptr,x
	lda	__Envelop_F,x
	and	#$0F
	sta	__Envelop_F,x
Note_End:

	cpx	#nsd::TR_BGM3
	beq	exit
	cpx	#nsd::TR_BGM5
	beq	exit

	;Voice Envelop keyoff
	lda	__env_voice,x
	sta	__ptr
	lda	__env_voice + 1,x
	sta	__ptr + 1
	lda	(__ptr),y
	beq	Voice_End
	sta	__env_voi_ptr,x
	lda	__Envelop_V,x
	and	#$0F
	sta	__Envelop_V,x
Voice_End:

	;Volume Envelop keyoff
	lda	__env_volume,x
	sta	__ptr
	lda	__env_volume + 1,x
	sta	__ptr + 1
	lda	(__ptr),y
	beq	Volume_End
	sta	__env_vol_ptr,x
	lda	__Envelop_V,x
	and	#$F0
	sta	__Envelop_V,x
Volume_End:

exit:	rts
.endproc

;=======================================================================
;		nsd_load_sequence;
;-----------------------------------------------------------------------
;<<Contents>>
;	read sequence data
;<<Input>>
;	x	Channel * 2
;	nds_work.Sequence_ptr
;<<Output>>
;	a	sequence datq
;=======================================================================
.proc	nsd_load_sequence
	lda	(__Sequence_ptr,x)
	inc	__Sequence_ptr,x
	bne	exit
	inc	__Sequence_ptr + 1,x
exit:	rts
.endproc

;=======================================================================
;	void	nsd_sequence(void);
;-----------------------------------------------------------------------
;<<Contents>>
;	Play sequence and control gate time
;<<Input>>
;	x	Channel * 2
;<<Output>>
;	nothing
;=======================================================================
.proc	nsd_sequence

.rodata

length:	.byte	96
	.byte	72
	.byte	48
	.byte	36
	.byte	32
	.byte	24
	.byte	18
	.byte	16
	.byte	12
	.byte	9
	.byte	8
	.byte	6
	.byte	4
	.byte	3
	.byte	2
	.byte	1

opaddr:	.addr	nsd_op00
	.addr	nsd_op01
	.addr	nsd_op02
	.addr	nsd_op03
	.addr	nsd_op04
	.addr	nsd_op05
	.addr	nsd_op06
	.addr	nsd_op07
	.addr	nsd_op08
	.addr	nsd_op09
	.addr	nsd_op0A
	.addr	nsd_op0B
	.addr	nsd_op0C
	.addr	nsd_op0D
	.addr	nsd_op0E
	.addr	nsd_op0F
	.addr	nsd_op10
	.addr	nsd_op11
	.addr	nsd_op12
	.addr	nsd_op13
	.addr	nsd_op14
	.addr	nsd_op15
	.addr	nsd_op16
	.addr	nsd_op17
	.addr	nsd_op18
	.addr	nsd_op19
	.addr	nsd_op1A
	.addr	nsd_op1B
	.addr	nsd_op1C
	.addr	nsd_op1D
	.addr	nsd_op1E
	.addr	nsd_op1F
	.addr	nsd_op20
	.addr	nsd_op21
	.addr	nsd_op22
	.addr	nsd_op23
	.addr	nsd_op24
	.addr	nsd_op25
	.addr	nsd_op26
	.addr	nsd_op27
	.addr	nsd_op28
	.addr	nsd_op29
	.addr	nsd_op2A
	.addr	nsd_op2B
	.addr	nsd_op2C
	.addr	nsd_op2D
	.addr	nsd_op2E
	.addr	nsd_op2F
	.addr	nsd_op30
	.addr	nsd_op31
	.addr	nsd_op32
	.addr	nsd_op33
	.addr	nsd_op34
	.addr	nsd_op35
	.addr	nsd_op36
	.addr	nsd_op37
	.addr	nsd_op38
	.addr	nsd_op39
	.addr	nsd_op3A
	.addr	nsd_op3B
	.addr	nsd_op3C
	.addr	nsd_op3D
	.addr	nsd_op3E
	.addr	nsd_op3F

.code

	;-------------------------------
	;now play?	(check: Sequence_ptr == 0 ?)	** upper 1 byte only **
	lda	__Sequence_ptr + 1,x
	beq	exit

	;channel <- x
	stx	__channel

	;length--
	dec	__Length_ctr,x

	;-------------------------------
	;Gatetime control	(length > 0)

	;length == gatetime q ?
	lda	__Length_ctr,x
	cmp	__Gate,x
	bne	GateTime_Exit
	jsr	nsd_keyoff
GateTime_Exit:
	lda	__Length_ctr,x
	beq	Sequence
exit:
	rts

	;-------------------------------
	;Sequence		(length == 0)
Sequence:
	jsr	nsd_load_sequence

	cmp	#$80
	bcs	Note			;a >= 80 ?
	;-----------------------
	;op-code = 0x00 - 0x7F
Control:
	cmp	#$40
	bcs	Short_Control
	;---------------
	;op-code = 0x00 - 0x3F
	asl
	tay				;x <- a * 2
	lda	opaddr,y
	sta	__ptr
	lda	opaddr + 1,y
	sta	__ptr + 1
	jmp	(__ptr)

	;---------------
	;op-code = 0x40 - 0x7F
Short_Control:

	;---------------
	;op-code = 0x40 - 0x7F
	;---------------
	;0x40 - 0x4F
op40:	cmp	#$50
	bcs	op50
	and	#$0F
	;Set default length
	tay
	lda	length,y
	sta	__length,x
	jmp	Sequence
	;---------------
	;0x50 - 0x5F
op50:	cmp	#$60
	bcs	op60
	and	#$0F
	;Set gate time (1)
	sta	__gate_q,x
	jmp	Sequence
	;---------------
	;0x60 - 0x6F
op60:	cmp	#$70
	bcs	op70
	and	#$0F
	;Set volume
	sta	__tmp
	lda	__volume,x
	and	#$F0
	ora	__tmp
	sta	__volume,x
	jmp	Sequence
	;---------------
	;0x70 - 0x7F
op70:	;Ser release volume
	shl	a, 4
	sta	__tmp
	lda	__volume,x
	and	#$0F
	ora	__tmp
	sta	__volume,x
	jmp	Sequence
	;-----------------------
	;op-code = 0x80 - 0xFF
Note:
	tay	;save a to y

	;-------
	;bit 4 check (Slur)
Chk_Slur:
	clc
	and	#$10
	beq	@L
	sec
@L:	rol	__tai,x

	;-------
	;bit 5 check (Length )
Chk_Length:
	tya
	and	#$20
	bne	@L
	lda	__length,x
	jmp	@E
@L:	jsr	nsd_load_sequence
@E:	sta	__Length_ctr,x

	;-------
	;bit 6 check (Gate Time)
Chk_GateTime:
	tya
	and	#$40
	beq	@L
	jsr	nsd_load_sequence
	sta	__tmp
	lda	__Length_ctr,x
	sub	__tmp			; a = __Length_ctr - __Gate;
	bcs	@gateset		; if(a < 0){
	lda	#$0			;    a = 0x0; //no gate
	jmp	@gateset		; }
@L:
	lda	__gate_u,x		;if (__gate_u,x == 0) then @q
	beq	@q			;
	lda	__Length_ctr,x
	sub	__gate_u,x		; a = __Length_ctr - __Gate
	bcc	@q			; if (a < 0) then @q
	cmp	__gate_q,x		;
	bcs	@gateset		; if( a < __gate_q){
@q:	lda	__gate_q,x		;    a = __gate_q;
@gateset:				; }
	sta	__Gate,x

Calc_Note_Number:
	tya
	and	#$0F
	cmp	#12
	bcs	@Rest
	add	__octave,x
	sta	__note,x
	jmp	nsd_keyon
@Rest:
	and	#$0F
	sub	#$0C
	sta	__tmp
	lda	__chflag,x
	and	#~nsd_chflag::KeyOff
	ora	__tmp
	sta	__chflag,x
	rts

;=======================================================================
;		opcode	0x00:	End of Track / End of Subroutine
;-----------------------------------------------------------------------
nsd_op00:
	lda	__subroutine + 1,x
	sta	__Sequence_ptr + 1,x
	beq	@L	;if (a == 0)
	lda	__subroutine,x
	sta	__Sequence_ptr,x	;–ß‚é‚Æ‚«‚Í‘ã“ü
	lda	#0
	sta	__subroutine,x
	sta	__subroutine + 1,x
	jmp	Sequence
@L:
	sta	__Sequence_ptr,x

	lda	__chflag,x
	and	#~nsd_chflag::KeyOff
	sta	__chflag,x

	;
	;¡¡¡	to do	when effect
	;
	rts

;=======================================================================
;		opcode	0x02:	Sub-routine call
;-----------------------------------------------------------------------
nsd_op02:
	lda	__Sequence_ptr,x
	sta	__subroutine,x
	lda	__Sequence_ptr + 1,x
	sta	__subroutine + 1,x

;=======================================================================
;		opcode	0x01:	Jump (End of Track with loop) 
;-----------------------------------------------------------------------
nsd_op01:
Jump:
	ldy	__Sequence_ptr,x
	sty	__ptr			;
	ldy	__Sequence_ptr + 1,x
	sty	__ptr + 1		;__ptr = __Sequence_ptr

	jsr	nsd_load_sequence
	sta	__tmp
	jsr	nsd_load_sequence
	sta	__tmp + 1		;__tmp = value

	lda	__ptr
	add	__tmp
	sta	__Sequence_ptr,x
	lda	__ptr + 1
	adc	__tmp + 1
	sta	__Sequence_ptr + 1,x	;__Sequence_ptr = __ptr + __tmp

	jmp	Sequence

;=======================================================================
;		opcode	0x03:	Repeat start.
;-----------------------------------------------------------------------
nsd_op03:
	jsr	nsd_load_sequence
	sta	__repeat_ctr,x
	jmp	Sequence

;=======================================================================
;		opcode	0x04:	Repeat break.
;-----------------------------------------------------------------------
nsd_op04:
	lda	__repeat_ctr,x
	cmp	#1
	beq	Jump
	jsr	nsd_load_sequence
	jsr	nsd_load_sequence
	jmp	Sequence

;=======================================================================
;		opcode	0x05:	Repeat end.
;-----------------------------------------------------------------------
nsd_op05:
	dec	__repeat_ctr,x
	beq	Jump
	jsr	nsd_load_sequence
	jsr	nsd_load_sequence
nsd_op06:
	jmp	Sequence

;=======================================================================
;		opcode	0x07:	Write data to memory.
;-----------------------------------------------------------------------
nsd_op07:
	jsr	nsd_load_sequence
	sta	__ptr
	jsr	nsd_load_sequence
	sta	__ptr + 1
	jsr	nsd_load_sequence
	ldy	#0
	sta	(__ptr),y
	jmp	Sequence

;=======================================================================
;		opcode	0x08:	Tempo [BPM]
;-----------------------------------------------------------------------
nsd_op08:
	lda	__chflag,x
	and	#nsd_chflag::SE1 + nsd_chflag::SE2
	bne	@se
	jsr	nsd_load_sequence
	sta	__Tempo
	jmp	Sequence
@se:
	jsr	nsd_load_sequence
	jmp	Sequence
;=======================================================================
;		opcode	0x09:	Note length of È—ªŽž [Ticks] 
;-----------------------------------------------------------------------
nsd_op09:
	jsr	nsd_load_sequence
	sta	__length,x
	jmp	Sequence

;=======================================================================
;		opcode	0x0A:	Gate time (key-off timing from last) 
;-----------------------------------------------------------------------
nsd_op0A:
	jsr	nsd_load_sequence
	sta	__gate_q,x
	jmp	Sequence

;=======================================================================
;		opcode	0x0B:	Gate time (length of ”­‰¹) 
;-----------------------------------------------------------------------
nsd_op0B:
	jsr	nsd_load_sequence
	sta	__gate_u,x
nsd_op0C:
	jmp	Sequence

;=======================================================================
;		opcode	0x0D:	gate mode	(vol = 0)
;-----------------------------------------------------------------------
nsd_op0D:
	lda	#$00
	sta	__gatemode,x
	jmp	Sequence
;=======================================================================
;		opcode	0x0E:	gate mode
;-----------------------------------------------------------------------
nsd_op0E:
	lda	#$01
	sta	__gatemode,x
	jmp	Sequence
;=======================================================================
;		opcode	0x0F:	gate mode	(release)
;-----------------------------------------------------------------------
nsd_op0F:
	lda	#$02
	sta	__gatemode,x
	jmp	Sequence

;=======================================================================
;		opcode	0x10:	Voice envelop. 
;-----------------------------------------------------------------------
nsd_op10:
	jsr	nsd_load_sequence
	sta	__env_voice,x
	jsr	nsd_load_sequence
	sta	__env_voice + 1,x
	jmp	Sequence

;=======================================================================
;		opcode	0x11:	Volume envelop.
;-----------------------------------------------------------------------
nsd_op11:
	jsr	nsd_load_sequence
	sta	__env_volume,x
	jsr	nsd_load_sequence
	sta	__env_volume + 1,x
	jmp	Sequence

;=======================================================================
;		opcode	0x12:	Frequency envelop. 
;-----------------------------------------------------------------------
nsd_op12:
	jsr	nsd_load_sequence
	sta	__env_frequency,x
	jsr	nsd_load_sequence
	sta	__env_frequency + 1,x
	jmp	Sequence

;=======================================================================
;		opcode	0x13:	Note envelop. 
;-----------------------------------------------------------------------
nsd_op13:
	jsr	nsd_load_sequence
	sta	__env_note,x
	jsr	nsd_load_sequence
	sta	__env_note + 1,x
	jmp	Sequence

;=======================================================================
;		opcode	0x14:	Detune (16 = 100 cent)
;-----------------------------------------------------------------------
nsd_op14:
	jsr	nsd_load_sequence
	sta	__detune_cent,x
	jmp	Sequence

;=======================================================================
;		opcode	0x15:	Detune (Resister base) 
;-----------------------------------------------------------------------
nsd_op15:
	jsr	nsd_load_sequence
	sta	__detune_fine,x
	jmp	Sequence

;=======================================================================
;		opcode	0x16:	Sweep (Resister base) 
;-----------------------------------------------------------------------
nsd_op16:
	jsr	nsd_load_sequence
	jsr	_nsd_snd_sweep
	jmp	Sequence

;=======================================================================
;		opcode	0x17:	Portamento (Frequency += n2, every n3 [VBlank]) 
;-----------------------------------------------------------------------
nsd_op17:
	jsr	nsd_load_sequence
	sta	__por_target,x
	jsr	nsd_load_sequence
	sta	__por_inc,x
	jsr	nsd_load_sequence
	sta	__por_rate,x
	sta	__por_ctr,x
	lda	#0
	sta	__por_lv,x
nsd_op18:
nsd_op19:
nsd_op1A:
nsd_op1B:
nsd_op1C:
nsd_op1D:
nsd_op1E:
nsd_op1F:
	jmp	Sequence

;=======================================================================
;		opcode	0x20:	Volume down (-1) 
;-----------------------------------------------------------------------
nsd_op20:
	lda	__volume,x
	and	#$0F
	beq	@L
	dec	__volume,x
@L:
	jmp	Sequence
	
;=======================================================================
;		opcode	0x21:	Volume up (+1) 
;-----------------------------------------------------------------------
nsd_op21:
	lda	__volume,x
	and	#$0F
	cmp	#$0F
	beq	@L
	inc	__volume,x
@L:
nsd_op22:
nsd_op23:
nsd_op24:
nsd_op25:
nsd_op26:
nsd_op27:
	jmp	Sequence

;=======================================================================
;		opcode	0x2A:	Transpose (an absolute value) 
;-----------------------------------------------------------------------
nsd_op2A:
	jsr	nsd_load_sequence
nsd_Set_Trans:
	sta	__trans,x
	jmp	Sequence

;=======================================================================
;		opcode	0x2B:	Transpose (a relative value) 
;-----------------------------------------------------------------------
nsd_op2B:
	jsr	nsd_load_sequence
	add	__trans,x
	jmp	nsd_Set_Trans

nsd_op2C:
nsd_op2D:
nsd_op2E:
nsd_op2F:
	jmp	Sequence

;=======================================================================
;		opcode	0x30 - 0x37:	Voice of release (after key-off) 
;-----------------------------------------------------------------------
nsd_op30:
	lda	#$00
nsd_Set_Voice:
	sta	__tmp
	lda	__voice,x
	and	#$0F
	ora	__tmp
	sta	__voice,x
	jmp	Sequence

nsd_op31:
	lda	#$10
	bne	nsd_Set_Voice

nsd_op32:
	lda	#$20
	bne	nsd_Set_Voice

nsd_op33:
	lda	#$30
	bne	nsd_Set_Voice

nsd_op34:
	lda	#$40
	bne	nsd_Set_Voice

nsd_op35:
	lda	#$50
	bne	nsd_Set_Voice

nsd_op36:
	lda	#$60
	bne	nsd_Set_Voice

nsd_op37:
	lda	#$70
	bne	nsd_Set_Voice

;=======================================================================
;		opcode	0x29:	Octave up (+1) 
;-----------------------------------------------------------------------
nsd_op29:
	lda	__octave,x
	add	#12
	cmp	#$80
	bcc	nsd_Set_Octave
	jmp	Sequence

;=======================================================================
;		opcode	0x28:	Octave down (-1) 
;-----------------------------------------------------------------------
nsd_op28:
	lda	__octave,x
	sub	#12
	bcc	nsd_op28_Exit
nsd_Set_Octave:
	sta	__octave,x
nsd_op28_Exit:
	jmp	Sequence

;=======================================================================
;		opcode	0x38 - 0x3F:	Octave
;-----------------------------------------------------------------------
nsd_op38:
	lda	#12
	bne	nsd_Set_Octave

nsd_op39:
	lda	#24
	bne	nsd_Set_Octave

nsd_op3A:
	lda	#36
	bne	nsd_Set_Octave

nsd_op3B:
	lda	#48
	bne	nsd_Set_Octave

nsd_op3C:
	lda	#60
	bne	nsd_Set_Octave

nsd_op3D:
	lda	#72
	bne	nsd_Set_Octave

nsd_op3E:
	lda	#84
	bne	nsd_Set_Octave

nsd_op3F:
	lda	#96
	bne	nsd_Set_Octave

.endproc

