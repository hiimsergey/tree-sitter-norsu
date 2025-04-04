/**
 * @file A modern markup language designed for personal knowledge management
 * @author Sergey Lavrent <sergeylavrent@protonmail.com>
 * @license GPL-3.0
 */

/// <reference types="tree-sitter-cli/dsl" />
// @ts-check

module.exports = grammar({
    name: "norsu",

    // TODO WIP
    rules: {
        document: $ => seq(
            optional($.metadata),
            repeat($._section)
        ),

        metadata: $ => seq(
            "/**",
            repeat("TODO metadata"),
            "*/"
        ),

        _section: $ => "TODO section"
    },

    externals: $ => [$.newline, $.eof]
});
