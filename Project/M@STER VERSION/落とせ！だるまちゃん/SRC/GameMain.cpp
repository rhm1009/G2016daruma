//======================================================================================//
#include      <windows.h>
#include      <stdio.h>
#include      <math.h>
#include      <time.h>
#include      "MASTER/Ci-Lib.H"
#include      "Game.H"

//****************************************************************************
//   ■   定義
//
//****************************************************************************
// 1280 720

#define         DARUMA_MAX         20  // 頭 = 1200 800
#define         DARUMA_IMAGE_X      150 // 1200 0.125f
#define         DARUMA_IMAGE_Y      100  // 800  0.125f

#define         HAM_IMAGE_SCALE      800 // 0.2f

#define         EFFECT_IMAGE_X      386 // "
#define         EFFECT_IMAGE_Y      418

#define         MOUSE_DISP


// だるまの色
#define         BLUE            0
#define         RED               800
#define         YELLOW            1600
#define         GREEN            2400
#define         HEAD            3200


// シーン状態
#define         STOP            0
#define         DARUMAFALLANI      1


// Ham State
#define         H_RIGHT            0
#define         H_LEFT            1

/// Daruma aninum
#define         D_NAME            0
#define         D_BUTTON         1
//

// Caution Animation
#define         C_ONANI            0
#define         C_ON            1
#define         C_OFFANI         2
#define         C_OFF            3
//

// Fade State
#define         FADE_START         1
#define         FADE_ON            2
#define         FADE_OUT         3
//

typedef struct DarumaState {
	short   num; // Daruma 顔の下から数える
	float   x, y;
	float   px, py; // 目標座標
	short   alp = 255;
	bool   state = false;
	bool   collstate = false;
};

typedef struct KemuriState {
	int alpha = 255;
	float x, y;
	float sx = 0.125f, sy = 0.125f;
	bool state = false;
};

GsSOUND      S_DARUMA;
GsSOUND      S_BGM;
GsSOUND      S_FallLogo;
GsSOUND      S_HAM;

GsTEXTURE   GameTexture;
GsTEXTURE   GameTexture2;
GsTEXTURE   D_TEXTURE;
GsTEXTURE   MENUTEX;

DarumaState   Daruma[20];
DarumaState M_Daruma[4];
KemuriState Kemuri[20];

short FadeStateNum;

int      Ani1ptn;        // タイトル始めパタン
float   Ani1timer = 0.0f;
bool   Ani1state = true; // Hamma 1 Animation ( Start )
bool   Ani2state = false; // Hamma 2 Animation ( Menu Select)

						  // Hammer
bool   H_State = false;
short   H_Dir;
short   H_AniTic = 0;
float   H_x, H_y;
float   H_Rot;

// Black Back Ground
float B_x, B_y;
short B_a;
float B_timer;
bool  B_state = false;
/////

// Caution Image
float C_x, C_sx, C_y, C_sy;
float C_timer;
float C_speed;
float C_Resistance = 10.0f;
short  C_state = C_OFF;
/////

//////// TEST
//float   H_anitimer = 0.0f;

//GsTEXTURE   T_Gage;
//float      T_sx = 1.0f;
//
//

// Effect
bool   E_State = false;
short   E_Alp = 0;
float   E_x, E_y;
//

// Logo  == Ani1ptn 2
bool   L_State = false;
float   L_x, L_y;

// Logo Animation
float deltaTime = -0.016666f;
float L_gravity = 2.0f;
float L_timer = 0.0f;
//

/// Vib
bool   V_State = false;
short   V_cnt = 0;
//

// Menu
short   aninum;
short   anicnt = 0;
float   M_timer = 0.0f;
bool   M_state = false;
//

float   Gravity = 5.0f;
float   g_Timer = 0.0f;
float   D_AniTimer = 0.0f;
int      Scene;

bool   d_state = true;
bool   d_GON = false;
float   d_Gravity = 1.0f;
float   d_timer = 0;
float   d_Speed = 1.0f;
float   d_move_x = 0.0f;


///// Hamma2 Animation
float H2_Move_x = 0.0f;
short H2_i = 0;

//

// System Out
bool SystemNum[4];
float Systemtimer;
//

//
//****************************************************************************
//   ■   内部使用　変数
//
//****************************************************************************

DarumaState DarumaCheck(DarumaState py);
void KemuriSetting();
void TitleAnimation();
int    DarumaNumRanSet(int ran);
void DarumaDownY(float move_y);
void DarumaDownX(float move_x, short num);
//void HamOn(short dir , float x, float y);
void Vib();
void RestartAni(void);
void CautionAni(void);
void LogoAni(void);

// TEST
void XBoxPadKeyNumDeb(void);
//

bool CollCheck(float x1, float y1, float sx1, float sy1, bool genten1,
	float x2, float y2, float sx2, float sy2, bool genten2);

void SystemCheck();

//--------------------------------------------------------------------------//
//   ●   初期化      ：シーンに移る時一回、変数の初期化
//
//--------------------------------------------------------------------------//


