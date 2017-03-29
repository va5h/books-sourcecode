Imports ESRI.ArcGIS.ArcMapUI
Imports ESRI.ArcGIS.Carto
Imports ESRI.ArcGIS.Geodatabase
Imports ESRI.ArcGIS.Geometry
Imports ESRI.ArcGIS.Display

Public Class btnShowTowerRange
    Inherits ESRI.ArcGIS.Desktop.AddIns.Button

    Public Sub New()

    End Sub

    Protected Overrides Sub OnClick()

        Try
            Dim pDocument As IMxDocument = My.ArcMap.Application.Document
            Dim pTowerLayer As IFeatureLayer = pDocument.FocusMap.Layer(0)

            Dim pFeature As IFeature

            Dim pFeatureCursor As IFeatureCursor = pTowerLayer.FeatureClass.Search(Nothing, False)
            pFeature = pFeatureCursor.NextFeature

            Do Until pFeature Is Nothing


                Dim pTopo As ITopologicalOperator = pFeature.Shape
                Dim range As Double = pFeature.Value(pFeature.Fields.FindField("TOWER_RANGE_METERS"))
                Dim pTowerRange As IGeometry = pTopo.Buffer(range)

                'draw

                Dim pElement As IElement = New PolygonElement
                pElement.Geometry = pTowerRange
                Dim pFillShapeElement As IFillShapeElement = pElement
                Dim pFillShapeSymbol As ISimpleFillSymbol = New SimpleFillSymbol

                pFillShapeSymbol.Style = esriSimpleFillStyle.esriSFSHorizontal
                pFillShapeElement.Symbol = pFillShapeSymbol
                pDocument.ActiveView.GraphicsContainer.AddElement(pElement, 0)

                pFeature = pFeatureCursor.NextFeature
            Loop

            pDocument.ActiveView.Refresh()
             

        Catch ex As Exception
            MsgBox(ex.ToString)
        End Try

    End Sub

    Protected Overrides Sub OnUpdate()

    End Sub
End Class
