; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "GraduDesign.h"
LastPage=0

ClassCount=6
Class1=CGraduDesignApp
Class2=CGraduDesignDoc
Class3=CGraduDesignView
Class4=CMainFrame

ResourceCount=3
Resource1=IDD_ABOUTBOX
Class5=CAboutDlg
Resource2=IDR_MAINFRAME
Class6=CSetDefData
Resource3=IDD_SETDEFDATA_DIALOG

[CLS:CGraduDesignApp]
Type=0
HeaderFile=GraduDesign.h
ImplementationFile=GraduDesign.cpp
Filter=N

[CLS:CGraduDesignDoc]
Type=0
HeaderFile=GraduDesignDoc.h
ImplementationFile=GraduDesignDoc.cpp
Filter=N
LastObject=CGraduDesignDoc

[CLS:CGraduDesignView]
Type=0
HeaderFile=GraduDesignView.h
ImplementationFile=GraduDesignView.cpp
Filter=C


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=CMainFrame




[CLS:CAboutDlg]
Type=0
HeaderFile=GraduDesign.cpp
ImplementationFile=GraduDesign.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_MRU_FILE1
Command6=ID_APP_EXIT
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_EDIT_COPY
Command10=ID_EDIT_PASTE
Command11=ID_VIEW_STATUS_BAR
Command12=ID_SetDefData
Command13=ID_RunModel
Command14=ID_APP_ABOUT
CommandCount=14

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_EDIT_COPY
Command2=ID_SetDefData
Command3=ID_FILE_NEW
Command4=ID_FILE_OPEN
Command5=ID_RunModel
Command6=ID_FILE_SAVE
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_NEXT_PANE
Command11=ID_PREV_PANE
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_EDIT_CUT
Command15=ID_EDIT_UNDO
CommandCount=15

[DLG:IDD_SETDEFDATA_DIALOG]
Type=1
Class=CSetDefData
ControlCount=24
Control1=IDC_BTN_ReadTsuData,button,1342242816
Control2=IDC_BTN_ReadRoadData,button,1476460544
Control3=IDC_BTN_ReadBuildData,button,1342242816
Control4=IDC_BTN_ReadshelterData,button,1342242816
Control5=IDC_STATIC,button,1342177287
Control6=IDC_STATIC,static,1342308352
Control7=IDC_RADIODay,button,1342373897
Control8=IDC_RADIONight,button,1342373897
Control9=IDC_STATIC,static,1342308353
Control10=IDC_EDITResident,edit,1350631552
Control11=IDC_STATIC,static,1342308865
Control12=IDC_EDITSchool,edit,1350631552
Control13=IDC_STATIC,static,1342308353
Control14=IDC_EDITShop,edit,1350631552
Control15=IDC_STATIC,static,1342308865
Control16=IDC_EDITPark,edit,1350631552
Control17=IDC_STATIC,static,1342308353
Control18=IDC_EDITHospital,edit,1350631552
Control19=IDC_STATIC,static,1342308865
Control20=IDC_EDITHotel,edit,1350631552
Control21=IDC_STATIC,static,1342308353
Control22=IDC_EDITFactory,edit,1350631552
Control23=IDC_STATIC,static,1342308865
Control24=IDC_EDITWorkPlace,edit,1350631552

[CLS:CSetDefData]
Type=0
HeaderFile=SetDefData.h
ImplementationFile=SetDefData.cpp
BaseClass=CDialog
Filter=D
LastObject=CSetDefData
VirtualFilter=dWC

