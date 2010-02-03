SUBDIRS = lux

BUILT_SOURCES = lux/luxconfig.h update_protobuf
CLEANFILES = lux/luxconfig.h
EXTRA_DIR = example
EXTRA_DIST = update_protobuf.sh

lux/luxconfig.h: $(top_srcdir)/config.h
	cp $(top_srcdir)/config.h $(top_srcdir)/lux/luxconfig.h

update_protobuf:
	sh update_protobuf.sh

dist-hook:
	for subdir in $(EXTRA_DIR); do \
		cp -rp $$subdir $(distdir); \
		rm -rf $(distdir)/$$subdir/.svn; \
		rm -rf $(distdir)/$$subdir/*/.svn; \
	done
