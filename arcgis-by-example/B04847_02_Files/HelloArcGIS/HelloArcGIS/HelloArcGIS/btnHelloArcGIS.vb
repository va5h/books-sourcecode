Public Class btnHelloArcGIS
  Inherits ESRI.ArcGIS.Desktop.AddIns.Button

  Public Sub New()

  End Sub

  Protected Overrides Sub OnClick()
        MsgBox("Hello, ArcGIS!")
  End Sub

  Protected Overrides Sub OnUpdate()

  End Sub
End Class
