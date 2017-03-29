Imports ESRI.ArcGIS.ArcMapUI
Imports ESRI.ArcGIS.Geometry
Imports ESRI.ArcGIS.Carto
Imports ESRI.ArcGIS.Display
Imports ESRI.ArcGIS.Geodatabase

Public Class btnRealTime
    Inherits ESRI.ArcGIS.Desktop.AddIns.Button

    Private _lines As New List(Of String)
    Private _currentline As Integer = 0

    Public Sub New()

    End Sub

    Private Sub Timer_tick(sender As Object, e As Timers.ElapsedEventArgs)
     

    End Sub

    Protected Overrides Sub OnClick()
  
        Dim pRealTimeForm As New frmRealTime
        pRealTimeForm.Show()

    End Sub


    Protected Overrides Sub OnUpdate()

    End Sub


    
End Class
