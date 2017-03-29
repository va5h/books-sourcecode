Public Class txtSearchform

    Private _application As ESRI.ArcGIS.Framework.IApplication
    Public Property ArcMapApplication() As ESRI.ArcGIS.Framework.IApplication
        Get
            Return _application
        End Get
        Set(ByVal value As ESRI.ArcGIS.Framework.IApplication)
            _application = value
        End Set
    End Property
     
    Private Sub txtSearch_TextChanged(sender As Object, e As EventArgs) Handles txtSearch.TextChanged

        Dim pDocument As ESRI.ArcGIS.ArcMapUI.IMxDocument = _application.Document
        Dim pFeatureLayer As ESRI.ArcGIS.Carto.IFeatureLayerDefinition = pDocument.FocusMap.Layer(0)
        pFeatureLayer.DefinitionExpression = "UPPER(NAME) LIKE '%" & txtSearch.Text.ToUpper & "%'"
        pDocument.UpdateContents()
        pDocument.ActiveView.Refresh()

    End Sub
End Class