Attribute VB_Name = "EditModule"
Option Explicit

Sub Edit()

    Dim ws As Worksheet
    Set ws = Worksheets(TABLE_SHEET_NAME)

    Dim targetRow As Long
    targetRow = ActiveCell.Row

    ' 編集できる行か
    If ActiveSheet.Name <> TABLE_SHEET_NAME Or targetRow < DATA_BEGINNING_ROW _
       Or CStr(ws.Cells(targetRow, ID_COL).Value) = "" Then
        MsgBox "編集する行を選択してください"
        Exit Sub
    End If

    ' 入力（キャンセルされたら何もしない）
    If Not InputAttack(targetRow, False) Then Exit Sub

    ' ソート（保存もここで行う）
    Call SortTable

    ' セルのサイズ調整
    ws.Columns("A:F").AutoFit

    MsgBox "編集しました"

End Sub
