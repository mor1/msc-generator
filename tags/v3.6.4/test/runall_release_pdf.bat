@echo off
for %%a IN (*.signalling) DO (
    echo %%a
    ..\Release\msc-gen "%%a" -T pdf -ah -s=width -p
)
pause
