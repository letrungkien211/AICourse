SUBDIRS = BPNN HMM Apps

all: subdirs

subdirs:
	for dir in $(SUBDIRS); do \
	$(MAKE) -C $$dir; \
	done;

clean:
	for dir in $(SUBDIRS); do \
	$(MAKE) clean -C $$dir; \
	done