void   TitleInit(void)
{
	SystemNum[0] = false , SystemNum[1] = false , SystemNum[2] = false , SystemNum[3] = false;

	Ani1ptn = 0;
	Ani1timer = 0.0f;
	Ani1state = true;
	Ani2state = false;

	E_State = false;
	E_Alp = 0;
	L_State = false;
	V_State = false;
	V_cnt = 0;
	anicnt = 0;
	M_timer = 0.0f;
	M_state = false;

	Gravity = 5.0f;
	g_Timer = 0.0f;
	D_AniTimer = 0.0f;

	d_state = true;
	d_GON = false;
	d_Gravity = 1.0f;
	d_timer = 0;
	d_Speed = 1.0f;
	d_move_x = 0.0f;

	deltaTime = -0.016666f;
	L_gravity = 2.0f;
	L_timer = 0.0f;

	GamePublic.MultiPlay = false;


	for (int i = 0; i < 20; i++)
	{
		Kemuri[i].alpha = 255;
		Kemuri[i].sx = 0.125f, Kemuri[i].sy = 0.125f;
		Kemuri[i].state = false;
	}
	// Daruma Start Setting
	for (int i = 0; i < 5; i++)
	{
		Daruma[i].state = true;
		Daruma[i].collstate = false;
		Daruma[i].alp = 255;
		Daruma[i].x = 640;
		if (i >= 4)
			Daruma[i].y = -100 - (i * (DARUMA_IMAGE_Y + 800));
		else
			Daruma[i].y = -100 - (i * (DARUMA_IMAGE_Y + 400));

		if (i > 0)            Daruma[i].py = Daruma[i - 1].py - DARUMA_IMAGE_Y;
		else if (i == 0)         Daruma[i].py = 670;

		switch (i)
		{
		case 0:      Daruma[i].num = GREEN;   break;
		case 1:      Daruma[i].num = YELLOW;   break;
		case 2:      Daruma[i].num = RED;   break;
		case 3:      Daruma[i].num = BLUE;   break;
		case 4:      Daruma[i].num = HEAD;   break;
		default:   Daruma[i].num = DarumaNumRanSet((rand() % 4) + 1); break;
		}
	}

	Scene = DARUMAFALLANI;
	//


	//Hammer
	H_State = true, H_Dir = H_LEFT, E_State = true, E_Alp = 255;
	H_x = 320, H_y = 320;
	E_x = 320, E_y = 600;
	//


	// Fade
	Gs_FadeIn(RGB(0, 0, 0), 10);
	FadeStateNum = FADE_ON;
	//

	S_BGM = Gs_LoadWAVE("DATA/SOUND/M_BGM.WAV", ON);
	S_DARUMA = Gs_LoadWAVE("DATA/SOUND/Daruma.WAV", OFF);
	S_FallLogo = Gs_LoadWAVE("DATA/SOUND/FallDaruma.WAV", OFF);
	S_HAM = Gs_LoadWAVE("DATA/SOUND/Hammer.WAV", OFF);
	S_DARUMA = Gs_LoadWAVE("DATA/SOUND/Daruma.WAV", OFF);
	Gs_PlayBGM(S_BGM);
}

DarumaState DarumaCheck(DarumaState py)
{
	if (py.y >= py.py && py.collstate == false)
	{
		if (py.num == HEAD)
			g_Timer = 0.0f, Ani1timer = 1.0f, Ani1state = false;
		Gs_PlaySE(S_DARUMA);
		py.collstate = true;
		py.y = py.py;
		KemuriSetting();
		for (int i = 0; i < 20; i++)
		{
			if (Kemuri[i].state == false)
			{
				Kemuri[i].x = py.x - 85;
				Kemuri[i].y = py.y + 25;
				Kemuri[i].alpha = 255;
				Kemuri[i].sx = 0.125f;
				Kemuri[i].sy = 0.125f;
				Kemuri[i].state = true;
				break;
			}
		}
	}
	return py;
}

float Lerp(float p1, float p2, float d1) {
	return float(p1 + ((float)(p2 - p1) * d1));
}

bool SLerp(float p1, float p2) {
	return float(p1 + ((float)(p2 - p1) / 10.0f));
}

void DarumaDownY(float move_y)
{
	for (int i = 0; i < 20; i++)
	{
		if (Daruma[i].state == true)
		{
			Daruma[i].y += move_y;
			Daruma[i].py += move_y;
		}
	}
}

void DarumaDownX(float move_x, short num)
{
	for (int i = 0; i < 4; i++)
	{
		if (i == num)
			continue;
		Daruma[i].x += move_x;
		Daruma[i].px += move_x;
	}
	/*Daruma[0].x += move_x;
	Daruma[0].px += move_x;
	Daruma[1].x += move_x;
	Daruma[1].px += move_x;
	Daruma[2].x += move_x;
	Daruma[2].px += move_x;
	Daruma[3].x += move_x;
	Daruma[3].px += move_x;*/

	//
	L_x += move_x;
	//
}

int      DarumaNumRanSet(int ran)
{
	switch (ran)
	{
	case 1: return BLUE;
	case 2: return RED;
	case 3: return YELLOW;
	case 4: return GREEN;
	default: return BLUE;
	}
}

