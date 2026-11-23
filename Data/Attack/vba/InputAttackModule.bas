Attribute VB_Name = "InputAttackModule"
Option Explicit


' 1項目を入力してもらう（キャンセルされたら False）
Private Function AskText(prompt As String, title As String, defaultValue As String, ByRef result As String) As Boolean

    Dim s As String
    s = InputBox(prompt, title, defaultValue)

    ' キャンセル（×ボタン含む）
    If StrPtr(s) = 0 Then
        AskText = False
        Exit Function
    End If

    result = Trim(s)
    AskText = True

End Function


' 定義シートの一覧を「 / 」区切りの文字列にする（入力の案内用）
Private Function DefineListText(sheetName As String) As String

    Dim ws As Worksheet
    Set ws = Worksheets(sheetName)

    Dim lastRow As Long
    lastRow = ws.Cells(ws.Rows.Count, DEFINE_COL).End(xlUp).Row

    Dim r As Long
    Dim text As String
    For r = DEFINE_BEGINNING_ROW To lastRow
        If CStr(ws.Cells(r, DEFINE_COL).Value) <> "" Then
            If text <> "" Then text = text & " / "
            text = text & ws.Cells(r, DEFINE_COL).Value
        End If
    Next r

    DefineListText = text

End Function


' 指定行以外に同じIDがあるか
Private Function IsIdUsed(ws As Worksheet, id As String, exceptRow As Long) As Boolean

    Dim lastRow As Long
    lastRow = ws.Cells(ws.Rows.Count, ID_COL).End(xlUp).Row

    Dim r As Long
    For r = DATA_BEGINNING_ROW To lastRow
        If r <> exceptRow And CStr(ws.Cells(r, ID_COL).Value) = id Then
            IsIdUsed = True
            Exit Function
        End If
    Next r

    IsIdUsed = False

End Function


