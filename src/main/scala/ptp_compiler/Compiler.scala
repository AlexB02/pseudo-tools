package tools.compiler

import parsley.Parsley
import parsley.Parsley.{attempt, lookAhead}
import parsley.combinator.{many, eof, manyUntil}
import parsley.errors.combinator._
import parsley.token.LanguageDef
import parsley.token.Lexer
import parsley.character.{char, string, anyChar, alphaNum}

import scala.collection.mutable

class Compiler(val sourceCode: String) {

  import parser.{indent, stat, program, print, Statement, StringLit, Print}

  val stringLiterals = mutable.Map[String, String]()

  def compile: String = {
    val result = program.parse(sourceCode)
    if (result.isSuccess) {
      generateCode(result.get)
    } else {
      result.toString()
    }
  }

  def generateCode(ast: List[(Int, Statement)]): String = {
    val instructions = new StringBuilder()
    instructions.append("P:\n")
    for (line <- ast) {
      instructions.append(handleLine(line))
    }
    instructions.insert(0, handleStringConstants)
    instructions.toString()
  }

  def handleLine(line: (Int, Statement)) = {
    val (indent, statement) = line
    statement match {
      case (Print(StringLit(string))) => {
        val stringNum = "S0"
        stringLiterals.addOne(stringNum -> string)
        "PRINT " + stringNum
      }
    }
  }

  def handleStringConstants = {
    val constants = new StringBuilder()
    for (stringId <- stringLiterals.keys) {
      constants.append(stringId + ":\n")
      constants.append(stringLiterals(stringId) + '\n')
    }
    constants
  }
}
