#pragma once
#ifndef Props_H_
#define Props_H_
#include"Game_BasicClass.h"
#include"Myd3d.h"

class CoinsClass
{
protected:
	LPDIRECT3DDEVICE9			_device;//要有设备
	LPDIRECT3DVERTEXBUFFER9		_buffer;//一个缓存区
	LPDIRECT3DTEXTURE9			_texture;//纹理
	int							_ncow;//纹理中的列数量，一个动作有多少个图像
	int							_nrow;
	float						_uset;//纹理U轴偏移量
	float						_vset;//纹理V轴偏移量
	float						_frame;//每秒帧数，决定了播放速度
	int							frameNum;
	float						_sizew, _sizeh;//图像的大小
	float						velocity;
	float _z;
	float _r;
	float _time;//该时间和金币生成相关。
	D3DXMATRIX					world;
	void(*regist)(coin* role);
	void AddCoin(D3DXVECTOR3 pos, CoinType type);
	void SortCoin(UINT star, UINT it, coin* acoin);
public:
	
vector<coin*>				coins;
	HRESULT Init(LPDIRECT3DDEVICE9	device, wchar_t *filename, int nrow, int ncow, float frame,
		float w, float h, float ve, void(*regis)(coin* acoin));
	void InitDigital();
 	void CreateCoins(int type);
 	void Coin_QG();
	void Coin_Stdio() {}
	void Coin_Guide();
 	void Coin_Random();
	bool Update(float timeDelta);
	void Render(float timeDelta);
	void Clear() { SAFE_RELEASE(_buffer); SAFE_RELEASE(_texture); }
	void Move(coin * acoin,float x, float y);
	float GetState() { return (float)(*coins.begin())->state;}
	CoinsClass() { _device = NULL; _buffer = NULL; _texture = NULL; velocity = 0;}
	virtual ~CoinsClass() { Clear(); }
};


#endif