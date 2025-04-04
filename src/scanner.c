#include "tree_sitter/parser.h"
#include "tree_sitter/alloc.h"

enum TokenType {
    NEWLINE,
    BLOCK_CLOSE,
    BLOCK_CONTINUE,
    QUOTE_MARKER,
    H1_MARKER,
    H2_MARKER,
    H3_MARKER,
    H4_MARKER,
    H5_MARKER,
    H6_MARKER,
    THEMATIC_BREAK,
    LIST_MARKER,
    LIST_MARKER_PARENTHESIS,
    LIST_MARKER_DOT,
    CODE_BLOCK_START,
    CODE_BLOCK_END,
    METADATA_START,
    METADATA_END,
    TABLE_START,
    TABLE_NEWLINE
};

typedef enum {
    QUOTE,
    CODE_BLOCK,
    LIST_ITEM,
    LIST_ITEM_INDENTED_1,
    LIST_ITEM_INDENTED_2,
    LIST_ITEM_INDENTED_3,
    LIST_ITEM_INDENTED_4,
    LIST_ITEM_INDENTED_5,
    LIST_ITEM_INDENTED_6,
    LIST_ITEM_INDENTED_7,
    LIST_ITEM_INDENTED_8
} Block;

typedef struct {
    struct {
        size_t size;
        size_t capacity;
        Block *items;
    } open_blocks;

    uint8_t state;
    uint8_t matched;
    uint8_t indentation;
    uint8_t column;
    
    bool simulate;
} Scanner;

bool scan(
    void *payload,
    TSLexer *lexer,
    const bool *valid_symbols
) {
    // TODO
    return true;
}

unsigned serialize(void *payload, char *buffer) {
    // TODO
    return 5;
}

void deserialize(
    void *payload,
    const char *buffer,
    unsigned length
) {
    // TODO
}

void *tree_sitter_norsu_external_scanner_create(void) {
    Scanner *s = ts_malloc(sizeof(Scanner));
    s->open_blocks.items = ts_calloc(1, sizeof(Block));
    return s;
}

bool tree_sitter_norsu_external_scanner_scan(
    void *payload,
    TSLexer *lexer,
    const bool *valid_symbols
) {
    Scanner *s = payload;
    s->simulate = false;
    return scan(s, lexer, valid_symbols);
}

unsigned tree_sitter_norsu_external_scanner_serialize(void *payload, char *buffer) {
    Scanner *s = payload;
    return serialize(s, buffer);
}

void tree_sitter_norsu_external_scanner_deserialize(
    void *payload,
    const char *buffer,
    unsigned length
) {
    Scanner *s = payload;
    deserialize(s, buffer, length);
}

void tree_sitter_norsu_external_scanner_destroy(void *payload) {
    Scanner *s = payload;
    ts_free(s->open_blocks.items);
    ts_free(s);
}
