Imports ESRI.ArcGIS.ArcMapUI
Imports ESRI.ArcGIS.Geometry
Imports ESRI.ArcGIS.Carto
Imports ESRI.ArcGIS.Display
Imports ESRI.ArcGIS.Geodatabase

Public Class tlFindClosestTower
    Inherits ESRI.ArcGIS.Desktop.AddIns.Tool

    Public Sub New()

    End Sub

    Protected Overrides Sub OnUpdate()

    End Sub


    Protected Overrides Sub OnMouseUp(arg As ESRI.ArcGIS.Desktop.AddIns.Tool.MouseEventArgs)
         
            Dim pDocument As IMxDocument = My.ArcMap.Application.Document
            Dim pPoint As IPoint = pDocument.ActiveView.ScreenDisplay.DisplayTransformation.ToMapPoint(arg.X, arg.Y)
            pDocument.ActiveView.GraphicsContainer.DeleteAllElements()


            Dim pElement As IElement = New MarkerElement
            pElement.Geometry = pPoint
            Dim pMarkerElement As IMarkerElement = pElement

            Dim pPictureMarkerSymbol As IPictureMarkerSymbol = New PictureMarkerSymbol
            pPictureMarkerSymbol.CreateMarkerSymbolFromFile(esriIPictureType.esriIPicturePNG, "C:\ArcGISByExample\telzaviba\Icons\Cell-phone.png")
            pPictureMarkerSymbol.Size = 20

            pMarkerElement.Symbol = pPictureMarkerSymbol

            pDocument.ActiveView.GraphicsContainer.AddElement(pElement, 0)
            pDocument.ActiveView.Refresh()
             
         
        Dim pTowerLayer As IFeatureLayer = pDocument.FocusMap.Layer(0)
        Dim pFeatureCursor As IFeatureCursor = pTowerLayer.FeatureClass.Search(Nothing, False)
        Dim pFeature As IFeature = pFeatureCursor.NextFeature

        Dim pProximityOperator As IProximityOperator = pPoint

        Dim closestTowerOID As Long
        Dim shortestdistance As Double = Double.MaxValue

        Do Until pFeature Is Nothing

            Dim distance As Double = pProximityOperator.ReturnDistance(pFeature.Shape)
            If distance < shortestdistance Then
                shortestdistance = distance
                closestTowerOID = pFeature.OID
            End If

            pFeature = pFeatureCursor.NextFeature
        Loop


        Dim pClosestTower As IFeature = pTowerLayer.FeatureClass.GetFeature(closestTowerOID)

        Dim pScreenDisplay As IScreenDisplay = pDocument.ActiveView.ScreenDisplay
        pScreenDisplay.StartDrawing(pScreenDisplay.hDC, ESRI.ArcGIS.Display.esriScreenCache.esriNoScreenCache)

        Dim pMarkerSymbol As IMarkerSymbol = New SimpleMarkerSymbol
        Dim pColor As IRgbColor = New RgbColor
        pColor.RGB = RGB(255, 0, 0)
        pMarkerSymbol.Color = pColor
        pMarkerSymbol.Size = 10
        Dim pSymbol As ISymbol = pMarkerSymbol
        pSymbol.ROP2 = esriRasterOpCode.esriROPNotXOrPen

        pScreenDisplay.SetSymbol(pSymbol)

        pScreenDisplay.DrawPoint(pClosestTower.Shape)
        Threading.Thread.Sleep(200)
        pScreenDisplay.DrawPoint(pClosestTower.Shape)
        Threading.Thread.Sleep(200)
        pScreenDisplay.DrawPoint(pClosestTower.Shape)
        Threading.Thread.Sleep(200)
        pScreenDisplay.DrawPoint(pClosestTower.Shape)

        pScreenDisplay.FinishDrawing()

        Dim t As String = pClosestTower.Value(pClosestTower.Fields.FindField("TOWER_ID"))
        Dim r As Double = pClosestTower.Value(pClosestTower.Fields.FindField("TOWER_RANGE_METERS"))
        Dim s As Double = Math.Round((r - shortestdistance) / r * 100)


        Dim pTextPoint As IPoint = pPoint
        pTextPoint.Y = pTextPoint.Y - 20
        Dim pTextElement As ITextElement = New TextElement
        pTextElement.Text = t & "(" & s & "%)"
        Dim pTextSymbol As ITextSymbol = New TextSymbol
        pTextSymbol.Size = 5

        Dim pTheTextElement As IElement = pTextElement
        pTheTextElement.Geometry = pTextPoint
        pDocument.ActiveView.GraphicsContainer.AddElement(pTheTextElement, 0)
        pDocument.ActiveView.Refresh()

        '   MsgBox("Closest Tower is " & pClosestTower.Value(pClosestTower.Fields.FindField("TOWER_ID")) & " with a distance of " & Math.Round(shortestdistance) & " meters" & vbCrLf & "Signal Strength is " & s & "%")

    End Sub

End Class
