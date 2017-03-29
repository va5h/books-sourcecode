<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class frmExcavationEditor
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
        Me.lblExcavationArea = New System.Windows.Forms.Label()
        Me.txtExcavationArea = New System.Windows.Forms.TextBox()
        Me.txtExcavationDepth = New System.Windows.Forms.TextBox()
        Me.lblExcavationDepth = New System.Windows.Forms.Label()
        Me.lblExcavationOID = New System.Windows.Forms.Label()
        Me.txtExcavationOID = New System.Windows.Forms.TextBox()
        Me.txtTreeCount = New System.Windows.Forms.TextBox()
        Me.lblTreeCount = New System.Windows.Forms.Label()
        Me.lblTotalCost = New System.Windows.Forms.Label()
        Me.txtTotalCost = New System.Windows.Forms.TextBox()
        Me.btnSave = New System.Windows.Forms.Button()
        Me.txtDesignID = New System.Windows.Forms.TextBox()
        Me.lblDesignID = New System.Windows.Forms.Label()
        Me.SuspendLayout()
        '
        'lblExcavationArea
        '
        Me.lblExcavationArea.AutoSize = True
        Me.lblExcavationArea.Location = New System.Drawing.Point(12, 82)
        Me.lblExcavationArea.Name = "lblExcavationArea"
        Me.lblExcavationArea.Size = New System.Drawing.Size(85, 13)
        Me.lblExcavationArea.TabIndex = 0
        Me.lblExcavationArea.Text = "Excavation Area"
        '
        'txtExcavationArea
        '
        Me.txtExcavationArea.Location = New System.Drawing.Point(124, 82)
        Me.txtExcavationArea.Name = "txtExcavationArea"
        Me.txtExcavationArea.ReadOnly = True
        Me.txtExcavationArea.Size = New System.Drawing.Size(100, 20)
        Me.txtExcavationArea.TabIndex = 1
        '
        'txtExcavationDepth
        '
        Me.txtExcavationDepth.Location = New System.Drawing.Point(124, 121)
        Me.txtExcavationDepth.Name = "txtExcavationDepth"
        Me.txtExcavationDepth.Size = New System.Drawing.Size(100, 20)
        Me.txtExcavationDepth.TabIndex = 2
        '
        'lblExcavationDepth
        '
        Me.lblExcavationDepth.AutoSize = True
        Me.lblExcavationDepth.Location = New System.Drawing.Point(12, 121)
        Me.lblExcavationDepth.Name = "lblExcavationDepth"
        Me.lblExcavationDepth.Size = New System.Drawing.Size(92, 13)
        Me.lblExcavationDepth.TabIndex = 3
        Me.lblExcavationDepth.Text = "Excavation Depth"
        '
        'lblExcavationOID
        '
        Me.lblExcavationOID.AutoSize = True
        Me.lblExcavationOID.Location = New System.Drawing.Point(12, 47)
        Me.lblExcavationOID.Name = "lblExcavationOID"
        Me.lblExcavationOID.Size = New System.Drawing.Size(105, 13)
        Me.lblExcavationOID.TabIndex = 4
        Me.lblExcavationOID.Text = "Excavation ObjectID"
        '
        'txtExcavationOID
        '
        Me.txtExcavationOID.Location = New System.Drawing.Point(124, 44)
        Me.txtExcavationOID.Name = "txtExcavationOID"
        Me.txtExcavationOID.ReadOnly = True
        Me.txtExcavationOID.Size = New System.Drawing.Size(100, 20)
        Me.txtExcavationOID.TabIndex = 5
        '
        'txtTreeCount
        '
        Me.txtTreeCount.Location = New System.Drawing.Point(124, 157)
        Me.txtTreeCount.Name = "txtTreeCount"
        Me.txtTreeCount.ReadOnly = True
        Me.txtTreeCount.Size = New System.Drawing.Size(100, 20)
        Me.txtTreeCount.TabIndex = 6
        '
        'lblTreeCount
        '
        Me.lblTreeCount.AutoSize = True
        Me.lblTreeCount.Location = New System.Drawing.Point(12, 160)
        Me.lblTreeCount.Name = "lblTreeCount"
        Me.lblTreeCount.Size = New System.Drawing.Size(86, 13)
        Me.lblTreeCount.TabIndex = 7
        Me.lblTreeCount.Text = "Number of Trees"
        '
        'lblTotalCost
        '
        Me.lblTotalCost.AutoSize = True
        Me.lblTotalCost.Location = New System.Drawing.Point(12, 197)
        Me.lblTotalCost.Name = "lblTotalCost"
        Me.lblTotalCost.Size = New System.Drawing.Size(111, 13)
        Me.lblTotalCost.TabIndex = 8
        Me.lblTotalCost.Text = "Total Excavation Cost"
        '
        'txtTotalCost
        '
        Me.txtTotalCost.Location = New System.Drawing.Point(124, 194)
        Me.txtTotalCost.Name = "txtTotalCost"
        Me.txtTotalCost.ReadOnly = True
        Me.txtTotalCost.Size = New System.Drawing.Size(100, 20)
        Me.txtTotalCost.TabIndex = 9
        '
        'btnSave
        '
        Me.btnSave.Location = New System.Drawing.Point(149, 231)
        Me.btnSave.Name = "btnSave"
        Me.btnSave.Size = New System.Drawing.Size(75, 23)
        Me.btnSave.TabIndex = 10
        Me.btnSave.Text = "Save"
        Me.btnSave.UseVisualStyleBackColor = True
        '
        'txtDesignID
        '
        Me.txtDesignID.Location = New System.Drawing.Point(124, 12)
        Me.txtDesignID.Name = "txtDesignID"
        Me.txtDesignID.ReadOnly = True
        Me.txtDesignID.Size = New System.Drawing.Size(100, 20)
        Me.txtDesignID.TabIndex = 12
        '
        'lblDesignID
        '
        Me.lblDesignID.AutoSize = True
        Me.lblDesignID.Location = New System.Drawing.Point(12, 15)
        Me.lblDesignID.Name = "lblDesignID"
        Me.lblDesignID.Size = New System.Drawing.Size(54, 13)
        Me.lblDesignID.TabIndex = 11
        Me.lblDesignID.Text = "Design ID"
        '
        'frmExcavationEditor
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(254, 263)
        Me.Controls.Add(Me.txtDesignID)
        Me.Controls.Add(Me.lblDesignID)
        Me.Controls.Add(Me.btnSave)
        Me.Controls.Add(Me.txtTotalCost)
        Me.Controls.Add(Me.lblTotalCost)
        Me.Controls.Add(Me.lblTreeCount)
        Me.Controls.Add(Me.txtTreeCount)
        Me.Controls.Add(Me.txtExcavationOID)
        Me.Controls.Add(Me.lblExcavationOID)
        Me.Controls.Add(Me.lblExcavationDepth)
        Me.Controls.Add(Me.txtExcavationDepth)
        Me.Controls.Add(Me.txtExcavationArea)
        Me.Controls.Add(Me.lblExcavationArea)
        Me.Name = "frmExcavationEditor"
        Me.Text = "Excavation Editor"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents lblExcavationArea As System.Windows.Forms.Label
    Friend WithEvents txtExcavationArea As System.Windows.Forms.TextBox
    Friend WithEvents txtExcavationDepth As System.Windows.Forms.TextBox
    Friend WithEvents lblExcavationDepth As System.Windows.Forms.Label
    Friend WithEvents lblExcavationOID As System.Windows.Forms.Label
    Friend WithEvents txtExcavationOID As System.Windows.Forms.TextBox
    Friend WithEvents txtTreeCount As System.Windows.Forms.TextBox
    Friend WithEvents lblTreeCount As System.Windows.Forms.Label
    Friend WithEvents lblTotalCost As System.Windows.Forms.Label
    Friend WithEvents txtTotalCost As System.Windows.Forms.TextBox
    Friend WithEvents btnSave As System.Windows.Forms.Button
    Friend WithEvents txtDesignID As System.Windows.Forms.TextBox
    Friend WithEvents lblDesignID As System.Windows.Forms.Label
End Class
