MAKE = bmake

release:
	cd build/release && $(MAKE)

all: release debug

debug:
	cd build/debug && $(MAKE)

install: all
	mkdir -p $(PREFIX)/include/cii
	mkdir -p $(PREFIX)/lib
	cp build/release/*.a $(PREFIX)/lib
	cp src/*.h $(PREFIX)/include/cii

clean: clean-release clean-debug

clean-release:
	cd build/release && $(MAKE) clean

clean-debug:
	cd build/debug && $(MAKE) clean
