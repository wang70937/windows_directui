
#include "stdafx.h"
#include "resource.h"

#include "Include/Def/Require.h"
#include "Include/Util/UtilTpl.h"

#include "Include/Common/ICVariant.h"
#include "Include/Common/ISPlugin.h"
#include "Include/Common/ISModule.h"
#include "Include/BaseMisc/ICTest.h"
#include "Include/UIEngine/IUIMsgLoop.h"
#include "Include/UIEngine/IUITimer.h"
#include "Include/UIEngine/IUIBrush.h"
#include "Include/UIEngine/IUIWin.h"
#include "Include/UIEngine/IUIFrameDef.h"
#include "Include/UIEngine/IUITexture.h"
#include "Include/UIEngine/IUIWebkit.h"
#include "Include/UIEngine/IUILayoutDefault.h"
#include "Include/UIEngine/IUIBackBlur.h"
#include "Include/UIEngine/IUIFrameInternal.h"
#include "Include/UIEngine/IUIStatic.h"
#include "Include/UIEngine/IUIAnimate.h"
#include "Include/UIEngine/IUITextField.h"
#include "Include/UIEngine/IUIButton.h"
#include "Include/UIEngine/IUIFactory.h"
#include "Include/UIEngine/IUITooltips.h"
#include "Include/UIEngine/IUIList.h"

[ module(exe, uuid = "{91C6F9F7-EE96-4F7A-8F4C-13D8EF179C40}", 
		 name = "Test", 
		 helpstring = "Test 1.0 类型库",
		 resource_name = "IDR_TEST") ]



 
//  int WINAPI _tWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd )
//  {
// 
//  	return 0;
//  }

 class CTestModule //: public CAtlExeModuleT <CTestModule>
{

 int i ;
public:
	CComSinkTimerCallbackPtr<CTestModule> _timer_callback ;
	CComSinkUIEventCallbackPtr<CTestModule> _event_callback ;

	CComPtr<IUITextField> pEdit ;
	CComPtr<IUIWebkit> pWebkit ;