void KemuriSetting()
{
	for (int i = 0; i < 20; i++)
	{
		if (Kemuri[i].state == true && i > 0)
		{
			if (Kemuri[i - 1].state == false)
			{
				Kemuri[i - 1] = Kemuri[i];
				Kemuri[i].state = false, Kemuri[i - 1].state = true;
			}
		}
	}
}

void TitleAnimation()
{
	switch (Ani1ptn)
	{
	case 0:
		if (!Ani1state)
		{
			if (Ani1timer <= 0.0f)
			{
				Ani1timer = 0.0f;
				Ani1ptn++;
			}
			else if (Ani1timer > 0.0f)
			{
				Ani1timer -= 0.016666f;
			}
		}

		for (int i = 0; i < 20; i++)
		{
			if (Kemuri[i].state == true)
			{
				if (Kemuri[i].alpha <= 0)
				{
					Kemuri[i].alpha = 0;
					Kemuri[i].sx = 0.125f;
					Kemuri[i].sy = 0.125f;
					Kemuri[i].state = false;
					D_AniTimer = 0.5f;
				}

				else
				{
					Kemuri[i].alpha -= 10;
					Kemuri[i].sx += 0.001f;
					Kemuri[i].sy += 0.001f;
					Kemuri[i].y -= 0.5f;
					Kemuri[i].x -= 0.8f;

					if (Kemuri[i].alpha <= 0)
					{
						Kemuri[i].alpha = 0;
						Kemuri[i].sx = 0.125f;
						Kemuri[i].sy = 0.125f;
						Kemuri[i].state = false;
						D_AniTimer = 0.5f;
					}
				}

			}

			Daruma[i] = DarumaCheck(Daruma[i]);

			if (Daruma[i].state == true && Daruma[i].collstate == false)
			{
				g_Timer += 0.016666f;
				Daruma[i].y += 2.0f + (Gravity * g_Timer);
			}

			if (Daruma[i + 1].state == false)
				if (Daruma[i + 2].state == false)
					if (Daruma[i + 3].state == false)   break;
		}
		break;

	case 1:
		if (!H_State && !Ani1state)
		{
			H_State = true, Ani1state = true, H_Dir = H_LEFT;
			Gs_PlaySE(S_HAM);
		}

		if (Daruma[4].x < -150)
		{
			Daruma[4].x = -3000;
			L_y = -500;
			L_x = Daruma[0].x;
			Ani1ptn++;
			Ani1state = false;
			L_State = true;
		}
		else   Daruma[4].x -= 20.25f;

		break;

	case 2:
		if (!Ani1state)
		{
			if (L_y < Daruma[3].y - ((DARUMA_IMAGE_Y / 2) + (231 / 2)))
			{
				g_Timer += 0.016666f;
				L_y += 8.0f + (Gravity * g_Timer);
			}
		}

		if (L_y >= Daruma[3].y - ((DARUMA_IMAGE_Y / 2) + (231 / 2)) && L_State && !V_State)
		{
			Gs_PlaySE(S_FallLogo);
			L_State = false, V_State = true, Ani1state = true;
			L_y = Daruma[3].y - ((DARUMA_IMAGE_Y / 2) + (231 / 2));
		}

		else if (!L_State && V_State)
		{
			Vib();
		}

		else if (!L_State && !V_State)
		{
			Ani1state = false, Ani1ptn++;
			aninum = D_NAME;
			for (int i = 0; i < 4; i++)
			{
				M_Daruma[i].alp = 0;
				M_Daruma[i].num = Daruma[i].num;
				M_Daruma[i].x = Daruma[i].x;
				M_Daruma[i].y = Daruma[i].y;
			}
		}
		break;

	case 3:

		LogoAni();

		for (int i = 0; i < 4; i++)
		{
			switch (aninum)
			{
			case D_NAME:
				switch (anicnt)
				{
				case 0: // fade in
					if (M_Daruma[i].alp >= 255 && !M_state)
					{
						M_state = true;
						M_timer = 2.0f;
						M_Daruma[i].alp = 255;
					}

					else if (M_Daruma[i].alp >= 255 && M_state)
					{
						if (M_timer <= 0.0f)
						{
							M_timer = 0.0f;
							M_state = false;
							anicnt++;
						}

						else
							M_timer -= 0.016666f;
					}

					else
					{
						M_Daruma[i].alp += 5;
					}
					break;

				case 1: // fade out
					if (M_Daruma[i].alp <= 0)
					{
						M_Daruma[i].alp = 0;
						aninum = D_BUTTON;
						anicnt = 0;
					}

					else
					{
						M_Daruma[i].alp -= 5;
					}
					break;
				}
				break;

			case D_BUTTON:
				switch (anicnt)
				{
				case 0: // fade in
					if (M_Daruma[i].alp >= 255 && !M_state)
					{
						M_state = true;
						M_timer = 2.0f;
						M_Daruma[i].alp = 255;
					}

					else if (M_Daruma[i].alp >= 255 && M_state)
					{
						if (M_timer <= 0.0f)
						{
							M_timer = 0.0f;
							M_state = false;
							anicnt++;
						}

						else
							M_timer -= 0.016666f;
					}

					else
					{
						M_Daruma[i].alp += 5;
					}
					break;

				case 1: // fade out
					if (M_Daruma[i].alp <= 0)
					{
						M_Daruma[i].alp = 0;
						aninum = D_NAME;
						anicnt = 0;
					}

					else
					{
						M_Daruma[i].alp -= 5;
					}
					break;
				}
				break;
			}
		}

		/*case 0:      Daruma[i].num = GREEN;   break;
		case 1:      Daruma[i].num = YELLOW;   break;
		case 2:      Daruma[i].num = RED;   break;
		case 3:      Daruma[i].num = BLUE;   break;
		case 4:      Daruma[i].num = HEAD;   break;*/

		if (Gs_GetPADtrg(PAD_1P, kPAD_C) || Gs_GetKEYtrg(kKEY_X) && C_state == C_OFF || (CollCheck(Daruma[3].x, Daruma[3].y, 150, 100, ON, Mouse.Px, Mouse.Py, 1, 1, ON) && C_state == C_OFF && Mouse.Trg & _lMOUSE)) // Game Start
		{
			Ani1ptn = 10, H2_i = 3;/*C_state = C_ONANI;*/
		}

		if (Gs_GetPADtrg(PAD_1P, kPAD_B) || Gs_GetKEYtrg(kKEY_B) && C_state == C_OFF || (CollCheck(Daruma[2].x, Daruma[2].y, 150, 100, ON, Mouse.Px, Mouse.Py, 1, 1, ON) && C_state == C_OFF && Mouse.Trg & _lMOUSE)) // Option
		{
			Ani1ptn = 10, H2_i = 2;/*C_state = C_ONANI;*/
		}

		if (Gs_GetPADtrg(PAD_1P, kPAD_X) || Gs_GetKEYtrg(kKEY_Y) && C_state == C_OFF || (CollCheck(Daruma[1].x, Daruma[1].y, 150, 100, ON, Mouse.Px, Mouse.Py, 1, 1, ON) && C_state == C_OFF && Mouse.Trg & _lMOUSE)) // Credits
		{
			Ani1ptn = 10, H2_i = 1;/*C_state = C_ONANI;*/
		}

		if (Gs_GetPADtrg(PAD_1P, kPAD_A) || Gs_GetKEYtrg(kKEY_A) && C_state == C_OFF || (CollCheck(Daruma[0].x, Daruma[0].y, 150, 100, ON, Mouse.Px, Mouse.Py, 1, 1, ON) && C_state == C_OFF && Mouse.Trg & _lMOUSE)) // Quit
		{
			Ani1ptn = 10, H2_i = 0;/*C_state = C_ONANI;*/
		}


		/*if ((Gs_GetPADtrg(PAD_1P, kPAD_B) || Gs_GetKEYtrg(kKEY_B)) && C_state == C_ON) // Option
			C_state = C_OFFANI;*/

		break;


	case 10: // Hamma2 Animation

		if (!H_State && !Ani2state)
		{
			Gs_PlaySE(S_HAM);
			H2_Move_x = 1500.0f;
			H_State = true, Ani2state = true;
			E_Alp = 255;
			H_AniTic = 0;
			switch (H2_i)
			{
			case 3:
				H_Dir = H_LEFT;
				break;

			case 2:
				H_Dir = H_RIGHT;
				break;

			case 1:
				H_Dir = H_LEFT;
				break;

			case 0:
				H_Dir = H_RIGHT;
				break;
			}
		}

		switch (H_Dir)
		{
		case H_LEFT:
			if (Daruma[H2_i].x < -500)
			{
				Daruma[H2_i].x = -4500;
				if (H2_i == 3)
				{
					Gs_FadeOut(RGB(0x00, 0x00, 0x00), 5);
					FadeStateNum = FADE_OUT;
				}
				else
				{
					Ani2state = false;
					C_state = C_ONANI;
					Ani1ptn = 11;
				}
			}
			else
			{
				if (H2_Move_x <= 0)
				{
					H2_Move_x = 0;
					Daruma[H2_i].x -= 24.5f;
				}
				else
				{
					DarumaDownX(24.5f, H2_i);
					H2_Move_x -= 24.5f;
				}

			}
			break;

		case H_RIGHT:
			if (Daruma[H2_i].x > 1500)
			{
				Daruma[H2_i].x = 4500;

				if (H2_i == 3)
				{
					Gs_FadeOut(RGB(0x00, 0x00, 0x00), 5);
					FadeStateNum = FADE_OUT;
				}

				else
				{
					Ani2state = false;
					C_state = C_ONANI;
					Ani1ptn = 11;
				}
			}
			else
			{
				if (H2_Move_x <= 0)
				{
					H2_Move_x = 0;
					Daruma[H2_i].x += 24.5f;
				}
				else
				{
					DarumaDownX(-24.5f, H2_i);
					H2_Move_x -= 24.5f;
				}

			}
			break;
		}


		/*if(H_State)
		DEB_TEXT("H_State is true");

		DEB_TEXT("E_Alp is %d", E_Alp);
		DEB_TEXT("H_AniTic is %d", H_AniTic);
		DEB_TEXT("H2_Move_x = %f", H2_Move_x);*/
		break;

	case 11:
		if (((Gs_GetPADtrg(PAD_1P, kPAD_B) || Gs_GetKEYtrg(kKEY_B)) && C_state == C_ON ) || (Mouse.Trg & _lMOUSE && C_state == C_ON)) // Option
			C_state = C_OFFANI;
		if (C_state == C_OFF)
			RestartAni();
		break;
	}
}

