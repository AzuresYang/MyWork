//--------------------------------------------------------------------------------------
// File: MyGame.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "DXUT.h"
#include "resource.h"
#include<d3dx9.h>
#include<d3d9.h>
#include"Myd3d.h"
#include"Game_Map.h"
#include"Players.h"
#include"Props.h"
#include"SceneManager.h"
#include"Monsters.h"
#include"Game_GUI.h"
#include"Skill.h"

#define Game_Velocity 10

int Width = 1440;
int Height = 800;

int Font_HUD_1 = 0;
int Font_HUD_2 = 0;
bool ButtonDown = false;
POINT	LastCursorPos;
POINT	CursorPos;
POINT   MousePos;
RECT	WinddowRect;
IDirect3DVertexBuffer9*		p_vertexbuffer = 0;
CSoundManager *g_soundmanager;
CSound *wav1;
HWND	hwnd;
IDirect3DDevice9*     Device = 0;
World	Wor;

Camera				camera;
MapClass			map;
Fond				g_fond;
BackGroudClass      g_BackGround;
int g_MouseX;
int g_MouseY;
bool g_LMBDown = false;
bool g_LMBDLast = false;
int Font_1 = 1;
PlayerClass		player;
CoinsClass		coins;
SceneManager	manager;
MonsterClass_Bat	bat;
Skill_SwordKeeClass swordkee;
SkillManager *g_skillManager;
LPD3DXFONT font = NULL;
GUIBasicClass g_Menu;
CalGUI		  g_calGUI;
HUDBasicClass *g_HUD;

GAMEPROCESS g_gameprocess = GAME_READY;
void  GUICallBack(GUIControlTag controlTag, int state);

//------------------------------------------------------------
//注册函数,技能，怪物，玩家，道具注册。用于管理
//------------------------------------------------------------
void Register(RoleBasic* role) { Wor.Player.push_back(role); }
void Register(PropBasic* prop) {
	if (prop->tag == PLAYERPROP)Wor.PlayerProp.push_back(prop);
	else Wor.EnemyProp.push_back(prop);
}
void Register(coin* acoin) { Wor.Coins.push_back(acoin); }
void Register(MonsterBasic* m) { Wor.Enemy.push_back(m); }
void Register(SkillBasicClass* skill) { Wor.Skill.push_back(skill); }
void InitDigital();
struct SnowTexture
{
	float x, y, z;
	float u, v;
};
#define FVF_SNOWTEXTURE (D3DFVF_XYZ | D3DFVF_TEX1)
IDirect3DTexture9* tex;
//--------------------------------------------------------------------------------------
// Rejects any D3D9 devices that aren't acceptable to the app by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat,
                                      bool bWindowed, void* pUserContext )
{
    // Typically want to skip back buffer formats that don't support alpha blending
    IDirect3D9* pD3D = DXUTGetD3D9Object();
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                                         AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
                                         D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
        return false;

    return true;
}


