Imports ESRI.ArcGIS.ArcMapUI
Imports ESRI.ArcGIS.Geometry
Imports ESRI.ArcGIS.Carto
Imports ESRI.ArcGIS.Display
Imports ESRI.ArcGIS.Geodatabase

Public Class frmRealTime
     



    Public Sub RunManeuver(X As Double, Y As Double)
        Try

            Dim pPoint As IPoint = New Point
            pPoint.PutCoords(X, Y)

            Dim pDocument As IMxDocument = My.ArcMap.Application.Document
            pDocument.ActiveView.GraphicsContainer.DeleteAllElements()

            Dim pElement As IElement = New MarkerElement
            pElement.Geometry = pPoint
            Dim pMarkerElement As IMarkerElement = pElement

            Dim pPictureMarkerSymbol As IPictureMarkerSymbol = New PictureMarkerSymbol
            pPictureMarkerSymbol.CreateMarkerSymbolFromFile(esriIPictureType.esriIPicturePNG, "C:\ArcGISByExample\telzaviba\Icons\Cell-phone.png")
            pPictureMarkerSymbol.Size = 20
            pMarkerElement.Symbol = pPictureMarkerSymbol


            pDocument.ActiveView.GraphicsContainer.AddElement(pElement, 0)

             

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



        Catch ex As Exception
            MsgBox(ex.ToString)
        End Try
    End Sub
     

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        OpenFileDialog1.ShowDialog()

        Dim gpsFile As String = OpenFileDialog1.FileName
        Dim eLines As IEnumerable(Of String) = IO.File.ReadLines(gpsFile)

        _lines = New List(Of String)
        _currentline = 0

        For Each sLine As String In eLines
           
            _lines.Add(sLine)
        Next

        Timer1.Interval = TextBox1.Text * 1000
        Timer1.Enabled = True


    End Sub


    Private _lines As New List(Of String)
    Private _currentline As Integer = 0

    Private Sub Timer1_Tick(sender As Object, e As EventArgs) Handles Timer1.Tick

        If _currentline >= _lines.Count Then Return

        Dim sLine As String = _lines(_currentline)
        Dim x As Double = sLine.Split(",")(0)
        Dim y As Double = sLine.Split(",")(1)

        RunManeuver(x, y)

        _currentline = _currentline + 1
    End Sub
End Class