#pragma once
#ifndef Monsters_H_
#define Monsters_H_
#include"Game_BasicClass.h"
#include"AI.h"
#include"Myd3d.h"

bool IsOutEdge(D3DXVECTOR3* pos, float w, float h);
//���Ｏ�ϵĻ��࣬��Ҫ�Ǵ�����Ⱦ����
class MonsterClassBasic
{
protected:
	LPDIRECT3DVERTEXBUFFER9		_buffer;//һ��������
	LPDIRECT3DTEXTURE9			_texture;//����
	LPDIRECT3DDEVICE9			_device;//Ҫ���豸
	D3DXMATRIX					world;//����任����
	float						_uset;//����U��ƫ����
	float						_vset;//����V��ƫ����
	float						_sizew;//ͼ��Ŀ���ΪҪ�������Ĵ�С
	float						_sizeh;//ͼ��ĸ�
	float						_z;//���
	int							_ncow;//�����е���������һ�������ж��ٸ�ͼ��
	int							_nrow;//�����е��У���������������
	vector<MonsterBasic*>		 monster;//���Ｏ��
	void(*regist)(MonsterBasic* role);//ע�ắ������������±��ϵǼǻ���
	
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
//����Ļ���
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