//======================================================================================//
//
//							Game Programming
//
//======================================================================================//
#include		<windows.h>
#include		<stdio.h>
#include		<stdlib.h>
#include		<time.h>
#include		<math.h>
#include		"MASTER/Ci-Lib.H"
#include		"Game.H"

//****************************************************************************
//	Define
//
//****************************************************************************
// Window Size
#define WD_Size_W		1280
#define WD_Size_H		720
// block atlas
#define Blk_Size_W		1200
#define Blk_Size_H		400
// Highscore txt file
FILE* file_I;
FILE* file_O;

// Press Key -> This Function
void KeyPress_Read();
// Combo Func
void Combo_Read(int& n_, int mode_);
// Star Effect Func
void StarEft(int& n_, int mode_);
// Daruma's Think (Block) Func
void ThinkEft(bool color_, int& n_);

void SystemCheckGame(void);
bool GameCollCheck(float x1, float y1, float sx1, float sy1, bool genten1,
	float x2, float y2, float sx2, float sy2, bool genten2);

//****************************************************************************
//	Variable
//
//****************************************************************************




struct GameState {
	enum State { Start, Game, Stop, Over };
	State status;
};

struct Block {
	//Throw Value Block
	POINT bPosition;
	float bSpdX, bSpdY;
	float bSpdX_Down, bSpdY_Down;
};

struct Block_ {
	//Block Value
	int Rdm, Bonus;
};


struct Star {
	int sRotate;
	int sRotSpd;
	float sSpeed, sSpdD;
	int sSize;
	POINT sPosition;
};

struct ThkBubble {
	POINT thkP[3];
	float tSpdX, tSpdY;
	int tAlpha; // No Ani, Temp
};

GameState GmSt;

 
GsTEXTURE BG;
GsTEXTURE BG2_1;
GsTEXTURE BG2_2;
GsTEXTURE BG2_3;
GsTEXTURE TEX_Game;
GsTEXTURE TEX_Label;

GsSOUND SND_BGM1;
GsSOUND SND_Start;
GsSOUND SND_GameOver;
GsSOUND SND_Hit;
GsSOUND SND_Daruma_fall;

POINT Pos_DarumaHead[7];

//1P
Block Blk[10];		// Block's Array -> Throwing situ On / Value Change No
Block_ BlkVl[10];	// Block's Value Array -> Throwing situ On / Value Change Yes
Star StEft[10];		// Star Effect's Array
ThkBubble TB[4];	// Daruma's Think's Array (4color)

bool GameAni;			// Block Throw Animation
bool Reset;				// Block Hit (true) -> Setting -> OK (false)
bool BlkReset;			// ''
bool Fever;				// FeverMode On/Off
bool Think_Color[4];	// Daruma's Think Bubble On/Off

int BlkClr;			// Appear Block Color Value (Init=2(Blue,Red))
int iGmCnt;			// int Game Start Count
int iFvrCnt;		// int Fever Count
int iFvrImgY;		// Fever Image Font Y Position ( FEVER(rainbow) )
float fFntSize;		// Fever Image Font Size

int BlkHit;		// 0 = Null, 1 = Right, 2 = Left
int AniMove;	// 0 = Null, 1 = Right, 2 = Left

int iScore, iGClr, iRClr;	// Score, Health Gauge Color(R, G, 255)
float fLife, fFevGge;		// Health Gauge Value, FeverGauge Value
int iHit, iMiss, iMsCnt;	// if more continue hit or miss -> more gauge plus/minus
bool bMsCnt;	// miss count mode

int Ani_Tick_Ham;
int iBB_Alp, iEft_Alp, iHam_Rot; //BB = Black BG
float BG_Y_Pos1, BG_Y_Pos2;
float BG_Y_Pos1_, BG_Y_Pos2_;
bool Castle;
float timeSet;

int iDarumaHead;

float mty_half;

//
bool bScrUpt; // bool Score Update
int iHScore; // int High Score
float fRsltX1, fRsltX2; // float Result Position
float fRsltX1_Spd, fRsltX2_Spd; // float Result Position Speed
float fRsltX1_DS, fRsltX2_DS; // float Result Position Down Speed
float fRsltY, fRsltY_Spd, fRsltY_DS;
int iSceneCng, iSC_Cnt; // int Scene Change, Scene Change Count


						/// System Setting
bool GameSystemNum[4];
float GameSystemtimer;

