#include"DXUT.h"
#include"Game_GUI.h"


void GUISetTexU(LPDIRECT3DVERTEXBUFFER9* buffer, UINT texset)
{
	HRESULT hr;
	GUIVERTEX* v;
	V((*buffer)->Lock(0,0,(void**)&v,0));
	float offset = 1.0f / 3;
	v[0]._u = v[1]._u = texset*offset;
	v[2]._u = v[3]._u = (texset + 1)*offset;
	(*buffer)->Unlock();
}

void GUICONTROL::SetTextureU(UINT texNum)
{
	HRESULT hr;
	GUIVERTEX* v;
	V((_buffer)->Lock(0, 0, (void**)&v, 0));
	float offset = 1.0f / 3;
	v[0]._u = v[1]._u = texNum*offset;
	v[2]._u = v[3]._u = (texNum + 1)*offset;
	_buffer->Unlock();
}
void GUICONTROL::Move(float x, float y)
{
	HRESULT hr;
	GUIVERTEX* v;
	V((_buffer)->Lock(0, 0, (void**)&v, 0));
	v[0]._x += x;	v[0]._y += y;
	v[1]._x += x;	v[1]._y += y;
	v[2]._x += x;	v[2]._y += y;
	v[3]._x += x;	v[3]._y += y;
	_buffer->Unlock();

	_area.top	 += y;
	_area.bottom += y;
	_area.left	 += x;
	_area.right  += x;
}

void GUICONTROLGROUP::Move(float x, float y)
{
	HRESULT hr;
	GUIVERTEX* v;
	V((_buffer)->Lock(0, 0, (void**)&v, 0));
	v[0]._x += x;	v[0]._y += y;
	v[1]._x += x;	v[1]._y += y;
	v[2]._x += x;	v[2]._y += y;
	v[3]._x += x;	v[3]._y += y;
	_buffer->Unlock();

	_area.top += y;
	_area.bottom += y;
	_area.left += x;
	_area.right += x;

	for (UINT i = 0; i < _controls.size(); i++)
		_controls[i]->Move(x, y);
}

void GUICONTROLGROUP::Move(float timeDelta)
{
	if (_area.bottom < _limitArea.bottom)
	{
		Move(0, timeDelta * 1);
			return;
	}
	if (_area.top > _limitArea.top)
	{
		Move(0, -timeDelta * 1);
		return;
	}
	if (_area.left < _limitArea.left)
	{
		Move(timeDelta * 1, 0);
		return;
	}
	if (_area.right > _limitArea.right)
		Move(-timeDelta * 1, 0);
	//判断前进还是后退
	if (_IsMoveBack)
		Move(-_velocity.x*timeDelta, -_velocity.y*timeDelta);
	else
		Move(_velocity.x*timeDelta, _velocity.y*timeDelta);
}
GUIBasicClass::GUIBasicClass()
{
	_sound = NULL;
	_device = NULL;
	_backBuffer = NULL;
	_texBackGround = NULL;
	_nTotalBufferNum = 0;
	_nTotalControlNum = 0;
	_kW = _kH = 1.0f;
	_ISUseBackGround = false;
}
//初始化
HRESULT GUIBasicClass::Init(LPDIRECT3DDEVICE9	device, CSoundManager *csound, GUITag tag, UINT w,UINT h,
						void(*funcPtr)(GUIControlTag conTag, int state))
{
	_sound  = csound;
	_device = device;
	_guiTag = tag;
	_originScrW = _nWindowWidth = w;
	_originScrH = _nWindowHeight = h;
	_guiProcessFun= funcPtr;
	return S_OK;
}

//增加背景
HRESULT GUIBasicClass::AddBackGround(wchar_t* backtex)
{
	HRESULT hr;
	if (backtex == NULL)
		return E_FAIL;
	V(D3DXCreateTextureFromFile(_device, backtex,&_texBackGround));
	float w = _nWindowWidth, h = _nWindowHeight;
	GUIVERTEX obj[] =
	{
		{0.0f, 0.0f,   0.0f, 0.0f },
		{0.0f, h  ,    0.0f,1.0f },
		{ w ,  0.0f,   1.0f,0.0f },
		{ w ,  h ,     1.0f, 1.0f },
	};
	// 创建顶点缓存
	V_RETURN(_device->CreateVertexBuffer(sizeof(obj), 0, D3DFVF_GUI,
		D3DPOOL_MANAGED, &_backBuffer, NULL));
	// 填充顶点缓存
	void *ptr;
	V_RETURN(_backBuffer->Lock(0, sizeof(obj), (void**)&ptr, 0));
	memcpy(ptr, obj, sizeof(obj));
	_backBuffer->Unlock();
	_ISUseBackGround = true;
	return hr;
}

