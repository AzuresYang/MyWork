#pragma once
#ifndef Game_BasicClass_H_
#define Game_BasicClass_H_
#include<d3dx9.h>
#include"DXUT.h"
#include<vector>
#include<algorithm>

#define LEFT_EDGE  -20
#define RIGHT_EDGE  240
#define HEIGHT_EDGE	120
#define LOW_EDGE	-20

#define Scr_LEFT_EDGE 0
#define Scr_Right_EDGE 120
#define Scr_LOW_EDGE 0
#define Scr_HIGHT_EDGE 80
using std::vector;
struct BASICVERTEX
{
	float _x, _y, _z;
	D3DCOLOR _color;
	float _u, _v;
	BASICVERTEX(float x, float y, float z, float u, float v)
	{
		_x = x, _y = y, _z = z, _u = u, _v = v;
		_color = D3DCOLOR_ARGB(255,255,255,255);
	}
};
#define D3DFVF_BASIC (D3DFVF_XYZ |D3DFVF_DIFFUSE |D3DFVF_TEX1)
enum Tag
{
	PLAYER, ENEMY, PLAYERPROP, ENEMYPROP,PLAYER_SKILL,ENEMY_SKILL
};
//����͵��ߵļ���״̬�����������棬���磨�������⣬��Ļ���� ���ݻ٣�����Ҳ����˵��������
 enum State
{
	DEATH, LIVE, OUTRANGE, DESTROYED,GOINGDYING
};
 enum ActionState
 {
	 NONE, READY, ATTACK, ATTCAKING, WONDER, RUN, JUMP, FALL, RELEASESIKILL_1, DYING,//������
	 ATT_CatchAttack,Wander//����
};
enum BoxTag
{
	SQUAREBOX, ROUNDBOX
};

enum HitState
{
	NoHit, Hit,HitByEnemy
};
enum CoinType
{
	GOLDCOIN,SILVERCOIN
};
//С�����ǵĹ���
enum MonsterTag
{
	BAT
};
enum AITag
{
	AI_STAND, AI_ATTLINE
};
enum SkillTag
{
	SKILL_Shield, SKILL_SwordKee, SKILL_Dart, SKILL_Bamboo_1, SKILL_Bamboo_3,Skill_Dash
};

//��ײ���ӵĻ���,��Ϊ����ָ��ֻ�ܵ��û���ķ���������Init()����Ҫ���ؾ��κ��Ӻ�ԭ�ͺ��ӵķ���
class BasicBox
{
public:
	D3DXVECTOR3 _position;
	BoxTag tag;
	BasicBox(){}
	virtual void Init(D3DXVECTOR3, float r){}
	virtual void Init(D3DXVECTOR3 pos, float w, float h) {}
	virtual void Set(D3DXVECTOR3 pos, float r) { }
	virtual void Set(D3DXVECTOR3 pos, float w, float h) {}
	virtual void Set(D3DXVECTOR3 pos){}
	virtual void Move(float x, float y) {}
};

//���κ���
class SquareBox:public BasicBox
{
public:
	float lx, hy, rx, ly;
	SquareBox() { tag = SQUAREBOX; }	
	SquareBox(D3DXVECTOR3 pos, float w, float h) {	_position = pos; lx = _position.x - w / 2; rx = lx + w;
													ly = _position.y - h / 2; hy = ly + h; tag = SQUAREBOX;}
	virtual void Init(D3DXVECTOR3 pos,float w, float h ) { _position = pos; lx = _position.x - w/2; rx = lx + w;
														ly = _position.y - h / 2; hy = ly + h;}
	virtual void Set(D3DXVECTOR3 pos) { Move(pos.x - _position.x, pos.y - _position.y); _position.z = pos.z; }
	virtual void Move(float x, float y) { lx += x, rx += x, ly += y, hy += y;
										_position += D3DXVECTOR3(x,y,0);}
};

class RoundBox: public BasicBox
{
public:
	float radius;
	RoundBox() { tag = ROUNDBOX; }
	virtual void Init(D3DXVECTOR3 pos, float r) { _position = pos; radius = r; }
	virtual void Set(D3DXVECTOR3 pos) { _position = pos; }
	void Move(float x,float y) { _position += D3DXVECTOR3(x, y, 0); }
};



//���ܻ���
class SkillBasicClass
{
public:
	D3DXVECTOR3	_velocity;
	D3DXVECTOR3 _pos;
	BasicBox	*_box;
	UINT		_atk;//������

	SkillTag    _skillTag;
	State		_liveState;
	Tag			_campTag;
	float		_angle;//������ת�Ƕ�
	float       _angleSpin;//��������
	float _time;
public:
	SkillBasicClass() { _angle = _angleSpin = 0; _liveState = LIVE; }
	//virtual void Init(D3DXVECTOR3 pos, UINT atk, BasicBox* box, D3DXVECTOR3 v) {};
	virtual void Update(float timeDelta) {};
	virtual void Move(float x, float y) { _pos.x += x; _pos.y += y; _box->Move(x, y); }
};

