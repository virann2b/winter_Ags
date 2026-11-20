Attribute VB_Name = "SortTableModule"
Option Explicit

Sub SortTable()

    Dim ws As Worksheet
    Set ws = Worksheets(TABLE_SHEET_NAME)

    Dim lastRow As Long
    Dim r As Long

    ' ç≈èIçséÊìæ
    lastRow = ws.Cells(ws.Rows.Count, ID_COL).End(xlUp).Row
    If lastRow < DATA_BEGINNING_ROW Then Exit Sub

    ' ä˘ë∂ÇÃãÛçsÇçÌèúÇµÇƒãlÇﬂÇÈ Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`

    For r = lastRow To DATA_BEGINNING_ROW Step -1
        If CStr(ws.Cells(r, ID_COL).Value) = "" Then
            ws.Rows(r).Delete
        End If
    Next r

    ' çsçÌèúå„Ç…ç≈èIçsÇçƒéÊìæ
    lastRow = ws.Cells(ws.Rows.Count, ID_COL).End(xlUp).Row

    ' Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`


    ' É\Å[Égî‘çÜÇï™óﬁÇ©ÇÁêUÇËíºÇ∑ Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`

    For r = DATA_BEGINNING_ROW To lastRow
        ws.Cells(r, SORT_COL).Value = GetCategorySortOrder(CStr(ws.Cells(r, CATEGORY_COL).Value))
    Next r

    ' Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`


    ' É\Å[Ég Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`

    With ws.Sort

        .SortFields.Clear

        ' É\Å[Égî‘çÜÅiï™óﬁÅj
        .SortFields.Add _
            Key:=ws.Range(ws.Cells(DATA_BEGINNING_ROW, SORT_COL), ws.Cells(lastRow, SORT_COL)), _
            SortOn:=xlSortOnValues, Order:=xlAscending, DataOption:=xlSortNormal

        ' ID
        .SortFields.Add _
            Key:=ws.Range(ws.Cells(DATA_BEGINNING_ROW, ID_COL), ws.Cells(lastRow, ID_COL)), _
            SortOn:=xlSortOnValues, Order:=xlAscending, DataOption:=xlSortNormal

        ' É\Å[ÉgîÕàÕ
        .SetRange ws.Range(ws.Cells(HEADER_ROW, ID_COL), ws.Cells(lastRow, SORT_COL))

        .Header = xlYes
        .Apply

    End With

    ' Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`


    ' ï™óﬁÇ™ïœÇÌÇÈÇΩÇ—Ç…1çsãÛÇØÇÈ Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`

    For r = lastRow To DATA_BEGINNING_ROW + 1 Step -1
        If ws.Cells(r, SORT_COL).Value <> ws.Cells(r - 1, SORT_COL).Value Then
            ws.Rows(r).Insert
        End If
    Next r

    ' Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`

    ThisWorkbook.Save

End Sub