// Initalize
void	GameInit(void)
{
	// undone mode
	// test, multi true
	GamePublic.MultiPlay = true;
	// Win&Obj size setting
	if (GamePublic.MultiPlay)	mty_half = 0.5f;
	else						mty_half = 1.0f;

	Gs_FadeIn(RGB(0, 0, 0), 10);

	srand((int)time(NULL));

	GmSt.status = GameState::Start;

	//Daruma's Head Position Initalizing in PNG Atlas File
	Pos_DarumaHead[0] = { 0, 0 };
	Pos_DarumaHead[1] = { 1200, 0 };
	Pos_DarumaHead[2] = { 1200, 800 };
	Pos_DarumaHead[3] = { 1200, 1600 };
	Pos_DarumaHead[4] = { 2400, 0 };
	Pos_DarumaHead[5] = { 2400, 800 };
	Pos_DarumaHead[6] = { 2400, 1600 };

	//---------------------------------------------//
	//Initalizing Star Effect and Block Value
	for (int i = 0; i < 9; i++) {
		BlkVl[i].Rdm = rand() % 2 + 2;
		StEft[i].sPosition.x = rand() % 1280;
		StEft[i].sPosition.y = WD_Size_H + 100;
		StEft[i].sRotate = 0;
		StEft[i].sRotSpd = rand() % 5 + 1;
		StEft[i].sSize = rand() % 8 + 1;
		StEft[i].sSpeed = rand() % 30 + 10;

	}
	for (int i = 0; i < 4; i++) {
		TB[i].tAlpha = 0; // No Ani, Temp
	}

	GameAni = false;
	Reset = false;
	BlkReset = false;

	BlkHit = 0;

	
	BlkClr = 2;
	iGmCnt = 0;
	iFvrCnt = 0;
	iFvrImgY = (WD_Size_H / 2) + 300;
	fFntSize = 1.0f;

	BG_Y_Pos1 = 0;
	BG_Y_Pos2 = 0;
	BG_Y_Pos1_ = 2048;
	BG_Y_Pos2_ = 3072;
	Castle = true;

	Blk[0].bPosition.x = WD_Size_W / 2;
	Blk[0].bSpdX = 50.0f;
	Blk[0].bSpdX_Down = 0.9f;
	iBB_Alp = 0;
	iEft_Alp = 255;
	iHam_Rot = 0;
	Ani_Tick_Ham = 0;
	timeSet = 0;

	iScore = 0;
	iGClr = 255;
	iRClr = 0;
	fLife = 20.0f;
	iHit = 1;
	iMiss = 1;
	iMsCnt = 0;
	bMsCnt = false;
	fFevGge = 280;

	iDarumaHead = 0;

	//-----------------------------------//

	bScrUpt = false;
	//fscanf(file_I, "%d", &iHScore);
	fRsltX1 = -300.0f, fRsltX2 = -300.0f;
	fRsltX1_Spd = 100.0f, fRsltX2_Spd = 100.0f;
	fRsltX1_DS = 0.85f, fRsltX2_DS = 0.85f;
	fRsltY = WD_Size_H + 300, fRsltY_Spd = 45.0f, fRsltY_DS = 0.9f;

	iSceneCng = 0;
	iSC_Cnt = 0;

	///System
	GameSystemNum[0] = false, GameSystemNum[1] = false, GameSystemNum[2] = false, GameSystemNum[3] = false;
	GameSystemtimer = 0.0f;
}

void	GameLoop(void)
{
	///System
	SystemCheckGame();

	//Background Loop
	if (BG_Y_Pos1 <= -664) {
		BG_Y_Pos1 = 1328;
	}
	if (BG_Y_Pos2 <= -1328) {
		BG_Y_Pos2 = 664;
	}
	//
	if (BG_Y_Pos1_ <= -824) {
		BG_Y_Pos1_ = 1224;
		Castle = false;
	}
	if (BG_Y_Pos2_ <= -824) {
		BG_Y_Pos2_ = 1224;
	}

	//Life Color Setting
	float F_Life = 20.0f / 3.0f;

	if (fLife > F_Life + F_Life)
	{
		iGClr = 255;
		iRClr = (255.0f / F_Life) * (F_Life - fLife);
	}
	else if (fLife > F_Life)
	{
		iRClr = 255;
		iGClr = 255 - (255.0f / F_Life) * (F_Life - fLife);
	}
	else
	{
		iGClr = 0;
	}

	if (bMsCnt) {
		if (iMsCnt >= 60) {
			iMsCnt = 0;
			bMsCnt = false;
		}
		iMsCnt++;
	}

	//Pause
	if ((Gs_GetPADtrg(PAD_1P, kPAD_R) || Gs_GetKEYtrg(kKEYSPACE)) && !Reset) {
		if (GmSt.status == GameState::Stop)
			GmSt.status = GameState::Game;
		else if (GmSt.status == GameState::Game)
			GmSt.status = GameState::Stop;
	}

	//Game State Check

	if (GmSt.status == GameState::Start) {
		//Start Screen
		iGmCnt++;
	}

	else if (GmSt.status == GameState::Stop) {
		//Pause Window
	}
	else if (GmSt.status == GameState::Over) {
		//Game Over Window
		iGmCnt++;
	}
	else if (GmSt.status == GameState::Game) {
		//  ----  Fever System
		// fever gauge full, fever mode off
		if (fFevGge >= 300 && !Fever) {
			iDarumaHead = 5; // daruma's fever face
			Fever = true;
			iFvrCnt = 0;
			fFntSize = 1.0f;
			iFvrImgY = WD_Size_H / 2 + 300;
		}
		// fever gauge = 0, FeverMode On
		if (fFevGge <= 0 && Fever) {
			Fever = false;
			iDarumaHead = 0;
		}
		if (Fever) {
			if (iBB_Alp < 150)
				iBB_Alp += 25;
			else
				iBB_Alp = 150;
			fFevGge -= 1;
			iFvrCnt++;
		}
		else {
			if (iBB_Alp > 0)
				iBB_Alp -= 25;
			fFevGge -= 0.1f;
		}

		if (fFevGge <= 0)
			fFevGge = 0;
		else if (fFevGge >= 300)
			fFevGge = 300;
		if (fLife > 0 && !Fever)
			fLife -= 0.1f;
		if (fLife > 20)
			fLife = 20;

		// life zero, game over
		if (fLife <= 0) {
			GmSt.status = GameState::Over;
			iDarumaHead = 4;
			Gs_PlaySE(SND_GameOver); // game over sound effect
			iGmCnt = 0;
			// score > High Score
			if (iScore >= iHScore) {
				// highscore file update
				bScrUpt = true;
				fprintf(file_O, "%d", iScore);
				fscanf(file_I, "%d", &iHScore);

				for (int i = 0; i < 5; i++) {
					StEft[i].sPosition.x = WD_Size_W / 2 + 100;
					StEft[i].sPosition.y = WD_Size_H / 2;
					StEft[i].sRotate = 0;
					StEft[i].sRotSpd = 3;
					StEft[i].sSize = 1;
					StEft[i].sSpeed = 1.0f;
					StEft[i].sSpdD = 1.09f;
				}
			}
		}
		//Block----------------------------------
		KeyPress_Read();


		if (AniMove == 1) {
			if (Blk[0].bPosition.x <= 370) {
				Blk[0].bSpdX *= Blk[0].bSpdX_Down;
			}
			Blk[0].bPosition.x -= Blk[0].bSpdX;
		}
		if (AniMove == 2) {
			if (Blk[0].bPosition.x >= 910) {
				Blk[0].bSpdX *= Blk[0].bSpdX_Down;
			}
			Blk[0].bPosition.x += Blk[0].bSpdX;
		}

		if (Reset) {
			if (Blk[0].bPosition.x <= 400 || Blk[0].bPosition.x >= 880) {
				if (BlkReset) {
					if (iScore >= 150) BlkClr = 3;
					if (iScore >= 300) BlkClr = 4;
					for (int i = 0; i < 8; i++) {
						BlkVl[i].Rdm = BlkVl[i + 1].Rdm;
						BlkVl[8].Rdm = rand() % BlkClr + 2; // % 2 = 2block, % 3 = 3block, % 4 = 4block
						BlkVl[8].Bonus = rand() % 50;
					}
					BlkReset = false;
				}
				BG_Y_Pos1--;
				BG_Y_Pos2--;
				BG_Y_Pos1_ -= 4;
				BG_Y_Pos2_ -= 4;
			}
			if (Blk[0].bPosition.x <= 100 || Blk[0].bPosition.x >= 1180) {
				Reset = false;
			}
		}
	}
}

