#include"SkyBox.h"

SkyBoxClass::SkyBoxClass(IDirect3DDevice9* device)
{
	_device = device;
}

bool SkyBoxClass::InitSkyBox(float length,	IDirect3DTexture9* *tex	)
{
	_length = length;
	for(int i = 0; i < 6; i++)
		_tex[i] = tex[i];
	_device->CreateVertexBuffer(24 * sizeof(SkyBoxTexture), 0 ,
							FVF_SKYBOXTEXTURE, D3DPOOL_MANAGED, 
							&_vertexbuffer,0);
	SkyBoxTexture vertices[]=
	{
		 //前面的四个顶点  
        { -_length/2, 0.0f,    _length/2, 0.0f, 1.0f, },  
        { -_length/2, _length/2,   _length/2, 0.0f, 0.0f, },  
        {  _length/2, 0.0f,    _length/2, 1.0f, 1.0f, },  
        {  _length/2, _length/2,   _length/2, 1.0f, 0.0f, },  
  
        //背面的四个顶点  
        {  _length/2, 0.0f,   -_length/2, 0.0f, 1.0f, },  
        {  _length/2, _length/2,  -_length/2, 0.0f, 0.0f, },  
        { -_length/2, 0.0f,   -_length/2, 1.0f, 1.0f, },  
        { -_length/2, _length/2,  -_length/2, 1.0f, 0.0f, },  
  
        //左面的四个顶点  
        { -_length/2, 0.0f,   -_length/2, 0.0f, 1.0f, },  
        { -_length/2, _length/2,  -_length/2, 0.0f, 0.0f, },  
        { -_length/2, 0.0f,    _length/2, 1.0f, 1.0f, },  
        { -_length/2, _length/2,   _length/2, 1.0f, 0.0f, },  
  
        //右面的四个顶点  
        { _length/2, 0.0f,   _length/2, 0.0f, 1.0f, },  
        { _length/2, _length/2,  _length/2, 0.0f, 0.0f, },  
        { _length/2, 0.0f,  -_length/2, 1.0f, 1.0f, },  
        { _length/2, _length/2, -_length/2, 1.0f, 0.0f, },  
  
        //上面的四个顶点  
        {  _length/2, _length/2, -_length/2, 1.0f, 0.0f, },  
        {  _length/2, _length/2,  _length/2, 1.0f, 1.0f, },  
        { -_length/2, _length/2, -_length/2, 0.0f, 0.0f, },  
        { -_length/2, _length/2,  _length/2, 0.0f, 1.0f, },  
		 //下面的四个顶点  
        {  _length/2, 0.0f,  _length/2, 1.0f, 0.0f, },  
        {  _length/2, 0.0f, -_length/2, 1.0f, 1.0f, },  
        { -_length/2, 0.0f,  _length/2, 0.0f, 0.0f, },  
        { -_length/2, 0.0f, -_length/2, 0.0f, 1.0f, }, 
  
	};

	void* pvertices;
	_vertexbuffer->Lock(0,0, (void**)&pvertices,0);
	memcpy(pvertices, vertices, sizeof(vertices));
	_vertexbuffer->Unlock();
	return TRUE;
}


void SkyBoxClass::RenderSkyBox(D3DXMATRIX *pMatWorld, bool bRenderFrame)
{
	_device->SetTextureStageState(0, D3DTSS_COLOROP,  D3DTOP_SELECTARG1);
	_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	_device->SetTransform(D3DTS_WORLD, pMatWorld);
	_device->SetStreamSource(0,_vertexbuffer, 0, sizeof(SkyBoxTexture));
	_device->SetFVF(FVF_SKYBOXTEXTURE);

	for(int i=0; i<6; i++)
	{
		_device->SetTexture(0,_tex[i]);
		_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, i*4,2);
	}
	if(bRenderFrame)
	{
		_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		for(int i = 0; i< 6; i++)
		{
			_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, i*4, 2);
		}
		_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
	
}

SkyBoxClass::~SkyBoxClass(void)
{
	if(_vertexbuffer)	
	{
		_vertexbuffer->Release();
		_vertexbuffer = NULL;
	}
	for(int i =0 ;i<6; i++ )
		if(_vertexbuffer)	
	{
		_tex[i]->Release();
		_tex[i] = NULL;
	}
}