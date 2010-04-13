'
' This script adds more attributes to SecureCustomProperties.
' This is needed so that these attributes are not forgotten during elevated rights install and 
' installation goes e.g., to the right target dir.
'
Option Explicit

Dim argCount:argCount = Wscript.Arguments.Count
If (argCount < 1) Then
	Wscript.Echo "Specify the msi to update."
	Wscript.Quit 1
End If
Dim databasePath:databasePath = Wscript.Arguments(0)
Dim queryText:queryText = "UPDATE Property SET Property.Value = 'PREVIOUSVERSIONSINSTALLED;NEWERPRODUCTFOUND;TARGETDIR;USERNAME;COMPANYNAME;SOURCEDIR;ROOTDRIVE' WHERE Property.Property = 'SecureCustomProperties'"


' Connect to Windows installer object
On Error Resume Next
Dim installer : Set installer = Nothing
Set installer = Wscript.CreateObject("WindowsInstaller.Installer") : CheckError

' Open database, 1=transaction mode
Dim database : Set database = installer.OpenDatabase(databasePath, 1) : CheckError

' Process SQL statement
Dim view
Set view = database.OpenView(queryText) : CheckError
view.Execute : CheckError
database.Commit
Wscript.Quit 0

Sub CheckError
	Dim message, errRec
	If Err = 0 Then Exit Sub
	message = Err.Source & " " & Hex(Err) & ": " & Err.Description
	If Not installer Is Nothing Then
		Set errRec = installer.LastErrorRecord
		If Not errRec Is Nothing Then message = message & vbLf & errRec.FormatText
	End If
	Fail message
End Sub

Sub Fail(message)
	Wscript.Echo message
	Wscript.Quit 2
End Sub
