// TODO FINAL COMMENT ALL
// #include "tree_sitter/parser.h"
// 
// typedef enum {
// 	TEXT,
// 	H1_MARKER,
// } TokenType;
// 
// // TODO FINAL CONSIDER
// static inline bool is_hspace(int32_t c) {
// 	return c == ' ' || c == '\t';
// }
// static inline bool is_newline(int32_t c) {
// 	return c == '\n' || c == '\r';
// }
// 
// bool tree_sitter_norsu_external_scanner_scan(
// 	void *payload,
// 	TSLexer *lexer,
// 	const bool *valid_symbols
// ) {
// 	// Handles H*_MARKER
// 	if (lexer->lookahead == '#' && valid_symbols[H1_MARKER]) {
// 		lexer->result_symbol = H1_MARKER;
// 		while (is_hspace(lexer->lookahead)) lexer->advance(lexer, true);
// 		lexer->mark_end(lexer);
// 		return true;
// 	}
// 
// 	// Handles TEXT
// 	// TODO CONSIDER !lexer->eof(lexer)
// 	if (valid_symbols[TEXT] && !lexer->eof(lexer)) {
// 		lexer->result_symbol = TEXT;
// 		lexer->mark_end(lexer);
// 		return true;
// 	}
// 
// 	return false;
// }
// 
// void *tree_sitter_norsu_external_scanner_create() { return NULL; }
// void tree_sitter_norsu_external_scanner_destroy() {}
// unsigned tree_sitter_norsu_external_scanner_serialize() { return 0; }
// void tree_sitter_norsu_external_scanner_deserialize() {}
