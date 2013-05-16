include depends.mk

AUTOGEN=./autogen.sh \
	--prefix=$(PREFIX) \
	--build=$(BUILD) \
	--host=$(HOST) \

SRC=../

all: dep autogen
	(cd $(SRC); make -j$(JOBS))

dep:
	(cd android; make -j$(JOBS))
	@echo "\nmake $@ for dependencies done.\n"

autogen:
	(cd $(SRC); $(AUTOGEN))

bin-only:
	(cd $(SRC); make)

clean:
	(cd $(SRC); make clean)
	(cd android; make clean)
	rm -rf tmp
	rm -f $(TARBALL)

install: all
	(cd $(SRC); make install)

tarball: $(LIBHYBRISPREFIX)/lib/libvecore.so.1.0.0
	(	rm -rf tmp &&\
		mkdir tmp &&\
		cd tmp &&\
		tar cf - $(LIBHYBRISPREFIX) | tar xf - &&\
		rm -rf $(LCLPREFIX)/include &&\
		tar cf - -C ../android system/ | tar xf - &&\
		$(STRIP) --strip-unneeded `find . -name *.so -print` \
	)
	tar -czvf $(TARBALL) -C tmp .
