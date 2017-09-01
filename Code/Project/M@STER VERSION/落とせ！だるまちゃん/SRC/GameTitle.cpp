////======================================================================================//
//#include		<windows.h>
//#include		<stdio.h>
//#include		<math.h>
//#include		"MASTER/Ci-Lib.H"
//#include		"Game.H"
//
////****************************************************************************
////	■	定義
////
////****************************************************************************
//
//
//
//GsTEXTURE tex1;
//
//int gamestate = 0;
//int scenestate = 0;
//int scenechange = 0;
//bool scenechangestate = false;
//
//float a = 10.0f;
//
//
////****************************************************************************
////	■	内部使用　変数
////
////****************************************************************************
//
//
//
////--------------------------------------------------------------------------//
////	●	初期化		：シーンに移る時一回、変数の初期化
////
////--------------------------------------------------------------------------//
//
//void	TitleInit(void)
//{
//
//	Gs_FontSet(5, 64);
//	Gs_FadeIn(RGB(0x00, 0x00, 0x00), 10);
//	scenestate = SAISHOSCENE;
//	scenechange = FADEIN;
//}
//
////--------------------------------------------------------------------------//
////	●	内部処理	：シーン中の処理、現在のシーンで行う処理
////
////--------------------------------------------------------------------------//
//
//void	TitleLoop(void)
//{
//
//	if (scenestate == SAISHOSCENE && scenechangestate == false)
//	{
//		if (Gs_GetKEY(kKEY_A))//DIKEYBOARD_NUMPAD1
//		{
//			GamePublic.SceneNum = 1;
//			scenechangestate = true;
//			Gs_FadeOut(RGB(0x00, 0x00, 0x00), 10);
//			scenechange = FADEOUT;
//		}
//
//		if (Gs_GetKEY(kKEY_B))
//		{
//			GamePublic.SceneNum = 2;
//			scenechangestate = true;
//			Gs_FadeOut(RGB(0x00, 0x00, 0x00), 10);
//			scenechange = FADEOUT;
//		}
//	}
//
//
//	else if (scenestate == SAISHOSCENE && scenechangestate == true)
//	{
//		if (Gs_FadeCheck() == OFF)
//		{
//			scenestate = TITLESCENE;
//			scenechangestate = false;
//			Gs_FadeIn(RGB(0x00, 0x00, 0x00), 10);
//			scenechange = FADEIN;
//		}
//	}
//
//
//
//
//	//////////////////////////////////////////////////////////////////////////////
//
//
//	else if (GamePublic.SceneNum == 1 && scenestate != SAISHOSCENE)  // GamePublic.SceneNum : 移動
//	{
//		if (a <= 0.0f && scenechangestate == false && scenestate != GAMESCENE)
//		{
//			a = 10.0f;
//			scenechangestate = true;
//			Gs_FadeOut(RGB(0x00, 0x00, 0x00), 10);
//			scenechange = FADEOUT;
//		}
//
//		else if (scenechangestate == true)
//		{
//			if (Gs_FadeCheck() == OFF)
//			{
//				if (scenestate == TITLESCENE)
//				{
//					scenestate = MAINSCENE;
//					scenechangestate = false;
//					Gs_FadeIn(RGB(0x00, 0x00, 0x00), 10);
//					scenechange = FADEIN;
//				}
//
//				else if (scenestate == MAINSCENE)
//				{
//					scenestate = GAMESCENE;
//					scenechangestate = false;
//					a = 0.0f;
//					Gs_FadeIn(RGB(0x00, 0x00, 0x00), 10);
//					scenechange = FADEIN;
//				}
//			}
//		}
//
//		else if (a > 0.0f && scenechangestate == false)
//			a -= 0.1f;
//
//	}
//
//
//	/////////////////////////////////////////////////////////////////////////////
//
//
//	else if (GamePublic.SceneNum == 2 && scenestate != SAISHOSCENE)  // GamePublic.SceneNum : マウス
//	{
//
//		if ((Mouse.Trg & _lMOUSE) && scenechangestate == false) {
//			if (Gs_FadeCheck() == OFF)
//			{
//				scenechangestate = true;
//				Gs_FadeOut(RGB(0x00, 0x00, 0x00), 10);
//				scenechange = FADEOUT;
//			}
//		}
//
//		else if (scenechangestate == true && Gs_FadeCheck() == OFF)
//		{
//			if (scenechange == FADEOUT && Gs_FadeCheck() == OFF)
//			{
//				switch (scenestate)
//				{
//				case TITLESCENE:
//					scenestate = MAINSCENE;
//					Gs_FadeIn(RGB(0x00, 0x00, 0x00), 10);
//					scenechangestate = false;
//					scenechange = NOFADE;
//					break;
//
//				case MAINSCENE:
//					scenestate = GAMESCENE;
//					Gs_FadeIn(RGB(0x00, 0x00, 0x00), 10);
//					scenechangestate = true;
//					scenechange = NOFADE;
//					break;
//				}
//			}
//		}
//
//	}
//
//	/*else if (GamePublic.SceneNum == 2 && scenestate != SAISHOSCENE)  // GamePublic.SceneNum : マウス
//	{
//	if (a <= 0.0f && scenechangestate == false &&
//	scenestate != GAMESCENE)
//	{
//	a = 10.0f;
//	scenechangestate = true;
//	Gs_FadeOut(RGB(0x00, 0x00, 0x00), 10);
//	}
//
//	else if (scenechangestate == true)
//	{
//	if (Gs_FadeCheck() == OFF)
//	{
//	if (scenestate == TITLESCENE)
//	{
//	scenestate = MAINSCENE;
//	scenechangestate = false;
//	Gs_FadeIn(RGB(0x00, 0x00, 0x00), 10);
//	}
//
//	else if (scenestate == MAINSCENE)
//	{
//	scenestate = GAMESCENE;
//	scenechangestate = false;
//	a = 0.0f;
//	Gs_FadeIn(RGB(0x00, 0x00, 0x00), 10);
//	}
//	}
//	}
//
//	else if (a > 0.0f && scenechangestate == false)
//	a -= 0.1f;
//
//	}*/
//
//
//}
//
////--------------------------------------------------------------------------//
////	●	描画		：シーン中の描画
////
////--------------------------------------------------------------------------//
//
//// Ball 32 32 , Bar 96 32 , Ba 150(?) 32
//
//
//
//
//void	TitleDraw(void)
//{
//	switch (scenestate)
//	{
//	case SAISHOSCENE:
//		Gs_DrawLayer(0, 0, tex1, 0, 480, 639, 479, OFF, ARGB(255, 255, 255, 255), OFF, 0, 1.0f, 1.0f);
//		break;
//
//	case GAMESCENE:
//		Gs_DrawLayer(0, 0, tex1, 1280, 0, 639, 479, OFF, ARGB(255, 255, 255, 255), OFF, 0, 1.0f, 1.0f);
//		Gs_DrawText(/*96*/ 110, 350, "～終わりです～", ARGB(255, 0, 0, 0)); //0xff, 0xff, 0xff, 0xff white
//		break;
//
//	case MAINSCENE:
//		Gs_DrawLayer(0, 0, tex1, 640, 0, 639, 479, OFF, ARGB(255, 255, 255, 255), OFF, 0, 1.0f, 1.0f);
//		break;
//
//	case TITLESCENE:
//		Gs_DrawLayer(0, 0, tex1, 0, 0, 639, 479, OFF, ARGB(255, 255, 255, 255), OFF, 0, 1.0f, 1.0f);
//		break;
//	}
//}
//
////--------------------------------------------------------------------------//
////	●	ロード		：シーンに入る時、そのシーンで必要なデータの読み込み
////
////--------------------------------------------------------------------------//
//
//short	TitleLoad(void)
//{
//	tex1 = Gs_LoadBMP("DATA/BMP/texture.PNG");
//	return		TITLE_SCENE;
//}
//
////--------------------------------------------------------------------------//
////	●	後処理		：現在のシーンから抜ける時、データの破棄
////
////--------------------------------------------------------------------------//
//
//void	TitleExit(void)
//{
//	Gs_ReleaseBMP(tex1);
//
//}
//
////****************************************************************************
////	★　シーンテーブル：各シーンで呼び出される関数の登録
////****************************************************************************
//
//SCENE_TBL	TitleSceneTbl = {
//	TitleLoad,
//	TitleInit,
//	TitleLoop,
//	TitleDraw,
//	TitleExit
//};