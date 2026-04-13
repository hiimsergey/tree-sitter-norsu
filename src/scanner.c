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
	X(LIST_UNINDENT)        \
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
#	define LOG(x) printf(                            \
		_Generic((x),                                \
			uint32_t: "LOG " #x ": %d\n",            \
			int:      "LOG " #x ": %d\n",            \
			size_t:   "LOG " #x ": %zu\n",           \
			char *:   "LOG " #x ": %s\n",            \
			default:  "LOG " #x ": (unknown type)\n" \
			), (x))
#else
#	define LOG(_) {}
#endif // LOG

typedef struct {
	struct {
		size_t level;
		size_t width;
		bool gauging;
		bool tabs;
	} list;
} Context;

static inline bool isoneof(int32_t c, const char *haystack) {
	while (*haystack) if (*(haystack++) == c) return true;
	return false;
}

static inline bool done(TSLexer *lexer, TokenType symbol) {
	lexer->result_symbol = symbol;
	lexer->mark_end(lexer);

#ifdef TOKSTREAM
	printf("%s (%c)\n", token_type_strings[symbol], lexer->lookahead);
#endif
	return true;
}

bool tree_sitter_norsu_external_scanner_scan(
	void *payload,
	TSLexer *lexer,
	const bool *valid_symbols
) {
	Context *const pl = (Context *) payload;

	if (valid_symbols[NEWLINE]) {
		if (lexer->eof(lexer)) return done(lexer, NEWLINE);
		if (isoneof(lexer->lookahead, "\n\r")) {
			if (lexer->lookahead == '\r') lexer->advance(lexer, false);
			if (lexer->lookahead == '\n') lexer->advance(lexer, false);
			return done(lexer, NEWLINE);
		}
	}

	if (valid_symbols[BLANK_LINE]) {
		const bool advancing = isoneof(lexer->lookahead, "\n\r");
		while (isoneof(lexer->lookahead, "\n\r")) {
			if (lexer->lookahead == '\r') lexer->advance(lexer, false);
			if (lexer->lookahead == '\n') lexer->advance(lexer, false);
			while (isoneof(lexer->lookahead, " \t")) lexer->advance(lexer, false);
		}
		if (advancing) return done(lexer, BLANK_LINE);
	}

	printf("valid LINK_OPEN: %d (%c)\n", valid_symbols[LINK_OPEN], lexer->lookahead);
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

	printf("meanwhile (%c)\n", lexer->lookahead);
	if (valid_symbols[LIST_BULLET]) {
		const uint32_t start_col = lexer->get_column(lexer);
		LOG(start_col);

		for (;; lexer->advance(lexer, false)) {
			printf("Meanwhile (%c)\n", lexer->lookahead);
			LOG((int) pl->list.width);
			if (!isoneof(lexer->lookahead, " \t")) {
				if (lexer->lookahead != '-') break;
				printf("postbreak\n");

				const uint32_t walked = lexer->get_column(lexer) - start_col;
				LOG(walked);
				if (walked == 0) {
					// TODO CHECK correctness of start_col v
					pl->list.level = pl->list.gauging ? 0 : start_col / pl->list.width;

					lexer->advance(lexer, false);
					if (!isoneof(lexer->lookahead, " \t\n\r") && !lexer->eof(lexer))
						break;
					while (isoneof(lexer->lookahead, " \t")) lexer->advance(lexer, false);
					return done(lexer, LIST_BULLET);
				}
				
				if (pl->list.gauging) {
					pl->list.width = walked;
					pl->list.gauging = false;

					lexer->advance(lexer, false);
					if (!isoneof(lexer->lookahead, " \t\n\r") && !lexer->eof(lexer))
						break;
					while (isoneof(lexer->lookahead, " \t")) lexer->advance(lexer, false);
					return done(lexer, LIST_BULLET);
				}

				if (walked % pl->list.width != 0) break;

				const uint32_t level = walked / pl->list.width;
				if (level > pl->list.level + 1) break;
				// TODO NOW deindenting twice
				if (level < pl->list.level + 1) {
					--pl->list.level;
					return done(lexer, LIST_UNINDENT);
				}

				pl->list.level = level;

				lexer->advance(lexer, false);
				if (!isoneof(lexer->lookahead, " \t\n\r") && !lexer->eof(lexer)) break;
				while (isoneof(lexer->lookahead, " \t")) lexer->advance(lexer, false);
				return done(lexer, LIST_BULLET);
			};

			const bool is_tab = lexer->lookahead == '\t';
			if (pl->list.gauging) pl->list.tabs = is_tab;
			if (is_tab != pl->list.tabs) break;
		}
	}
	printf("0meanwhile (%c)\n", lexer->lookahead);

	if (valid_symbols[LIST_UNINDENT]) return done(lexer, LIST_UNINDENT);

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

	if (!lexer->eof(lexer) && !isoneof(lexer->lookahead, "\n\r")) {
		printf("ciao\n");
		while (!lexer->eof(lexer) && !isoneof(lexer->lookahead, "\n\r")) {
			printf("    (%c)\n", lexer->lookahead);
			// TODO NOW DEBUG if i swap these two lines, the latest link test passes
			// but [[foo|bar|baz]] enters loop
			if (isoneof(lexer->lookahead, "[]|")) break;
			lexer->advance(lexer, false);
		}
		return done(lexer, TEXT);
	}

	return false;
}

void *tree_sitter_norsu_external_scanner_create() {
	Context *pl = ts_calloc(1, sizeof(Context));
	pl->list.gauging = true;
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
