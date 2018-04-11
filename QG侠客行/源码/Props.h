#pragma once
#ifndef Props_H_
#define Props_H_
#include"Game_BasicClass.h"
#include"Myd3d.h"

class CoinsClass
{
protected:
	LPDIRECT3DDEVICE9			_device;//Ҫ���豸
	LPDIRECT3DVERTEXBUFFER9		_buffer;//һ��������
	LPDIRECT3DTEXTURE9			_texture;//����
	int							_ncow;//�����е���������һ�������ж��ٸ�ͼ��
	int							_nrow;
	float						_uset;//����U��ƫ����
	float						_vset;//����V��ƫ����
	float						_frame;//ÿ��֡���������˲����ٶ�
	int							frameNum;
	float						_sizew, _sizeh;//ͼ��Ĵ�С
	float						velocity;
	float _z;
	float _r;
	float _time;//��ʱ��ͽ��������ء�
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