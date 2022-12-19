package tools

import scala.io.Source
import emulator.Emulator
import compiler.Compiler
import tools.IO
import java.io.BufferedWriter
import java.io.FileWriter
import java.io.File

object Main {
  val OUTPUT_FLAG = "-o"
  def writeToOutput(compiled: String, outputDirectory: String): Unit = {
    val outputFile = new File(outputDirectory)
    val writer = new BufferedWriter(new FileWriter(outputFile))
    writer.write(compiled)
    writer.close()
  }

  def hasOutputDirectoryFlag(args: Array[String]): Boolean = {
    val index = args.indexOf(OUTPUT_FLAG)
    if (index == -1) {
      false
    } else {
      index + 1 < args.length
    }
  }

  def getOutputDirectoryFlag(args: Array[String]): String = {
    args(args.indexOf(OUTPUT_FLAG) + 1)
  }

  def main(args: Array[String]): Unit = {
    if (args.length == 0) {
      println("Usage: pscompile <filepath>")
      sys.exit(-1)
    }
    val sourceFilename = args(0)
    val source = Source.fromFile(sourceFilename).getLines().mkString
    val compiler = new Compiler(source)
    val compiled = compiler.compile
    var outputDirectory = "out.ptb"
    if (hasOutputDirectoryFlag(args)) {
      outputDirectory = getOutputDirectoryFlag(args)
    }
    writeToOutput(compiled, outputDirectory)
  }
}