//������࣬��������Һ�BOSS����
class RoleBasic
{
protected:
	LPDIRECT3DVERTEXBUFFER9		_buffer;//һ��������
	LPDIRECT3DDEVICE9			_device;//Ҫ���豸
	LPDIRECT3DTEXTURE9			_texture;//����
	float						_uset;//����U��ƫ����
	float						_vset;//����V��ƫ����
	float						_frame;//ÿ��֡���������˲����ٶ�
	float						_sizew, _sizeh;//ͼ��Ĵ�С
	float						time;//��ʱ��	
	float						mass;
	int							_ncow;//�����е���������һ�������ж��ٸ�ͼ��
	int							_nrow;//�����е��У���������������
public:
	int						_lifeValue;
	Tag						_tag;//��Ӫ
	State					_state;//����ļ���״̬
	BasicBox*				box;//��ײ����
	ActionState				_action;//���ﶯ��״̬
	D3DXVECTOR3				_position;//λ��
	RoleBasic() { _buffer = NULL, _texture = NULL; box = NULL; }
	virtual void Clear() {  SAFE_RELEASE(_buffer); SAFE_RELEASE(_texture); }
	virtual ~RoleBasic() { Clear(); }
	virtual	HRESULT	Init() { return S_OK; }
	virtual void Move(float x, float y) {	if(box)box->Move(x, y);_position -= D3DXVECTOR3(x, y, 0);}
	virtual UINT GetSkillNum(SkillTag tag) { return 0; };
	virtual void  ChangerSkillNum(SkillTag tag, int num) {};
};



//---------------------��С�ֻ��ࡿ---------------------------//

class MonsterBasic
{
	Tag						tag;//��Ӫ
protected:
	
	int						score;//����
	MonsterTag				tag_monster;
public:
	unsigned int			_framenum;//�Դ��ڵڼ�֡
	unsigned int			_frame;//�Դ�ÿ��֡��
	unsigned int			_rownum;//�����ڵڼ���
	float                   _time;//�Դ���ʱ��
	float                   ai_time;
	float					_hitTime;//��ײ��ʱ���������õ�������;����������ײ
	float					mass;
	int						_lifeValue;//����ֵ
	int						atk;//������
	ActionState				action;//����״̬
	D3DXVECTOR3				_velocity;//�ٶ�
	D3DXVECTOR3				_position;//λ��
	D3DCOLOR				 _color;
	BasicBox*				box;//��ײ����
	State					state;//����ļ���״̬

	virtual int		 GetATK(){ return atk; }
	virtual int		 GetScore(){ return score; }
	virtual void	 Move(float x, float y){}
	virtual void	 ChangeAction(ActionState act) {}
	MonsterBasic() { tag = ENEMY, box = NULL; action = NONE; state = LIVE; 
					_time = 0, ai_time = 0; _hitTime = 0;
					_rownum = _framenum = 0; _frame = 5; _lifeValue = 1; atk = 1;
					_color = D3DCOLOR_ARGB(255, 255, 255, 255);
					}
};
	
//***************�����߻��ࡿ******************//
class PropBasic
{
public:
	D3DXVECTOR3				_position;//λ��
	ActionState				action;//����״̬
	BasicBox*				box;//��ײ����
	State					state;//����״̬
	Tag						tag;//��Ӫ
	float					time;
	int						lastFrame;
	int						power;
};


class AIBasic
{
protected:
	AITag  AItag;
	virtual void Update(MonsterBasic* user, RoleBasic* obj) {}
	virtual void Avoid(MonsterBasic* user, BasicBox* box) {}
};
//Ӳ��
struct coin
{
	D3DXVECTOR3				_position;//λ��
	CoinType				type;
	RoundBox				box;//��ײ����
	State					state;//����״̬
	float					time;
	int						frame;
	int						value;
	coin() { time = 0; frame = 0; state = LIVE; }
};
typedef vector<SkillBasicClass*>::iterator   PSkillIter;
typedef vector<PropBasic*>::iterator         PPropIter;
typedef vector<RoleBasic*>::iterator	     PRoleIter;
typedef vector<MonsterBasic*>::iterator	     PMonsIter;
typedef vector<coin*>::iterator				 PCoinIter;

typedef vector<SkillBasicClass*>		     PSkill;
typedef vector<MonsterBasic*>				 PMonster;
typedef vector<PropBasic*>					 PProp;
typedef vector<RoleBasic*>					 PRole;
typedef vector<coin*>						 PCoin;


// typedef vector<SkillBasicClass>::iterator   SkillIter;
// typedef vector<PropBasic>::iterator         PropIter;
// typedef vector<RoleBasic>::iterator	       RoleIter;
   typedef vector<MonsterBasic>::iterator	   MonsIter;
// typedef vector<coin>::iterator			   CoinIter;
// typedef vector<SkillBasicClass>		       Skill;
// typedef vector<MonsterBasic>				   Monster;
// typedef vector<PropBasic>				   Prop;
// typedef vector<RoleBasic>				   Role;
// typedef vector<coin>						   v_Coin;
//typedef vector<MonsterBasic>::iterator       
#endif // Game_Charactor_H_
