#include"DXUT.h"
#include"SceneManager.h"

//

bool IsOutRanger(D3DXVECTOR3 pos)
{
	float x = pos.x;
	float y = pos.y;
	if (x > RIGHT_EDGE || x <LEFT_EDGE || y > HEIGHT_EDGE || y < LOW_EDGE)
		return true;
	return false;
}
bool CollisionSystem::IsHit(BasicBox * box1, BasicBox * box2)
{
	bool hit = false;
	if (box1->tag == ROUNDBOX)
	{
		if (box2->tag == ROUNDBOX)
			hit = D2DHit((RoundBox*)box1, (RoundBox*)box2);
		else
			hit = D2DHit((SquareBox*)box2, (RoundBox*)box1);
	}
	else
	{
		if (box2->tag == SQUAREBOX)
			hit = D2DHit((SquareBox*)box1, (SquareBox*)box2);
		else
			hit = D2DHit((SquareBox*)box1, (RoundBox*)box2);
	}
	return hit;
}

//圆型盒子先碰撞
bool CollisionSystem::D2DHit(RoundBox * box1, RoundBox * box2)
{
	//留意，这里只是做距离的平方的比较，并不开根号
	float mindis = box1->radius + box2->radius;
	mindis *= mindis;//不开跟，用平方作比较
	float deltaX = box1->_position.x - box2->_position.x;
	float deltaY = box1->_position.y - box2->_position.y;
	float Delta = deltaX*deltaX + deltaY*deltaY;

	return Delta <= mindis;
}

bool CollisionSystem::D2DHit(SquareBox *box1, SquareBox* box2)
{
	//根据相斥来判断，即不碰撞的情况
	//先确定左右盒子
	SquareBox *lbox, *rbox;
	if (box1->_position.x > box2->_position.x)
		rbox = box1, lbox = box2;
	else
		rbox = box2, lbox = box1;
	//右盒子的左手边大于左盒子的右手边，不会碰撞
	if(rbox->lx - lbox->rx>0.001)
		return false;
	//同理，看上下边之间的关系
	if (rbox->ly - lbox->hy > 0.001 || rbox->hy - lbox->ly<0.001)
		return false;
	return true;
}

//此算法是检测在矩形是水平的情况
bool CollisionSystem::D2DHit(SquareBox *sbox, RoundBox *rbox)
{
	float rx = rbox->_position.x;
	float ry = rbox->_position.y;
	float r = rbox->radius;
	//圆在矩形水平两侧的范围之外，就不会发生碰撞
	if (sbox->rx+r < rx || sbox->lx- r > rx)
		return false;
	//如果圆在矩形左右两侧之中，看看是否在上下两侧之外，是就不会相交
	if (sbox->hy + r < ry || sbox->ly-r>ry)
		return false;
	return true;
}




//-----------------------------------------------------------
//场景管理器
//-----------------------------------------------------------

void SceneManager::InitDigital()
{
	score = 0, distance = 0; gameTime = 0;
	IsDefeatBoss = false;
	goldCoinNum = silverCoinNum = 0;
	wo->Coins.erase(wo->Coins.begin(),wo->Coins.end());
	wo->Player.erase(wo->Player.begin(), wo->Player.end());
	wo->PlayerProp.erase(wo->PlayerProp.begin(),wo->PlayerProp.end());
	wo->Enemy.erase(wo->Enemy.begin(), wo->Enemy.end());
	wo->EnemyProp.erase(wo->EnemyProp.begin(),wo->EnemyProp.end());
}


