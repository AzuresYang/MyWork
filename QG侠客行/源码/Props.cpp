#include"DXUT.h"
#include"Props.h"



//--------------------------------------------
//��ʼ������
//--------------------------------------------
void CoinsClass::InitDigital()
{
	vector<coin*>::iterator iter;
	for (iter = coins.begin(); iter != coins.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	coins.erase(coins.begin(), coins.end());
	_time = 0;
}


//--------------------------------------------
//��ʼ�����
//--------------------------------------------
HRESULT CoinsClass::Init(LPDIRECT3DDEVICE9 device, wchar_t *filename, int nrow, int ncow, float frame,
						float w, float h,float ve, void(*regis)(coin* acoin))
{
	HRESULT hr;
	_device = device;
	_ncow = ncow, _nrow = nrow;//���и�ֵ
	_sizew = w, _sizeh = h;//��߸�ֵ
	_vset = 1.0f / _nrow, _uset = 1.0f / _ncow;//���ֺ�����ͼ�е�����
	_frame = frame;//ÿ��֡��
	frameNum = 1;
	velocity = ve;
	D3DXMatrixIdentity(&world);
	regist = regis;
	V_RETURN(D3DXCreateTextureFromFile(_device, filename, &_texture));
	_z = -1.0f;
	_r = _sizew*0.8;
	BASICVERTEX obj[] =
	{

		{ -w / 2, -h / 2,   -1.0,  0.0f,_vset },
		{ -w / 2, h / 2,	-1.0,  0.0f, 0.0f },
		{ w / 2, -h / 2,    -1.0,  _uset,_vset },
		{ w / 2, h / 2,     -1.0,  _uset, 0.0f },
	};
	// �������㻺��
	V_RETURN(_device->CreateVertexBuffer(sizeof(obj), 0, D3DFVF_BASIC,
		D3DPOOL_MANAGED, &_buffer, NULL));
	// ��䶥�㻺��
	void *ptr;
	V_RETURN(_buffer->Lock(0, sizeof(obj), (void**)&ptr, 0));
	memcpy(ptr, obj, sizeof(obj));
	_buffer->Unlock();
	return S_OK;
}

void CoinsClass::Move(coin * acoin, float x, float y)
{
	acoin->_position += D3DXVECTOR3(x, y, 0);
	acoin->box.Move(x, y);
}


//------------------------------------------
//����
//------------------------------------------
bool CoinsClass::Update( float timeDelta)
{
	
	vector<coin*>::iterator iter;
	coin* acoin;//��ʱʹ��
	for (iter = coins.begin(); iter != coins.end(); )
	{
		if ((*iter)->state!= LIVE)
		{
			acoin = *iter;
			iter = coins.erase(iter);
			SAFE_DELETE(acoin);
			if (iter == coins.end())
				break;
		}
		else
			iter++;
	}

	for (iter = coins.begin(); iter != coins.end(); iter++)
	{
		acoin = *iter;
		acoin->time += timeDelta;
		if (acoin->time > 1 / _frame)
		{
			acoin->frame = (acoin->frame + 1) % _ncow;
			acoin->time = 0;
		}
		Move(acoin, velocity*timeDelta, 0);
	}

	//------------�������--------------------
	
// 	_time -= timeDelta;
// 	if (_time < 0)
// 	{
// 		CreateCoins(0);
// 		_time = GetRandomFloat(8, 14);
// 	}
	return true;
}

//--------------------------------------------
//��Ⱦ
//--------------------------------------------
void CoinsClass::Render(float timeDelta)
{
	HRESULT hr;
	BASICVERTEX *v;
	vector<coin*>::iterator iter;
	coin* acoin;
	_device->SetTexture(0, _texture);
	_device->SetFVF(D3DFVF_BASIC);
	int num;
	for (iter = coins.begin(); iter != coins.end(); iter++)
	{
		acoin = *iter;
		
		_device->SetTransform(D3DTS_WORLD, &world);
		_device->SetStreamSource(0, _buffer, 0, sizeof(BASICVERTEX));
		if (acoin->type == GOLDCOIN)
			num = 1;
		else
 			num = 0;
		//����Ӳ�ҵ������������򣬴ﵽ����Ч��
		V(_buffer->Lock(0, 0, (void**)&v, 0));
		v[0]._u = v[1]._u = _uset*acoin->frame;
		v[2]._u = v[3]._u = _uset*(acoin->frame + 1);
		v[0]._v = v[2]._v = _vset*(num+1);
		v[1]._v = v[3]._v =	 _vset*num;
		_buffer->Unlock();
		//�����ػ�λ��
		D3DXMatrixTranslation(&world, acoin->_position.x, acoin->_position.y, acoin->_position.z);
		_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		
	}
	
	_device->SetTexture(0, 0);
}



//--------------------------------------------
//����һ�����
//--------------------------------------------
void CoinsClass::AddCoin(D3DXVECTOR3 pos, CoinType type)
{
	coin* acoin = new coin;
	acoin->_position = pos;
	acoin->type = type;
	acoin->value = 200;
	acoin->frame = GetRandomFloat(0, frameNum + 0.2);
	acoin->box.Init(acoin->_position, _r);
	coins.push_back(acoin);
	regist(acoin);
}


void CoinsClass::SortCoin(UINT star,UINT it,coin* acoin)
{
	float distance;
	float setx, sety;
	setx = _r*0.1;
	sety = GetRandomFloat(-_r*0.1, _r*0.1);//��������ƶ�
	for (UINT i = star; i < coins.size(); i++)
	{
		if (i == it)
			continue;
		distance = CalDistanceSq(&acoin->_position, &coins[i]->_position);
		//����̫��ʱ����������ҷֿ�
		if (distance < _sizew*_sizew)//����_r��ԭ���ǽ�ҵ��ڵ����⣬�й��زģ���ʱ�޷����
		{	
			
			Move(acoin, setx, sety);
			while (distance < _sizew*_sizew)
			{
				Move(acoin, setx,sety);
				if (acoin->_position.y < 6 || acoin->_position.y > 25)//���Ƹߵ�
					sety = 0;
				distance = CalDistanceSq(&acoin->_position, &coins[i]->_position);
			}
		}
	}
}


void CoinsClass::CreateCoins(int type)
{
	
	if(type ==1)
	 Coin_Random();
	else if(type == 2)
		Coin_Guide();
	else if (type == 3)
		Coin_QG();

	else
	{
		type = GetRandomFloat(1, 3);
		CreateCoins(type);
	}
	
}

void CoinsClass::Coin_Random()
{
	int num;
	num = GetRandomFloat(9.1, 14.5);//������ɽ������
	float lx, rx, ly, hy;
	lx = Scr_Right_EDGE;
	ly = 13;//�������λ��
	D3DXVECTOR3 pos;
	CoinType type;
	if (GetRandomFloat(0, 2) > 1.0)
		type = GOLDCOIN;
	else
		type = SILVERCOIN;
	UINT star = coins.size()-1;//��¼��һ���ε���Ϣ
	if (coins.size() == 0)
		star = 0;
	num = 10;
	for (int i = 0; i < num; i++)
	{
		pos.x = lx;
		pos.y = ly;
		pos.z = _z;
		AddCoin(pos, type);
		SortCoin(star, coins.size() - 1,coins[coins.size() - 1]);//ȷ����Ҳ����ص�
	}
}

void CoinsClass::Coin_QG()
{
	float lx = Scr_Right_EDGE;
	float ly = 10.5;
	float offsetx = 5.5;
	float offsety = 6;

	//ƴдQ
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if ((j == 1 || j== 2)&& i != 0 && i != 4)
				continue;
			AddCoin({ lx + offsetx*j,ly + offsety*i,_z }, GOLDCOIN);
		}
	}
	AddCoin({ lx + offsetx*4-1,ly - 2,_z }, GOLDCOIN);
	AddCoin({ lx + offsetx * 5-1,ly-2.5f,_z }, GOLDCOIN);

	//ƴдG
	float glx = lx + offsetx * 5 - 1 + 8;
	float goffsety = 6;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (i == 1)
				if (j == 0 || j == 4)
					AddCoin({ glx + offsetx*j,ly + goffsety*i,_z }, GOLDCOIN);

			if (i == 2)
				if (j != 1)
					AddCoin({ glx + offsetx*j,ly + goffsety*i,_z }, GOLDCOIN);
			if(i==3)
			{
				AddCoin({ glx + offsetx*j,ly + goffsety*i,_z }, GOLDCOIN);
				break;
			}
			if(i==0||i==4)
				AddCoin({ glx + offsetx*j,ly + goffsety*i,_z }, GOLDCOIN);
		}
	}
}


void CoinsClass::Coin_Guide()
{
	float lx = Scr_Right_EDGE;
	float ly = 15;
	float offsetx = 5.5;
	float offsety = 4.5;

	//ƴд��ͷ
	for (int i = 0; i < 8; i++)
	{
		if(i!=5&&i!=6&&i!=7)
			AddCoin({ lx + offsetx*i,ly,_z  }, GOLDCOIN);
		if (i == 5)
		{
			AddCoin({ lx + offsetx*i-3,ly+offsety*2,_z }, GOLDCOIN);
			AddCoin({ lx + offsetx*i-3,ly + -offsety * 2,_z }, GOLDCOIN);
		}
		if (i == 6)
		{
			AddCoin({ lx + offsetx*i-5,ly + offsety ,_z }, GOLDCOIN);
			AddCoin({ lx + offsetx*i-5,ly + -offsety ,_z }, GOLDCOIN);
		}
		if (i == 7)
			AddCoin({ lx + offsetx*i - 7,ly ,_z }, GOLDCOIN);
	}
}