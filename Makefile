.phony: $(MAKECMDGOALS) all

$(MAKECMDGOALS):
	./build $(MAKECMDGOALS)

all: build-*/tsoc.hex

build-*/tsoc.hex: *.ino *.c *.h build Make*
	./build all
