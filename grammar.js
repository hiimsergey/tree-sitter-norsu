/**
 * @file A modern markup language designed for personal knowledge management
 * @author Sergey Lavrent <sergeylavrent|at|protonmail|dot|com>
 * @license GPL-3.0
 */

/// <reference types="tree-sitter-cli/dsl" />
// @ts-check

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
			$.heading1,
			repeat($._block),
			repeat(choice($.section2, $.section3, $.section4, $.section5, $.section6))
		)),
		section2: $ => prec.right(seq(
			$.heading2,
			repeat($._block),
			repeat(choice($.section3, $.section4, $.section5, $.section6))
		)),
		section3: $ => prec.right(seq(
			$.heading3,
			repeat($._block),
			repeat(choice($.section4, $.section5, $.section6))
		)),
		section4: $ => prec.right(seq(
			$.heading4,
			repeat($._block),
			repeat(choice($.section5, $.section6))
		)),
		section5: $ => prec.right(seq(
			$.heading5,
			repeat($._block),
			repeat($.section6)
		)),
		section6: $ => prec.right(seq(
			$.heading6,
			repeat($._block)
		)),

		// TODO add inline markup to headings
		heading1: $ => prec(1, seq($._h1_open, optional($._text), $._newline)),
		heading2: $ => prec(1, seq($._h2_open, optional($._text), $._newline)),
		heading3: $ => prec(1, seq($._h3_open, optional($._text), $._newline)),
		heading4: $ => prec(1, seq($._h4_open, optional($._text), $._newline)),
		heading5: $ => prec(1, seq($._h5_open, optional($._text), $._newline)),
		heading6: $ => prec(1, seq($._h6_open, optional($._text), $._newline)),

		paragraph: $ => prec.right(repeat1($._line)),
		_line: $ => seq(repeat1($._inline), $._newline),
		_inline: $ => choice(
			$._text,
			$.link,
			$._link_open, $._link_close
		),

		link: $ => seq($._link_open, repeat($._text), $._link_close)
	},

	externals: $ => [
		$._newline,
		$._blank_line,
		$._text,
		$._h1_open,
		$._h2_open,
		$._h3_open,
		$._h4_open,
		$._h5_open,
		$._h6_open,
		$._link_open,
		$._link_close
	],

	extras: $ => [],

	// TODO FINAL CONSIDER REPLACE by a more elegant solution, if is not
	conflicts: $ => [[$.link, $._inline]]
});

/* NOTE test cases (seemingly) not expressable with corpus:
# foo
*/

/* TODO TEST
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