//--------------------------------------------------------------------------------------
// Before a device is created, modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
    return true;
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
// and aren't tied to the back buffer size
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                     void* pUserContext )
{
	Device = DXUTGetD3D9Device();
	hwnd = DXUTGetHWND();
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
// or that are tied to the back buffer size 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                    void* pUserContext )
{
	HRESULT hr;
	g_soundmanager= new CSoundManager();
	g_soundmanager->Initialize(hwnd, DSSCL_PRIORITY);
	g_soundmanager->SetPrimaryBufferFormat(2, 22050, 16);
	g_soundmanager->Create(&wav1, L"Media//召唤循环.wav");
	g_LMBDown = false;
	camera = Camera(pd3dDevice, Width, Height);
	g_skillManager = new SkillManager;
	g_skillManager->Init(Device, g_soundmanager, Register);
	g_skillManager->LoadSource();

	//世界管理权限交接
	manager.SetVelocity(Game_Velocity);
	manager.Init(Device, &Wor, g_soundmanager, g_skillManager);
	manager.AddGameProcess(&g_gameprocess);


	//场景资源载入
	V(g_BackGround.Init(&Device,L"Media//场景分层.png",Width/10,Height/10,5,1.1f));
	g_fond.Init(&Device);
	//摄像机位置初始化
	camera.SetCameraPosition(&D3DXVECTOR3(50.50f, 26.50f, -66.0f));
	camera.SetTargetPosition(&D3DXVECTOR3(50.50f, 26.50f, 0.0f));
	camera.SetViewMatrix(0);
	camera.SetProjMatrix(0);
	Light_Set(Device, 1);
	V(coins.Init(Device,L"Media//金银币.png",2,8,10,5,5,-Game_Velocity,Register));
	V(player.Init(Device, L"Media//QG猫1.png", 2, 3, 15, 13, 13, -1.0f, Register));
	V(bat.Init(Device, L"Media//蝙蝠.png", 2, 2,  8, 8, -1.0f, Register))
	map.Init(&Device,L"Media//road.png",15,3, -2.5f,200, Game_Velocity);

	swordkee.Init(Device, L"Media//剑气1.png",1,1,1,7,7,-1.0f,25,5,Register);
	manager.Register(&map);
	manager.Register(&camera);
	g_Menu.Init(Device, 0, GUI_MAIN, Width, Height, GUICallBack);
	g_Menu.LoadSourceMenu();
	RECT rec;


	//-----------------------CalGUI----------------------------
	g_calGUI.LoadSource(Device, 0, Width, Height, GUICallBack);
	manager.AddCalGUI(&g_calGUI);

	//-----------------HUD-------------------------------------------
	g_HUD = new HUDBasicClass;
	manager.AddHUD(g_HUD);
	g_HUD->Init(Device);
	
	//过滤方式
	Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{
	// 	D3DXMATRIX matProject;
	// 	// 这个函数是设置正交投影矩阵
	// 	D3DXMatrixOrthoLH(&matProject, Width,Height, 0,1000);
	// 	Device->SetTransform(D3DTS_PROJECTION, &matProject);

	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	D3DXMATRIX matview;
	camera.CalculateViewMatrix(&matview);
	Device->SetTransform(D3DTS_VIEW, &matview);
	static float timers[6] = { 2.0f,2.0f,2.0f,2.0f,2.0f,0.0f };
	//为了避免用户按键过快，设置一个间隔时间
	for (int i = 0; i<5; i++)
		timers[i] += fElapsedTime;
	
	int i = 0;
	if (::GetAsyncKeyState('2') & 0x8000f)
		if (timers[1] > 1.9)
		{
			bat.Create();
			timers[1] = 1.0f;
		}

	if (::GetAsyncKeyState('3') & 0x8000f)
		if (timers[2] > 1.9)
		{
			coins.CreateCoins(1);
			timers[2] = 1.0f;
			i++;
		}
	static bool fb=false;
	if (::GetAsyncKeyState('4') & 0x8000f)
	{
		if (timers[3] > 1.9)
		{
			coins.CreateCoins(2);
			timers[3] = 1.0f;
			fb = true;
		}
	}
	if (fb)
	{
		timers[5] += fElapsedTime;
		if (timers[5] > 5.0f)
		{
			timers[5] = 0;
			coins.CreateCoins(3);
			fb = false;
		}
	}

	if (::GetAsyncKeyState('5') & 0x8000f)
	{
		if (timers[0] > 1.9)
		{
			DXUTSetConstantFrameTime(true,0);
			timers[0] = 1.0f;
			
		}
	}

	if (::GetAsyncKeyState('6') & 0x8000f)
	{
		if (timers[4] > 1.9)
		{
			DXUTSetConstantFrameTime(false);
			timers[4] = 1.0f;

		}
	}
	
	//管理者状态更新，清除非Live状态的物体，进行碰撞检测	
	if (g_gameprocess == GAME_PLAYING || g_gameprocess == GAME_OVER)
	{
		coins.Update(fElapsedTime);
		manager.Check();
		manager.HitTest();
		float y = Scr_HIGHT_EDGE - g_MouseY / 11;
		float x = g_MouseX / 11;
		manager.DisposInput(fElapsedTime, x,y, 0);

		player.Update(fElapsedTime);
		swordkee.Update(fElapsedTime);//剑气技能更新
		g_skillManager->Update(fElapsedTime);//技能管理更新

		bat.Update(fElapsedTime,player._position);//蝙蝠小怪集合更新
		map.UpdateMap(fElapsedTime);			//地图更新
		g_BackGround.Update(fElapsedTime*0.05);	//场景更新
		manager.Update(fElapsedTime);			//管理者更新
		//结算界面只有在游戏结束时才更新
		if(g_gameprocess == GAME_OVER)
			g_calGUI.Update(fElapsedTime);
	}
	
}


//--------------------------------------------------------------------------------------
// Render the scene using the D3D9 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    HRESULT hr;
	bool buttonDownToUp = false;
	if (g_LMBDLast == true && g_LMBDown == false)
		buttonDownToUp = true;
    // Clear the render target and the zbuffer 
    V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 45, 50, 170 ), 1.0f, 0 ) );
	Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    // Render the scene
	D3DXVECTOR3 cpos;
	cpos = camera.v_CameraPosition;
    if( SUCCEEDED( pd3dDevice->BeginScene() ) )
    {
		Device->SetMaterial(&d3d::WHITE_MTRL);

		D3DXMATRIX world;
		D3DXMatrixTranslation(&world, 0, 0, 0);
		Device->SetTransform(D3DTS_WORLD, &world);
		if (g_gameprocess == GAME_PLAYING || g_gameprocess == GAME_OVER)
		{
			g_BackGround.Render();
			Device->SetTransform(D3DTS_WORLD, &world);
			map.Render();
			player.Render();
			coins.Render(fElapsedTime);
			swordkee.Render();
			g_skillManager->Render();
			bat.Render();

			g_HUD->Render();
		}
		
	//	g_gameprocess = GAME_PLAYING;
		if(g_gameprocess == GAME_READY)
			g_Menu.Render(buttonDownToUp, g_MouseX, g_MouseY);
		if(g_gameprocess == GAME_OVER)
			g_calGUI.Render(buttonDownToUp, g_MouseX, g_MouseY);
// 		g_fond.draw(DXUTGetFPS(), 5,5);
// 		g_fond.draw(L"Mx:",5,30);
// 		g_fond.draw((float)g_MouseX,35,30);
// 		g_fond.draw(L"My:", 5, 55);
// 		g_fond.draw((float)g_MouseY, 35, 55);
// 		g_fond.draw((float)manager.GetScore(), 5,25);
// 		g_fond.draw(L"lx:",5, 45);
// 		g_fond.draw(map.GetBoxLx(), 45,45);
// 		g_fond.draw(L"rx:",125, 45);
// 		g_fond.draw(map.GetBoxRx(),175,45);
// 		g_fond.draw(L"b_x:", 5, 85);
// 		g_fond.draw(bat.GetBOX(), 125, 85);
// 		g_fond.draw(L"action:", 5, 105);
// 		if(player.action == RUN)
// 			g_fond.draw(L"run", 85, 105);
// 		else if(player.action == FALL)
// 			g_fond.draw(L"fall", 85, 105);
// 		else if (player.action == JUMP)
// 			g_fond.draw(L"jump", 85, 105);
// 		g_fond.draw(L"B_y:", 5, 125);
// 		g_fond.draw(player.GetBoxLy(), 85, 125);
// 		g_fond.draw(cpos.x, 5, 175);
// 		g_fond.draw(cpos.y,100,175);
// 		g_fond.draw(cpos.z,200,175);
        V( pd3dDevice->EndScene() );
    }
	if (buttonDownToUp)
		g_LMBDLast = false;
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}


