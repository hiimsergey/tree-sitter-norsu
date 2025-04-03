package tree_sitter_norsu_test

import (
	"testing"

	tree_sitter "github.com/tree-sitter/go-tree-sitter"
	tree_sitter_norsu "github.com/hiimsergey/tree-sitter-norsu/bindings/go"
)

func TestCanLoadGrammar(t *testing.T) {
	language := tree_sitter.NewLanguage(tree_sitter_norsu.Language())
	if language == nil {
		t.Errorf("Error loading Norsu grammar")
	}
}