//Draw in Game
void	GameDraw(void)
{
	//BG--------------------------------------------------------------------
	Gs_DrawLayer(WD_Size_W / 2, (WD_Size_H / 2 - 1024 / 2) + BG_Y_Pos1, BG2_1, 0, 0,
		1280, 1024, OFF, ARGB(255, 255, 255, 255), ON, 0, 1.0f, 1.0f);
	Gs_DrawLayer(WD_Size_W / 2, (WD_Size_H / 2 + 1024 / 2) + BG_Y_Pos2, BG2_1, 0, 0,
		1280, 1024, OFF, ARGB(255, 255, 255, 255), ON, 180, 1.0f, 1.0f);
	
	//Castle (No Loop)

	if (Castle) {
		Gs_DrawLayer(WD_Size_W / 2 - 400, BG_Y_Pos1_ - 512, BG2_2, 0, 0,
			512, 2048, OFF, ARGB(255, 255, 255, 255), ON, 0, 1.0f, 1.0f);
		Gs_DrawLayer(WD_Size_W / 2 + 400, BG_Y_Pos1_ - 512, BG2_2, 512, 0,
			512, 2048, OFF, ARGB(255, 255, 255, 255), ON, 0, 1.0f, 1.0f);
	}
	//Castle (Loop)
	if (!Castle) {
		Gs_DrawLayer(WD_Size_W / 2 - 400, BG_Y_Pos1_, BG2_3, 0, 0,
			512, 1024, OFF, ARGB(255, 255, 255, 255), ON, 0, 1.0f, 1.0f);
		Gs_DrawLayer(WD_Size_W / 2 + 400, BG_Y_Pos1_, BG2_3, 512, 0,
			512, 1024, OFF, ARGB(255, 255, 255, 255), ON, 0, 1.0f, 1.0f);
	}
	Gs_DrawLayer(WD_Size_W / 2 - 400, BG_Y_Pos2_, BG2_3, 0, 0,
		512, 1024, OFF, ARGB(255, 255, 255, 255), ON, 0, 1.0f, 1.0f);
	Gs_DrawLayer(WD_Size_W / 2 + 400, BG_Y_Pos2_, BG2_3, 512, 0,
		512, 1024, OFF, ARGB(255, 255, 255, 255), ON, 0, 1.0f, 1.0f);
	//Black Alpha BG (Tex -> 583, 3158), 96*96
	if (iBB_Alp != 0) {
		Gs_DrawLayer(WD_Size_W / 2, WD_Size_H / 2, BG2_1, 583, 3158,
			96, 96, OFF, ARGB(iBB_Alp, 0, 0, 0), ON, 0, 14.0f, 11.0f);
		for (int i = 0; i < 10; i++) { StarEft(i, 0); }
	}

	//Block-----------------------------------------------------------------

	//Block Initalize (First Block)
	if (!GameAni) {
		Gs_DrawLayer(WD_Size_W / 2, (WD_Size_H / 2 - 100) + (0 * 60), TEX_Game, 0, BlkVl[0].Rdm * (Blk_Size_H),
			1200, 400, OFF, ARGB(255, 255, 255, 255), ON, 0, 0.15f, 0.15f);
	}
	//Block Initalize
	for (int i = 1; i < 9; i++) {
		Gs_DrawLayer(WD_Size_W / 2, (WD_Size_H / 2 - 100) + (i * 60), TEX_Game, 0, BlkVl[i].Rdm * (Blk_Size_H),
			1200, 400, OFF, ARGB(255, 255, 255, 255), ON, 0, 0.15f, 0.15f);
	}

	//Animation Start
	if (GameAni) {
		if (BlkHit == 1) {
			Gs_DrawLayer(WD_Size_W / 2 + 115, (WD_Size_H / 2 - 100), TEX_Game, 580, 2674,
				386, 418, OFF, ARGB(iEft_Alp -= 15, 255, 255, 255), ON, 180, 0.15f, 0.15f);
			Gs_DrawLayer(WD_Size_W / 2 + 200, (WD_Size_H / 2 - 100), TEX_Game, 0, 3254,
				800, 800, OFF, ARGB(255, 255, 255, 255), ON, iHam_Rot, 0.15f, 0.15f);
			if (Ani_Tick_Ham < 5)
			{
				if (Ani_Tick_Ham == 0) {
					iHam_Rot -= 15;
					Ani_Tick_Ham++;
				}
				else if (Ani_Tick_Ham == 1) {
					iHam_Rot -= 8;
					Ani_Tick_Ham++;
				}
				else if (Ani_Tick_Ham == 2) {
					iHam_Rot -= 4;
					Ani_Tick_Ham++;
				}
				else if (Ani_Tick_Ham == 3) {
					iHam_Rot -= 2;
					Ani_Tick_Ham++;
				}
				else if (Ani_Tick_Ham == 4) {
					iHam_Rot -= 1;
					Ani_Tick_Ham++;
				}
			}
			if (iEft_Alp <= 0) {
				BlkHit = 0;
				iEft_Alp = 255;
				iHam_Rot = 0;
				Ani_Tick_Ham = 0;
			}
		}
		else if (BlkHit == 2) {
			Gs_DrawLayer(WD_Size_W / 2 - 115, (WD_Size_H / 2 - 100), TEX_Game, 580, 2674,
				386, 418, OFF, ARGB(iEft_Alp -= 15, 255, 255, 255), ON, 0, 0.15f, 0.15f);
			Gs_DrawLayer(WD_Size_W / 2 - 200, (WD_Size_H / 2 - 100), TEX_Game, 0, 3254,
				800, 800, OFF, ARGB(255, 255, 255, 255), ON, iHam_Rot, 0.15f, 0.15f);
			if (Ani_Tick_Ham < 5)
			{
				if (Ani_Tick_Ham == 0) {
					iHam_Rot += 15;
					Ani_Tick_Ham++;
				}
				else if (Ani_Tick_Ham == 1) {
					iHam_Rot += 8;
					Ani_Tick_Ham++;
				}
				else if (Ani_Tick_Ham == 2) {
					iHam_Rot += 4;
					Ani_Tick_Ham++;
				}
				else if (Ani_Tick_Ham == 3) {
					iHam_Rot += 2;
					Ani_Tick_Ham++;
				}
				else if (Ani_Tick_Ham == 4) {
					iHam_Rot += 1;
					Ani_Tick_Ham++;
				}
			}
			if (iEft_Alp <= 0) {
				BlkHit = 0;
				iEft_Alp = 255;
				iHam_Rot = 0;
				Ani_Tick_Ham = 0;
			}
		}
		if (Blk[0].bPosition.x > -50 || Blk[0].bPosition.x < 1330) {
			Gs_DrawLayer(Blk[0].bPosition.x, (WD_Size_H / 2 - 100) + (0 * 60), TEX_Game, 0, BlkVl[9].Rdm * (Blk_Size_H),
				1200, 400, OFF, ARGB(255, 255, 255, 255), ON, 0, 0.15f, 0.15f);
		}
		else { GameAni = false; AniMove = 0; }
		if (Blk[0].bPosition.x <= 400 || Blk[0].bPosition.x >= 880) {
			for (int i = 0; i < 9; i++) {
				Gs_DrawLayer(WD_Size_W / 2, (WD_Size_H / 2 - 100) + (i * 60), TEX_Game, 0, BlkVl[i].Rdm * (Blk_Size_H),
					1200, 400, OFF, ARGB(255, 255, 255, 255), ON, 0, 0.15f, 0.15f);
			}
		}
	}
	//Think Bubble
	for (int i = 0; i < BlkClr; i++) {
		if (TB[i].tAlpha < 255) TB[i].tAlpha += 5;
		ThinkEft(Think_Color[i], i);
		//very small bubble
		Gs_DrawLayer(TB[i].thkP[0].x, TB[i].thkP[0].y, TEX_Game, 1004, 2762,
			27, 17, OFF, ARGB(TB[i].tAlpha, 255, 255, 255), ON, 0, 1.0f, 1.0f);
		//small bubble
		Gs_DrawLayer(TB[i].thkP[1].x, TB[i].thkP[1].y, TEX_Game, 967, 2762,
			37, 22, OFF, ARGB(TB[i].tAlpha, 255, 255, 255), ON, 0, 1.0f, 1.0f);
		//bubble
		Gs_DrawLayer(TB[i].thkP[2].x, TB[i].thkP[2].y, TEX_Game, 967, 2653,
			177, 109, OFF, ARGB(TB[i].tAlpha, 255, 255, 255), ON, 0, 1.0f, 1.0f);
		Gs_DrawLayer(TB[i].thkP[2].x, TB[i].thkP[2].y, TEX_Game, 0, (i + 2) * (Blk_Size_H),
			1200, 400, OFF, ARGB(TB[i].tAlpha, 255, 255, 255), ON, 0, 0.1f, 0.1f);
	}


	//Life
	if (GmSt.status != GameState::Over)
		Gs_DrawLayer(WD_Size_W / 2, WD_Size_H / 2 - 280, TEX_Game, 1050, 3696,
			1200, 200, OFF, ARGB(255, iRClr, iGClr, 0), ON, 0, fLife*0.01f, 0.2f);




	//Combo
	Combo_Read(iScore, 0);

	//Daruma Head
	Gs_DrawLayer(WD_Size_W / 2, WD_Size_H / 2 - 190, TEX_Game, Pos_DarumaHead[iDarumaHead].x, Pos_DarumaHead[iDarumaHead].y,
		1200, 800, OFF, ARGB(255, 255, 255, 255), ON, 0, 0.15f, 0.15f);

	//Fever
	int GgeVal = 1200 / 300 * fFevGge;
	if (GgeVal >= 1200)
		GgeVal = 1200;
	else if (GgeVal <= 0)
		GgeVal = 0;
	//Null Gauge (Back Gauge)
	Gs_DrawLayer(WD_Size_W / 2, (WD_Size_H / 2) + 300, TEX_Game, 1050, 3896,
		1200, 200, OFF, ARGB(255, 255, 255, 255), ON, 0, 0.3f, 0.3f);
	//Gauge
	// (WD_Size_W / 2 - ((1200 - GgeVal)*0.3f) / 2) is Image's X Position
	//                                    this method is left line up
	Gs_DrawLayer(WD_Size_W / 2 - ((1200 - GgeVal)*0.3f) / 2, (WD_Size_H / 2) + 300, TEX_Game, 2250, 3896,
		GgeVal, 200, OFF, ARGB(255, 255, 255, 255), ON, 0, 0.3f, 0.3f);

	if (Fever) {

		if (iFvrImgY > WD_Size_H / 2) {
			fFntSize = (float)(WD_Size_H / 2) / (float)iFvrImgY;
			iFvrImgY -= 15;
		}


		if (iFvrCnt < 40) {
			Gs_DrawLayer(WD_Size_W / 2, iFvrImgY, TEX_Label, 0, 345,
				425, 145, OFF, ARGB(255, 255, 255, 255), ON, 0, fFntSize, fFntSize);
		}
		else {
			if (fFntSize > 0) {
				fFntSize -= 0.05f;
				iFvrImgY -= 10;
				Gs_DrawLayer(WD_Size_W / 2, WD_Size_H / 2, TEX_Game, 1050, 3120,
					570, 570, OFF, ARGB(255, 255, 255, 255), ON, iFvrImgY, fFntSize, fFntSize);
			}
		}
	}

	if (GmSt.status == GameState::Start) {
		int num = 0, num2 = 86;
		float fFontEft = 0;

		if (iGmCnt >= 60) num = 1;
		if (iGmCnt >= 120) num = 2;
		if (iGmCnt >= 180) { num = 3; num2 = 450; }
		if (iGmCnt >= 240) {
			GmSt.status = GameState::Game;
			Gs_PlaySE(SND_Start);
		}
		if (iGmCnt < 180)
			fFontEft = (60 - iGmCnt % 60) * 0.03f;

		Gs_DrawLayer(WD_Size_W / 2, WD_Size_H / 2, TEX_Label, 86 * num, 205,
			num2, 120, OFF, ARGB(255, 255, 255, 255), ON, 0, 1.3f + fFontEft, 1.3f + fFontEft);
	}

	if (GmSt.status == GameState::Over) {
		float fImgScl = 0;
		if (iGmCnt < 60) {
			if (iGmCnt < 30) {
				fImgScl = 1.5f;
			}
			else {
				fImgScl = 1.5f - (iGmCnt % 30 * 0.05f);
				if (fImgScl < 0) fImgScl = 0;
			}
			Gs_DrawLayer(WD_Size_W / 2, WD_Size_H / 2, TEX_Label, 430, 340,
				230, 130, OFF, ARGB(255, 255, 255, 255), ON, 0, fImgScl, fImgScl);
		}
		else if (iGmCnt <= 120) {
			fImgScl = (iGmCnt - 60) * (0.01f * iGmCnt);
			Gs_DrawLayer(WD_Size_W / 2, WD_Size_H / 2, TEX_Game, 840, 3685,
				170, 170, OFF, ARGB(130, 255, 255, 255), ON, 0, fImgScl, fImgScl);
		}
		else {
			int OrderCnt = (iGmCnt - 120) / 30, Btn_Visbl = 7;

			Gs_DrawLayer(WD_Size_W / 2, WD_Size_H / 2, TEX_Game, 840, 3685,
				170, 170, OFF, ARGB(130, 255, 255, 255), ON, 0, 60.0f, 60.0f);

			if (OrderCnt >= 0) {
				if (!(fRsltX1_Spd <= 0)) {
					fRsltX1 += fRsltX1_Spd;
					fRsltX1_Spd *= fRsltX1_DS;
				}
			}
			if (OrderCnt >= 2) {
				Combo_Read(iScore, 1);
			}
			if (OrderCnt >= 3) {
				if (!(fRsltX2_Spd <= 0)) {
					fRsltX2 += fRsltX2_Spd;
					fRsltX2_Spd *= fRsltX2_DS;
				}
			}
			if (OrderCnt >= 5) {
				if (!bScrUpt)	Combo_Read(iHScore, 2);
				else {
					if (OrderCnt < 7)
						Combo_Read(iHScore, 2);
				}
			}
			if (OrderCnt >= 7) {
				if (bScrUpt) {
					Btn_Visbl = 10;
					Combo_Read(iScore, 2);
					for (int i = 0; i < 5; i++) { StarEft(i, 1); }
					Gs_DrawLayer(WD_Size_W / 2 + 100, WD_Size_H / 2 - 30, TEX_Label, 670, 480,
						230, 135, OFF, ARGB(255, 255, 255, 255), ON, -10, 0.5f, 0.5f);
				}
			}
			if (OrderCnt >= Btn_Visbl)
			{
				int BtnSize_X = 490/2, BtnSize_Y = 140/2;

				//Restart
				Gs_DrawLayer(WD_Size_W / 2, fRsltY + 50, TEX_Label, 490, 740,
					BtnSize_X*2, BtnSize_Y*2, OFF, ARGB(255, 255, 255, 255), ON, 0, 0.5f, 0.5f);

				if (!(fRsltY_Spd <= 0)) {
					fRsltY -= fRsltY_Spd;
					fRsltY_Spd *= fRsltY_DS;
				}

				if ((WD_Size_W/2 - BtnSize_X <= Mouse.Px && Mouse.Px <= WD_Size_W/2 + BtnSize_X && (Mouse.Trg & _lMOUSE))) {
					Gs_FadeOut(RGB(0, 0, 0), 10);
					iSceneCng = 2;
				}

				if (iSceneCng != 0) {
					iSC_Cnt++;
					if (iSC_Cnt >= 25) {
						if (iSceneCng == 1) { SceneChange(TITLE_SCENE); }
						else if (iSceneCng == 2) { SceneChange(GAME_SCENE); }
					}
				}
			}

			Gs_DrawLayer(fRsltX1, WD_Size_H / 2 - 200, TEX_Label, 0, 480,
				670, 135, OFF, ARGB(255, 255, 255, 255), ON, 0, 0.75f, 0.75f);
			Gs_DrawLayer(fRsltX2, WD_Size_H / 2 - 50, TEX_Label, 0, 615,
				460, 135, OFF, ARGB(255, 255, 255, 255), ON, 0, 0.75f, 0.75f);

		}
	}
}

