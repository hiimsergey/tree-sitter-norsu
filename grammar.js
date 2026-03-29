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

		_block: $ => choice($.paragraph, $._blank_line),

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

		heading1: $ => prec(1, seq($._h1_marker, optional($._text), $._newline)),
		heading2: $ => prec(1, seq($._h2_marker, optional($._text), $._newline)),
		heading3: $ => prec(1, seq($._h3_marker, optional($._text), $._newline)),
		heading4: $ => prec(1, seq($._h4_marker, optional($._text), $._newline)),
		heading5: $ => prec(1, seq($._h5_marker, optional($._text), $._newline)),
		heading6: $ => prec(1, seq($._h6_marker, optional($._text), $._newline)),

		paragraph: $ => prec.right(repeat1(
			seq($._text, $._newline)
		)),
	},
	externals: $ => [
		$._newline,
		$._blank_line,
		$._text,
		$._h1_marker,
		$._h2_marker,
		$._h3_marker,
		$._h4_marker,
		$._h5_marker,
		$._h6_marker
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
