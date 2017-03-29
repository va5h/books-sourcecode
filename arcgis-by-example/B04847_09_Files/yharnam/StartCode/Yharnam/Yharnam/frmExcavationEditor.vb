Imports ESRI.ArcGIS.Framework
Imports ESRI.ArcGIS.Geodatabase
Imports ESRI.ArcGIS.Geometry
Imports ESRI.ArcGIS.DataSourcesGDB

Public Class frmExcavationEditor

    Private _application As IApplication
    Public Property ArcMapApplication() As IApplication
        Get
            Return _application
        End Get
        Set(ByVal value As IApplication)
            _application = value
        End Set
    End Property

    Public Sub PopulateExcavation(pFeature As IFeature)

        Dim designID As Long = 0
        Dim dDepth As Double = 0
        If IsDBNull(pFeature.Value(pFeature.Fields.FindField("DESIGNID"))) = False Then
            designID = pFeature.Value(pFeature.Fields.FindField("DESIGNID"))
        End If

        If IsDBNull(pFeature.Value(pFeature.Fields.FindField("DEPTH"))) = False Then
            dDepth = pFeature.Value(pFeature.Fields.FindField("DEPTH"))
        End If

        txtDesignID.Text = designID
        txtExcavationDepth.Text = dDepth
        txtExcavationOID.Text = pFeature.OID

        Dim pArea As IArea = pFeature.Shape
        txtExcavationArea.Text = pArea.Area
    End Sub


    Public Function getYharnamWorkspace() As IWorkspace
        Dim pWorkspaceFactory As IWorkspaceFactory = New FileGDBWorkspaceFactory
        Return pWorkspaceFactory.OpenFromFile("C:\ArcGISByExample\yharnam\Data\Yharnam.gdb", _application.hWnd)
    End Function

    Public Function getExcavationFeatureClass(pWorkspace As IWorkspace) As IFeatureClass
        Dim pFWorkspace As IFeatureWorkspace = pWorkspace
        Return pFWorkspace.OpenFeatureClass("Excavation")
    End Function


    Private Sub btnSave_Click(sender As Object, e As EventArgs) Handles btnSave.Click
        Dim pWorkspaceEdit As IWorkspaceEdit = getYharnamWorkspace()
        Dim pFeatureClass As IFeatureClass = getExcavationFeatureClass(pWorkspaceEdit)

        Dim pFeature As IFeature = pFeatureClass.GetFeature(txtExcavationOID.Text)

        pWorkspaceEdit.StartEditing(True)
        pWorkspaceEdit.StartEditOperation()

        pFeature.Value(pFeature.Fields.FindField("DEPTH")) = txtExcavationDepth.Text
        pFeature.Store()

        pWorkspaceEdit.StopEditOperation()
        pWorkspaceEdit.StopEditing(True)
        Me.Close()
    End Sub
End Class