// Loading Data
short	GameLoad(void)
{
	BG = Gs_LoadBMP("DATA/BMP/Background_01.png");
	BG2_1 = Gs_LoadBMP("DATA/BMP/Background_02_1.png");
	BG2_2 = Gs_LoadBMP("DATA/BMP/Background_02_2.png");
	BG2_3 = Gs_LoadBMP("DATA/BMP/Background_02_3.png");
	TEX_Game = Gs_LoadBMP("DATA/BMP/Tex_Game.PNG");
	TEX_Label = Gs_LoadBMP("DATA/BMP/Tex_Label.PNG");

	SND_BGM1 = Gs_LoadWAVE("DATA/SOUND/BGM.WAV", ON);
	Gs_PlayBGM(SND_BGM1);
	SND_Start = Gs_LoadWAVE("DATA/SOUND/Start.WAV", OFF);
	SND_GameOver = Gs_LoadWAVE("DATA/SOUND/GameOver.WAV", OFF);
	SND_Hit = Gs_LoadWAVE("DATA/SOUND/Hammer.WAV", OFF);


	file_I = fopen("DATA/X/HighScore.txt", "rt");
	if (file_I) { fscanf(file_I, "%d", &iHScore); }
	file_O = fopen("DATA/X/HighScore.txt", "wt");
	if (file_I == NULL || file_O == NULL) {
		return TITLE_SCENE;
	}

	return		GAME_SCENE;
}