//增加一个按钮
HRESULT GUIBasicClass::AddButton(GUIControlTag tag, wchar_t *filename, RECT rec)
{
	HRESULT hr;
	GUICONTROL *control = new GUICONTROL;
	control->_id = tag;
	control->_type = ISCONTROLER;
	control->_area = rec;
	float h = (float)(rec.top - rec.bottom);
	float w = (float)(rec.right - rec.left);
	V(D3DXCreateTextureFromFile(_device, filename, &control->_tex));
	GUIVERTEX obj[] =
	{
		{ (float)rec.left,	 (float)rec.top ,	  0.0f,0.0f },
		{ (float)rec.left,	 (float)rec.bottom,		  0.0f, 1.0f },
		{ (float)rec.right ,(float)rec.top,		  1.0f,0.0f },
		{ (float)rec.right ,(float)rec.bottom ,     1.0f, 1.0f },
	};
	// 创建顶点缓存
	V_RETURN(_device->CreateVertexBuffer(sizeof(obj), 0, D3DFVF_GUI,
		D3DPOOL_MANAGED, &control->_buffer, NULL));
	// 填充顶点缓存
	void *ptr;
	V_RETURN((control->_buffer)->Lock(0, sizeof(obj), (void**)&ptr, 0));
	memcpy(ptr, obj, sizeof(obj));
	(control->_buffer)->Unlock();
	_pControls.push_back(control);
	//控件数自增
	_nTotalControlNum++;
	return hr;
}

//创建字体
HRESULT GUIBasicClass::CreateFont(wchar_t * fontname, int size, int *fontID)
{
	HRESULT hr;
	LPD3DXFONT* font = new LPD3DXFONT;
	V(D3DXCreateFont(_device, size, 0, 0, 1000, 0,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		0, fontname,
		font));
	// 存下字体ID和ID计数。
	_pFonts.push_back(font);
	if (fontID)
		*fontID = _pFonts.size() - 1;
	_nTotalFontNum++;
	return hr;
}

//增加文字
void GUIBasicClass::AddText(wchar_t *text, RECT rec, D3DCOLOR color, int fontID)
{
	if (text == NULL)
		return ;
	GUICONTROL* temp = new GUICONTROL;
	swprintf_s(temp->_text, 256, L"%ws", text);
	temp->_type = ISTEXT;
	temp->_fontID = fontID;
	temp->_area = rec;
	temp->_color = color;
	_pControls.push_back(temp);
	_nTotalControlNum++;//控件数自增

}

void GUIBasicClass::AddTextWithTag(wchar_t *text, RECT rec, D3DCOLOR color, int fontID, int * textTag)
{
	AddText(text,rec,color,fontID);
	*textTag = _pControls.size() - 1;
}

//只是增加图片
HRESULT GUIBasicClass::AddHUDPIC(wchar_t *filename, RECT rec)
{
	HRESULT hr;
	GUICONTROL *control = new GUICONTROL;
	control->_type = ISHUDPIC;
	control->_area = rec;
	V(D3DXCreateTextureFromFile(_device, filename, &control->_tex));
	control->_fontID = _nTotalBufferNum;
	GUIVERTEX obj[] =
	{
		{ (float)rec.left,	 (float)rec.top ,	  0.0f,0.0f },
		{ (float)rec.left,	 (float)rec.bottom,		  0.0f, 1.0f },
		{ (float)rec.right ,(float)rec.top,		  1.0f,0.0f },
		{ (float)rec.right ,(float)rec.bottom ,     1.0f, 1.0f },
	};
	// 创建顶点缓存
	V_RETURN(_device->CreateVertexBuffer(sizeof(obj), 0, D3DFVF_GUI,
		D3DPOOL_MANAGED, &control->_buffer, NULL));
	// 填充顶点缓存
	void *ptr;
	V_RETURN((control->_buffer)->Lock(0, sizeof(obj), (void**)&ptr, 0));
	memcpy(ptr, obj, sizeof(obj));
	control->_buffer->Unlock();
	_pControls.push_back(control);
	//buffer数和控件数自增
	_nTotalControlNum++;
	return hr;
}

