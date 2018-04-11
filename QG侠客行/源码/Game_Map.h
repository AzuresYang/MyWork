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
	LPDIRECT3DTEXTURE9			_pTexture;//地图纹理
	int							_numOfBuffer;//地图缓存数量
	int							_level;//目前难度等级
	float						_time;//目前时间累计
	float						_fVelocity;//前进速度
	float						_height;//地图在场景中的高度
	float						_z;//深度
	int						    _screenWidth;//屏幕宽度，在生成坐标时有用
	int							_elementWidth;//单位长度,此处可以代指人物角色的贴图长度
	float						_lastMapPosition;//最后一块地图的位置
	float						_leftEdge[5];//每块拼接地图的开始位置
	float						_rightEdge[5];//每块拼接的图的接受位置，在x轴上
	int							_firstMap;//第一块地图标记，注意是从0到4
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
	LPDIRECT3DTEXTURE9			_pTexture;//地图纹理
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