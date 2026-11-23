Attribute VB_Name = "DeleteModule"
Option Explicit

Sub Delete()

    Dim ws As Worksheet
    Set ws = Worksheets(TABLE_SHEET_NAME)

    Dim targetRow As Long
    targetRow = ActiveCell.Row

    ' ヘッダ行・空行ガード
    If ActiveSheet.Name <> TABLE_SHEET_NAME Or targetRow < DATA_BEGINNING_ROW _
       Or CStr(ws.Cells(targetRow, ID_COL).Value) = "" Then
        MsgBox "削除する行を選択してください"
        Exit Sub
    End If

    ' 確認
    If MsgBox("「" & ws.Cells(targetRow, ID_COL).Value & "」を削除しますか？", _
              vbYesNo + vbExclamation) <> vbYes Then
        Exit Sub
    End If

    ws.Rows(targetRow).Delete

    ' ソート（保存もここで行う）
    Call SortTable

    MsgBox "削除しました"

End Sub
