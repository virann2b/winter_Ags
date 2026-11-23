Attribute VB_Name = "AddModule"
Option Explicit

Sub Add()

    Dim ws As Worksheet
    Set ws = Worksheets(TABLE_SHEET_NAME)

    ' 追加先の行番号（一番下の次の行）
    Dim targetRow As Long
    targetRow = ws.Cells(ws.Rows.Count, ID_COL).End(xlUp).Row + 1
    If targetRow < DATA_BEGINNING_ROW Then targetRow = DATA_BEGINNING_ROW

    ' 入力（キャンセルされたら何もしない）
    If Not InputAttack(targetRow, True) Then Exit Sub

    ' ソート（保存もここで行う）
    Call SortTable

    ' セルのサイズ調整
    ws.Columns("A:F").AutoFit

    MsgBox "追加しました"

End Sub
