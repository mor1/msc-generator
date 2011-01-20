================================================================================
    MICROSOFT FOUNDATION CLASS LIBRARY : Msc-generator Project Overview
===============================================================================

The application wizard has created this Msc-generator application for
you.  This application not only demonstrates the basics of using the Microsoft
Foundation Classes but is also a starting point for writing your application.

This file contains a summary of what you will find in each of the files that
make up your Msc-generator application.

Msc-generator.vcproj
    This is the main project file for VC++ projects generated using an application wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    application wizard.

Msc-generator.h
    This is the main header file for the application.  It includes other
    project specific headers (including Resource.h) and declares the
    CMscGenApp application class.

Msc-generator.cpp
    This is the main application source file that contains the application
    class CMscGenApp.

Msc-generator.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
    Visual C++. Your project resources are in 1033.

res\Msc-generator.ico
    This is an icon file, which is used as the application's icon.  This
    icon is included by the main resource file Msc-generator.rc.

res\Mscgenerator.rc2
    This file contains resources that are not edited by Microsoft
    Visual C++. You should place all resources not editable by
    the resource editor in this file.

Msc-generator.reg
    This is an example .reg file that shows you the kind of registration
    settings the framework will set for you.  You can use this as a .reg
    file to go along with your application or just delete it and rely
    on the default RegisterShellFileTypes registration.


/////////////////////////////////////////////////////////////////////////////

For the main frame window:
    The project includes a standard MFC interface.

MainFrm.h, MainFrm.cpp
    These files contain the frame class CMainFrame, which is derived from
    CFrameWnd and controls all SDI frame features.

res\Toolbar.bmp
    This bitmap file is used to create tiled images for the toolbar.
    The initial toolbar and status bar are constructed in the CMainFrame
    class. Edit this toolbar bitmap using the resource editor, and
    update the IDR_MAINFRAME TOOLBAR array in Msc-generator.rc to add
    toolbar buttons.

res\IToolbar.bmp
    This bitmap file is used to create tiled images for the toolbar
    when your server application is in-place activated inside another
    container. This toolbar is constructed in the CInPlaceFrame
    class. This bitmap is similar to the bitmap in res\Toolbar.bmp
    except that it has many nonserver commands removed.

/////////////////////////////////////////////////////////////////////////////

The application wizard creates one document type and one view:

MscGenDoc.h, MscGenDoc.cpp - the document
    These files contain your CMscGenDoc class.  Edit these files to
    add your special document data and to implement file saving and loading
    (via CMscGenDoc::Serialize).
    The Document will have the following strings:
        File extension:      signalling
        File type ID:        Mscgenerator.SignallingChart
        Main frame caption:  Msc-generator
        Doc type name:       SigChart
        Filter name:         Msc-generator Files (*.signalling)
        File new short name: Untitled Chart
        File type long name: Msc-generator Signalling Chart

MscGenView.h, MscGenView.cpp - the view of the document
    These files contain your CMscGenView class.
    CMscGenView objects are used to view CMscGenDoc objects.



/////////////////////////////////////////////////////////////////////////////

The application wizard has also created classes specific to OLE


SrvrItem.h, SrvrItem.cpp
    These files contain your CMscGenSrvrItem. This class is used to
    connect your CMscGenDoc class to the OLE system, and optionally
    provide links to your document.
    The project has support to create and manage Active documents.

IpFrame.h, IpFrame.cpp
    These files contain your CInPlaceFrame. This class is derived
    from COleIPFrameWnd and controls all frame features during in-place activation.



/////////////////////////////////////////////////////////////////////////////

Other Features:

Printing and Print Preview support
    The application wizard has generated code to handle the print, print setup, and print preview
    commands by calling member functions in the CView class from the MFC library.

Split Window
    The application wizard has added support for splitter windows for your application documents.

/////////////////////////////////////////////////////////////////////////////

Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named Msc-generator.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

Msc-generator.manifest
	Application manifest files are used by Windows XP to describe an applications
	dependency on specific versions of Side-by-Side assemblies. The loader uses this
	information to load the appropriate assembly from the assembly cache or private
	from the application. The Application manifest  maybe included for redistribution
	as an external .manifest file that is installed in the same folder as the application
	executable or it may be included in the executable in the form of a resource.
/////////////////////////////////////////////////////////////////////////////

Other notes:

The application wizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

If your application uses MFC in a shared DLL, you will need
to redistribute the MFC DLLs. If your application is in a language
other than the operating system's locale, you will also have to
redistribute the corresponding localized resources MFC90xxxx.DLL.
For more information on both of these topics, please see the section on
redistributing Visual C++ applications in MSDN documentation.

/////////////////////////////////////////////////////////////////////////////
