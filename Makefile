DIRS = sark spin1_api

GNU = 1

all: $(DIRS)
	for d in $(DIRS); do (cd $$d; "$(MAKE)" GNU=$(GNU)) || exit $$?; done

clean: $(DIRS)
	for d in $(DIRS); do (cd $$d; "$(MAKE)" GNU=$(GNU) clean) || exit $$?; done
