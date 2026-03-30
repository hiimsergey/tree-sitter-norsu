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
		_inline: $ => choice($._text, $.link),

		link: $ => seq($._link_open, repeat($._inline), $._link_close)
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

	extras: $ => []
});

// TODO PLAN TEST
// two consecutive headings
// empty headings
// heading, _blank line, paragraph
// multiline paragraph
//
// multiline paragraph
// `_block`s before section 1
// no sections
// multiple empty headings
// H7+ headings
// single line of non-markup text
// #hashtag should not be a heading
//
// line comments:
// by itself
// multiple consecutive
// splitting one paragraph
// splitting table
//
// block comments:
// splitting heading
// splitting table
// splitting bullet list
// "wrapping style" (%% foo\nbar %%)
//
// links:
// [[foo]]
// foo [[bar]] baz
// foo [[bar]]
// [[foo\nbar]] // TODO NOW NOW DEBUG
// foo [[bar
// foo]] bar
// [foo] bar
// ]foo
// ]]foo
// [[foo [[bar]] baz]]
