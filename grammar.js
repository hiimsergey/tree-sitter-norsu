/**
 * @file A modern markup language designer for personal knowledge management
 * @author Sergey Lavrent <sergeylavrent|at|protonmail|dot|com>
 * @license GPL-3.0
 */

/// <reference types="tree-sitter-cli/dsl" />
// @ts-check

export default grammar({
	name: "norsu",
	rules: {
		document: $ => repeat(choice(
			$.heading,
			$.paragraph
		)),
		heading: $ => prec(2, seq(
			$._h1_marker,
			optional($._text), // TODO NOW CONSIDER
			$._newline
		)),
		paragraph: $ => prec.right(1, repeat1(
			seq($._text, $._newline)
		))
	},
	externals: $ => [
		$._newline,
		$._text,
		$._h1_marker
	]
});

// TODO PLAN TEST
// two consecutive headings
// empty headings
// heading, blank line, paragraph
// multiline paragraph

// TODO
// should i use optional($._text) for empty heading texts or texts of length 0?
// ^ what's easier to query?
