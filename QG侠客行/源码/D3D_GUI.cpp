#include "DXUT.h"
#include"D3D_GUI.h"
//#include"d3d9.h"
#include"d3dx9.h"
#include<tchar.h>
BOOL GUICONTROL::OverButton(unsigned int x, unsigned int y)
{
	if (x > _xPos&& x<_xPos + _width&&y<_yPos&&y > _yPos - _height)
		return TRUE;
	return FALSE;

}
GUIClass::GUIClass()
{
	_device				 = NULL;
	_pFonts				 = NULL;
	_pControls			 = NULL;
	_pVertexBuffer		 = NULL;
	_Background			 = NULL;
	_BackgroundBuffer	 = NULL;
	_IdOfGUI			 = -1;
	_bIsBackgroundUsed   = false;
	_nTotalFontNum		 = 0;
	_nTotalControlNum	 = 0;
	_nTotalBufferNum	 = 0;				  
	_nWindowWidth		 = 1280;					  
	_nWindowHeight		 = 960;					 
}

void GUIClass::Clear()
{
	int i = 0;
	for (; i < _nTotalFontNum; i++)
	{
		if (_pFonts[i])
		{
			_pFonts[i]->Release();
			_pFonts[i] = NULL;
		}
	}
		//SAFE_RELEASE(_pFonts[i]);
	GUICONTROL *temp = _pControls;
	GUICONTROL *temp1 = temp;
	for (i = 0; i < _nTotalControlNum; i++)
	{
		SAFE_RELEASE(temp->_tex[UPTEX]);
		SAFE_RELEASE(temp->_tex[DOWNTEX]);
		SAFE_RELEASE(temp->_tex[OVERTEX]);
		temp = temp->next;
		temp1 = NULL;
		temp1 = temp;
	}
	for (i = 0; i < _nTotalBufferNum; i++)
		SAFE_RELEASE(_pVertexBuffer[i]);
	SAFE_DELETE_ARRAY(_pControls);
	SAFE_DELETE_ARRAY(_pFonts);
	SAFE_RELEASE(_BackgroundBuffer);
	SAFE_RELEASE(_Background);
	_IdOfGUI = -1;
	_bIsBackgroundUsed = false;
	_nTotalFontNum = 0;
	_nTotalControlNum = 0;
	_nTotalBufferNum = 0;
}		

