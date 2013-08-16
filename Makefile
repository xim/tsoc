.phony: $(MAKECMDGOALS) all

$(MAKECMDGOALS):
	./build $(MAKECMDGOALS)

all: build-cli/temp.cpp

build-cli/temp.cpp: *.ino *.c *.h build Make*
	./build all
