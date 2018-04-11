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
//人物和道具的几种状态，死亡，生存，出界（超出界外，屏幕）， 被摧毁（道具也可以说是死亡）
 enum State
{
	DEATH, LIVE, OUTRANGE, DESTROYED,GOINGDYING
};
 enum ActionState
 {
	 NONE, READY, ATTACK, ATTCAKING, WONDER, RUN, JUMP, FALL, RELEASESIKILL_1, DYING,//死亡中
	 ATT_CatchAttack,Wander//漫游
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
//小怪物们的狗牌
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

//碰撞盒子的基类,以为基类指针只能调用基类的方法，所以Init()函数要重载矩形盒子和原型盒子的方法
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

//矩形盒子
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



//技能基类
class SkillBasicClass
{
public:
	D3DXVECTOR3	_velocity;
	D3DXVECTOR3 _pos;
	BasicBox	*_box;
	UINT		_atk;//攻击力

	SkillTag    _skillTag;
	State		_liveState;
	Tag			_campTag;
	float		_angle;//技能旋转角度
	float       _angleSpin;//技能自旋
	float _time;
public:
	SkillBasicClass() { _angle = _angleSpin = 0; _liveState = LIVE; }
	//virtual void Init(D3DXVECTOR3 pos, UINT atk, BasicBox* box, D3DXVECTOR3 v) {};
	virtual void Update(float timeDelta) {};
	virtual void Move(float x, float y) { _pos.x += x; _pos.y += y; _box->Move(x, y); }
};

//人物基类，可用做玩家和BOSS开发
class RoleBasic
{
protected:
	LPDIRECT3DVERTEXBUFFER9		_buffer;//一个缓存区
	LPDIRECT3DDEVICE9			_device;//要有设备
	LPDIRECT3DTEXTURE9			_texture;//纹理
	float						_uset;//纹理U轴偏移量
	float						_vset;//纹理V轴偏移量
	float						_frame;//每秒帧数，决定了播放速度
	float						_sizew, _sizeh;//图像的大小
	float						time;//计时器	
	float						mass;
	int							_ncow;//纹理中的列数量，一个动作有多少个图像
	int							_nrow;//纹理中的行，表明动作的数量
public:
	int						_lifeValue;
	Tag						_tag;//阵营
	State					_state;//人物的几种状态
	BasicBox*				box;//碰撞盒子
	ActionState				_action;//人物动作状态
	D3DXVECTOR3				_position;//位置
	RoleBasic() { _buffer = NULL, _texture = NULL; box = NULL; }
	virtual void Clear() {  SAFE_RELEASE(_buffer); SAFE_RELEASE(_texture); }
	virtual ~RoleBasic() { Clear(); }
	virtual	HRESULT	Init() { return S_OK; }
	virtual void Move(float x, float y) {	if(box)box->Move(x, y);_position -= D3DXVECTOR3(x, y, 0);}
	virtual UINT GetSkillNum(SkillTag tag) { return 0; };
	virtual void  ChangerSkillNum(SkillTag tag, int num) {};
};



//---------------------【小怪基类】---------------------------//

class MonsterBasic
{
	Tag						tag;//阵营
protected:
	
	int						score;//分数
	MonsterTag				tag_monster;
public:
	unsigned int			_framenum;//自带在第几帧
	unsigned int			_frame;//自带每秒帧数
	unsigned int			_rownum;//动作在第几行
	float                   _time;//自带计时器
	float                   ai_time;
	float					_hitTime;//碰撞计时器，无奈用的他，用途：避免多次碰撞
	float					mass;
	int						_lifeValue;//生命值
	int						atk;//攻击力
	ActionState				action;//动作状态
	D3DXVECTOR3				_velocity;//速度
	D3DXVECTOR3				_position;//位置
	D3DCOLOR				 _color;
	BasicBox*				box;//碰撞盒子
	State					state;//人物的几种状态

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
	
//***************【道具基类】******************//
class PropBasic
{
public:
	D3DXVECTOR3				_position;//位置
	ActionState				action;//动作状态
	BasicBox*				box;//碰撞盒子
	State					state;//几种状态
	Tag						tag;//阵营
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
//硬币
struct coin
{
	D3DXVECTOR3				_position;//位置
	CoinType				type;
	RoundBox				box;//碰撞盒子
	State					state;//几种状态
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
