#==============================================================================
#	NES Sound Driver Libraly
#		Make file
#;				A.Waranabe
#==============================================================================

########################################
#;	LOG files
########################################

LOG_FILE    =	comlog.txt		#各コマンドの標準出力先です。
ERRLOG_FILE =	errlog.txt		#各コマンドのエラー出力先です。
BLDLOG_FILE =	buildlog.txt		#make.exeの出力先です。



########################################
#;	Define of build tools & option
########################################

#---------------
#;  MML Compiler (for Music & SE data)
MC      = $(TOOL_PASS)nsc
MCFLAGS = -a -e

#---------------
#;  C Compiler
CC      = $(TOOL_PASS)cl65
CFLAGS  = -t none -I$(INC_PASS) -c -o

#---------------
#;  Assembler
AS      = $(TOOL_PASS)ca65
ASFLAGS = -t none -I$(INC_PASS) --listing --list-bytes 255

#---------------
#;  Archiver
AR      = $(TOOL_PASS)ar65
ARFLAGS = a

#---------------
#;  Linker
LINKER  = $(TOOL_PASS)ld65
LDFLAGS = -C $(CFG_FILE) -L$(LIB_PASS) -v -m $(MAP_FILE) -vm -o

#---------------
#;  File delete
RM      = del

#---------------
#;  File copy
CP      = copy



#===============================
#; 依存関係（ヘッダーファイル等を、依存関係に追加するために、定義）

%.o : %.s $(INC_FILES)			#;".inc"ファイルが更新された場合は、.sは、全部アセンブルする。
	@echo [Assemble] : $< >>$(ERRLOG_FILE)
	$(AS) $(ASFLAGS) $< 1>>$(LOG_FILE) 2>>$(ERRLOG_FILE)


%.o : %.c $(H_FILES)			#;".h"ファイルが更新された場合は、.cは、全部コンパイルする。
	@echo [Compile] : $< >>$(ERRLOG_FILE)
	$(CC)  $(CFLAGS) $*.o $< 1>>$(LOG_FILE) 2>>$(ERRLOG_FILE)


%.o : %.mml $(DMC_FILES)		#;".dmc"ファイルが更新された場合は、.mmlは、全部コンパイルする。
	@echo [Compile] : $< >>$(ERRLOG_FILE)
	$(MC) $(MCFLAGS) $< 1>>$(LOG_FILE) 2>>$(ERRLOG_FILE)
	$(AS) $(ASFLAGS) $*.s 1>>$(LOG_FILE) 2>>$(ERRLOG_FILE)
	$(RM) $*.s 1>>$(LOG_FILE) 2>>$(ERRLOG_FILE)

%.o : %.bmp
	
