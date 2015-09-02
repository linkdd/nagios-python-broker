ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libnagios-python-broker.la

libnagios_python_broker_la_SOURCES = \
	generate/nebstructs/build/*.c generate/nebstructs/build/*.h \
	src/module.c src/module.h \
	src/logging.c src/logging.h \
	src/nebmodule.c src/nebmodule.h

EXTRA_DIST = AUTHORS LICENSE README.rst

generate/nebstructs/build/%.lo: generate/nebstructs/build/%.c
	$(AM_V_CC)depbase=`echo $@ | sed 's|[^/]*$$|$(DEPDIR)/&|;s|\.lo$$||'`;\
	$(LTCOMPILE) -MT $@ -MD -MP -MF $$depbase.Tpo -c -o $@ $< &&\
	$(am__mv) $$depbase.Tpo $$depbase.Plo

generate/nebstructs/build/*.c: generate/.pystructs

generate/.pystructs:
	(cd generate && ./runner)
	touch $@

clean-local:
	rm generate/.pystructs
