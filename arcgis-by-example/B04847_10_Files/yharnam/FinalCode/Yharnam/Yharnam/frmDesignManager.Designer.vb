<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class frmDesignManager
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.lblDesignID = New System.Windows.Forms.Label()
        Me.lblDesignDate = New System.Windows.Forms.Label()
        Me.lblExcavations = New System.Windows.Forms.Label()
        Me.lstExcavations = New System.Windows.Forms.ListBox()
        Me.lblTrees = New System.Windows.Forms.Label()
        Me.lstTrees = New System.Windows.Forms.ListBox()
        Me.txtDesignID = New System.Windows.Forms.TextBox()
        Me.txtDesignDate = New System.Windows.Forms.TextBox()
        Me.lblTotalArea = New System.Windows.Forms.Label()
        Me.lblTreeCount = New System.Windows.Forms.Label()
        Me.lblExcavationCost = New System.Windows.Forms.Label()
        Me.lblTreeCost = New System.Windows.Forms.Label()
        Me.Label5 = New System.Windows.Forms.Label()
        Me.btnNewDesign = New System.Windows.Forms.Button()
        Me.btnOpenDesign = New System.Windows.Forms.Button()
        Me.btnDeleteDesign = New System.Windows.Forms.Button()
        Me.btnReport = New System.Windows.Forms.Button()
        Me.txtTotalArea = New System.Windows.Forms.TextBox()
        Me.txtTreeCount = New System.Windows.Forms.TextBox()
        Me.txtExcavationCost = New System.Windows.Forms.TextBox()
        Me.txtTreeRemovalCost = New System.Windows.Forms.TextBox()
        Me.txtTotalCost = New System.Windows.Forms.TextBox()
        Me.btnAddExcavation = New System.Windows.Forms.Button()
        Me.btnDeleteExcavation = New System.Windows.Forms.Button()
        Me.SuspendLayout()
        '
        'lblDesignID
        '
        Me.lblDesignID.AutoSize = True
        Me.lblDesignID.Location = New System.Drawing.Point(25, 19)
        Me.lblDesignID.Name = "lblDesignID"
        Me.lblDesignID.Size = New System.Drawing.Size(54, 13)
        Me.lblDesignID.TabIndex = 0
        Me.lblDesignID.Text = "Design ID"
        '
        'lblDesignDate
        '
        Me.lblDesignDate.AutoSize = True
        Me.lblDesignDate.Location = New System.Drawing.Point(25, 44)
        Me.lblDesignDate.Name = "lblDesignDate"
        Me.lblDesignDate.Size = New System.Drawing.Size(66, 13)
        Me.lblDesignDate.TabIndex = 1
        Me.lblDesignDate.Text = "Design Date"
        '
        'lblExcavations
        '
        Me.lblExcavations.AutoSize = True
        Me.lblExcavations.Location = New System.Drawing.Point(25, 68)
        Me.lblExcavations.Name = "lblExcavations"
        Me.lblExcavations.Size = New System.Drawing.Size(65, 13)
        Me.lblExcavations.TabIndex = 2
        Me.lblExcavations.Text = "Excavations"
        '
        'lstExcavations
        '
        Me.lstExcavations.FormattingEnabled = True
        Me.lstExcavations.Location = New System.Drawing.Point(28, 84)
        Me.lstExcavations.Name = "lstExcavations"
        Me.lstExcavations.Size = New System.Drawing.Size(232, 69)
        Me.lstExcavations.TabIndex = 3
        '
        'lblTrees
        '
        Me.lblTrees.AutoSize = True
        Me.lblTrees.Location = New System.Drawing.Point(25, 165)
        Me.lblTrees.Name = "lblTrees"
        Me.lblTrees.Size = New System.Drawing.Size(34, 13)
        Me.lblTrees.TabIndex = 4
        Me.lblTrees.Text = "Trees"
        '
        'lstTrees
        '
        Me.lstTrees.FormattingEnabled = True
        Me.lstTrees.Location = New System.Drawing.Point(28, 181)
        Me.lstTrees.Name = "lstTrees"
        Me.lstTrees.Size = New System.Drawing.Size(232, 69)
        Me.lstTrees.TabIndex = 5
        '
        'txtDesignID
        '
        Me.txtDesignID.Location = New System.Drawing.Point(97, 16)
        Me.txtDesignID.Name = "txtDesignID"
        Me.txtDesignID.ReadOnly = True
        Me.txtDesignID.Size = New System.Drawing.Size(163, 20)
        Me.txtDesignID.TabIndex = 6
        '
        'txtDesignDate
        '
        Me.txtDesignDate.Location = New System.Drawing.Point(97, 42)
        Me.txtDesignDate.Name = "txtDesignDate"
        Me.txtDesignDate.ReadOnly = True
        Me.txtDesignDate.Size = New System.Drawing.Size(163, 20)
        Me.txtDesignDate.TabIndex = 7
        '
        'lblTotalArea
        '
        Me.lblTotalArea.AutoSize = True
        Me.lblTotalArea.Location = New System.Drawing.Point(27, 268)
        Me.lblTotalArea.Name = "lblTotalArea"
        Me.lblTotalArea.Size = New System.Drawing.Size(56, 13)
        Me.lblTotalArea.TabIndex = 8
        Me.lblTotalArea.Text = "Total Area"
        '
        'lblTreeCount
        '
        Me.lblTreeCount.AutoSize = True
        Me.lblTreeCount.Location = New System.Drawing.Point(27, 293)
        Me.lblTreeCount.Name = "lblTreeCount"
        Me.lblTreeCount.Size = New System.Drawing.Size(86, 13)
        Me.lblTreeCount.TabIndex = 9
        Me.lblTreeCount.Text = "Number of Trees"
        '
        'lblExcavationCost
        '
        Me.lblExcavationCost.AutoSize = True
        Me.lblExcavationCost.Location = New System.Drawing.Point(27, 317)
        Me.lblExcavationCost.Name = "lblExcavationCost"
        Me.lblExcavationCost.Size = New System.Drawing.Size(84, 13)
        Me.lblExcavationCost.TabIndex = 10
        Me.lblExcavationCost.Text = "Excavation Cost"
        '
        'lblTreeCost
        '
        Me.lblTreeCost.AutoSize = True
        Me.lblTreeCost.Location = New System.Drawing.Point(25, 339)
        Me.lblTreeCost.Name = "lblTreeCost"
        Me.lblTreeCost.Size = New System.Drawing.Size(103, 13)
        Me.lblTreeCost.TabIndex = 11
        Me.lblTreeCost.Text = "Trees Removal Cost"
        '
        'Label5
        '
        Me.Label5.AutoSize = True
        Me.Label5.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label5.Location = New System.Drawing.Point(25, 386)
        Me.Label5.Name = "Label5"
        Me.Label5.Size = New System.Drawing.Size(65, 13)
        Me.Label5.TabIndex = 12
        Me.Label5.Text = "Total Cost"
        '
        'btnNewDesign
        '
        Me.btnNewDesign.Location = New System.Drawing.Point(6, 426)
        Me.btnNewDesign.Name = "btnNewDesign"
        Me.btnNewDesign.Size = New System.Drawing.Size(75, 23)
        Me.btnNewDesign.TabIndex = 13
        Me.btnNewDesign.Text = "New Design"
        Me.btnNewDesign.UseVisualStyleBackColor = True
        '
        'btnOpenDesign
        '
        Me.btnOpenDesign.Location = New System.Drawing.Point(87, 426)
        Me.btnOpenDesign.Name = "btnOpenDesign"
        Me.btnOpenDesign.Size = New System.Drawing.Size(88, 23)
        Me.btnOpenDesign.TabIndex = 14
        Me.btnOpenDesign.Text = "Open Design"
        Me.btnOpenDesign.UseVisualStyleBackColor = True
        '
        'btnDeleteDesign
        '
        Me.btnDeleteDesign.Location = New System.Drawing.Point(181, 426)
        Me.btnDeleteDesign.Name = "btnDeleteDesign"
        Me.btnDeleteDesign.Size = New System.Drawing.Size(88, 23)
        Me.btnDeleteDesign.TabIndex = 15
        Me.btnDeleteDesign.Text = "Delete Design"
        Me.btnDeleteDesign.UseVisualStyleBackColor = True
        '
        'btnReport
        '
        Me.btnReport.Location = New System.Drawing.Point(275, 426)
        Me.btnReport.Name = "btnReport"
        Me.btnReport.Size = New System.Drawing.Size(88, 23)
        Me.btnReport.TabIndex = 16
        Me.btnReport.Text = "Report"
        Me.btnReport.UseVisualStyleBackColor = True
        '
        'txtTotalArea
        '
        Me.txtTotalArea.Location = New System.Drawing.Point(142, 261)
        Me.txtTotalArea.Name = "txtTotalArea"
        Me.txtTotalArea.ReadOnly = True
        Me.txtTotalArea.Size = New System.Drawing.Size(118, 20)
        Me.txtTotalArea.TabIndex = 17
        '
        'txtTreeCount
        '
        Me.txtTreeCount.Location = New System.Drawing.Point(142, 286)
        Me.txtTreeCount.Name = "txtTreeCount"
        Me.txtTreeCount.ReadOnly = True
        Me.txtTreeCount.Size = New System.Drawing.Size(118, 20)
        Me.txtTreeCount.TabIndex = 18
        '
        'txtExcavationCost
        '
        Me.txtExcavationCost.Location = New System.Drawing.Point(142, 310)
        Me.txtExcavationCost.Name = "txtExcavationCost"
        Me.txtExcavationCost.ReadOnly = True
        Me.txtExcavationCost.Size = New System.Drawing.Size(118, 20)
        Me.txtExcavationCost.TabIndex = 19
        '
        'txtTreeRemovalCost
        '
        Me.txtTreeRemovalCost.Location = New System.Drawing.Point(142, 336)
        Me.txtTreeRemovalCost.Name = "txtTreeRemovalCost"
        Me.txtTreeRemovalCost.ReadOnly = True
        Me.txtTreeRemovalCost.Size = New System.Drawing.Size(118, 20)
        Me.txtTreeRemovalCost.TabIndex = 20
        '
        'txtTotalCost
        '
        Me.txtTotalCost.Location = New System.Drawing.Point(142, 379)
        Me.txtTotalCost.Name = "txtTotalCost"
        Me.txtTotalCost.ReadOnly = True
        Me.txtTotalCost.Size = New System.Drawing.Size(118, 20)
        Me.txtTotalCost.TabIndex = 21
        '
        'btnAddExcavation
        '
        Me.btnAddExcavation.Font = New System.Drawing.Font("Microsoft Sans Serif", 6.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.btnAddExcavation.Location = New System.Drawing.Point(266, 84)
        Me.btnAddExcavation.Name = "btnAddExcavation"
        Me.btnAddExcavation.Size = New System.Drawing.Size(97, 23)
        Me.btnAddExcavation.TabIndex = 22
        Me.btnAddExcavation.Text = "Add Excavation"
        Me.btnAddExcavation.UseVisualStyleBackColor = True
        '
        'btnDeleteExcavation
        '
        Me.btnDeleteExcavation.Font = New System.Drawing.Font("Microsoft Sans Serif", 6.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.btnDeleteExcavation.Location = New System.Drawing.Point(266, 113)
        Me.btnDeleteExcavation.Name = "btnDeleteExcavation"
        Me.btnDeleteExcavation.Size = New System.Drawing.Size(97, 23)
        Me.btnDeleteExcavation.TabIndex = 23
        Me.btnDeleteExcavation.Text = "Delete Excavation"
        Me.btnDeleteExcavation.UseVisualStyleBackColor = True
        '
        'frmDesignManager
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(368, 456)
        Me.Controls.Add(Me.btnDeleteExcavation)
        Me.Controls.Add(Me.btnAddExcavation)
        Me.Controls.Add(Me.txtTotalCost)
        Me.Controls.Add(Me.txtTreeRemovalCost)
        Me.Controls.Add(Me.txtExcavationCost)
        Me.Controls.Add(Me.txtTreeCount)
        Me.Controls.Add(Me.txtTotalArea)
        Me.Controls.Add(Me.btnReport)
        Me.Controls.Add(Me.btnDeleteDesign)
        Me.Controls.Add(Me.btnOpenDesign)
        Me.Controls.Add(Me.btnNewDesign)
        Me.Controls.Add(Me.Label5)
        Me.Controls.Add(Me.lblTreeCost)
        Me.Controls.Add(Me.lblExcavationCost)
        Me.Controls.Add(Me.lblTreeCount)
        Me.Controls.Add(Me.lblTotalArea)
        Me.Controls.Add(Me.txtDesignDate)
        Me.Controls.Add(Me.txtDesignID)
        Me.Controls.Add(Me.lstTrees)
        Me.Controls.Add(Me.lblTrees)
        Me.Controls.Add(Me.lstExcavations)
        Me.Controls.Add(Me.lblExcavations)
        Me.Controls.Add(Me.lblDesignDate)
        Me.Controls.Add(Me.lblDesignID)
        Me.Name = "frmDesignManager"
        Me.Text = "Design Manager"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents lblDesignID As System.Windows.Forms.Label
    Friend WithEvents lblDesignDate As System.Windows.Forms.Label
    Friend WithEvents lblExcavations As System.Windows.Forms.Label
    Friend WithEvents lstExcavations As System.Windows.Forms.ListBox
    Friend WithEvents lblTrees As System.Windows.Forms.Label
    Friend WithEvents lstTrees As System.Windows.Forms.ListBox
    Friend WithEvents txtDesignID As System.Windows.Forms.TextBox
    Friend WithEvents txtDesignDate As System.Windows.Forms.TextBox
    Friend WithEvents lblTotalArea As System.Windows.Forms.Label
    Friend WithEvents lblTreeCount As System.Windows.Forms.Label
    Friend WithEvents lblExcavationCost As System.Windows.Forms.Label
    Friend WithEvents lblTreeCost As System.Windows.Forms.Label
    Friend WithEvents Label5 As System.Windows.Forms.Label
    Friend WithEvents btnNewDesign As System.Windows.Forms.Button
    Friend WithEvents btnOpenDesign As System.Windows.Forms.Button
    Friend WithEvents btnDeleteDesign As System.Windows.Forms.Button
    Friend WithEvents btnReport As System.Windows.Forms.Button
    Friend WithEvents txtTotalArea As System.Windows.Forms.TextBox
    Friend WithEvents txtTreeCount As System.Windows.Forms.TextBox
    Friend WithEvents txtExcavationCost As System.Windows.Forms.TextBox
    Friend WithEvents txtTreeRemovalCost As System.Windows.Forms.TextBox
    Friend WithEvents txtTotalCost As System.Windows.Forms.TextBox
    Friend WithEvents btnAddExcavation As System.Windows.Forms.Button
    Friend WithEvents btnDeleteExcavation As System.Windows.Forms.Button
End Class
