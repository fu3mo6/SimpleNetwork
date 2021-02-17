TOPTARGETS := all clean

#SUBDIRS := $(wildcard */.)
SUBDIRS := src counter-client counter-server

$(TOPTARGETS): $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TOPTARGETS) $(SUBDIRS)