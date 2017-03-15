#include "StdAfx.h"
#include "Edislab Pro.h"
#include "Edislab ProView.h"
#include "CommandEntry.h"
#include "Msg.h"
#include "DlgAddPage.h"
#include "DlgTabPanel.h"
#include "DlgPageNameConfig.h"
#include "Utility.h"
#include "ComImple.h"
//���ҳ����
const int MAX_PAGE_NUM = 4;
//�����������
const int MAX_GRID_NUM = 9;
const int MAX_DEVICE_NUM = 9;
const int MAX_DIAGRAM_NUM = 9;
//��������ҳ��
static void HandleAddPage(CEdislabProView* pView);
//��������ҳ��
static void UpdateHandleAddPage(CEdislabProView* pView,CCmdUI* pCmdUI);
//ɾ��ҳ��
static void HandleDelPage(CEdislabProView* pView);
//����ɾ��ҳ��
static void UpdateHandleDelPage(CEdislabProView* pView,CCmdUI* pCmdUI);
//ҳ������
static void HandlePageName(CEdislabProView* pView);
//����ҳ������
static void UpdateHandlePageName(CEdislabProView* pView,CCmdUI* pCmdUI);
//��תҳ��
static void HandleJumpPage(CEdislabProView* pView);
//������ת����
static void UpdateHandleJumpPage(CEdislabProView* pView,CCmdUI* pCmdUI);
//���ӱ���
void HandleAddTable(CEdislabProView* pView);
//�������ӱ���
void UpdateHandleAddTable(CEdislabProView* pView,CCmdUI* pCmdUI);
//����ͼ
void HandleAddImage(CEdislabProView* pView);
//��������ͼ
void UpdateHandleAddImage(CEdislabProView* pView,CCmdUI* pCmdUI);
//�����Ǳ�
void HandleAddDevice(CEdislabProView* pView);
//���������Ǳ�
void UpdateHandleAddDevice(CEdislabProView* pView,CCmdUI* pCmdUI);
//ɾ��Ԫ��
void HandleDelElement(CEdislabProView* pView);
//����ɾ��Ԫ��
void UpdateHandleDelElement(CEdislabProView* pView,CCmdUI* pCmdUI);
//��ʼ�ɼ�
void HandleStart(CEdislabProView* pView);
//�Զ�ʶ��
void HandleAutoSelect(CEdislabProView* pView);
//���ʵ�鱨��
void HandleOutputTestReport(CEdislabProView* pView);

CCommandEntry& CCommandEntry::CreateInstance( void )
{
	static CCommandEntry s_Entry;
	return s_Entry;
}

CCommandEntry::CCommandEntry(void)
{
	InitCommandEntry();
	InitUpdateCommandEntry();
}


CCommandEntry::~CCommandEntry(void)
{
	UnInitCommandEntry();
	UnInitUpdateCommandEntry();
}

/*****************************************************************************
@FunctionName : GetCommandEntryByCommanID
@FunctionDescription : ��ȡ����������ӿ�
@inparam  : nCommandID:����ID
@outparam :  
@return : �����������ַ
*****************************************************************************/
pCommandEntry CCommandEntry::GetCommandEntryByCommanID( unsigned int nCommandID )
{

	std::map<unsigned int,pCommandEntry>::iterator Iter = m_CommandEntryMap.find(nCommandID);

	if (Iter == m_CommandEntryMap.end())
	{
		return NULL;
	}

	return Iter->second;

}

/*****************************************************************************
@FunctionName : GetUpdateCommandEntryByCommanID
@FunctionDescription : ��ȡ������´��������ӿ�
@inparam  : nCommandID:����ID
@outparam :  
@return : ������´���������ַ
*****************************************************************************/
pUpdateCommandEntry CCommandEntry::GetUpdateCommandEntryByCommanID( unsigned int nCommandID )
{
	std::map<unsigned int,pUpdateCommandEntry>::iterator Iter = m_UpdateCommandEntryMap.find(nCommandID);

	if (Iter == m_UpdateCommandEntryMap.end())
	{
		return NULL;
	}

	return Iter->second;
}

