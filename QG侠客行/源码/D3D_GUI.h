#pragma once
#ifndef _D3D_GUI_
#define _D3D_GUI_
#include"d3dx9.h"
#if defined(DEBUG) || defined(_DEBUG)
#ifndef V
#define V(x)          x //{ hr = (x); if( FAILED(hr) ) { DXUTTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }
#endif
#ifndef V_RETURN
#define V_RETURN(x)   x //{ hr = (x); if( FAILED(hr) ) { return DXUTTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }
#endif
#endif


#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif   

#define ISBUTTON 1
#define ISTEXT   2

#define UPTEX	0
#define DOWNTEX 1
#define OVERTEX 2

//鼠标按键宏
#define MOUSE_UP 1
#define MOUSE_DOWN 2
#define MOUSE_OVER 3

#define GUI_STATE_MENU	1
#define GUI_GUISTATE_OPTION	2
#define GUI_STATE_ESC		3
#define GUI_STATE_PAUSE   4

#define BUTTON_STAEGAME 1
#define BUTTON_OPTION   2
#define BUTTON_QUITGAME 3
#define zLEVEL1 10
struct GUIVERTEX1
{
	float _x, _y, _z, rzm;
	unsigned long color;
	float _u, _v;
	GUIVERTEX1(float x, float y, float z, float r, unsigned long c, float u, float v)
	{
		_x = x, _y = y, _z = z, rzm = r, color = c, _u = u, _v = v;
	}
};
#define D3DFVF_GUI1 (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

struct GUIVERTEX
{
	float _x, _y, _z;
	float _u, _v;
	GUIVERTEX(float x, float y, float z, float u, float v)
	{
		_x = x, _y = y, _z = z,  _u = u,  _v= v;
	}
};
#define D3DFVF_GUI (D3DFVF_XYZ | D3DFVF_TEX1)

typedef struct GUICONTROL
{
	//操作类型，ID和颜色  
	int _type;  //控件类型  ,文字还是其他
	int _id;  //控件ID  
	unsigned long _color; //控件颜色  
	int _listID;   //如果是文字的话，这个变量就表示它使用的字体，否则就表示顶点缓存  
	float _xPos, _yPos;   //控件的起始位置  
	float _width, _height;   // 控件的宽度和高度  
	wchar_t *_wctext;		// 文字内容 
	GUICONTROL* next;
	IDirect3DTexture9* _tex[3];   // 分别存放控件背景的填充图像,按钮弹起，按下和鼠标经过时的4张纹理图
	GUICONTROL() { next = NULL; for (int i = 0; i < 3; i++) _tex[i] = NULL; }
	BOOL OverButton(unsigned int x,unsigned int y);
	
}GUICONTROL, *PGUICONTROL;


typedef class GUIClass
{
protected:
	LPDIRECT3DDEVICE9			_device;  //D3D设备对象      
	LPD3DXFONT					*_pFonts;  //D3D字体对象  
	GUICONTROL					*_pControls;  //控件对象数组  
	LPDIRECT3DVERTEXBUFFER9		*_pVertexBuffer;   //顶点缓存对象指针  
	LPDIRECT3DTEXTURE9			_Background;  //背景图对象  
	LPDIRECT3DVERTEXBUFFER9		_BackgroundBuffer;   //背景图缓冲区对象  

	int  _IdOfGUI;
	bool _bIsBackgroundUsed;				//一个标识，用于标识是否已经用了背景  
	int _nTotalFontNum;					 //字体数目计数器  
	int _nTotalControlNum;				//控件数目计数器  
	int _nTotalBufferNum;				//缓冲区数目计数器  
	int _nWindowWidth;					 //窗口宽度  
	int _nWindowHeight;					 //窗口高度  
public:
	GUIClass();
	void Clear();
	~GUIClass() { Clear(); }	
	void Init(LPDIRECT3DDEVICE9 *device, int w, int h)
	{
		_device = *device, _nWindowHeight = h, _nWindowWidth = h;
	}
	BOOL CreateTextFont(wchar_t *fontName, int size, int *fontID);  //字体创建函数
	BOOL AddBackGround(wchar_t * texname);
	BOOL AddButton(int id, float x, float y, wchar_t *up, wchar_t *over, wchar_t *down);
	BOOL AddText(int id, wchar_t *text, float x,float y, unsigned long color, int fontID);
	BOOL IsBackgroundUsed()		  { return _bIsBackgroundUsed; }
	void SetWindow(unsigned int w,unsigned int h)  { _nWindowHeight = h, _nWindowWidth = h; }
	int  GetWindowWidth()	   { return _nWindowWidth; }
	int  GetWindowHeight()     { return _nWindowHeight; }
	int  GetTotalControlNum()  { return _nTotalControlNum; }
	int  GetTotalBufferNum()   { return _nTotalBufferNum; }
	
	LPD3DXFONT		  GetFont(int i)     { return _pFonts[i]; }
	LPDIRECT3DDEVICE9 GetDevice() { return _device; }
	LPDIRECT3DTEXTURE9		 GetBackgroundTex() { return _Background; }
	LPDIRECT3DVERTEXBUFFER9  GetBackgroundBuffer() { return _BackgroundBuffer; }
	LPDIRECT3DVERTEXBUFFER9  GetVertexBuffer(unsigned int listid) { return _pVertexBuffer[listid]; }
	GUICONTROL* GetGUIControl()  { return _pControls;}

}GUIClass, *PGUIClass;

void ProcessGUI(GUIClass *gui, bool mousedown, int mouseX, int mouseY,
				void(*funcPtr)(int id, int state));  //回调函数
#endif