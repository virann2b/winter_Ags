Attribute VB_Name = "FunctionDefineModule"
Option Explicit


' 定義シートに値が定義されているか
Public Function IsDefined(sheetName As String, value As String) As Boolean

    Dim ws As Worksheet
    Set ws = Worksheets(sheetName)

    ' 定義の最終行を取得
    Dim lastRow As Long
    lastRow = ws.Cells(ws.Rows.Count, DEFINE_COL).End(xlUp).Row

    Dim r As Long
    For r = DEFINE_BEGINNING_ROW To lastRow
        If CStr(ws.Cells(r, DEFINE_COL).Value) = value Then
            IsDefined = True
            Exit Function
        End If
    Next r

    IsDefined = False

End Function


' 分類のソート番号を取得する関数（CategoryDefine の並び順）
Public Function GetCategorySortOrder(category As String) As Long

    Dim ws As Worksheet
    Set ws = Worksheets(CATEGORY_DEFINE_SHEET_NAME)

    ' 定義の最終行を取得
    Dim lastRow As Long
    lastRow = ws.Cells(ws.Rows.Count, DEFINE_COL).End(xlUp).Row

    ' 定義行を探索
    Dim r As Long
    For r = DEFINE_BEGINNING_ROW To lastRow

        ' 一致したらその順番を返す（先頭を0とする）
        If CStr(ws.Cells(r, DEFINE_COL).Value) = category Then
            GetCategorySortOrder = r - DEFINE_BEGINNING_ROW
            Exit Function
        End If

    Next r

    ' 見つからなかった場合
    GetCategorySortOrder = 99

End Function


' 0以上の整数か
Public Function IsNonNegativeInteger(v As Variant) As Boolean

    IsNonNegativeInteger = False

    If IsEmpty(v) Or CStr(v) = "" Then Exit Function
    If Not IsNumeric(v) Then Exit Function
    If CDbl(v) < 0 Then Exit Function
    If CDbl(v) <> Fix(CDbl(v)) Then Exit Function

    IsNonNegativeInteger = True

End Function


' 0以上の数値か
Public Function IsNonNegativeNumber(v As Variant) As Boolean

    IsNonNegativeNumber = False

    If IsEmpty(v) Or CStr(v) = "" Then Exit Function
    If Not IsNumeric(v) Then Exit Function
    If CDbl(v) < 0 Then Exit Function

    IsNonNegativeNumber = True

End Function


' サウンドテーブル(CSV)のID一覧を読み込む（見つからなければ Nothing）
Public Function LoadSoundIds() As Object

    Set LoadSoundIds = Nothing

    Dim filePath As String
    filePath = ThisWorkbook.Path & SOUND_TABLE_PATH

    If Dir(filePath) = "" Then Exit Function

    Dim dict As Object
    Set dict = CreateObject("Scripting.Dictionary")

    Dim f As Integer
    f = FreeFile

    Open filePath For Input As #f

    Dim textLine As String
    Do While Not EOF(f)
        Line Input #f, textLine
        If textLine <> "" Then
            Dim id As String
            id = Split(textLine, ",")(0)
            If Not dict.Exists(id) Then dict.Add id, True
        End If
    Loop

    Close #f

    Set LoadSoundIds = dict

End Function