void	GameExit(void)
{
	Gs_ReleaseBMP(BG);
	Gs_ReleaseBMP(BG2_1);
	Gs_ReleaseBMP(BG2_2);
	Gs_ReleaseBMP(TEX_Game);
	Gs_ReleaseBMP(TEX_Label);

	Gs_ReleaseSOUND(SND_BGM1);
	Gs_ReleaseSOUND(SND_Start);
	Gs_ReleaseSOUND(SND_GameOver);
	Gs_ReleaseSOUND(SND_Hit);

	fclose(file_I);
	fclose(file_O);
}

SCENE_TBL	GameSceneTbl = {
	GameLoad,
	GameInit,
	GameLoop,
	GameDraw,
	GameExit
};

//======================================================================================//
//							EOP															//
//======================================================================================//

// Key Press Function
void KeyPress_Read()
{
	// Left, Blue & Yellow
	if ((Gs_GetKEY(kKEY_A) || Gs_GetKEYtrg(kKEYLEFT) || (((Mouse.Trg & _lMOUSE) || (Mouse.WClk & _lMOUSE)) && (Mouse.Px < WD_Size_W / 2))) && !Reset) {
		if (BlkVl[0].Rdm == 2 || BlkVl[0].Rdm == 4) {
			if (!Fever)
				iDarumaHead = 1;
			Gs_PlaySE(SND_Hit);
			GameAni = true; Reset = true; AniMove = 1;
			BlkReset = true; BlkHit = 1;
			Blk[0].bPosition.x = WD_Size_W / 2;
			Blk[0].bSpdX = 150.0f;
			Blk[0].bSpdX_Down = 0.9f;
			BlkVl[9] = BlkVl[0]; // 9 = Block's pointer
			iScore++;
			if (fLife + 2 > 20) fLife = 20;
			else				fLife += iHit;
			if (iHit <= 4) iHit++;
			if (!Fever) fFevGge += 10;
		}
		else {
			if (Fever) {
				Gs_PlaySE(SND_Hit);
				GameAni = true; Reset = true; AniMove = 1;
				BlkReset = true; BlkHit = 1;
				Blk[0].bPosition.x = WD_Size_W / 2;
				Blk[0].bSpdX = 150.0f;
				Blk[0].bSpdX_Down = 0.9f;
				BlkVl[9] = BlkVl[0]; // 9 = Block's pointer
				iScore++;
				if (fLife + 2 > 20) fLife = 20;
				else				fLife += iHit;
				if (iHit <= 4) { iHit++; iMiss = 1; }
			}
			if (fLife > 0) {
				if (!Fever) {
					if (bMsCnt) fLife -= 6;
					fLife -= iMiss;
					bMsCnt = true;
					iMsCnt = 0;
					if (iMiss <= 4) { iMiss++; iHit = 1; }
					fFevGge -= 20;
					iDarumaHead = 6;
				}
			}
			else {
				//Game Over
			}
		}
	}
	//Right, Red & Green
	if ((Gs_GetKEY(kKEY_A) || Gs_GetKEYtrg(kKEYRIGHT) || (((Mouse.Trg & _lMOUSE) || (Mouse.WClk & _lMOUSE)) && (Mouse.Px > WD_Size_W / 2))) && !Reset) {
		if (BlkVl[0].Rdm == 3 || BlkVl[0].Rdm == 5) {
			if (!Fever)
				iDarumaHead = 1;
			Gs_PlaySE(SND_Hit);
			GameAni = true; Reset = true; AniMove = 2;
			BlkReset = true; BlkHit = 2;
			Blk[0].bPosition.x = WD_Size_W / 2;
			Blk[0].bSpdX = 150.0f;
			Blk[0].bSpdX_Down = 0.9f;
			BlkVl[9] = BlkVl[0]; // 9 = Block's pointer
			iScore++;
			if (fLife + 2 > 20) fLife = 20;
			else				fLife += iHit;
			if (iHit <= 4) iHit++;
			if (!Fever) fFevGge += 10;
		}
		else {
			if (Fever) {
				Gs_PlaySE(SND_Hit);
				GameAni = true; Reset = true; AniMove = 2;
				BlkReset = true; BlkHit = 2;
				Blk[0].bPosition.x = WD_Size_W / 2;
				Blk[0].bSpdX = 150.0f;
				Blk[0].bSpdX_Down = 0.9f;
				BlkVl[9] = BlkVl[0]; // 9 = Block's pointer
				iScore++;
				if (fLife + 2 > 20) fLife = 20;
				else				fLife += iHit;
				if (iHit <= 4) { iHit++; iMiss = 1; }
			}
			if (fLife > 0) {
				if (!Fever) {
					if (bMsCnt) fLife -= 6;
					fLife -= iMiss;
					bMsCnt = true;
					iMsCnt = 0;
					if (iMiss <= 4) { iMiss++; iHit = 1; }
					fFevGge -= 20;
					iDarumaHead = 6;
				}
			}
			else {
				//Game Over
			}
		}
	}
	/*
	if (Gs_GetPADtrg(PAD_1P, kPAD_A)) {
		//Left
		iDarumaHead = 2;
	}
	if (Gs_GetPADtrg(PAD_1P, kPAD_A)) {
		//Right
		iDarumaHead = 3;
	}
	*/
}

