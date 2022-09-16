package tools

import scala.io.Source
import emulator.Emulator
import compiler.Compiler
import tools.IO

object Main {
  def main(args: Array[String]): Unit = {
    if (args.length == 2) {
      if (args(0) == "emulate") {
        val bytecodeFilename = args(1)
        val io = new IO()
        val emulator = new Emulator(Source.fromFile(bytecodeFilename).getLines(), io)
        emulator.emulate
        io.generateLogFile("emulator.log")
      } else if (args(0) == "compile") {
        val sourceFilename = args(1)
        val compiler = new Compiler(Source.fromFile(sourceFilename).getLines().mkString)
        println(compiler.compile)
      }
    }
  }
}