package testingtools

import scala.io.Source

object FileHandler {
  def compareFile(string: String, filename: String): Boolean = {
    Source.fromFile(filename).getLines().mkString("\n") == string
  }
}