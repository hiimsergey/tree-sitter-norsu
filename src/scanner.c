#include "tree_sitter/parser.h"
#include <stdio.h> // TODO FINAL REMOVE + REMOVE all printf calls

typedef enum {
	NEWLINE,
	BLANK_LINE,
	TEXT,
	H1_OPEN,
	H2_OPEN,
	H3_OPEN,
	H4_OPEN,
	H5_OPEN,
	H6_OPEN,
	LINK_OPEN,
	LINK_CLOSE
} TokenType;

static inline bool is_hspace(int32_t c) { return c == ' ' || c == '\t'; }
static inline bool is_newline(int32_t c) { return c == '\n' || c == '\r'; }

bool tree_sitter_norsu_external_scanner_scan(
	void *payload,
	TSLexer *lexer,
	const bool *valid_symbols
) {
	if (valid_symbols[NEWLINE]) {
		printf("newline valid\n");
		if (lexer->eof(lexer)) {
			lexer->result_symbol = NEWLINE;
			lexer->mark_end(lexer);
			printf("NEWLINE (%c)\n", lexer->lookahead);
			return true;
		}
		if (is_newline(lexer->lookahead)) {
			printf("is_newline\n");
			if (lexer->lookahead == '\r') lexer->advance(lexer, false);
			if (lexer->lookahead == '\n') lexer->advance(lexer, false);
			lexer->result_symbol = NEWLINE;
			lexer->mark_end(lexer);
			printf("NEWLINE (%c)\n", lexer->lookahead);
			return true;
		}
	}
	else if (lexer->eof(lexer)) return false;

	if (valid_symbols[BLANK_LINE]) {
		bool advanced = false;
		while (is_newline(lexer->lookahead)) {
			if (lexer->lookahead == '\r') lexer->advance(lexer, false);
			if (lexer->lookahead == '\n') lexer->advance(lexer, false);
			while (is_hspace(lexer->lookahead)) lexer->advance(lexer, false);
			advanced = true;
		}
		if (advanced) {
			lexer->result_symbol = BLANK_LINE;
			lexer->mark_end(lexer);
			printf("BLANK_LINE (%c)\n", lexer->lookahead);
			return true;
		}
	}

	if (valid_symbols[H1_OPEN]) {
		int count = 0;
		while (lexer->lookahead == '#' && count <= 6) {
			lexer->advance(lexer, false);
			++count;
		}
		if (count >= 1 && count <= 6 &&
			(is_hspace(lexer->lookahead) ||
				is_newline(lexer->lookahead) ||
				lexer->eof(lexer)))
		{
			while (is_hspace(lexer->lookahead)) lexer->advance(lexer, false);

			lexer->result_symbol = H1_OPEN + count - 1;
			lexer->mark_end(lexer);
			printf("H*_OPEN (%c)\n", lexer->lookahead);
			return true;
		}
	}

	if (valid_symbols[LINK_OPEN] && lexer->lookahead == '[') {
		lexer->advance(lexer, false);

		if (lexer->lookahead == '[') {
			lexer->advance(lexer, false);
			lexer->result_symbol = LINK_OPEN;
			lexer->mark_end(lexer);
			printf("LINK_OPEN (%c)\n", lexer->lookahead);
			return true;
		}
	}
	if (valid_symbols[LINK_CLOSE] && lexer->lookahead == ']') {
		lexer->advance(lexer, false);

		if (lexer->lookahead == ']') {
			lexer->advance(lexer, false);
			lexer->result_symbol = LINK_CLOSE;
			lexer->mark_end(lexer);
			// TODO NOW DEBUG why does [[foo]] produce two LINK_CLOSE tokens
			// ^ and most importantly, why does it still work?
			printf("LINK_CLOSE (%c)\n", lexer->lookahead);
			return true;
		}
	}

	while (!lexer->eof(lexer) && !is_newline(lexer->lookahead)) {
		printf("lookahead (%c)\n", lexer->lookahead);
		lexer->advance(lexer, false);
		if (lexer->lookahead == '[' || lexer->lookahead == ']') break;
	}
	lexer->result_symbol = TEXT;
	lexer->mark_end(lexer);
	printf("TEXT (%c)\n", lexer->lookahead);
	return true;
}

void *tree_sitter_norsu_external_scanner_create() { return NULL; }
void tree_sitter_norsu_external_scanner_destroy() {}
unsigned tree_sitter_norsu_external_scanner_serialize() { return 0; }
void tree_sitter_norsu_external_scanner_deserialize() {}
