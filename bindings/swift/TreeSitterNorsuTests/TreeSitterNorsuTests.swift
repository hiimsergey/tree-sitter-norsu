import XCTest
import SwiftTreeSitter
import TreeSitterNorsu

final class TreeSitterNorsuTests: XCTestCase {
    func testCanLoadGrammar() throws {
        let parser = Parser()
        let language = Language(language: tree_sitter_norsu())
        XCTAssertNoThrow(try parser.setLanguage(language),
                         "Error loading Norsu grammar")
    }
}
