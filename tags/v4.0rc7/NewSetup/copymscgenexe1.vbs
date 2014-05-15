Dim InstallDir
InstallDir = Session.Property("INSTALLDIR")
Set wshShell = CreateObject( "WScript.Shell" )
Set wshSystemEnv = wshShell.Environment( "USER" )
' Set the environment variable
wshSystemEnv( "_MscGenInstall" ) = InstallDir
Set wshSystemEnv = Nothing
Set wshShell     = Nothing