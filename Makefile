include ../Makefiles/git1.mk

.PHONY: commit notes readme all

all:
	@echo "There is nothing to build in this directory."

clean:
	@echo "\"make clean\" doesn't do anything here."

spell:
	aspell --lang=en check $(documentation)

include ../Makefiles/git2.mk

notes:
	(cd ../notes && make notes)

