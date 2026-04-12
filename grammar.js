/**
 * @file A modern markup language designed for personal knowledge management
 * @author Sergey Lavrent <sergeylavrent|at|protonmail|dot|com>
 * @license GPL-3.0
 *
 * @typedef {import("tree-sitter-cli/dsl")}
 */

export default grammar({
	name: "norsu",
	rules: {
		document: $ => seq(
			repeat($._block),
			repeat(choice($.section1, $.section2, $.section3, $.section4, $.section5,
				$.section6))
		),

		_block: $ => choice(
			$._blank_line,
			$.paragraph,
			$.list
		),

		section1: $ => prec.right(seq(
			$.h1,
			repeat($._block),
			repeat(choice($.section2, $.section3, $.section4, $.section5, $.section6))
		)),
		section2: $ => prec.right(seq(
			$.h2,
			repeat($._block),
			repeat(choice($.section3, $.section4, $.section5, $.section6))
		)),
		section3: $ => prec.right(seq(
			$.h3,
			repeat($._block),
			repeat(choice($.section4, $.section5, $.section6))
		)),
		section4: $ => prec.right(seq(
			$.h4,
			repeat($._block),
			repeat(choice($.section5, $.section6))
		)),
		section5: $ => prec.right(seq(
			$.h5,
			repeat($._block),
			repeat($.section6)
		)),
		section6: $ => prec.right(seq(
			$.h6,
			repeat($._block)
		)),

		// TODO add inline markup to headings
		h1: $ => prec(1, seq($.h1_open, repeat($._text), $._newline)),
		h2: $ => prec(1, seq($.h2_open, repeat($._text), $._newline)),
		h3: $ => prec(1, seq($.h3_open, repeat($._text), $._newline)),
		h4: $ => prec(1, seq($.h4_open, repeat($._text), $._newline)),
		h5: $ => prec(1, seq($.h5_open, repeat($._text), $._newline)),
		h6: $ => prec(1, seq($.h6_open, repeat($._text), $._newline)),

		paragraph: $ => prec.right(repeat1($._line)),
		_line: $ => seq(repeat1($._inline), $._newline),
		_inline: $ => choice(
			$._text,
			$.link,
			$.link_open, $.link_close, $.link_alias_separator
		),

		list: $ => prec.right(seq(repeat1($.list_item))),
		list_item: $ => seq(
			$.list_bullet,
			repeat($._inline),
			$._newline,
			optional(seq(
				$._list_indent,
				$.list,
				$._list_unindent
			)),
			$._list_unindent
		),

		link: $ => prec.left(seq(
			$.link_open,
			$.link_address,
			optional(seq(
				$.link_alias_separator,
				repeat($._text),
			)),
			$.link_close
		)),
		link_address: $ => repeat1(choice($._text, $.link_open))
	},

	externals: $ => [
		$._newline,
		$._blank_line,
		$._text,
		$.h1_open,
		$.h2_open,
		$.h3_open,
		$.h4_open,
		$.h5_open,
		$.h6_open,
		$.list_bullet,
		$._list_indent,
		$._list_unindent,
		$.link_open,
		$.link_close,
		$.link_alias_separator
	],

	extras: _ => [],

	// TODO FINAL CONSIDER REPLACE by a more elegant solution, if it is not
	conflicts: $ => [[$.link, $._inline]]
});

// TODO NOW decide how to properly highlight the file format to work on all colorschemes

/* NOTE test cases (seemingly) not expressable with corpus:
# foo
*/

/* TODO TEST
lists:
	- foo
	-foo
	-
	- foo\n - bar
	- foo\n-bar
	- foo\n- bar\n- baz
	--foo
	-- foo
	- foo\n-  bar
	-\tfoo\n- bar
	- foo\n\t- bar
	- foo\n\t- bar\n\t- baz
	- foo\n\t- bar\n    - baz
	lvl 2 bullet followed by lvl 0
	one-sided lvl staircase
	double-sided lvl staircase

headings:
	trailing spaces in headings (should not be part of the name)

line comments:
	by itself
	multiple consecutive
	splitting one paragraph
	splitting table

block comments:
	splitting heading
	splitting table
	splitting bullet list
	"wrapping style" (%% foo\nbar %%)

markup:
	[[foo *bar* baz]]

markup edge cases:
	## heading\n\n\n##heading
	empty headings
	[[foo\nbar]]
	[[foo|]]
	[[foo#]]
	[[file://]]
	foo* bar*
	*foo *bar
	```c\n``
	#/
	>
	>>
	<whatever callouts are supposed to look like>
	%% % foo
	-
	- [ ]
	%%%\n%%
	%%%\n%%%
	|||\n|||
*/
