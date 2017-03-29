Imports ESRI.ArcGIS.Geodatabase
Imports ESRI.ArcGIS.DataSourcesGDB
Imports ESRI.ArcGIS.esriSystem
Imports ESRI.ArcGIS.Framework
Imports ESRI.ArcGIS.Geometry
Imports ESRI.ArcGIS.Carto
Imports ESRI.ArcGIS.ArcMapUI

Public Class frmDesignManager

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

    Private Sub btnNewDesign_Click(sender As Object, e As EventArgs) Handles btnNewDesign.Click
        Dim pWorkspaceEdit As IWorkspaceEdit = getYharnamWorkspace()
        Dim pDesignTable As ITable = getDesignTable(pWorkspaceEdit)

        pWorkspaceEdit.StartEditing(True)
        pWorkspaceEdit.StartEditOperation()

        Dim pNewDesignRow As IRow = pDesignTable.CreateRow()
        pNewDesignRow.Value(pNewDesignRow.Fields.FindField("DESIGNID")) = pNewDesignRow.OID
        pNewDesignRow.Value(pNewDesignRow.Fields.FindField("DESIGNDATE")) = Now
        pNewDesignRow.Store()

        pWorkspaceEdit.StopEditOperation()
        pWorkspaceEdit.StopEditing(True)

        txtDesignDate.Text = Now
        txtDesignID.Text = pNewDesignRow.OID
    End Sub

    Public Sub LoadDesign(designid As Long)

        Try

            Dim pWorkspace As IWorkspace = getYharnamWorkspace()
            Dim pTable As ITable = getDesignTable(pWorkspace)
            Dim pQFilter As IQueryFilter = New QueryFilter
            pQFilter.WhereClause = "DESIGNID = " & designid
            Dim pCursor As ICursor = pTable.Search(pQFilter, False)
            Dim pRow As IRow = pCursor.NextRow

            If pRow Is Nothing Then
                MsgBox("Design was not found!")
                Exit Sub
            End If

            txtDesignID.Text = pRow.Value(pRow.Fields.FindField("DESIGNID"))
            txtDesignDate.Text = pRow.Value(pRow.Fields.FindField("DESIGNDATE"))

            Dim pFeatureClass As IFeatureClass = getExcavationFeatureClass(pWorkspace)
            Dim pFCursor As IFeatureCursor = pFeatureClass.Search(pQFilter, False)
            Dim pFeature As IFeature = pFCursor.NextFeature
            lstExcavations.Items.Clear()
            lstTrees.Items.Clear()
            Dim totalTreeCount As Long = 0
            Dim dtotalArea As Double = 0
            Dim dtreeRemovalcost As Double = 0
            Dim dExcavationCost As Double = 0
            Dim dTotalCost As Double = 0

            Do Until pFeature Is Nothing
                lstExcavations.Items.Add(pFeature.OID)

                Dim pExcavationCalculator As New ExcavationCostCalculator(pFeature)
                dtreeRemovalcost = dtreeRemovalcost + pExcavationCalculator.TreesRemovalCost
                dExcavationCost = dExcavationCost + pExcavationCalculator.SoilRemovalCost


                Dim pArea As IArea = pFeature.Shape
                dtotalArea = dtotalArea + Math.Abs(pArea.Area)

                Dim pTreeFeatureClass As IFeatureClass = getTreesFeatureClass(pWorkspace)
                Dim pSFilter As ISpatialFilter = New SpatialFilter
                pSFilter.Geometry = pFeature.Shape
                pSFilter.SpatialRel = esriSpatialRelEnum.esriSpatialRelIntersects

                Dim pFeatureCursor As IFeatureCursor = pTreeFeatureClass.Search(pSFilter, False)
                Dim pTreeFeature As IFeature = pFeatureCursor.NextFeature

                Do Until pTreeFeature Is Nothing
                    lstTrees.Items.Add(pTreeFeature.OID)
                    totalTreeCount = totalTreeCount + 1
                    pTreeFeature = pFeatureCursor.NextFeature
                Loop

                pFeature = pFCursor.NextFeature
            Loop

            txtTotalArea.Text = Math.Round(dtotalArea, 2) & "m2"
            txtTreeCount.Text = totalTreeCount
            txtTreeRemovalCost.Text = "$" & Math.Round(dtreeRemovalcost, 2)
            txtExcavationCost.Text = "$" & Math.Round(dExcavationCost, 2)
            txtTotalCost.Text = "$" & Math.Round(dtreeRemovalcost + dExcavationCost, 2)

            Dim pDoc As IMxDocument = _application.Document
            Dim pLayerDef As IFeatureLayerDefinition = pDoc.FocusMap.Layer(1)
            pLayerDef.DefinitionExpression = "DESIGNID=" & designid
            pDoc.ActiveView.Refresh()
        Catch ex As Exception
            MsgBox(ex.ToString)
        End Try
    End Sub


    Private Sub btnAddExcavation_Click(sender As Object, e As EventArgs) Handles btnAddExcavation.Click
        tlNewExcavation.DesignID = txtDesignID.Text

        Dim pUID As New UID
        Dim pCmdItem As ICommandItem
        pUID.Value = "Yharnam.tlNewExcavation"
        pUID.SubType = 3
        pCmdItem = _application.Document.CommandBars.Find(pUID)

        _application.CurrentTool = pCmdItem

    End Sub


    Public Function getExcavationFeatureClass(pWorkspace As IWorkspace) As IFeatureClass
        Dim pFWorkspace As IFeatureWorkspace = pWorkspace
        Return pFWorkspace.OpenFeatureClass("Excavation")
    End Function
    Private Sub frmDesignManager_FormClosed(sender As Object, e As Windows.Forms.FormClosedEventArgs) Handles Me.FormClosed
        _application.CurrentTool = Nothing
        Dim pDoc As IMxDocument = _application.Document
        Dim pLayerDef As IFeatureLayerDefinition = pDoc.FocusMap.Layer(1)
        pLayerDef.DefinitionExpression = ""
        pDoc.ActiveView.Refresh()
    End Sub

    Private Sub btnOpenDesign_Click(sender As Object, e As EventArgs) Handles btnOpenDesign.Click
        Dim designid As Long = InputBox("Enter design id to open ...")
        LoadDesign(designid)
    End Sub

    Private Sub lblDesignID_Click(sender As Object, e As EventArgs) Handles lblDesignID.Click
        LoadDesign(txtDesignID.Text)
    End Sub


    Public Function getTreesFeatureClass(pWorkspace As IWorkspace) As IFeatureClass
        Dim pFWorkspace As IFeatureWorkspace = pWorkspace
        Return pFWorkspace.OpenFeatureClass("Trees")
    End Function

    Private Sub btnDeleteExcavation_Click(sender As Object, e As EventArgs) Handles btnDeleteExcavation.Click
        Dim lExcavationOID As Long = lstExcavations.SelectedItem
        Dim pWorkspaceEdit As IWorkspaceEdit = getYharnamWorkspace()
        pWorkspaceEdit.StartEditing(True)
        pWorkspaceEdit.StartEditOperation()

        Dim pFeatureClass As IFeatureClass = getExcavationFeatureClass(pWorkspaceEdit)

        Dim pFeature As IFeature = pFeatureClass.GetFeature(lExcavationOID)
        pFeature.Delete()
        pFeature.Store()

        pWorkspaceEdit.StopEditOperation()
        pWorkspaceEdit.StopEditing(True)

        LoadDesign(txtDesignID.Text)
    End Sub

    Private Sub btnDeleteDesign_Click(sender As Object, e As EventArgs) Handles btnDeleteDesign.Click
        Dim pWorkspaceEdit As IWorkspaceEdit = getYharnamWorkspace()
        pWorkspaceEdit.StartEditing(True)
        pWorkspaceEdit.StartEditOperation()

        Dim pTable As ITable = getDesignTable(pWorkspaceEdit)
        Dim pRow As IRow = pTable.GetRow(txtDesignID.Text)
        pRow.Delete()
        pRow.Store()


        Dim pFeatureClass As IFeatureClass = getExcavationFeatureClass(pWorkspaceEdit)
        Dim pQFilter As IQueryFilter = New QueryFilter
        pQFilter.WhereClause = "DESIGNID = " & txtDesignID.Text
        Dim pFCursor As IFeatureCursor = pFeatureClass.Update(pQFilter, False)
        Dim pFeature As IFeature = pFCursor.NextFeature

        Do Until pFeature Is Nothing
            pFeature.Delete()
            pFeature.Store()

            pFeature = pFCursor.NextFeature
        Loop

        pWorkspaceEdit.StopEditOperation()
        pWorkspaceEdit.StopEditing(True)
        Me.Close()
    End Sub

    Private Sub btnReport_Click(sender As Object, e As EventArgs) Handles btnReport.Click
        Dim sHTML As String = ""

        sHTML &= "<html><body><br>"
        sHTML &= "<h1>Yharnam Excavation Planning Manager:</h1>"
        sHTML &= "<b>DesignID:</b> " & txtDesignID.Text & "<br>"
        sHTML &= "<b>Design Date:</b> " & txtDesignDate.Text & "<br>"
        sHTML &= "<b>Total Area:</b> " & txtTotalArea.Text & "<br>"
        sHTML &= "<b>Number of Trees:</b> " & txtTreeCount.Text & "<br>"
        sHTML &= "<b>Excavation Cost:</b> " & txtExcavationCost.Text & "<br>"
        sHTML &= "<b>Tree Removal Cost:</b> " & txtTreeRemovalCost.Text & "<br>"
        sHTML &= "<br><br>"
        sHTML &= "<h3>Total Cost:</h3>" & txtTotalCost.Text
   
        sHTML &= "</body></html>"
        Dim sPath As String = "C:\ArcGISByExample\yharnam\Report.html"
        IO.File.WriteAllText(sPath, sHTML)
        Process.Start(sPath)

    End Sub
End Class