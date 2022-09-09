
ThisBuild / scalacOptions ++= Seq("-unchecked", "-deprecation")

lazy val root = (project in file(".")).settings(
    name := "Pseudo Tools Project",
    scalaVersion := "2.13.8",
    assembly / assemblyOutputPath := file("target/ptp-latest.jar")
)