/*****************************************************************************
@FunctionName : InitCommandEntry
@FunctionDescription : ��ʼ��������ӿ�����
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CCommandEntry::InitCommandEntry( void )
{
	m_CommandEntryMap.clear();

	m_CommandEntryMap[ID_ADD_PAGE] = HandleAddPage;
	m_CommandEntryMap[ID_DEL_PAGE] = HandleDelPage;
	m_CommandEntryMap[ID_SET_PAGE_NAME] = HandlePageName;
	m_CommandEntryMap[ID_GOTO_PAGE] = HandleJumpPage;
	m_CommandEntryMap[ID_ADD_TABLE] = HandleAddTable;
	m_CommandEntryMap[ID_DEL_ELEMENT] = HandleDelElement;
	m_CommandEntryMap[ID_ADD_IMAGE] = HandleAddImage;
	m_CommandEntryMap[ID_ADD_DEVICE] = HandleAddDevice;
	m_CommandEntryMap[ID_AUTO_SELECT] = HandleAutoSelect;
	m_CommandEntryMap[ID_START] = HandleStart;
	m_CommandEntryMap[ID_OUTPUT_TEST_REPORT] = HandleOutputTestReport;
}

/*****************************************************************************
@FunctionName : InitUpdateCommandEntry
@FunctionDescription : ��ʼ������������ӿ�����
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CCommandEntry::InitUpdateCommandEntry( void )
{
	m_UpdateCommandEntryMap.clear();
	m_UpdateCommandEntryMap[ID_ADD_PAGE] = UpdateHandleAddPage;
	m_UpdateCommandEntryMap[ID_DEL_PAGE] = UpdateHandleDelPage;
	m_UpdateCommandEntryMap[ID_SET_PAGE_NAME] = UpdateHandlePageName;
	m_UpdateCommandEntryMap[ID_GOTO_PAGE] = UpdateHandleJumpPage;
	m_UpdateCommandEntryMap[ID_ADD_TABLE] = UpdateHandleAddTable;
	m_UpdateCommandEntryMap[ID_DEL_ELEMENT] = UpdateHandleDelElement;
	m_UpdateCommandEntryMap[ID_ADD_IMAGE] = UpdateHandleAddImage;
	m_UpdateCommandEntryMap[ID_ADD_DEVICE] = UpdateHandleAddDevice;

}

/*****************************************************************************
@FunctionName : UnInitCommandEntry
@FunctionDescription : ���������ӿ�����
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CCommandEntry::UnInitCommandEntry( void )
{
	m_CommandEntryMap.clear();
}

/*****************************************************************************
@FunctionName : UnInitUpdateCommandEntry
@FunctionDescription : ��ո���������ӿ�����
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CCommandEntry::UnInitUpdateCommandEntry( void )
{
	m_UpdateCommandEntryMap.clear();
}

/*****************************************************************************
@FunctionName : HandleAddPage
@FunctionDescription : ��������ҳ�����Ӧ
@inparam  : pView:��ͼָ��
@outparam :  
@return : 
*****************************************************************************/
void HandleAddPage( CEdislabProView* pView )
{
	if (NULL == pView)
	{
		return;
	}

	CDlgAddPage AddDlg;

	if (IDOK == AddDlg.DoModal())
	{

		//��ȡ���ӵĶ���
		ADD_PAGE_ELEMENT AddPageElement = AddDlg.GetAddPageElement();
		if (NEW_PAGE_COPY == AddPageElement.eumOpt)
		{
			CDlgTabPanel* pPanel = pView->GetCurrentPage();

			if (NULL != pPanel)
			{
				AddPageElement.nGridNum = pPanel->GetGridNum();
				AddPageElement.nDiagramNum = pPanel->GetDiagramNum();
				AddPageElement.nDeviceNum = pPanel->GetDeviceNum();
			}
		}		

		if (pView->AddNewTabWnd(AddPageElement.strPageName,AddPageElement.nGridNum,AddPageElement.nDiagramNum,AddPageElement.nDeviceNum))
		{
			//�������һ��TabΪ���Tab
			int nPageNum = pView->GetPageNum();
			pView->SetActivePage(nPageNum - 1);
		}
	}
}

/*****************************************************************************
@FunctionName : UpdateHandleAddPage
@FunctionDescription : ���´�������ҳ�����Ӧ
@inparam  : pView:��ͼָ��
@outparam :  
@return : 
*****************************************************************************/
void UpdateHandleAddPage( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (NULL == pView || NULL == pCmdUI)
	{
		return;
	}

	int nNum = pView->GetPageNum();

	if (nNum < MAX_PAGE_NUM)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}

}

/*****************************************************************************
@FunctionName : HandleDelPage
@FunctionDescription : ����ɾ��ҳ�����Ӧ
@inparam  : pView:��ͼָ��
@outparam :  
@return : 
*****************************************************************************/
void HandleDelPage( CEdislabProView* pView )
{
	if (NULL == pView)
	{
		return;
	}


	if (IDYES == Utility::AfxBCGPMessageBox(_T("ȷ��Ҫɾ��ҳ��?"),MB_YESNO))
	{
		pView->DeleteCurrentPage();
	}

}