// Combo Function
void Combo_Read(int &n_, int mode_)
{
	int Combo_XPos, Combo_YPos;
	switch (mode_)
	{
	case 0:
		Combo_XPos = 0;
		Combo_YPos = WD_Size_H / 8;
		break;
	case 1:
		Combo_XPos = WD_Size_W / 2 + 100;
		Combo_YPos = WD_Size_H / 2 - 140;
		break;
	case 2:
		Combo_XPos = WD_Size_W / 2 + 100;
		Combo_YPos = WD_Size_H / 2;
		break;
	}


	int indent;

	indent = 1;

	if (n_ >= 10) {
		indent = 2;
		if (n_ >= 100) {
			indent = 3;
			if (n_ >= 1000) {
				indent = 4;
			}
		}
	}

	// 1000
	if (n_ / 1000 != 0) {
		Gs_DrawLayer(Combo_XPos + (int)(65 * 0.8f) * (indent - 3), Combo_YPos, TEX_Label, 67 * ((n_ / 1000) % 10), 0,
			65, 100, OFF, ARGB(255, 255, 255, 255), ON, 0, 0.8f, 0.8f);
	}
	// 100
	if (n_ / 100 != 0) {
		Gs_DrawLayer(Combo_XPos + (int)(65 * 0.8f) * (indent - 2), Combo_YPos, TEX_Label, 67 * ((n_ / 100) % 10), 0,
			65, 100, OFF, ARGB(255, 255, 255, 255), ON, 0, 0.8f, 0.8f);
	}
	// 10
	if (n_ / 10 != 0) {
		Gs_DrawLayer(Combo_XPos + (int)(65 * 0.8f) * (indent - 1), Combo_YPos, TEX_Label, 67 * ((n_ / 10) % 10), 0,
			65, 100, OFF, ARGB(255, 255, 255, 255), ON, 0, 0.8f, 0.8f);
	}
	// 1
	Gs_DrawLayer(Combo_XPos + (int)(65 * 0.8f) * indent, Combo_YPos, TEX_Label, 67 * (n_ % 10), 0,
		65, 100, OFF, ARGB(255, 255, 255, 255), ON, 0, 0.8f, 0.8f);

	//dan
	Gs_DrawLayer(Combo_XPos + (int)(65 * 0.8f) * indent + (102 + 65) / 2 * 0.8f, Combo_YPos, TEX_Label, 0, 100,
		102, 103, OFF, ARGB(255, 255, 255, 255), ON, 0, 0.8f, 0.8f);
}

