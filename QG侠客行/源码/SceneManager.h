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
	PRole			    Player;//���
	PMonster			Enemy;//С����
	PProp				PlayerProp;//��ҵ���
	PProp				EnemyProp;//������ߣ�Ҳ�������壬����
	PSkill				Skill;
	PCoin				Coins;

};

bool IsOutRanger(D3DXVECTOR3 pos);
//Ŀǰ4��״������ײ�������ײ����ң��˵��߻��й���˵��߻��ез�����
//����״���ٿ�
enum EVEN
{
	HIT_NONE,
	HIT_ENHPL,//����������
	HIT_PLHCO,//����������
	HIT_PLPRHENPR,//��������з�����
	HIT_ENPRHPL,
	HIT_ONMAP,
	HIT_MAPHPL,//������ײ�¼�
	HIT_PLSKHEN,
	FALLDOWN,
	EVEN_GAMEOVER
};
class CollisionSystem
{

public:
	bool IsHit(BasicBox * box1, BasicBox * box2);//������ײ���Ӽ����ײ��⣬������Դ��ݸ�����ӵ���Ϣ
	bool D2DHit(SquareBox *box1, SquareBox* box2);//��ͬ���Ӽ����ײ����㷨�ǲ�ͬ��
	bool D2DHit(RoundBox * box1, RoundBox * box2);
	bool D2DHit(SquareBox *sbox, RoundBox *rbox);
	void CollisionTest();
};


//------------------------------------------------
//��������
//------------------------------------------------
class SceneManager
{
	LPDIRECT3DDEVICE9			device;//Ҫ���豸
	CSoundManager*				soundManager;
	SkillManager*				skillManager;
	World*				wo;
	MapClass *			map;
	Camera	*			camera;
	HUDBasicClass*      hud;
	CalGUI*				calGUI;
	CoinsClass*         coins;
	LONG				distance;//����
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