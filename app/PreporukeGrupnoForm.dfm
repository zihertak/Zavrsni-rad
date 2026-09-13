object form_preporuke_grupno: Tform_preporuke_grupno
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Grupno generiranje preporuka'
  ClientHeight = 420
  ClientWidth = 500
  Color = clWhite
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poScreenCenter
  TextHeight = 15
  object label_naslov: TLabel
    Left = 24
    Top = 16
    Width = 231
    Height = 15
    Caption = 'Generiranje preporuka za odabranu djecu'
  end
  object label_napredak: TLabel
    Left = 24
    Top = 370
    Width = 89
    Height = 15
    Caption = 'Gotovo 0 / 0'
  end
  object list_status: TListBox
    Left = 24
    Top = 45
    Width = 452
    Height = 315
    ItemHeight = 15
    TabOrder = 0
  end
  object button_zatvori: TButton
    Left = 380
    Top = 365
    Width = 96
    Height = 32
    Caption = 'Zatvori'
    Enabled = False
    TabOrder = 1
    OnClick = button_zatvoriClick
  end
end
