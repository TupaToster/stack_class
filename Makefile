CC=g++

CFLAGS+=-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wmissing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_SIDE


LIB=lib/flog.cpp lib/text.cpp
CPU=processor/proc.cpp
ASM=asm/asm_funcs.cpp asm/asm.cpp
DEASM=deasm/deasm.cpp deasm/deasm_funcs.cpp

all: asm cpu

%.o: %.cpp
	$(CC) $(CFLAGS) -MD -c $<

-include: *.d

cpu: $(CPU:.cpp=.o) $(LIB:.cpp=.o)
	$(CC) $(CFLAGS) $(CPU:processor/%.cpp=%.o) $(LIB:lib/%.cpp=%.o) -o cpu.exe

asm: $(ASM:.cpp=.o) $(LIB:.cpp=.o)
	$(CC) $(CFLAGS) $(ASM:asm/%.cpp=%.o) $(LIB:lib/%.cpp=%.o) -o asm.exe

clean:
	rm -rf */*.o *.exe* */*.d *.o *.d
	clear

.PHONY: clean
