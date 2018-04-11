#pragma once
#ifndef Monsters_H_
#define Monsters_H_
#include"Game_BasicClass.h"
#include"AI.h"
#include"Myd3d.h"

bool IsOutEdge(D3DXVECTOR3* pos, float w, float h);
//怪物集合的基类，主要是处理渲染工作
class MonsterClassBasic
{
protected:
	LPDIRECT3DVERTEXBUFFER9		_buffer;//一个缓存区
	LPDIRECT3DTEXTURE9			_texture;//纹理
	LPDIRECT3DDEVICE9			_device;//要有设备
	D3DXMATRIX					world;//世界变换矩阵
	float						_uset;//纹理U轴偏移量
	float						_vset;//纹理V轴偏移量
	float						_sizew;//图像的宽，即为要画出来的大小
	float						_sizeh;//图像的高
	float						_z;//深度
	int							_ncow;//纹理中的列数量，一个动作有多少个图像
	int							_nrow;//纹理中的行，表明动作的数量
	vector<MonsterBasic*>		 monster;//怪物集合
	void(*regist)(MonsterBasic* role);//注册函数，在世界记事本上登记户口
	
public:
	virtual HRESULT Init(LPDIRECT3DDEVICE9	device, wchar_t *filename, int nrow, int ncow,
		float w, float h, float z, void(*regis)(MonsterBasic* role));

	virtual HRESULT  LoadWav() { return S_OK; }
	virtual void	 InitDigital() { monster.erase(monster.begin(),monster.end()); };
	virtual void	 Clear();
	virtual void	 Check() {}
	virtual void	 Create() {}
	virtual void	 Render();
	virtual void	 Update(float timeDelta) {}
	virtual			 ~MonsterClassBasic() { Clear(); }
	MonsterClassBasic() { _buffer = NULL; _texture = NULL; }
	
};
//蝙蝠的基类
class Monster_Bat :public MonsterBasic
{
public:
	
	AI_AttackLine bat_ai;
	Monster_Bat() { state = LIVE; score = 500; }
	virtual void ChangeAction(ActionState act);
	virtual void Move(float x, float y) {  _position.x += x, _position.y += y; box->Move(x, y); }
};

class MonsterClass_Bat :public MonsterClassBasic
{
	CSound * wav;
public:
	void AddBat(D3DXVECTOR3 pos);
	virtual HRESULT LoadWav() { return S_OK; }
	virtual void	Check();
	virtual void	Create();
	virtual void	Update(float timeDelta, D3DXVECTOR3 tag);
};
#endif