//------------------------------------------------------------------------
//检测生存情况
//------------------------------------------------------------------------
void SceneManager::Check()
{
	PRoleIter roleiter;
	PMonsIter monsiter;
	PPropIter propiter;
	PCoinIter coiniter;
	PSkillIter skilliter;
	//检查玩家情况
	for (roleiter = wo->Player.begin(); roleiter != wo->Player.end(); )
	{
		if ((*roleiter)->_state != LIVE)
		{
			roleiter = wo->Player.erase(roleiter);
			if (roleiter == wo->Player.end())
				break;
		}		
		else
			roleiter++;
	}

	//检查怪物情况，若不是活着，将其从系统中除去
	for (monsiter = wo->Enemy.begin(); monsiter != wo->Enemy.end(); )
	{	
		if ((*monsiter)->state != LIVE )
		{
			monsiter =wo->Enemy.erase(monsiter);
			if (monsiter == wo->Enemy.end())
				break;
		}
		else
			monsiter++;
	}
	//检查道具情况
	for (propiter = wo->PlayerProp.begin(); propiter != wo->PlayerProp.end(); )
	{
		if ((*propiter)->state != LIVE)
		{
			propiter = wo->PlayerProp.erase(propiter);
			if (propiter == wo->PlayerProp.end())
				break;
		}
		else
			propiter++;
	}
	for (propiter = wo->EnemyProp.begin(); propiter != wo->EnemyProp.end(); )
	{
		if ((*propiter)->state != LIVE)
		{
			propiter = wo->EnemyProp.erase(propiter);
			if (propiter == wo->EnemyProp.end())
				break;
		}
		else
			propiter++;
	}
	for (coiniter = wo->Coins.begin(); coiniter != wo->Coins.end(); )
	{
		if ((*coiniter)->state != LIVE)
		{
			coiniter = wo->Coins.erase(coiniter);
			if (coiniter == wo->Coins.end())
				break;
		}
		else
			coiniter++;
	}

	for (skilliter = wo->Skill.begin(); skilliter != wo->Skill.end(); )
	{
		if ((*skilliter)->_liveState != LIVE)
		{
			skilliter = wo->Skill.erase(skilliter);
			if (skilliter == wo->Skill.end())
				break;
		}
		else
			skilliter++;
	}
}


//----------------------------------------------------------------------
//碰撞检测
//----------------------------------------------------------------------
void SceneManager::HitTest()
{
	PRoleIter roleiter;
	PMonsIter monsiter;
	PPropIter propiter;
	PCoinIter coiniter;
	PSkillIter skill;

	//-------------------------------------------------
	//越界检测
	//-------------------------------------------------

	if (wo->Player.size() > 0)
	{
		roleiter = wo->Player.begin();
		if (IsOutRanger((*roleiter)->_position))
			SendMessge(EVEN_GAMEOVER, (void*)(*roleiter), 0);
	}
	

	for (monsiter = wo->Enemy.begin(); monsiter != wo->Enemy.end(); monsiter++)
	{
		if (IsOutRanger((*monsiter)->_position))
			(*monsiter)->state = DEATH;
	}



	for (skill = wo->Skill.begin(); skill != wo->Skill.end(); skill++)
	{
		if (IsOutRanger((*skill)->_pos))
			(*skill)->_liveState = DEATH;
	}

	
	for (coiniter = wo->Coins.begin(); coiniter != wo->Coins.end(); coiniter++)
	{
		if (IsOutRanger((*coiniter)->_position))
			(*coiniter)->state = DEATH;

	}
	//----------------------------------------------------------------
	//碰撞检测
	//----------------------------------------------------------------
	for (roleiter = wo->Player.begin(); roleiter != wo->Player.end(); )
	{	//先是做玩家和怪物的碰撞检测

		for (monsiter = wo->Enemy.begin(); monsiter != wo->Enemy.end(); monsiter++)
		{
			if (ColSys.IsHit((BasicBox*)(*roleiter)->box, (BasicBox*)(*monsiter)->box))
				SendMessge(HIT_ENHPL, (void*)(*roleiter), (void*)(*monsiter));

			//技能和怪物的碰撞
			for (skill = wo->Skill.begin(); skill != wo->Skill.end(); skill++)
			{
				if (ColSys.IsHit((BasicBox*)(*skill)->_box, (BasicBox*)(*monsiter)->box))
					SendMessge(HIT_PLSKHEN,(void*)(*skill), (void*)(*monsiter));
			}
		}

		//做玩家和对方道具的碰撞检测
		for (propiter = wo->EnemyProp.begin(); propiter != wo->EnemyProp.end(); propiter++)
		{
			if (ColSys.IsHit((BasicBox*)(*roleiter)->box, (BasicBox*)(*propiter)->box))
				SendMessge(HIT_PLPRHENPR, (void*)(*roleiter), (void*)(*propiter));
		}

		


		//是否捡到了硬币
		for (coiniter = wo->Coins.begin(); coiniter != wo->Coins.end(); coiniter++)
		{
			//（*coiniter）==coin*, coin*->box == RoundBox, 要将其转化为基类box就只能用这种方法了
			if (ColSys.IsHit((BasicBox*)(*roleiter)->box, (BasicBox*)(&(*coiniter)->box)))
				SendMessge(HIT_PLHCO, (void*)(*roleiter), (void*)(*coiniter));
		}



		//检查是否在地图上
		for (int i = 0; i < map->GetNum(); i++)
		{
			PlayerClass *play = (PlayerClass *)(*roleiter);
			SquareBox *box = (SquareBox*)(play->box);
			if (!map->IsOnMap(box->lx, box->rx))//不在地图上，那就是掉下去了，下面也不用检查
				{	SendMessge(FALLDOWN, (void*)(*roleiter), 0); break;	}
			if (ColSys.IsHit((BasicBox*)(*roleiter)->box, (BasicBox*)(&map->box[i])))
			{
				if (map->IsOnMap( box->lx, box->rx))
				{
					play->velocity.y = 0; 
					float y = map->box[i].hy + (box->hy-box->ly)/2+0.001;
					play->MoveToY(y);
					SendMessge(HIT_ONMAP, (void*)(*roleiter), (void*)(&map->box[i]));
				}
				else
				{
					play->velocity.x = map->GetVelocity();
					SendMessge(FALLDOWN, (void*)(*roleiter), (void*)(&map->box[i]));
				}
				break;
			}
		}
		roleiter++;
	}
}