BOOL GUIClass::AddBackGround( wchar_t * texname)
{
	if (!texname)
		return false;
	HRESULT hr;
	hr = D3DXCreateTextureFromFile(_device, texname, &_Background);
	if (FAILED(hr))
	{
		texname = NULL;
		::MessageBox(0, L"Loading Background texture error", 0, 0);
		return false;
	}
	// 获取窗口宽度和高度，以便稍后的背景铺满窗口
	float w = (float)_nWindowWidth;
	float h = (float)_nWindowHeight;
	GUIVERTEX obj[] =
	{
		{ -w/2, 0, 0.0f,  0.0f, 1.0f },
		{ -w/2, h, 0.0f,  0.0f, 0.0f },
		{ w/2, 0,  0.0f,  1.0f, 1.0f },
		{ w/2, h,  0.0f,  1.0f, 0.0f },
	};
	// 创建顶点缓存
	if (FAILED(_device->CreateVertexBuffer(sizeof(obj), 0, D3DFVF_GUI,
		D3DPOOL_DEFAULT, &_BackgroundBuffer, NULL))) {
		::MessageBox(0, L" Add back ground ,creat buffer error", 0, 0);
		return false;
	}
	// 填充顶点缓存
	void *ptr;
	if (FAILED(_BackgroundBuffer->Lock(0, sizeof(obj), (void**)&ptr, 0)))
	{
		::MessageBox(0, L"lock buffer error", 0, 0);
		return false;
	}
	memcpy(ptr, obj, sizeof(obj));
	_BackgroundBuffer->Unlock();
	_bIsBackgroundUsed = true;
	return true;
};
BOOL GUIClass::AddButton(int id, float x, float y, wchar_t *up, wchar_t *over, wchar_t *down)
{
	if ( !up || !over || !down)
		return false;
	GUICONTROL * abutton = new GUICONTROL;
	if (!_pControls)
		_pControls = abutton;
	else
		_pControls->next = abutton;
	abutton->_id = id;
	abutton->_type = ISBUTTON;
	abutton->_xPos = x;
	abutton->_yPos = y;
	abutton->_listID = _nTotalBufferNum;
	if (FAILED(D3DXCreateTextureFromFile(_device, up,  &abutton->_tex[UPTEX])))
	{
		::MessageBox(0, L"Create Button texture[Up] error", 0, 0);
		return false;
	}
	if (FAILED(D3DXCreateTextureFromFile(_device, over, &abutton->_tex[OVERTEX])))
	{
		::MessageBox(0, L"Create Button texture[Over] error", 0, 0);
		return false;
	}
	if (FAILED(D3DXCreateTextureFromFile(_device, down, &abutton->_tex[DOWNTEX])))
	{
		::MessageBox(0, L"Create Button texture[Down] error", 0, 0);
		return false;
	}
	D3DSURFACE_DESC desc;
	abutton->_tex[UPTEX]->GetLevelDesc(0, &desc);
	float w =  (float)desc.Width;
	float h = (float)desc.Height;
	abutton->_width = w;
	abutton->_height = h;
	GUIVERTEX obj[] =
	{
		{ 0 + x, y + 0,  0.0f, 0.0f, 0.0f },
		{ 0 + x, y - h,  0.0f, 0.0f, 1.0f },
		{ w + x, y + 0,  0.0f, 1.0f, 0.0f },
		{ w + x, y - h,  0.0f, 1.0f, 1.0f },
	};
	//创建顶点缓存
	if (!_pVertexBuffer)
	{
		_pVertexBuffer = new LPDIRECT3DVERTEXBUFFER9[1];
		if (!_pVertexBuffer) return false;
	}
	else
	{
		LPDIRECT3DVERTEXBUFFER9 *temp;
		temp = new LPDIRECT3DVERTEXBUFFER9[_nTotalBufferNum + 1];
		if (!temp) return false;
		memcpy(temp, _pVertexBuffer,
			sizeof(LPDIRECT3DVERTEXBUFFER9) * _nTotalBufferNum);

		delete[] _pVertexBuffer;
		_pVertexBuffer = temp;
	}
	if (FAILED(_device->CreateVertexBuffer(sizeof(obj), 0,
		D3DFVF_GUI, D3DPOOL_DEFAULT, &_pVertexBuffer[_nTotalBufferNum], NULL)))
	{
		::MessageBox(0, L"Create Button buffer error", 0, 0);
		return false;
	}
	//填充顶点缓存
	void *ptr;
	if (FAILED(_pVertexBuffer[_nTotalBufferNum]->Lock(0, sizeof(obj), (void**)&ptr, 0)))
	{
		return false;
	}
	memcpy(ptr, obj, sizeof(obj));
	_pVertexBuffer[_nTotalBufferNum]->Unlock();
	// 自增
	_nTotalBufferNum++;
	_nTotalControlNum++;
	return true;
}
BOOL GUIClass::AddText(int id, wchar_t *text, float x, float y, unsigned long color, int fontID)
{
	if (!text || fontID < 0 || fontID >= _nTotalFontNum) return false;
	if (x < 0 || y < 0)
		return false;
	//下面的代码用于创建一个控件
	PGUICONTROL abutton = new GUICONTROL[1];
	if (!_pControls)
		_pControls = abutton;
	else
		_pControls->next = abutton;
	// 填充我们需要的文本信息
	abutton->_type = ISTEXT;
	abutton->_id = id;
	abutton->_color = color;
	abutton->_xPos = x;
	abutton->_yPos = y;
	abutton->_listID = fontID;
	// 复制文本数据 
	int len = wcslen(text);
	abutton->_wctext = text;//new wchar_t[len + 2];
	_nTotalControlNum++;
}
BOOL GUIClass::CreateTextFont(wchar_t *fontName, int size, int *fontID)
{
	if (!fontName)
		return false;
	if (!_pFonts)
	{
		_pFonts = new LPD3DXFONT[1];
		if (!_pFonts) return false;
	}
	else
	{
		LPD3DXFONT *temp;
		temp = new LPD3DXFONT[_nTotalFontNum + 1];
		if (!temp) return false;

		memcpy(temp, _pFonts,
			sizeof(LPD3DXFONT) * _nTotalFontNum);
		delete[] _pFonts;
		_pFonts = temp;
	}
	// 调用D3DXCreateTextFont函数创建字体
	D3DXCreateFont(_device, size, 0, 0, 1000, 0,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		0,fontName,
		&_pFonts[_nTotalFontNum]);
	if (!_pFonts[_nTotalFontNum]) return false;

	// 存下字体ID和ID计数。
	if (fontID) *fontID = _nTotalFontNum;
	_nTotalFontNum++;
	return true;
}

 //回调函数
