SUBDIRS = lux

BUILT_SOURCES = lux/luxconfig.h
CLEANFILES = lux/luxconfig.h
EXTRA_DIR = example

lux/luxconfig.h: $(top_srcdir)/config.h
	cp $(top_srcdir)/config.h $(top_srcdir)/lux/luxconfig.h

dist-hook:
	for subdir in $(EXTRA_DIR); do \
		cp -rp $$subdir $(distdir); \
		rm -rf $(distdir)/$$subdir/.svn; \
		rm -rf $(distdir)/$$subdir/*/.svn; \
	done