void Vib()
{
	switch (V_cnt)
	{
	case 0:
		for (int i = 0; i < 4; i++)
		{
			Daruma[i].x -= 20;
		}
		V_cnt++;
		break;

	case 1:
		for (int i = 0; i < 4; i++)
		{
			Daruma[i].x += 40;
		}
		V_cnt++;
		break;

	case 2:
		for (int i = 0; i < 4; i++)
		{
			Daruma[i].x -= 30;
		}
		V_cnt++;
		break;

	case 3:
		for (int i = 0; i < 4; i++)
		{
			Daruma[i].x += 20;
		}
		V_cnt++;
		break;

	case 4:
		for (int i = 0; i < 4; i++)
		{
			Daruma[i].x -= 10;
		}
		V_cnt++;
		break;

	case 5:
		for (int i = 0; i < 4; i++)
		{
			Daruma[i].x += 10;
		}
		V_cnt++;
		break;

	case 6:
		for (int i = 0; i < 4; i++)
		{
			Daruma[i].x -= 5;
		}
		V_cnt = 0;
		V_State = false;
		break;
	}
}

void XBoxPadKeyNumDeb(void)
{
	if (Gs_GetPAD(PAD_1P, kPAD_A))
		DEB_TEXT("This Key Is [A] Button");
	if (Gs_GetPAD(PAD_1P, kPAD_B))
		DEB_TEXT("This Key Is [B] Button");
	if (Gs_GetPAD(PAD_1P, kPAD_C))
		DEB_TEXT("This Key Is [C] Button");
	if (Gs_GetPAD(PAD_1P, kPAD_X))
		DEB_TEXT("This Key Is [X] Button");
	if (Gs_GetPAD(PAD_1P, kPAD_Y))
		DEB_TEXT("This Key Is [Y] Button");
	if (Gs_GetPAD(PAD_1P, kPAD_Z))
		DEB_TEXT("This Key Is [Z] Button");
}

