all:
	@echo "There is nothing to build in this directory."

.PHONY: commit notes readme all

clean:
	@echo "\"make clean\" doesn't do anything here."

spell:
	aspell --lang=en check README.md

include common.mk