void GUIBasicClass::AddPICWithTag(wchar_t *filename, RECT rec, int* picTag)
{
	AddHUDPIC(filename, rec);
	*picTag = _pControls.size() - 1;
}

//增加一个控件组
HRESULT GUIBasicClass::AddControlGroup(GUIControlGroupTag tag, wchar_t* texName, D3DXVECTOR3 v, RECT area, RECT limitArea)
{
	HRESULT hr;
	GUICONTROLGROUP* group = new GUICONTROLGROUP;
	group->_groupTag = tag;
	group->_area = area;
	group->_limitArea = limitArea;
	
	V(D3DXCreateTextureFromFile(_device, texName, &group->_tex));
	GUIVERTEX obj[] =
	{
		{ (float)area.left,	 (float)area.top ,	  0.0f,0.0f },
		{ (float)area.left,	 (float)area.bottom,		  0.0f, 1.0f },
		{ (float)area.right ,(float)area.top,		  1.0f,0.0f },
		{ (float)area.right ,(float)area.bottom ,     1.0f, 1.0f },
	};
	// 创建顶点缓存
	V_RETURN(_device->CreateVertexBuffer(sizeof(obj), 0, D3DFVF_GUI,
		D3DPOOL_MANAGED, &group->_buffer, NULL));
	// 填充顶点缓存
	void *ptr;
	V_RETURN((group->_buffer)->Lock(0, sizeof(obj), (void**)&ptr, 0));
	memcpy(ptr, obj, sizeof(obj));
	group->_buffer->Unlock();
	_pConGroups.push_back(group);
	return hr;
	

}

HRESULT GUIBasicClass::AddControlToGroup(GUIControlGroupTag gtag, GUIControlTag tag, wchar_t *filename, RECT rec)
{
	HRESULT hr;
	bool find = false;
	V_RETURN(AddButton(tag, filename, rec));
	UINT i;
	for (i = 0; i < _pConGroups.size(); i++)
	{
		if (_pConGroups[i]->_groupTag == gtag)
		{
			find = true;
			break;
		}
	}
	if (!find)
	{
		::MessageBox(0, L"AddConGroup Failed, false: Find Group tag", 0, 0);
		return S_FALSE;
	}
	//将最新加上的控件挂载上相应的控件组上
	(_pConGroups[i]->_controls).push_back(_pControls[_pControls.size()-1]);
	return S_OK;
}

void GUIBasicClass::Render(bool mousedown, int mouseX, int mouseY)
{

	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);//打开ALPHA渲染
	_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	if (_ISUseBackGround)
	{
		_device->SetTexture(0, _texBackGround);
		_device->SetStreamSource(0, _backBuffer, 0, sizeof(GUIVERTEX));
		_device->SetFVF(D3DFVF_GUI);
		_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		_device->SetTexture(0, NULL);
	}
	LPD3DXFONT pFont = NULL;
	int mousestatus = MOUSE_UP;
	RECT* r;
	GUIControlTag controlID = GCT_NONE;
	_device->SetFVF(D3DFVF_GUI);
	
	for (UINT i = 0; i < _pControls.size(); i++)
	{
		switch (_pControls[i]->_type)
		{
			case ISTEXT:
				pFont = *_pFonts[_pControls[i]->_fontID];
				pFont->DrawText(NULL, _pControls[i]->_text, -1, &_pControls[i]->_area,
					DT_RIGHT, _pControls[i]->_color);
				break;
			case ISCONTROLER:
				r = &_pControls[i]->_area;
				_pControls[i]->SetTextureU(0);
				if (mouseX >= r->left&&mouseX <= r->right&&mouseY >= r->top&&mouseY <= r->bottom)//在控件之上
				{
					controlID = _pControls[i]->_id;
					if (mousedown)
						mousestatus = MOUSE_DOWN,_pControls[i]->SetTextureU(2);
					else
						mousestatus = MOUSE_OVER,_pControls[i]->SetTextureU(1);
				}
						
				_device->SetTexture(0, _pControls[i]->_tex);
				_device->SetStreamSource(0, _pControls[i]->_buffer, 0, sizeof(GUIVERTEX));
				_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
				break;
				
			case ISHUDPIC:
				_device->SetTexture(0,_pControls[i]->_tex);
				_device->SetStreamSource(0, _pControls[i]->_buffer, 0, sizeof(GUIVERTEX));
				_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);	
				break;
		}
	}
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	_guiProcessFun(controlID, mousestatus);
}



