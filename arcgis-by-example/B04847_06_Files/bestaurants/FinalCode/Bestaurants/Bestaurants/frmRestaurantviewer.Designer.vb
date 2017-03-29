<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class frmRestaurantviewer
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
        Me.cmbCategory = New System.Windows.Forms.ComboBox()
        Me.lblCategory = New System.Windows.Forms.Label()
        Me.lblVenue = New System.Windows.Forms.Label()
        Me.cmbVenue = New System.Windows.Forms.ComboBox()
        Me.lstReviews = New System.Windows.Forms.ListBox()
        Me.lblRatingHeader = New System.Windows.Forms.Label()
        Me.lblAvgRating = New System.Windows.Forms.Label()
        Me.btnHighlight = New System.Windows.Forms.Button()
        Me.SuspendLayout()
        '
        'cmbCategory
        '
        Me.cmbCategory.FormattingEnabled = True
        Me.cmbCategory.Location = New System.Drawing.Point(81, 18)
        Me.cmbCategory.Name = "cmbCategory"
        Me.cmbCategory.Size = New System.Drawing.Size(191, 21)
        Me.cmbCategory.TabIndex = 0
        '
        'lblCategory
        '
        Me.lblCategory.AutoSize = True
        Me.lblCategory.Location = New System.Drawing.Point(13, 21)
        Me.lblCategory.Name = "lblCategory"
        Me.lblCategory.Size = New System.Drawing.Size(49, 13)
        Me.lblCategory.TabIndex = 1
        Me.lblCategory.Text = "Category"
        '
        'lblVenue
        '
        Me.lblVenue.AutoSize = True
        Me.lblVenue.Location = New System.Drawing.Point(13, 55)
        Me.lblVenue.Name = "lblVenue"
        Me.lblVenue.Size = New System.Drawing.Size(38, 13)
        Me.lblVenue.TabIndex = 2
        Me.lblVenue.Text = "Venue"
        '
        'cmbVenue
        '
        Me.cmbVenue.FormattingEnabled = True
        Me.cmbVenue.Location = New System.Drawing.Point(81, 52)
        Me.cmbVenue.Name = "cmbVenue"
        Me.cmbVenue.Size = New System.Drawing.Size(191, 21)
        Me.cmbVenue.TabIndex = 3
        '
        'lstReviews
        '
        Me.lstReviews.FormattingEnabled = True
        Me.lstReviews.Location = New System.Drawing.Point(16, 119)
        Me.lstReviews.Name = "lstReviews"
        Me.lstReviews.Size = New System.Drawing.Size(256, 95)
        Me.lstReviews.TabIndex = 4
        '
        'lblRatingHeader
        '
        Me.lblRatingHeader.AutoSize = True
        Me.lblRatingHeader.Location = New System.Drawing.Point(13, 87)
        Me.lblRatingHeader.Name = "lblRatingHeader"
        Me.lblRatingHeader.Size = New System.Drawing.Size(81, 13)
        Me.lblRatingHeader.TabIndex = 5
        Me.lblRatingHeader.Text = "Average Rating"
        '
        'lblAvgRating
        '
        Me.lblAvgRating.AutoSize = True
        Me.lblAvgRating.Location = New System.Drawing.Point(100, 87)
        Me.lblAvgRating.Name = "lblAvgRating"
        Me.lblAvgRating.Size = New System.Drawing.Size(14, 13)
        Me.lblAvgRating.TabIndex = 6
        Me.lblAvgRating.Text = "X"
        '
        'btnHighlight
        '
        Me.btnHighlight.Location = New System.Drawing.Point(197, 79)
        Me.btnHighlight.Name = "btnHighlight"
        Me.btnHighlight.Size = New System.Drawing.Size(75, 23)
        Me.btnHighlight.TabIndex = 7
        Me.btnHighlight.Text = "Highlight"
        Me.btnHighlight.UseVisualStyleBackColor = True
        '
        'frmRestaurantviewer
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(280, 226)
        Me.Controls.Add(Me.btnHighlight)
        Me.Controls.Add(Me.lblAvgRating)
        Me.Controls.Add(Me.lblRatingHeader)
        Me.Controls.Add(Me.lstReviews)
        Me.Controls.Add(Me.cmbVenue)
        Me.Controls.Add(Me.lblVenue)
        Me.Controls.Add(Me.lblCategory)
        Me.Controls.Add(Me.cmbCategory)
        Me.Name = "frmRestaurantviewer"
        Me.Text = "Restaurant viewer"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents cmbCategory As System.Windows.Forms.ComboBox
    Friend WithEvents lblCategory As System.Windows.Forms.Label
    Friend WithEvents lblVenue As System.Windows.Forms.Label
    Friend WithEvents cmbVenue As System.Windows.Forms.ComboBox
    Friend WithEvents lstReviews As System.Windows.Forms.ListBox
    Friend WithEvents lblRatingHeader As System.Windows.Forms.Label
    Friend WithEvents lblAvgRating As System.Windows.Forms.Label
    Friend WithEvents btnHighlight As System.Windows.Forms.Button
End Class
