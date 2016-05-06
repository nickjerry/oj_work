.PHONY: run clean asm cpp submit

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
	git reset --soft 838f612f9c55fec16aa42c01041ca80dd68aa587
	git push -f
	git commit -m "backup point" --allow-empty
	git push
