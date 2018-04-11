#pragma once
#include"DXUT.h"
#ifndef _GAME_GUI_GUI_H
#define _GAME_GUI_GUI_H
#include "DXUT.h"
#include<d3dx9.h>
#include<vector>
#include<algorithm>
#include"Game_BasicClass.h"
#include"Myd3d.h"
#define GUI_UP		0
#define GUI_OVER	1
#define GUI_DOWN	2

#define MOUSE_UP   0
#define MOUSE_DOWN 1
#define MOUSE_OVER 2

using std::vector;
enum GUITag
{
	GUI_MAIN, GUI_OPTION, GUI_PAUSE,GUI_QUIT,GUI_CAL
};

enum GUIControlTag
{
	GCT_STAR, GCT_QUIT, GCT_PAUSE, GCT_RESTAR,GCT_NONE,GCT_ScoreRecord,GCT_MAKEINFO,GCT_HELP,
	GCT_GOTOMENU
};

enum GUIControlGroupTag
{
	GUI_GROUP_FIRST, GUI_GROUP_SECOND
};

enum GUIControlType
{
	ISCONTROLER,ISTEXT,ISHUDPIC,ISCONGROUP
};

enum GUIControlEven
{
	GCE_OVER,GCE_STAR,GCE_PAUSE
};


enum HUDControlType
{
	HUD_ISTEXT, HUD_ISSKILL, HUD_ISLIFEVALUE, HUD_ISPICTURE
};

enum HUDTextTag
{
	HUD_TEXT_LIFEVALUE
};

enum GAMEPROCESS
{
	GAME_PLAYING, GAME_OVER, GAME_READY, GAME_QUIT
};

struct GUIVERTEX
{
	float _x, _y, _z, _h;
	float _u, _v;
	GUIVERTEX(float x, float y, float u, float v)
	{
		_x = x, _y = y, _z = -1.0f, _u = u, _v = v, _h = 2.0f;
	}
};
#define D3DFVF_GUI (D3DFVF_XYZRHW | D3DFVF_TEX1)

typedef struct HUDVERTEX
{
	float _x, _y, _z, _h;
	D3DCOLOR color;
	float _u, _v;
	HUDVERTEX(float x, float y, float u, float v)
	{
		_x = x, _y = y, _z = -1.0f, _u = u, _v = v, _h = 2.0f;
		color = D3DCOLOR_ARGB(255, 255, 255, 255);
	}
}HUDVERTEX;
#define D3DFVF_HUD (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

struct GUICONTROL
{
	//操作类型，ID和颜色  
	LPDIRECT3DVERTEXBUFFER9 _buffer;	
	LPDIRECT3DTEXTURE9 _tex;   // 分别存放控件背景的填充图像,按钮弹起，按下和鼠标经过时的3张纹理图
	GUIControlType _type;  //控件类型  ,文字还是其他
	GUIControlTag _id;  //控件ID  
	D3DCOLOR _color; //控件颜色  
	RECT	_area;//控件覆盖区域
	wchar_t _text[256];		// 文字内容 
	int _fontID;   //如果是文字的话，这个变量就表示它使用的字体
	int _controlTag;
	void  Move(float x,float y);
	void SetTextureU(UINT texNum);
	BOOL OverButton(unsigned int x, unsigned int y);
	~GUICONTROL() { SAFE_RELEASE(_buffer); SAFE_RELEASE(_tex); }
};


struct GUICONTROLGROUP
{
	GUIControlGroupTag _groupTag;
	vector<GUICONTROL*> _controls;
	LPDIRECT3DVERTEXBUFFER9 _buffer;
	LPDIRECT3DTEXTURE9	_tex;
	D3DXVECTOR3 _velocity;
	RECT _area;
	RECT _limitArea;
	BOOL _IsMoveBack;
	float _moveMaxLength;
	float _moveLength;
	void Move(float x, float y);
	void Move(float timeDelta);
	~GUICONTROLGROUP() { SAFE_RELEASE(_buffer); }
};




class GUIBasicClass
{
protected:
	LPDIRECT3DDEVICE9			_device;
	CSoundManager *				_sound;
	LPDIRECT3DVERTEXBUFFER9		_backBuffer;
	LPDIRECT3DTEXTURE9			_texBackGround;
	vector<LPD3DXFONT*>			_pFonts;//字体储存对象
	vector<GUICONTROL*>			_pControls;//控件组
	vector<GUICONTROLGROUP*>	_pConGroups;
	vector<CSound *>			_wavs;		 //音频文件组
	GUITag	_guiTag;
	bool	_ISUseBackGround;			//标记是否使用背景
	int		_nTotalFontNum;			 //字体数目计数器  
	int		_nTotalControlNum;		//控件数目计数器  
	int		_nTotalBufferNum;		//缓冲区数目计数器  
	float	_kW, _kH;				//和初始化窗口相比的宽高比
	unsigned int  _originScrW, _originScrH;	//初始最佳宽高
	unsigned int  _nWindowWidth;	//窗口宽度  
	unsigned int  _nWindowHeight;	//窗口高度 
	void(*_guiProcessFun)(GUIControlTag conTag, int state);
	
public:
	GUIBasicClass();
	virtual void Clear() {}
	virtual ~GUIBasicClass() { Clear(); }
	virtual HRESULT Init(LPDIRECT3DDEVICE9	device,  CSoundManager *csound, GUITag tag, UINT w,UINT h, 
										void(*funcPtr)(GUIControlTag conTag, int state));
	virtual HRESULT AddBackGround(wchar_t* backtex);
	virtual HRESULT CreateFont(wchar_t * fontname, int size, int *fontID);

