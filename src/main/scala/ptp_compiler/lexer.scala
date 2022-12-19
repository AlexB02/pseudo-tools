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