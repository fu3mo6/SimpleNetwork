TOPTARGETS := all clean

#SUBDIRS := $(wildcard */.)
SUBDIRS := src echo-server counter-server counter-client 

PLATFORM ?= LINUX

$(TOPTARGETS): $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) PLATFORM=$(PLATFORM) -C $@ $(MAKECMDGOALS)

.PHONY: $(TOPTARGETS) $(SUBDIRS)