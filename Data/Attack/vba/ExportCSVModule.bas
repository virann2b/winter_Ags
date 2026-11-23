Attribute VB_Name = "ExportCSVModule"
Option Explicit

Sub ExportCSV()

    ' 出力前に必ずチェック（問題があれば出力しない）
    If Not CheckTableCore() Then Exit Sub

    Dim ws As Worksheet
    Set ws = Worksheets(TABLE_SHEET_NAME)

    Const firstCol As Long = ID_COL
    Const lastCol As Long = KNOCKBACK_COL

    Dim lastRow As Long
    lastRow = ws.Cells(ws.Rows.Count, firstCol).End(xlUp).Row

    Dim filePath As String
    filePath = ThisWorkbook.Path & "\" & CSV_FILE_NAME

    Dim f As Integer
    f = FreeFile

    Open filePath For Output As #f

    Dim r As Long, c As Long

    For r = DATA_BEGINNING_ROW To lastRow

        ' 空行（分類の区切り）は出力しない
        If CStr(ws.Cells(r, ID_COL).Value) <> "" Then

            Dim line As String
            line = ""

            ' 空欄でも列の位置がずれないよう、必ずカンマで区切る
            For c = firstCol To lastCol
                line = line & CStr(ws.Cells(r, c).Value)
                If c < lastCol Then line = line & ","
            Next c

            Print #f, line

        End If

    Next r

    Close #f

    ThisWorkbook.Save

    MsgBox "CSV出力完了！" & vbCrLf & filePath

End Sub
