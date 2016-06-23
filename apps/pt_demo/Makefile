APPS = aggregator tracer
GNU := 1

all:
	for app in $(APPS); do ("$(MAKE)" -f Makefile.spinn GNU=$(GNU) APP=$$app) || exit $$?; done
	"$(MAKE)" -f Makefile.host

clean:
	for app in $(APPS); do ("$(MAKE)" -f Makefile.spinn GNU=$(GNU) APP=$$app clean) || exit $$?; done
	"$(MAKE)" -f Makefile.host clean