void SystemCheck()
{
	if (Systemtimer > 0)
	{
		Systemtimer -= 0.016666f;

		if (Systemtimer <= 0.0f)
		{
			Systemtimer = 0.0f;
			SystemNum[0] = false, SystemNum[1] = false , SystemNum[2] = false, SystemNum[3] = false;
		}
	}

	if ((CollCheck(0, 0, 50, 50, OFF, Mouse.Px, Mouse.Py, 1, 1, ON) && Mouse.Trg & _lMOUSE))
	{
		SystemNum[0] = true;
		Systemtimer = 1.0f;
	}

	if ((CollCheck(1230, 0, 50, 50, OFF, Mouse.Px, Mouse.Py, 1, 1, ON) && Mouse.Trg & _lMOUSE))
	{
		SystemNum[1] = true;
		Systemtimer = 1.0f;
	}

	if ((CollCheck(0, 680, 50, 50, OFF, Mouse.Px, Mouse.Py, 1, 1, ON) && Mouse.Trg & _lMOUSE))
	{
		SystemNum[2] = true;
		Systemtimer = 1.0f;
	}

	if ((CollCheck(1230, 680, 50, 50, OFF, Mouse.Px, Mouse.Py, 1, 1, ON) && Mouse.Trg & _lMOUSE))
	{
		SystemNum[3] = true;
		Systemtimer = 1.0f;
	}

	if (SystemNum[0] && SystemNum[1] && SystemNum[2] && SystemNum[3])
	{
		SceneExit(ON);
	}
}

void RestartAni(void)
{
	SystemNum[0] = false, SystemNum[1] = false, SystemNum[2] = false, SystemNum[3] = false;

	Ani1ptn = 0;
	Ani1timer = 0.0f;
	Ani1state = true;

	E_State = false;
	E_Alp = 0;
	L_State = false;
	V_State = false;
	V_cnt = 0;
	anicnt = 0;
	M_timer = 0.0f;
	M_state = false;

	Gravity = 5.0f;
	g_Timer = 0.0f;
	D_AniTimer = 0.0f;

	d_state = true;
	d_GON = false;
	d_Gravity = 1.0f;
	d_timer = 0;
	d_Speed = 1.0f;
	d_move_x = 0.0f;

	deltaTime = -0.016666f;
	L_gravity = 2.0f;
	L_timer = 0.0f;

	GamePublic.MultiPlay = false;

	for (int i = 0; i < 20; i++)
	{
		Kemuri[i].alpha = 255;
		Kemuri[i].sx = 0.125f, Kemuri[i].sy = 0.125f;
		Kemuri[i].state = false;
	}
	// Daruma Start Setting
	for (int i = 0; i < 5; i++)
	{
		Daruma[i].state = true;
		Daruma[i].collstate = false;
		Daruma[i].alp = 255;
		Daruma[i].x = 640;
		if (i >= 4)
			Daruma[i].y = -100 - (i * (DARUMA_IMAGE_Y + 800));
		else
			Daruma[i].y = -100 - (i * (DARUMA_IMAGE_Y + 400));

		if (i > 0)            Daruma[i].py = Daruma[i - 1].py - DARUMA_IMAGE_Y;
		else if (i == 0)         Daruma[i].py = 670;

		switch (i)
		{
		case 0:      Daruma[i].num = GREEN;   break;
		case 1:      Daruma[i].num = YELLOW;   break;
		case 2:      Daruma[i].num = RED;   break;
		case 3:      Daruma[i].num = BLUE;   break;
		case 4:      Daruma[i].num = HEAD;   break;
		default:   Daruma[i].num = DarumaNumRanSet((rand() % 4) + 1); break;
		}
	}

	Scene = DARUMAFALLANI;
	//


	//Hammer
	H_State = true, H_Dir = H_LEFT, E_State = true, E_Alp = 255;
	H_x = 320, H_y = 320;
	E_x = 320, E_y = 600;
	//
}

