reformat:
	clang-format --style=WebKit --sort-includes=false -i $(shell git ls-files src/*.{cpp,h,ino} | grep -Ev '(assets|icon)')

.PHONY: reformat
