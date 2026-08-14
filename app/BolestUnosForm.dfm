object form_bolest_unos: Tform_bolest_unos
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Dodaj / Uredi zapis o bolesti'
  ClientHeight = 360
  ClientWidth = 600
  Color = clWhite
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poScreenCenter
  TextHeight = 15
  object group_podaci: TGroupBox
    Left = 24
    Top = 8
    Width = 537
    Height = 260
    Caption = 'Podaci o bolesti'
    Color = 11064738
    ParentBackground = False
    ParentColor = False
    TabOrder = 0
    object label_dijete: TLabel
      Left = 24
      Top = 35
      Width = 34
      Height = 15
      Caption = 'Dijete'
    end
    object label_od: TLabel
      Left = 24
      Top = 80
      Width = 15
      Height = 15
      Caption = 'Od'
    end
    object label_do: TLabel
      Left = 24
      Top = 125
      Width = 13
      Height = 15
      Caption = 'Do'
    end
    object label_razlog: TLabel
      Left = 24
      Top = 170
      Width = 40
      Height = 15
      Caption = 'Razlog'
    end
    object label_napomena: TLabel
      Left = 24
      Top = 215
      Width = 62
      Height = 15
      Caption = 'Napomena'
    end
    object combo_dijete: TComboBox
      Left = 136
      Top = 35
      Width = 265
      Height = 23
      Style = csDropDownList
      TabOrder = 0
    end
    object date_od: TDateTimePicker
      Left = 136
      Top = 80
      Width = 265
      Height = 23
      Date = 46227.000000000000000000
      Time = 0.500000000000000000
      TabOrder = 1
    end
    object date_do: TDateTimePicker
      Left = 136
      Top = 125
      Width = 265
      Height = 23
      Date = 46227.000000000000000000
      Time = 0.500000000000000000
      TabOrder = 2
    end
    object combo_razlog: TComboBox
      Left = 136
      Top = 170
      Width = 265
      Height = 23
      Style = csDropDownList
      TabOrder = 3
      Items.Strings = (
        'Bolest'
        'Obiteljski razlozi'
        'Ostalo')
    end
    object edit_napomena: TEdit
      Left = 136
      Top = 215
      Width = 265
      Height = 23
      TabOrder = 4
    end
  end
  object button_spremi: TButton
    Left = 128
    Top = 300
    Width = 121
    Height = 41
    Caption = 'Spremi'
    Default = True
    TabOrder = 1
    OnClick = button_spremiClick
  end
  object button_odustani: TButton
    Left = 280
    Top = 300
    Width = 121
    Height = 41
    Cancel = True
    Caption = 'Odustani'
    ModalResult = 2
    TabOrder = 2
  end
  object query_djeca: TFDQuery
    Connection = data_module.connection
    SQL.Strings = (
      'SELECT id_dijete, ime, prezime'
      'FROM dijete'
      'ORDER BY prezime, ime')
    Left = 456
    Top = 16
  end
end
