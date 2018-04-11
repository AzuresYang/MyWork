#pragma once
#ifndef SceneManager_H_
#define SceneManager_H_
#include"DXUT.h"
#include<vector>
#include<algorithm>
#include"Game_BasicClass.h"
#include"Game_Map.h"
#include"Players.h"
#include"Myd3d.h"
#include"Skill.h"
#include"Game_GUI.h"
#include"Props.h"
struct World 
{
	PRole			    Player;//玩家
	PMonster			Enemy;//小怪物
	PProp				PlayerProp;//玩家道具
	PProp				EnemyProp;//怪物道具，也包括陷阱，其他
	PSkill				Skill;
	PCoin				Coins;

};

bool IsOutRanger(D3DXVECTOR3 pos);
//目前4种状况，人撞到怪物，人撞到金币，人道具击中怪物，人道具击中敌方道具
//其他状况再看
enum EVEN
{
	HIT_NONE,
	HIT_ENHPL,//怪物碰到人
	HIT_PLHCO,//玩家碰到金币
	HIT_PLPRHENPR,//玩家碰到敌方道具
	HIT_ENPRHPL,
	HIT_ONMAP,
	HIT_MAPHPL,//四种碰撞事件
	HIT_PLSKHEN,
	FALLDOWN,
	EVEN_GAMEOVER
};
class CollisionSystem
{

public:
	bool IsHit(BasicBox * box1, BasicBox * box2);//两种碰撞盒子间的碰撞检测，基类可以传递各类盒子的信息
	bool D2DHit(SquareBox *box1, SquareBox* box2);//不同盒子间的碰撞检测算法是不同的
	bool D2DHit(RoundBox * box1, RoundBox * box2);
	bool D2DHit(SquareBox *sbox, RoundBox *rbox);
	void CollisionTest();
};


//------------------------------------------------
//场景管理
//------------------------------------------------
class SceneManager
{
	LPDIRECT3DDEVICE9			device;//要有设备
	CSoundManager*				soundManager;
	SkillManager*				skillManager;
	World*				wo;
	MapClass *			map;
	Camera	*			camera;
	HUDBasicClass*      hud;
	CalGUI*				calGUI;
	CoinsClass*         coins;
	LONG				distance;//分数
	LONG				score;
	UINT				goldCoinNum;
	UINT				silverCoinNum;
	double				gameTime;
	bool				IsDefeatBoss;
	CollisionSystem		ColSys;
	float				_gameVelocity;
	GAMEPROCESS*		_gameprocess;
public:
	SceneManager() { coins = NULL; }
	void Init(LPDIRECT3DDEVICE9 _device, World *wor,CSoundManager* sound,SkillManager* skill) 
	{
		device = _device;  wo = wor; soundManager = sound; skillManager = skill; InitDigital(); 
	}
	void AddHUD(HUDBasicClass* _hud) { hud = _hud; }
	void AddCalGUI(CalGUI* cal) { calGUI = cal; }
	void AddCoin(CoinsClass* coin) { coins = coin; }
	void AddGameProcess(GAMEPROCESS* process) { _gameprocess = process; }
	void SetVelocity(float v) { _gameVelocity = v; }
	void InitDigital();
	void Register(MapClass * m) { map = m; }
	void Register(Camera* ca) { camera = ca; }
	void Check();
	void HitTest();
	long GetScore();
	LONG GetDistance() { return distance; }
	UINT GetGoldCoinNum() { return goldCoinNum; }
	UINT GetSilverCoinNum() { return silverCoinNum; }
	UINT DefeatBoss() { return IsDefeatBoss; }
	void SendMessge(EVEN even, void* user, void* obj);
	void DisposInput(float timeDelta,float mousex, float mousey, int mousestate);
	void Update(float timeDelta);
	void Render();
};


#endif