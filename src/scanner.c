#include <stdio.h>
#include "tree_sitter/parser.h"

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

static inline bool isoneof(int32_t c, const char *haystack) {
	while (*haystack) if (*(haystack++) == c) return true;
	return false;
}

static inline bool done(TSLexer *lexer, TokenType symbol) {
	lexer->result_symbol = symbol;
	lexer->mark_end(lexer);
#if 1
	switch (symbol) {
		case NEWLINE:
			printf("NEWLINE (%c)\n", lexer->lookahead);
			break;
		case BLANK_LINE:
			printf("BLANK_LINE (%c)\n", lexer->lookahead);
			break;
		case TEXT:
			printf("TEXT (%c)\n", lexer->lookahead);
			break;
		case H1_OPEN:
			printf("H1_OPEN (%c)\n", lexer->lookahead);
			break;
		case H2_OPEN:
			printf("H2_OPEN (%c)\n", lexer->lookahead);
			break;
		case H3_OPEN:
			printf("H3_OPEN (%c)\n", lexer->lookahead);
			break;
		case H4_OPEN:
			printf("H4_OPEN (%c)\n", lexer->lookahead);
			break;
		case H5_OPEN:
			printf("H5_OPEN (%c)\n", lexer->lookahead);
			break;
		case H6_OPEN:
			printf("H6_OPEN (%c)\n", lexer->lookahead);
			break;
		case LINK_OPEN:
			printf("LINK_OPEN (%c)\n", lexer->lookahead);
			break;
		case LINK_CLOSE:
			printf("LINK_CLOSE (%c)\n", lexer->lookahead);
			break;
		default:
			break;
	}
#endif
	return true;
}

bool tree_sitter_norsu_external_scanner_scan(
	void *payload,
	TSLexer *lexer,
	const bool *valid_symbols
) {
	if (valid_symbols[NEWLINE]) {
		if (lexer->eof(lexer)) return done(lexer, NEWLINE);
		if (isoneof(lexer->lookahead, "\n\r")) {
			if (lexer->lookahead == '\r') lexer->advance(lexer, false);
			if (lexer->lookahead == '\n') lexer->advance(lexer, false);
			return done(lexer, NEWLINE);
		}
	}

	if (valid_symbols[BLANK_LINE]) {
		bool advanced = false;
		while (isoneof(lexer->lookahead, "\n\r")) {
			if (lexer->lookahead == '\r') lexer->advance(lexer, false);
			if (lexer->lookahead == '\n') lexer->advance(lexer, false);
			while (isoneof(lexer->lookahead, " \t")) lexer->advance(lexer, false);
			advanced = true;
		}
		if (advanced) return done(lexer, BLANK_LINE);
	}

	if (valid_symbols[H1_OPEN]) {
		int count = 0;
		while (lexer->lookahead == '#' && count <= 6) {
			lexer->advance(lexer, false);
			++count;
		}
		if (count >= 1 && count <= 6 &&
			(isoneof(lexer->lookahead, " \t\n\r") || lexer->eof(lexer)))
		{
			while (isoneof(lexer->lookahead, " \t")) lexer->advance(lexer, false);
			return done(lexer, H1_OPEN + count - 1);
		}
	}

	if (valid_symbols[LINK_OPEN] && lexer->lookahead == '[') {
		lexer->advance(lexer, false);

		if (lexer->lookahead == '[') {
			lexer->advance(lexer, false);
			return done(lexer, LINK_OPEN);
		}
		return done(lexer, TEXT);
	}
	if (valid_symbols[LINK_CLOSE] && lexer->lookahead == ']') {
		lexer->advance(lexer, false);

		if (lexer->lookahead == ']') {
			lexer->advance(lexer, false);
			return done(lexer, LINK_CLOSE);
		}
		return done(lexer, TEXT);
	}

	if (!lexer->eof(lexer) && !isoneof(lexer->lookahead, "\n\r")) {
		while (!lexer->eof(lexer) && !isoneof(lexer->lookahead, "\n\r")) {
			lexer->advance(lexer, false);
			if (isoneof(lexer->lookahead, "[]")) break;
		}
		return done(lexer, TEXT);
	}

	return false;
}

void *tree_sitter_norsu_external_scanner_create() { return NULL; }
void tree_sitter_norsu_external_scanner_destroy() {}
unsigned tree_sitter_norsu_external_scanner_serialize() { return 0; }
void tree_sitter_norsu_external_scanner_deserialize() {}
