documentation = README.md

EDIT = vi

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
	git pull --rebase
	git push

notes:
	(cd ../notes && make notes)

