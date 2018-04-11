#include"DXUT.h"
#include"Players.h"


void PlayerClass::InitDigital()
{
	_position = D3DXVECTOR3(27, 9.6, -1);
	_lifeValue = 3;//生命值
	box->Init(_position, boxw, boxh);//初始化碰撞盒子
	velocity = D3DXVECTOR3(0, 0, 0);
	ChangeAction(RUN);
	_action = RUN, _state = LIVE;//打上标签:活着的，没有动作的
	regist(this);//向系统登记注册
}
HRESULT PlayerClass::Init(LPDIRECT3DDEVICE9	device, wchar_t *filename, int nrow, int ncow, float frame,
							float w, float h, float z, void(*regis)(RoleBasic* role))
{
	HRESULT hr;
	_device = device;
	mass = 50;//质量为50
	_ncow = ncow,		_nrow = nrow;//行列赋值
	_sizew = w,			_sizeh = h;//宽高赋值
	boxw = 0.8*w, boxh = 0.8*h;
	_vset = 1.0f/_nrow, _uset = 1.0f/_ncow;//划分好纹理图中的区域
	_frame = frame;//每秒帧数
	frameNum = 0;
	jumpnum = 2; 
	regist = regis;//注册函数
	box = new SquareBox;
	D3DXMatrixIdentity(&world);
	V_RETURN(D3DXCreateTextureFromFile(_device, filename, &_texture));
	BASICVERTEX obj[] =
	{
		{ -w / 2, -h/2, _position.z,  0.0f,_vset},
		{ -w / 2, h/2, _position.z,  0.0f, 0.0f},
		{ w / 2, -h/2,  _position.z,  _uset,_vset},
		{ w / 2, h/2,  _position.z,  _uset, 0.0f},
	};
	// 创建顶点缓存
	V_RETURN(_device->CreateVertexBuffer(sizeof(obj), 0, D3DFVF_BASIC,
		D3DPOOL_MANAGED, &_buffer, NULL));
	// 填充顶点缓存
	void *ptr;
	V_RETURN(_buffer->Lock(0, sizeof(obj), (void**)&ptr, 0));
	memcpy(ptr, obj, sizeof(obj));
	_buffer->Unlock();

	InitDigital();
	return S_OK;
}


void PlayerClass::Update(float timeDelta)
{
	HRESULT hr;
	BASICVERTEX *v;
	V(_buffer->Lock(0, 0, (void**)&v, 0));
	switch (_action)
	{
	case RUN:
		time += timeDelta;
		if (time > 1.0f / _frame)
		{
			frameNum = (frameNum + 1) % 2;
			time = 0;
		}	
		break;
	case JUMP:
		if (velocity.y < 0)
		{
			frameNum = 1;
		}
		velocity.y -= 50 * timeDelta;
		break;
	case DYING:
	case FALL:
		frameNum = 1;
		velocity.y -= 50 * timeDelta;
		break;
	
	}
	v[0]._u = v[1]._u = _uset*frameNum;
	v[2]._u = v[3]._u = _uset*(frameNum + 1);
	Move(velocity.x*timeDelta,velocity.y*timeDelta);
	_buffer->Unlock();
}

void PlayerClass::ChangeAction(ActionState act)
{
	int num = 0;
	frameNum = 0;
	BASICVERTEX *v;
	_buffer->Lock(0, 0, (void**)&v, 0);
	switch (act)
	{
	case RUN:
		_action = RUN,  num = 0, jumpnum=2;
		break;
	case JUMP:
		_action = JUMP, num = 1;
		break;
	case FALL:
		_action = FALL, num = 2,jumpnum = 0;
		break;
	case DYING:
		_action = FALL, num = 2, jumpnum = 0;
		_state = GOINGDYING;
	}
	v[0]._v = v[2]._v = _vset*(num + 1);
	v[1]._v = v[3]._v = _vset*num;
	_buffer->Unlock();
}

UINT PlayerClass::GetSkillNum(SkillTag tag)
{
	switch (tag)
	{
	case SKILL_SwordKee:return _sworkKeeNum;

	default: return 0;
	}
	return 0;
}

void PlayerClass::ChangerSkillNum(SkillTag tag, int num)
{
	switch (tag)
	{
		case SKILL_SwordKee: _sworkKeeNum += num;
	}
}
void PlayerClass::Render()
{
	D3DXMatrixTranslation(&world, _position.x, _position.y, _position.z);
	_device->SetTransform(D3DTS_WORLD, &world);
	_device->SetTexture(0, _texture);
	_device->SetStreamSource(0, _buffer, 0, sizeof(BASICVERTEX));
	_device->SetFVF(D3DFVF_BASIC);
	_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	_device->SetTexture(0, 0);
}