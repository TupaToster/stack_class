CC=g++

CFLAGS+=-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wmissing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_SIDE

SRCS=lib/flog.cpp lib/text.cpp processor/proc.cpp asm/asm_funcs.cpp asm/asm.cpp deasm/deasm_funcs.cpp deasm/deasm.cpp
SRCS_CPU=processor/proc.cpp
SRCS_ASM=asm/asm_funcs.cpp asm/asm.cpp
SRCS_DEASM=deasm/deasm_funcs.cpp deasm/deasm.cpp
SRCS_LIB=lib/text.cpp lib/flog.cpp

OBJ=$(SRCS:.cpp=.o)
OBJ_CPU=$(SRCS_CPU:.cpp=.o)
OBJ_ASM=$(SRCS_ASM:.cpp=.o)
OBJ_DEASM=$(SRCS_DEASM:.cpp=.o)
OBJ_LIB=$(SRCS_LIB:.cpp=.o)

all: depend asm cpu deasm

depend: .depend

.depend: $(SRCS)
	rm -f "$@"
	$(CC) $(CFLAGS) -MM $^ -MF "$@"

include .depend

asm: $(OBJ_ASM) $(OBJ_LIB)
	$(CC) $(OBJ_ASM) $(OBJ_LIB) $(CFLAGS) -o asm.exe

cpu: $(OBJ_CPU) $(OBJ_LIB)
	$(CC) $(OBJ_ASM) $(OBJ_LIB)$(CFLAGS) -o cpu.exe

deasm: $(OBJ_DEASM) $(OBJ_LIB)
	$(CC) $(OBJ_DEASM) $(OBJ_LIB) $(CFLAGS) -o deasm.exe

*.o: *.cpp
	$(CC) $(CFLAGS) *.cpp -c -o

clean:
	rm -rf */*.o *.exe*
	clear

.PHONY: clean depend .depend run