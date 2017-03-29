Imports System.Runtime.InteropServices
Imports System.Drawing
Imports ESRI.ArcGIS.ADF.BaseClasses
Imports ESRI.ArcGIS.ADF.CATIDs
Imports ESRI.ArcGIS.Framework
Imports ESRI.ArcGIS.ArcMapUI
Imports System.Windows.Forms
Imports ESRI.ArcGIS.Display
Imports ESRI.ArcGIS.Geometry
Imports ESRI.ArcGIS.Geodatabase
Imports ESRI.ArcGIS.DataSourcesGDB

<ComClass(tlNewExcavation.ClassId, tlNewExcavation.InterfaceId, tlNewExcavation.EventsId), _
 ProgId("Yharnam.tlNewExcavation")> _
Public NotInheritable Class tlNewExcavation
    Inherits BaseTool

#Region "COM GUIDs"
    ' These  GUIDs provide the COM identity for this class 
    ' and its COM interfaces. If you change them, existing 
    ' clients will no longer be able to access the class.
    Public Const ClassId As String = "fd1ef75b-80f0-435d-94ca-0668e0edc174"
    Public Const InterfaceId As String = "5f1cdb8a-c1ec-474f-b1a1-f7afb6bc1465"
    Public Const EventsId As String = "1b90e4cf-188b-4316-8db1-6d649bb24695"
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

    ' A creatable COM class must have a Public Sub New() 
    ' with no parameters, otherwise, the class will not be 
    ' registered in the COM registry and cannot be created 
    ' via CreateObject.
    Public Sub New()
        MyBase.New()

        ' TODO: Define values for the public properties
        ' TODO: Define values for the public properties
        MyBase.m_category = "Yharnam"  'localizable text 
        MyBase.m_caption = "New Excavation"   'localizable text 
        MyBase.m_message = "New Excavation"   'localizable text 
        MyBase.m_toolTip = "New Excavation" 'localizable text 
        MyBase.m_name = "Yharnam_NewExcavation"  'unique id, non-localizable (e.g. "MyCategory_ArcMapTool")

        Try
            'TODO: change resource name if necessary
            Dim bitmapResourceName As String = Me.GetType().Name + ".bmp"
            MyBase.m_bitmap = New Bitmap(Me.GetType(), bitmapResourceName)
            MyBase.m_cursor = New System.Windows.Forms.Cursor(Me.GetType(), Me.GetType().Name + ".cur")
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
    End Sub

    Public Overrides Sub OnClick()
        'TODO: Add tlNewExcavation.OnClick implementation
    End Sub

    Public Overrides Sub OnMouseDown(ByVal Button As Integer, ByVal Shift As Integer, ByVal X As Integer, ByVal Y As Integer)

        Dim pDocument As IMxDocument = m_application.Document

        Dim pRubberBand As IRubberBand = New RubberPolygonClass

        Dim pFillSymbol As ISimpleFillSymbol = New SimpleFillSymbol

        Dim pPolygon As IGeometry = pRubberBand.TrackNew(pDocument.ActiveView.ScreenDisplay, pFillSymbol)

        Dim pWorkspaceEdit As IWorkspaceEdit = getYharnamWorkspace()
        pWorkspaceEdit.StartEditing(True)
        pWorkspaceEdit.StartEditOperation()

        Dim pExcavationFeatureClass As IFeatureClass = getExcavationFeatureClass(pWorkspaceEdit)
        Dim pFeature As IFeature = pExcavationFeatureClass.CreateFeature()
        pFeature.Shape = pPolygon
        pFeature.Store()

        pWorkspaceEdit.StopEditOperation()
        pWorkspaceEdit.StopEditing(True)

    End Sub

    Public Function getYharnamWorkspace() As IWorkspace
        Dim pWorkspaceFactory As IWorkspaceFactory = New FileGDBWorkspaceFactory
        Return pWorkspaceFactory.OpenFromFile("C:\ArcGISByExample\yharnam\Data\Yharnam.gdb", m_application.hWnd)
    End Function

    Public Function getExcavationFeatureClass(pWorkspace As IWorkspace) As IFeatureClass   
        Dim pFWorkspace As IFeatureWorkspace = pWorkspace
        Return pFWorkspace.OpenFeatureClass("Excavation")
    End Function



    Public Overrides Sub OnMouseMove(ByVal Button As Integer, ByVal Shift As Integer, ByVal X As Integer, ByVal Y As Integer)
        'TODO: Add tlNewExcavation.OnMouseMove implementation
    End Sub

    Public Overrides Sub OnMouseUp(ByVal Button As Integer, ByVal Shift As Integer, ByVal X As Integer, ByVal Y As Integer)
        'TODO: Add tlNewExcavation.OnMouseUp implementation
    End Sub
End Class

