#ifndef DXMAN_H
#define DXMAN_H

//#include <targastruct.h>
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_mixer.h>
#include <base.h>
#include <camera.h>
#include "entity.h"
#include <RAnim_bgra32.h>
#include <RFont_bgra32.h>
#include <vector>
//for cpp:
#include <SDL2/SDL.h> //remember to add mingw to path in compiler settings, put SDL includes and libs into mingw path, and add SDL libs to linker settings
//#include <SDL2/SDL_mixer.h>
//#include <SDL2/SDL_TTF.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <cmath>
#include <queue>
#include <RSprite_bgra32.h>
#include <RImage_bgra32.h> //Dr	awSubPixelArc etc
//#include <RAnim_bgra32.h>
//#include <RFont_bgra32.h>
#include <RMath.h> //LineRelationshipToString etc
//#include "entity.h"
//#include <camera.h>
//#include <base.h>
#include <frameworkdummy.h>;
#include <StringQ.h>;
#include <RSound_audiere.h>;


#define MYNAME_AND_VERSION "DXMan 2014-10-25 SDL2"

using namespace std;
using namespace ExpertMultimediaBase;

///#region SDL.h and SDL_mixer.h externs
//from SDL.h, but SDL.h can't be used or linker error (undefined reference to SDL_main) will occur
//typedef struct Mix_Chunk;
typedef uint32_t	Uint32;
///#endregion SDL.h externs

int main(int iArgs, char *lpsArg[]); //int main(int iArgs, char** lpsArg) {

namespace ExpertMultimediaBase {
	// DEFINES ////////////////////////////////////////////////
	#define SOUND_BUFFERS 15
	#define SOUND_SOURCES 20
	#define BOUNDARY_BOUNCE 0
	#define BOUNDARY_STOP   1
	#define BOUNDARY_DIE	2
	#define FXMIN   -10.0f
	#define FXMAX   10.0f
	#define FYMIN   -10.0f
	#define FYMAX   10.0f
	#define FXRANGE 20.0f
	#define FYRANGE 20.0f
	//default screen size
	//#define SCREEN_WIDTH			640
	//#define SCREEN_HEIGHT			480
	//#define FSCREEN_WIDTH			640.0f
	//#define FSCREEN_HEIGHT			480.0f
	//#define BUFFER_WIDTH			1280
	//#define BUFFER_HEIGHT			960
	//#define BUFFER_PIXELS			1228800 //1280x960
	//#define SCREEN_OFFSET_X			320 //screen margin to avoid clipping
	//#define SCREEN_OFFSET_Y			240
	//#define FSCREEN_OFFSET_X		320.0f //screen margin to avoid clipping
	//#define FSCREEN_OFFSET_Y		240.0f
	#define SCREEN_BPP				32	 //bits per pixel, GameInit tries 32 or 24 if other is not available
	#define CODE_THRESHOLD		  90
	#define GRAVITY_METERS_PERSEC_PERFRAME	 0.026666666666666666666666666666667f //== (.8f/30.0f) //meters per second per frame (positive) gravity
	//States for game loop
	#define GAMESTATE_INIT				0
	#define GAMESTATE_START_AREA		1
	#define GAMESTATE_START_ENCOUNTER	2
	#define GAMESTATE_RUN				3
	#define GAMESTATE_SHUTDOWN			4
	#define GAMESTATE_EXIT				5
	#define GAMESTATE_WIN_ENCOUNTER		6
	#define GAMESTATE_WIN_GAME			7
	#define GAMESTATE_YOU_LOSE			8
	#define GAMESTATE_LOSEGUY			9

	//Operations for Explosion
	#define EXPLOSION_CHECK_COUNT	0
	#define EXPLOSION_START			1
	#define EXPLOSION_CONTINUE		2
	#define PI						3.1416
	//States of hero or enemies
	#define STATUS_ALIVE			1
	#define STATUS_SHIELD			2
	#define STATUS_DOUBLESPEED		4
	#define STATUS_ANTIGRAVITY		8
	#define STATUS_SHOOTER			16
	#define STATUS_BOMBER			32
	#define STATUS_BOSS				64
	#define STATUS_RAPIDFIRE		256
	#define STATUS_AIMBOMBS			512 //AIM AI
	#define STATUS_VARIABLESHIELD   1024