void CautionAni(void)
{
	switch (C_state)
	{
	case C_ONANI:
		if (!B_state)
		{
			B_state = true;
			B_x = 0; B_y = 0, B_a = 0;

			C_x = 1675;
			C_sx = 640;
			C_y = 0;
			C_y = 360;
		}
		else
		{
			if (B_a < 255)
			{
				B_a += 5;
				if (B_a >= 255)
					B_a = 255;
			}
			C_timer += 0.016666f;
			if (C_x <= C_sx + 1.0f)
			{
				C_x = C_sx;
				C_sx = -400;
				C_state = C_ON;
			}
			else
				C_x = Lerp(C_x, C_sx, 0.1f);

			//C_x -= (C_speed * C_timer) - C_Resistance;
		}
		break;

		/*case C_ON:

		break;*/

	case C_OFFANI:
		if (B_a > 0)
		{
			B_a -= 5;
			if (B_a <= 0)
				B_a = 0;
		}
		//C_timer += 0.016666f;
		if (C_x <= C_sx + 1.0f)
		{
			C_x = C_sx;
			C_state = C_OFF;
			B_state = false;
		}
		else
			C_x = Lerp(C_x, C_sx, 0.1f);
		break;
	}

	/*case C_OFF:
	C_state = false;
	B_state = false;
	B_x = 0; B_y = 0, B_a = 0;

	C_x = 1675;
	C_sx = 1034;
	C_y = 0;
	C_y = 360;
	break;
	}*/
}

bool CollCheck(float x1, float y1, float sx1, float sy1, bool genten1,
	float x2, float y2, float sx2, float sy2, bool genten2)
{
	if (genten1)
	{
		x1 -= (sx1 / 2);
		y1 -= (sy1 / 2);
	}
	if (genten2)
	{
		x2 -= (sx2 / 2);
		y2 -= (sy2 / 2);
	}

	if (((x1 <= x2 + sx2) && ((x1 + sx1) >= x2)) && ((y1 <= y2 + sy2) && (y1 + sy1) >= y2))
		return true;
	else return false;
}


void LogoAni(void)
{
	L_timer += deltaTime;

	if (L_timer > 0.5f || L_timer < -0.5f)
		deltaTime = -deltaTime;

	L_y += L_gravity * L_timer;
}

//--------------------------------------------------------------------------//
//   ●   内部処理   ：シーン中の処理、現在のシーンで行う処理
//
//--------------------------------------------------------------------------//

void   TitleLoop(void)
{
	if (FadeStateNum == FADE_ON)
	{
		TitleAnimation();

		if (Gs_GetKEYtrg(kKEY_F1))
			RestartAni();

		CautionAni();
	}

	else
	{
		switch (FadeStateNum)
		{
		case FADE_START:
			if (Gs_FadeCheck() == false)
			{
				FadeStateNum = FADE_ON;
			}
			break;

		case FADE_OUT:
			if (Gs_FadeCheck() == false)
			{
				FadeStateNum = FADE_OUT;
				SceneChange(GAME_SCENE);
			}
			break;
		}
	}


	//System setting
	SystemCheck();
	//
}


//--------------------------------------------------------------------------//
//   ●   描画      ：シーン中の描画
//
//--------------------------------------------------------------------------//


