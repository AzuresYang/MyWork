#include"DXUT.h"
#include"Monsters.h"

bool IsOutEdge(D3DXVECTOR3* pos, float w, float h)
{
	float hy, ly, lx, rx;
	hy = pos->y + h / 2;
	ly = hy - h;
	lx = pos->x - w / 2;
	rx = lx + w;
	if (hy > HEIGHT_EDGE || ly < LOW_EDGE || rx<LEFT_EDGE || lx>RIGHT_EDGE)
		return true;
	return false;
}
void MonsterClassBasic::Clear()
{
	 SAFE_RELEASE(_buffer); SAFE_RELEASE(_texture); 
	 for (int i = 0; i < monster.size(); i++)
		 SAFE_DELETE(monster[i]->box);
}

HRESULT MonsterClassBasic::Init(LPDIRECT3DDEVICE9	device, wchar_t *filename, int nrow, int ncow, 
								float w, float h, float z, void(*regis)(MonsterBasic* role))
{
	HRESULT hr = S_OK;
	_device = device;
	_ncow = ncow, _nrow = nrow;//行列赋值
	_sizew = w, _sizeh = h;//宽高赋值
	_z = z;
	_vset = 1.0f / _nrow, _uset = 1.0f / _ncow;//划分好纹理图中的区域
	D3DXMatrixIdentity(&world);
	regist = regis;//注册函数	
	V(D3DXCreateTextureFromFile(_device, filename, &_texture));
	BASICVERTEX obj[] =
	{
		{ -w / 2, -h / 2,   z,  0.0f,_vset },
		{ -w / 2, h / 2,	z,  0.0f, 0.0f },
		{ w / 2, -h / 2,    z,  _uset,_vset },
		{ w / 2, h / 2,     z,  _uset, 0.0f },
	};
	// 创建顶点缓存
	V(_device->CreateVertexBuffer(sizeof(obj), 0, D3DFVF_BASIC,
		D3DPOOL_MANAGED, &_buffer, NULL));
	// 填充顶点缓存
	void *ptr;
	V(_buffer->Lock(0, sizeof(obj), (void**)&ptr, 0));
	memcpy(ptr, obj, sizeof(obj));
	_buffer->Unlock();
	V(LoadWav());
	return hr;
}





void MonsterClass_Bat::Create()
{
	float fnum = GetRandomFloat(1.0, 3.99);
	int num = (int)fnum;
	if (num > 1.5)
		num = 2;
	float ly = GetRandomFloat(10, 40);
	float rx = Scr_Right_EDGE;
	float offsetx, offsety;
	offsetx = _sizew + 2;
	offsety = GetRandomFloat(5, 7);
	for (int i = 0; i < num; i++)
	{
		if (i == 0)
			AddBat({rx,ly,_z});
		if (i == 1)
		{
			offsety = GetRandomFloat(0,6);
			AddBat({rx+offsetx,ly+offsety,_z});
		}
		if (i == 2)
		{
			offsety = GetRandomFloat(0, 6);
			AddBat({ rx + offsetx,ly + -offsety,_z });
		}			
	}
}

void MonsterClass_Bat::AddBat(D3DXVECTOR3 pos)
{
	Monster_Bat* abat = new Monster_Bat;
	abat->_position = pos;
	abat->_velocity = D3DXVECTOR3(-10, 0, 0);
	abat->mass = 2;
	abat->_lifeValue = 1;
	abat->atk = 1;
	abat->box = new SquareBox(abat->_position, _sizew, _sizeh);
	monster.push_back(abat);
	regist(abat);
}

void MonsterClass_Bat::Check()
{
	PMonsIter monsiter;
	MonsterBasic* temp; 

	for (monsiter = monster.begin(); monsiter != monster.end(); )
	{
		
		if ((*monsiter)->state != LIVE && (*monsiter)->state != GOINGDYING)
		{
			temp = *monsiter;
			monsiter = monster.erase(monsiter);
			SAFE_DELETE(temp);
			if (monsiter == monster.end())
				break;
		}
		else if (IsOutEdge(&(*monsiter)->_position, _sizew, _sizeh))
		{
			(*monsiter)->state = OUTRANGE;
		}
		monsiter++;
	}
}

void MonsterClass_Bat::Update(float timeDelta, D3DXVECTOR3 tag)
{
// 	PMonsIter monsiter;
// 	for (monsiter = monster.begin(); monsiter != monster.end(); monsiter++)
// 	{
// 		monsiter->
// 	}
	Check();

	if (monster.size() > 0)
	{
		Monster_Bat *bat;
		for (int i = 0; i < monster.size(); i++)
		{
			bat = (Monster_Bat *)monster[i];
			//濒死状态更新
			if (bat->action == DYING)
			{
				bat->_time -= timeDelta;
				bat->_color = D3DCOLOR_ARGB((int)(bat->_time * 255), 255, 255, 255);
				if (bat->_time < 0)
					bat->state = DEATH;
				continue;
			}
			else
				bat->bat_ai.TakeAction(monster[i], &tag, timeDelta);

			//帧数计时器
			monster[i]->_time += timeDelta;
			if (monster[i]->_time > 1.0f / monster[i]->_frame)
			{
				monster[i]->_framenum = (monster[i]->_framenum + 1) % _ncow;
				monster[i]->_time = 0;
			}
			//碰撞计时器
			monster[i]->_hitTime += timeDelta;
			if (monster[i]->_hitTime>5.0f)
				monster[i]->_hitTime = 0.1f;
			//正常移动
			monster[i]->Move(monster[i]->_velocity.x*timeDelta, monster[i]->_velocity.y*timeDelta);
		}
	}
	static float time = 3;
	time -= timeDelta;
	if (time < 0)
	{
		Create();
		time = GetRandomFloat(7,9);
	}
		//
	
}

void Monster_Bat::ChangeAction(ActionState act)
{
	switch (act)
	{
	case READY:
		action = READY;
		break;
	case ATT_CatchAttack:
		action = ATT_CatchAttack, _framenum = 0, _rownum = 1, _frame = 10;
		break;
	case ATTACK:
		action = ATTACK,_framenum = 0, _rownum = 1,_frame = 10;
		break;
	case Wander:
		action = Wander;
		break;
	case DYING:
		action = DYING; state = GOINGDYING; _time = 1.0f;
		break;
	}

}void MonsterClassBasic::Render()
{
	HRESULT hr;
	PMonsIter monsiter;
	BASICVERTEX *v;
	
	_device->SetTexture(0, _texture);
	_device->SetStreamSource(0, _buffer, 0, sizeof(BASICVERTEX));
	_device->SetFVF(D3DFVF_BASIC);
	for (unsigned int i = 0; i < monster.size(); i++)
	{
		V(_buffer->Lock(0, 0, (void**)&v, 0));
		D3DXMatrixTranslation(&world, monster[i]->_position.x, monster[i]->_position.y, monster[i]->_position.z);
		v[0]._u = v[1]._u = _uset*monster[i]->_framenum;
		v[2]._u = v[3]._u = _uset*(monster[i]->_framenum + 1);
		v[0]._v = v[2]._v = _vset*(monster[i]->_rownum + 1);
		v[1]._v = v[3]._v = _vset*monster[i]->_rownum;
		v[0]._color = v[1]._color = v[2]._color = v[3]._color = monster[i]->_color;
		_buffer->Unlock();
		_device->SetTransform(D3DTS_WORLD, &world);
		_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}


