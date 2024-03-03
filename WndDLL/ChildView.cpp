// ChildView.cpp : implementation of the CChildView class
//

#include "pch.h"
#include "framework.h"
#include "ChildView.h"

#define IDM_BLINK 1


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildView

CChildView::CChildView()
{
	m_colors[(int)COLOR::BACKGROUND].CreateSolidBrush(RGB(0, 0, 0));
	m_colors[(int)COLOR::NOTCUT].CreateSolidBrush(RGB(20, 30, 15));
	m_colors[(int)COLOR::PROCESSING].CreateSolidBrush(RGB(0, 0, 255));
	m_colors[(int)COLOR::MAX].CreateSolidBrush(RGB(0, 0, 0));
}

CChildView::~CChildView()
{
	
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CChildView message handlers
CBrush* CChildView::GetStockBrush(COLOR type)
{
	return &m_colors[(int)type];
}
BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), nullptr);

	return TRUE;
}

void CChildView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rc;
	GetClientRect(&rc);
	int half;
	half = min(rc.Width(), rc.Height()) / 2;
	CPoint center = rc.CenterPoint();
	rc.left = center.x - half;
	rc.top = center.y - half;
	rc.right = center.x + half;
	rc.bottom = center.y + half;

	dc.StretchBlt(rc.left, rc.top, rc.Width(), rc.Height(), &dcMem, 0, 0, MemSize, MemSize, SRCCOPY);

}



int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CBrush cBrush;
	CBrush* pOldBrush;

	CDC* pDC;
	pDC = GetDC();
	pBitmap = new CBitmap();
	
	pBitmap->CreateCompatibleBitmap(pDC, MemSize, MemSize);
	dcMem.CreateCompatibleDC(pDC);
	
	pOldBitmap = dcMem.SelectObject(pBitmap);

	cBrush.CreateSolidBrush(RGB(100, 100, 100));
	pOldBrush = dcMem.SelectObject(&cBrush);
	dcMem.PatBlt(0, 0, MemSize, MemSize, PATCOPY);

	dcMem.SelectObject(pOldBrush);
	cBrush.DeleteObject();
	ReleaseDC(pDC);

	return 0;
}
BOOL CChildView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CChildView::OnDestroy()
{
	CWnd::OnDestroy();

	for (auto it : m_scanviews)
	{
		delete it;
	}
	m_scanviews.clear();

	dcMem.SelectObject(pOldBitmap);
	ReleaseDC(&dcMem);
	pBitmap->DeleteObject();
	delete pBitmap;
}