void   TitleDraw(void)
{

	/// BG
	Gs_DrawLayer(0, 0, MENUTEX, 0, 0, 1280, 720, OFF, ARGB(255, 255, 255, 255),
		OFF, 0.0f, 1.0f/*0.993f*/, 1.0f/*0.986f*/);
	//

	for (int i = 0; i < 20; i++)
	{
		if (Daruma[i].state == true)
			DEB_TEXT("Daruma[%d] is true", i);
		else if (Daruma[i + 1].state == false)
			break;
	}

	for (int i = 0; i < 20; i++)
	{
		if (Ani1ptn == 1 && Daruma[i].num == HEAD)
		{
			Gs_DrawLayer(Daruma[i].x, Daruma[i].y, D_TEXTURE, 2400, Daruma[i].num, 1200, 800, OFF, ARGB(255, 255, 255, 255),
				ON, 0.0f, 0.125f, 0.125f);
		}
		else if (Daruma[i].state == true)
		{
			Gs_DrawLayer(Daruma[i].x, Daruma[i].y, D_TEXTURE, 0, Daruma[i].num, 1200, 800, OFF, ARGB(255, 255, 255, 255),
				ON, 0.0f, 0.125f, 0.125f);
			//Gs_DrawBox(Daruma[i].x-75, Daruma[i].y-50, Daruma[i].x + 75, Daruma[i].y + 50, BETA_COLOR, OFF);
		}
		else if (Daruma[i + 1].state == false)
			break;
	}
	/*Gs_DrawBox(0, 0, 50, 50, BETA_COLOR, OFF);
	Gs_DrawBox(1230, 0, 1280, 50, BETA_COLOR, OFF);
	Gs_DrawBox(0, 680, 50, 720, BETA_COLOR, OFF);
	Gs_DrawBox(1230, 680, 1280, 720, BETA_COLOR, OFF);*/

	for (int i = 0; i < 20; i++)
	{
		if (Kemuri[i].state == true)
			Gs_DrawLayer(Kemuri[i].x, Kemuri[i].y, GameTexture, 0, 2400, 1500, 400, OFF, ARGB(Kemuri[i].alpha, 255, 255, 255),
				OFF, 0.0f, Kemuri[i].sx, Kemuri[i].sy);
		else if (Kemuri[i + 1].state == false)
			break;
	}

	////////// Hammer

	// 120 , 120
	if (H_State)
	{
		if (H_Dir == H_RIGHT)
		{
			if (Ani1ptn == 10)
			{
				Gs_DrawLayer(Daruma[H2_i].x - ((DARUMA_IMAGE_X / 2) + ((HAM_IMAGE_SCALE * 0.2f) / 2) + 50), Daruma[H2_i].y, GameTexture2, 0, 3254, HAM_IMAGE_SCALE, HAM_IMAGE_SCALE,
					OFF, ARGB(255, 255, 255, 255), ON, H_Rot, 0.2f, 0.2f);
				Gs_DrawLayer(Daruma[H2_i].x - ((DARUMA_IMAGE_X / 2) + ((EFFECT_IMAGE_X * 0.2f) / 2)), Daruma[H2_i].y, GameTexture2, 580, 2674, EFFECT_IMAGE_X, EFFECT_IMAGE_Y, OFF,
					ARGB(E_Alp -= 15, 255, 255, 255), ON, 0, 0.2f, 0.2f);
			}

			else
			{
				Gs_DrawLayer(Daruma[4].x - ((DARUMA_IMAGE_X / 2) + ((HAM_IMAGE_SCALE * 0.2f) / 2) + 50), Daruma[4].y, GameTexture2, 0, 3254, HAM_IMAGE_SCALE, HAM_IMAGE_SCALE,
					OFF, ARGB(255, 255, 255, 255), ON, H_Rot, 0.2f, 0.2f);
				Gs_DrawLayer(Daruma[4].x - ((DARUMA_IMAGE_X / 2) + ((EFFECT_IMAGE_X * 0.2f) / 2)), Daruma[4].y, GameTexture2, 580, 2674, EFFECT_IMAGE_X, EFFECT_IMAGE_Y, OFF,
					ARGB(E_Alp -= 15, 255, 255, 255), ON, 0, 0.2f, 0.2f);
			}

			if (H_AniTic < 5)
			{
				if (H_AniTic == 0)
				{
					H_Rot += 15;
					H_AniTic++;
				}
				else if (H_AniTic == 1)
				{
					H_Rot += 8;
					H_AniTic++;
				}
				else if (H_AniTic == 2)
				{
					H_Rot += 4;
					H_AniTic++;
				}
				else if (H_AniTic == 3)
				{
					H_Rot += 2;
					H_AniTic++;
				}
				else if (H_AniTic == 4)
				{
					H_Rot += 1;
					H_AniTic++;
					//H_anitimer = 1.0f;
				}
			}
			else if (E_Alp <= 0)
			{
				E_Alp = 0;
				H_Rot = 0.0f;
				H_AniTic = 0;
				H_State = false;
				/*if (H_anitimer <= 0.0f)
				{
				H_Rot = 0.0f;
				H_anitimer = 0;
				H_AniTic = 0;
				}
				else if (H_anitimer > 0.0f)
				{
				H_anitimer -= 0.016666f;
				}*/
			}
		}

		else if (H_Dir == H_LEFT)
		{
			if (Ani1ptn == 10)
			{
				Gs_DrawLayer(Daruma[H2_i].x + ((DARUMA_IMAGE_X / 2) + ((HAM_IMAGE_SCALE * 0.2f) / 2) + 50), Daruma[H2_i].y, GameTexture2, 0, 3254, HAM_IMAGE_SCALE, HAM_IMAGE_SCALE,
					OFF, ARGB(255, 255, 255, 255), ON, H_Rot, 0.2f, 0.2f);
				Gs_DrawLayer(Daruma[H2_i].x + ((DARUMA_IMAGE_X / 2) + ((EFFECT_IMAGE_X * 0.2f) / 2)), Daruma[H2_i].y, GameTexture2, 580, 2674, EFFECT_IMAGE_X, EFFECT_IMAGE_Y, OFF,
					ARGB(E_Alp -= 15, 255, 255, 255), ON, 180, 0.2f, 0.2f);
			}
			else
			{
				Gs_DrawLayer(Daruma[4].x + ((DARUMA_IMAGE_X / 2) + ((HAM_IMAGE_SCALE * 0.2f) / 2) + 50), Daruma[4].y, GameTexture2, 0, 3254, HAM_IMAGE_SCALE, HAM_IMAGE_SCALE,
					OFF, ARGB(255, 255, 255, 255), ON, H_Rot, 0.2f, 0.2f);
				Gs_DrawLayer(Daruma[4].x + ((DARUMA_IMAGE_X / 2) + ((EFFECT_IMAGE_X * 0.2f) / 2)), Daruma[4].y, GameTexture2, 580, 2674, EFFECT_IMAGE_X, EFFECT_IMAGE_Y, OFF,
					ARGB(E_Alp -= 15, 255, 255, 255), ON, 180, 0.2f, 0.2f);
			}
			if (H_AniTic < 5)
			{
				if (H_AniTic == 0)
				{
					H_Rot -= 15;
					H_AniTic++;
				}
				else if (H_AniTic == 1)
				{
					H_Rot -= 8;
					H_AniTic++;
				}
				else if (H_AniTic == 2)
				{
					H_Rot -= 4;
					H_AniTic++;
				}
				else if (H_AniTic == 3)
				{
					H_Rot -= 2;
					H_AniTic++;
				}
				else if (H_AniTic == 4)
				{
					H_Rot -= 1;
					H_AniTic++;
					//H_anitimer = 1.0f;
				}
			}
			else if (E_Alp <= 0)
			{
				E_Alp = 0;
				H_Rot = 0.0f;
				H_AniTic = 0;
				H_State = false;
			}
		}
	}
	//

	//Logo
	if (Ani1ptn == 2 || Ani1ptn == 3 || Ani1ptn == 10)
		Gs_DrawLayer(L_x, L_y, MENUTEX, 0, 720, 579, 231, OFF, ARGB(255, 255, 255, 255),
			ON, 0.0f, 1.0f, 1.0f);
	//

	if (Ani1ptn == 3)
	{
		for (int i = 0; i < 4; i++)
		{
			switch (aninum)
			{
			case D_NAME:
				Gs_DrawLayer(M_Daruma[i].x, M_Daruma[i].y, D_TEXTURE, 1200, M_Daruma[i].num, 1200, 800, OFF,
					ARGB(M_Daruma[i].alp, 255, 255, 255), ON, 0.0f, 0.125f, 0.125f);
				break;

			case D_BUTTON:
				Gs_DrawLayer(M_Daruma[i].x, M_Daruma[i].y, D_TEXTURE, 2400, M_Daruma[i].num, 1200, 800, OFF,
					ARGB(M_Daruma[i].alp, 255, 255, 255), ON, 0.0f, 0.125f, 0.125f);
				break;
			}

		}
	}

	if (C_state != C_OFF)
	{
		Gs_DrawLayer(B_x, B_y, MENUTEX, 768, 1328, 1280, 720, OFF, ARGB(B_a, 255, 255, 255),
			OFF, 0.0f, 1.0f, 1.0f);
		Gs_DrawLayer(C_x, C_y, MENUTEX, 0, 951, 788, 252, OFF, ARGB(255, 255, 255, 255),
			ON, 0.0f, 1.0f, 1.0f);
	}

	/*Gs_DrawLayer(Kemuri[0].x, Kemuri[0].y, GameTexture, 0, 2400, 1500, 400, OFF, ARGB(Kemuri[0].alpha, 255, 255, 255),
	OFF, 0.0f, Kemuri[0].sx, Kemuri[0].sy);*/
}

