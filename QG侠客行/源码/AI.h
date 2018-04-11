#pragma once
#ifndef _AI_H_
#define _AI_H_
#include"Game_BasicClass.h"
#include"Myd3d.h"
class AI_AttackLine:public AIBasic
{
	D3DXVECTOR3 dir;
public:
	AI_AttackLine() { AItag = AI_ATTLINE; }
	virtual void TakeAction(MonsterBasic* user, D3DXVECTOR3* tag, float timeDelta);
	virtual void AttackingReady(){}
	virtual void Attacking() {}
};
#endif