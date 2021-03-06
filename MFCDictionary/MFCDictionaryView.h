
// MFCDictionaryView.h: интерфейс класса CMFCDictionaryView
//

#pragma once


class CMFCDictionaryView : public CView
{
protected: // создать только из сериализации
	CMFCDictionaryView();
	DECLARE_DYNCREATE(CMFCDictionaryView)

// Атрибуты
public:
	CMFCDictionaryDoc* GetDocument() const;

// Операции
public:
// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Реализация
public:
	virtual ~CMFCDictionaryView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void DrawTable(CDC* pDC, vector<pair<string, int>> texts, vector<vector<int>> infos, int count);
	vector<vector<int>> NewInit(int rect_size[], vector<pair<string, int>> vect);
// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // версия отладки в MFCDictionaryView.cpp
inline CMFCDictionaryDoc* CMFCDictionaryView::GetDocument() const
   { return reinterpret_cast<CMFCDictionaryDoc*>(m_pDocument); }
#endif
