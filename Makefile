include ../Makefiles/git1.mk

all:
	@echo "There is nothing to build in this directory."

include ../Makefiles/git2.mk

.PHONY: commit notes readme all

clean:
	@echo "\"make clean\" doesn't do anything here."

spell:
	aspell --lang=en check $(documentation)

notes:
	(cd ../notes && make notes)

