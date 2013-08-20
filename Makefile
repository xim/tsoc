.PHONY: all cwappy clean depends upload raw_upload reset size show_boards

all: build-*/tsoc.hex cwappy

cwappy:
	@make -C cwappy

clean:
	@./build clean
	@make -C cwappy clean

depends:
	@./build depends

upload:
	@./build upload

raw_upload:
	@./build raw_upload

reset:
	@make -fMakefile_ reset

size:
	@./build size

show_boards:
	@make -fMakefile_ show_boards


build-*/tsoc.hex: *.ino *.c *.h build Make*
	@./build
