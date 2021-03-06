
// MFCDictionaryView.cpp: реализация класса CMFCDictionaryView
//

#include "stdafx.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "MFCDictionary.h"
#endif

#include "MFCDictionaryDoc.h"
#include "MFCDictionaryView.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCDictionaryView

IMPLEMENT_DYNCREATE(CMFCDictionaryView, CView)

BEGIN_MESSAGE_MAP(CMFCDictionaryView, CView)
END_MESSAGE_MAP()


// Создание или уничтожение CMFCDictionaryView

CMFCDictionaryView::CMFCDictionaryView()
{
	// TODO: добавьте код создания

}


CMFCDictionaryView::~CMFCDictionaryView()
{
}

BOOL CMFCDictionaryView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Рисование CMFCDictionaryView

void CMFCDictionaryView::OnDraw(CDC* pDC)
{
	CMFCDictionaryDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	auto vekv = pDoc->GetDictionaryVector();

	CRect rect;
	GetClientRect(&rect);
	auto sas = rect.Size();

	//pDC->Ellipse(1,1, 100, 100);

	int size[] = { sas.cx, sas.cy };
	int vek_size = vekv.size();

	try {
		// vector<vector<int>> infos = InitInfos(size, vekv);
		vector<vector<int>> infos = NewInit(size, vekv);
		DrawTable(pDC, vekv, infos, vek_size);
	}
	catch (...)
	{
		// "U wot m8, y height r width equls 0? Wt rings u got bicth?"
	}

}

void CMFCDictionaryView::DrawTable(CDC* pDC, vector<pair<string, int>> texts, vector<vector<int>> infos, int count)
{
	vector<int> start_info = infos[0];
	vector<int> rows_info = infos[1];
	vector<int> cols_info = infos[2];

	const int x = start_info[0] - 1;
	const int y = start_info[1] - 1;
	const int rows = start_info[2];
	const int cols = start_info[3];


	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	pDC->SelectObject(pen);

	int row_x = x;
	int row_y = y;

	int cell_max = count;
	int text_cell_count = 0;
	int number_cell_count = 0;

	for (int j = 0; j < cols; j++)
	{
		for (int i = 0; i < rows; i++)
		{
			int x_start = row_x;
			int y_start = row_y;
			int x_stop = row_x + cols_info[j];
			int y_stop = row_y + rows_info[i];

			int pos = j / 2 * rows + i;
			pair<string, int> data;
			if (pos < texts.size())
				data = texts[pos];
			CRect lpr = { x_start, y_start, x_stop, y_stop };

			if (!(j % 2) && text_cell_count < cell_max)
			{
				pDC->Rectangle(lpr);
				pDC->Rectangle(x_start, y_start, x_stop, y_stop);
				CString cstr;
				if (i == rows - 1 && j == cols - 2 && text_cell_count < cell_max)
					cstr = CString("<Не конец>");
				else
					cstr = CString(data.first.c_str());
				pDC->DrawText(cstr, lpr, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
				text_cell_count++;
			}
			if (j % 2 && number_cell_count < cell_max)
			{
				if (i != rows - 1 || j != cols - 1 || number_cell_count >= cell_max)
				{
					pDC->Rectangle(x_start, y_start, x_stop, y_stop);
					CString cstr = CString(std::to_string(data.second).c_str());
					pDC->DrawText(cstr, lpr, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
					number_cell_count++;
				}
			}
			row_y += rows_info[i] - 1;
		}
		row_y = y;
		row_x += cols_info[j] - 1;
	}
}

vector<vector<int>> CMFCDictionaryView::NewInit(int rect_size[], vector<pair<string, int>> vect)
{
	const int width = rect_size[0];
	const int height = rect_size[1];
	const int word_count = vect.size();

	if (!width || !height)
		throw "Dafuq, height or width is 0";

	if (!word_count)
		throw "No words";

	int greatest_word_length = -1;
	int greatest_number = -1;

	for (auto iter = vect.begin(); iter != vect.end(); ++iter)
	{
		int sz = iter->first.size();
		if (greatest_word_length < sz)
			greatest_word_length = sz;

		if (greatest_number < iter->second)
			greatest_number = iter->second;
	}

	const int min_width = 8 * greatest_word_length;
	const int min_height = 10;

	const int word_block_width = min_width + 20;
	int digits = 0;
	while(greatest_number)
	{
		greatest_number /= 10;
		digits++;
	}
	const int number_block_width = digits * 8 + 10;
	const int block_height = min_height + 10;

	const int x_margin = 30;
	const int y_margin = 30;

	const int available_height = height - y_margin - y_margin;
	const int available_width = width - x_margin - x_margin;

	// ========================
	// Определить количество блоков

	int vertical_blocks = available_height / block_height;
	int horizontal_blocks = available_width / (number_block_width + word_block_width);

	// ================
	// И теперь вроде затолкать сколько влезет

	vector<int> start_info;
	vector<int> rows_info;
	vector<int> cols_info;

	// Сначала воткнуть верхний левый угол таблицы
	start_info.push_back(x_margin);
	start_info.push_back(y_margin);

	for (int row = 0; row < vertical_blocks; row++)
		rows_info.push_back(block_height);
	for (int col = 0; col < horizontal_blocks; col++)
	{
		cols_info.push_back(word_block_width);
		cols_info.push_back(number_block_width);
	}

	// Затолкать количество строк и столбцов
	start_info.push_back(rows_info.size());
	start_info.push_back(cols_info.size());

	vector<vector<int>> infos;
	infos.push_back(start_info);
	infos.push_back(rows_info);
	infos.push_back(cols_info);

	return infos;
}


// Диагностика CMFCDictionaryView

#ifdef _DEBUG
void CMFCDictionaryView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCDictionaryView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCDictionaryDoc* CMFCDictionaryView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCDictionaryDoc)));
	return static_cast<CMFCDictionaryDoc*>(m_pDocument);
}

#endif //_DEBUG


// Обработчики сообщений CMFCDictionaryView

