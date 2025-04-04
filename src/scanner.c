#include "tree_sitter/parser.h"
#include "tree_sitter/alloc.h"
#include "tree_sitter/array.h"

enum TokenType {
    NEWLINE,
    TOKEN_EOF
};

void *tree_sitter_norsu_external_scanner_create() {
    Scanner
}

void tree_sitter_norsu_external_scanner_destroy(void *payload) {
    return;
}

unsigned tree_sitter_norsu_external_scanner_serialize(void *payload, char *buffer) {
    return TREE_SITTER_SERIALIZATION_BUFFER_SIZE;
}

void tree_sitter_norsu_external_scanner_deserialize(
    void *payload,
    const char *buffer,
    unsigned length
) {
    return;
}

bool tree_sitter_norsu_external_scanner_scan(
    void *payload,
    TSLexer *lexer,
    const bool *valid_symbols
) {
    return true;
}
