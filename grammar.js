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
			"#",
			$._text,
			$._newline
		),

		paragraph: $ => prec.right(repeat1(seq($._text, $._newline))),

		_text: $ => /[^\r\n]+/,

		// TODO move to scanner
		// TODO recognize EOF
		_newline: $ => /\r?\n/
	}
});
