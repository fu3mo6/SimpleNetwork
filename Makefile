TOPTARGETS := all clean

#SUBDIRS := $(wildcard */.)
SUBDIRS := src echo-server counter-server counter-client 

PLATFORM ?= LINUX

$(TOPTARGETS): $(SUBDIRS)
$(SUBDIRS):
	PLATFORM=$(PLATFORM) $(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TOPTARGETS) $(SUBDIRS)