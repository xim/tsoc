.PHONY: all clean

all: libcwap.so

clean:
	./setup.py clean
	rm libcwap.so

libcwap.so: libcwapmodule.c ../libcwap.c ../libcwap.h
	./setup.py install --prefix=. --install-lib=.
