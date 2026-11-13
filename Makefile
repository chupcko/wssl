.DEFAULT_GOAL := examples

.PHONY: source
source:
	$(MAKE) -C source
	
.PHONY: examples
examples: source
	$(MAKE) -C examples

.PHONY: debug
debug: clean
	$(MAKE) -C source debug
	$(MAKE) -C examples debug

.PHONY: test
test: examples
	$(MAKE) -C tests test

.PHONY: deb
deb: test
	./tools/make_deb

.PHONY: clean
clean: 
	$(MAKE) -C source clean
	$(MAKE) -C examples clean
	$(MAKE) -C tests clean
	rm -rf out core
