object form_dijete_unos: Tform_dijete_unos
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Dodaj / Uredi dijete'
  ClientHeight = 633
  ClientWidth = 600
  Color = clWhite
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poScreenCenter
  TextHeight = 15
  object group_osnovni: TGroupBox
    Left = 24
    Top = 8
    Width = 537
    Height = 345
    Caption = 'Osnovni podaci'
    Color = 11064738
    ParentBackground = False
    ParentColor = False
    TabOrder = 0
    object label_ime: TLabel
      Left = 24
      Top = 35
      Width = 20
      Height = 15
      Caption = 'Ime'
    end
    object label_prezime: TLabel
      Left = 24
      Top = 80
      Width = 42
      Height = 15
      Caption = 'Prezime'
    end
    object label_datum_rodjenja: TLabel
      Left = 24
      Top = 125
      Width = 79
      Height = 15
      Caption = 'Datum ro'#273'enja'
    end
    object label_spol: TLabel
      Left = 24
      Top = 170
      Width = 23
      Height = 15
      Caption = 'Spol'
    end
    object label_datum_upisa: TLabel
      Left = 24
      Top = 215
      Width = 67
      Height = 15
      Caption = 'Datum upisa'
    end
    object label_kontakt_roditelja: TLabel
      Left = 24
      Top = 260
      Width = 87
      Height = 15
      Caption = 'Kontakt roditelja'
    end
    object label_skupina: TLabel
      Left = 24
      Top = 305
      Width = 42
      Height = 15
      Caption = 'Skupina'
    end
    object image_slika: TImage
      Left = 424
      Top = 30
      Width = 100
      Height = 100
      Center = True
      Proportional = True
      Stretch = True
    end
    object edit_ime: TEdit
      Left = 136
      Top = 35
      Width = 265
      Height = 23
      TabOrder = 0
    end
    object edit_prezime: TEdit
      Left = 136
      Top = 80
      Width = 265
      Height = 23
      TabOrder = 1
    end
    object date_datum_rodjenja: TDateTimePicker
      Left = 136
      Top = 125
      Width = 265
      Height = 23
      Date = 46227.000000000000000000
      Time = 0.545279293983185200
      TabOrder = 2
    end
    object combo_spol: TComboBox
      Left = 136
      Top = 170
      Width = 265
      Height = 23
      Style = csDropDownList
      TabOrder = 3
      Items.Strings = (
        'M'
        #381)
    end
    object date_datum_upisa: TDateTimePicker
      Left = 136
      Top = 215
      Width = 265
      Height = 23
      Date = 46227.000000000000000000
      Time = 0.547750555553648200
      TabOrder = 4
    end
    object edit_kontakt_roditelja: TEdit
      Left = 136
      Top = 260
      Width = 265
      Height = 23
      TabOrder = 5
    end
    object combo_skupina: TComboBox
      Left = 136
      Top = 305
      Width = 265
      Height = 23
      Style = csDropDownList
      TabOrder = 6
    end
    object button_ucitajSliku: TButton
      Left = 424
      Top = 140
      Width = 100
      Height = 30
      Caption = 'U'#269'itaj sliku'
      TabOrder = 7
      OnClick = button_ucitajSlikuClick
    end
    object button_ukloniSliku: TButton
      Left = 424
      Top = 176
      Width = 100
      Height = 30
      Caption = 'Ukloni sliku'
      TabOrder = 8
      OnClick = button_ukloniSlikuClick
    end
    object button_uvezi_profil: TButton
      Left = 424
      Top = 220
      Width = 108
      Height = 60
      Caption = 'Uvezi razvojni profil'
      TabOrder = 9
      OnClick = button_uvezi_profilClick
    end
  end
  object group_zdravstveni: TGroupBox
    Left = 24
    Top = 359
    Width = 537
    Height = 210
    Caption = 'Zdravstveni podaci'
    Color = 11064738
    ParentBackground = False
    ParentColor = False
    TabOrder = 1
    object label_alergije: TLabel
      Left = 24
      Top = 35
      Width = 40
      Height = 15
      Caption = 'Alergije'
    end
    object label_posebne_potrebe: TLabel
      Left = 24
      Top = 90
      Width = 89
      Height = 15
      Caption = 'Posebne potrebe'
    end
    object label_zdravstvene_napomene: TLabel
      Left = 24
      Top = 145
      Width = 124
      Height = 15
      Caption = 'Zdravstvene napomene'
    end
    object memo_alergije: TMemo
      Left = 168
      Top = 35
      Width = 297
      Height = 45
      ScrollBars = ssVertical
      TabOrder = 0
    end
    object memo_posebne_potrebe: TMemo
      Left = 168
      Top = 90
      Width = 297
      Height = 45
      ScrollBars = ssVertical
      TabOrder = 1
    end
    object memo_zdravstvene_napomene: TMemo
      Left = 168
      Top = 145
      Width = 297
      Height = 45
      ScrollBars = ssVertical
      TabOrder = 2
    end
  end
  object button_spremi: TButton
    Left = 128
    Top = 585
    Width = 121
    Height = 41
    Caption = 'Spremi'
    Default = True
    TabOrder = 2
    OnClick = button_spremiClick
  end
  object button_odustani: TButton
    Left = 280
    Top = 585
    Width = 121
    Height = 41
    Cancel = True
    Caption = 'Odustani'
    ModalResult = 2
    TabOrder = 3
  end
  object query_skupine: TFDQuery
    Connection = data_module.connection
    SQL.Strings = (
      'SELECT id_skupina, naziv'
      'FROM skupina'
      'ORDER BY naziv')
    Left = 24
    Top = 600
  end
  object query_spremanje: TFDQuery
    Connection = data_module.connection
    Left = 120
    Top = 600
  end
  object query_dijete: TFDQuery
    Connection = data_module.connection
    Left = 216
    Top = 600
  end
  object open_dialog_slika: TOpenDialog
    Filter = 'Slike|*.jpg;*.jpeg;*.png'
    Left = 312
    Top = 600
  end
  object open_dialog_profil: TOpenDialog
    Filter = 'Razvojni profil (*.dat)|*.dat'
    Left = 376
    Top = 600
  end
  object query_uvoz_pregled: TFDQuery
    Connection = data_module.connection
    Left = 440
    Top = 600
  end
  object query_uvoz_procjena: TFDQuery
    Connection = data_module.connection
    Left = 504
    Top = 600
  end
end