//--------------------------------------------------------------------------//
//   ●   ロード      ：シーンに入る時、そのシーンで必要なデータの読み込み
//
//--------------------------------------------------------------------------//

short   TitleLoad(void)
{
	GameTexture = Gs_LoadBMP("DATA/BMP/GameTexture.PNG");
	GameTexture2 = Gs_LoadBMP("DATA/BMP/Tex_Game.PNG");
	D_TEXTURE = Gs_LoadBMP("DATA/BMP/D_TEXTURE2.PNG");
	MENUTEX = Gs_LoadBMP("DATA/BMP/MENUTEX.PNG");
	return      GAME_SCENE;
}

//--------------------------------------------------------------------------//
//   ●   後処理      ：現在のシーンから抜ける時、データの破棄
//
//--------------------------------------------------------------------------//

void   TitleExit(void)
{
	Gs_ReleaseBMP(D_TEXTURE);
	Gs_ReleaseBMP(GameTexture);
	Gs_ReleaseBMP(GameTexture2);
	Gs_ReleaseBMP(MENUTEX);
	Gs_ReleaseSOUND(S_BGM);
	Gs_ReleaseSOUND(S_DARUMA);
	Gs_ReleaseSOUND(S_HAM);
	Gs_ReleaseSOUND(S_FallLogo);
}

//****************************************************************************
//   ★　シーンテーブル：各シーンで呼び出される関数の登録
//****************************************************************************

SCENE_TBL   TitleSceneTbl = {
	TitleLoad,
	TitleInit,
	TitleLoop,
	TitleDraw,
	TitleExit
};