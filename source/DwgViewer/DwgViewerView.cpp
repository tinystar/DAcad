
// DwgViewerView.cpp : CDwgViewerView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "DwgViewer.h"
#endif

#include "DwgViewerDoc.h"
#include "DwgViewerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDwgViewerView

IMPLEMENT_DYNCREATE(CDwgViewerView, CView)

BEGIN_MESSAGE_MAP(CDwgViewerView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDwgViewerView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CDwgViewerView ����/����

CDwgViewerView::CDwgViewerView()
{
	// TODO:  �ڴ˴���ӹ������

}

CDwgViewerView::~CDwgViewerView()
{
}

BOOL CDwgViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CDwgViewerView ����

void CDwgViewerView::OnDraw(CDC* /*pDC*/)
{
	CDwgViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CDwgViewerView ��ӡ


void CDwgViewerView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDwgViewerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CDwgViewerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CDwgViewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӵ�ӡ����е��������
}

void CDwgViewerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDwgViewerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDwgViewerView ���

#ifdef _DEBUG
void CDwgViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CDwgViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDwgViewerDoc* CDwgViewerView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDwgViewerDoc)));
	return (CDwgViewerDoc*)m_pDocument;
}
#endif //_DEBUG


// CDwgViewerView ��Ϣ�������
