lib_LTLIBRARIES = \
	libvecore.la

libvecore_la_SOURCES = vecore.c
libvecore_la_CFLAGS = -I$(top_srcdir)/include
if WANT_TRACE
libvecore_la_CFLAGS += -DDEBUG
endif
if WANT_DEBUG
libvecore_la_CFLAGS += -ggdb -O0
endif
libvecore_la_LDFLAGS = \
	$(top_builddir)/common/libhybris-common.la \
	-version-info "1":"0":"0"
