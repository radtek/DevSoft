// ChartFigureDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "ChartFigureDlg.h"
#include <boost/checked_delete.hpp>
#include "DlgChartSet.h"
#include "DlgTabPanel.h"
#include "Msg.h"
#include "Macro.h"
// ChartFigureDlg 对话框
#define TIMER_CHART_EVENT (1000011)
#define TIMER_CHART (300)
#define TIMER_AXIS_EVENT (1000)
#define TIMER_GAP    (500)
IMPLEMENT_DYNAMIC(ChartFigureDlg, CBaseDialog)
ChartFigureDlg::ChartFigureDlg(CWnd* pParent /*=NULL*/)
	: CBaseDialog(ChartFigureDlg::IDD, pParent),
	m_bActiveFlag(FALSE)
{
	m_charxy = NULL;
}

ChartFigureDlg::~ChartFigureDlg()
{
	boost::checked_delete(m_charxy);
}

void ChartFigureDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ChartFigureDlg, CBaseDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_ACTIVATE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CHART_SET, &ChartFigureDlg::OnChartSet)
	ON_COMMAND(ID_CHART_DEL, &ChartFigureDlg::OnChartDel)
	ON_COMMAND(ID_CHART_ZOOM_IN, &ChartFigureDlg::OnChartZoomIn)
	ON_COMMAND(ID_CHART_ZOOM_OUT, &ChartFigureDlg::OnChartZoomOut)
	ON_COMMAND(ID_CHART_SHOW_ALL, &ChartFigureDlg::OnChartShowAll)
	ON_WM_TIMER()
	ON_MESSAGE(WM_SET_DLG_ACTIVE,&ChartFigureDlg::NotifyActive)
END_MESSAGE_MAP()

BOOL ChartFigureDlg::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CDC* pDC = GetDC();
	m_charxy = new ChartXY(pDC->m_hDC);

	CRect rect;
	GetClientRect(rect);
	CSize size(rect.Width(), rect.Height());
	m_charxy->resize(size);
	//SetTimer(TIMER_CHART_EVENT, TIMER_CHART, NULL);
	SetTimer(TIMER_AXIS_EVENT,TIMER_GAP,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
void ChartFigureDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CBaseDialog::OnActivate(nState, pWndOther, bMinimized);

}
// ChartFigureDlg 消息处理程序
void ChartFigureDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetCapture();
	if (m_charxy)
	{
		m_charxy->mousePressEvent(point);
		//Invalidate();
	}

	CWnd* pWnd = AfxGetMainWnd();
	if (NULL != pWnd)
	{
		pWnd->PostMessage(WM_NOTIFY_ACTIVE_WND_TYPE,2,0);
	}
	CBaseDialog::OnLButtonDown(nFlags, point);
}


void ChartFigureDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_charxy)
	{
		m_charxy->mouseReleaseEvent(point);
		//Invalidate();
	}
	ReleaseCapture();
	CBaseDialog::OnLButtonUp(nFlags, point);
}


BOOL ChartFigureDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_charxy)
	{
		CPoint pts = pt;
		ScreenToClient(&pts);
		m_charxy->wheelEvent(pts, zDelta);
		Invalidate();
	}
	return CBaseDialog::OnMouseWheel(nFlags, zDelta, pt);
}


void ChartFigureDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_charxy)
	{
		m_charxy->mouseMoveEvent(point);
		//Invalidate();
	}
	CBaseDialog::OnMouseMove(nFlags, point);
}


void ChartFigureDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CWnd::OnPaint()
	//CDialog::OnPaint();
	if (m_charxy)
	{
		m_charxy->paintEvent();
	}
	//描绘边框
	CWnd* parent = GetParent();
	if(parent)
	{
		parent = parent->GetParent();
		CDlgTabPanel* pTabPanel = dynamic_cast<CDlgTabPanel*>(parent);
		if(pTabPanel)
		{
			CRect rc;
			GetClientRect(rc);
			//if(this == pTabPanel->GetActiveDlg())//当前窗口激活
			if (TRUE == m_bActiveFlag)
			{
				CPen BoradrPen;
				BoradrPen.CreatePen(PS_SOLID,5,ACTIVE_COLOR);
				CPen* pOldPen = dc.SelectObject(&BoradrPen);

				dc.MoveTo(rc.left,rc.top);
				dc.LineTo(rc.right,rc.top);

				dc.MoveTo(rc.right,rc.top);
				dc.LineTo(rc.right,rc.bottom);

				dc.MoveTo(rc.right,rc.bottom);
				dc.LineTo(rc.left,rc.bottom);

				dc.MoveTo(rc.left,rc.bottom);
				dc.LineTo(rc.left,rc.top);

				dc.SelectObject(pOldPen);
				BoradrPen.DeleteObject();
			}else
			{
				CPen BoradrPen;
				BoradrPen.CreatePen(PS_SOLID,5,INACTIVE_COLOR);
				CPen* pOldPen = dc.SelectObject(&BoradrPen);
				dc.MoveTo(rc.left,rc.top);
				dc.LineTo(rc.right,rc.top);

				dc.MoveTo(rc.right,rc.top);
				dc.LineTo(rc.right,rc.bottom);

				dc.MoveTo(rc.right,rc.bottom);
				dc.LineTo(rc.left,rc.bottom);

				dc.MoveTo(rc.left,rc.bottom);
				dc.LineTo(rc.left,rc.top);

				dc.SelectObject(pOldPen);
				BoradrPen.DeleteObject();
			}
		}
	}
}


void ChartFigureDlg::OnSize(UINT nType, int cx, int cy)
{
	CBaseDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_charxy)
	{
		m_charxy->resize(CSize(cx, cy));
		Invalidate();
	}
}


void ChartFigureDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CBaseDialog::OnSizing(fwSide, pRect);

	// TODO: 在此处添加消息处理程序代码
	//Invalidate();
}

void ChartFigureDlg::updateData() const
{
	if (m_charxy)
	{
		return m_charxy->updateData();
	}
}
void ChartFigureDlg::ChartSet()
{
	OnChartSet();
}
//保存数据
int ChartFigureDlg::saveData()const
{
	//保存当前图表的数据
	return 0;
}
//读取数据
int ChartFigureDlg::readData()
{
	//读取当前图表的数据
	return 0;
}


void ChartFigureDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CBCGPContextMenuManager* pContexMenuManager = theApp.GetContextMenuManager();
	if (nullptr != pContexMenuManager)
	{
		pContexMenuManager->ShowPopupMenu(IDR_MENU_CHART,point.x,point.y,pWnd,TRUE);
	}	
	ScreenToClient(&point);
	m_ptContext = point;
}


void ChartFigureDlg::OnChartSet()
{
	// TODO: Add your command handler code here
	DlgChartSet dlgSet(this);
	//初始化当前值
	dlgSet.m_XKeyID = m_charxy->getXID();
	dlgSet.m_eLineStyle = m_charxy->getLineStyle();
	dlgSet.m_eMoveStyle = m_charxy->getMoveStyle();
	dlgSet.m_eChartType = m_charxy->getChartType();
	boost::unordered_map<SENSOR_TYPE_KEY, bool> mapV = m_charxy->getMapVisible();
	dlgSet.m_setShowID.clear();
	for(auto itr = mapV.begin(); itr != mapV.end(); ++itr)
	{
		if(itr->second)
		{
			dlgSet.m_setShowID.insert(itr->first);
		}
	}
	if(IDOK == dlgSet.DoModal())
	{
		boost::unordered_map<SENSOR_TYPE_KEY, bool> mapV;
		auto setV = dlgSet.m_setShowID;
		for(auto itr = setV.begin(); itr != setV.end(); ++itr)
		{
			mapV[*itr] = true;
		}
		m_charxy->setMapVisible(mapV);

		m_charxy->setXID(dlgSet.m_XKeyID);
		m_charxy->setMoveStyle(dlgSet.m_eMoveStyle);
		m_charxy->setChartType(dlgSet.m_eChartType);
		m_charxy->setLineStyle(dlgSet.m_eLineStyle);
		m_charxy->refreshData();
		Invalidate();
	}
}


