Dim InstallDir
Dim fso
Set wshShell = CreateObject( "WScript.Shell" )
Set wshSystemEnv = wshShell.Environment( "USER" )
' Get the environment variable
 InstallDir = wshSystemEnv( "_MscGenInstall" ) 
' remove it
wshSystemEnv.Remove( "_MscGenInstall" )
Set wshSystemEnv = Nothing
Set wshShell     = Nothing
Set fso = CreateObject("Scripting.FileSystemObject")
fso.CopyFile InstallDir & "msc-gen.exe", InstallDir & "mscgen.exe"
fso = Nothing