//------------------------------------------------------------
//HUD
//------------------------------------------------------------


HRESULT HUDBasicClass::AddPic(wchar_t* texName, RECT rec)
{
	HRESULT hr;
	HUD_INFOCONTROL * control = new HUD_INFOCONTROL;
	control->_area = rec;
	control->_type = HUD_ISPICTURE;
	V_RETURN(D3DXCreateTextureFromFile(_device, texName, &control->_tex));
	HUDVERTEX obj[] =
	{
		{ (float)rec.left,	 (float)rec.top ,	  0.0f,0.0f },
		{ (float)rec.left,	 (float)rec.bottom,		  0.0f, 1.0f },
		{ (float)rec.right ,(float)rec.top,		  1.0f,0.0f },
		{ (float)rec.right ,(float)rec.bottom ,     1.0f, 1.0f },
	};
	// 创建顶点缓存
	V_RETURN(_device->CreateVertexBuffer(sizeof(obj), 0, D3DFVF_HUD,
		D3DPOOL_MANAGED, &control->_buffer, NULL));
	// 填充顶点缓存
	void *ptr;
	V_RETURN((control->_buffer)->Lock(0, sizeof(obj), (void**)&ptr, 0));
	memcpy(ptr, obj, sizeof(obj));
	(control->_buffer)->Unlock();
	_controls.push_back(control);
	return hr;
}

HRESULT HUDBasicClass::AddLifeValuePic(wchar_t* filename, RECT rec)
{
	HRESULT hr;
	V_RETURN(AddPic(filename, rec));
	UINT i = _controls.size();
	_controls[i - 1]->_type = HUD_ISLIFEVALUE;
	return hr;
}


HRESULT HUDBasicClass::AddSkill(SkillTag skilltag, wchar_t* filename, RECT rec)
{
	HRESULT hr;
	V_RETURN(AddPic(filename, rec));
	UINT i = _controls.size();
	_controls[i - 1]->_type = HUD_ISSKILL;
	_controls[i - 1]->_skill = skilltag;
	return hr;
}

void HUDBasicClass::AddText(wchar_t *text, RECT rec, D3DCOLOR color, int fontID)
{
	HUD_TEXT* htext = new HUD_TEXT;
	htext->_fontID = fontID;
	htext->_color = color;
	htext->_area = rec;
	htext->_textTag = -999;
	swprintf_s(htext->_text, 256, L"%s", text);
	_texts.push_back(htext);
}

HRESULT HUDBasicClass::CreateFont(wchar_t * fontname, int size, int *fontID)
{
	HRESULT hr;
	LPD3DXFONT* font = new LPD3DXFONT;
	V(D3DXCreateFont(_device, size, 0, 0, 1000, 0,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		0, fontname,
		font));
	// 存下字体ID和ID计数。
	_fonts.push_back(font);
	if (fontID) 
		*fontID = _fonts.size() -1;
	return hr;
}

void HUDBasicClass::InitDistance(int fontID, RECT rect, D3DCOLOR color)
{
	_runDistance = 0;
	_runDistanceTag = -998;
	wchar_t* text = L"0";
	AddText(text, rect, color, fontID);
	_runDistanceTag = _texts.size() - 1;
}

void HUDBasicClass::UpdateDistance(long distance)
{
	_runDistance = distance;
	swprintf_s(_texts[_runDistanceTag]->_text,
		256,
		L"%ld", _runDistance);
}

void HUDBasicClass::ChangeSkillState(SkillTag tag, bool hasIt)
{
	vector<HUD_INFOCONTROL*>::iterator   coniter;
	for (coniter = _controls.begin(); coniter != _controls.end(); coniter++)
	{
		if ((*coniter)->_skill == tag)
			(*coniter)->_hasSkill = hasIt;
	}
}

