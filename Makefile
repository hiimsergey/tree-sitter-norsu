all:
	tree-sitter generate
parse:
	tree-sitter parse test/.test
test:
	tree-sitter test
