all:
	tree-sitter generate
c cat:
	cat -n test/.test
p parse:
	tree-sitter parse test/.test
t test:
	tree-sitter test
i install:
	tree-sitter generate
	tree-sitter build
	mv norsu.so ~/.local/share/nvim/site/parser/norsu.so
	cp queries/norsu/highlights.scm ~/.local/share/nvim/site/queries/norsu/highlights.scm