	#define GAMEKEY_UP				1
	#define GAMEKEY_DOWN	 		2
	#define GAMEKEY_LEFT	 		4
	#define GAMEKEY_RIGHT			8
	#define GAMEKEY_FIRE	 		16
	#define GAMEKEY_JUMP	 		32
	#define GAMEKEY_EXIT	 		64
	#define GAMEKEY_RAPIDFIRE 		128
	#define GAMEKEY_DOUBLESPEED		512
	#define GAMEKEY_UP2				1024
	#define GAMEKEY_DOWN2	 		2048
	#define GAMEKEY_LEFT2		   4096
	#define GAMEKEY_RIGHT2			8192
	#define SCREENITEM_NULL			0
	#define SCREENITEM_ALIEN		1
	#define SCREENITEM_SHOT			2
	#define SCREENITEM_HERO			3
	#define SCREENITEM_3DCURSOR	4
	#define SCREENITEM_CUBE			5
	extern int iScreenItemTypes;//=6;
	extern string sarrScreenItem_DONTUSEMEDIRECTLY[];// = {"(invalid screenitem:0)","SCREENITEM_ALIEN","SCREENITEM_SHOT","SCREENITEM_HERO","SCREENITEM_3DCURSOR","SCREENITEM_CUBE"};
	#define MAXSHOTS					50
	#define MAX_SCREENITEMS			58  //maximum number of objects to render per frame
	#define MAX_ZORDER		  		50   //maximum zOrder value - INCLUSIVE
	#define FMAX_ZORDER				50.0f
	// MACROS /////////////////////////////////////////////////

	#define ZORDER_FROMY_FORCE(Y) (   (  ( FYRANGE-((Y)+(-FYMIN)) ) / FYRANGE )   *   FMAX_ZORDER   )
	#define ZORDER_FROMY(Y) ( (int) (((Y)<FYMIN) ? MAX_ZORDER : (((Y)>FYMAX)?0:ZORDER_FROMY_FORCE(Y))) )	 //get rendering Z-order from Y-value
	//Build a 32 bit color value in A.8.8.8 format (8-bit alpha mode):
	#define _RGB32BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))
	//24-bit specific, needed because of some video cards:
	#define _RGB24BIT(r,g,b) ((b) + ((g) << 8) + ((r) << 16))
	//audio panning:
	#define L_FROM_FACTOR(factorzerotoone) ((int)((1.0f-factorzerotoone)*255.0f))
	#define R_FROM_FACTOR(factorzerotoone) ((int)(factorzerotoone*255.0f))
	//Get the b,g,r or a of BGRA pixel:
	//#define _BYTEXOFPIXEL(m3dEnt.X,pixel) ((pixel >> 8*4(m3dEnt.X-1)) & 255)

	#define ENTITY_TYPE_HERO  1
	#define ENTITY_TYPE_SHOT  2
	#define ENTITY_TYPE_ALIEN 3
	#define ENTITY_TYPE_BOSS  4

	#define FRADAR_ZOOM			1.0f
	#define FRADAR_ZOOMINNER	1.0f
	#define XPIX_RADAR  		564
	#define YPIX_RADAR			55
	#define RADAR_TOP			0
	#define RADAR_BOTTOM		110
	#define RADAR_LEFT			490
	#define RADAR_RIGHT			639

	typedef struct SCREENITEM_STRUCT {
		int iType;//entity type i.e. SCREENITEM_ALIEN
		int zOrder;
		int index;//entity index, irrelevant for hero etc
		//IPoint ip2dCenter;//usually unused unless custom
		Mass3d m3dCube;//usually unused unless custom
	} SCREENITEM, * LPSCREENITEM;

	///#region functions

	//int SDL_main();

	//int main(int argc, char *argv[]) {
	//int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR
	//	lpCmdLine, unsigned int nCmdShow ) {
	bool GameMain();
	////////////////////////////////////////////////////////////
	bool GameInit();
	/////////////////////////////////////////////////////////////
	bool GameShutdown();

	int Base_GetTicks_Absolute();
	/*inline*/Uint32 Base_GetTicks_Relative();
	void SleepWrapper(int iTicks);