void ProcessGUI(GUIClass *gui, bool mousedown, int mouseX, int mouseY,
	void(*funcPtr)(int id, int state)) 
{
	if (!gui) return;
	LPDIRECT3DDEVICE9 device = gui->GetDevice();
	if (!device) return;
	// 绘制背景
	LPDIRECT3DTEXTURE9 pBackgroundTex = gui->GetBackgroundTex();
	LPDIRECT3DVERTEXBUFFER9 pBackgroundBuffer = gui->GetBackgroundBuffer();

	//已经创建出的东西才绘制，所以来个if
	if (gui->IsBackgroundUsed() && pBackgroundTex && pBackgroundBuffer)
	{
		device->SetTexture(0, pBackgroundTex);
		device->SetStreamSource(0, pBackgroundBuffer, 0, sizeof(GUIVERTEX));
		device->SetFVF(D3DFVF_GUI);
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		device->SetTexture(0, NULL);
	}
	//用来显示文本的对象
	LPD3DXFONT pFont = NULL;
	
		RECT fontPosition = { 0, 0, (long)(gui->GetWindowWidth()),
		(long)gui->GetWindowHeight() };

	// 创建一个顶点缓存对象用于按钮的渲染
	LPDIRECT3DVERTEXBUFFER9 pBuffer = NULL;
	int mousestatus = MOUSE_UP;
	GUICONTROL *pControl = gui->GetGUIControl();
	// 一个循环，用于各种控件的渲染
	for (int i = 0; i < gui->GetTotalControlNum(); i++)
	{
		if (!pControl) continue;
		// 根据不同的类型做不同的操作
		switch (pControl->_type)
		{
		case ISTEXT:
			// 这种情况下获取字体对象
			pFont = gui->GetFont(pControl->_listID);
			if (!pFont) continue;

			// 设置文字位置
			fontPosition.left = pControl->_xPos;
			fontPosition.top = pControl->_yPos;

			// 显示文字
			pFont->DrawText(NULL, pControl->_wctext, -1, &fontPosition,
				DT_LEFT, pControl->_color);
			break;
		case ISBUTTON:
			mousestatus = MOUSE_UP;
			//::MessageBox(0, L"RELEASE FONET", 0, 0);
			//获取按钮所对应的顶点缓存对象
			pBuffer = gui->GetVertexBuffer(pControl->_listID);
			if (!pBuffer) continue;

			// 设置纹理的alpha透明选项
			device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			if(pControl->OverButton(mouseX,mouseY))
			//检查鼠标是否悬停或者点击了按钮
			{
				if (mousedown) mousestatus = MOUSE_DOWN;
				else mousestatus =  MOUSE_OVER;
			}

			//根据不同的鼠标和按钮之间缠绵悱恻的状态来准备不同的纹理图 
			if (mousestatus == MOUSE_UP) device->SetTexture(0, pControl->_tex[UPTEX]);
			if (mousestatus == MOUSE_OVER) device->SetTexture(0, pControl->_tex[OVERTEX]);
			if (mousestatus == MOUSE_DOWN) device->SetTexture(0, pControl->_tex[DOWNTEX]);
			//::MessageBox(0, L"STAR RENDER", 0, 0);
			// 万事俱备，开始渲染按钮
			device->SetStreamSource(0, pBuffer, 0, sizeof(GUIVERTEX));
			device->SetFVF(D3DFVF_GUI);
			device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

			// 关闭alpha混合.
			device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			break;
		default:
			::MessageBox(0, L"Control type error", 0, 0);
		}
		//调用回调函数处理控件消息
		if (funcPtr) funcPtr(pControl->_id, mousestatus);
		pControl = pControl->next;
	}
}