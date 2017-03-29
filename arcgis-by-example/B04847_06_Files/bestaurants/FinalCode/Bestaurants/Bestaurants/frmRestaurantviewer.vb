Imports ESRI.ArcGIS.Geodatabase
Imports ESRI.ArcGIS.DataSourcesGDB
Imports System.Windows.Forms
Imports ESRI.ArcGIS.ArcMapUI
Imports ESRI.ArcGIS.Display
Imports ESRI.ArcGIS.Geometry
Imports ESRI.ArcGIS.Carto

Public Class frmRestaurantviewer

    Private Sub frmRestaurantviewer_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        Dim pFeatureClass As IFeatureClass = GetVenuesFeatureClass()

        Dim pSubtypes As ISubtypes = pFeatureClass
        Dim eSubtypes As IEnumSubtype = pSubtypes.Subtypes

        eSubtypes.Reset()
        Dim rCode As Integer
        Dim rName As String
        cmbCategory.Items.Clear()

        Dim dicCategories As New Dictionary(Of Integer, String)

        rName = eSubtypes.Next(rCode)
        Do While rName <> ""
            dicCategories.Add(rCode, rName)

            rName = eSubtypes.Next(rCode)
        Loop


        cmbCategory.DisplayMember = "Value"
        cmbCategory.ValueMember = "Key"
        cmbCategory.DataSource = New BindingSource(dicCategories, Nothing)

    End Sub

    Private Function GetVenuesFeatureClass() As IFeatureClass
        Dim pWorkspaceFactory As IWorkspaceFactory = New FileGDBWorkspaceFactory
        Dim pWorkspace As IWorkspace = pWorkspaceFactory.OpenFromFile("c:\ArcGISbyExample\bestaurants\data\bestaurants.gdb", Me.Handle.ToInt32)

        Dim pFeatureWorkspace As IFeatureWorkspace = pWorkspace
        Dim pFeatureClass As IFeatureClass = pFeatureWorkspace.OpenFeatureClass("Food_and_Drinks")
        Return pFeatureClass
    End Function


    Private Function GetReviewsTable() As ITable
        Dim pWorkspaceFactory As IWorkspaceFactory = New FileGDBWorkspaceFactory
        Dim pWorkspace As IWorkspace = pWorkspaceFactory.OpenFromFile("c:\ArcGISbyExample\bestaurants\data\bestaurants.gdb", Me.Handle.ToInt32)

        Dim pFeatureWorkspace As IFeatureWorkspace = pWorkspace
        Dim pTable As ITable = pFeatureWorkspace.OpenTable("VENUES_REVIEW")
        Return pTable
    End Function



    Private Sub cmbCategory_SelectedIndexChanged(sender As Object, e As EventArgs) Handles cmbCategory.SelectedIndexChanged
        Dim pKeyValuePair As KeyValuePair(Of Integer, String) = cmbCategory.SelectedItem

        Dim pFeatureClass As IFeatureClass = GetVenuesFeatureClass()

        Dim pQFilter As IQueryFilter = New QueryFilter
        pQFilter.WhereClause = "CATEGORY = " & pKeyValuePair.Key

        Dim pFeatureCursor As IFeatureCursor = pFeatureClass.Search(pQFilter, False)

        Dim pFeature As IFeature = pFeatureCursor.NextFeature
        Dim dictVenues As New Dictionary(Of Integer, String)

        Do Until pFeature Is Nothing
            Dim sName As String = pFeature.Value(pFeature.Fields.FindField("NAME"))
            Dim oid As Integer = pFeature.OID
            dictVenues.Add(oid, sName)
            pFeature = pFeatureCursor.NextFeature
        Loop


        cmbVenue.DisplayMember = "Value"
        cmbVenue.ValueMember = "Key"
        cmbVenue.DataSource = New BindingSource(dictVenues, Nothing)

        Dim pDocument As IMxDocument = _application.Document
        Dim pFeatureLayer As IFeatureLayerDefinition = pDocument.FocusMap.Layer(0)
        pFeatureLayer.DefinitionExpression = "CATEGORY = " & pKeyValuePair.Key
        pDocument.UpdateContents()
        pDocument.ActiveView.Refresh()

    End Sub

    Private Sub cmbVenue_SelectedIndexChanged(sender As Object, e As EventArgs) Handles cmbVenue.SelectedIndexChanged
        Dim pSelectedVenueKeyValuePair As KeyValuePair(Of Integer, String) = cmbVenue.SelectedItem

        lstReviews.Items.Clear()

        Dim pTable As ITable = GetReviewsTable()
        Dim pQFilter As IQueryFilter = New QueryFilter
        pQFilter.WhereClause = "VENUE_OBJECTID = " & pSelectedVenueKeyValuePair.Key
        Dim pRowCursor As ICursor = pTable.Search(pQFilter, False)
        Dim dAvgRating As Double = 0
        Dim lSumRating As Integer = 0
        Dim lReviewsCount As Integer = 0

        Dim pRow As IRow = pRowCursor.NextRow
        Do Until pRow Is Nothing
            Dim rating As Integer = pRow.Value(pRow.Fields.FindField("RATING"))
            Dim sreview As String = pRow.Value(pRow.Fields.FindField("REVIEW"))

            lstReviews.Items.Add(sreview & " [" & rating & "]")
            lSumRating = lSumRating + rating
            lReviewsCount = lReviewsCount + 1
            pRow = pRowCursor.NextRow
        Loop

        dAvgRating = lSumRating / lReviewsCount
        lblAvgRating.Text = Math.Round(dAvgRating, 1)
    End Sub

    Private Sub btnHighlight_Click(sender As Object, e As EventArgs) Handles btnHighlight.Click

        Dim pDocument As IMxDocument = _application.Document

        Dim pScreenDisplay As IScreenDisplay = pDocument.ActiveView.ScreenDisplay
        pScreenDisplay.StartDrawing(pScreenDisplay.hDC, ESRI.ArcGIS.Display.esriScreenCache.esriNoScreenCache)

        Dim pMarkerSymbol As ISymbol = New SimpleMarkerSymbol
        Dim pSymbol As ISymbol = pMarkerSymbol
        pSymbol.ROP2 = esriRasterOpCode.esriROPNotXOrPen
        pScreenDisplay.SetSymbol(pMarkerSymbol)

        Dim pSelectedVenueKeyValuePair As KeyValuePair(Of Integer, String) = cmbVenue.SelectedItem

        Dim pFeatureClass As IFeatureClass = GetVenuesFeatureClass()
        Dim pFeature As IFeature = pFeatureClass.GetFeature(pSelectedVenueKeyValuePair.Key)

        pScreenDisplay.DrawPoint(pFeature.Shape)
        Threading.Thread.Sleep(100)
        pScreenDisplay.DrawPoint(pFeature.Shape)
        Threading.Thread.Sleep(100)
        pScreenDisplay.DrawPoint(pFeature.Shape)
        Threading.Thread.Sleep(100)
        pScreenDisplay.DrawPoint(pFeature.Shape)

        pScreenDisplay.FinishDrawing()

    End Sub

    Private _application As ESRI.ArcGIS.Framework.IApplication
    Public Property ArcMapApp() As ESRI.ArcGIS.Framework.IApplication
        Get
            Return _application
        End Get
        Set(ByVal value As ESRI.ArcGIS.Framework.IApplication)
            _application = value
        End Set
    End Property



End Class