	void LoadSequence(Anim &animToLoad, string sFileBaseNameOnly, int iFramesExpectedElseError);
	void LoadImage(GBuffer* &gbToLoad, string sFile);
	void LoadImage(GBuffer &gbToLoad, string sFile);
	void DirKeyDown();
	void DirKeyUp();
	//////////////////////////////////////////////////
	bool AddScreenItem(int iType, int zOrder, int iEntityIndex);
	bool AddScreenItem(int iType, int zOrder, int iEntityIndex, Mass3d& m3dNow);
	string ScreenItemToString(int iType);
	///////////////////////////////////////////////////////////////
	void QuitSafe();
	////////////////////////////////////////////////////////////////////////////////
	float RadarCenterX();
	float RadarCenterY();
	float RadarHeight();
	void RadarDotAt(int &xPixReturn, int &yPixReturn, float xPos, float yPos);
	void DrawRadarDot(float xPos, float yPos, float zPos, UInt32 u32Pixel);
	void DrawRadarDot(float xPos, float yPos, float zPos, UInt32 u32Pixel, bool bFilled);
	void DrawRadarRect(float left, float top, float bottom, float right, UInt32 u32Pixel, bool bFilled);
	void DrawRadarField();
	////////////////////////////////////////////////////////////////////////////////
	void ShowDebugVars();
	////////////////////////////////////////////////////////////////////
	void DrawPointWithZOffsetIndicator(Mass3d m3dNow, string sDebugMessage);
	void DrawPointWithGroundSquareIndicator(Mass3d m3dNow, Pixel color, string sDebugMessage);
	void MoveSoundSource(string SourceName, Mass3d& thisMass3d);
	string GetOrCreateSoundInstanceName(string SoundName, string SourceName);
	void PlaySound(string SoundName, string SourceName);
	void PlaySound(string SoundName, string SourceName, Mass3d& thisMass3d);
	void PlaySound(string SoundName, string SourceName, Mass3d& thisMass3d, bool IsLooped);
	//void PlaySound(string sInstanceName, string sSoundName, Mass3d& m3dLoc, Mass3d& m3dVel);
	void StopSoundInstance(string SoundName, string SourceName);
	////////////////////////////////////////////////////////////////////
	void Refresh3dCursor();
	void Draw3dCursor(byte byBrightness);
	void SayWhatIDrewIfFalse(bool bDrawFunctionReturn, int iEntityType, string sDescription);
	string EntityTypeToString(int iEntityType);
	bool DrawScreen();
	string GameStateToString(int iGameStateX);
	bool DoEvents();
	void SleepAllowingEvents(int delay_ms);
	void DoCollision(Entity* bumpee, Entity* bumper, int shot_index);
	///#endregion functions

	extern string sarrGameState[];// = {"GAMESTATE_INIT","GAMESTATE_START_AREA","GAMESTATE_START_ENCOUNTER","GAMESTATE_RUN","GAMESTATE_SHUTDOWN","GAMESTATE_EXIT","GAMESTATE_WIN_ENCOUNTER","GAMESTATE_WIN_GAME","GAMESTATE_YOU_LOSE","GAMESTATE_LOSEGUY"};
	extern long narrGameStateCount[];// = {0,0,0,0,0,0,0,0,0,0};

	///region globals defined in h file
	//extern int iLastIntersectionType;
	extern int iLastLineRelationshipType;
	extern double HITDETECTION2D_CONSTANT_Z_DBL;
	extern GFont gfontDefault;
	extern bool bDoneShutdown;
	extern bool bShuttingDown;
	extern string sVerb_Engine;
	extern UInt32 u32Pressing;
	//DEBUG: ZBUFFER_YRATIO MUST be 50/YRANGE
	extern int iScreenItems;
	extern bool bInMenu;
	extern Uint32 dwTicksAcquiredOutsideOfGameState_Run;
	extern Uint32 dwAbsoluteTickLastGameStateState_Run;
	extern bool bShowOnce_TargaToTarga32_ANY_Debug;
	extern FPOINT3D p3dHero;
	//extern bool bDebug
	extern long iMainEventLoopCount;//counts iterations of main event loop
	extern Mass3d m3dTemp;
	extern Mass3d m3darrDebug[9];
	extern Mass3d m3dHeroEye, m3dCursor;
	//extern float xAimFlipper;
	//extern float yAimFlipper;
	//extern float zAimFlipper;
	//extern bool bFlipYaw;
	//extern bool bFlipPitch;
	extern float fBoss;//arrpentAlien[iBoss]->fHealth;
	extern float fBossMax;//arrpentAlien[iBoss]->fMaxHealth;
	extern int iBoss;//global link to boss;
	extern bool bSDLQuitDone;
	extern Variables settings;
	extern Variables dat;
	extern bool bSDLQuitSent;
	extern bool bSplash;//if splash screen was drawn
	extern int iSplashTick;//time when splash screen appeared
	//now in RImage_bgra32.h extern BYTE by2d_ByteAsByte_times_ByteAsFloatLookup[256][256];//lookup multiplication result of byte-as-byte times byte-as-decimal-zero-to-one
	extern UInt32 arru32EnergyGrad[256];//gradient for energy currency (ready attack) meter
	extern UInt32 arru32HealthGrad[256];//gradient for health meter
	extern int xCursor, yCursor, xCursorDown, yCursorDown;
	extern bool bMouseDown;
	extern bool bMouseMove;
	extern bool bMouseDownR;
	extern float xfCursor,yfCursor,xfCursorDown,yfCursorDown;

