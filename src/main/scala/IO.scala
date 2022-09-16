package tools

import scala.collection.mutable.ArrayBuffer
import java.io.File
import java.io.BufferedWriter
import java.io.FileWriter
import scala.io.StdIn.readLine
import scala.Console
import scala.io.AnsiColor.RED

class IO {

  val logs: ArrayBuffer[String] = ArrayBuffer()

  def log(message: String): Unit = {
    this.logs.addOne(message)
  }

  def println(message: String): Unit = {
    Console.println(message)
    log("> " + message)
  }

  def error(message: String): Unit = {
    Console.println(Console.RED + message)
    log("> " + message)
  }

  def getExitMessage(code: Int) = if (code == 0) "[exit success]" else "[exit " + code.toString() + "]"  

  def exit(code: Int): Unit = {
    val message = getExitMessage(code)
    if (code == 0) {
      Console.println(Console.GREEN + message)
      log("> " + message)
    }
    else
      error(message)
  }

  def input: String = {
    val input = readLine()
    log("< " + input)
    input
  }
  
  def generateLogFile(location: String) = {
    val file = new File(location)
    val bufferedWriter = new BufferedWriter(new FileWriter(file))
    for (log <- logs) {
      bufferedWriter.write(log)
      bufferedWriter.newLine()
    }
    bufferedWriter.close()
  }
}