//-----------------------------------------------------------------------------
//Disc:通过判断事件，转换传进来的参数，再处理时间
//对于碰撞事件，最后要改变碰撞一方或双方的状态
//-----------------------------------------------------------------------------
void SceneManager::SendMessge(EVEN even, void* user, void* obj)
{
	RoleBasic*  _player;
	MonsterBasic* _enemy;
	PropBasic*    playerprop;
	PropBasic*	  enemyprop;
	coin*		acoin;
	PlayerClass* play;
	SkillBasicClass* skill;
	if (*_gameprocess != GAME_PLAYING)
		return;
	switch (even)
	{
	case HIT_ENHPL://怪物撞人事件
		play = (PlayerClass*)user;
		_player = (RoleBasic*)user; _enemy = (MonsterBasic*)obj;
		if (_enemy->_hitTime > 0)
		{
			_player->_lifeValue -= _enemy->GetATK();
			score += 1;
			if (_player->_lifeValue <= 0)
				SendMessge(EVEN_GAMEOVER,user,0);
			_enemy->_hitTime = -1.5f;
		}
		
		break;

	case HIT_PLHCO://捡到了个硬币
		acoin = (coin*)obj; score += acoin->value;
		if (acoin->type == GOLDCOIN)
			goldCoinNum++;
		else if (acoin->type == SILVERCOIN)
			silverCoinNum++;
		acoin->state = DEATH;
		break;

	case HIT_PLSKHEN:
		skill = (SkillBasicClass*)user;
		_enemy = (MonsterBasic*)obj;
		//没有死亡才继续处理
		if (_enemy->_lifeValue > 0)
		{
			_enemy->_lifeValue -= skill->_atk;
			if (_enemy->_lifeValue <= 0)
				_enemy->ChangeAction(DYING);
		}
		break;

	case HIT_ONMAP://落在地图上
		play = (PlayerClass*)user;
		if (play->_action == JUMP) play->ChangeAction(RUN);
		break;

	case FALLDOWN:
		play = (PlayerClass*)user;
		if (play->_action != JUMP) play->ChangeAction(FALL);
		break;

	case HIT_MAPHPL:
		play = (PlayerClass*)user;
		play->ChangeAction(FALL), score += 1;
		break;

	case EVEN_GAMEOVER:
		play = (PlayerClass*)user;
		play->ChangeAction(FALL);
		play->_state = DEATH;
		*_gameprocess = GAME_OVER;
		calGUI->SetDigital(distance,goldCoinNum,silverCoinNum);
		calGUI->SetDefeatBoss(IsDefeatBoss);
		break;
	}
}

