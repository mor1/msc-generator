@echo off
for %%a IN (*.signalling) DO (
    echo %%a
    msc-gen "%%a" -T png 
)
pause
