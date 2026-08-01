object form_Login: Tform_Login
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Prijava'
  ClientHeight = 561
  ClientWidth = 884
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poScreenCenter
  OnShow = FormShow
  TextHeight = 15
  object panel_main: TPanel
    Left = 0
    Top = 0
    Width = 884
    Height = 561
    Align = alClient
    Color = clGradientActiveCaption
    ParentBackground = False
    TabOrder = 0
    ExplicitWidth = 878
    ExplicitHeight = 544
    DesignSize = (
      884
      561)
    object panel_header: TPanel
      Left = 1
      Top = 1
      Width = 882
      Height = 100
      Align = alTop
      Color = clGradientActiveCaption
      ParentBackground = False
      TabOrder = 0
      ExplicitWidth = 876
      object label_naslov: TLabel
        Left = 1
        Top = 1
        Width = 880
        Height = 98
        Margins.Top = 0
        Margins.Bottom = 0
        Align = alClient
        Alignment = taCenter
        AutoSize = False
        Caption = 'Naslov aplikacije'
        Color = 14339973
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -36
        Font.Name = 'Segoe UI'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
        Layout = tlCenter
        ExplicitLeft = 2
        ExplicitTop = 2
      end
    end
    object panel_login: TPanel
      Left = 232
      Top = 185
      Width = 400
      Height = 260
      Anchors = []
      Color = clGradientInactiveCaption
      ParentBackground = False
      TabOrder = 1
      ExplicitLeft = 229
      ExplicitTop = 175
      object label_username: TLabel
        Left = 40
        Top = 45
        Width = 78
        Height = 15
        Caption = 'Korisni'#269'ko ime'
      end
      object label_password: TLabel
        Left = 40
        Top = 120
        Width = 40
        Height = 15
        Caption = 'Lozinka'
      end
      object edit_username: TEdit
        Left = 40
        Top = 70
        Width = 320
        Height = 23
        TabOrder = 0
      end
      object edit_password: TEdit
        Left = 40
        Top = 145
        Width = 320
        Height = 23
        PasswordChar = '*'
        TabOrder = 1
      end
      object button_login: TButton
        Left = 140
        Top = 195
        Width = 120
        Height = 35
        Caption = 'Prijava'
        Default = True
        TabOrder = 2
        OnClick = button_loginClick
      end
    end
  end
end
