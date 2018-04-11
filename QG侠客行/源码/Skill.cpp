#include"DXUT.h"
#include"Skill.h"

Skill_SwordKeeClass::Skill_SwordKeeClass()
{
	_device = NULL;
	_texture = NULL;
	_buffer = NULL;
}

void Skill_SwordKeeClass::Clear()
{
	SAFE_RELEASE(_texture);
	SAFE_RELEASE(_device);
	for (UINT i = 0; i < _skills.size(); i++)
	{
		SAFE_DELETE(_skills[i]->_box);
		SAFE_DELETE(_skills[i]);
	}
	_skills.erase(_skills.begin(),_skills.end());
}

HRESULT Skill_SwordKeeClass::Init(LPDIRECT3DDEVICE9 device, wchar_t *filename,
								int nrow, int ncow, float frame, float w, float h,float z, float velocity,UINT atk,
								void(*regis)(SkillBasicClass* skill))
{
	HRESULT hr;
	_device = device;
	_atk = atk;
	_velocity = velocity;
	regist = regis;
	_ncow = ncow, _nrow = nrow;//行列赋值
	_sizew = w, _sizeh = h;//宽高赋值
	_vset = 1.0f / _nrow, _uset = 1.0f / _ncow;//划分好纹理图中的区域
	D3DXMatrixIdentity(&_world);
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
	V_RETURN(_device->CreateVertexBuffer(sizeof(obj), 0, D3DFVF_BASIC,
		D3DPOOL_MANAGED, &_buffer, NULL));
	// 填充顶点缓存
	void *ptr;
	V_RETURN(_buffer->Lock(0, sizeof(obj), (void**)&ptr, 0));
	memcpy(ptr, obj, sizeof(obj));
	_buffer->Unlock();
	return hr;
}

void Skill_SwordKeeClass::AddSkill(D3DXVECTOR3 pos,D3DXVECTOR3 *taget)
{
	SkillBasicClass * skill = new SkillBasicClass;
	skill->_atk = _atk;
	skill->_campTag = PLAYER;
	skill->_liveState = LIVE;
	skill->_skillTag = SKILL_SwordKee;
	skill->_pos = pos;
	skill->_time = 0;

	(skill->_velocity)= *taget - pos;
	D3DVecNormal(&skill->_velocity);

	skill->_velocity *= _velocity;
	skill->_box = new SquareBox(pos,_sizew*0.9,_sizeh*0.9);
	float angle = (taget->y - pos.y) / fabs((taget->x - pos.x));
	skill->_angle = atan(angle);
	regist(skill);
	_skills.push_back(skill);
}


void Skill_SwordKeeClass::Update(float timeDelta)
{
	vector<SkillBasicClass*>::iterator skillIter;
	SkillBasicClass* skill;
	//清除不是活着的技能
	for (skillIter = _skills.begin(); skillIter != _skills.end(); )
	{
		if ((*skillIter)->_liveState != LIVE)
		{
			skillIter = _skills.erase(skillIter);

			if (skillIter == _skills.end())
				break;
		}
		else
			skillIter++;
	}

	for (UINT i = 0; i < _skills.size(); i++)
	{
		_skills[i]->Move(_skills[i]->_velocity.x*timeDelta, _skills[i]->_velocity.y*timeDelta);
	}
}


void Skill_DartClass::Update(float timeDelta)
{
	vector<SkillBasicClass*>::iterator skillIter;
	SkillBasicClass* skill;
	//清除不是活着的技能
	for (skillIter = _skills.begin(); skillIter != _skills.end(); )
	{
		if ((*skillIter)->_liveState != LIVE)
		{
			skillIter = _skills.erase(skillIter);

			if (skillIter == _skills.end())
				break;
		}
		else
			skillIter++;
	}

	for (UINT i = 0; i < _skills.size(); i++)
	{
		_skills[i]->Move(_skills[i]->_velocity.x*timeDelta, _skills[i]->_velocity.y*timeDelta);
		_skills[i]->_angleSpin += timeDelta;
	}
}


void Skill_SwordKeeClass::Render()
{
	HRESULT hr;
	BASICVERTEX *v;
	_device->SetTexture(0, _texture);
	_device->SetStreamSource(0, _buffer, 0, sizeof(BASICVERTEX));
	_device->SetFVF(D3DFVF_BASIC);
	D3DXMATRIX spinWorld;
	for (UINT i = 0; i < _skills.size(); i++)
	{
		D3DXMatrixTranslation(&_world, _skills[i]->_pos.x, _skills[i]->_pos.y, _skills[i]->_pos.z);
		D3DXMatrixRotationZ(&spinWorld,_skills[i]->_angle);
		_world = spinWorld*_world;
		_device->SetTransform(D3DTS_WORLD, &_world);
		_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}


void SkillManager::UseSkill(RoleBasic* role,SkillTag tag,  D3DXVECTOR3 pos, D3DXVECTOR3 *taget)
{
	//UINT num = role->GetSkillNum(tag);
	//if (num < 1)
	//	return;
	switch (tag)
	{
	case SKILL_SwordKee:
		_swordKee->AddSkill(pos,taget);
		//role->ChangerSkillNum(tag, -1);
		break;
	}
}

void SkillManager::Update(float timeDelta)
{
	_swordKee->Update(timeDelta);
}

void SkillManager::Render()
{
	_swordKee->Render();
}

void SkillManager::LoadSource()
{
	_swordKee = new Skill_SwordKeeClass;
	_swordKee->Init(_device, L"Media//剑气1.png", 1, 1, 1, 7, 7, -1.0f, 75, 5, _regist);
}

