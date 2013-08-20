.PHONY: all clean

all: libcwap.so

clean:
	./setup.py clean
	rm -rf libcwap.so build

libcwap.so: libcwapmodule.c ../libcwap.c ../libcwap.h
	./setup.py install --prefix=. --install-lib=.
