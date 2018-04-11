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

//��갴����
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
	//�������ͣ�ID����ɫ  
	int _type;  //�ؼ�����  ,���ֻ�������
	int _id;  //�ؼ�ID  
	unsigned long _color; //�ؼ���ɫ  
	int _listID;   //��������ֵĻ�����������ͱ�ʾ��ʹ�õ����壬����ͱ�ʾ���㻺��  
	float _xPos, _yPos;   //�ؼ�����ʼλ��  
	float _width, _height;   // �ؼ��Ŀ�Ⱥ͸߶�  
	wchar_t *_wctext;		// �������� 
	GUICONTROL* next;
	IDirect3DTexture9* _tex[3];   // �ֱ��ſؼ����������ͼ��,��ť���𣬰��º���꾭��ʱ��4������ͼ
	GUICONTROL() { next = NULL; for (int i = 0; i < 3; i++) _tex[i] = NULL; }
	BOOL OverButton(unsigned int x,unsigned int y);
	
}GUICONTROL, *PGUICONTROL;


typedef class GUIClass
{
protected:
	LPDIRECT3DDEVICE9			_device;  //D3D�豸����      
	LPD3DXFONT					*_pFonts;  //D3D�������  
	GUICONTROL					*_pControls;  //�ؼ���������  
	LPDIRECT3DVERTEXBUFFER9		*_pVertexBuffer;   //���㻺�����ָ��  
	LPDIRECT3DTEXTURE9			_Background;  //����ͼ����  
	LPDIRECT3DVERTEXBUFFER9		_BackgroundBuffer;   //����ͼ����������  

	int  _IdOfGUI;
	bool _bIsBackgroundUsed;				//һ����ʶ�����ڱ�ʶ�Ƿ��Ѿ����˱���  
	int _nTotalFontNum;					 //������Ŀ������  
	int _nTotalControlNum;				//�ؼ���Ŀ������  
	int _nTotalBufferNum;				//��������Ŀ������  
	int _nWindowWidth;					 //���ڿ��  
	int _nWindowHeight;					 //���ڸ߶�  
public:
	GUIClass();
	void Clear();
	~GUIClass() { Clear(); }	
	void Init(LPDIRECT3DDEVICE9 *device, int w, int h)
	{
		_device = *device, _nWindowHeight = h, _nWindowWidth = h;
	}
	BOOL CreateTextFont(wchar_t *fontName, int size, int *fontID);  //���崴������
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
				void(*funcPtr)(int id, int state));  //�ص�����
#endif