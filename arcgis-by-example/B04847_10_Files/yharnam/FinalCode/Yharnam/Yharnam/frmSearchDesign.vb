Imports ESRI.ArcGIS.Geodatabase
Imports ESRI.ArcGIS.DataSourcesGDB
Imports ESRI.ArcGIS.Framework

Public Class frmSearchDesign

    Private Sub btnSearch_Click(sender As Object, e As EventArgs) Handles btnSearch.Click
        Dim pWorkspace As IWorkspace = getYharnamWorkspace()

        Dim pTable As ITable = getDesignTable(pWorkspace)
        Dim pQFilter As IQueryFilter = New QueryFilter
        pQFilter.WhereClause = "DESIGNDATE >= date '" & dtFrom.Value & "' and DESIGNDATE <= date '" & dtTo.Value & "'"
        Dim pCursor As ICursor = pTable.Search(pQFilter, False)
        Dim pRow As IRow = pCursor.NextRow

        Do Until pRow Is Nothing
            lstDesigns.Items.Add(pRow.OID)

            pRow = pCursor.NextRow
        Loop

    End Sub

    Private _application As IApplication
    Public Property ArcMapApplication() As IApplication
        Get
            Return _application
        End Get
        Set(ByVal value As IApplication)
            _application = value
        End Set
    End Property

    Public Function getYharnamWorkspace() As IWorkspace
        Dim pWorkspaceFactory As IWorkspaceFactory = New FileGDBWorkspaceFactory
        Return pWorkspaceFactory.OpenFromFile("C:\ArcGISByExample\yharnam\Data\Yharnam.gdb", 0)
    End Function


    Public Function getDesignTable(pWorkspace As IWorkspace) As ITable
        Dim pFWorkspace As IFeatureWorkspace = pWorkspace
        Return pFWorkspace.OpenTable("Design")
    End Function


    Private Sub lstDesigns_SelectedIndexChanged(sender As Object, e As EventArgs) Handles lstDesigns.SelectedIndexChanged
        Dim pDesignmanager As New frmDesignManager
        pDesignmanager.ArcMapApplication = _application
        pDesignmanager.LoadDesign(lstDesigns.SelectedItem)
        pDesignmanager.Show()
    End Sub
End Class