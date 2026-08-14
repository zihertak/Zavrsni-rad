object form_najava_unos: Tform_najava_unos
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Dodaj / Uredi najavu'
  ClientHeight = 350
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
    Height = 250
    Caption = 'Podaci o najavi'
    Color = 11064738
    ParentBackground = False
    ParentColor = False
    TabOrder = 0
    object label_datum: TLabel
      Left = 24
      Top = 35
      Width = 34
      Height = 15
      Caption = 'Datum'
    end
    object label_naziv: TLabel
      Left = 24
      Top = 80
      Width = 32
      Height = 15
      Caption = 'Naziv'
    end
    object label_vrsta: TLabel
      Left = 24
      Top = 125
      Width = 32
      Height = 15
      Caption = 'Vrsta'
    end
    object label_opis: TLabel
      Left = 24
      Top = 170
      Width = 25
      Height = 15
      Caption = 'Opis'
    end
    object date_datum: TDateTimePicker
      Left = 136
      Top = 35
      Width = 265
      Height = 23
      Date = 46227.000000000000000000
      Time = 0.500000000000000000
      TabOrder = 0
    end
    object edit_naziv: TEdit
      Left = 136
      Top = 80
      Width = 265
      Height = 23
      TabOrder = 1
    end
    object combo_vrsta: TComboBox
      Left = 136
      Top = 125
      Width = 265
      Height = 23
      Style = csDropDownList
      TabOrder = 2
      Items.Strings = (
        'Doga'#273'aj'
        'Zatvaranje vrti'#263'a'
        'Podsjetnik'
        'Roditeljski sastanak')
    end
    object memo_opis: TMemo
      Left = 136
      Top = 170
      Width = 265
      Height = 60
      ScrollBars = ssVertical
      TabOrder = 3
    end
  end
  object button_spremi: TButton
    Left = 128
    Top = 290
    Width = 121
    Height = 41
    Caption = 'Spremi'
    Default = True
    TabOrder = 1
    OnClick = button_spremiClick
  end
  object button_odustani: TButton
    Left = 280
    Top = 290
    Width = 121
    Height = 41
    Cancel = True
    Caption = 'Odustani'
    ModalResult = 2
    TabOrder = 2
  end
end
