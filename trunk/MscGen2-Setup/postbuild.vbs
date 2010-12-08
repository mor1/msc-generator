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


' Connect to Windows installer object
On Error Resume Next
Dim installer : Set installer = Nothing
Set installer = Wscript.CreateObject("WindowsInstaller.Installer") : CheckError

' Open database, 1=transaction mode
Dim database : Set database = installer.OpenDatabase(databasePath, 1) : CheckError
Dim view

' Change SecureCustomProperties so that TARGETDIR goes through (bugfx)
Dim queryText:queryText = "UPDATE Property SET Property.Value = 'PREVIOUSVERSIONSINSTALLED;NEWERPRODUCTFOUND;TARGETDIR;USERNAME;COMPANYNAME;SOURCEDIR;ROOTDRIVE' WHERE Property.Property = 'SecureCustomProperties'"
Set view = database.OpenView(queryText) : CheckError
view.Execute : CheckError
View.Close

' Now first query the Component that creates the targetdir
Dim queryComponent:queryComponent = "SELECT CreateFolder.Component_ FROM CreateFolder WHERE CreateFolder.Directory_ = 'TARGETDIR'"
Set view = database.OpenView(queryComponent) : CheckError
view.Execute : CheckError
Dim record
Set record = view.Fetch : CheckError
view.Close

' Now add PATH row to Environment table
Dim queryPATH:queryPATH = "INSERT INTO Environment (Environment.Component_, Environment.Environment, Environment.Name,  Environment.Value) VALUES (?, 'PATH', '*=-PATH', '[~];[TARGETDIR]')"
Set view = database.OpenView(queryPATH) : CheckError
view.Execute(record) : CheckError
view.Close

' Commit database and exit
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
