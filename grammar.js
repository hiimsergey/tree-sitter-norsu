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

		// TODO NOW DEBUG dont let it be a paragraph
		heading: $ => seq(
			$._h1_marker,
			$._text,
			$._newline
		),

		paragraph: $ => prec.right(repeat1($._line)),
		_line: $ => seq(
			$._text,
			$._newline
		),
	},

	externals: $ => [
		$._newline,
		$._text,
		$._h1_marker
	]
});