	extern int xCursorCenter, yCursorCenter;
	extern int iGuysStart;
	extern int iGuys;

	extern int iAreas,
		iBackdrops,
		iFramesCursor,
		iEncounters;

	extern int iFramesHero,
		iFramesHeroShadow,
		iFramesFlyer,
		iFramesFlyerShadow,
		iFramesBoss,
		iFramesBossShadow,
		iFramesShot,
		iFramesShotShadow,
		iFramesBurn,
		iFramesExpArt,
		iFramesGameScreen;
		//TODO: actually use these instead of hard-coding frames
						//may be okay as long as they're reset to iFoundFrames upon TargaLoadSeq
	extern int iEscapeTime;
	extern int ix;//temp var for error#'s
	extern int iTickAbsoluteEscapeLastPress;
	extern int iErrorsSaved;
	extern int iFramesDropped;
	extern int REAL_BPP;
	extern int REAL_BYTEDEPTH;
	extern int keyDelay;
	extern int iDoubleCodeCounter; //player cheat code
	extern int iRapidFireCodeCounter;//player cheat code
	extern bool bBombed;
	extern Anim		animBackdrop;
	extern GBuffer*	screen_ptr;
	extern GBuffer	gbIntro;
	extern GBuffer	gbIntroTextBack;
	extern GBuffer	gbIntroText;
	extern GBuffer*	shield_symbol_image_ptr;
	extern GBuffer*	boss_health_symbol_image_ptr;
	extern GBuffer	gbLives;
	extern Anim		animHero;
	extern Anim		animHeroShadow;
	extern Anim		animFlyer;
	extern Anim		animFlyerShadow;
	extern Anim		animBoss;
	extern Anim		animBossShadow;
	extern Anim		animShot;
	extern Anim		animShotShadow;
	extern Anim		animBurn;
	extern Anim		animExpArt;
	extern Anim		animGameScreen;
	extern Anim		animGameScreenArea;
	extern Anim		animGameScreenEncounter;
	extern Anim		animCursor;
	//extern LPSPRITE*		lpspritearr;//sprite array
	extern SCREENITEM		screenitemarr[MAX_SCREENITEMS];
	//extern char sTemp[800];								// general printing buffer
	extern int explosionResult;
	extern int iGameState;
	extern int iGameStateWas;
	extern int iArea;		//reset later in GAMESTATE_INIT			//3 Areas total
	extern int iEncounter; //set to 1 again later in GAMESTATE_INIT					//3 Encounters total, encounters are in Areas
	extern int iAliens;
	extern int iMaxAliens;//changed later using ini file
	extern int iMaxAliensNow;//changes by level


	extern Camera camera;
	extern Entity* arrpentShot[MAXSHOTS];
	extern std::vector<Entity*> arrpentAlien;//extern Entity** arrpentAlien;
	extern Entity* hero;
	//TODO: improve boss rect alignment
	////////////////////////////////////////////////////////////////////////////////
	/* The screen surface */
	//extern SDL_Surface *screen;
	extern int iDirKeysDown;
	extern bool bDone;
	extern bool bFirstRunOfCurrentState;
	///#endregion globals in h file

}//end namespace
#endif