void HUDBasicClass::Render()
{
	_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	_device->SetFVF(D3DFVF_HUD);

	HUDVERTEX *v;
	for (UINT i = 0; i < _controls.size(); i++)
	{
		switch(_controls[i]->_type)
		{
		case HUD_ISPICTURE:
			_device->SetTexture(0,_controls[i]->_tex);
			_device->SetStreamSource(0, _controls[i]->_buffer, 0, sizeof(HUDVERTEX));
			_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
			break;
		case HUD_ISSKILL://技能，没有就半透明
				_controls[i]->_buffer->Lock(0, 0, (void**)&v, 0);
				for (int j = 0; j < 4; j++)
				{
					if (_controls[i]->_hasSkill)
						v[j].color = D3DCOLOR_ARGB(255, 255, 255, 255);
					else
						v[j].color = D3DCOLOR_ARGB((int)(0.3 * 255), 255, 255, 255);
					
				}
				_controls[i]->_buffer->Unlock();
			_device->SetTexture(0, _controls[i]->_tex);
			_device->SetStreamSource(0, _controls[i]->_buffer, 0, sizeof(HUDVERTEX));
			_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
			break;

		case HUD_ISLIFEVALUE://生命值,如果大于1，多次绘制
			if (_lifeValue > 0)
			{	
			
				_device->SetTexture(0, _controls[i]->_tex);
				
				float width = _controls[i]->_area.right - _controls[i]->_area.left;
				for (int j = 0; j < _lifeValue; j++)
				{		
						_controls[i]->_buffer->Lock(0, 0, (void**)&v, 0);
						v[0]._x =v[1]._x =  _controls[i]->_area.left+(width*j);
						v[2]._x= v[3]._x =  _controls[i]->_area.right+(width*j);
						_controls[i]->_buffer->Unlock();
						_device->SetStreamSource(0, _controls[i]->_buffer, 0, sizeof(HUDVERTEX));
						_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
				}
				
			}
			break;
		}
	}
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	LPD3DXFONT pFont = NULL;
	HRESULT hr;

	for (UINT i = 0; i < _texts.size(); i++)
	{
		pFont = *_fonts[_texts[i]->_fontID];
		V(pFont->DrawText(NULL, _texts[i]->_text, -1, &_texts[i]->_area,
			DT_RIGHT, _texts[i]->_color));
	}
}

void HUDBasicClass::LoadSource()
{
	RECT rec;
	CreateFont(L"陈继世-怪怪体", 60, &_Font_1);
	rec = MakeRect(530, 50, 280, 100);
	InitDistance(_Font_1, rec, D3DCOLOR_ARGB(255, 255, 255, 255));

	rec = MakeRect(810, 50, 61, 61);
	AddText(L"M", rec, D3DCOLOR_ARGB(255, 194, 0, 0), _Font_1);

	rec = MakeRect(550, 5, 400, 160);
	AddPic(L"Media//卷轴.png", rec);

	rec = MakeRect(10, 15, 300, 100);
	AddPic(L"Media//血量背景.png", rec);

	rec = MakeRect(35, 45, 42, 42);
	AddLifeValuePic(L"Media//血量1.png", rec);

	rec = MakeRect(25, 550, 100, 100);
	AddSkill(SKILL_Shield, L"Media//护盾.png", rec);

	rec = MakeRect(115, 580, 100, 100);
	AddSkill(SKILL_Dart, L"Media//飞镖.png", rec);

	rec = MakeRect(25, 650, 130, 130);
	AddSkill(Skill_Dash, L"Media//冲锋.png", rec);


	rec = MakeRect(170, 660, 100, 100);
	AddSkill(SKILL_SwordKee, L"Media//剑气.png", rec);

}


//--------------------------------------------------------------------------
//结算界面
//--------------------------------------------------------------------------

CalGUI::CalGUI()
{
	c_defeatBossTag = -100;
	c_goldNumTag = -101;
	c_silverNumTag = -102;
	c_distanceTag = -103;

	_distance = _distanceT = 0;
	_goldNum =	_goldNumT = 0;
	_silverNum = _silverNumT = 0;
}

void CalGUI::InitDigital()
{
	_distance = _distanceT = 0;
	_goldNum = _goldNumT = 0;
	_silverNum = _silverNumT = 0;
}

