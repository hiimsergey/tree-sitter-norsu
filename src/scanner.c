#include "tree_sitter/parser.h"
#include <stdio.h> // TODO FINAL REMOVE

typedef enum {
	NEWLINE,
	TEXT,
	H1_MARKER
} TokenType;

// TODO CONSIDER
static inline bool is_hspace(int32_t c) { return c == ' ' || c == '\t'; }
static inline bool is_newline(int32_t c) { return c == '\n' || c == '\r'; }

bool tree_sitter_norsu_external_scanner_scan(
	void *payload,
	TSLexer *lexer,
	const bool *valid_symbols
) {
	// TODO REMOVE
	printf("\n");
	printf("starting\n");
	printf("lookahead is %c\n", lexer->lookahead);
	printf("newline valid: %d\n", valid_symbols[NEWLINE]);
	printf("text valid: %d\n", valid_symbols[TEXT]);
	printf("heading valid: %d\n", valid_symbols[H1_MARKER]);

	if (valid_symbols[NEWLINE]) {
		if (lexer->eof(lexer)) {
			lexer->result_symbol = NEWLINE;
			lexer->mark_end(lexer);
			printf("settling for NEWLINE (eof flavor)\n");
			return true;
		}

		switch (lexer->lookahead) {
			case '\r':
				lexer->advance(lexer, false);
				if (lexer->lookahead != '\n') break;
			case '\n':
				lexer->advance(lexer, false);
			default:
				goto out_newline;
		}
		lexer->result_symbol = NEWLINE;
		lexer->mark_end(lexer);
		printf("settling for NEWLINE\n");
		return true;
	}
out_newline:

	// Handles H*_MARKER
	if (lexer->lookahead == '#' && valid_symbols[H1_MARKER]) {
		lexer->result_symbol = H1_MARKER;
		lexer->mark_end(lexer);
		printf("settling for H1_MARKER\n");
		return true;
	}

	// Handles TEXT
	// TODO CONSIDER !lexer->eof(lexer)
	if (valid_symbols[TEXT] && !lexer->eof(lexer)) {
		lexer->result_symbol = TEXT;
		lexer->mark_end(lexer);
		printf("settling for TEXT\n");
		return true;
	}

	// TODO FINAL REMOVE
	printf("\n");
	printf("oops (lookahead is %c)\n", lexer->lookahead);
	printf("newline valid: %d\n", valid_symbols[NEWLINE]);
	printf("text valid: %d\n", valid_symbols[TEXT]);
	printf("heading valid: %d\n", valid_symbols[H1_MARKER]);
	printf("\n");
	return false;
}

void *tree_sitter_norsu_external_scanner_create() { return NULL; }
void tree_sitter_norsu_external_scanner_destroy() {}
unsigned tree_sitter_norsu_external_scanner_serialize() { return 0; }
void tree_sitter_norsu_external_scanner_deserialize() {}

// TODO NOTE REMOVE
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
