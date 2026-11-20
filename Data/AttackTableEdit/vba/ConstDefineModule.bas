Attribute VB_Name = "ConstDefineModule"
Option Explicit


' シート名の定数定義～～～～～～～～～～～～～～～～～

' 攻撃テーブルのシート名
Public Const TABLE_SHEET_NAME As String = "AttackTable"

' 属性定義のシート名
Public Const ELEMENT_DEFINE_SHEET_NAME As String = "ElementDefine"

' 分類定義のシート名
Public Const CATEGORY_DEFINE_SHEET_NAME As String = "CategoryDefine"

' ～～～～～～～～～～～～～～～～～シート名の定数定義



' 各要素行番号の定数定義～～～～～～～～～～～～～～

' 操作マクロ配置用の行番号
Public Const OPERATION_ROW As Long = 1

' ヘッダーの行番号
Public Const HEADER_ROW As Long = OPERATION_ROW + 1

' データ格納の最初の1行目の行番号
Public Const DATA_BEGINNING_ROW As Long = HEADER_ROW + 1

' ～～～～～～～～～～～～～～各要素行番号の定数定義



' 各要素列番号の定数定義～～～～～～～～～～～～～～～

' IDの列番号
Public Const ID_COL As Long = 1

' 属性の列番号
Public Const ELEMENT_COL As Long = ID_COL + 1

' ダメージ量の列番号
Public Const DAMAGE_COL As Long = ELEMENT_COL + 1

' 怯み値の列番号
Public Const STAGGER_COL As Long = DAMAGE_COL + 1

' HIT SEの列番号
Public Const HITSE_COL As Long = STAGGER_COL + 1

' ノックバックの列番号
Public Const KNOCKBACK_COL As Long = HITSE_COL + 1


' 分類の列番号（CSVには出力しない）
Public Const CATEGORY_COL As Long = 25

' ソート番号の列番号（CSVには出力しない）
Public Const SORT_COL As Long = CATEGORY_COL + 1

' ～～～～～～～～～～～～～～～各要素列番号の定数定義



' 定義シートの共通設定～～～～～～～～～～～～～～～～

' 定義場所の列番号
Public Const DEFINE_COL As Long = 1

' 定義場所の一番最初の行番号
Public Const DEFINE_BEGINNING_ROW As Long = 2

' ～～～～～～～～～～～～～～～～定義シートの共通設定



' ファイルの設定～～～～～～～～～～～～～～～～～～～

' 出力するCSVのファイル名（このブックと同じフォルダに出力）
Public Const CSV_FILE_NAME As String = "AttackTable.csv"

' HIT SEのチェックに使うサウンドテーブル（このブックから見た相対パス）
Public Const SOUND_TABLE_PATH As String = "\..\Sound\SoundTable.csv"

' ～～～～～～～～～～～～～～～～～～～ファイルの設定
