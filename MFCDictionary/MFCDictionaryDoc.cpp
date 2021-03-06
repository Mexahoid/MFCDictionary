
// MFCDictionaryDoc.cpp: реализация класса CMFCDictionaryDoc 
//

#include "stdafx.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "MFCDictionary.h"
#endif

#include "MFCDictionaryDoc.h"

#include <propkey.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <regex>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFCDictionaryDoc

IMPLEMENT_DYNCREATE(CMFCDictionaryDoc, CDocument)

BEGIN_MESSAGE_MAP(CMFCDictionaryDoc, CDocument)
	ON_BN_CLICKED(ID_OPEN_TARGET_TEXTFILE, OnLoadTargetFile)
	ON_BN_CLICKED(ID_SORT_KEY_MODE, SetDescKey)
	ON_BN_CLICKED(ID_SORT_VALUE_MODE, SetDescValue)
END_MESSAGE_MAP()


// Создание или уничтожение CMFCDictionaryDoc

CMFCDictionaryDoc::CMFCDictionaryDoc()
{
	// TODO: добавьте код для одноразового вызова конструктора
}

CMFCDictionaryDoc::~CMFCDictionaryDoc()
{

}

char tolower_rus(char c)
{
	return tolower(c, locale(""));
}

void CMFCDictionaryDoc::OnLoadTargetFile()
{
	CFileDialog fileDialog(TRUE, nullptr, L"*.txt");
	int result = fileDialog.DoModal();	//запустить диалоговое окно
	if (result == IDOK)	//если файл выбран
	{
		CFileException ex;
		CStdioFile f;
		if (f.Open(fileDialog.GetPathName(), CFile::modeNoTruncate | CFile::modeRead, &ex))
		{
			setlocale(LC_ALL, "Rus.1251");
			CString strText;
			while (f.ReadString(strText))
			{
				regex ws_re("[^A-Za-zА-Яа-я]+");
				const CT2CA strt(strText);
				const string str(strt);
				vector<string> result{
					sregex_token_iterator(str.begin(), str.end(), ws_re, -1),{}
				};

				for(string strng : result)
				{
					/// Тутай основная магия
					if (strng.size())
					{
						transform(strng.begin(), strng.end(), strng.begin(), tolower_rus);
						auto it = dictionary.find(strng);
						if (it != dictionary.end())
						{
							it->second++;
						}
						else
						{
							dictionary.insert(pair<string, int>(strng, 1));
						}
					}
				}
			}
			f.Close();
			UpdateView();
		}
		//AfxMessageBox(fileDialog.GetPathName()); // показать полный путь
	}
}

void CMFCDictionaryDoc::UpdateView()
{
	UpdateAllViews(nullptr);
}

vector<pair<string, int>> CMFCDictionaryDoc::GetDictionaryVector()
{
	const auto cmp = [](std::pair<string, int> const & a, std::pair<string, int> const & b)
	{
		return a.second != b.second ? a.second < b.second : a.first < b.first;
	};

	auto dict = dictionary;
	vector<pair<string, int>> v;
	for (const std::pair<std::string, int> pair : dict)
	{
		v.insert(v.end(), pair);
	}
	if(is_value)
		std::sort(v.begin(), v.end(), cmp);
	return v;
}

void CMFCDictionaryDoc::SetDescValue()
{
	is_value = true;
	UpdateView();
}

void CMFCDictionaryDoc::SetDescKey()
{
	is_value = false;
	UpdateView();
}

BOOL CMFCDictionaryDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: добавьте код повторной инициализации
	// (Документы SDI будут повторно использовать этот документ)

	return TRUE;
}

// Сериализация CMFCDictionaryDoc

void CMFCDictionaryDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << dictionary.size();

		for (auto it = dictionary.begin(); it != dictionary.end(); ++it)
		{
			string d = it->first;
			const CString crt = CString(d.c_str());
			ar.WriteString(crt);
			ar.WriteString(_T("\n"));

			/*const CString srt = CString(to_string(it->second).c_str());
			ar.WriteString(srt); */

			ar << it->second;
			ar.WriteString(_T("\n"));
		}
	}
	else
	{
		int f;
		ar >> f;

		for (int k = 0; k < f; k++)
		{
			CString crt;
			ar.ReadString(crt);
			const CT2CA strt(crt);
			const std::string str(strt);

			int count;
			ar >> count;
			ar.ReadString(crt);

			/*ar.ReadString(srt);
			CT2CA ctrt(srt);
			const std::string cstr(ctrt);
			int count = stoi(cstr);*/

			auto pair = std::pair<string, int>(str, count);

			dictionary.insert(pair);
		}
	}
}

#ifdef SHARED_HANDLERS

// Поддержка для эскизов
void CMFCDictionaryDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Измените этот код для отображения данных документа
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Поддержка обработчиков поиска
void CMFCDictionaryDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Задание содержимого поиска из данных документа.
	// Части содержимого должны разделяться точкой с запятой ";"

	// Например:  strSearchContent = _T("точка;прямоугольник;круг;объект ole;");
	SetSearchContent(strSearchContent);
}

void CMFCDictionaryDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Диагностика CMFCDictionaryDoc

#ifdef _DEBUG
void CMFCDictionaryDoc::AssertValid()
{
	CDocument::AssertValid();
}

void CMFCDictionaryDoc::Dump(CDumpContext& dc)
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Команды CMFCDictionaryDoc
