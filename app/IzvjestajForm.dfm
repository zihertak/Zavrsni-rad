object form_izvjestaj: Tform_izvjestaj
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Izvje'#353'taj po skupinama'
  ClientHeight = 560
  ClientWidth = 920
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poMainFormCenter
  OnShow = FormShow
  TextHeight = 15
  object label_naslov: TLabel
    Left = 24
    Top = 12
    Width = 300
    Height = 38
    Caption = 'Izvje'#353'taj po skupinama'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -26
    Font.Name = 'Segoe UI'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object label_opis: TLabel
    Left = 26
    Top = 52
    Width = 280
    Height = 22
    Caption = 'Pregled djece i razvoja po skupinama'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = -16
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
  end
  object label_graf_djece: TLabel
    Left = 24
    Top = 84
    Width = 122
    Height = 17
    Alignment = taCenter
    Caption = 'Broj djece po skupini'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = -13
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
  end
  object label_graf_razine: TLabel
    Left = 466
    Top = 84
    Width = 172
    Height = 17
    Alignment = taCenter
    Caption = 'Prosje'#269'na razina razvoja (1-5)'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = -13
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
  end
  object paint_broj_djece: TPaintBox
    Left = 24
    Top = 104
    Width = 430
    Height = 130
    OnPaint = paint_broj_djecePaint
  end
  object paint_razina_razvoja: TPaintBox
    Left = 466
    Top = 104
    Width = 430
    Height = 130
    OnPaint = paint_razina_razvojaPaint
  end
  object memo_izvjestaj: TMemo
    Left = 24
    Top = 244
    Width = 872
    Height = 250
    Color = 14020857
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Consolas'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 2
  end
  object button_pdf: TButton
    Left = 24
    Top = 506
    Width = 160
    Height = 35
    Caption = 'Izvezi u PDF'
    TabOrder = 0
    OnClick = button_pdfClick
  end
  object button_zatvori: TButton
    Left = 736
    Top = 506
    Width = 160
    Height = 35
    Caption = 'Zatvori'
    TabOrder = 1
    OnClick = button_zatvoriClick
  end
  object query_izv_skupine: TFDQuery
    Connection = data_module.connection
    SQL.Strings = (
      'SELECT'
      '    id_skupina,'
      '    naziv,'
      '    pedagoska_godina,'
      '    dob_od,'
      '    dob_do'
      'FROM skupina'
      'ORDER BY naziv ASC')
    Left = 64
    Top = 400
  end
  object query_izv_djeca: TFDQuery
    Connection = data_module.connection
    Left = 128
    Top = 400
  end
  object query_izv_procjene: TFDQuery
    Connection = data_module.connection
    Left = 192
    Top = 400
  end
  object query_izv_odgojitelj: TFDQuery
    Connection = data_module.connection
    Left = 256
    Top = 400
  end
end