	HRESULT OnEvent(IUIFrame* pFrame, INT nEventId, tEventArg* pArgs)
	{
// 		if (pEdit && pWebkit)
// 		{
// 			CComBSTR str ;
// 			pEdit->GetText(&str) ;
// 			pWebkit->SetUrl(str) ;
// 		}
		BSTR bstrName;
		if ( S_OK == pFrame->GetName(&bstrName) && nEventId == 256 )
		{
			CComPtr<IUIElement> pRoot;
			pFrame->GetRoot(&pRoot);

			CComQIPtr<IUIWin> pWin = pRoot ;
			if ( pWin )
			{
				HWND hWnd;
				pWin->GetHWnd(&hWnd);
				if ( _tcscmp(bstrName, _T("BtnClose")) == 0 )
				{
					::PostMessage(hWnd, WM_DESTROY, 0, 0);
				}
				else if ( _tcscmp(bstrName, _T("BtnMin")) == 0 )
				{
					::PostMessage(hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
				}
				
			}
		}
		
		return S_OK ;
	}

	CTestModule():_timer_callback(this),_event_callback(this)
	{

		HINSTANCE hUser32 = LoadLibrary( L"user32.dll" );
		if( hUser32 )
		{
			typedef BOOL ( WINAPI* LPSetProcessDPIAware )( void );
			LPSetProcessDPIAware pSetProcessDPIAware = ( LPSetProcessDPIAware )GetProcAddress( hUser32,
				"SetProcessDPIAware" );
			if( pSetProcessDPIAware )
			{
				pSetProcessDPIAware();
			}
			FreeLibrary( hUser32 );
		}

		Common::Util::Init() ;

		CWeakPtr<ISModule> wpModule ;
		Common::Util::GetService(&wpModule) ;
		wpModule->LoadModule(_T("BaseMisc")) ;
		wpModule->LoadModule(_T("UIEngine")) ;

		/////////////////
		CString sPicPath;
		TCHAR chPath[2*MAX_PATH] = {0};
		GetModuleFileName(NULL, chPath, 2*MAX_PATH);
		PathRemoveFileSpec(chPath);
		_tcscat(chPath, _T("\\pic"));

/*
		//#########################################
		CComPtr<IUIWin> pWin ;
		Common::Util::CreateObject(&pWin) ;
		DEBUG_ASSERT(pWin) ;
		CComPtr<IUITexture> pTexture ;
		Common::Util::CreateObject(&pTexture) ;
		sPicPath.Format(_T("%s\\bk.png"), chPath);
		pTexture->LoadFromFile(CComBSTR(sPicPath)) ;
		pTexture->SetDrawGrid(CRect(30,30,30,30)) ;
		pTexture->SetDrawType(TEXTURE::DRAW::GRID) ;
		pWin->SetBkgDraw(pTexture) ;

		//pWin->SetToolWindow(TRUE) ;
		//pWin->SetTopMost(TRUE) ;
		//pWin->SetLayerWin(TRUE) ;
		pWin->CreateWin(NULL, NULL) ;
		pWin->ShowWin(SW_SHOW) ;
		pWin->SetWinSize(CSize(680, 370)) ;
		pWin->SetTitle(_T("guodong")) ;
		pWin->SetDoubleBuffer(TRUE) ;
		pWin->SetPadding(CRect(10,5,0,10)) ;
		pWin->SetWindowBorder(CRect(5,5,5,5)) ;
		//pWin->SetAutoWidth(TRUE) ;
		//pWin->SetAutoHeight(TRUE) ;


		//###########Tooltip###################
		CWeakPtr<IUITooltips> wpTooltips ;
		Common::Util::GetService(&wpTooltips) ;
		wpTooltips->SetTooltipsDraw(pTexture) ;
		wpTooltips->SetTooltipsPadding(CRect(10,0,10,10)) ;

		//###########LayoutVertical##############
		CComPtr<IUILayoutVertical> pLayoutVer ;
		Common::Util::CreateObject(&pLayoutVer) ;
		pLayoutVer->SetFillItemIndex(-1) ;
		pWin->SetLayouter(pLayoutVer) ;

		//###########LayoutHorizontal########
// 		CComPtr<IUILayoutHorizontal> pLayoutHor;
// 		Common::Util::CreateObject(&pLayoutHor);
// 		pLayoutHor->SetFillItemIndex(0);
		//pWin->SetLayouter(pLayoutHor) ;

		//###########Brush##################
		CComPtr<IUIBrush> pBrush;
		Common::Util::CreateObject(&pBrush) ;
		pBrush->SetColor(RGB(255, 60, 100)) ;
		pBrush->SetBrushType(BRUSH::RECT_BORDER) ;

		CComPtr<IUIBrush> pBrush2;
		Common::Util::CreateObject(&pBrush2) ;
		pBrush2->SetColor(RGB(120, 230, 56)) ;
		pBrush2->SetBrushType(BRUSH::RECT_FILL) ;


		//############Font##################
		CComPtr<IUIFont> pFont;
		Common::Util::CreateObject(&pFont);
		CString strFontName = _T("楷体");
		pFont->SetFacename(CComBSTR(strFontName));
		pFont->SetItalic(TRUE);
		pFont->SetWeight(700);
		pFont->SetHeight(26);
		pFont->SetUnderline(TRUE);

		//############Frame#################
		CComPtr<IUIFrame> pFrame ;
		Common::Util::CreateObject(&pFrame) ;
		pFrame->SetAutoSize(TRUE) ;
		pFrame->SetAutoWidth(FALSE) ;
		pFrame->SetWidthAsParent(TRUE) ;
		//pFrame->SetSize(CSize(200, 100)) ;
		pFrame->SetMargin(CRect(10,0,10,0)) ;
		pFrame->SetBkgDraw(pBrush) ;
		{
			CComPtr<IUILayoutHorizontal> pLayout ;
			Common::Util::CreateObject(&pLayout) ;
			pLayout->SetFillItemIndex(1) ;
			pFrame->SetLayouter(pLayout) ;
		}

		pWin->AddChildBack(pFrame);


		//############Static################
		CComPtr<IUIStatic> pStatic ;
		Common::Util::CreateObject(&pStatic) ;
		pStatic->SetWidthAsParent(FALSE) ;
		pStatic->SetSize(CSize(200, 30)) ;
		pStatic->SetAutoSize(FALSE) ;
		pStatic->SetTextColor(RGB(0, 0, 0)) ;
		CString strTitle = _T("投资") ;
		pStatic->SetText(CComBSTR(strTitle)) ;
		pStatic->SetMutiLine(TRUE) ;
		pStatic->SetFont(pFont);
		pStatic->SetBkgDraw(pBrush2);
		pFrame->AddChildBack(pStatic) ;

		//###########Edit##################
// 		CComPtr<IUITextField> pEdit;
// 		Common::Util::CreateObject(&pEdit);
// 		pEdit->SetSize(CSize(100,30));
// 		//pEdit->SetPadding(CRect(0,0,0,0));
// 		pEdit->SetBkgDraw(pBrush);
// 		pEdit->SetTextColor(RGB(0,0,0));
//		pFrame->AddChildBack(pEdit);

		//###########Button#################
		CComPtr<IUIButton> pButton;
		Common::Util::CreateObject(&pButton);
		pButton->SetSize(CSize(36,25));
		//pButton->SetLocation(CPoint(400,3));
		pButton->SetAutoHeight(FALSE);
		pButton->SetPadding(CRect(5,5,5,5));
		pButton->SetName(_T("BtnMin"));

		pButton->SetToolTips(_T("最小化"));


		_event_callback->Hook_OnEvent(&CTestModule::OnEvent) ;
		pButton->AddEventListener(UI::EVENT::BUTTON_CLICK, _event_callback) ;
		//normal status
		{
			CComPtr<IUITexture> pNormal;
			Common::Util::CreateObject(&pNormal);
			sPicPath.Format(_T("%s\\btn_min.png"), chPath);
			pNormal->LoadFromFile(CComBSTR(sPicPath));
			pButton->SetNormalBkgDraw(pNormal);
		}
		{
			CComPtr<IUITexture> pHighlight ;
			Common::Util::CreateObject(&pHighlight) ;
			sPicPath.Format(_T("%s\\btn_min_hover.png"), chPath);
			pHighlight->LoadFromFile(CComBSTR(sPicPath)) ;
			pHighlight->SetDrawGrid(CRect(20,10,20,10)) ;
			pHighlight->SetDrawType(TEXTURE::DRAW::GRID) ;
			pButton->SetHighlightBkgDraw(pHighlight) ;
		}

		{
			CComPtr<IUITexture> pPushed ;
			Common::Util::CreateObject(&pPushed) ;
			sPicPath.Format(_T("%s\\btn_min_press.png"), chPath);
			pPushed->LoadFromFile(CComBSTR(sPicPath)) ;
			pPushed->SetDrawGrid(CRect(20,10,20,10)) ;
			pPushed->SetDrawType(TEXTURE::DRAW::GRID) ;
			pButton->SetPushedBkgDraw(pPushed) ;
		}
		pFrame->AddChildBack(pButton);

		///////
		CComPtr<IUIButton> pButton2;
		Common::Util::CreateObject(&pButton2);
		pButton2->SetSize(CSize(38,25));
		pButton2->SetAutoSize(FALSE);
		pButton2->SetName(_T("BtnClose"));
		pButton2->SetToolTips(_T("关闭"));

		//_event_callback->Hook_OnEvent(&CTestModule::OnEvent) ;
		pButton2->AddEventListener(UI::EVENT::BUTTON_CLICK, _event_callback) ;

		{
			CComPtr<IUITexture> pNormal;
			Common::Util::CreateObject(&pNormal);
			sPicPath.Format(_T("%s\\btn_close.png"), chPath);
			pNormal->LoadFromFile(CComBSTR(sPicPath));
			pButton2->SetNormalBkgDraw(pNormal);
		}
		{
			CComPtr<IUITexture> pHighlight ;
			Common::Util::CreateObject(&pHighlight) ;
			sPicPath.Format(_T("%s\\btn_close_hover.png"), chPath);
			pHighlight->LoadFromFile(CComBSTR(sPicPath)) ;
			pHighlight->SetDrawGrid(CRect(20,10,20,10)) ;
			pHighlight->SetDrawType(TEXTURE::DRAW::GRID) ;
			pButton2->SetHighlightBkgDraw(pHighlight) ;
		}

		{
			CComPtr<IUITexture> pPushed ;
			Common::Util::CreateObject(&pPushed) ;
			sPicPath.Format(_T("%s\\btn_close_press.png"), chPath);
			pPushed->LoadFromFile(CComBSTR(sPicPath)) ;
			pPushed->SetDrawGrid(CRect(20,10,20,10)) ;
			pPushed->SetDrawType(TEXTURE::DRAW::GRID) ;
			pButton2->SetPushedBkgDraw(pPushed) ;
		}
		pFrame->AddChildBack(pButton2);

		///////////////////////////////
		///////////////////////////////
		///////////////////////////////
		AddFrame(pWin, TRUE);

		/////////////////
//  		CComPtr<IUIWebkit> pWeb;
//  		Common::Util::CreateObject(&pWeb);
//  		pWeb->SetSize(CSize(200,400));
//  		pWeb->SetWidthAsParent(TRUE);
//  		pWeb->SetUrl(_T("http://www.baidu.com"));
// 
// 		pWin->AddChildBack(pWeb);

		//###########Frame##################
// 		CComPtr<IUIFrame> pFrameUrl ;
// 		Common::Util::CreateObject(&pFrameUrl) ;
// 		pWin->AddChildBack(pFrameUrl) ;
// 		pFrameUrl->SetAutoHeight(TRUE) ;
// 		pFrameUrl->SetWidthAsParent(TRUE) ;
// 		pFrameUrl->SetSize(CSize(200, 100)) ;
// 		pFrameUrl->SetMargin(CRect(0,0,0,10)) ;
// 		pFrameUrl->SetBkgDraw(pBrush) ;
// 		{
// 			CComPtr<IUILayoutHorizontal> pLayout ;
// 			Common::Util::CreateObject(&pLayout) ;
// 			pLayout->SetFillItemIndex(1) ;
// 			pFrameUrl->SetLayouter(pLayout) ;
// 		}
// 
// 		//###########Edit###################
// 		CComPtr<IUITextField> pEdit ;
// 		Common::Util::CreateObject(&pEdit) ;
// 		pEdit->SetSize(CSize(100, 50)) ;
// 		pEdit->SetHeightAsParent(TRUE) ;
// 		pEdit->SetBkgDraw(pBrush) ;
// 		pEdit->SetPadding(CRect(10,10,10,10)) ;
// 		pEdit->SetSingleLine(FALSE) ;
// 		pEdit->SetLineHeight(25) ;
// 		//pEdit->SetPassWord(TRUE) ;
// 		pFrameUrl->AddChildBack(pEdit) ;
// 
// 		this->pEdit = pEdit ;

		//
		CComPtr<IUITextField> pEdit ;
		Common::Util::CreateObject(&pEdit) ;
		pEdit->SetSize(CSize(0, 30)) ;
		//pEdit->SetHeightAsParent(TRUE) ;
		pEdit->SetBkgDraw(pBrush2) ;
		pEdit->SetPadding(CRect(10,10,10,10)) ;
		pEdit->SetSingleLine(TRUE) ;
		pEdit->SetLineHeight(35) ;
		//pEdit->SetPassWord(TRUE) ;
		pFrameUrl->AddChildBack(pEdit) ;

		this->pEdit = pEdit ;

		CComPtr<IUIButton> pButton ;
		Common::Util::CreateObject(&pButton) ;
		pButton->SetAutoSize(TRUE) ;
		pButton->SetPadding(CRect(5, 5, 5, 5)) ;
		pButton->SetText(_T("确 认")) ;
		pButton->SetToolTips(_T("tooltip confim"));

		_event_callback->Hook_OnEvent(&CTestModule::OnEvent) ;
		pButton->AddEventListener(UI::EVENT::BUTTON_CLICK, _event_callback) ;

		pButton->SetEnableDrag(TRUE) ;
		{
			CComPtr<IUITexture> p1 ;
			Common::Util::CreateObject(&p1) ;
			p1->LoadFromFile(_T("e:/button_normal.png")) ;
			p1->SetDrawGrid(CRect(20,10,131,38)) ;
			p1->SetDrawType(TEXTURE::DRAW::GRID) ;
			pButton->SetNormalBkgDraw(p1) ;
		}

		{
			CComPtr<IUITexture> p1 ;
			Common::Util::CreateObject(&p1) ;
			p1->LoadFromFile(_T("e:/button_hover.png")) ;
			p1->SetDrawGrid(CRect(20,10,131,38)) ;
			p1->SetDrawType(TEXTURE::DRAW::GRID) ;
			pButton->SetHighlightBkgDraw(p1) ;
		}

		{
			CComPtr<IUITexture> p1 ;
			Common::Util::CreateObject(&p1) ;
			p1->LoadFromFile(_T("e:/button_press.png")) ;
			p1->SetDrawGrid(CRect(20,10,131,38)) ;
			p1->SetDrawType(TEXTURE::DRAW::GRID) ;
			pButton->SetPushedBkgDraw(p1) ;
		}
		pFrameUrl->AddChildBack(pButton) ;

//    		CComPtr<IUIWebkit> pWeb ;
//    		Common::Util::CreateObject(&pWeb) ;
//    		pWeb->SetSize(CSize(200, 400)) ;
//    		pWeb->SetWidthAsParent(TRUE) ;
//    		pWeb->SetUrl(_T("http://www.baidu.com")) ;
//    		pWin->AddChildBack(pWeb) ;
   
   		//this->pWebkit = pWeb ;

		//////////////////////////
// 		CComPtr<IUIList> pList;
// 		Common::Util::CreateObject(&pList);
// 		for ( int i=0; i<10; i++ )
// 		{
// 			CComPtr<IUIListItem> pListItem;
// 			Common::Util::CreateObject(&pListItem);
// 
// 			//
// 			CComPtr<IUIElement> pElement;
// 			Common::Util::CreateObject(&pElement);
// 
// 			pElement->SetIndex(0);
// 			pElement->SetName(_T("a1"));
// 
// 			/////////
// 			CComPtr<IUIStatic> pStatic ;
// 			Common::Util::CreateObject(&pStatic) ;
// 			pStatic->SetWidthAsParent(TRUE) ;
// 			//pStatic->SetSize(CSize(400, 400)) ;
// 			pStatic->SetAutoHeight(TRUE) ;
// 			pStatic->SetTextColor(RGB(55, 255, 55)) ;
// 			CString s ;
// 			s.Format(_T("row %d"), i);
// 			pStatic->SetText(CComBSTR(s)) ;
// 			//////////
// 			pElement->AddChildBack(pStatic);
// 
// 			pListItem->SetHostList(pElement);
// 
// 			pList->AddItem(pListItem);
// 		}		

		//
// 		for (int j = 0; j < 1 ; ++j)
// 		{
// 			CComPtr<IUIFrame> pFrame ;
// 			Common::Util::CreateObject(&pFrame) ;
// 			pFrame->SetAutoSize(TRUE) ;
// 			
// 			CComPtr<IUILayoutHorizontal> pLayout ;
// 			Common::Util::CreateObject(&pLayout) ;
// 			pFrame->SetLayouter(pLayout) ;
// 
// 			for(int i = 0 ; i < 5 ; ++i)
// 			{
// 				CComPtr<IUIAnimate> pAnimate ;
// 				Common::Util::CreateObject(&pAnimate) ;
// 				pAnimate->SetSize(CSize(75, 86)) ;
// 
// 				for (int i = 1; i <= 6; i++)
// 				{
// 					CString str ;
// 					str.Format(_T("e:/gif-%d.png"),i) ; ;
// 					CComPtr<IUITexture> pTexture ;
// 					Common::Util::CreateObject(&pTexture) ;
// 					pTexture->LoadFromFile(CComBSTR(str)) ;
// 					pTexture->SetDrawType(TEXTURE::DRAW::HOLE) ;
// 					pAnimate->AddFrame(pTexture) ;
// 				}
// 				pFrame->AddChildBack(pAnimate) ;
// 			}
// 
// 			pWin->AddChildBack(pFrame) ;
// 		}
*/	
	
		CWeakPtr<IUIFactory> pFactory ;
		Common::Util::GetService(&pFactory) ;
		if (pFactory)
		{
			pFactory->LoadTagConfigFromFile(_T("./res/tag.xml")) ;
		}

		CComPtr<IUIElement> pElement ;
  		pFactory->CreateFromFile(_T("./res/ui.xml"), _T("Main"), &pElement) ;
  		CComQIPtr<IUIWin> pWin = pElement ;
  		if (pWin)
  		{
  			pWin->CreateWin(NULL, NULL) ;
			pWin->SetWinIcon(TRUE, ::LoadIcon(NULL, MAKEINTRESOURCE(IDI_ICON_MAIN)));
			//
 			pWin->SetTitle(_T("guodong")) ;
			//			

			CComPtr<IUILayoutVertical> pLayoutVer ;
			Common::Util::CreateObject(&pLayoutVer) ;
			pLayoutVer->SetFillItemIndex(0) ;
			pWin->SetLayouter(pLayoutVer) ;

  			pWin->ShowWin(SW_SHOW) ;
  		}

		CWeakPtr<IUIMsgLoop> wpLoop ;
		Common::Util::GetService(&wpLoop) ;
		wpLoop->StartLoop() ;
	}

	void AddFrame(IUIWin* pWin, BOOL bHorizontal)
	{
		/////////////////////
		CComPtr<IUIBrush> pBrush;
		Common::Util::CreateObject(&pBrush);
		pBrush->SetColor(RGB(55, 130, 226)) ;
		pBrush->SetBrushType(BRUSH::RECT_FILL) ;

		CComPtr<IUIBrush> pBrush2;
		Common::Util::CreateObject(&pBrush2);
		pBrush2->SetColor(RGB(155, 30, 26)) ;
		pBrush2->SetBrushType(BRUSH::DOTE_RECT_BORDER) ;

		//////////////////////
		CComPtr<IUIFrame> pFrame;
		Common::Util::CreateObject(&pFrame);

		//pFrame->SetAutoSize(FALSE) ;
		pFrame->SetAutoWidth(FALSE) ;//*******
		pFrame->SetAutoHeight(TRUE);
		pFrame->SetWidthAsParent(TRUE) ;
		pFrame->SetMargin(CRect(10,0,10,0)) ;
		pFrame->SetSize(CSize(0,60));
		pFrame->SetBkgDraw(pBrush) ;
		if(bHorizontal)
		{
			CComPtr<IUILayoutHorizontal> pLayout ;
			Common::Util::CreateObject(&pLayout) ;
			pLayout->SetFillItemIndex(1) ;
			pFrame->SetLayouter(pLayout) ;
		}
		else
		{
			CComPtr<IUILayoutVertical> pLayout;
			Common::Util::CreateObject(&pLayout);
			pLayout->SetFillItemIndex(1);
			pFrame->SetLayouter(pLayout);
		}

		/////////////////////
		CComPtr<IUITextField> pEdit;
		Common::Util::CreateObject(&pEdit);
		pEdit->SetAutoSize(FALSE);
		pEdit->SetSize(CSize(100,50));
		pEdit->SetSingleLine(FALSE);
		pEdit->SetText(_T("asbaseltw\nsdlwerwer"));
		pEdit->SetBkgDraw(pBrush2);

		pFrame->AddChildBack(pEdit);

		/////////////////////
		CComPtr<IUIButton> pButton;
		Common::Util::CreateObject(&pButton);
		pButton->SetAutoSize(FALSE);
		pButton->SetSize(CSize(180,26));
		pButton->SetText(_T("ButtonText"));
		pButton->SetBkgDraw(pBrush);
		pButton->SetBkgDrawAlpha(10);

		pFrame->AddChildBack(pButton);

		/////////////////////
		CComPtr<IUIStatic> pStatic;
		Common::Util::CreateObject(&pStatic);
		//pStatic->SetAutoHeight(TRUE);
		pStatic->SetSize(CSize(100,80));
		pStatic->SetTextColor(RGB(255,0,255));
		pStatic->SetText(_T("水电费了哦五色土\n换行\nserew"));
		pStatic->SetMutiLine(TRUE);

		pFrame->AddChildBack(pStatic);

		//////////////////////
		CComPtr<IUIAnimate> pAnimate;
		Common::Util::CreateObject(&pAnimate);
		pAnimate->SetSize(CSize(120,95));

		for ( int i=0; i<6; i++ )
		{
			CComPtr<IUITexture> pTexture;
			Common::Util::CreateObject(&pTexture);
			CString strPath;
			strPath.Format(_T("e:/gif-%d.png"), i+1);

			pTexture->LoadFromFile(CComBSTR(strPath));
			pTexture->SetDrawType(TEXTURE::DRAW::HOLE);
			pAnimate->AddFrame(pTexture);
		}
		
		pFrame->AddChildBack(pAnimate);

		//////////////
		pWin->AddChildBack(pFrame);
	}
};