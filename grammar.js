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

		_block: $ => choice($._blank_line, $.paragraph),

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
		h1: $ => prec(1, seq($.h1_open, optional($.h_text), $._newline)),
		h2: $ => prec(1, seq($.h2_open, optional($.h_text), $._newline)),
		h3: $ => prec(1, seq($.h3_open, optional($.h_text), $._newline)),
		h4: $ => prec(1, seq($.h4_open, optional($.h_text), $._newline)),
		h5: $ => prec(1, seq($.h5_open, optional($.h_text), $._newline)),
		h6: $ => prec(1, seq($.h6_open, optional($.h_text), $._newline)),
		h_text: $ => repeat1($._text),

		paragraph: $ => prec.right(repeat1($._line)),
		_line: $ => seq(repeat1($._inline), $._newline),
		_inline: $ => choice(
			$._text,
			$.link,
			$.link_open, $.link_close
		),

		link: $ => seq(
			$.link_open,
			$.link_address,
			// TODO TEST extensively
			optional(seq($.link_alias_separator, repeat($._text))),
			$.link_close
		),
		link_address: $ => repeat1($._text)
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
		$.link_open,
		$.link_close,
		$.link_alias_separator
	],

	extras: $ => [],

	// TODO FINAL CONSIDER REPLACE by a more elegant solution, if it is not
	conflicts: $ => [[$.link, $._inline]]
});

// TODO NOW DEBUG [[]] takes you to ]].no
// TODO NOW DEBUG dont conceal link tokens if link is not intact

/* NOTE test cases (seemingly) not expressable with corpus:
# foo
*/

/* TODO TEST
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
