/**
 * @file A modern markup language designed for personal knowledge management
 * @author Sergey Lavrent <sergeylavrent@protonmail.com>
 * @license GPL-3.0
 */

/// <reference types="tree-sitter-cli/dsl" />
// @ts-check

module.exports = grammar({
  name: "norsu",

  rules: {
    // TODO: add the actual grammar rules
    source_file: $ => "hello"
  }
});
