DIRS = sark spin1_api

GNU = 1

all: $(DIRS)
	@for d in $(DIRS); do $(MAKE) -C $$d GNU=$(GNU) || exit $$?; done

clean: $(DIRS)
	@for d in $(DIRS); do $(MAKE) -C $$d GNU=$(GNU) clean || exit $$?; done
