#pragma once


FILE* file_I; // Input
FILE* file_O; // Output

// Read getting Key's data
void KeyPress_Read();
// Combo Judge Function
void Combo_Read(int& n_, int mode_);
// Show Fever Effect
void StarEft(int& n_, int mode_);
// Thinking bubble
void ThinkEft(bool color_, int& n_);

// State of the Game
struct GameState {
	enum State { Start, Game, Stop, Over };
	State status;
};

//Throwing Block's values Struct
struct Block {
	POINT bPosition;
	float bSpdX, bSpdY;
	float bSpdX_Down, bSpdY_Down;
};

// Block Value Struct
struct Block_ {
	int Rdm;
};

// Star Effect's Data Struct (Fever, HighScore)
struct Star {
	int sRotate;
	int sRotSpd;
	float sSpeed, sSpdD;
	int sSize;
	POINT sPosition;
};

// Daruma's Think Bubble's Data Struct
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

POINT Pos_DarumaHead[7];	////

							// ------- Game Data ------- //
Block Blk[10];
Star StEft[10];
ThkBubble TB[4];
Block_ BlkVl[10];

bool GameAni;
bool Reset;
bool BlkReset;
bool Fever;
bool Think_Color[4]; ////

int iBlkClr;
int iGmCnt; // int Game Start Count
int iFvrCnt; // int Fever Count
int iFvrImgY;
float fFntSize;

int iBlkHit;		// 0 = Null, 1 = Right, 2 = Left
int AniMove;

int iScore, iGClr, iRClr;
float fLife, fFevGge;
int iHit, iMiss, iMsCnt;
bool bMsCnt;

int Ani_Tick_Ham; ////
int iBB_Alp, iEft_Alp, iHam_Rot; //BB = Black BG ////iHamRot
float BG_Y_Pos1, BG_Y_Pos2;
float BG_Y_Pos1_, BG_Y_Pos2_;
bool Castle;

int iDarumaHead;

//
bool bScrUpt; // bool Score Update
int iHScore; // int High Score
float fRsltX1, fRsltX2; //// // float Result Position
float fRsltX1_Spd, fRsltX2_Spd;////  // float Result Position Speed
float fRsltX1_DS, fRsltX2_DS;//// // float Result Position Down Speed
float fRsltY, fRsltY_Spd, fRsltY_DS; ////
int iSceneCng, iSC_Cnt; //// // int Scene Change, Scene Change Count