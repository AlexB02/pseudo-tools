package testingtools

import scala.io.Source

object FileHandler {
  def getFile(filename: String): String = {
    Source.fromFile(filename).getLines().mkString("\n")
  }
}