// Star Effect Function
void StarEft(int &n_, int mode_)
{
	//Fever Effect
	if (mode_ == 0) {
		if (StEft[n_].sPosition.y < -100)
		{
			StEft[n_].sPosition.x = rand() % 1280;
			StEft[n_].sPosition.y = WD_Size_H + 100;
			StEft[n_].sRotate = 0;
			StEft[n_].sRotSpd = rand() % 5 + 1;
			StEft[n_].sSize = rand() % 8 + 1;
			StEft[n_].sSpeed = rand() % 30 + 10;
		}

		Gs_DrawLayer(StEft[n_].sPosition.x, StEft[n_].sPosition.y, TEX_Game, 1050, 3120,
			570, 570, OFF, ARGB((iBB_Alp / 2) * 3, 255, 255, 255), ON, StEft[n_].sRotate, 0.05f * StEft[n_].sSize, 0.05f * StEft[n_].sSize);

		if (n_ % 2 == 0)	StEft[n_].sRotate += StEft[n_].sRotSpd;
		else				StEft[n_].sRotate -= StEft[n_].sRotSpd;

		StEft[n_].sPosition.y -= StEft[n_].sSpeed;
	}
	//High Score Effect
	else if (mode_ == 1) {
		if (StEft[n_].sPosition.y < WD_Size_H) {
			Gs_DrawLayer(StEft[n_].sPosition.x, StEft[n_].sPosition.y, TEX_Game, 1050, 3120,
				570, 570, OFF, ARGB(200, 255, 255, 255), ON, StEft[n_].sRotate, 0.05f * StEft[n_].sSize, 0.05f * StEft[n_].sSize);

			if (n_ % 2 == 0)	StEft[n_].sRotate += StEft[n_].sRotSpd;
			else				StEft[n_].sRotate -= StEft[n_].sRotSpd;

			StEft[n_].sPosition.y += StEft[n_].sSpeed;
			if (-1.0f < StEft[n_].sSpeed && StEft[n_].sSpeed < 0) {
				StEft[n_].sSpeed *= -1.0f;
				StEft[n_].sSpdD = 1.5f;
			}
			StEft[n_].sSpeed *= StEft[n_].sSpdD;
			if (n_ == 0) StEft[n_].sPosition.x -= StEft[n_].sSpdD * 2.5f;
			if (n_ == 1) StEft[n_].sPosition.x -= StEft[n_].sSpdD * 1.4f;
			if (n_ == 3) StEft[n_].sPosition.x += StEft[n_].sSpdD * 1.7f;
			if (n_ == 4) StEft[n_].sPosition.x += StEft[n_].sSpdD * 2.2f;

		}
	}
}

