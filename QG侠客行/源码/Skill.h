#pragma once
#ifndef Skill_H_
#define Skill_H_
#include"Game_BasicClass.h"
#include"Myd3d.h"
class Skill_SwordKeeClass:public SkillBasicClass//����
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
	int							_frameNum;
	float						_sizew, _sizeh;//ͼ��Ĵ�С
	float						_velocity;
	float						 _angle;//��ת�Ƕ�
	UINT						_atk;
	D3DXMATRIX					_world;
	vector<SkillBasicClass*>	_skills;
	void(*regist)(SkillBasicClass* skill);
public:
	Skill_SwordKeeClass();
	virtual void InitDigital() {}
	virtual void Clear();
	virtual ~Skill_SwordKeeClass() { Clear(); }
	virtual HRESULT Init(LPDIRECT3DDEVICE9	device, wchar_t *filename, int nrow, int ncow, float frame,
		float w, float h,  float z,float velocity, UINT atk, 
		void(*regis)(SkillBasicClass* skill));

	virtual void AddSkill(D3DXVECTOR3 pos,D3DXVECTOR3*taget = NULL);
	virtual void Update(float timeDelta);
	virtual void Render();
};
//����

class Skill_DartClass:public Skill_SwordKeeClass
{
	public:
		void Update(float timeDelta);
		void Render();
};
class SkillManager
{
	LPDIRECT3DDEVICE9 _device;
	CSoundManager* _soundManager;
	Skill_SwordKeeClass* _swordKee;
	void(*_regist)(SkillBasicClass* skill);
public:
	void Clear() { SAFE_DELETE(_swordKee); }
	~SkillManager() { Clear(); }
	void Init(LPDIRECT3DDEVICE9 device, CSoundManager* sound, void(*regist)(SkillBasicClass* skill)) 
									{	_device = device; _soundManager = sound; _regist = regist;}
	void LoadSource();
	void UseSkill(RoleBasic* role,SkillTag tag, D3DXVECTOR3 pos, D3DXVECTOR3* taget = NULL);
	void Update(float timeDelta);
	void Render();
};
#endif