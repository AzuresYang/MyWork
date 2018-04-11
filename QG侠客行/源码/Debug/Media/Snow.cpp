#include"Snow.h"
SnowClass::SnowClass(IDirect3DDevice9 *pd3dDevice)
{
	p_device = pd3dDevice;
	p_vertexbuffer = NULL;
	wind_X = 4;
	for(int i = 0; i< 5;i++)
		p_texture[i] = NULL;
	
}

HRESULT SnowClass::InitSnow(IDirect3DTexture9* *tex)
{
	int i;
	for(i = 0; i<6; i++)
		p_texture[i] = tex[i];
	srand(GetTickCount());
	for(i=0; i<SNOW_NUMBER; i++)
	{
		m_Snows[i].x			  = float(rand()%SNOW_LENGTH_X - SNOW_LENGTH_X/2);
		m_Snows[i].z			  = float(rand()%SNOW_WIDTH_Z - SNOW_WIDTH_Z/2);
		m_Snows[i].y			  = float(rand()%SNOW_HEIGHT_Y);
		m_Snows[i].RotationY	  = (rand()%100)/50.0f*D3DX_PI;
		m_Snows[i].RotationX	  = (rand()%100)/50.0f*D3DX_PI;
		m_Snows[i].FallSpeed	  = 100.0f+rand()%100;
		m_Snows[i].RotationSpeed  = 5.0f+rand()%10/10.0f;
		m_Snows[i].TextureIndex = rand()%6;
	}
	p_device->CreateVertexBuffer(4*sizeof(SnowTexture), 0, FVF_SNOWTEXTURE, D3DPOOL_MANAGED,
								&p_vertexbuffer, NULL);
	float slen = 5.0f;
	SnowTexture vertices[]=
	{
		{-slen,  0.0f,   0.0f, 0.0f, 1.0f,},
		{-slen,  2*slen, 0.0f, 0.0f, 0.0f,},
		{ slen,  0.0f,   0.0f, 1.0f, 1.0f,},
		{ slen,	 2*slen, 0.0f, 1.0f, 0.0f}
	};

	void* pvertices;
	p_vertexbuffer->Lock(0, sizeof(vertices), (void **)&pvertices, 0);
	memcpy(pvertices, vertices, sizeof(vertices));
	p_vertexbuffer->Unlock();

	return S_OK;
}

HRESULT SnowClass::UpdateSnow(float fElapsedTime, int wind)
{	
	int i;
	for(i = 0; i<SNOW_NUMBER;i++)
	{
		m_Snows[i].y -= m_Snows[i].FallSpeed*fElapsedTime;
		if(m_Snows[i].y<0)
			m_Snows[i].y=SNOW_WIDTH_Z;
		if(wind == 1)
		{
			m_Snows[i].x += wind_X;
			if(m_Snows[i].x >SNOW_LENGTH_X/2)
				m_Snows[i].x = float(rand()%SNOW_LENGTH_X - SNOW_LENGTH_X/2);
		}
		
		m_Snows[i].RotationY	+=m_Snows[i].RotationSpeed * fElapsedTime;
		m_Snows[i].RotationX	+=m_Snows[i].RotationSpeed * fElapsedTime;
	}
	return S_OK;
}

HRESULT SnowClass::RenderSnow(int flag)
{
	if(flag == 1)
	{
		//禁用照明效果
		p_device->SetRenderState(D3DRS_LIGHTING,false);

		//设置纹理状态
		p_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);//纹理颜色的第一个参数的值就取纹理颜色值
		p_device->SetTextureStageState(0, D3DTSS_COLOROP,  D3DTOP_SELECTARG1);//将纹理颜色混合的第一个参数颜色值用于输出
		
		p_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		p_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

		//设置Alpha混合系数
		p_device->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
		p_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		p_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		//剔除模式为不剔除任何面
		p_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		int i;
		
		for(i = 0;i<SNOW_NUMBER; i++)
		{
			static D3DXMATRIX matYaw, matPitch, matTrans, matWorld;
			D3DXMatrixRotationY(&matYaw, m_Snows[i].RotationY);
			D3DXMatrixRotationX(&matPitch, m_Snows[i].RotationX);
			
			D3DXMatrixTranslation(&matTrans, m_Snows[i].x, m_Snows[i].y, m_Snows[i].z);
			matWorld = matYaw * matPitch * matTrans;
			p_device->SetTransform(D3DTS_WORLD, &matWorld);

			p_device->SetTexture(0, p_texture[m_Snows[i].TextureIndex]);
			//这一行影响了害怕的坐标，应该是因为vertexbuffer同名的缘故重新绘制了害怕的坐标
			p_device->SetStreamSource(0, p_vertexbuffer, 0, sizeof(SnowTexture));
			p_device->SetFVF(FVF_SNOWTEXTURE);
			p_device->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
		}
		//恢复光照,剔除状态， Alpha混合
		p_device->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
		p_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		p_device->SetRenderState(D3DRS_LIGHTING,true);
	}
	return S_OK;
}

SnowClass::~SnowClass()
{
	if(p_vertexbuffer)	
	{
		p_vertexbuffer->Release();
		p_vertexbuffer = NULL;
	}
	for(int i =0 ;i<6; i++ )
		if(p_texture[i])	
	{
		p_texture[i]->Release();
		p_texture[i] = NULL;
	}
}
