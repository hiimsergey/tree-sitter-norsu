all:
	tree-sitter generate
cat:
	cat test/.test | nl -b a
parse:
	tree-sitter parse test/.test
test:
	tree-sitter test
install:
	tree-sitter build
	mv norsu.so ~/.local/share/nvim/site/parser/norsu.so
	cp queries/norsu/highlights.scm ~/.local/share/nvim/site/queries/norsu/highlights.scm
