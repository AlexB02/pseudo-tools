package tools.emulator

import scala.collection.mutable
import parsley.Parsley._
import parsley.character.{char, string, whitespace, digit, letter, alphaNum}
import parsley.combinator.{some, many}
import parsley.Parsley
import tools.IO
import scala.util.control.Breaks._

class Register

case class SP() extends Register

class Instruction

case class PrintLit(message: String) extends Instruction
case class PrintRef(address: Int) extends Instruction
case class Input(address: Int) extends Instruction
case class Exit(code: Int) extends Instruction

object Lexer {
    private def lexeme[A](parser: Parsley[A]) = parser <~ whitespace
    def token[A](parser: Parsley[A]) = lexeme(attempt(parser))
}

object InstructionParser {
    import Lexer.token
    val number = some(digit).map(_.mkString.toInt)
    val printLitParser: Parsley[Instruction] = token(string("PRINT")) ~> some(alphaNum).map(x => PrintLit(x.mkString))
    val printRefParser: Parsley[Instruction] = token(string("PRINT")) ~> char('&') ~> number.map(x => PrintRef(x))
    val inputParser: Parsley[Instruction] = token(string("INPUT")) ~> char('&') ~> number.map(x => Input(x))
    val exitParser: Parsley[Instruction] = token(string("EXIT")) ~> number.map(x => Exit(x))
    val instructionParser: Parsley[Instruction] = many(whitespace) ~> (attempt(printLitParser) <|> attempt(printRefParser) <|> inputParser <|> exitParser)
}

class Emulator(val bytecodeArray: Iterator[String], val io: IO) {

    private val constants = mutable.Map[String, String]()
    private val registers = mutable.Map[Register, Int]()
    private val memory = Array.ofDim[Byte](10)
    private var lineCount = 0;
    private var termination = -1;
    private val SUCCESSFUL_EXIT = "EXIT 0"

    import InstructionParser.instructionParser

    def emulate: Unit = {
        var prevLine = ""
        val (setup, instructions) = this.bytecodeArray.span(line => line != "P:")
        for (line <- setup) {
            val instruction = line.strip();
            if (prevLine.startsWith("S") && prevLine.endsWith(":")) {
                this.constants += (prevLine.dropRight(1) -> instruction)
            }
            lineCount += 1;
            prevLine = instruction;
        }
        instructions.drop(1)
        val instructionsWithExit = instructions.toBuffer += SUCCESSFUL_EXIT
        for (line <- instructionsWithExit) {
            if (termination != -1) {
                io.exit(termination)
                return
            } else {
                val result = instructionParser.parse(line)
                if (result.isSuccess) {
                    handleInstruction(result.get)
                }
            }
        }
    }

    def handleInstruction(instruction: Instruction) = {
        instruction match {
            case PrintLit(stringId) => {
                io.println(this.constants(stringId))
            }
            case PrintRef(address) => {
                val string = new StringBuilder()
                var currentChar: Char = ' '
                var nextChar: Char = ' '
                var offset = 0;
                do {
                    currentChar = this.memory(address + offset).toChar
                    nextChar = this.memory(address + offset + 1).toChar
                    string.append(currentChar)
                    offset += 1
                } while (nextChar != '\u0000')
                io.println(string.toString())
            }
            case Input(pointer) => {
                val input = io.input
                var i = 0
                for (char <- input + '\u0000') {
                    this.memory.update(pointer + i, char.toByte)
                    i += 1;
                }
            }
            case Exit(code) => {
                termination = code
            }
            case _ =>
        }
    }
}
