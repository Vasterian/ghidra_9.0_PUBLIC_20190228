@echo off
set HOME=D:\Works\NTSystem
set PATH=%HOME%;%PATH%
java -classpath C:\Jaas1.0\lib\jaas.jar;%HOME%;%CLASSPATH% com.tagish.auth.TestHarness > %HOME%\service.log 2> %HOME%\service.err

