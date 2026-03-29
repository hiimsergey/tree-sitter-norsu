#include "tree_sitter/parser.h"

typedef enum {
	NEWLINE,
	BLANK_LINE,
	TEXT,
	H1_MARKER
} TokenType;

static inline bool is_hspace(int32_t c) { return c == ' ' || c == '\t'; }
static inline bool is_newline(int32_t c) { return c == '\n' || c == '\r'; }

bool tree_sitter_norsu_external_scanner_scan(
	void *payload,
	TSLexer *lexer,
	const bool *valid_symbols
) {
	if (valid_symbols[BLANK_LINE]) {
		bool empty = true;
		while (is_newline(lexer->lookahead)) {
			if (lexer->lookahead == '\r') lexer->advance(lexer, false);
			if (lexer->lookahead == '\n') lexer->advance(lexer, false);
			while (is_hspace(lexer->lookahead)) lexer->advance(lexer, false);
			empty = false;
		}
		if (!empty) {
			lexer->result_symbol = BLANK_LINE;
			lexer->mark_end(lexer);
			return true;
		}
	}

	if (valid_symbols[NEWLINE]) {
		if (lexer->eof(lexer)) {
			lexer->result_symbol = NEWLINE;
			lexer->mark_end(lexer);
			return true;
		}
		if (is_newline(lexer->lookahead)) {
			if (lexer->lookahead == '\r') lexer->advance(lexer, false);
			if (lexer->lookahead == '\n') lexer->advance(lexer, false);
			lexer->result_symbol = NEWLINE;
			lexer->mark_end(lexer);
			return true;
		}
	}

	if (valid_symbols[H1_MARKER] && lexer->lookahead == '#') {
		lexer->advance(lexer, false);
		while (is_hspace(lexer->lookahead)) lexer->advance(lexer, false);

		lexer->result_symbol = H1_MARKER;
		lexer->mark_end(lexer);
		return true;
	}

	if (valid_symbols[TEXT] &&
		!lexer->eof(lexer) &&
		!is_newline(lexer->lookahead))
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
