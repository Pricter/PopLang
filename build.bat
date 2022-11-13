@ECHO OFF
SetLocal EnableDelayedExpansion

SET cFilenames=
FOR /R %%f in (*.c) do (
    SET cFilenames=!cFilenames! %%f
)

SET assembly=popc
SET compilerFlags=-w -g

ECHO "Building %assembly%%..."
clang %cFilenames% %compilerFlags% -o .\bin\%assembly%.exe
del bin\test.ppopd