// Daruma's Think Function
void ThinkEft(bool color_, int& n_) 
{
	if (!color_) {/*
		TB[n_].thkP[0] = { WD_Size_W / 2, WD_Size_H / 2 - 190 };
		TB[n_].thkP[1] = { WD_Size_W / 2, WD_Size_H / 2 - 190 };
		TB[n_].thkP[2] = { WD_Size_W / 2, WD_Size_H / 2 - 190 };*/
		
		//No Ani, temp ---
		if (n_ == 0) {
			TB[n_].thkP[0] = { WD_Size_W / 2 - 130, WD_Size_H / 2 - 230 };
			TB[n_].thkP[1] = { WD_Size_W / 2 - 160, WD_Size_H / 2 - 250 };
			TB[n_].thkP[2] = { WD_Size_W / 2 - 250, WD_Size_H / 2 - 300 };
		}if (n_ == 1) {
			TB[n_].thkP[0] = { WD_Size_W / 2 + 130, WD_Size_H / 2 - 230 };
			TB[n_].thkP[1] = { WD_Size_W / 2 + 160, WD_Size_H / 2 - 250 };
			TB[n_].thkP[2] = { WD_Size_W / 2 + 250, WD_Size_H / 2 - 300 };
		}if (n_ == 2) {
			TB[n_].thkP[0] = { WD_Size_W / 2 - 150, WD_Size_H / 2 - 190 };
			TB[n_].thkP[1] = { WD_Size_W / 2 - 200, WD_Size_H / 2 - 200 };
			TB[n_].thkP[2] = { WD_Size_W / 2 - 330, WD_Size_H / 2 - 210 };
		}if (n_ == 3) {
			TB[n_].thkP[0] = { WD_Size_W / 2 + 150, WD_Size_H / 2 - 190 };
			TB[n_].thkP[1] = { WD_Size_W / 2 + 200, WD_Size_H / 2 - 200 };
			TB[n_].thkP[2] = { WD_Size_W / 2 + 330, WD_Size_H / 2 - 210 };
		}
		//---
		if (n_ == 0) {
			TB[n_].tSpdX = 100.0f;
			TB[n_].tSpdY = 100.0f - TB[0].tSpdX;
		}if (n_ == 1) {
			TB[n_].tSpdX = -100.0f;
			TB[n_].tSpdY = 100.0f - TB[0].tSpdX;
		}if (n_ == 2) {
			TB[n_].tSpdX = 200.0f;
			TB[n_].tSpdY = 100.0f - TB[0].tSpdX;
		}if (n_ == 3) {
			TB[n_].tSpdX = -200.0f;
			TB[n_].tSpdY = 100.0f - TB[0].tSpdX;
		}

		color_ = true;
	}
}
void SystemCheckGame(void)
{
	if (GameSystemtimer > 0)
	{
		GameSystemtimer -= 0.016666f;

		if (GameSystemtimer <= 0.0f)
		{
			GameSystemtimer = 0.0f;
			GameSystemNum[0] = false, GameSystemNum[1] = false, GameSystemNum[2] = false, GameSystemNum[3] = false;
		}
	}

	if ((GameCollCheck(0, 0, 50, 50, OFF, Mouse.Px, Mouse.Py, 1, 1, ON) && Mouse.Trg & _lMOUSE))
	{
		GameSystemNum[0] = true;
		GameSystemtimer = 1.0f;
	}

	if ((GameCollCheck(1230, 0, 50, 50, OFF, Mouse.Px, Mouse.Py, 1, 1, ON) && Mouse.Trg & _lMOUSE))
	{
		GameSystemNum[1] = true;
		GameSystemtimer = 1.0f;
	}

	if ((GameCollCheck(0, 680, 50, 50, OFF, Mouse.Px, Mouse.Py, 1, 1, ON) && Mouse.Trg & _lMOUSE))
	{
		GameSystemNum[2] = true;
		GameSystemtimer = 1.0f;
	}

	if ((GameCollCheck(1230, 680, 50, 50, OFF, Mouse.Px, Mouse.Py, 1, 1, ON) && Mouse.Trg & _lMOUSE))
	{
		GameSystemNum[3] = true;
		GameSystemtimer = 1.0f;
	}

	if (GameSystemNum[0] && GameSystemNum[1] && GameSystemNum[2] && GameSystemNum[3])
	{
		SceneChange(TITLE_SCENE);
	}
}

bool GameCollCheck(float x1, float y1, float sx1, float sy1, bool genten1,
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