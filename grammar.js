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
            repeat($.metadata_line),
            "*/"
        ),

        metadata_line: $ => "TODO metadata",

        _section: $ => choice(
            $.paragraph,
            $._heading,
            $.code_block,
            $.list,
            $.quote,
            $.separator,
            $.table
        ),

        paragraph: $ => seq(
            "TODO paragraph",
            $._break
        ),

        _heading: $ => choice(
            $.heading_1,
            $.heading_2,
            $.heading_3,
            $.heading_4,
            $.heading_5,
            $.heading_6
        ),

        heading_1: $ => seq(
            "#",
            "TODO one"
        ),

        heading_2: $ => "TODO heading 2",
        heading_3: $ => "TODO heading 3",
        heading_4: $ => "TODO heading 4",
        heading_5: $ => "TODO heading 5",
        heading_6: $ => "TODO heading 6",

        code_block: $ => "TODO code block",
        list: $ => "TODO list",
        quote: $ => "TODO quote",
        separator: $ => "TODO separator",
        table: $ => "TODO table",

        _break: $ => choice($._newline, $._eof),
        _newline: $ => seq($._line_ending, optional($.block_continuation))
    },

    externals: $ => [
        $._eof,
        $.block_continuation,
        $._line_ending
    ]
});
