@echo off
for %%a IN (*.signalling) DO (
    echo %%a
    ..\Debug\msc-gen "%%a" -T png -s=4
)
pause
