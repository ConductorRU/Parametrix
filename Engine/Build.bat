@echo off
setlocal enabledelayedexpansion

REM The %~dp0 specifier resolves to the path to the directory where this .bat is located in.
REM We use this so that regardless of where the .bat file was executed from, we can change to
REM directory relative to where we know the .bat is stored.
pushd "%~dp0\..\..\Source"

REM %1 is the game name
REM %2 is the platform name
REM %3 is the configuration name
