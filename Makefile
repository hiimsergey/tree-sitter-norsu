all:
	tree-sitter generate
cat:
	cat test/.test | nl -b a
parse:
	tree-sitter parse test/.test
test:
	tree-sitter test
