.PHONY: run clean asm cpp submit

CC = g++
CFILES := mincut/deterministic.cpp
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
	@mkdir -p obj
	@gcc -E $(CFILES) -o obj/tar.i
	@vim obj/tar.i

asm:$(TAR)
	@mkdir -p obj
	@objdump -S obj/$(TAR) > obj/$(TAR).S
	@vim obj/$(TAR).S

submit:
	git add . -A
	git commit -m "backup point" --allow-empty
	git push
