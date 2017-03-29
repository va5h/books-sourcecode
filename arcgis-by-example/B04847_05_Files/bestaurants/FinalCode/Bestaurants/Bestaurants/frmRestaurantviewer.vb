Imports ESRI.ArcGIS.Geodatabase
Imports ESRI.ArcGIS.DataSourcesGDB
Imports System.Windows.Forms

Public Class frmRestaurantviewer

    Private Sub frmRestaurantviewer_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        Dim pWorkspaceFactory As IWorkspaceFactory = New FileGDBWorkspaceFactory
        Dim pWorkspace As IWorkspace = pWorkspaceFactory.OpenFromFile("c:\ArcGISbyExample\bestaurants\data\bestaurants.gdb", Me.Handle.ToInt32)

        Dim pFeatureWorkspace As IFeatureWorkspace = pWorkspace
        Dim pFeatureClass As IFeatureClass = pFeatureWorkspace.OpenFeatureClass("Food_and_Drinks")

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

    Private Sub cmbCategory_SelectedIndexChanged(sender As Object, e As EventArgs) Handles cmbCategory.SelectedIndexChanged
        Dim pKeyValuePair As KeyValuePair(Of Integer, String) = cmbCategory.SelectedItem
         
        Dim pWorkspaceFactory As IWorkspaceFactory = New FileGDBWorkspaceFactory
        Dim pWorkspace As IWorkspace = pWorkspaceFactory.OpenFromFile("c:\ArcGISbyExample\bestaurants\data\bestaurants.gdb", Me.Handle.ToInt32)

        Dim pFeatureWorkspace As IFeatureWorkspace = pWorkspace
        Dim pFeatureClass As IFeatureClass = pFeatureWorkspace.OpenFeatureClass("Food_and_Drinks")

        Dim pQFilter As IQueryFilter = New QueryFilter
        pQFilter.WhereClause = "CATEGORY = " & pKeyValuePair.Key

        Dim pFeatureCursor As IFeatureCursor = pFeatureClass.Search(pQFilter, False)

        Dim pFeature As IFeature = pFeatureCursor.NextFeature
        Dim dictVenues As New Dictionary(Of Integer, String)

        Do Until pFeature Is Nothing

            dictVenues.Add(pFeature.OID, pFeature.Value(pFeature.Fields.FindField("NAME")))
            pFeature = pFeatureCursor.NextFeature
        Loop


        cmbVenue.DisplayMember = "Value"
        cmbVenue.ValueMember = "Key"
        cmbVenue.DataSource = New BindingSource(dictVenues, Nothing)

    End Sub
End Class