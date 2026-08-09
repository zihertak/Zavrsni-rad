object form_korisnik_unos: Tform_korisnik_unos
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Korisnik'
  ClientHeight = 500
  ClientWidth = 520
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poScreenCenter
  TextHeight = 15
  object group_osnovni: TGroupBox
    Left = 8
    Top = 8
    Width = 497
    Height = 409
    Caption = 'Podaci o korisniku'
    Color = 11064738
    ParentBackground = False
    ParentColor = False
    TabOrder = 0
    object label_ime: TLabel
      Left = 32
      Top = 48
      Width = 20
      Height = 15
      Caption = 'Ime'
    end
    object label_prezime: TLabel
      Left = 32
      Top = 96
      Width = 42
      Height = 15
      Caption = 'Prezime'
    end
    object label_username: TLabel
      Left = 32
      Top = 152
      Width = 78
      Height = 17
      Caption = 'Korisni'#269'ko ime'#10
    end
    object label_lozinka: TLabel
      Left = 32
      Top = 208
      Width = 40
      Height = 15
      Caption = 'Lozinka'
    end
    object label_ponovi_lozinku: TLabel
      Left = 32
      Top = 264
      Width = 88
      Height = 15
      Caption = 'Ponovite lozinku'
    end
    object label_uloga_unos: TLabel
      Left = 32
      Top = 320
      Width = 31
      Height = 15
      Caption = 'Uloga'
    end
    object label_skupina: TLabel
      Left = 32
      Top = 368
      Width = 42
      Height = 15
      Caption = 'Skupina'
    end
    object edit_ime: TEdit
      Left = 176
      Top = 45
      Width = 201
      Height = 23
      TabOrder = 0
    end
    object edit_prezime: TEdit
      Left = 176
      Top = 93
      Width = 201
      Height = 23
      TabOrder = 1
    end
    object edit_username: TEdit
      Left = 176
      Top = 149
      Width = 201
      Height = 23
      TabOrder = 2
    end
    object edit_lozinka: TEdit
      Left = 176
      Top = 205
      Width = 201
      Height = 23
      PasswordChar = '*'
      TabOrder = 3
    end
    object edit_ponovi_lozinku: TEdit
      Left = 176
      Top = 261
      Width = 201
      Height = 23
      PasswordChar = '*'
      TabOrder = 4
    end
    object combo_uloga_unos: TComboBox
      Left = 176
      Top = 312
      Width = 201
      Height = 23
      Style = csDropDownList
      TabOrder = 5
      OnChange = combo_uloga_unosChange
    end
    object combo_skupina: TComboBox
      Left = 176
      Top = 365
      Width = 201
      Height = 23
      Style = csDropDownList
      TabOrder = 6
    end
  end
  object button_spremi: TButton
    Left = 120
    Top = 448
    Width = 75
    Height = 25
    Caption = 'Spremi'
    Default = True
    TabOrder = 1
    OnClick = button_spremiClick
  end
  object button_odustani: TButton
    Left = 254
    Top = 448
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Odustani'
    ModalResult = 2
    TabOrder = 2
    OnClick = button_odustaniClick
  end
  object query_spremanje: TFDQuery
    Connection = data_module.connection
    Left = 464
    Top = 88
  end
  object query_korisnik: TFDQuery
    Connection = data_module.connection
    Left = 472
    Top = 184
  end
  object query_uloge: TFDQuery
    Connection = data_module.connection
    Left = 464
    Top = 256
  end
  object query_skupine: TFDQuery
    Connection = data_module.connection
    Left = 472
    Top = 344
  end
  object query_provjera: TFDQuery
    Connection = data_module.connection
    Left = 480
    Top = 432
  end
end
