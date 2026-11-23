Attribute VB_Name = "CheckTableModule"
Option Explicit

' ボタン用：チェックして結果を表示
Sub CheckTable()

    If CheckTableCore() Then
        MsgBox "チェックOK"
    End If

End Sub


' 全行をチェックする（問題があればメッセージを出してそのセルを選択し、False を返す）
Public Function CheckTableCore() As Boolean

    CheckTableCore = False

    Dim ws As Worksheet
    Set ws = Worksheets(TABLE_SHEET_NAME)

    Dim lastRow As Long
    lastRow = ws.Cells(ws.Rows.Count, ID_COL).End(xlUp).Row

    ' ID重複チェック用
    Dim ids As Object
    Set ids = CreateObject("Scripting.Dictionary")

    ' HIT SEチェック用（サウンドテーブルが見つからなければチェックしない）
    Dim soundIds As Object
    Set soundIds = LoadSoundIds()

    Dim r As Long
    For r = DATA_BEGINNING_ROW To lastRow

        Dim id As String
        id = CStr(ws.Cells(r, ID_COL).Value)

        ' 空行（分類の区切り）は飛ばす
        If id = "" Then GoTo NextRow

        ' ID重複
        If ids.Exists(id) Then
            Call ShowError(ws, r, ID_COL, "IDに重複があります " & ids(id) & "行 と " & r & "行")
            Exit Function
        End If
        ids.Add id, r

        ' 属性
        If Not IsDefined(ELEMENT_DEFINE_SHEET_NAME, CStr(ws.Cells(r, ELEMENT_COL).Value)) Then
            Call ShowError(ws, r, ELEMENT_COL, "属性が ElementDefine にありません 行:" & r)
            Exit Function
        End If

        ' ダメージ量
        If Not IsNonNegativeInteger(ws.Cells(r, DAMAGE_COL).Value) Then
            Call ShowError(ws, r, DAMAGE_COL, "ダメージ量は0以上の整数で入力してください 行:" & r)
            Exit Function
        End If

        ' 怯み値
        If Not IsNonNegativeInteger(ws.Cells(r, STAGGER_COL).Value) Then
            Call ShowError(ws, r, STAGGER_COL, "怯み値は0以上の整数で入力してください 行:" & r)
            Exit Function
        End If

        ' ノックバック
        If Not IsNonNegativeNumber(ws.Cells(r, KNOCKBACK_COL).Value) Then
            Call ShowError(ws, r, KNOCKBACK_COL, "ノックバックは0以上の数値で入力してください 行:" & r)
            Exit Function
        End If

        ' 分類
        If Not IsDefined(CATEGORY_DEFINE_SHEET_NAME, CStr(ws.Cells(r, CATEGORY_COL).Value)) Then
            Call ShowError(ws, r, CATEGORY_COL, "分類が CategoryDefine にありません 行:" & r)
            Exit Function
        End If

        ' HIT SE（空欄はOK・サウンドテーブルにあるIDか）
        Dim se As String
        se = CStr(ws.Cells(r, HITSE_COL).Value)
        If se <> "" And Not soundIds Is Nothing Then
            If Not soundIds.Exists(se) Then
                Call ShowError(ws, r, HITSE_COL, "HIT SE「" & se & "」が SoundTable.csv にありません 行:" & r)
                Exit Function
            End If
        End If

        ' カンマが入っているとCSVが崩れる
        If InStr(id, ",") > 0 Or InStr(se, ",") > 0 Then
            Call ShowError(ws, r, ID_COL, "IDやHIT SEにカンマは使えません 行:" & r)
            Exit Function
        End If

NextRow:
    Next r

    If soundIds Is Nothing Then
        MsgBox "SoundTable.csv が見つからなかったため、HIT SE のチェックは行っていません" & vbCrLf & _
               ThisWorkbook.Path & SOUND_TABLE_PATH
    End If

    CheckTableCore = True

End Function


' エラー表示してセルを選択
Private Sub ShowError(ws As Worksheet, r As Long, c As Long, message As String)

    MsgBox message
    ws.Activate
    ws.Cells(r, c).Select

End Sub
