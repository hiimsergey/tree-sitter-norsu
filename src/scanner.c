#define TOKSTREAM
//#define LOG

#include <stdbool.h>
#if defined(TOKSTREAM) || defined(LOG)
#	include <stdio.h>
#endif
#include <string.h>
#include "tree_sitter/alloc.h"
#include "tree_sitter/parser.h"

#define FOREACH             \
	X(NEWLINE)              \
	X(BLANK_LINE)           \
	X(TEXT)                 \
	X(H1_OPEN)              \
	X(H2_OPEN)              \
	X(H3_OPEN)              \
	X(H4_OPEN)              \
	X(H5_OPEN)              \
	X(H6_OPEN)              \
	X(LIST_BULLET)          \
	X(LIST_INDENT)          \
	X(LIST_DEDENT)          \
	X(LIST_SPACE)           \
	X(LINK_OPEN)            \
	X(LINK_CLOSE)           \
	X(LINK_ALIAS_SEPARATOR)

typedef enum {
#define X(x) x,
FOREACH
#undef X
} TokenType;

#ifdef TOKSTREAM
const char *token_type_strings[] = {
#	define X(x) #x,
FOREACH
#	undef X
};
#endif // TOKSTREAM

#ifdef LOG
#	undef LOG
#	define LOG(x) printf(                              \
		_Generic((x),                                  \
			uint32_t: "(log) " #x ": %d\n",            \
			int:      "(log) " #x ": %d\n",            \
			bool:     "(log) " #x ": %d\n",            \
			size_t:   "(log) " #x ": %zu\n",           \
			char *:   "(log) " #x ": %s\n",            \
			default:  "(log) " #x ": (unknown type)\n" \
			), (x))
#else
#	define LOG(_) {}
#endif // LOG

typedef enum {
	INCONSISTENT = 0,
	NONE,
	GAUGING,
	SPACE = ' ',
	TAB = '\t',
} SpacingState;

typedef struct {
	struct {
		size_t level; // TODO FINAL CHECK used
		size_t width; // TODO FINAL CHECK used
		bool gauging;
	} list;
	SpacingState spacing_state;
} Context;

static inline void update_spacing_status(SpacingState *spacing_status, char rhs) {
	const SpacingState lhs = *spacing_status;
	*spacing_status =
		(lhs == rhs) * rhs |
		(lhs != rhs) * (lhs == NONE || lhs == GAUGING) * rhs;
}

static inline bool isoneof(int32_t c, const char *haystack) {
	while (*haystack) if (*(haystack++) == c) return true;
	return false;
}

static inline bool done(TSLexer *lexer, TokenType symbol) {
	lexer->result_symbol = symbol;
	lexer->mark_end(lexer);

#ifdef TOKSTREAM
	printf("%s (next: '%c')\n", token_type_strings[symbol], lexer->lookahead);
#endif
	return true;
}

bool tree_sitter_norsu_external_scanner_scan(
	void *payload,
	TSLexer *lexer,
	const bool *valid_symbols
) {
	Context *const pl = payload;

	if (valid_symbols[NEWLINE]) {
		if (lexer->eof(lexer)) return done(lexer, NEWLINE);
		if (isoneof(lexer->lookahead, "\n\r")) {
			if (lexer->lookahead == '\r') lexer->advance(lexer, false);
			if (lexer->lookahead == '\n') lexer->advance(lexer, false);
			return done(lexer, NEWLINE);
		}
	}

	if (valid_symbols[BLANK_LINE]) {
		LOG("BLANK_LINE valid");

		pl->spacing_state = NONE;
		while (true) {
			switch (lexer->lookahead) {
				case '\n':
				case '\r':
					pl->spacing_state = GAUGING;
					if (lexer->lookahead == '\r') lexer->advance(lexer, false);
					if (lexer->lookahead == '\n') lexer->advance(lexer, false);
					goto while_continue;
				case ' ':
				case '\t':
					update_spacing_status(&pl->spacing_state, lexer->lookahead);
					lexer->advance(lexer, false);
					goto while_continue;
				default:
					if (pl->spacing_state == GAUGING) return done(lexer, BLANK_LINE);
					goto not_blank_line;
			}
while_continue:
			{}
		}
	}
not_blank_line:

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

	if (valid_symbols[LINK_ALIAS_SEPARATOR] && lexer->lookahead == '|') {
		lexer->advance(lexer, false);
		return done(lexer, LINK_ALIAS_SEPARATOR);
	}

	if (valid_symbols[LIST_BULLET] &&
		lexer->lookahead == '-' &&
		pl->spacing_state != INCONSISTENT)
	{
		const uint32_t start_col = lexer->get_column(lexer);
		LOG(start_col);

		if (start_col == 0) {
			pl->list.gauging = true;
			pl->list.level = 1;
			lexer->advance(lexer, false);
			return done(lexer, LIST_BULLET);
		}

		if (pl->list.level == 0) goto not_list_bullet;

		if (pl->list.gauging) {
			pl->list.gauging = false;
			pl->list.width = start_col;
		}

		if (start_col % pl->list.width != 0) goto not_list_bullet;

		const uint32_t level = start_col / pl->list.width + 1;
		if (level > pl->list.level + 1) goto not_list_bullet;
		if (level == pl->list.level + 1) {
			++pl->list.level;
			return done(lexer, LIST_INDENT);
		}
		if (valid_symbols[LIST_SPACE]) return done(lexer, LIST_SPACE);
		if (level == pl->list.level) {
			lexer->advance(lexer, false);
			return done(lexer, LIST_BULLET);
		}
		--pl->list.level;
		return done(lexer, LIST_DEDENT);

not_list_bullet:
		pl->list.level = 0;
	}

	if (valid_symbols[H1_OPEN] && lexer->get_column(lexer) == 0) {
		int count = 0;
		while (lexer->lookahead == '#' && count <= 6) {
			lexer->advance(lexer, false);
			++count;
		}
		// TODO FINAL ALL CONSIDER global handling system for whitespace between
		// token and text (like with headings and lists)
		if (count >= 1 && count <= 6 &&
			(isoneof(lexer->lookahead, " \t\n\r") || lexer->eof(lexer)))
		{
			done(lexer, H1_OPEN + count - 1);
			while (isoneof(lexer->lookahead, " \t")) lexer->advance(lexer, false);
			return true;
		}
	}

	if (!lexer->eof(lexer) && !isoneof(lexer->lookahead, "\n\r")) {
		while (isoneof(lexer->lookahead, " \t")) lexer->advance(lexer, true);
		if (isoneof(lexer->lookahead, "\n\r")) return true;

		while (!lexer->eof(lexer) && !isoneof(lexer->lookahead, "\n\r")) {
			// TODO NOW DEBUG if i swap these two lines, the latest link test passes
			// but [[foo|bar|baz]] enters loop
			lexer->advance(lexer, false);
			if (isoneof(lexer->lookahead, "[]|")) break;
		}
		return done(lexer, TEXT);
	}

	return false;
}

void *tree_sitter_norsu_external_scanner_create() {
	Context *const pl = ts_calloc(1, sizeof(Context));
	return pl;
}

void tree_sitter_norsu_external_scanner_destroy(void *pl) {
	ts_free(pl);
}

unsigned tree_sitter_norsu_external_scanner_serialize(void *pl, char *buf) {
	const size_t size = sizeof(Context);
	memcpy(buf, pl, size);
	return size;
}

void tree_sitter_norsu_external_scanner_deserialize(void *pl, char *buf, unsigned len) {
	if (len == 0) return;
	memcpy(pl, buf, len);
}
