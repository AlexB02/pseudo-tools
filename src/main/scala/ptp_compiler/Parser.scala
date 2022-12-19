package tools.compiler

import parsley.Parsley
import parsley.Parsley.{attempt, lookAhead}
import parsley.combinator.{many, eof, manyUntil}
import parsley.errors.combinator._
import parsley.token.LanguageDef
import parsley.token.Lexer
import parsley.character.{char, string, anyChar, alphaNum}
import scala.collection.mutable

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