void ChartFigureDlg::OnChartDel()
{
}


void ChartFigureDlg::OnChartZoomIn()
{
	if (m_charxy)
	{
		m_charxy->zoomIn(m_ptContext);
	}
}


void ChartFigureDlg::OnChartZoomOut()
{
	if (m_charxy)
	{
		m_charxy->ZoomOut(m_ptContext);
	}
}


void ChartFigureDlg::OnChartShowAll()
{
	// TODO: Add your command handler code here
	if (m_charxy)
	{
		m_charxy->showAll();
	}
}


BOOL ChartFigureDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (pMsg->message)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		{
			CWnd* pDiagramPanel = GetParent();
			if (nullptr != pDiagramPanel)
			{
				CDlgTabPanel* pTabWnd = dynamic_cast<CDlgTabPanel*>(pDiagramPanel->GetParent());

				if (nullptr != pTabWnd)
				{
					pTabWnd->SetActive(CDlgTabPanel::DIAGRAM_INDEX,this);
				}	
			}
		}
		break;
	default:
		break;
	}
	//end add by hanxiaowei


	if (WM_LBUTTONDOWN == pMsg->message || WM_RBUTTONDOWN == pMsg->message)
	{
		CWnd* pWnd = AfxGetMainWnd();
		if (NULL != pWnd)
		{
			pWnd->PostMessage(WM_NOTIFY_ACTIVE_WND_TYPE,2,0);
		}
	}
	return CBaseDialog::PreTranslateMessage(pMsg);
}


void ChartFigureDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (TIMER_CHART_EVENT == nIDEvent)
	{
		updateData();
	}
	if (TIMER_AXIS_EVENT == nIDEvent)
	{
		if (nullptr != m_charxy)
		{
			m_charxy->ResfreshAxisTitle();
			m_charxy->paintEvent();
		}
	}
	CBaseDialog::OnTimer(nIDEvent);
}

LRESULT ChartFigureDlg::NotifyActive( WPARAM wp,LPARAM lp )
{
	int nActiveFlag = (int)wp;

	if (nActiveFlag)
	{
		m_bActiveFlag = TRUE;
	}
	else
	{
		m_bActiveFlag = FALSE;
	}
	return 0L;
}

void ChartFigureDlg::SetXAxisSensorID(const SENSOR_TYPE_KEY& XAxisID)
{
	if (nullptr == m_charxy)
	{
		return;
	}
	
	m_charxy->setXID(XAxisID);
}

SENSOR_TYPE_KEY ChartFigureDlg::GetXAxisSensorID(void)
{
	if (nullptr == m_charxy)
	{
		return SENSOR_TYPE_KEY();
	}

	return m_charxy->getXID();
}

void ChartFigureDlg::SetYAxisSensorID(const SENSOR_TYPE_KEY& YAxisID)
{
	if (nullptr == m_charxy)
	{
		return;
	}

	m_charxy->setVisible(YAxisID,true);
}

void ChartFigureDlg::RestYAxisSensorID(void)
{
	if (nullptr == m_charxy)
	{
		return;
	}
	m_charxy->ResetYAxisSensor();
}

bool ChartFigureDlg::IsYAxisSensorIDExist(const SENSOR_TYPE_KEY& YAxisID) const
{
	if (nullptr == m_charxy)
	{
		return false;
	}

	return m_charxy->getVisible(YAxisID);
}

void ChartFigureDlg::NotifyControlsStartRefresh()
{
	SetTimer(TIMER_CHART_EVENT,TIMER_CHART,NULL);
}

void ChartFigureDlg::NotifyControlsStopRefresh()
{
	KillTimer(TIMER_CHART_EVENT);
}


