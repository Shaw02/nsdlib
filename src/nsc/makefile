#//	gcc用のmakefileです。
#//	その他のコンパイラを使用する場合は、それに合わせて書き換えてください。

CC			= gcc
CXX			= g++
CFLAGS		= -finput-charset=cp932 -fexec-charset=cp932 -O4 -Wall -I/usr/local/include -D_UNICODE
CXXFLAGS	= -finput-charset=cp932 -fexec-charset=cp932 -O4 -Wall -I/usr/local/include -D_UNICODE
DEST		= /usr/local/bin
LDFLAGS		= -L/usr/local/lib
LIBS		= -lm
OBJS		= BGM.o DPCM.o DPCMinfo.o Envelop.o FileInput.o FileOutput.o MMLfile.o mml_Address.o mml_general.o mml_repeat.o mml_note.o mml_poke.o MusicEvent.o MusicFile.o MusicHeader.o MusicItem.o MusicTrack.o N163.o nsc.o Option.o SE.o StdAfx.o Sub.o TrackSet.o VRC7.o FDSC.o FDSM.o Patch.o SearchPass.o
PROGRAM		= nsc

all: $(PROGRAM)

$(PROGRAM):		$(OBJS)
				$(CXX) -o $(PROGRAM) $(OBJS) $(LDFLAGS) $(LIBS)

clean:;			rm -f *.o *~ $(PROGRAM)

install:		$(PROGRAM)
				install -s $(PROGRAM) $(DEST)
