�
 TTQSLFM 0�  TPF0TtqslFmtqslFmLeftnToptWidth�Height� CaptionTrusted QSL Font.CharsetDEFAULT_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameMS Sans Serif
Font.Style Menu	MainMenu1PositionpoScreenCenterPixelsPerInch`
TextHeight TLabelLabel1Left`TopWidth!HeightCaption:*** WARNING -- Private Keys are stored unencrypted!!!  ***Font.CharsetDEFAULT_CHARSET
Font.ColorclRedFont.Height�	Font.NameMS Sans Serif
Font.StylefsBold 
ParentFont  TLabelLabel2LeftqTop0Width�HeightCaptionIThis is currently proof of concept and should NOT be used for Trusted CA Font.CharsetDEFAULT_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameMS Sans Serif
Font.StylefsBold 
ParentFont  TLabelLabel3Left� TopPWidth7HeightCaption- until private key encryption has been added.Font.CharsetDEFAULT_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameMS Sans Serif
Font.StylefsBold 
ParentFont  TBitBtnBitBtn1LeftKTopxWidthKHeightTabOrder KindbkClose  	TMainMenu	MainMenu1LeftTop 	TMenuItemFile1Caption&File 	TMenuItemImportPrivateKey1CaptionImport Private Key  	TMenuItemImportPublicKey1CaptionImport Public Key  	TMenuItemImportCertificate1CaptionImport CertificateOnClickImportCertificate1Click  	TMenuItemImportPublicKey2CaptionImport Public Key  	TMenuItemExit1CaptionE&xit   	TMenuItem
KeyCreate1CaptionKeys 	TMenuItemCreateNewKeyPair1CaptionCreate New Key PairOnClickCreateNewKeyPair1Click  	TMenuItem	ListKeys1Caption	List Keys  	TMenuItemSign1CaptionSign   	TMenuItemCAFunctions1CaptionCA Functions. 	TMenuItemSignacertificate1CaptionSign a CertificateOnClickSignacertificate1Click    TOpenDialogopenDlgLeftTop@  TTable
forCertTblActive	DatabaseName
TrustedQSL	FieldDefsNamePidDataType	ftAutoInc	Precision RequiredSize  NameCallSignDataTypeftString	Precision RequiredSize NameCertNumDataTypeftFloat	Precision RequiredSize  NameCertCADataTypeftString	Precision RequiredSize NameCertTypeDataTypeftString	Precision RequiredSize NameCertDataTypeftMemo	Precision RequiredSize�  NameIssueDataTypeftDate	Precision RequiredSize  NameExpiresDataTypeftDate	Precision RequiredSize   	StoreDefs		TableNameforcerts.DBLeftxTop` TAutoIncFieldforCertTblPid	FieldNamePid  TStringFieldforCertTblCallSign	FieldNameCallSignSize  TFloatFieldforCertTblCertNum	FieldNameCertNum  TStringFieldforCertTblCertCA	FieldNameCertCA  TStringFieldforCertTblCertType	FieldNameCertTypeSize  
TMemoFieldforCertTblCert	FieldNameCertBlobTypeftMemoSize�   
TDateFieldforCertTblIssue	FieldNameIssue  
TDateFieldforCertTblExpires	FieldNameExpires   TTablecertTblDatabaseName
TrustedQSL	TableNamecerts.DBLeftPToph  TTablepubTblActive	DatabaseName
TrustedQSL	FieldDefsNameKidDataType	ftAutoInc	Precision RequiredSize  Name	PublicKeyDataTypeftString	Precision RequiredSize�  NameCallSignDataTypeftString	Precision RequiredSize Name	PubkeyNumDataTypeftString	Precision RequiredSize NameSignedDataType	ftBoolean	Precision RequiredSize   	StoreDefs		TableName
pubkeys.dbLeft�Toph TAutoIncField	pubTblKid	FieldNameKid  TStringFieldpubTblPublicKey	FieldName	PublicKeySize�   TStringFieldpubTblCallSign	FieldNameCallSignSize  TStringFieldpubTblPubkeyNum	FieldName	PubkeyNumSize   TTablePrivTblActive	DatabaseName
TrustedQSL	FieldDefsNamePidDataType	ftAutoInc	Precision RequiredSize  NameCallSignDataTypeftString	Precision RequiredSize NameKeyNumDataType	ftInteger	Precision RequiredSize  NameKeyDataTypeftString	Precision RequiredSize�   	StoreDefs		TableNameprivkeys.DBLeft� Topp  TDataSourceprivDSDataSetPrivTblLeftTopx  TDataSourcepubDSDataSetpubTblLeft�Topp  TDataSourcecertDsDataSetcertTblLeft�Top`   