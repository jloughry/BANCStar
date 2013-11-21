documentation = README.md

EDIT = vi

#
# Note: make requires that we set the value of a variable OUTSIDE any rules.
#

timestamp = `date +%Y%m%d.%H%M`

.PHONY: commit notes readme all

all:
	@echo "There is nothing to build in this directory."

readme:
	$(EDIT) $(documentation)

spell:
	aspell --lang=en check $(documentation)

commit:
	git add .
	git commit -am "commit from Makefile `date +%Y%m%d.%H%M`"
	make sync

sync:
	git pull --rebase
	git push

notes:
	(cd ../notes && make notes)