//--------------------------------------------------------------
//输入处理
//--------------------------------------------------------------
void SceneManager::DisposInput(float timeDelta,float mousex,float mousey, int mousestate)
{
	if (*_gameprocess != GAME_PLAYING)
		return;
	PlayerClass *play = (PlayerClass*)(*(wo->Player.begin()));

	//时间间隔控制器，防止一下按得过快
	static float timespan[5] = { 1.0f,1.0f,1.0f,1.0f,1.0f };
	for (int i = 0; i < 5; i++)
	{
		timespan[i] += timeDelta;
		if (timespan[i] > 2.0f)
			timespan[i] = 1.0f;
	}

	if (timespan[0] > 0.93f)
	{
		if (::GetAsyncKeyState('N') & 0x8000f)
		{		
			if (play->jumpnum == 2)
				
					play->velocity.y = 40, play->ChangeAction(JUMP), play->jumpnum--;
			else if (play->jumpnum == 1)
				if (play->velocity.y > 0)
					play->velocity.y += 30, play->ChangeAction(JUMP), play->jumpnum--;	
				else
					play->velocity.y = 30, play->ChangeAction(JUMP);
			timespan[0] = 0.45f;	
		}		
	}
	if (timespan[1] > 0.93f)
	{
		if (::GetAsyncKeyState('M') & 0x8000f)
			play->ChangeAction(RUN),timespan[1] = 0.5f;
	}
	if (::GetAsyncKeyState('B') & 0x8000f)
		play->InitDigital();
	if (::GetAsyncKeyState('V') & 0x8000f)
		play->Move(0,1*timeDelta);
	if (::GetAsyncKeyState('C') & 0x8000f)
		play->Move(0, -1 * timeDelta);

	if (timespan[2] > 0.93f)
	{
		if (::GetAsyncKeyState('1') & 0x8000f)
		{
			D3DXVECTOR3 ppos = play->GetPos();
			ppos += D3DXVECTOR3(6,0,0);
			skillManager->UseSkill(play, SKILL_SwordKee, ppos, &D3DXVECTOR3(mousex,mousey,-1.0));
		}
			
		timespan[2] = 0.5f;
	}
	

	float casp = 0.50f;
	if (::GetAsyncKeyState('A') & 0x8000f)
		camera->MoveAlongRightVec(-casp);
	if (::GetAsyncKeyState('D') & 0x8000f)
		camera->MoveAlongRightVec(casp);
	if (::GetAsyncKeyState('W') & 0x8000f)
		camera->MoveAlongLookVec(casp);
	if (::GetAsyncKeyState('S') & 0x8000f)
		camera->MoveAlongLookVec(-casp);
	if (::GetAsyncKeyState('Q') & 0x8000f)
		camera->MoveAlongUpVec(casp);
	if (::GetAsyncKeyState('E') & 0x8000f)
		camera->MoveAlongUpVec(-casp);
	float v = 0.05f;
	//倾斜
	if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
		camera->RotationUpVec(v);
	if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
		camera->RotationUpVec(-v);
	if (::GetAsyncKeyState(VK_UP) & 0x8000f)
		camera->RotationRightVec(-v);
	if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
		camera->RotationRightVec(v);

	if (::GetAsyncKeyState('L') & 0x8000f)
		camera->RotationUpVec(v);
	if (::GetAsyncKeyState('J') & 0x8000f)
		camera->RotationUpVec(-v);
	if (::GetAsyncKeyState('I') & 0x8000f)
		camera->RotationRightVec(-v);
	if (::GetAsyncKeyState('K') & 0x8000f)
		camera->RotationRightVec(v);
	if (::GetAsyncKeyState('U') & 0x8000f)
		camera->RotationLookVec(v);
	if (::GetAsyncKeyState('O') & 0x8000f)
		camera->RotationLookVec(-v);
}


//--------------------------------------------------------
//获得分数
//--------------------------------------------------------
LONG SceneManager::GetScore()
{
	score += distance;
	return score;
}

void SceneManager::Update(float timeDelta)
{
	
	if (wo->Player.size() <= 0)
	{
		hud->UpdateLifeValue(0);
		return;
	}
	PlayerClass *play = (PlayerClass*)(*(wo->Player.begin()));
	if(((int)gameTime) % 10 == 0)//每十秒增加一个技能
		play->ChangerSkillNum(SKILL_SwordKee, 1);

	hud->ChangeSkillState(SKILL_SwordKee, play->GetSkillNum(SKILL_SwordKee));
	hud->ChangeSkillState(Skill_Dash,play->GetSkillNum(Skill_Dash));
	hud->ChangeSkillState(SKILL_Shield,play->GetSkillNum(SKILL_Shield));
	hud->ChangeSkillState(SKILL_Dart, play->GetSkillNum(SKILL_Dart)); 
	
	
	hud->UpdateDistance(distance);
	hud->UpdateLifeValue(play->_lifeValue);
	gameTime += timeDelta;
	distance = _gameVelocity*gameTime;

	if (gameTime < 30)
		map->SetLevel(1);
	else if (gameTime < 100)
		map->SetLevel(2);
	else
		map->SetLevel(3);
}