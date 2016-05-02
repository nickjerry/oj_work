.PHONY: run clean asm cpp

CC = g++
CFILES := halmitonian_tour/sekanina.cpp
#CFLAGS :=
CFLAGS := -Wall -Werror

TAR = tar.exe

$(TAR):$(CFILES)
	@mkdir -p obj
	@$(CC) $(CFILES) -o obj/$(TAR) $(CFLAGS)

clean:
	@rm -rf obj

run:$(TAR)
	@cat sample.txt | obj/$(TAR)

cpp:$(TAR)
	@gcc -E $(CFILES) -o obj/tar.i
	@vim obj/tar.i

asm:$(TAR)
	@mkdir -p asm
	@objdump -S obj/$(TAR) > asm/$(TAR).S
	@vim asm/$(TAR).S
