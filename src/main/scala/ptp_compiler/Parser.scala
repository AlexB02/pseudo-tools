package tools.compiler

import parsley.Parsley
import parsley.Parsley.{attempt, lookAhead}
import parsley.combinator.{many, eof, manyUntil}
import parsley.errors.combinator._
import parsley.token.LanguageDef
import parsley.token.Lexer
import parsley.character.{char, string, anyChar, alphaNum}

import scala.collection.mutable

object lexer {

  val whitespace = char('\n') <|> char('\r') <|> char('\f') <|> char(' ')
  private def lexeme[A](parser: Parsley[A]): Parsley[A] = parser <~ many(whitespace)
  def token[A](parser: Parsley[A]): Parsley[A] = lexeme(attempt(parser))
}

object parser {
  import lexer.token

  case class Indent(size: Int)

  abstract class Expression
  abstract class Statement
  
  case class StringLit(string: String) extends Expression
  case class Print(expression: Expression) extends Statement

  val indent = many(char('\t')).map(tabs => tabs.length)
  val stringLit = char('"') ~> manyUntil(anyChar, attempt(lookAhead(char('"')))).map(lits => StringLit(lits.mkString)).label("string contents") <~ char('"').label("closing string quote")
  val exp = stringLit
  val print = token(string("print") ~> char('(') ~> exp.map(Print(_)) <~ char(')').label("closing print bracket"))
  val stat = indent <~> (print).label("statement")
  val program = many(stat) <~ eof
}

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
      instructions.append("\t" + handleLine(line))
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
      constants.append("\t" + stringLiterals(stringId) + '\n')
    }
    constants
  }
}
