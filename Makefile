reformat:
	clang-format --style=WebKit --sort-includes=false -i $(shell git ls-files src/*.{cpp,h,ino} src/*/*.{cpp,h} | grep -Ev '(assets|icon|src/libs)')

.PHONY: reformat
