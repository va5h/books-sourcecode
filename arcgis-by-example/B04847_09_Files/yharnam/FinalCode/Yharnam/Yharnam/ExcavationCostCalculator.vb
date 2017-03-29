Imports ESRI.ArcGIS.Geodatabase
Imports ESRI.ArcGIS.DataSourcesGDB
Imports ESRI.ArcGIS.Geometry

Public Class ExcavationCostCalculator
    Private _excavationfeature As IFeature

    Public Sub New(pFeature As IFeature)
        _excavationfeature = pFeature
    End Sub

    Public Function TreesCount() As Integer
        Dim pWorkspace As IWorkspace = getYharnamWorkspace()
        Dim pTreeFeatureClass As IFeatureClass = getTreesFeatureClass(pWorkspace)
        Dim pSFilter As ISpatialFilter = New SpatialFilter
        pSFilter.Geometry = _excavationfeature.Shape
        pSFilter.SpatialRel = esriSpatialRelEnum.esriSpatialRelIntersects

        Return pTreeFeatureClass.FeatureCount(pSFilter)
    End Function

    Public Function TreesRemovalCost() As Double
        Dim pWorkspace As IWorkspace = getYharnamWorkspace()
        Dim dTotalTreeRemovalCost As Double = 0
        Dim pTreesFC As IFeatureClass = getTreesFeatureClass(pWorkspace)

        Dim pSFilter As ISpatialFilter = New SpatialFilter
        pSFilter.Geometry = _excavationfeature.Shape
        pSFilter.SpatialRel = esriSpatialRelEnum.esriSpatialRelIntersects
        Dim pFCursor As IFeatureCursor = pTreesFC.Search(pSFilter, False)
        Dim pTreeFeature As IFeature = pFCursor.NextFeature

        Do Until pTreeFeature Is Nothing
            Dim sTreeType As String = pTreeFeature.Value(pTreeFeature.Fields.FindField("TREETYPE"))
            Dim dTreeCost As Double = 0

            Dim pRemovalCost As ITable = getRemovalCostTable(pWorkspace)
            Dim pQfilter As IQueryFilter = New QueryFilter
            pQfilter.WhereClause = "ITEM = '" & sTreeType & "'"
            Dim pCursor As ICursor = pRemovalCost.Search(pQfilter, False)
            Dim pRow As IRow = pCursor.NextRow

            dTreeCost = pRow.Value(pRow.Fields.FindField("COST"))

            dTotalTreeRemovalCost = dTotalTreeRemovalCost + dTreeCost

            pTreeFeature = pFCursor.NextFeature
        Loop

        Return dTotalTreeRemovalCost
    End Function

    Public Function SoilRemovalCost() As Double
        Dim pWorkspace As IWorkspace = getYharnamWorkspace()
        Dim dTotalSoilRemovalCost As Double = 0
        Dim pSoilFC As IFeatureClass = getSoilFeatureClass(pWorkspace)
        ' Dim pExcavationFC As IFeatureClass = getExcavationFeatureClass(pWorkspace)
        Dim pSFilter As ISpatialFilter = New SpatialFilter
        pSFilter.Geometry = _excavationfeature.Shape
        pSFilter.SpatialRel = esriSpatialRelEnum.esriSpatialRelIntersects
        Dim pFCursor As IFeatureCursor = pSoilFC.Search(pSFilter, False)
        Dim pSoilFeature As IFeature = pFCursor.NextFeature

        Do Until pSoilFeature Is Nothing

            Dim pTopologicalOperator As ITopologicalOperator = _excavationfeature.Shape
            Dim pIntersection As IGeometry = pTopologicalOperator.Intersect(pSoilFeature.Shape, esriGeometryDimension.esriGeometry2Dimension)
            Dim pArea As IArea = pIntersection
            Dim dExcavationDepth As Double
            If IsDBNull(_excavationfeature.Value(_excavationfeature.Fields.FindField("DEPTH"))) Then
                dExcavationDepth = 0
            Else
                dExcavationDepth = _excavationfeature.Value(_excavationfeature.Fields.FindField("DEPTH"))
            End If
            Dim dSoilExcavationVolume As Double = Math.Abs(pArea.Area) * dExcavationDepth

            Dim sSoilType As String = pSoilFeature.Value(pSoilFeature.Fields.FindField("SOILTYPE"))
            Dim pRemovalCost As ITable = getRemovalCostTable(pWorkspace)
            Dim pQfilter As IQueryFilter = New QueryFilter
            pQfilter.WhereClause = "ITEM = '" & sSoilType & "'"
            Dim pCursor As ICursor = pRemovalCost.Search(pQfilter, False)
            Dim pRow As IRow = pCursor.NextRow

            Dim dCost As Double = pRow.Value(pRow.Fields.FindField("COST"))

            dTotalSoilRemovalCost = dTotalSoilRemovalCost + dCost * dSoilExcavationVolume

            pSoilFeature = pFCursor.NextFeature
        Loop

        Return dTotalSoilRemovalCost
    End Function

    Public Function TotalExcavationCost() As Double
        Return SoilRemovalCost() + TreesRemovalCost()
    End Function

    Public Function getYharnamWorkspace() As IWorkspace
        Dim pWorkspaceFactory As IWorkspaceFactory = New FileGDBWorkspaceFactory
        Return pWorkspaceFactory.OpenFromFile("C:\ArcGISByExample\yharnam\Data\Yharnam.gdb", 0)
    End Function

    Public Function getExcavationFeatureClass(pWorkspace As IWorkspace) As IFeatureClass
        Dim pFWorkspace As IFeatureWorkspace = pWorkspace
        Return pFWorkspace.OpenFeatureClass("Excavation")
    End Function

    Public Function getSoilFeatureClass(pWorkspace As IWorkspace) As IFeatureClass
        Dim pFWorkspace As IFeatureWorkspace = pWorkspace
        Return pFWorkspace.OpenFeatureClass("Soil")
    End Function

    Public Function getTreesFeatureClass(pWorkspace As IWorkspace) As IFeatureClass
        Dim pFWorkspace As IFeatureWorkspace = pWorkspace
        Return pFWorkspace.OpenFeatureClass("Trees")
    End Function

    Public Function getRemovalCostTable(pWorkspace As IWorkspace) As ITable
        Dim pFWorkspace As IFeatureWorkspace = pWorkspace
        Return pFWorkspace.OpenTable("RemovalCost")
    End Function


End Class
