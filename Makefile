.DEFAULT_GOAL := all
.PHONY: all
all:
	$(MAKE) -C source
	$(MAKE) -C examples

.PHONY: debug
debug: clean
	$(MAKE) -C source debug
	$(MAKE) -C examples debug

.PHONY: clean
clean: 
	$(MAKE) -C source clean
	$(MAKE) -C examples clean
	$(MAKE) -C test clean
	rm -rf out core

.PHONY: test
test:
	$(MAKE) -C test test

.PHONY: deb
deb: all
	./tools/make_deb
