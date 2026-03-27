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
			$._text,
			$._newline,
		)),
		paragraph: $ => prec.right(1, repeat1(
			seq($._text, $._newline)
		))
	},
	externals: $ => [
		$._newline,
		$._text,
		$._h1_marker,
	]
});