' 攻撃パラメータを順番に入力して、指定行に書き込む
' isNew : True=追加（初期値で入力開始） False=編集（その行の値で入力開始）
' 戻り値 : 書き込んだら True、途中でキャンセルしたら False（何も書き込まない）
Public Function InputAttack(ByVal targetRow As Long, ByVal isNew As Boolean) As Boolean

    InputAttack = False

    Dim ws As Worksheet
    Set ws = Worksheets(TABLE_SHEET_NAME)

    Dim title As String
    If isNew Then title = "攻撃の追加" Else title = "攻撃の編集"

    ' 入力開始時の値～～～～～～～～～～～～～～～～～～～～

    Dim id As String, element As String, damage As String, stagger As String
    Dim hitSE As String, knockback As String, category As String

    If isNew Then
        id = ""
        element = "Physics"
        damage = "10"
        stagger = "0"
        hitSE = ""
        knockback = "0"
        category = "Player"
    Else
        id = CStr(ws.Cells(targetRow, ID_COL).Value)
        element = CStr(ws.Cells(targetRow, ELEMENT_COL).Value)
        damage = CStr(ws.Cells(targetRow, DAMAGE_COL).Value)
        stagger = CStr(ws.Cells(targetRow, STAGGER_COL).Value)
        hitSE = CStr(ws.Cells(targetRow, HITSE_COL).Value)
        knockback = CStr(ws.Cells(targetRow, KNOCKBACK_COL).Value)
        category = CStr(ws.Cells(targetRow, CATEGORY_COL).Value)
    End If

    ' ～～～～～～～～～～～～～～～～～～～～入力開始時の値

    Dim s As String

    ' ID
    Do
        If Not AskText("ID を入力してください（コード側で探す名前）", title, id, s) Then Exit Function
        If s = "" Then
            MsgBox "IDが未入力です"
        ElseIf InStr(s, ",") > 0 Then
            MsgBox "IDにカンマは使えません"
        ElseIf IsIdUsed(ws, s, IIf(isNew, 0, targetRow)) Then
            MsgBox "ID「" & s & "」はすでに使われています"
        Else
            id = s
            Exit Do
        End If
        id = s
    Loop

    ' 属性
    Do
        If Not AskText("属性を入力してください" & vbCrLf & DefineListText(ELEMENT_DEFINE_SHEET_NAME), title, element, s) Then Exit Function
        If IsDefined(ELEMENT_DEFINE_SHEET_NAME, s) Then
            element = s
            Exit Do
        End If
        MsgBox "属性は次の中から入力してください" & vbCrLf & DefineListText(ELEMENT_DEFINE_SHEET_NAME)
    Loop

    ' ダメージ量
    Do
        If Not AskText("ダメージ量（0以上の整数）", title, damage, s) Then Exit Function
        If IsNonNegativeInteger(s) Then
            damage = s
            Exit Do
        End If
        MsgBox "0以上の整数で入力してください"
    Loop

    ' 怯み値
    Do
        If Not AskText("怯み値（0以上の整数）", title, stagger, s) Then Exit Function
        If IsNonNegativeInteger(s) Then
            stagger = s
            Exit Do
        End If
        MsgBox "0以上の整数で入力してください"
    Loop

    ' HIT SE（空欄OK）
    Dim soundIds As Object
    Set soundIds = LoadSoundIds()
    Do
        If Not AskText("HIT SE（SoundTable の ID・空欄で鳴らさない）", title, hitSE, s) Then Exit Function
        If InStr(s, ",") > 0 Then
            MsgBox "カンマは使えません"
        ElseIf s <> "" And Not soundIds Is Nothing Then
            If soundIds.Exists(s) Then
                hitSE = s
                Exit Do
            End If
            If MsgBox("「" & s & "」は SoundTable.csv にありません。このまま登録しますか？", vbYesNo + vbExclamation) = vbYes Then
                hitSE = s
                Exit Do
            End If
        Else
            hitSE = s
            Exit Do
        End If
        hitSE = s
    Loop

    ' ノックバック
    Do
        If Not AskText("ノックバックの強さ（0以上・0で無し）", title, knockback, s) Then Exit Function
        If IsNonNegativeNumber(s) Then
            knockback = s
            Exit Do
        End If
        MsgBox "0以上の数値で入力してください"
    Loop

    ' 分類
    Do
        If Not AskText("分類を入力してください" & vbCrLf & DefineListText(CATEGORY_DEFINE_SHEET_NAME), title, category, s) Then Exit Function
        If IsDefined(CATEGORY_DEFINE_SHEET_NAME, s) Then
            category = s
            Exit Do
        End If
        MsgBox "分類は次の中から入力してください" & vbCrLf & DefineListText(CATEGORY_DEFINE_SHEET_NAME)
    Loop


    ' すべて入力できたら書き込む～～～～～～～～～～～～～～～～～

    ws.Cells(targetRow, ID_COL).Value = id
    ws.Cells(targetRow, ID_COL).HorizontalAlignment = xlLeft

    ws.Cells(targetRow, ELEMENT_COL).Value = element
    ws.Cells(targetRow, ELEMENT_COL).HorizontalAlignment = xlCenter

    ws.Cells(targetRow, DAMAGE_COL).Value = CLng(damage)
    ws.Cells(targetRow, DAMAGE_COL).HorizontalAlignment = xlCenter

    ws.Cells(targetRow, STAGGER_COL).Value = CLng(stagger)
    ws.Cells(targetRow, STAGGER_COL).HorizontalAlignment = xlCenter

    ws.Cells(targetRow, HITSE_COL).Value = hitSE
    ws.Cells(targetRow, HITSE_COL).HorizontalAlignment = xlCenter

    ws.Cells(targetRow, KNOCKBACK_COL).Value = CDbl(knockback)
    ws.Cells(targetRow, KNOCKBACK_COL).HorizontalAlignment = xlCenter

    ws.Cells(targetRow, CATEGORY_COL).Value = category
    ws.Cells(targetRow, CATEGORY_COL).HorizontalAlignment = xlCenter

    ws.Cells(targetRow, SORT_COL).Value = GetCategorySortOrder(category)

    ' ～～～～～～～～～～～～～～～～～すべて入力できたら書き込む

    InputAttack = True

End Function