//--------------------------------------------------------------------------------------
// Handle messages to the application 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                          bool* pbNoFurtherProcessing, void* pUserContext )
{

	switch (uMsg)
	{
	case WM_PAINT:					 // 客户区重绘消息
		OnD3D9FrameRender(Device, 0.01, 0.01, 0);          //调用Direct3D_Render函数，进行画面的绘制
		ValidateRect(DXUTGetHWND(), NULL);
		break;
	case WM_LBUTTONDOWN:
		g_LMBDLast = g_LMBDown;
		g_LMBDown = true;
		break;

	case WM_LBUTTONUP:
		g_LMBDLast = g_LMBDown;
		g_LMBDown = false;
		break;

	case WM_MOUSEMOVE:
		g_MouseX = LOWORD(lParam);
		g_MouseY = HIWORD(lParam);
		break;
	}
    return 0;
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9ResetDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9LostDevice( void* pUserContext )
{

}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9DestroyDevice( void* pUserContext )
{
	SAFE_RELEASE(tex);
	SAFE_RELEASE(p_vertexbuffer);
	g_fond.Clear();
	g_BackGround.Clear();
	player.Clear();
	coins.Clear();
	map.Clear();
	bat.Clear();
	swordkee.Clear();
	g_Menu.Clear();
	g_calGUI.Clear();
	SAFE_DELETE(g_HUD);
}


//--------------------------------------------------------------------------------------
// Initialize everything and go into a render loop
//--------------------------------------------------------------------------------------
INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    // Set the callback functions
    DXUTSetCallbackD3D9DeviceAcceptable( IsD3D9DeviceAcceptable );
    DXUTSetCallbackD3D9DeviceCreated( OnD3D9CreateDevice );
    DXUTSetCallbackD3D9DeviceReset( OnD3D9ResetDevice );
    DXUTSetCallbackD3D9FrameRender( OnD3D9FrameRender );
    DXUTSetCallbackD3D9DeviceLost( OnD3D9LostDevice );
    DXUTSetCallbackD3D9DeviceDestroyed( OnD3D9DestroyDevice );
    DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackFrameMove( OnFrameMove );

    // TODO: Perform any application-level initialization here

    // Initialize DXUT and create the desired Win32 window and Direct3D device for the application
    DXUTInit( true, true ); // Parse the command line and show msgboxes
    DXUTSetHotkeyHandling( true, true, true );  // handle the default hotkeys
    DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
    DXUTCreateWindow( L"MyGame" );
    DXUTCreateDevice( true, Width,Height);

    // Start the render loop
    DXUTMainLoop();

    // TODO: Perform any application-level cleanup here

    return DXUTGetExitCode();
}

void  GUICallBack(GUIControlTag controlTag, int state)
{
	switch (controlTag)
	{
	case GCT_RESTAR:
	case GCT_STAR:
		if (state == MOUSE_DOWN)
		{
			g_gameprocess = GAME_PLAYING;
			InitDigital();
		}
		break;
	case GCT_GOTOMENU:
		if (state == MOUSE_DOWN)
			g_gameprocess = GAME_READY;
		break;
	case GCT_QUIT:
		if (state == MOUSE_DOWN)
		PostQuitMessage(0);		//向系统表明有个线程有终止请求。用来响应WM_DESTROY消息
		break;
	}
}

void InitDigital()
{	
	manager.InitDigital();
	g_calGUI.InitDigital();
	player.InitDigital();
	bat.InitDigital();
	coins.InitDigital();
	map.SetLevel(1); 
}