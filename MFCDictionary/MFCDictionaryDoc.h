
// MFCDictionaryDoc.h: интерфейс класса CMFCDictionaryDoc 
//


#pragma once
#include <map>
#include <vector>

using namespace std;

class CMFCDictionaryDoc : public CDocument
{
protected: // создать только из сериализации
	CMFCDictionaryDoc();
	DECLARE_DYNCREATE(CMFCDictionaryDoc)

// Атрибуты
public:
	bool is_value = false;
	map<string, int> dictionary;
// Операции
public:

// Переопределение
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Реализация
public:
	virtual ~CMFCDictionaryDoc();
	void OnLoadTargetFile();
	void UpdateView();
	vector<pair<string, int>> GetDictionaryVector();
	void SetDescValue();
	void SetDescKey();
#ifdef _DEBUG
	virtual void AssertValid();
	virtual void Dump(CDumpContext& dc);
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Вспомогательная функция, задающая содержимое поиска для обработчика поиска
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
