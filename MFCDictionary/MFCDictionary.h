
// MFCDictionary.h: основной файл заголовка для приложения MFCDictionary
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CMFCDictionaryApp:
// Сведения о реализации этого класса: MFCDictionary.cpp
//

class CMFCDictionaryApp : public CWinApp
{
public:
	CMFCDictionaryApp();


// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Реализация
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFCDictionaryApp theApp;
