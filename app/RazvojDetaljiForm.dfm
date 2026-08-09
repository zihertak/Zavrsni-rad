object form_razvoj_detalji: Tform_razvoj_detalji
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Detalji razvojnog pregleda'
  ClientHeight = 470
  ClientWidth = 620
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poMainFormCenter
  TextHeight = 15
  object group_pregled: TGroupBox
    Left = 24
    Top = 8
    Width = 588
    Height = 390
    Caption = 'Razvojni pregled'#10
    TabOrder = 0
    object label_dijete_naslov: TLabel
      Left = 24
      Top = 35
      Width = 33
      Height = 15
      Caption = 'Dijete:'
    end
    object label_dijete: TLabel
      Left = 120
      Top = 35
      Width = 5
      Height = 15
      Caption = '-'
    end
    object label_datum_naslov: TLabel
      Left = 24
      Top = 60
      Width = 39
      Height = 15
      Caption = 'Datum:'
    end
    object label_datum: TLabel
      Left = 120
      Top = 60
      Width = 5
      Height = 15
      Caption = '-'
    end
    object label_odgojitelj_naslov: TLabel
      Left = 24
      Top = 85
      Width = 55
      Height = 15
      Caption = 'Odgojitelj:'
    end
    object label_odgojitelj: TLabel
      Left = 120
      Top = 85
      Width = 5
      Height = 15
      Caption = '-'
    end
    object label_tjelesni: TLabel
      Left = 24
      Top = 140
      Width = 155
      Height = 15
      Caption = 'Tjelesni i psihomotorni razvoj'
    end
    object label_tjelesni_zvjezdice: TLabel
      Left = 300
      Top = 140
      Width = 85
      Height = 28
      Caption = #9734#9734#9734#9734#9734
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
    end
    object label_socio: TLabel
      Left = 24
      Top = 175
      Width = 134
      Height = 15
      Caption = 'Socio-emocionalni razvoj'
    end
    object label_socio_zvjezdice: TLabel
      Left = 300
      Top = 174
      Width = 85
      Height = 28
      Caption = #9734#9734#9734#9734#9734
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
    end
    object label_govorni: TLabel
      Left = 24
      Top = 210
      Width = 76
      Height = 15
      Caption = 'Govorni razvoj'
    end
    object label_govorni_zvjezdice: TLabel
      Left = 300
      Top = 210
      Width = 85
      Height = 28
      Caption = #9734#9734#9734#9734#9734
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
    end
    object label_spoznajni: TLabel
      Left = 24
      Top = 245
      Width = 85
      Height = 15
      Caption = 'Spoznajni razvoj'
    end
    object label_spoznajni_zvjezdice: TLabel
      Left = 300
      Top = 245
      Width = 85
      Height = 28
      Caption = #9734#9734#9734#9734#9734
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
    end
    object label_napomena: TLabel
      Left = 24
      Top = 290
      Width = 62
      Height = 15
      Caption = 'Napomena:'
    end
    object memo_napomena: TMemo
      Left = 24
      Top = 315
      Width = 540
      Height = 55
      TabStop = False
      ReadOnly = True
      ScrollBars = ssVertical
      TabOrder = 0
    end
  end
  object button_zatvori: TButton
    Left = 244
    Top = 420
    Width = 110
    Height = 34
    Caption = 'Zatvori'
    Default = True
    ModalResult = 8
    TabOrder = 1
  end
  object query_detalji: TFDQuery
    Connection = data_module.connection
    Left = 496
    Top = 128
  end
end
