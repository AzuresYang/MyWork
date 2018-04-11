#pragma once
#ifndef _GAME_MAP_H_
#define _GAME_MAP_H_
#include<d3dx9.h>
#include"Game_BasicClass.h"
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif

#define LEVELMAX  3
typedef struct MAPVERTEX
{
	float _x, _y, _z;
	float _u, _v;
	MAPVERTEX(float x, float y, float z, float u, float v)
	{
		_x = x, _y = y, _z = z, _u = u, _v = v;
	}
}MAPVERERX;
#define D3DFVF_GAMEMAP (D3DFVF_XYZ | D3DFVF_TEX1)

typedef class MapClass
{
	LPDIRECT3DDEVICE9			_device;
	LPDIRECT3DVERTEXBUFFER9		_pBuffer[5];
	LPDIRECT3DTEXTURE9			_pTexture;//��ͼ����
	int							_numOfBuffer;//��ͼ��������
	int							_level;//Ŀǰ�Ѷȵȼ�
	float						_time;//Ŀǰʱ���ۼ�
	float						_fVelocity;//ǰ���ٶ�
	float						_height;//��ͼ�ڳ����еĸ߶�
	float						_z;//���
	int						    _screenWidth;//��Ļ��ȣ�����������ʱ����
	int							_elementWidth;//��λ����,�˴����Դ�ָ�����ɫ����ͼ����
	float						_lastMapPosition;//���һ���ͼ��λ��
	float						_leftEdge[5];//ÿ��ƴ�ӵ�ͼ�Ŀ�ʼλ��
	float						_rightEdge[5];//ÿ��ƴ�ӵ�ͼ�Ľ���λ�ã���x����
	int							_firstMap;//��һ���ͼ��ǣ�ע���Ǵ�0��4
public:
	SquareBox					box[5];
	MapClass();
	bool Init(LPDIRECT3DDEVICE9 * device, wchar_t* filename, int character_length, float h,float z,int screenWidth, float v);
	void InitDigital();
	void SetVelocity(float v) { _fVelocity = v; }
	bool CreateMap();
	bool UpdateOneMap(int i);
	void UpdateMap(float timeDelta);
	void CreateWidth(float &width, float &blank_width);
	bool IsOnMap(float lx,float rx);
	bool IsOnMap(int i, float lx, float rx) { return (lx > _leftEdge[i] && rx < _rightEdge[i]); }
	float GetPosition() { return _lastMapPosition; }
	int  GetFirstMap() { return _firstMap; }
	int GetNum() { return _numOfBuffer; }
	float GetVelocity() { return _fVelocity; }
	void Render();
	void SetLevel(int level) { _level = level; }
	void Clear();
	float GetBoxLx() { return box[_firstMap].lx; }
	float GetBoxRx() { return box[_firstMap].rx; }
	float GetBoxHy() { return box[_firstMap].hy; }
	float GetMapLeftEdge() { return _leftEdge[_firstMap ]; }
	float GetMapRightEdge() { return _rightEdge[_firstMap]; }
	~MapClass() { Clear(); }
}MapClass, *PMapClass;

typedef class BackGroudClass
{
	LPDIRECT3DDEVICE9			_device;
	LPDIRECT3DVERTEXBUFFER9		_pBuffer[7];
	LPDIRECT3DTEXTURE9			_pTexture;//��ͼ����
	float						_z[7];
	int							_totalNum;
	float						_fVelocity[7];
	float						_screenWidth;
	float						_screenHeight;
	bool	CreateBuffer();
public:
	BackGroudClass() { for (int i = 0; i < 7; i++)_pBuffer[i] = NULL, _z[i] = _fVelocity[i] = 0; }
	HRESULT	Init(LPDIRECT3DDEVICE9* device,wchar_t * filename,float w, float h, unsigned int num,float v );
	void	Update(float timeDelta);
	void	Render();
	void	Clear();
	~BackGroudClass() { Clear(); }
}BackGroudClass, *PBackGroudClass;
#endif