package ptp_compiler

import org.scalatest.flatspec.AnyFlatSpec
import scala.io.Source
import tools.compiler.Compiler
import testingtools.FileHandler.getFile
import org.scalatest.matchers.should._

class OCRCompilerIntegrationSpec extends AnyFlatSpec with Matchers {
  
  "print-string.ocr" should "compile to print-string.ptb" in {
    val sourceFilename = "examples/ocr/pseudocode/print-string.ocr"
    val compiler = new Compiler(Source.fromFile(sourceFilename).getLines().mkString)
    compiler.compile should equal (getFile("examples/ocr/bytecode/print-string.ptb"))
  }
}