void CalGUI::SetDefeatBoss(bool b)
{
	if (b)
		swprintf_s(_pControls[c_defeatBossTag]->_text, L"%s", L"击杀");
	else
		swprintf_s(_pControls[c_defeatBossTag]->_text, L"%s", L"无");
}


void CalGUI::Update(float timeDelta)
{
	if (_distanceT < _distance)
	{
		_distanceT += (LONG)(2000 * timeDelta);
		if (_distanceT > _distance)
			_distanceT = _distance;
		SetDistanceText(_distanceT);
	}

	if (_goldNumT < _goldNum)
	{
		_goldNumT += (UINT)(100*timeDelta);
		if (_goldNumT > _goldNum)
			_goldNumT = _goldNum;
		SetGoldCoinNumText(_goldNumT);
	}

	if (_silverNumT < _silverNum)
	{
		_silverNumT += (UINT)(130 * timeDelta);
		if (_silverNumT > _silverNum)
			_silverNumT = _silverNum;
		SetSilverNumText(_silverNumT);
	}
}

void CalGUI::LoadSource(LPDIRECT3DDEVICE9	device, CSoundManager *csound, UINT w, UINT h,
	void(*funcPtr)(GUIControlTag conTag, int state))
{
	InitDigital();
	Init(device, csound, GUI_CAL, w, h, funcPtr);
	float tsize = 60;
	float tsize2 = 80;
	CreateFont(L"陈继世-怪怪体", tsize, &Font_1);
	//CreateFont(L"陈继世-怪怪体", tsize2, &Font_2);
	RECT rec;

	float jw = 1100;
	float jh = jw / 2.67;
	rec = MakeRect(170, 160, jw, jh);
	AddHUDPIC(L"Media//结算背景卷轴.png", rec);

	rec = MakeRect(950, 400, 100, 100);
	AddButton(GCT_GOTOMENU, L"Media//主菜单按钮.png", rec);

	rec = MakeRect(1050, 400, 100, 100);
	AddButton(GCT_RESTAR, L"Media//重置按钮.png", rec);
	//金银币
	rec = MakeRect(330, 330, tsize * 3, tsize);
	AddTextWithTag(L"0", rec, D3DCOLOR_ARGB(255, 255, 255, 255), Font_1, &c_goldNumTag);

	rec = MakeRect(330, 430, tsize * 3, tsize);
	AddTextWithTag(L"0", rec, D3DCOLOR_ARGB(255, 255, 255, 255), Font_1, &c_silverNumTag);

	rec = MakeRect(210, 250, tsize * 4, tsize);
	AddTextWithTag(L"1000000", rec, D3DCOLOR_ARGB(255, 255, 255, 255), Font_1, &c_distanceTag);

	rec = MakeRect(1094, 250, tsize, tsize);
	AddTextWithTag(L"无", rec, D3DCOLOR_ARGB(255, 200, 5, 25), Font_1, &c_defeatBossTag);


}

void GUIBasicClass::LoadSourceMenu()
{
	RECT rec;
	AddBackGround(L"Media//主界面背景.png");
	
	rec = MakeRect(-50, -50, 770, 900);
	AddHUDPIC(L"Media//主界面房顶.png", rec);

	rec = MakeRect(1000, -300, 700, 700);
	AddHUDPIC(L"Media//月亮.png", rec);

	rec = MakeRect(275, 335, 300, 300);
	AddHUDPIC(L"Media//主界面忍者1.png", rec);

	rec = MakeRect(700, 270, 850, 430);
	AddHUDPIC(L"Media//控件组背景水平.png", rec);


	rec = MakeRect(840, 365, 300, 115);
	AddButton(GCT_ScoreRecord, L"Media//分数记录.png", rec);

	rec = MakeRect(840, 480, 300, 120);
	AddButton(GCT_HELP, L"Media//游戏帮助.png", rec);

	rec = MakeRect(1140, 365, 300, 115);
	AddButton(GCT_MAKEINFO, L"Media//游戏信息.png", rec);

	rec = MakeRect(1140, 480, 300, 115);
	AddButton(GCT_QUIT, L"Media//退出游戏.png", rec);

	rec = MakeRect(1000, 600, 100, 100);
	AddButton(GCT_STAR, L"Media//开始按钮.png", rec);
}