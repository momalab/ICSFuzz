CC=/opt/OSELAS.Toolchain-2016.06.0/arm-v7a-linux-gnueabihf/gcc-5.4.0-glibc-2.23-binutils-2.26-kernel-4.6-sanitized/bin/arm-v7a-linux-gnueabihf-gcc

all: fuzzer.o
	$(CC) fuzzer.c mutator.c mutator.h -o fuzzer

clean:
	-rm fuzzer.o fuzzer