/*****************************************************************************
@FunctionName : UpdateHandleDelPage
@FunctionDescription : ���´���ɾ��ҳ�����Ӧ
@inparam  : pView:��ͼָ��
@outparam :  
@return : 
*****************************************************************************/
void UpdateHandleDelPage( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (NULL == pView || NULL == pCmdUI)
	{
		return;
	}

	int nNum = pView->GetPageNum();

	if (nNum <= 1)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}

}

/*****************************************************************************
@FunctionName : HandlePageName
@FunctionDescription : ����ҳ���������õ���Ӧ
@inparam  : pView:��ͼָ��
@outparam :  
@return : 
*****************************************************************************/
void HandlePageName( CEdislabProView* pView )
{
	if (NULL == pView)
	{
		return;
	}

	CString strPageName = pView->GetCurrentPageName();


	CDlgPageNameConfig PageNameConfigDlg(strPageName);

	if (IDOK == PageNameConfigDlg.DoModal())
	{
		strPageName = PageNameConfigDlg.GetPageName();
		//��������
		pView->SetCurrentPageName(strPageName);
	}

}

/*****************************************************************************
@FunctionName : UpdateHandlePageName
@FunctionDescription : ����ҳ���������õ���Ӧ
@inparam  : pView:��ͼָ��
@outparam :  
@return : 
*****************************************************************************/
void UpdateHandlePageName( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (NULL == pView || NULL == pCmdUI)
	{
		return;
	}

	CDlgTabPanel* pWnd = pView->GetCurrentPage();

	if (NULL == pWnd)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}


/*****************************************************************************
@FunctionName : HandleJumpPage
@FunctionDescription : ������תҳ�����Ӧ
@inparam  : pView:��ͼָ��
@outparam :  
@return : 
*****************************************************************************/
void HandleJumpPage( CEdislabProView* pView )
{
	if (NULL == pView)
	{
		return;
	}
}

/*****************************************************************************
@FunctionName : UpdateHandleJumpPage
@FunctionDescription : ������תҳ�����Ӧ
@inparam  : pView:��ͼָ��
@outparam :  
@return : 
*****************************************************************************/
void UpdateHandleJumpPage( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (NULL == pView || NULL == pCmdUI)
	{
		return;
	}

	int nNum = pView->GetPageNum();

	if (nNum <= 1)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}
//���ӱ���
void HandleAddTable(CEdislabProView* pView)
{
	if (NULL == pView)
	{
		return;
	}

	pView->AddGrid();
}
//����ͼ
void HandleAddImage(CEdislabProView* pView)
{
	if (NULL == pView)
	{
		return;
	}

	pView->AddDiagram();
	
}
//�����Ǳ�
void HandleAddDevice(CEdislabProView* pView)
{
	if (NULL == pView)
	{
		return;
	}

	pView->AddDevice();
}
//ɾ��Ԫ��
void HandleDelElement(CEdislabProView* pView)
{
	if (NULL == pView)
	{
		return;
	}
	if(pView)
	{
		CDlgTabPanel* tabPanel = pView->GetCurrentPage();
		if(tabPanel)
		{
			tabPanel->DelWnd(tabPanel->GetActiveDlg());
		}
	}
}

void UpdateHandleAddTable( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (NULL == pView || NULL == pCmdUI)
	{
		return;
	}

	if (pView->GetGridNum() >= MAX_GRID_NUM)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

void UpdateHandleAddImage( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (NULL == pView || NULL == pCmdUI)
	{
		return;
	}

	if (pView->GetDiagramNum() >= MAX_DIAGRAM_NUM)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

void UpdateHandleAddDevice( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (NULL == pView || NULL == pCmdUI)
	{
		return;
	}

	if (pView->GetDeviceNum() >= MAX_DEVICE_NUM)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

void UpdateHandleDelElement( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (NULL == pView || NULL == pCmdUI)
	{
		return;
	}

	if (0 == pView->GetGridNum() && 0 == pView->GetDeviceNum() && 0 == pView->GetDiagramNum())
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

//��ʼ�ɼ�
void HandleStart(CEdislabProView* pView)
{
	if (NULL == pView)
	{
		return;
	}
	if(pView)
	{
		BYTE sendMsg[3] = {0xAA,0x03,0xFF};
		COMIPLE.SendComData(sendMsg,3);
	}
}
//�Զ�ʶ��
void HandleAutoSelect(CEdislabProView* pView)
{
	if (NULL == pView)
	{
		return;
	}
	if(pView)
	{
		CWnd* pWnd = AfxGetMainWnd();
		if(NULL == pWnd)
		{
			return;
		}

		COMIPLE.StartCom(pWnd);
	}
}

//���ʵ�鱨��
void HandleOutputTestReport( CEdislabProView* pView )
{
	TRACE("HandleOutputTestReport!\r\n");
}