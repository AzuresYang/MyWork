#include"DXUT.h"
#include"AI.h"

void AI_AttackLine::TakeAction(MonsterBasic* user, D3DXVECTOR3 *tag, float timeDelta)
{
	switch (user->action)
	{
	case NONE:
	case READY:
		if (user->_position.x <Scr_Right_EDGE + 10)
			user->_velocity.x = -12;
		user->ai_time += timeDelta;
		if (user->ai_time > 2.0f)
		{
			float f = GetRandomFloat(0, 2.99);
			if(user->_position.y<10|| f<1.0)//Ì«µÍÎªÖ±Ïß¹¥»÷
				user->ChangeAction(ATTACK);
			else if (f < 1.99)
				user->ChangeAction(ATT_CatchAttack);
			else
				user->ChangeAction(Wander);
			user->ai_time = 0;
		}
			//user->_velocity = D3DXVECTOR3(0, 0, 0);
		user->_frame = 3;
		break;
	case ATTACK:
		user->_velocity.x = -100;
		break;
	case ATT_CatchAttack:
		user->ai_time += timeDelta;
		if (user->ai_time < 0.2f)
		{
			user->_velocity = (*tag) - user->_position;
			D3DXVec3Normalize(&user->_velocity, &user->_velocity);
			user->_velocity *= 100;
		}
		break;
	case Wander:
		user->ai_time += timeDelta;	
		user->_velocity.y = GetRandomFloat(3,10-(user->_position.y/10))*sin(user->ai_time);
		break;
	}
}