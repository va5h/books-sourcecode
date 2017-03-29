Imports System.Runtime.InteropServices
Imports System.Drawing
Imports ESRI.ArcGIS.ADF.BaseClasses
Imports ESRI.ArcGIS.ADF.CATIDs
Imports ESRI.ArcGIS.Framework
Imports ESRI.ArcGIS.ArcMapUI

<ComClass(cmSearch.ClassId, cmSearch.InterfaceId, cmSearch.EventsId), _
 ProgId("Bestaurants.cmSearch")> _
Public NotInheritable Class cmSearch
    Inherits BaseCommand
    Implements ESRI.ArcGIS.SystemUI.IToolControl


#Region "COM GUIDs"
    ' These  GUIDs provide the COM identity for this class 
    ' and its COM interfaces. If you change them, existing 
    ' clients will no longer be able to access the class.
    Public Const ClassId As String = "e6286de9-4ae3-4da6-8fc7-f6e553bb7842"
    Public Const InterfaceId As String = "1bad1046-0968-4aaf-989d-962240017ecc"
    Public Const EventsId As String = "0c885912-bd0e-41ab-973b-f98d0c7b09dd"
#End Region

#Region "COM Registration Function(s)"
    <ComRegisterFunction(), ComVisibleAttribute(False)> _
    Public Shared Sub RegisterFunction(ByVal registerType As Type)
        ' Required for ArcGIS Component Category Registrar support
        ArcGISCategoryRegistration(registerType)

        'Add any COM registration code after the ArcGISCategoryRegistration() call

    End Sub

    <ComUnregisterFunction(), ComVisibleAttribute(False)> _
    Public Shared Sub UnregisterFunction(ByVal registerType As Type)
        ' Required for ArcGIS Component Category Registrar support
        ArcGISCategoryUnregistration(registerType)

        'Add any COM unregistration code after the ArcGISCategoryUnregistration() call

    End Sub

#Region "ArcGIS Component Category Registrar generated code"
    Private Shared Sub ArcGISCategoryRegistration(ByVal registerType As Type)
        Dim regKey As String = String.Format("HKEY_CLASSES_ROOT\CLSID\{{{0}}}", registerType.GUID)
        MxCommands.Register(regKey)

    End Sub
    Private Shared Sub ArcGISCategoryUnregistration(ByVal registerType As Type)
        Dim regKey As String = String.Format("HKEY_CLASSES_ROOT\CLSID\{{{0}}}", registerType.GUID)
        MxCommands.Unregister(regKey)

    End Sub

#End Region
#End Region


    Private m_application As IApplication
    Private _txtsearchform As New txtSearchform

    Public Sub New()
        MyBase.New()

        ' TODO: Define values for the public properties
        MyBase.m_category = "Bestaurants"  'localizable text 
        MyBase.m_caption = "Bestaurants Quick Search"   'localizable text 
        MyBase.m_message = "Bestaurants Quick Search"   'localizable text 
        MyBase.m_toolTip = "Bestaurants Quick Search" 'localizable text 
        MyBase.m_name = "Bestaurants_BestaurantsQuickSearch"  'unique id, non-localizable (e.g. "MyCategory_ArcMapCommand")

        Try
            'TODO: change bitmap name if necessary
            Dim bitmapResourceName As String = Me.GetType().Name + ".bmp"
            MyBase.m_bitmap = New Bitmap(Me.GetType(), bitmapResourceName)
        Catch ex As Exception
            System.Diagnostics.Trace.WriteLine(ex.Message, "Invalid Bitmap")
        End Try


    End Sub


    Public Overrides Sub OnCreate(ByVal hook As Object)
        If Not hook Is Nothing Then
            m_application = CType(hook, IApplication)

            'Disable if it is not ArcMap
            If TypeOf hook Is IMxApplication Then
                MyBase.m_enabled = True
            Else
                MyBase.m_enabled = False
            End If
        End If
        ' TODO:  Add other initialization code
        _txtsearchform.ArcMapApplication = m_application

    End Sub

    Public Overrides Sub OnClick()
        'TODO: Add cmSearch.OnClick implementation
    End Sub

    Public ReadOnly Property hWnd As Integer Implements ESRI.ArcGIS.SystemUI.IToolControl.hWnd
        Get
            Return _txtsearchform.Handle.ToInt32
        End Get
    End Property

    Public Function OnDrop(barType As ESRI.ArcGIS.SystemUI.esriCmdBarType) As Boolean Implements ESRI.ArcGIS.SystemUI.IToolControl.OnDrop

    End Function

    Public Sub OnFocus(complete As ESRI.ArcGIS.SystemUI.ICompletionNotify) Implements ESRI.ArcGIS.SystemUI.IToolControl.OnFocus

    End Sub
End Class