	virtual HRESULT AddControlGroup(GUIControlGroupTag tag, wchar_t* texName, D3DXVECTOR3 v, RECT area, RECT limitArea);
	virtual HRESULT AddControlToGroup(GUIControlGroupTag gtag, GUIControlTag tag, wchar_t *filename, RECT rec);

	virtual HRESULT AddButton(GUIControlTag tag,wchar_t *filename, RECT rec);

	virtual void AddText(wchar_t *text, RECT rec, D3DCOLOR color, int fontID);
	virtual void AddTextWithTag(wchar_t *text, RECT rec, D3DCOLOR color, int fontID, int * textTag);

	virtual HRESULT AddHUDPIC(wchar_t *filename, RECT rec);
	virtual void AddPICWithTag(wchar_t *filename, RECT rec, int* picTag);

	virtual void SetWindow(unsigned int w, unsigned int h) { _nWindowWidth = h, _nWindowHeight = h; }
	virtual void LoadWav() {}
	virtual void Render(bool mousedown, int mouseX, int mouseY);

	LPD3DXFONT		  GetFont(int i) { return *_pFonts[i]; }
	LPDIRECT3DDEVICE9 GetDevice() { return _device; }
	LPDIRECT3DTEXTURE9		 GetBackgroundTex() { return _texBackGround; }
	
	virtual void LoadSourceMenu();
};


void GUISetTexU(LPDIRECT3DVERTEXBUFFER9* buffer,UINT texset);

struct HUD_INFOCONTROL
{
	LPDIRECT3DVERTEXBUFFER9 _buffer;
	LPDIRECT3DTEXTURE9 _tex;   // 分别存放控件背景的填充图像,按钮弹起，按下和鼠标经过时的3张纹理图
	HUDControlType _type;  //HUD类型  ,文字还是其他
	SkillTag      _skill;
	RECT		_area;//控件覆盖区域
	
	bool		  _hasSkill;
	
	HUD_INFOCONTROL() { _buffer = NULL; _tex = NULL; _hasSkill = false; }
	~HUD_INFOCONTROL() { SAFE_RELEASE(_buffer); SAFE_RELEASE(_tex); }
};

struct HUD_TEXT
{
	D3DCOLOR _color; //控件颜色  
	RECT		 _area;//控件覆盖区域
	wchar_t		 _text[256];// 文字内容
	int _fontID;   //这个变量表示使用的字体
	int _textTag;
};

class HUDBasicClass
{
	LPDIRECT3DDEVICE9			_device;
	vector<HUD_INFOCONTROL*>	_controls;
	vector<HUD_TEXT*>			_texts;
	vector<LPD3DXFONT*>			_fonts;//字体储存对象
	int							_Font_1;
	int							_lifeValue;
	long						_runDistance;
	long						_scode;

	UINT _runDistanceTag;


public:
	HUDBasicClass() { _device = NULL; }
	virtual void Clear() {}
	virtual ~HUDBasicClass() { Clear(); }

	virtual void LoadSource();
	virtual void Init(LPDIRECT3DDEVICE9	device) { _device = device; LoadSource(); }
	virtual HRESULT AddSkill(SkillTag skilltag,wchar_t* filename,RECT rec);
	virtual HRESULT AddPic(wchar_t* filename,RECT rec);
	
	virtual HRESULT CreateFont(wchar_t * fontname, int size, int *fontID);
	virtual void AddText(wchar_t *text, RECT rec, D3DCOLOR color, int fontID);
	
	virtual void UpdateLifeValue(UINT value) { _lifeValue = value; }
	virtual void UpdateDistance(long distance);
	virtual void ChangeSkillState(SkillTag tag, bool hasIt);

	virtual HRESULT AddLifeValuePic(wchar_t* texName, RECT rec);
	virtual void InitDistance(int fontID,RECT rect,D3DCOLOR color);	
	
	virtual void Render();
};


class CalGUI :public GUIBasicClass
{
	int c_defeatBossTag;
	int c_goldNumTag;
	int c_silverNumTag;
	int c_distanceTag;

	LONG _distance;
	int _goldNum;//数量，用来做动画效果
	int _silverNum;

	LONG _distanceT;
	int _goldNumT;//数量，用来做动画效果
	int _silverNumT;

	int Font_1;
	int Font_2;
	
	
public:
	CalGUI();
	void InitDigital();
	void LoadSource(LPDIRECT3DDEVICE9	device, CSoundManager *csound,UINT w, UINT h,
					void(*funcPtr)(GUIControlTag conTag, int state));
	
// 	void AddGoldCoin();
// 	void AddSilverCoin();
// 	void AddDistance();
// 	void AddBossText();

	//下列函数用作动态处理
	void SetDigital(LONG dis,UINT goldnum,UINT silvernum) 
	{
		_distance = dis, _goldNum = goldnum, _silverNum = silvernum;
	}

	void SetDistanceText(LONG dis)
	{
		swprintf_s(_pControls[c_distanceTag]->_text, L"%ld", dis);
	}

	void SetGoldCoinNumText(UINT goldnum)
	{
		swprintf_s(_pControls[c_goldNumTag]->_text, L"%u", goldnum);
	}

	void SetSilverNumText(UINT silvernum)
	{
		swprintf_s(_pControls[c_silverNumTag]->_text, L"%u", silvernum);
	}

	void SetDefeatBoss(bool b);
	void Update(float timeDelta);
};


#endif