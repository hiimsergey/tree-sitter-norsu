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
	// TODO NOW PLAN implement blank lines
	// they span multiple blank lines, if possible
	// invisible ofc
	// they separate multiple paragraphs

	// TODO REMOVE
	printf("\n");
	printf("starting\n");
	printf("lookahead is '%c'\n", lexer->lookahead);
	printf("newline valid: %d\n", valid_symbols[NEWLINE]);
	printf("text valid: %d\n", valid_symbols[TEXT]);
	printf("heading valid: %d\n", valid_symbols[H1_MARKER]);

	if (valid_symbols[NEWLINE]) {
		if (lexer->eof(lexer)) {
			lexer->result_symbol = NEWLINE;
			lexer->mark_end(lexer);
			return true;
		}

		// TODO CONSIDER supporting legacy mac newline encoding
		if (is_newline(lexer->lookahead)) {
			if (lexer->lookahead == '\r') lexer->advance(lexer, false);
			if (lexer->lookahead == '\n') lexer->advance(lexer, false);
			lexer->result_symbol = NEWLINE;
			lexer->mark_end(lexer);
			printf("TODO POST NEWLINE: '%c'\n", lexer->lookahead);
			return true;
		}
	}

	// Handles H*_MARKER
	if (valid_symbols[H1_MARKER] && lexer->lookahead == '#') {
		printf("HEADING AGAIN\n");
		lexer->advance(lexer, false);
		while (is_hspace(lexer->lookahead)) lexer->advance(lexer, false);

		lexer->result_symbol = H1_MARKER;
		lexer->mark_end(lexer);
		return true;
	}

	// Handles TEXT
	// TODO CONSIDER !lexer->eof(lexer)
	if (valid_symbols[TEXT] &&
		!lexer->eof(lexer) &&
		!is_newline(lexer->lookahead)) // TODO CONSIDER
	{
		while (!lexer->eof(lexer) && !is_newline(lexer->lookahead))
			lexer->advance(lexer, false);

		lexer->result_symbol = TEXT;
		lexer->mark_end(lexer);
		return true;
	}

	return false;
}

void *tree_sitter_norsu_external_scanner_create() { return NULL; }
void tree_sitter_norsu_external_scanner_destroy() {}
unsigned tree_sitter_norsu_external_scanner_serialize() { return 0; }
void tree_sitter_norsu_external_scanner_deserialize() {}
