
ThisBuild / scalacOptions ++= Seq("-unchecked", "-deprecation")
libraryDependencies += "com.github.j-mie6" %% "parsley" % "3.3.10"
libraryDependencies += "org.scalatest" %% "scalatest" % "3.2.13" % "test"

lazy val root = (project in file(".")).settings(
    name := "Pseudo Tools Project",
    scalaVersion := "2.13.8",
    assembly / assemblyOutputPath := file("target/ptp-latest.jar")
)
