@echo off
setlocal enabledelayedexpansion

if "!GCST_WERROR!"=="" set GCST_WERROR=OFF

set "PYTHONPATH=%~dp0.gcst;%PYTHONPATH%"
python3 .gcst/scripts/build.py %*