void CChildView::SetupView()
{
	CDC* pDC;
	CDC dc;
	CBitmap* old_bmp;
	CBrush* old_brush;

	pDC = GetDC();

	for (auto it : m_scanviews)
	{
		delete it;
	}
	m_scanviews.clear();

	dc.CreateCompatibleDC(&dcMem);
	m_totalview.CreateCompatibleBitmap(&dcMem,
		CTotalView::PxlSize, CTotalView::PxlSize);
	m_progressview.CreateCompatibleBitmap(&dcMem,
		CTotalView::PxlSize, CTotalView::PxlSize);

	old_bmp = dc.SelectObject((CBitmap*) &m_totalview);
	old_brush = dc.SelectObject(GetStockBrush(COLOR::BACKGROUND));
	dc.PatBlt(0, 0, m_totalview.PixelSize(), m_totalview.PixelSize(), PATCOPY);

	float ratio = m_totalview.PixelSize() / GetDocument()->GetSize();

	auto& scans = GetDocument()->GetScans();
	for (auto& s : scans) {
		CScanView& sv = *(new CScanView());
		CDC scandc;
		CBitmap* pBmp;
		CBrush* oldBrush;

		scandc.CreateCompatibleDC(&dcMem);
		sv.CreateCompatibleBitmap(&dcMem, sv.PixelSize(), sv.PixelSize());
		pBmp = scandc.SelectObject((CBitmap*)&sv);
		oldBrush = scandc.SelectObject(GetStockBrush(COLOR::NOTCUT));
		scandc.PatBlt(25, 50, sv.PixelSize() - 50, sv.PixelSize() - 100, PATCOPY);
		
		//copy scanview to totalview
		int dst_x, dst_y, dst_w, dst_h;
		dst_x = s->Left() * ratio + m_totalview.PixelSize() / 2;
		dst_y = s->Top() * ratio + m_totalview.PixelSize() / 2;
		dst_w = s->Width() * ratio;
		dst_h = s->Height() * ratio;
		dc.StretchBlt(
			dst_x, dst_y, dst_w, dst_h, 
			&scandc,0,0, sv.PixelSize(), sv.PixelSize(), SRCCOPY);

		m_scanviews.push_back(&sv);

		scandc.SelectObject(pBmp);
		scandc.SelectObject(oldBrush);
		scandc.DeleteDC();
	}

	dc.SelectObject(old_bmp);
	dc.SelectObject(old_brush);
	
	ReleaseDC(pDC);
}
void CChildView::DisplayScan(int incr)
{
	if (incr == 0) {
		m_EachScanNo = 0;
	}
	else if (incr > 0) {
		m_EachScanNo++;
	}
	else if (incr < 0) {
		m_EachScanNo--;
	}
	if (m_EachScanNo >= m_scanviews.size())
		m_EachScanNo = m_scanviews.size() - 1;
	if (m_EachScanNo < 0)
		m_EachScanNo = 0;
	m_FullShow = false;
	m_EachScanShow = true;
	DrawToMemDC();
}
void CChildView::DisplayFit()
{
	m_FullShow = true;
	m_EachScanShow = false;
	DrawToMemDC();
}
void CChildView::Display()
{
	DrawToMemDC();
}
void CChildView::DrawToMemDC()
{
	if (m_FullShow) {
		CDC dc;
		CBitmap* pBmp;
		CBrush* oldBrush;
		auto sv = &m_totalview;
		dc.CreateCompatibleDC(&dcMem);
		pBmp = dc.SelectObject((CBitmap*)sv);

		dcMem.StretchBlt(0, 0, MemSize, MemSize, &dc, 0, 0,
			sv->PixelSize(), sv->PixelSize(), SRCCOPY);
		dcMem.DrawText(L"Fit", -1, CRect(0, 0, 100, 50), DT_LEFT | DT_WORDBREAK);
		dc.SelectObject(pBmp);
		dc.DeleteDC();
	}
	if (m_EachScanShow) {
		CDC dc;
		CBitmap* pBmp;
		CBrush* oldBrush;
		auto sv = m_scanviews[m_EachScanNo];
		dc.CreateCompatibleDC(&dcMem);
		pBmp = dc.SelectObject((CBitmap*)sv);
		dcMem.StretchBlt(0, 0, MemSize, MemSize, &dc, 0, 0,
			sv->PixelSize(), sv->PixelSize(), SRCCOPY);
		CString label;
		label.Format(L"Scan: %d", m_EachScanNo + 1);
		dcMem.DrawText(label, -1, CRect(0, 0, 100, 50), DT_LEFT | DT_WORDBREAK);
		dc.SelectObject(pBmp);
		dc.DeleteDC();
	}
	
	Invalidate(FALSE);
}
void CChildView::BlinkOn()
{
	auto doc = GetDocument();
	float ratio = CTotalView::PxlSize / doc->GetSize();
	m_blinkingAreas.clear();
	for (auto s : doc->GetScans()) {
		if (s->Status() == SCAN_STAT::Cutting) {
			int dst_x, dst_y, dst_w, dst_h;
			dst_x = s->Left() * ratio + CTotalView::PxlSize / 2;
			dst_y = s->Top() * ratio + CTotalView::PxlSize / 2;
			dst_w = s->Width() * ratio;
			dst_h = s->Height() * ratio;
			m_blinkingAreas.push_back(CRect(POINT{dst_x, dst_y}, SIZE{dst_w, dst_h}));
		}
	}
	m_blinkingCount = 0;
	SetTimer(IDM_BLINK, 100, NULL);
}
void CChildView::BlinkOff()
{
	KillTimer(IDM_BLINK);
}
void CChildView::UpdateProgress()
{
	CDC dc,dcscan;
	CBitmap* pBmp,* pBmpscan;
	CBrush* oldBrush;
	dc.CreateCompatibleDC(&dcMem);
	dcscan.CreateCompatibleDC(&dcMem);
	pBmp = dc.SelectObject((CBitmap*)&m_totalview);

	auto doc = GetDocument();
	float ratio = CTotalView::PxlSize / doc->GetSize();
	auto doc_scans = doc->GetScans();

	BITMAP bmp;
	DWORD dwLen;

	for (int i = 0; i < doc->GetScans().size();++i) {
		auto s = doc->GetScans()[i];
		if (s->Status() == SCAN_STAT::Done) {
			auto sv = m_scanviews[i];

			sv->GetBitmap(&bmp);
			dwLen = bmp.bmWidth * bmp.bmHeight * 4;
			PBYTE m_p = (BYTE*)realloc(nullptr, dwLen);
			sv->GetBitmapBits(dwLen, m_p);
			for (int y = 0; y < bmp.bmHeight; ++y) {
				BYTE* px = m_p + bmp.bmWidth * 4 * y;
				for (int x = 0; x < bmp.bmWidth; ++x) {
					if (px[0] == 0x00 && px[1] == 0x00 && px[2] == 0x00) {
						px[3] = 0;
					}
					else {
						px[0] = 0xff;
						px[1] = 0xff;
						px[2] = 0xff;
						px[3] = 0xff;
					}
					px += 4;
				}
			}
			sv->SetBitmapBits(dwLen, m_p);
			delete[] m_p;
			pBmpscan = dcscan.SelectObject((CBitmap*)sv);
			
			int dst_x, dst_y, dst_w, dst_h;
			dst_x = s->Left() * ratio + CTotalView::PxlSize / 2;
			dst_y = s->Top() * ratio + CTotalView::PxlSize / 2;
			dst_w = s->Width() * ratio;
			dst_h = s->Height() * ratio;
			dc.StretchBlt(dst_x, dst_y, dst_w, dst_h, &dcscan, 0, 0, sv->PxlSize, sv->PxlSize, SRCCOPY);
			dcscan.SelectObject(pBmpscan);
		}
	}
	dcscan.DeleteDC();
	dc.SelectObject(pBmp);
	dc.DeleteDC();
}
void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == IDM_BLINK) {
		KillTimer(IDM_BLINK);
		if (m_FullShow) {
			CDC dc;
			CBitmap* pBmp;
			CBrush* oldBrush;
			CPen pen, * oldPen;
			HGDIOBJ hBrs = ::GetStockObject(NULL_BRUSH);
			
			if (m_blinkingCount++ % 2 == 0) {
				pen.CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
			}
			else {
				pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			}

			dc.CreateCompatibleDC(&dcMem);
			pBmp = dc.SelectObject((CBitmap*)&m_totalview);
			dcMem.StretchBlt(0, 0, MemSize, MemSize, &dc, 0, 0,
				CTotalView::PxlSize, CTotalView::PxlSize, SRCCOPY);
			dc.SelectObject(pBmp);
			dc.DeleteDC();

			oldPen = (CPen*)dcMem.SelectObject(&pen);
			HGDIOBJ hOldBrs = dcMem.SelectObject(hBrs);

			for (auto& br : m_blinkingAreas) {
				CRect rc;
				float rat = MemSize / CTotalView::PxlSize;
				rc.left = br.left * rat;
				rc.right = br.right * rat;
				rc.top = br.top * rat;
				rc.bottom = br.bottom * rat;
				dcMem.Rectangle(rc);
			}

			dcMem.SelectObject(oldPen);
			dcMem.SelectObject(hOldBrs);
			
			pen.DeleteObject();

			Invalidate(FALSE);
		}
		SetTimer(IDM_BLINK, 100, NULL);
	}
	CWnd::OnTimer(nIDEvent);
}
