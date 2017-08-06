// Gameboy Basic.cpp : Defines the entry point for the console application.
// ob

//#include "stdafx.h" // Windows only
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <bitset>
#include <string>
#include <sstream>
#include <SDL_ttf.h>
//#include <SDL.h> //for windows
#include <SDL2/SDL.h> //for android
#include <random>
#include "buttonHandler.h"

//Modules
//MMUEx - Started Structure - VramMapping works
//CPU -- Function mapping and Opcode mapping - Current TASK
//Timers -- 50% Need to actually update them
//Interrupts - 0%
//FlagHandling - DONE 100%
//Register combining - DONE 100%
//Gfx - Researching SDL Structure Groundwork ready
//Sound - 0%
//Input - 0%

//SDL support

//SDL_Renderer* renderer = NULL;

int min;
int max;

std::string ROM = "tetris.gb";
std::string BIOS ="DMG_ROM.bin";

unsigned char  memoryT[0xFFFF];
unsigned char memoryA[0xFFFF];
char letter;
unsigned char addr;
long opcode;
unsigned char opLen;
std::string functType;
unsigned short $aabb;
unsigned short memCatch;
bool nullRender = true;
std::string Dm;
std::string Dreg;
std::string Sreg;
std::string Starget;
std::string Dtarget;

std::stringstream funcText;
std::stringstream memValue;
std::string instamem;

/// DEBUG MESSAGE COMPARE VARIABLES
std::string pastDms[10];
std::string currentDms[10];
bool captureDm = true;
long opCounter;
long loopInstances;
bool matchFound = true;

bool loopDebugger = false;
bool debugMode = true;
static bool running = true;
bool loopDetected = false;
int numberMatchs;

/// DEBUG MESSAGE COMPARE VARIABLES

//SDL_TTF Handles
bool fontLoaded = false;

//VRAM PEEKER
char Vram[0x2000];
int vramCounter = 0;
char vramGfx[8281];
SDL_Rect pixelVram[8192];
SDL_Rect r;
int size = 4;
int numCol = 0;
int numRow = 0;


unsigned short _16bitIn;
unsigned long fullOpcode;
unsigned char $bb;
unsigned char $aa;
signed int $xx;
unsigned char _8bitIn3;

unsigned char TempRf;
///////////// - SDL debugger

// Position integers

int textX;
int textY;
int texW = 1;
int texH = 2;

// SDL Textures

SDL_Texture *texture[16];
SDL_Texture *blitTexture;

// SDL Surfaces

SDL_Surface *blitSurface;
SDL_Surface *surface[16];
SDL_Surface *clearRect;

SDL_Rect posTextreg1;
std::string line = "Debug line 1";


TTF_Font *font1;

	

////////////

class gb
{
public:


//CPU Registers
	unsigned short Raf, Rbc, Rde, Rhl;
	unsigned char Ra, Rf, Rb, Rc, Rd, Re, Rl, Rh;
	unsigned short pc, sp;
	unsigned char stack[0xFFFF];
	
	//new cpu registers structure
	
	
	struct reg {
	struct  {
		union {
			struct {
				unsigned char Rf;
				unsigned char Ra;
			};
			unsigned short Raf;
		};
	};
	
	struct  {
		union {
			struct {
				unsigned char Rc;
				unsigned char Rb;
			};
			unsigned short Rbc;
		};
	};
	
	struct  {
		union {
			struct {
				unsigned char Re;
				unsigned char Rd;
			};
			unsigned short Rde;
		};
	};
	
	struct  {
		union {
			struct {
				unsigned char Rl;
				unsigned char Rh;
			};
			unsigned short Rhl;
		};
	};
	
};
	
	reg registers;
	////
	

//I-O Registers
	unsigned char FF00_P1; //Key input R/W bit 0-5
	unsigned char FF01_SB; //Serial transfer R/W -8 Bt
	unsigned char FF02_SC; //SIO control R/W bits 7 and 0
	unsigned char FF04_DIV;//DIV R/W
	unsigned char FF05_TIMA; //TIMA Counter R/W
	unsigned char FF06_TMA; //Timer Modulo R/W
	unsigned char FF07_TAC; //Timer Control R/W
	unsigned char FF0F_IF; // Interrupt Flag R/W
	//Interrupts
	// V-Blank, LCDC Status, Timer Overflow, Serial Transfer, Hi-Lo P10-P13
	unsigned char FF10_NR10; //Sound Mode 1 Register R/W
	unsigned char FF11_NR11; //NR11
	unsigned char FF12_NR12;
	unsigned char FF13_NR13;
	unsigned char FF14_NR14;
	unsigned char FF16_NR21;
	unsigned char FF17_NR22;
	unsigned char FF18_NR23;
	unsigned char FF19_NR24;
	unsigned char FF1A_NR30;
	unsigned char FF1B_NR31;
	unsigned char FF1C_NR32;
	unsigned char FF1D_NR33;
	unsigned char FF1E_NR34;
	unsigned char FF20_NR41;
	unsigned char FF21_NR42;
	unsigned char FF22_NR43;
	unsigned char FF23_NR44;
	unsigned char FF24_NR50;
	unsigned char FF25_NR51;
	unsigned char FF26_NR52;
	unsigned char FF30_WPR, FF31_WPR, FF32_WPR, FF33_WPR, FF34_WPR, FF35_WPR, FF36_WPR, FF37_WPR, FF38_WPR, FF39_WPR, FF3A_WPR, FF3B_WPR, FF3C_WPR, FF3D_WPR, FF3E_WPR, FF3F_WPR;
	unsigned char FF40_LCDC; // LCDC
	unsigned char FF41_STAT; // STAT
	unsigned char FF42_SCY; //SCY
	unsigned char FF43_SCX; //SCX
	unsigned char FF44_LY; //Vertical line for coordinate Y
	unsigned char FF45_LYC; //Compare LY
	unsigned char FF46_DMA; //DMA transfer
	unsigned char FF47_BGP; //Window palette DATA
	unsigned char FF48_OBP0; //Object Pallete 0 Data
	unsigned char FF49_OBP1; //Object pallete 1 Data
	unsigned char FF4A_WY; //Windows Y position
	unsigned char FF4B_WX; //Windows X position
	unsigned char FFFF_IE; //Interrupt Enable
	
	unsigned char colorNum[4];
	unsigned char palleteColor[4];
	unsigned char color[4];

	unsigned char opclock = 0;
	unsigned long cpuClock = 0;
	unsigned char mathTC1 = 0;
	unsigned char mathTC2 = 0;
	unsigned char mathTS1 = 0;
	unsigned char mathTS2 = 0;
	
	bool whiteC = false;
	bool lightGreyC = false;
	bool darkGreyC = false;
	bool blackC = false;
	
	std::string prevM;
	std::string actualM;
	
std::string debugSDL;
std::string mergedDebug;

	unsigned short IF;
	bool DInt = false;
	bool EInt = true;
	bool memCatchFlag = false;

	unsigned char TIMA, TMA, TAC, NR10, NR11, NR12, NR14, NR21, NR22, NR24, NR30, NR31, NR32, NR33, NR41, NR42, NR43, NR50, NR51, NR52, LCDC, SCY, SCX, LYC, BGP, OBP0, OBP1, WY, WX ;


void TextBlit(std::string debugLine, int posX, int posY)
	{
if ( debugMode == true )
{
		// tContainer = std::string("Prueba") + std::string(" Text on demand
		// ");

/*if (fontLoaded)
{
		font1 = TTF_OpenFont("arial.ttf", 40);
		fontLoaded = false;
		if (font1 == NULL)
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Font not found", NULL);
		}
}
*/

		SDL_Color color1 = { 0, 255, 0 };

		// surface donde se cargara el texto(string)

		blitSurface = TTF_RenderText_Solid(font1, debugLine.c_str(), color1);


		// textura convertida desde el surface
		blitTexture = SDL_CreateTextureFromSurface(renderer, blitSurface);
		SDL_CreateTextureFromSurface(renderer, blitSurface);
		// Hacer calzar la textura mostrada dentro de un rango
		SDL_QueryTexture(blitTexture, NULL, NULL, &texW, &texH);
		// declarar un rect contenedor de la textura

		posTextreg1.x = posX;
		posTextreg1.y = posY;
		posTextreg1.w = texW;
		posTextreg1.h = texH;

		// copiar al render la textura en la posicion
		SDL_RenderCopy(renderer, blitTexture, NULL, &posTextreg1);
		// listo para renderiza

		SDL_FreeSurface(blitSurface);
		SDL_DestroyTexture(blitTexture);
	//	TTF_CloseFont(font1);

	}
	}


	unsigned char Z,N,H,C;
	float Rm, Rt;
///////////////////////////////////////////////////////////////////////////////////////
//- Graphics Related code 
//- This section can be enabled or disabled, but is meant for research
///////////////////////////////////////////////////////////////////////////////////////		
	SDL_Rect pixelVram[8192]; // la pantalla es de 256 x 256 pero 144 y 164 son visibles
	
	
	void Vram2() {
		numCol = 0;
		numRow = 0;

		r.h = size;
		r.w = size;
			
		for (int i = 0; i <8192; i++) {
			
			 r.x = numCol * (size + 1);
			r.y = numRow * (size + 1);
			
			
			if ( numCol % 91  == 0 && numCol != 0 ) {

			numRow ++;
			numCol = 0;
		}
		

		else
		{
		numCol++;
		}
		
		if ( blackC ) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	   	
			blackC = false;
		}
		
		else if (lightGreyC) {
			SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
	   	
			lightGreyC = false;
		}
		
		else if (darkGreyC) {
			SDL_SetRenderDrawColor(renderer, 96, 96, 96, 255);
	   	
			darkGreyC = false;
		}
		else if (whiteC) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			whiteC = false;
		}
		else { SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); }
		
			
			SDL_RenderFillRect(renderer, &r);
			
			//std::cout << " i: " << i;
			//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Vram Peek", "91", NULL);
			
			
		}
		
		//SDL_RenderPresent(renderer);
	} //fin Vram2
	
		

	void VRAMpeek() {
		
		//digamos que tengo 0x500 posisiones de me

			unsigned int rowN;
			unsigned int colN;
			int yCont = 0;
			for (int j = 0; j <= 91; j++)
			{
				for (int i = 0; i <= 91; i++)
				{

					pixelVram[i].w = 2;
					pixelVram[i].h = 2;
					pixelVram[i].x = (2 * i) + (i * 2);
					pixelVram[i].y = (2 * j) + (j * 2);
					
					
//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

					if (memoryA[vramCounter] == 0 )
					{
						SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
					}
					else { SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); }
					
					SDL_RenderFillRect(renderer, &pixelVram[i]);
					//SDL_Delay(100);
					
					
				} //fin i
				

			}  //fin j
			//SDL_RenderPresent(renderer);
			/*
					pixel[20][i].h = 5;
					pixel[20][i].w = 5;
					pixel[20][i].x = 140 + (i * 5);
					pixel[20][i].y = 200 + (j * 5);
		
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
					SDL_RenderFillRect(renderer, &pixel[j][i]);

				SDL_RenderPresent(renderer);
				*/

	} // End VRAMpeek
	
	unsigned long gpuClock =0;
	unsigned int modeClock =0;
	int gpuMode =0 ;
	SDL_Rect pixel[144][164]; // la pantalla es de 256 x 256 pero 144 y 164 son visibles
	void gfxHandler()
	{
		unsigned int rowN;
		unsigned int colN;
		
		gpuClock += opclock;
		rowN = 0;
		//cpu clock and times accesing Vram and OAM
		//scanline access OAM - gpuMode 2 - 80 cycles
		//scanline access VRAM - gpuMode 3 - 172 cyc
		//horizontal blank - gpuMode 0 - 204 cycles
		//one line scan and blank - 456 cycles
		//vertical blank - gpuamode 1 - 4560 10lines
		//full frame 70224
		//	144 * 456 + (14 * 4560)?? - 204
		

		switch (gpuMode) {
		case 2: //access OAM for scanline read mode
			if (gpuClock == 80) {
				gpuClock = 0;
				gpuMode = 3;
				//semi proceso 1/2
			}
			break;

		case 3: //access VRAM for scanline read mode
			if (gpuClock == 172) {
				gpuClock = 0;
				gpuMode = 0;
				//fin semi proceso 2/2
			}
			break;

		case 0: // horizontal blanking
			if (gpuClock == 204) {
				//--scanea para regresar al punyo de partida
				//pasa la info al framebuffer

				gpuClock = 0;
				rowN++;
				if (rowN == 145) {
					gpuMode = 1;
					rowN = 0;
				}
				else
					gpuMode = 2;

			}
			break;

		case 1: //vertical blanking
			if (gpuMode == 4560) {
				//grab all lines from fb and place them on the screen
				cpuClock = 0;
				gpuClock = 0;
				gpuMode = 2;
				//ready for another frame

			}
			break;


		}


		//framebuffer shennanigans
		//mem 8000-87FF TS1 0-127
		//mem 8800-8FFF TS1 128-255 TS0 -1 a -127
		//mem 9000-97FF TS0 0-127
		//mem 9800-9BFF TM0
		//mem 9C00-9FFF TM1

		//tile system 8x8 dibujos de 64pixeles
		//tile map 32x32 fondos de 256 scrolleables
		//8x16 dice gbcuman

		//unique tiles in mem 256
		//total tiles 384

		//paletas controlado por el background pallete GPU register


		// valor       color
		//0                255,255,255
		//1                192,192,192
		//2                96,96,96
		//3                 0,0,0

		//	7 6 | 5 4 | 3 2 | 1 0
		//     3      2    1    0

		//acceso a paleta, orden y preferencia


		//for (int z = 0; z<10; z++)	{
		for (int j = 0; j<145; j++)
		{
			for (int i = 0; i<165; i++) {
				pixel[j][i].h = 5;
				pixel[j][i].w = 5;
				pixel[j][i].x = 140 + (i * 5);
				pixel[j][i].y = 200 + (j * 5);


				//randomGen(0,255);
				int cr, cg, cb;

				/*
				//way of selecting RED
				if ((rand() % 100) == 0){
				cr = 96;
				cg = 96;
				cb = 96;
				}
				//		 //way of selecting GREEN
				//		 else if ((rand() % 100 == 1)){
				//		 	cr=0;
				//		 	cg=255;
				//		 	cb=0;
				//		 }
				//way of selecting BLUE
				else if ((rand() % 100 == 2)) {
				cr = 0;
				cg = 0;
				cb = 0;
				}
				//way of selecting Black
				else if ((rand() % 100 == 4)) {
				cr = 192;
				cg = 192;
				cb = 192;
				}
				//way of selecting white
				else if ((rand() % 100 == 5)) {
				cr = 255;
				cg = 255;
				cb = 255;
				}
				*/

				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				SDL_RenderFillRect(renderer, &pixel[j][i]);
				//SDL_RenderPresent(renderer);
				//SDL_Delay(5);

			}

		}
		//SDL_RenderPresent(renderer);
		//}//Video support
	}//gfxHandler

	// reemplazar desde afuera de GFX handler
	

///////////////////////////////////////////////////////////////////////////////////////
//- Im using the MMU as a BP for when the CPU is mature enough to start accesing VRAM 
//- This will be a Major milestone in the project
///////////////////////////////////////////////////////////////////////////////////////
	void MMUexp(unsigned short toTarget, unsigned short fromSource, std::string sourceReg, int MMUopType) {

		switch (MMUopType)
		{
		case 1: // Read 8 bit from memory (I need a source[Mem] and a Target[Reg])
			if (fromSource < 0x4000) { std::cout << " Reading 8bit from ROM bank0" << "\n"; }
			else if (fromSource >= 0x4000 && fromSource < 0x8000) { std::cout << " Reading 8bit from ROM bank1" << "\n"; }
			else if (fromSource >= 0x8000 && fromSource < 0xA000) { std::cout << " Reading 8bit from GFX" << "\n"; }		
			else if (fromSource >= 0xA000 && fromSource < 0xC000) { std::cout << " Reading 8bit from EXT RAM bank" << "\n"; }
			else if (fromSource >= 0xC000 && fromSource < 0xE000) { std::cout << " Reading 8bit from WORKING RAM" << "\n"; }
			else if (fromSource >= 0xE000 && fromSource < 0xFE00) { std::cout << " Reading 8bit from WORKING RAM SHADOW" << "\n"; }
			else if (fromSource >= 0xFE00 && fromSource < 0xFF00) { std::cout << " Reading 8bit from GFX SPRITES" << "\n"; }
			else if (fromSource >= 0xFF00 && fromSource < 0xFF80) { std::cout << " Reading 8bit from INPUT" << "\n"; 
			
			switch (fromSource & 0xFFFF)
			{
			case 0xFF00: std::cout << "- trying to read input \n"; break;
			case 0xFF01: std::cout << "- trying to read Serial transfer state \n"; break;
			case 0xFF03: std::cout << "- trying to read SIO control state \n"; break;
			case 0xFF04: std::cout << "- trying to read Divider \n"; break;
			case 0xFF05: std::cout << "- trying to read Tima Counter \n"; break;
			case 0xFF06: std::cout << "- trying to read TMA \n"; break;
			case 0xFF07: std::cout << "- trying to read TAC \n"; break;
			case 0xFF0F: std::cout << "- trying to read Interrupt Flag \n"; break;
			case 0xFF10: std::cout << "- trying to read Sound Mode 1 \n"; break;
			case 0xFF11: std::cout << "- trying to read NR11 \n"; break;
			case 0xFF12: std::cout << "- trying to read NR12 \n"; break;
			case 0xFF13: std::cout << "- trying to read NR13 \n"; break;
			case 0xFF14: std::cout << "- trying to read NR14 \n"; break;
			case 0xFF16: std::cout << "- trying to read NR21 \n"; break;
			case 0xFF17: std::cout << "- trying to read NR22 \n"; break;
			case 0xFF18: std::cout << "- trying to read NR23 \n"; break;
			case 0xFF19: std::cout << "- trying to read NR24 \n"; break;
			case 0xFF1A: std::cout << "- trying to read NR30 \n"; break;
			case 0xFF1B: std::cout << "- trying to read NR31 \n"; break;
			case 0xFF1C: std::cout << "- trying to read NR32 \n"; break;
			case 0xFF1D: std::cout << "- trying to read NR33 \n"; break;
			case 0xFF1E: std::cout << "- trying to read NR34 \n"; break;
			case 0xFF20: std::cout << "- trying to read NR41 \n"; break;
			case 0xFF21: std::cout << "- trying to read NR42 \n"; break;
			case 0xFF22: std::cout << "- trying to read NR43 \n"; break;
			case 0xFF23: std::cout << "- trying to read NR44 \n"; break;
			case 0xFF24: std::cout << "- trying to read NR50 \n"; break;
			case 0xFF25: std::cout << "- trying to read NR51 \n"; break;
			case 0xFF26: std::cout << "- trying to read NR52 \n"; break;
			case 0xFF30: std::cout << "- trying to read WPR0 \n"; break;
			case 0xFF31: std::cout << "- trying to read WPR1 \n"; break;
			case 0xFF32: std::cout << "- trying to read WPR2 \n"; break;
			case 0xFF33: std::cout << "- trying to read WPR3 \n"; break;
			case 0xFF34: std::cout << "- trying to read WPR4 \n"; break;
			case 0xFF35: std::cout << "- trying to read WPR5 \n"; break;
			case 0xFF36: std::cout << "- trying to read WPR6 \n"; break;
			case 0xFF37: std::cout << "- trying to read WPR7 \n"; break;
			case 0xFF38: std::cout << "- trying to read WPR8 \n"; break;
			case 0xFF39: std::cout << "- trying to read WPR9 \n"; break;
			case 0xFF3A: std::cout << "- trying to read WPRA \n"; break;
			case 0xFF3B: std::cout << "- trying to read WPRB \n"; break;
			case 0xFF3C: std::cout << "- trying to read WPRC \n"; break;
			case 0xFF3D: std::cout << "- trying to read WPRD \n"; break;
			case 0xFF3E: std::cout << "- trying to read WPRE \n"; break;
			case 0xFF3F: std::cout << "- trying to read WPRF \n"; break;
			case 0xFF40: std::cout << "- trying to read LCDC \n"; break;
			case 0xFF41: std::cout << "- trying to read STAT \n"; break;
			case 0xFF42: std::cout << "- trying to read SCY \n"; break;
			case 0xFF43: std::cout << "- trying to read SCX \n"; break;
			case 0xFF44: std::cout << "- trying to read LY \n"; break;
			case 0xFF45: std::cout << "- trying to read LYC \n"; break;
			case 0xFF46: std::cout << "- trying to read DMA \n"; break;
			case 0xFF47: std::cout << "- trying to read Window Pallete Data \n"; break;
			case 0xFF48: std::cout << "- trying to read Object Pallete 0 Data \n"; break;
			case 0xFF49: std::cout << "- trying to read Object Pallete 1 Data \n"; break;
			case 0xFF4A: std::cout << "- trying to read Windows Pos Y \n"; break;
			case 0xFF4B: std::cout << "- trying to read Windows Pos X \n"; break;

			
			}

			}
			else if (fromSource >= 0xFF80 && fromSource < 0xFFFF) { std::cout << " Reading 8bit from ZERO PAGE RAM" << "\n";
			switch (fromSource & 0xFFFF) {
			case 0xFFFF: std::cout << "- trying to read Interrupt Enable State \n"; break;
			}
			
			}
			break;
		

	case 2: // Write 8 bit to memory (I need a source[Reg] and a Target[Mem])
		if (toTarget < 0x4000) { std::cout << " Writing 8bit from ROM bank0" << "\n"; }
		else if (toTarget >= 0x4000 && toTarget < 0x8000) { std::cout << " Writing 8bit to ROM bank1" << "\n"; }
		else if (toTarget >= 0x8000 && toTarget < 0xA000) { //std::cout << " Writing 8bit to GFX" << "\n"; 
		vramGfx[toTarget - 0x8000] = fromSource;
		}
		else if (toTarget >= 0xA000 && toTarget < 0xC000) { std::cout << " Writing 8bit to EXT RAM bank" << "\n"; }
		else if (toTarget >= 0xC000 && toTarget < 0xE000) { std::cout << " Writing 8bit to WORKING RAM" << "\n"; }
		else if (toTarget >= 0xE000 && toTarget < 0xFE00) { std::cout << " Writing 8bit to WORKING RAM SHADOW" << "\n"; }
		else if (toTarget >= 0xFE00 && toTarget < 0xFF00) { std::cout << " Writing 8bit to GFX SPRITES" << "\n"; }
		else if (toTarget >= 0xFF00 && toTarget < 0xFF80) { std::cout << " Writing 8bit to INPUT" << "\n"; 
		
		switch (toTarget & 0xFFFF)
		{
		case 0xFF00: std::cout << "- trying to write input \n"; break;
		case 0xFF01: std::cout << "- trying to write Serial transfer state \n"; break;
		case 0xFF03: std::cout << "- trying to write SIO control state \n"; break;
		case 0xFF04: std::cout << "- trying to write Divider \n"; break;
		case 0xFF05: std::cout << "- trying to write Tima Counter \n"; break;
		case 0xFF06: std::cout << "- trying to write TMA \n"; break;
		case 0xFF07: std::cout << "- trying to write TAC \n"; break;
		case 0xFF0F: std::cout << "- trying to write Interrupt Flag \n"; break;
		case 0xFF10: std::cout << "- trying to write Sound Mode 1 \n"; break;
		case 0xFF11: std::cout << "- trying to write NR11 \n"; break;
		case 0xFF12: std::cout << "- trying to write NR12 \n"; break;
		case 0xFF13: std::cout << "- trying to write NR13 \n"; break;
		case 0xFF14: std::cout << "- trying to write NR14 \n"; break;
		case 0xFF16: std::cout << "- trying to write NR21 \n"; break;
		case 0xFF17: std::cout << "- trying to write NR22 \n"; break;
		case 0xFF18: std::cout << "- trying to write NR23 \n"; break;
		case 0xFF19: std::cout << "- trying to write NR24 \n"; break;
		case 0xFF1A: std::cout << "- trying to write NR30 \n"; break;
		case 0xFF1B: std::cout << "- trying to write NR31 \n"; break;
		case 0xFF1C: std::cout << "- trying to write NR32 \n"; break;
		case 0xFF1D: std::cout << "- trying to write NR33 \n"; break;
		case 0xFF1E: std::cout << "- trying to write NR34 \n"; break;
		case 0xFF20: std::cout << "- trying to write NR41 \n"; break;
		case 0xFF21: std::cout << "- trying to write NR42 \n"; break;
		case 0xFF22: std::cout << "- trying to write NR43 \n"; break;
		case 0xFF23: std::cout << "- trying to write NR44 \n"; break;
		case 0xFF24: std::cout << "- trying to write NR50 \n"; break;
		case 0xFF25: std::cout << "- trying to write NR51 \n"; break;
		case 0xFF26: std::cout << "- trying to write NR52 \n"; break;
		case 0xFF30: std::cout << "- trying to write WPR0 \n"; break;
		case 0xFF31: std::cout << "- trying to write WPR1 \n"; break;
		case 0xFF32: std::cout << "- trying to write WPR2 \n"; break;
		case 0xFF33: std::cout << "- trying to write WPR3 \n"; break;
		case 0xFF34: std::cout << "- trying to write WPR4 \n"; break;
		case 0xFF35: std::cout << "- trying to write WPR5 \n"; break;
		case 0xFF36: std::cout << "- trying to write WPR6 \n"; break;
		case 0xFF37: std::cout << "- trying to write WPR7 \n"; break;
		case 0xFF38: std::cout << "- trying to write WPR8 \n"; break;
		case 0xFF39: std::cout << "- trying to write WPR9 \n"; break;
		case 0xFF3A: std::cout << "- trying to write WPRA \n"; break;
		case 0xFF3B: std::cout << "- trying to write WPRB \n"; break;
		case 0xFF3C: std::cout << "- trying to write WPRC \n"; break;
		case 0xFF3D: std::cout << "- trying to write WPRD \n"; break;
		case 0xFF3E: std::cout << "- trying to write WPRE \n"; break;
		case 0xFF3F: std::cout << "- trying to write WPRF \n"; break;
		case 0xFF40: std::cout << "- trying to write LCDC \n"; break;
		case 0xFF41: std::cout << "- trying to write STAT \n"; break;
		case 0xFF42: std::cout << "- trying to write SCY \n"; break;
		case 0xFF43: std::cout << "- trying to write SCX \n"; break;
		case 0xFF44: std::cout << "- trying to write LY \n"; break;
		case 0xFF45: std::cout << "- trying to write LYC \n"; break;
		case 0xFF46: std::cout << "- trying to write DMA \n"; break;
		case 0xFF47: { std::cout << "- trying to write Window / BG Pallete Data \n";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Debug M","Writing Pallete Data (BG)", NULL);
	   FF47_BGP = memoryA[0xFF00 + Rc];
	   std::bitset<8> FF47bits( FF47_BGP);//Window
	  // palette DATA converted to bits
	  std::bitset<8> pair0();
	  std::bitset<8> pair1();
	  std::bitset<8> pair2();
	  std::bitset<8> pair3();
	  
	  int pair00;
	  int pair01;
	  int pair10;
	  int pair11;
	  int pair20;
	  int pair21;
	  int pair30;
	  int pair31;
	  
	  
	  // The register can hold 4 colors per call
	  // numered from 0 to 3
	  
	  pair00= FF47bits[0];
	  pair01= FF47bits[1];
	  
	  palleteColor[0] = ( pair00 << 1) | pair01;
	  
	  pair10 = FF47bits[2];
	  pair11 = FF47bits[3];
	  
	  palleteColor[1] = ( pair10 << 1) | pair11;
	  
	  pair20 = FF47bits[4];
	  pair21 = FF47bits[5];
	  
	  palleteColor[2] = ( pair20 << 1) | pair21;
	  
	  pair30 = FF47bits[6];
	  pair31 = FF47bits[7];
	  
	  palleteColor[3] = ( pair30 << 1) | pair31;
	  
	 //palleteColor[0] = pair0.to_ulong();
	  //palleteColor[1] = pair1.to_ulong();
	  //palleteColor[2] = pair2.to_ulong();
	  //palleteColor[3] = pair3.to_ulong();

	  
	  //color0 of pallete
	  switch (palleteColor[0]) {
	  	case 0:
	  	color[0] = SDL_SetRenderDrawColor(renderer, 255,255,255, 255);
	  	break;
	  	
	  	case 1:
	  	color[0] = SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
	  	break;
	  	
	  	case 2:
	  	color[0] = SDL_SetRenderDrawColor(renderer, 96, 96, 96, 255);
	  	break;
	  	
	  	case 3:
	  	color[0] = SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	  	break;
	  }
	  
	  //color 1 of pallete
	  
	  switch (palleteColor[1]) {
	  	case 0:
	  	color[1] = SDL_SetRenderDrawColor(renderer, 255,255,255, 255);
	  	break;
	  	
	  	case 1:
	  	color[1] = SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
	  	break;
	  	
	  	case 2:
	  	color[1] = SDL_SetRenderDrawColor(renderer, 96, 96, 96, 255);
	  	break;
	  	
	  	case 3:
	  	color[1] = SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	  	break;
	  }
	  
	  //color 2 of pallete
	  
	  switch (palleteColor[2]) {
	  	case 0:
	  	color[2] = SDL_SetRenderDrawColor(renderer, 255,255,255, 255);
	  	break;
	  	
	  	case 1:
	  	color[2] = SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
	  	break;
	  	
	  	case 2:
	  	color[2] = SDL_SetRenderDrawColor(renderer, 96, 96, 96, 255);
	  	break;
	  	
	  	case 3:
	  	color[2] = SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	  	break;
	  }
	  //color 3 of pallete
	  
	  switch (palleteColor[3]) {
	  	case 0:
	  	color[3] = SDL_SetRenderDrawColor(renderer, 255,255,255, 255);
	  	break;
	  	
	  	case 1:
	  	color[3] = SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
	  	break;
	  	
	  	case 2:
	  	color[3] = SDL_SetRenderDrawColor(renderer, 96, 96, 96, 255);
	  	break;
	  	
	  	case 3:
	  	color[3] = SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	  	break;
	  }
	  
	  
	  
	  
	  /*
	   if (FF47bits.test(0) == 0 && FF47bits.test(1) == 0 ) {
	   	whiteC = true;
	   	// neutral white 255 255 255 bits 0 0
	   	
	   }
	   else if  (FF47bits.test(2) == 1 && FF47bits.test(3) == 0 ) {
	   	lightGreyC = true;
	   	//lightest gray 192 192 192 bits  1 0
	   	
	   }
	   else if (FF47bits.test(4) == 0 && FF47bits.test(5) == 1) {
	   	//darkest gray 96 96 96 bits 0 1
	   	darkGreyC = true;
	   	
	   }
	   else if (FF47bits.test(6) == 1 && FF47bits.test(7) == 1 ) {
	   	//neutral black 0 0 0 bits 1 1
	   	blackC = true;
	   }
		}
		*/
		}
		 break;
		 
	case 0xFF48: { std::cout << "- trying to write Object Pallete 0 Data\n";
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "I/O","Writing Object 0 Pallet Data", NULL);
		std::bitset<8> FF48bits(FF48_OBP0); //Object Pallete 0 Data
	}
		break;
		
		case 0xFF49: { std::cout << "- trying to write Object Pallete 1 Data \n";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"I/O", "Writing Object 1 Pallet Data", NULL);
	//std::bitset<8> FF49bits(FF49_OBP1); //Object pallete 1 Data
		}
		  break;
		case 0xFF4A: std::cout << "- trying to write Windows Pos Y \n"; break;
		case 0xFF4B: std::cout << "- trying to write Windows Pos X \n"; break;
		}

		}
		else if (toTarget >= 0xFF80 && toTarget < 0xFFFF) { std::cout << " Writing 8bit to ZERO PAGE RAM" << "\n";
		
		switch (toTarget & 0xFFFF) {
		case 0xFFFF: std::cout << "- trying to write Interrupt Enable State \n"; break;
		}
		}
		break;

	case 3: // Read 16 bit from memory (I need a source[Mem] and a Target[Reg]
		if (fromSource < 0x4000) { std::cout << " Reading 16bit from ROM bank0" << "\n"; }
		else if (fromSource >= 0x4000 && fromSource < 0x8000) { std::cout << " Reading 16bit from ROM bank1" << "\n"; }
		else if (fromSource >= 0x8000 && fromSource < 0xA000) { std::cout << " Reading 16bit from GFX" << "\n"; }
		else if (fromSource >= 0xA000 && fromSource < 0xC000) { std::cout << " Reading 16bit from EXT RAM bank" << "\n"; }
		else if (fromSource >= 0xC000 && fromSource < 0xE000) { std::cout << " Reading 16bit from WORKING RAM" << "\n"; }
		else if (fromSource >= 0xE000 && fromSource < 0xFE00) { std::cout << " Reading 16bit from WORKING RAM SHADOW" << "\n"; }
		else if (fromSource >= 0xFE00 && fromSource < 0xFF00) { std::cout << " Reading 16bit from GFX SPRITES" << "\n"; }
		else if (fromSource >= 0xFF00 && fromSource < 0xFF80) { std::cout << " Reading 16bit from INPUT" << "\n"; }
		else if (fromSource >= 0xFF80 && fromSource < 0xFFFF) { std::cout << " Reading 16bit from ZERO PAGE RAM" << "\n";
		
		}
		break;

	case 4: // Write 16 bit to memory (I need a source[Reg] and a Target[Mem]
		if (toTarget < 0x4000) { std::cout << " Writing 16bit to ROM bank0" << "\n"; }
		else if (toTarget >= 0x4000 && toTarget < 0x8000) { std::cout << " Writing 16bit to ROM bank1" << "\n"; }
		else if (toTarget >= 0x8000 && toTarget < 0xA000) { std::cout << " Writing 16bit to GFX" << "\n";
		//std::cout << "Writing " << sourceReg << std::hex << (int)fromSource << "] to Mem region [0x" << std::hex << (int)toTarget << "]\n";
		//std::cout << "Parsing mem to VramGfx Pos = " << std::dec << int(toTarget - 0x8000) << "\n";
		}
		else if (toTarget >= 0xA000 && toTarget < 0xC000) { std::cout << " Writing 16bit to EXT RAM bank" << "\n"; }
		else if (toTarget >= 0xC000 && toTarget < 0xE000) { std::cout << " Writing 16bit to WORKING RAM" << "\n"; }
		else if (toTarget >= 0xE000 && toTarget < 0xFE00) { std::cout << " Writing 16bit to WORKING RAM SHADOW" << "\n"; }
		else if (toTarget >= 0xFE00 && toTarget < 0xFF00) { std::cout << " Writing 16bit to GFX SPRITES" << "\n"; }
		else if (toTarget >= 0xFF00 && toTarget < 0xFF80) { std::cout << " Writing 16bit to INPUT" << "\n"; }
		else if (toTarget >= 0xFF80 && toTarget < 0xFFFF) { std::cout << " Writing 16bit to ZERO PAGE RAM" << "\n"; }
		break;
		break;
	}
	
	} // MMU EXP 
	


///////////////////////////////////////////////////////////////////////////////////////
//- Register Combiner and Flag managing 
//- This Section is DONE
///////////////////////////////////////////////////////////////////////////////////////
#pragma region FlagCalculator


	void FLGH(unsigned char Z, unsigned char N, unsigned char H, unsigned char C, unsigned short calcT1, unsigned short calcT2, unsigned char Optype) {
		//opTypes
		// 1 - ADD
		// 2 - SUB
		// 3 - Comp
		// 4 - AND
		// 5 - OR
		// 6 - XOR
		unsigned short TT1, TT2, Tresult;
		TT1 = calcT1;	
		TT2 = calcT2;

		std::bitset<8> Rf_as_bit(registers.Rf);
		switch (Z) {
		case 0:	Rf_as_bit.reset(7); break;
		case 1:	Rf_as_bit.set(7);	break;
		case 2: //Calculate Flag based on result
			switch (Optype) {			
			
			case 1:
				if (TT1 == 0 && TT2 == 0) { Rf_as_bit.set(7); }
				else  { Rf_as_bit.reset(7); }
				break;
			
			case 2: 
			Rf_as_bit.set(6);
			Tresult = TT1 - TT2;
			if (Tresult == 0) { Rf_as_bit.set(7); }	else  { Rf_as_bit.reset(7); }
			break;

			case 3:
			Tresult = TT1 - TT2;
			if (Tresult == 0) { Rf_as_bit.set(7); }	else  { Rf_as_bit.reset(7); }
			break;

			case 4:
			Tresult = TT1 & TT2;
			if (Tresult == 0) { Rf_as_bit.set(7); } else  { Rf_as_bit.reset(7); }
			break;
			
			case 5: 
			Tresult = TT1 | TT2;
			if (Tresult == 0) { Rf_as_bit.set(7); }	else  { Rf_as_bit.reset(7); }
			break;
			
			case 6:
			Tresult = TT1 ^ TT2;
			if (Tresult == 0) {  Rf_as_bit.set(7); }	else  { Rf_as_bit.reset(7); }
			break;
			} // End Switch Z - (Optype)
			break;
		case 3: //Ignore Flag		
		break;
		} //END switch (Z)

		switch (N) {
		case 0:	Rf_as_bit.reset(6); break;
		case 1:	Rf_as_bit.set(6);	break;
		case 2: //Calculate Flag based on result
			if (Optype == 2)	{ Rf_as_bit.set(6); }	else { Rf_as_bit.reset(6); }
			break;
		case 3: //Ignore Flag	
		break;
		} //END switch (N)

		switch (H) {
		case 0:	Rf_as_bit.reset(5); break;
		case 1:	Rf_as_bit.set(5);	break;
		case 2: //Calculate Flag based on result
			switch (Optype) {
				case 1: 
				Tresult = TT1 + TT2;
				if (((TT1 & 0xF) + (TT2 & 0xF)) & 0x10 == 0x10) { Rf_as_bit.set(5); }
				else { Rf_as_bit.reset(5); }
				break;
				case 2:
				Rf_as_bit.set(6);
				
				if (((TT1 & 0xF) - (TT2 & 0xF)) & 0x10 == 0x10) { Rf_as_bit.set(5); //std::cout << " H SET " << "\n"; 
				}
				else { Rf_as_bit.reset(5); }
				break;
			}// END N - Optype Switch
			break;
		case 3: //Ignore Flag
		break;
		} //END switch (H)

		switch (C) {
		case 0:	Rf_as_bit.reset(4); break;
		case 1:	Rf_as_bit.set(4);	break;
		case 2: //Calculate Flag based on result
			switch (Optype) {
			case 1:
				Tresult = TT1 + TT2;
				if (Tresult > 0xFF) { Rf_as_bit.set(4); }
				else {}
				break;
			case 2:
				Rf_as_bit.set(6);
				Tresult = TT1 - TT2;
				if (Tresult < 0x0) { Rf_as_bit.set(4); }
				else {}
				break;
			}// END C - Optype Switch
			break;
			
		case 3: //Ignore Flag
		break;
		} //END switch (C)
		// Return Bits in Temp containers for Usage as Carry container
		if (Rf_as_bit.test(7)) { TempRf = 0; std::bitset<8> Rf_as_Tbit(TempRf); Rf_as_Tbit.set(7); TempRf = Rf_as_Tbit.to_ulong(); }

		//Display Box for Flags - Maybe Switch position
		//std::cout << "Z[" << Rf_as_bit.test(7) << "] N[" << Rf_as_bit.test(6) << "] H[" << Rf_as_bit.test(5) << "] C[" << Rf_as_bit.test(4) << "]\n";
		registers.Rf = Rf_as_bit.to_ulong();
	}
#pragma endregion
#pragma region RegisterCombine


	void Rcomb(unsigned char regPair) {
		switch (regPair) {
		case 0:	Raf = (Ra << 8) | Rf;	break;
		case 1:	Rbc = (Rb << 8) | Rc;	break;
		case 2:	Rde = (Rd << 8) | Re;	break;
		case 3:	Rhl = (Rh << 8) | Rl;	break;
		}
	}
#pragma endregion
void RegComb() {
		if (pc != 0x100) { //unsafe be sure to fix when BiOs works
		gb::Rcomb(0);
		gb::Rcomb(1);
		gb::Rcomb(2);
		gb::Rcomb(3);
		}
		//-Done
		//std::cout << "\nAF: " << std::hex << (int)Raf << "\n";
		//std::cout << "BC: " << std::hex << (int)Rbc << "\n";
		//std::cout << "DE: " << std::hex << (int)Rde << "\n";
		//std::cout << "HL: " << std::hex << (int)Rhl << "\n";
	}
void RegRecombiner(std::string Dtarget) {
	if (Dtarget == "AF") { Ra = Raf >> 8; Rf = (Raf & 0xF); }
	else if (Dtarget == "BC") { Rb = Rbc >> 8; Rc = (Rbc & 0xF); }
	else if (Dtarget == "DE") { Re = Rde >> 8; Re = (Rde & 0xF); }
	else if (Dtarget == "HL") { Rh = Rhl >> 8; Rl = (Rhl & 0xF); }
}

#pragma region InstructionMapping
	
///////////////////////////////////////////////////////////////////////////////////////
//- Function Backbone - Order of execution is given by the preset 
//- This will be a Major milestone in the project (Current TASK)
///////////////////////////////////////////////////////////////////////////////////////

	//Instruction Preset
	/*
		////-- Instruction preset
		opLen = 1;
		FLGH(2, 0, 2, 2, $any, Ra, 1 );
		Ra = ($any + TempRf);
		funcText << "ADC " << Dtarget << ", " << Starget << "(" << std::hex << (int)$any << ")";
		functType = funcText.str();
		BoxDeb(loopDetected);
		pc += opLen;
		////-- Instruction preset

	*/ //Hidden Instruction Preset //
	//Instruction Preset

//------------------------------
//8-Bit Loads
//------------------------------

//- Load 8 bit data into register r -- 
// R1 = A,B,C,D,E,H,L
// n = 8bit inmediate value
// 06,0E,16,1E,26,2E
//-------------------------------

void LDR8_n(unsigned char &_R, unsigned char _n, std::string str_R, std::string str_n) {
	////-- Instruction preset
	opLen = 2;
	FLGH(3, 3, 3, 3, NULL, NULL, NULL);
	funcText << "LOAD |" << str_R << ",|n|," << std::hex << std::setw(4) << std::setfill('0') << _n << "|\n";
	functType = funcText.str();
	BoxDeb(loopDetected);
	_R = _n;
	pc += opLen;
	////-- Instruction preset
}

//- LD r1 into r2
// r1 = A,B,C,D,E,H,L
// r2 = A,B,C,D,E,H,L
// 7F,78,79,7A,7B,7C,7D,40,41,42,43,44,45,48,49,4A,4B,4C,4D,50,51
// 52,53,54,55,58,59,5A,5B,5C,5D,60,61,62,63,64,65,68,69,6A,6B,6C
// 6D
//-------------------------------
void LDR8_R8(unsigned char &_R, unsigned char _R1, std::string str_R, std::string str_R1) {
	////-- Instruction preset
	opLen = 2;
	FLGH(3, 3, 3, 3, NULL, NULL, NULL);
	funcText << "LOAD |" << str_R << ",|R8|," << std::hex << std::setw(4) << std::setfill('0') << _R1 << "|\n";
	functType = funcText.str();
	BoxDeb(loopDetected);
	_R = _R1;
	pc += opLen;
	////-- Instruction preset
}

//- Load contents of memory HL(8 bits) into register r -- 
// (HL) = $HL
//  r = A,B,C,D,E,H,L
// 7E,46,4E,56,5E,66,6E
//-------------------------------
void LDR8_$HL(unsigned char &_R, unsigned char _$HL, std::string str_R, std::string str_R1) {
	////-- Instruction preset
	opLen = 2;
	FLGH(3, 3, 3, 3, NULL, NULL, NULL);
	funcText << "LOAD |" << str_R << ",|(HL)|" << std::hex << std::setw(4) << std::setfill('0') << _$HL << "|\n";
	functType = funcText.str();
	BoxDeb(loopDetected);
	_R = memoryA[registers.Rhl];
	pc += opLen;
	////-- Instruction preset
}
//- Stores the contents of register r in memory specified by register pair HL  
//  (HL) = $HL
//  r = A,B,C,D,E,H,L
// 70,71,72,73,74,75
//-------------------------------
void LD$HL_R(unsigned char &_$HL, unsigned char _R, std::string str_$HL, std::string str_R) {
	////-- Instruction preset
	opLen = 2;
	FLGH(3, 3, 3, 3, NULL, NULL, NULL);
	funcText << "LOAD (HL),|" << std::hex << std::setw(4) << std::setfill('0') << _R << "|\n";
	functType = funcText.str();
	BoxDeb(loopDetected);
	memoryA[registers.Rhl] = _R;
	pc += opLen;
	////-- Instruction preset
}
//- stores n(8 bit inmediate) into memory specified by register pair HL -- 
// (HL) = $HL
//  n = 8bit inmediate
// 36
void LD$HL_n(unsigned char &_$HL, unsigned char _n, std::string str_$HL, std::string str_n) {
	////-- Instruction preset
	opLen = 2;
	FLGH(3, 3, 3, 3, NULL, NULL, NULL);
	funcText << "LOAD (HL)" << ",n|" << std::hex << std::setw(4) << std::setfill('0') << _n << "|\n";
	functType = funcText.str();
	BoxDeb(loopDetected);
	memoryA[registers.Rhl] = _n;
	pc += opLen;
	////-- Instruction preset
}
// Loads the contents of the memory specified by register pair RR into Reg A -- 
// (BC) (DE) = memoryA[RR]
// Register A = Ra
// 0A,1A,7E,FA
void LD$RR_n(unsigned char &_$RR, unsigned char _n, std::string str_$HL, std::string str_n) {
	////-- Instruction preset
	opLen = 2;
	FLGH(3, 3, 3, 3, NULL, NULL, NULL);
	funcText << "LOAD (HL)" << ",n|" << std::hex << std::setw(4) << std::setfill('0') << _n << "|\n";
	functType = funcText.str();
	BoxDeb(loopDetected);
	memoryA[_$RR] = _n;
	pc += opLen;
	////-- Instruction preset
}
// Loads into Reg A the contents of the internal RAM port Register
// or mode register at the address range 0xFF00-0xFFFF
// (C) = memoryA[0xFF00 + memoryA[C]]
// Register A = Ra
// E2
void LDA_$C(unsigned char &_$R, unsigned char _$C, std::string str_R, std::string str_$C) {
	////-- Instruction preset
	opLen = 2;
	FLGH(3, 3, 3, 3, NULL, NULL, NULL);
	funcText << "LOAD A" << ",0xFF00 +C|" << std::hex << std::setw(4) << std::setfill('0') << _$C << "|\n";
	functType = funcText.str();
	BoxDeb(loopDetected);
	_$R = memoryA[0xFF00 + _$C];
	pc += opLen;
}

//---End New Opcode Layout

void ADC8RhlM(unsigned short &$any, std::string Dtarget, std::string Starget) { // n + carry flag to A
		////-- Instruction preset
		opLen = 1;
		FLGH(2, 0, 2, 2, $any, registers.Ra, 1 );
		registers.Ra = ($any + TempRf);
		funcText << "ADC " << Dtarget << ", " << Starget << "(" << std::hex << (int)$any << ")";
		functType = funcText.str();
		BoxDeb(loopDetected);
		pc += opLen;
		////-- Instruction preset
	}

	void ADCA$xx(unsigned short $any, std::string Dtarget, std::string Starget)  {
		////-- Instruction preset
		opLen = 2;
		FLGH(2, 0, 2, 2, $any, registers.Ra, 1);
		registers.Ra = ($any + TempRf);
		funcText << "ADC " << Dtarget << ", " << Starget << "(" << std::hex << (int)$any << ")";
		functType = funcText.str();
		BoxDeb(loopDetected);
		pc += opLen;
		////-- Instruction preset
	}
	void ADCR8(unsigned short &$any, std::string Dtarget, std::string Starget) {
		////-- Instruction preset
		opLen = 1;
		FLGH(2, 0, 2, 2, $any, registers.Ra, 1);
		registers.Ra = ($any + TempRf);
		funcText << "ADC " << Dtarget << ", " << Starget << "(" << std::hex << (int)$any << ")";
		functType = funcText.str();
		BoxDeb(loopDetected);
		pc += opLen;
		////-- Instruction preset
	}
	void ADDAhlM(std::string Dtarget, std::string Starget) {
		
		////-- Instruction preset
		opLen = 1;
		FLGH(2, 0, 2, 2, registers.Ra, memoryA[Rhl], 1);
		registers.Ra += memoryA[Rhl];
		funcText << "ADD " << Dtarget << ", " << Starget << "(" << std::hex << (int)memoryA[registers.Rhl] << ")";
		functType = funcText.str();
		BoxDeb(loopDetected);
		pc += opLen;
		////-- Instruction preset

	}
	void ADDA$xx(unsigned char _8int) {
		//FLGH(2, 0, 2, 2);
		//CALCFLG(Ra, _8int, 1);
		registers.Ra += _8int;
		opclock = 2;
		functType = "ADD $xx ";

	}
	void ADDA8R(unsigned char &_8r, std::string Dtarget, std::string Starget) {
		////-- Instruction preset
		opLen = 1;
		FLGH(2, 0, 1, 0, Ra, _8r, 1);
		registers.Ra += _8r;
		funcText << "AND " << Dtarget << ", " << Starget;
		functType = funcText.str();
		BoxDeb(loopDetected);
		pc += opLen;
		////-- Instruction preset
	}
	void ADDHL16R(unsigned short &_R16) {
		//FLGH(3, 0, 2, 2);
		//CALCFLG(Rhl, _R1, 1);
		registers.Rhl += _R16;
		opclock = 1;
		functType = "ADD HL 16R ";
	}
	void ADDSP8OFF(unsigned char _8off) {
		//FLGH(0, 0, 2, 2);
		//CALCFLG(sp, _8off, 1);
		sp = +_8off;
		opclock = 2;
		functType = "ADD SP 8off";
	}
	void ANDAhlM() {
		//FLGH(2, 0, 1, 0);
		//CALCFLG(Ra, memoryA[Rhl], 1);
		//Ra &= memoryA[Rhl];
		opclock = 1;
		functType = "AND A (HL) ";
	}
	void ANDA$xx(unsigned char _8int) {
		//FLGH(2, 0, 1, 0);
		//CALCFLG(Ra, _8int, 3); //set optype3 as AND
		registers.Ra &= _8int;
		opclock = 2;
		functType = "AND A $xx ";
	}
	void ANDA8R(unsigned char &_8r, std::string Dtarget, std::string Starget) {
		////-- Instruction preset
		opLen = 1;
		FLGH(2, 0, 1, 0, Ra, _8r, 4);
		registers.Ra &= _8r;
		funcText << "AND " << Dtarget << ", " << Starget;
		functType = funcText.str();
		BoxDeb(loopDetected);
		pc += opLen;
		////-- Instruction preset
	}
	void BIThlM(unsigned char bitnum) {

		switch (memoryA[registers.Rhl] << bitnum) { // refinar 0x1
		case 1:
			break;

		case 0:

			//FLGH(2, 0, 1, 3);
			opclock = 2;
			functType = "BIT (HL) ";
			//CALCFLG(Ra, _8int, 3) //set optype4 op de revision valor memoryA[Rhl] o hacer directo aca

			//bit test agains
			//update timers
			break;
		}
	}
	 void BIT8R(  unsigned char _8r,  int bitnum, std::string r8char) {
			////-- Instruction preset
			opLen = 2;
			std::bitset<8> regBit(_8r);
			if  (regBit.test(7) == 0 ) {
				FLGH(1, 0, 1, 3, NULL, NULL, NULL);
}
else { FLGH(0, 0, 1, 3, NULL, NULL, NULL); }
			
			funcText <<  "BIT:" << bitnum << r8char;
			functType = funcText.str();
			BoxDeb(loopDetected);
			pc += opLen;
			////-- Instruction preset	
	}
	
	void CALLpcF$aabb(int flagcc, unsigned short _16BA) {

		if (flagcc == 1)
		{
			pc = _16BA;
		}
		//FLGH(3, 3, 3, 3);
		opclock = 3;
		functType = "CALL pc IF F  $aabb ";
	}
	void CALL16BA(unsigned short _$aabb) {
	
		////-- Instruction preset
		opLen = 3;
		FLGH(3, 3, 3, 3, NULL, NULL, NULL);
		funcText << "CALL " << _$aabb;
		functType = funcText.str();
		BoxDeb(loopDetected);
		stack[sp] = pc + 3;
		sp--;
		pc = _$aabb;
		////-- Instruction preset

	}
	void CARRYFLAG() {
		//FLGH(3, 0, 0, 2);
		//CALCFLG(Ra, _8int, 3) //set optype4 op de revision valor memoryA[Rhl] o hacer directo aca
		opclock = 1;
		functType = "CARRY FLAG ";
		//FLGH(3, 3, 3, 1);
	}
	void COMPhlM() {
		//FLGH(2, 1, 2, 2);
		opclock = 1;
		functType = "COMPLEMENT (HL) ";

	}
	void COMP8INT(unsigned char _8int) {
		//FLGH(2, 1, 2, 2);
		opclock = 2;
		functType = "COMPLEMENT $xx ";
	}
	void COMP8R(unsigned char &_8r) {
		//FLGH(2, 1, 2, 2);
		opclock = 1;
		functType = "COMPLEMENT 8R ";
	}
	void CPL() {
		//FLGH(3, 1, 1, 3);
		opclock = 1;
		functType = "COMPLEMENT L";
	}
	void DAA() {
		//FLGH(2, 3, 0, 2);
		opclock = 1;
		functType = "DAA ";
	}
	void DEChlM() {
		//FLGH(2, 1, 2, 3);
		memoryA[registers.Rhl]--;
		opclock = 1;
		functType = "DEC (HL) ";

	}
	void DECR16(unsigned short &_16r) {
		_16r--;
		opclock = 1;
		//update flags
		functType = "DEC R16 ";
		//FLGH(3, 3, 3, 3);
	}
	void DEC8R(unsigned char &_r8, std::string Dtarget) {
		////-- Instruction preset
		opLen = 1;
		FLGH(2, 1, 2, 3, _r8, 1, 2);
		_r8--;
		funcText << "DEC " << Dtarget;
		functType = funcText.str();
		BoxDeb(loopDetected);
		pc += opLen;
		////-- Instruction preset
	}
	void DI() {
		opclock = 1;
		opLen = 1;
		
		functType = "DISABLE INTERRUPTS ";
		pc += opLen;
		//FLGH(3, 3, 3, 3);
	}
	void EI() {
		opclock = 1;
		opLen = 1;
		pc += opclock;
		//FLGH(3, 3, 3, 3);
	}
	void HALT() {
		opclock = 1;
		//FLGH(3, 3, 3, 3);
	}
	void INChlM() {
		//FLGH(2, 0, 2, 3);
		memoryA[registers.Rhl]++;
		opclock = 1;
		//update timers
		//update flags
		functType = "INC (HL) ";
	}
	void INC16R(unsigned short &_16r, std::string Dtarget) {
		////-- Instruction preset
		opLen = 1;
		FLGH(3, 3, 3, 3, NULL, NULL, NULL);
		_16r++;
		RegRecombiner(Dtarget);
		funcText << "INC " << Dtarget;
		functType = funcText.str();
		
		pc += opLen;
		////-- Instruction preset
	}
	void INC8R(unsigned char &_r8, std::string Dtarget) {
		////-- Instruction preset
		opLen = 1;
		FLGH(2, 0, 2, 3, _r8, 1, 2);
		_r8++;
		funcText << "INC " << Dtarget;
		functType = funcText.str();
		BoxDeb(loopDetected);
		pc += opLen;
		////-- Instruction preset
	}
	void JUMPhlM() {
		pc = memoryA[registers.Rhl];

		//update timers
		opclock = 1;
		//update flags
		functType = "JP (HL) ";
		//FLGH(3, 3, 3, 3);
	}
	void JUMpcC$aabb(int flagcc, unsigned short _16BA) {
		if (flagcc == 1)
			pc = _16BA;
		else
			pc++;
		opclock = 3;
		functType = "JUMP CC $aabb ";
		//FLGH(3, 3, 3, 3);
	}
	void JP$aabb(unsigned short _$aabb) {
		////-- Instruction preset
		opLen = 3;
		FLGH(3, 3, 3, 3, NULL, NULL, NULL);
		funcText << "JUMP pc -> " << std::hex << std::setw(4) << std::setfill('0') << (int)_$aabb;
		functType = funcText.str();
		BoxDeb(loopDetected);
		pc = _$aabb;
		////-- Instruction preset
	}
	void JRCC$xx(std::string flagOp, signed char _$xx) {
		////-- Instruction preset
		opLen = 2;
		////std::cout << "8bit int inm " << (int)_$xx << "\n";
		FLGH(3, 3, 3, 3, NULL, NULL, NULL);
		funcText << "JR + " << std::hex << std::setw(4) << std::setfill('0') << (int)_$xx << " IF FlagBit = TRUE  ->";
		functType = funcText.str();
		BoxDeb(loopDetected);
		std::bitset<8> flagChecker(registers.Rf);
		unsigned char byteNum;
		unsigned char testCase;
		if (flagOp == "Z"){ byteNum = 7; testCase = 1; }
		else if (flagOp == "NZ"){ byteNum = 7; testCase = 0; }

		else if (flagOp == "C"){ byteNum = 4; testCase = 1; }
		else if (flagOp == "NC"){ byteNum = 4; testCase = 0; }

		if (flagChecker.test(byteNum) == testCase)
		{
			std::string result = "TRUE\n";
			//std::cout << "Case " << flagOp << "tested " << result;
			pc = pc + _$xx + opLen;
		}

		else if (flagChecker.test(byteNum) != testCase)
		{
			std::string result = "FALSE\n";
			//std::cout << "Case " << flagOp << "tested " << result;
			pc += opLen;
		}
		////-- Instruction preset
	}
	void JR$xx(unsigned char _8int) {
		pc = memoryA[_8int];
		//skip
		opclock = 2;
		functType = "JR $xx ";
		//FLGH(3, 3, 3, 3);
	}
	void LOADCMRA() {
		registers.Ra = memoryA[registers.Rc];
		//update timers
		opclock = 1;
		//update flags
		functType = "LOAD C (A) ";
		//FLGH(3, 3, 3, 3);
	}
	void LOADhlM$xx(unsigned char _8int) {
		memoryA[registers.Rhl] = _8int;
		//update timers
		opclock = 2;
		//update flags
		functType = "LOAD (HL) $xx ";
		//FLGH(3, 3, 3, 3);
	}
	void LOADhlM8R(unsigned char &_8r) {
		memoryA[registers.Rhl] = _8r;
		//update timers
		opclock = 1;
		//update flags
		functType = "LOAD (HL) R8 ";
		///FLGH(3, 3, 3, 3);
	}
	void LOAD$aabbMRA(unsigned short _16ba) {
		memoryA[_16ba] = registers.Ra;
		//update timers
		opclock = 3;
		//update flags
		functType = "LOAD $aabb (A) ";
		//FLGH(3, 3, 3, 3);
	}
	void LOAD$aabbSP(unsigned short _16ba) {
		memoryA[_16ba] = sp;
		//update timers
		opclock = 3;
		//update flags
		functType = "LOAD $aabb SP ";
		//FLGH(3, 3, 3, 3);
	}
	void LOAD16RMRA(unsigned short &_16r) {
		memoryA[_16r] =registers.Ra;
		//update timers
		opclock = 1;
		//update flags
		functType = "LOAD (16R) A ";
		//FLGH(3, 3, 3, 3);
	}
	void LOADRACM(unsigned char &_8r) {
		registers.Ra = memoryA[_8r];
		//update timers
		opclock = 1;
		//update flags
		functType = "LOAD A (C) - REVISAR ";
		//FLGH(3, 3, 3, 3);
		///////////////////REVISAR PUEDE ESTAR MALO
	}
	void LOADRA$aabb(unsigned short _16ba) {
		registers.Ra = memoryA[_16ba];
		//update timers
		opclock = 3;
		//update flags
		functType = "LOAD A $aabb ";
		//FLGH(3, 3, 3, 3);
	}
	void LOADRA16RM(unsigned short _16r, std::string Starget) {
		////-- Instruction preset
		opLen = 1;		
		FLGH(3, 3, 3, 3, NULL, NULL, NULL);
		funcText << "LOAD A, (" << Starget << ") |" << std::hex << std::setw(4) << std::setfill('0') << (int)memoryA[_16r] << "|";
		functType = funcText.str();
		BoxDeb(loopDetected);
		Ra = memoryA[_16r];
		MMUexp(NULL, _16r, "Ra", 3);
		pc += opLen;
		////-- Instruction preset
	}
	void LOADRHLSPOFF(unsigned char _8boff) {
		//FLGH(0, 0, 2, 2);
		registers.Rhl = (sp + _8boff);
		//update timers
		opclock = 2;
		//update flags
		functType = "LOAD HL SPOFF";
	}
	void LOADR16$aabb(unsigned short &_16r, unsigned short $_aabb, std::string Dtarget) {
		////-- Instruction preset
		opLen = 3;
		FLGH(3, 3, 3, 3, NULL, NULL, NULL);
		funcText << "LOAD " << Dtarget << ", "  << std::hex << std::setw(4) << std::setfill('0') << (int)$_aabb;
		functType = funcText.str();
		BoxDeb(loopDetected);
		_16r = $_aabb;
		MMUexp($_aabb, NULL, Dtarget, 3);
		//if (Dtarget == "HL") { Rh = $aa; Rl = $bb; }
		//else if (Dtarget == "AF") { Ra = $aa; Rf = $bb; }
		//else if (Dtarget == "BC") { Rb = $aa; Rc = $bb; }
		//else if (Dtarget == "DE") { Rd = $aa; Re = $bb; }
		memCatch =$_aabb;
		memCatchFlag = true;
		pc += opLen;
		
		
		////-- Instruction preset
	}
	void LOADR8hlM(unsigned char &_8r) {
		_8r = memoryA[registers.Rhl];
		functType = "LD (HL)";
		//gb::DebugReg(functType);
		//update timers
		opclock = 1;
		opLen = 1;
		pc += opLen;
	//	FLGH(3, 3, 3, 3);
		//update flags
	}
	void LDR8$xx(unsigned char &_8r, unsigned char _$xx, std::string Dtarget) {
		////-- Instruction preset
		opLen = 2;
		FLGH(3, 3, 3, 3, NULL, NULL, NULL);
		funcText << "LOAD |" << Dtarget << "|, $XX|" << std::hex << std::setw(4) << std::setfill('0') << _$xx << "|\n";
		functType = funcText.str();
		BoxDeb(loopDetected);
		_8r = _$xx;
		MMUexp(registers.Ra, _$xx, "XX", 1);
		pc += opLen;
		////-- Instruction preset
	}
	void LOADR8R8(unsigned char &_8r, unsigned char  &_8r2, std::string Dtarget, std::string Starget) {
		////-- Instruction preset
		opLen = 1;
		FLGH(3, 3, 3, 3, NULL, NULL, NULL);
		_8r = _8r2;
		funcText << "LOAD " << Dtarget << ", " << Starget;
		functType = funcText.str();
		BoxDeb(loopDetected);
		pc += opLen;
		////-- Instruction preset
	}
	void LOADSPHL(unsigned short &SP, unsigned short &HL) {
		SP = HL;
		//update timers
		opclock = 1;
		//update flags
		functType = "SP HL ";
		//FLGH(3, 3, 3, 3);
	}
	void LOADDR8hlM() {
		registers.Ra = memoryA[registers.Rhl];
		Rhl--;
		//update timers
		opclock = 1;
		//update flags
		functType = "R8 (HL) ";
		//FLGH(3, 3, 3, 3);
		opLen = 1;
	}
	void LOADDhlMR8(unsigned char &_R8, std::string Dtarget, std::string Starget) {
		////-- Instruction preset
		opLen = 1;
		FLGH(3, 3, 3, 3, NULL, NULL, NULL);
		//std::cout << "MEMORY RHL: " << std::dec << (int)vramGfx[8191] << "\n";
		funcText << "LOADD " << "(HL) ," << Starget;
		functType = funcText.str();
		//std::cout << "MEMORY RHL DESP: " << std::dec << (int)vramGfx[(8191)] << "\n";
	
		BoxDeb(loopDetected);
		memoryA[registers.Rhl] = _R8;
		MMUexp(registers.Rhl, _R8, Starget, 2);
		registers.Rhl--;
		pc += opLen;
		////-- Instruction preset

	}
	void LOADHAOFF(signed int $_xx, unsigned char R8, std::string Starget) { //LDH
		////-- Instruction preset
		opLen = 2;
		FLGH(3, 3, 3, 3, NULL, NULL, NULL);
		funcText << "LD " << std::hex << (int)$_xx << ", " << Starget;
		functType = funcText.str();
		BoxDeb(loopDetected);
		memoryA[0xff00 | $_xx] = R8;
		MMUexp((0xff00 | $_xx), R8 , Starget, 2);
		pc += opLen;
		////-- Instruction preset
	}
	void LOADOFFR8A(unsigned char _R8, std::string Starget) { //LOAD A into R8 + 0xff00
		//It would be wise to know which part of memory is trying to access
		////-- Instruction preset
		opLen = 1;
		FLGH(3, 3, 3, 3, NULL, NULL, NULL);
		funcText << "LOAD ( 0xff00 + " << Starget << ")|(" << std::hex << (int)_R8 << "| ) ,A";
		functType = funcText.str();
		BoxDeb(loopDetected);
		memoryA[0xff00 | _R8] = registers.Ra;
		MMUexp(0xff00 | _R8,registers.Ra, "Ra", 2);
		pc += opLen;
		////-- Instruction preset
	}
	void LDIhlMRA() {
		memoryA[registers.Rhl] = registers.Ra;
		registers.Ra++;
		//update timers
		opclock = 1;
		//update flags
		functType = " LDI (HL) A ";
		//FLGH(3, 3, 3, 3);
	}
	void LDIRAhlM() {
		registers.Ra = memoryA[registers.Rhl];
		memoryA[registers.Rhl]++;
		//update timers
		opclock = 1;
		//update flags
		functType = "LDI A (HL) ";
	//	FLGH(3, 3, 3, 3);
	}
	void NOP() {
		
		//FLGH(3, 3, 3, 3);
		//pc += opLen;
		functType = "NOP ";
	}
	void ORhlMRA() {
	//	FLGH(2, 0, 0, 0);
		memoryA[registers.Rhl] |= registers.Ra;
		opclock = 1;
		functType = "OR (HL) A ";
	}
	void ORRA$xx(unsigned char _8int) {
	//	FLGH(2, 0, 0, 0);
		registers.Ra |= _8int;
		opclock = 2;
		functType = "OR ADC R8 ";
	}
	void ORRA8R(unsigned char &_r8, std::string Dtarget, std::string Starget) {
		////-- Instruction preset
		opLen = 1;
		FLGH(2, 0, 0, 0, registers.Ra, _r8, 5);
		registers.Ra |= _r8;
		funcText << "OR " << Dtarget << ", " << Starget;
		functType = funcText.str();
		BoxDeb(loopDetected);
		pc += opLen;
		////-- Instruction preset
	}
	void POPr16(unsigned short &_16r) {
		opLen = 1;
		_16r = stack[sp];
		sp++;
		opclock = 1;
		functType = "POP 16R ";
		BoxDeb(loopDetected);
		pc += opLen;
	//	FLGH(3, 3, 3, 3);
	}
	void PUSH( unsigned short _R16, std::string Starget) {
		////-- Instruction preset
		opLen = 1;
		sp--;
		stack[sp] = _R16;
		FLGH(3, 3, 3, 3, NULL, NULL, NULL);
		funcText << "PUSH " << Starget;
		functType = funcText.str();
		BoxDeb(loopDetected);
		pc += opLen;
		////-- Instruction preset
	}
	void RESBhlM(unsigned char _bit) {
		//bit de memoryA[Rhl] a cero
		opclock = 2;
		functType = "RES BIT (HL) ";
	//	FLGH(3, 3, 3, 3);
	}
	void RESBR8(unsigned char _bit, unsigned  char &_8r){
		//bit de _8r a cero
		opclock = 2;
		functType = "RES BIT R8 ";
	//	FLGH(3, 3, 3, 3);
	}
	void RET() {
		opclock = 1;
		functType = "RET ";
	//	FLGH(3, 3, 3, 3);
	}

	void RETCC(unsigned char flagcc) {
		opclock = 1;
		functType = "RET CC ";
	//	FLGH(3, 3, 3, 3);
	}
	void RETI() {
		opclock = 1;
		functType = "RETI ";
	//	FLGH(3, 3, 3, 3);
	}
	void RLHM() {
	//	FLGH(2, 0, 0, 2);
		opclock = 2;
		functType = "RL (H)";
	}
	void RLR8(unsigned char &_8R, std::string dTarget) {
	//	FLGH(2, 0, 0, 2);
			////-- Instruction preset
		opLen = 2;
		//FLGH(2, 0, 0, 0, registers.Ra, _r8, 6);
		std::bitset<8> tempRegContainer(_8R);
		std::bitset<8> tempRegRf(registers.Rf);
		unsigned short tempR8;
		std::bitset<8> tempAssemble();
		
		int Cvalue;
		int MLvalue;
		
		//preparing to store carried over bit
		
		
		Cvalue = tempRegRf.test(4);
		MLvalue = tempRegContainer.test(7);
		
		tempR8 = (_8R << 1);
		std::bitset<8> tempR8b(tempR8);
		
		tempR8b[0] = Cvalue;
		tempRegRf[4] = MLvalue;

		_8R = tempR8b.to_ulong();		
		registers.Rf = tempRegRf.to_ulong();
		
		//registers.Ra ^= _r8;
		functType = "RL" + dTarget;
		functType = funcText.str();
		BoxDeb(loopDetected);
		pc += opLen;
		////-- Instruction preset
	}
	void RL(unsigned char _R8) {
	//	FLGH(0, 0, 0, 2);
		opclock = 1;
		opLen = 1;
		functType = "RL R8 ";
		
		std::bitset<8> tempRegContainer(_R8);
		std::bitset<8> tempRegRf(registers.Rf);
		unsigned short tempR8;
		std::bitset<8> tempAssemble();
		
		int MRvalue;
		int MLvalue;
		
		//preparing to store carried over bit
		
		MRvalue = tempRegContainer[0];
		MLvalue = tempRegContainer[7];
		
		tempR8 = (_R8<< 1);
		std::bitset<8> tempR8b(tempR8);
		
		tempR8b[0] = MLvalue;
		tempRegRf[4] = MLvalue;
		
		_R8 = tempR8b.to_ulong();		
		registers.Rf = tempRegRf.to_ulong();
		BoxDeb(loopDetected);
		pc += opLen;
		
	}
	void RLCHM() {
	//	FLGH(2, 0, 0, 2);
		opclock = 2;
		functType = "RLC (H) ";
	}
	void RLCR8R8(unsigned char &_r8, unsigned char &_r82) {
	//	FLGH(2, 0, 0, 2);
		opclock = 2;
		functType = "RLC R8 R8 ";
	}
	void RLC() {
	//	FLGH(0, 0, 0, 2);
		opclock = 1;
		functType = "RLC A ";
	}
	void RRHM() {
	//	FLGH(2, 0, 0, 2);
		opclock = 2;
		functType = "RR (H) ";
	}
	void RRR8R8(unsigned char &_r8, unsigned char &_r82) {
	//	FLGH(2, 0, 0, 2);
		opclock = 2;
		functType = "RR R8 R8 ";

	}
	void RRA() {
	//	FLGH(0, 0, 0, 2);
		opclock = 1;
		functType = "RR A ";

	}
	void RRCHM() {
	//	FLGH(2, 0, 0, 2);
		opclock = 2;
		functType = "RRC (H) ";
	}
	void RRCR8R8(unsigned char &_r8, unsigned char &_r82) {
	//	FLGH(2, 0, 0, 2);
		opclock = 2;
		functType = "RRC R8 R8 ";
	}
	void RRCA() {
	//	FLGH(0, 0, 0, 2);
		opclock = 1;
		functType = "RRC A ";
	}
	void RSTF(unsigned char _byte) {
		functType = "RST F ";
	}
	void SBCHM() {
	//	FLGH(2, 1, 2, 2);
		opclock = 1;
		functType = "SBC (H) ";
	}
	void SBC$xx(unsigned char _8int) {
	//	FLGH(2, 1, 2, 2);
		opclock = 2;
		functType = "SBC $xx ";
	}
	void SBC8R(unsigned char &_r8) {
	//	FLGH(2, 1, 2, 2);
		opclock = 1;
		functType = "SBC 8R ";
	}
	void SCF() {
	//	FLGH(3, 0, 0, 1);
		opclock = 1;
		functType = "SCF ";
	}
	void SETBHM(unsigned char _bit) {
		opclock = 2;
		functType = "SET B (H) ";
	//	FLGH(3, 3, 3, 3);
	}
	void SETBR8(unsigned char _bit, unsigned char &_r8) {
		opclock = 2;
		functType = "SET B R8 ";
	//	FLGH(3, 3, 3, 3);
	}
	void SLAhlM() {
	//	FLGH(2, 0, 0, 2);
		opclock = 2;
		functType = "SLA (HL) ";
	}
	void SLAR8(unsigned char &_r8) {
	//	FLGH(2, 0, 0, 2);
		opclock = 2;
		functType = "SLA R8 ";
	}
	void SRAHM() {
	//	FLGH(2, 0, 0, 2);
		opclock = 2;
		functType = "SRA (H) ";
	}
	void SRAR8R8(unsigned char &_8r, unsigned char &_r82) {
	//	FLGH(2, 0, 0, 2);
		opclock = 2;
		functType = "SRA R8 R8 ";
	}
	void SRLHM() {
	//	FLGH(2, 0, 0, 2);
		opclock = 2;
		functType = "SRL (H) ";
	}
	void SRLR8R8(unsigned char &_8r, unsigned char &_r82) {
	//	FLGH(2, 0, 0, 2);
		opclock = 2;
		functType = "SRL R8 R8 ";
	}
	void STOP() {
		opclock = 2;
		functType = "STOP ";
	//	FLGH(3, 3, 3, 3);
	}
	void SUBAhlM() {
//		FLGH(2, 1, 2, 2);
		opclock = 1;
		functType = "SUB A (HL) ";
	}
	void SUBA$xx(unsigned char _8int) {
	//	FLGH(2, 1, 2, 2);
		opclock = 2;
		functType = "SUB A $xx ";
	}

	void SUBAR8(unsigned char &_8r, std::string Dtarget, std::string Starget) {
		////-- Instruction preset
		opLen = 1;
		FLGH(2, 1, 2, 2, registers.Ra, _8r, 2);
		registers.Ra -= _8r;
		funcText << "SUB " << Dtarget << ", " << Starget;
		functType = funcText.str();
		BoxDeb(loopDetected);
		pc += opLen;
		////-- Instruction preset
	}
	void SWAPhlM() {
	//	FLGH(2, 0, 0, 0);
		opclock = 2;
		functType = "SWAP (HL) ";
	}
	void SWAPR8R8(unsigned char &_r8, unsigned char &_r82) {
	//	FLGH(2, 0, 0, 0);
		opclock = 2;
		functType = "SWAP R8 R8 ";
	}
	void XORhlM() {
	//	FLGH(2, 0, 0, 0);
		opclock = 1;
		functType = "XOR (HL) - MAYBE A? ";
	}
	void XOR$xx(unsigned char _8int) {
	//	FLGH(2, 0, 0, 0);
		opclock = 2;
		functType = "XOR $xx ";
	}
	void XORAR8(unsigned char &_r8, std::string Dtarget, std::string Starget) {		
		////-- Instruction preset
		opLen = 1;
		FLGH(2, 0, 0, 0, registers.Ra, _r8, 6);
		registers.Ra ^= _r8;
		funcText << "XOR R8";
		functType = funcText.str();
		BoxDeb(loopDetected);
		pc += opLen;
		////-- Instruction preset
		
		//opLen, flag, math, mmu, text, opDeb(text clear)
	}
	/// Interrupt Work, research only
	
	/*

	unsigned short intType;
	unsigned char TIMA;

	bool IE = false;
	void InterrupHandling(unsigned short iType) {

	switch (interruptType) {
	if (IE)
	{
	//	case "VBLANK": exception
	//	break;

	//	case LCDCSTATUS: exception
	//	break;

	case 1: //TIMA overflow

	//check for TIMA overflow
	break;

	case SERIALTRANSFERCOMPLETE:
	break;

	case HL10P10-13:
	break;

	//IO Registers

	case FF00-P1:
	break;

	case FF01-SB:
	break;

	case FF02-SC:
	break;

	case FF04-DIV:
	break;

	case FF05-TIMA:
	break;

	case FF06-TMA:
	break;

	case FF07-TAC:
	break;

	case FF0F-IF:
	break;

	case FF10-NR10:
	break;


	}
	}

	}

	*/
	//-- fin interrupts

	// Opcodes 
	///////////////////////////////////////////////////////////////////////////////////////
	//- Opcode table with debug info - Started
	//- This will be a Major milestone in the project (Current TASK)
	///////////////////////////////////////////////////////////////////////////////////////

	void opDecoder(bool& loopDetected, long int& opCounter) {
		
		funcText.str(std::string());
		_16bitIn = (memoryA[pc + 2] << 8) | memoryA[pc + 3];
		fullOpcode = memoryA[pc] << 8 | memoryA[pc + 1];
		long opcodeAndPar = memoryA[pc] << 8 | memoryA[pc + 1];

		//opcode = memoryA[pc];
		opcode = memoryA[pc];
		unsigned short opcodeCB = memoryA[pc] | memoryA[pc + 1];
		$bb = memoryA[pc + 1];
		$xx = memoryA[pc + 1];
		$aa = memoryA[pc + 2];
		memoryA[Rhl] = memoryA[Rhl];
		_8bitIn3 = memoryA[pc + 3];
		$aabb = (memoryA[pc + 2] << 8) | (memoryA[pc + 1]);

		
		//_8bitInM = memoryA[_8bitIn];

		// aa bb = 16bit integer
		//Opcode is also refered ( BY ME :D ) as ENGAGER a 16bit Entity capable of capuring from 00 to FFFF
		//By masking it like this I only capture the lower Byte tho
		switch (opcode) {
		case 0X00:	NOP();			Dm = " - 0x00 - NOP"; Dissam();			break;
		case 0X01:					Dm = " - 0x01 - LD BC $aabb";Dissam();	break;
		case 0X02:					Dm = " - 0x02 - LD (BC) $aabb";Dissam();	break;
		case 0X03:INC16R(registers.Rbc, "BC");	Dm = " - 0x03 - INC BC";Dissam();		break;
		case 0X04:INC8R(registers.Rb, "B");	Dm = " - 0x04 - INC B"; 	Dissam();		break;
		case 0X05:DEC8R(registers.Rb, "B");	Dm = " - 0x05 - DEC B";	Dissam();		break;
		case 0X06:LDR8$xx(registers.Rb, (int)$xx, "B");Dm = " - 0x06 - LD B $xx";	Dissam();	break;
		case 0X07:			Dm = " - 0x07 - RLCA";		Dissam();	break;
		case 0X08:					Dm = " - 0x08 - LD ($aabb) SP";Dissam();	break;
		case 0X09:					Dm = " - 0x09 - ADD HL BC";	Dissam();	break;
		case 0X0A:					Dm = " - 0x0A - LD A (BC)";	Dissam();	break;
		case 0X0B:					Dm = " - 0x0B - DEC BC";	Dissam();	break;
		case 0X0C:INC8R(registers.Rc, "C");		Dm = " - 0x0C - INC C";	Dissam();		break;
		case 0X0D:DEC8R(registers.Rc, "C");		Dm = " - 0x0D - DEC C";	Dissam();		break;
		case 0X0E:LDR8$xx(registers.Rc, (int)$xx, "C"); Dm = " - 0x0E - LD C $xx";	Dissam();	break;
		case 0X0F:						Dm = " - 0x0F - RRCA";	Dissam();		break;
		case 0X10:						Dm = " - 0x10 - STOP";	Dissam();		break;
		case 0X11:LOADR16$aabb(registers.Rde, $aabb, "DE");  Dm = " - 0x11 - LD DE $aabb";Dissam();	break;
		case 0X12:						Dm = " - 0x12 - LD (DE) A";	Dissam();	break;
		case 0X13:INC16R(registers.Rde, "DE");	Dm = " - 0x13 - INC DE";Dissam();		break;
		case 0X14:INC8R(registers.Rd, "D");		Dm = " - 0x14 - INC D";	Dissam();		break;
		case 0X15:DEC8R(registers.Rd, "D");		Dm = " - 0x15 - DEC D";	Dissam();		break;
		case 0X16:LDR8$xx(registers.Rd, (int)$xx, "D");	Dm = " - 0x15 - LD D $xx ";Dissam();		break;
		case 0X17:	RLR8(registers.Ra, "A");					Dm = " - 0x17 - RLA";	Dissam();		break;
		case 0X18:						Dm = " - 0x18 - JR $xx";	Dissam();	break;
		case 0X19:						Dm = " - 0x19 - ADD HL DE";	Dissam();	break;
		case 0X1A:LOADRA16RM(registers.Rde, "DE");Dm = " - 0x1A - LD A (DE)";	Dissam();	break;
		case 0X1B:						Dm = " - 0x1B - DEC DE";	Dissam();	break;
		case 0X1C:INC8R(registers.Re, "E");		Dm = " - 0x1C - INC E";	Dissam();		break;
		case 0X1D:DEC8R(registers.Re, "E");		Dm = " - 0x1D - DEC E";	Dissam();		break;
		case 0X1E:LDR8$xx(registers.Re, (int)$xx, "E");	Dm = " - 0x1E - LD E $xx";Dissam();		break;
		case 0X1F:						Dm = " - 0x1F - RRA";	Dissam();		break;
		case 0X20:JRCC$xx("NZ", (int)$xx);	Dm = " - 0x20 - JR NZ $xx";Dissam();		break;
		case 0X21:LOADR16$aabb(registers.Rhl, $aabb , "HL");	Dm = " - 0x21 - LD HL $aabb";Dissam();	break;
		case 0X22:						Dm = " - 0x22 - LD (HLI) A";Dissam();	break;
		case 0X23:INC16R(registers.Rhl, "HL");	Dm = " - 0x23 - INC HL";Dissam();		break;
		case 0X24:INC8R(registers.Rh, "H");		Dm = " - 0x24 - INC H";	Dissam();		break;
		case 0X25:DEC8R(registers.Rh, "H");		Dm = " - 0x25 - DEC H";	Dissam();		break;
		case 0X26:LDR8$xx(registers.Rh, (int)$xx, "H");	Dm = " - 0x26 - LD H $xx";	Dissam();	break;
		case 0X27:						Dm = " - 0x27 - DAA";	Dissam();		break;
		case 0X28:						Dm = " - 0x28 - JR Z $xx";	Dissam();	break;
		case 0X29:						Dm = " - 0x29 - ADD HL HL";	Dissam();	break;
		case 0X2A:						Dm = " - 0x2A - LD A (HLI)";Dissam();	break;
		case 0X2B:						Dm = " - 0x2B - DEC HL";	Dissam();	break;
		case 0X2C:INC8R(registers.Rl, "L");		Dm = " - 0x2C - INC L";	Dissam();		break;
		case 0X2D:DEC8R(registers.Rl, "L");		Dm = " - 0x2D - DEC L";			break;
		case 0X2E:LDR8$xx(registers.Rl, (int)$xx, "L");	Dm = " - 0x2E - LD L $xx";	Dissam();	break;
		case 0X2F:						Dm = " - 0x2F - CPL";	Dissam();		break;
		case 0X30:						Dm = " - 0x30 - JR NC $xx";	Dissam();	break;
		case 0X31:LOADR16$aabb(sp, $aabb,"SP");				Dm = " - 0x31 - LD SP $aabb";Dissam();	break;
		case 0X32:LOADDhlMR8(registers.Ra, "(HL)", "A");	Dm = " - 0x32 - LDD (HLD) A";Dissam();	break;
		case 0X33:INC16R(sp, "SP");		Dm = " - 0x33 - INC SP";	Dissam();	break;
		case 0X34:						Dm = " - 0x34 - INC (HL)";		break;
		case 0X35:						Dm = " - 0x2B - DEC HL";		break;
		case 0X36:						Dm = " - 0x36 - LD (HL) $xx";	break;
		case 0X37:						Dm = " - 0x37 - SCF";			break;
		case 0X38:						Dm = " - 0x38 - JR C $xx";		break;
		case 0X39:						Dm = " - 0x39 - ADD HL SP";		break;
		case 0X3A:						Dm = " - 0x3A - LD A (HLD)";	break;
		case 0X3B:						Dm = " - 0x3B - DEC SP";		break;
		case 0X3C:INC8R(registers.Ra, "A");		Dm = " - 0x3C - INC A";		Dissam();	break;
		case 0X3D:DEC8R(registers.Ra, "A");		Dm = " - 0x3D - DEC A";	Dissam();		break;
		case 0X3E:LDR8$xx(registers.Ra, $xx, "A");	Dm = " - 0x3E - LD A $xx";	Dissam();	break;
		case 0X3F:				Dm = " - 0x3F - CCF";		Dissam();	break;
		case 0X40:LOADR8R8(registers.Rb, registers.Rb, "B", "B");	Dm = " - 0x40 - LD B B";Dissam();		break;
		case 0X41:LOADR8R8(registers.Rb, registers.Rc, "B", "C");	Dm = " - 0x41 - LD B C";	Dissam();	break;
		case 0X42:LOADR8R8(registers.Rb, registers.Rd, "B", "D");	Dm = " - 0x42 - LD B D";	Dissam();	break;
		case 0X43:LOADR8R8(registers.Rb, registers.Re, "B", "E");	Dm = " - 0x43 - LD B E";	Dissam();	break;
		case 0X44:LOADR8R8(registers.Rb, registers.Rh, "B", "H");	Dm = " - 0x44 - LD B H";	Dissam();	break;
		case 0X45:LOADR8R8(registers.Rb,registers.Rl, "B", "L");	Dm = " - 0x45 - LD B L";	Dissam();	break;
		case 0X46:LOADR8R8(registers.Rb, memoryA[registers.Rhl], "B", "(HL)");	Dm = " - 0x46 - LD B (HL)";	Dissam();	break;
		case 0X47:LOADR8R8(registers.Rb, registers.Ra, "B", "A");	Dm = " - 0x47 - LD B A";	Dissam();	break;
		case 0X48:LOADR8R8(registers.Rc, registers.Rb, "C", "B");	Dm = " - 0x48 - LD C B";	Dissam();	break;
		case 0X49:LOADR8R8(registers.Rc, registers.Rc, "C", "C");	Dm = " - 0x49 - LD C C";	Dissam();	break;
		case 0X4A:LOADR8R8(registers.Rc, registers.Rd, "C", "D");	Dm = " - 0x4A - LD C D";	Dissam();	break;
		case 0X4B:LOADR8R8(registers.Rc, registers.Re, "C", "E");	Dm = " - 0x4B - LD C E";	Dissam();	break;
		case 0X4C:LOADR8R8(registers.Rc, registers.Rh, "C", "H");	Dm = " - 0x4C - LD C H";	Dissam();	break;
		case 0X4D:LOADR8R8(registers.Rc, registers.Rl, "C", "L");	Dm = " - 0x4D - LD C L";	Dissam();	break;
		case 0X4E:LOADR8R8(registers.Rc, memoryA[registers.Rhl], "C", "(HL)");	Dm = " - 0x4E - LD C (HL)";	Dissam();	break;
		case 0X4F:LOADR8R8(registers.Rc, registers.Ra, "C", "A");	Dm = " - 0x4F - LD C A";	Dissam();	break;
		case 0X50:LOADR8R8(registers.Rd, registers.Rb, "D", "B");	Dm = " - 0x50 - LD D B";	Dissam();	break;
		case 0X51:LOADR8R8(registers.Rd, registers.Rc, "D", "C");	Dm = " - 0x51 - LD D C";	Dissam();	break;
		case 0X52:LOADR8R8(registers.Rd, registers.Rd, "D", "D");	Dm = " - 0x52 - LD D D";	Dissam();	break;
		case 0X53:LOADR8R8(registers.Rd, registers.Re, "D", "E");	Dm = " - 0x53 - LD D E";	Dissam();	break;
		case 0X54:LOADR8R8(registers.Rd, registers.Rh, "D", "H");	Dm = " - 0x54 - LD D H";	Dissam();	break;
		case 0X55:LOADR8R8(registers.Rd, registers.Rl, "D", "L");	Dm = " - 0x55 - LD D L";	Dissam();	break;
		case 0X56:LOADR8R8(registers.Rd, memoryA[registers.Rhl], "D", "(HL)");	Dm = " - 0x56 - LD D (HL)";	Dissam();	break;
		case 0X57:LOADR8R8(registers.Rd, registers.Ra, "D", "A");	Dm = " - 0x57 - LD D A";	Dissam();	break;
		case 0X58:LOADR8R8(registers.Re, registers.Rb, "E", "B");	Dm = " - 0x58 - LD E B";	Dissam();	break;
		case 0X59:LOADR8R8(registers.Re, registers.Rc, "E", "C");	Dm = " - 0x59 - LD E C";	Dissam();	break;
		case 0X5A:LOADR8R8(registers.Re, registers.Rd, "E", "D");	Dm = " - 0x5A - LD E D";	Dissam();	break;
		case 0X5B:LOADR8R8(registers.Re, registers.Re, "E", "E");	Dm = " - 0x5B - LD E E";	Dissam();	break;
		case 0X5C:LOADR8R8(registers.Re, registers.Rh, "E", "H");	Dm = " - 0x5C - LD E H";Dissam();		break;
		case 0X5D:LOADR8R8(registers.Re, registers.Rl, "E", "L");	Dm = " - 0x5D - LD E L";	Dissam();	break;
		case 0X5E:LOADR8R8(registers.Re, memoryA[registers.Rhl], "E", "(HL)");	Dm = " - 0x5E - LD E (HL)";Dissam();		break;
		case 0X5F:LOADR8R8(registers.Re, registers.Ra, "E", "A");	Dm = " - 0x5F - LD E A";	Dissam();	break;
		case 0X60:LOADR8R8(registers.Rh, registers.Rb, "H", "B");	Dm = " - 0x60 - LD H B";	Dissam();	break;
		case 0X61:LOADR8R8(registers.Rh, registers.Rc, "H", "C");	Dm = " - 0x61 - LD H C";	Dissam();	break;
		case 0X62:LOADR8R8(registers.Rh, registers.Rd, "H", "D");	Dm = " - 0x62 - LD H D";Dissam();		break;
		case 0X63:LOADR8R8(registers.Rh, registers.Re, "H", "E");	Dm = " - 0x63 - LD H E";	Dissam();	break;
		case 0X64:LOADR8R8(registers.Rh, registers.Rh, "H", "H");	Dm = " - 0x64 - LD H H";	Dissam();	break;
		case 0X65:LOADR8R8(registers.Rh, registers.Rl, "H", "L");	Dm = " - 0x65 - LD H L";	Dissam();	break;
		case 0X66:LOADR8R8(registers.Rh, memoryA[registers.Rhl], "H", "(HL)");	Dm = " - 0x66 - LD H (HL)";	Dissam();	break;
		case 0X67:LOADR8R8(registers.Re, memoryA[registers.Rhl], "E", "(HL)");	Dm = " - 0x67 - LD E (HL)";	Dissam();	break;
		case 0X68:LOADR8R8(registers.Rl, registers.Rb, "L", "B");	Dm = " - 0x68 - LD L B";	Dissam();	break;
		case 0X69:LOADR8R8(registers.Rl, registers.Rc, "L", "C");	Dm = " - 0x69 - LD L C";	Dissam();	break;
		case 0X6A:LOADR8R8(registers.Rl, registers.Rd, "L", "D");	Dm = " - 0x6A - LD L D";	Dissam();	break;
		case 0X6B:LOADR8R8(registers.Rl, registers.Re, "L", "E");	Dm = " - 0x6B - LD L E";	Dissam();	break;
		case 0X6C:LOADR8R8(registers.Rl, registers.Rh, "L", "H");	Dm = " - 0x6C - LD L H";	Dissam();	break;
		case 0X6D:LOADR8R8(registers.Rl , registers.Rl, "L", "L");	Dm = " - 0x6D - LD L L";Dissam();		break;
		case 0X6E:LOADR8R8(registers.Rl, memoryA[registers.Rhl], "L", "(HL)");	Dm = " - 0x6E - LD L (HL)";Dissam();		break;
		case 0X6F:LOADR8R8(registers.Rh, registers.Ra, "H", "A");	Dm = " - 0x6F - LD H A";	Dissam();	break;
		case 0X70:LOADR8R8(memoryA[registers.Rhl], registers.Rb, "(HL)", "B");	Dm = " - 0x70 - LD (HL) B";Dissam();		break;
		case 0X71:LOADR8R8(memoryA[registers.Rhl], registers.Rc, "(HL)", "C");	Dm = " - 0x71 - LD (HL) C";Dissam();		break;
		case 0X72:LOADR8R8(memoryA[registers.Rhl], registers.Rd, "(HL)", "D");	Dm = " - 0x72 - LD (HL) D";Dissam();		break;
		case 0X73:LOADR8R8(memoryA[registers.Rhl], registers.Re, "(HL)", "E");	Dm = " - 0x73 - LD (HL) E";	Dissam();	break;
		case 0X74:LOADR8R8(memoryA[registers.Rhl], registers.Rh, "(HL)", "H");	Dm = " - 0x74 - LD (HL) H";Dissam();		break;
		case 0X75:LOADR8R8(memoryA[registers.Rhl], registers.Rl, "(HL)", "L");	Dm = " - 0x75 - LD (HL) L";	Dissam();	break;
		case 0X76:						Dm = " - 0x76 - HALT";	Dissam();		break;
		case 0X77:LOADR8R8(memoryA[registers.Rhl], registers.Ra, "(HL)", "A");	Dm = " - 0x77 - LD (HL) A";Dissam();		break;
		case 0X78:LOADR8R8(registers.Ra, registers.Rb, "A", "B");	Dm = " - 0x78 - LD A B";	Dissam();	break;
		case 0X79:LOADR8R8(registers.Ra, registers.Rc, "A", "C");	Dm = " - 0x79 - LD A C";	Dissam();break;
		case 0X7A:LOADR8R8(registers.Ra, registers.Rd, "A", "D");	Dm = " - 0x7A - LD A D";	Dissam();	break;
		case 0X7B:LOADR8R8(registers.Ra, registers.Re, "A", "E");	Dm = " - 0x7B - LD A E";	Dissam();	break;
		case 0X7C:LOADR8R8(registers.Ra, registers.Rh, "A", "H");	Dm = " - 0x7C - LD A H";	Dissam();	break;
		case 0X7D:LOADR8R8(registers.Ra, registers.Rl, "A", "L");	Dm = " - 0x7D - LD A L";	Dissam();	break;
		case 0X7E:LOADR8R8(registers.Ra, memoryA[registers.Rhl], "A", "(HL)");Dm = " - 0x7E - LD A (HL)";	Dissam();	break;
		case 0X7F:LOADR8R8(registers.Ra, registers.Ra, "A", "A");	Dm = " - 0x7F - LD A A";	Dissam();	break;
		case 0X80:ADDA8R(registers.Rb, "A", "B");	Dm = " - 0x80 - ADD A B";	Dissam();	break;
		case 0X81:ADDA8R(registers.Rc, "A", "C");	Dm = " - 0x81 - ADD A C";	Dissam();	break;
		case 0X82:ADDA8R(registers.Rd, "A", "D");	Dm = " - 0x82 - ADD A D";	Dissam();	break;
		case 0X83:ADDA8R(registers.Re, "A", "E");	Dm = " - 0x83 - ADD A E";	Dissam();	break;
		case 0X84:ADDA8R(registers.Rh, "A", "H");	Dm = " - 0x84 - ADD A H";	Dissam();	break;
		case 0X85:ADDA8R(registers.Rl, "A", "L");	Dm = " - 0x85 - ADD A L";	Dissam();	break;
		case 0X86:ADDA8R(memoryA[registers.Rhl], "A", "(HL)");	Dm = " - 0x86 - ADD A (HL)";Dissam();	break;
		case 0X87:ADDA8R(registers.Ra, "A", "A");	Dm = " - 0x87 - ADD A A";Dissam();		break;
		case 0X88:						Dm = " - 0x88 - ADC A B";		break;
		case 0X89:						Dm = " - 0x89 - ADC A C";		break;
		case 0X8A:						Dm = " - 0x8A - ADC A D";		break;
		case 0X8B:						Dm = " - 0x8B - ADC A E";		break;
		case 0X8C:						Dm = " - 0x8C - ADC A H";		break;
		case 0X8D:						Dm = " - 0x8D - ADC A L";		break;
		case 0X8E:						Dm = " - 0x8E - ADC A (HL)";	break;
		case 0X8F:						Dm = " - 0x8F - ADC A A";		break;
		case 0X90:SUBAR8(registers.Rb, "A", "B");	Dm = " - 0x90 - SUB B";		Dissam();	break;
		case 0X91:SUBAR8(registers.Rc, "A", "C");	Dm = " - 0x91 - SUB C";		Dissam();	break;
		case 0X92:SUBAR8(registers.Rd, "A", "D");	Dm = " - 0x92 - SUB D";		Dissam();	break;
		case 0X93:SUBAR8(registers.Re, "A", "E");	Dm = " - 0x93 - SUB E";		Dissam();	break;
		case 0X94:SUBAR8(registers.Rh, "A", "H");	Dm = " - 0x94 - SUB H";		Dissam();	break;
		case 0X95:SUBAR8(registers.Rl, "A", "L");	Dm = " - 0x95 - SUB L";		Dissam();	break;
		case 0X97:SUBAR8(registers.Ra, "A", "A");	Dm = " - 0x97 - SUB A";	Dissam();		break;
		case 0X98:						Dm = " - 0x98 - SBC A B";		break;
		case 0X99:						Dm = " - 0x99 - SBC A C";		break;
		case 0X9A:						Dm = " - 0x9A - SBC A D";		break;
		case 0X9B:						Dm = " - 0x9B - SBC A E";		break;
		case 0X9C:						Dm = " - 0x9C - SBC A H";		break;
		case 0X9D:						Dm = " - 0x9D - SBC A L";		break;
		case 0X9E:						Dm = " - 0x9E - SBC A (HL)";	break;
		case 0X9F:						Dm = " - 0x9F - SBC A A";		break;
		case 0XA0:ANDA8R(registers.Rb, "A", "B");	Dm = " - 0xA0 - AND B";		Dissam();	break;
		case 0XA1:ANDA8R(registers.Rc, "A", "C");	Dm = " - 0xA1 - AND C";	Dissam();		break;
		case 0XA2:ANDA8R(registers.Rd, "A", "D");	Dm = " - 0xA2 - AND D";	Dissam();	break;
		case 0XA3:ANDA8R(registers.Re, "A", "E");	Dm = " - 0xA3 - AND E";		Dissam();	break;
		case 0XA4:ANDA8R(registers.Rh, "A", "H");	Dm = " - 0xA4 - AND H";		Dissam();	break;
		case 0XA5:ANDA8R(registers.Rl, "A", "L");	Dm = " - 0xA5 - AND L";		Dissam();	break;
		case 0XA6:ANDA8R(memoryA[registers.Rhl], "A", "(HL)");	Dm = " - 0xA6 - AND (HL)";	Dissam();	break;
		case 0XA7:ANDA8R(registers.Ra, "A", "A");	Dm = " - 0xA7 - AND A";		Dissam();	break;
		case 0XA8:XORAR8(registers.Rb, "A", "B");	Dm = " - 0xA8 - XOR B";		Dissam();	break;
		case 0XA9:XORAR8(registers.Rc, "A", "C");	Dm = " - 0xA9 - XOR C";	Dissam();	break;
		case 0XAA:XORAR8(registers.Rd, "A", "D");	Dm = " - 0xAA - XOR D";	Dissam();		break;
		case 0XAB:XORAR8(registers.Re, "A", "E");	Dm = " - 0xAB - XOR E";	Dissam();	break;
		case 0XAC:XORAR8(registers.Rh, "A", "H");	Dm = " - 0xAC - XOR H";	Dissam();		break;
		case 0XAD:XORAR8(registers.Rl, "A", "L");	Dm = " - 0xAD - XOR L";	Dissam();		break;
		case 0XAE:XORAR8(memoryA[registers.Rhl], "A", "(HL)");	Dm = " - 0xAE - XOR (HL)";	Dissam();	break;
		case 0XAF: XORAR8(registers.Ra, "A", "A");	Dm = " - 0xAF - XOR A";	Dissam();		break;
		case 0XB0:ORRA8R(registers.Rb, "A", "B");	Dm = " - 0xB0 - OR B";		Dissam();	break;
		case 0XB1:ORRA8R(registers.Rc, "A", "C");	Dm = " - 0xB1 - OR C";		Dissam();	break;
		case 0XB2:ORRA8R(registers.Rd, "A", "D");	Dm = " - 0xB2 - OR D";	Dissam();		break;
		case 0XB3:ORRA8R(registers.Re, "A", "E");	Dm = " - 0xB3 - OR E";		Dissam();	break;
		case 0XB4:ORRA8R(registers.Rh, "A", "H");	Dm = " - 0xB4 - OR H";		Dissam();	break;
		case 0XB5:ORRA8R(registers.Rl, "A", "L");	Dm = " - 0xB5 - OR L";		Dissam();	break;
		case 0XB6:ORRA8R(memoryA[registers.Rhl], "A", "(HL)");	Dm = " - 0xB6 - OR (HL)";	Dissam();	break;
		case 0XB7:ORRA8R(registers.Rb, "A", "A");	Dm = " - 0xB7 - OR A";		Dissam();	break;
		case 0XB8:						Dm = " - 0xB8 - CP B";			break;
		case 0XB9:						Dm = " - 0xB9 - CP C";			break;
		case 0XBA:						Dm = " - 0xBA - CP D";			break;
		case 0XBB:						Dm = " - 0xBB - CP E";			break;
		case 0XBC:						Dm = " - 0xBC - CP H";			break;
		case 0XBD:						Dm = " - 0xBD - CP L";			break;
		case 0XBE:						Dm = " - 0xBE - CP (HL)";		break;
		case 0XBF:						Dm = " - 0xBF - CP A";			break;
		case 0XC0:						Dm = " - 0xC0 - RET NZ";		break;
		case 0XC1:	POPr16(registers.Rbc); Dissam();					Dm = " - 0xC1 - POP BC";		break;
		case 0XC2:						Dm = " - 0xC2 - JP NZ $aabb";	break;
		case 0XC3:	JP$aabb($aabb);		Dm = " - 0xC3 - JUMP $aabb";Dissam();	break;
		case 0XC4:						Dm = " - 0xC4 - CALL NZ $aabb";	break;
		case 0XC5:PUSH(registers.Rbc, "BC");	Dm = " - 0xC5 - PUSH BC";	Dissam();	break;
		case 0XC6:						Dm = " - 0xC6 - ADD A $xx";		break;
		case 0XC7:						Dm = " - 0xC7 - RST $ 0";		break;
		case 0XC8:						Dm = " - 0xC8 - RET Z";			break;
		case 0XC9:						Dm = " - 0xC9 - RET";			break;
		case 0XCA:						Dm = " - 0xCA - JP Z $aabb";	break;
		case 0xCB: {
			//std::cout << "OPCODE: " << std::hex << std::setw(6) << std::setfill('0') << (int)fullOpcode << "\n";
			// Full opcode a 16bit unfiltered entity. Captures the whole 2 bytes for CB cases
			switch (fullOpcode)
			{
			case 0XCB00:					Dm = " - 0xCB00 - RLC B";		break;
			case 0XCB01:					Dm = " - 0xCB01 - RLC C";		break;
			case 0XCB02:					Dm = " - 0xCB02 - RLC D";		break;
			case 0XCB03:					Dm = " - 0xCB03 - RLC E";		break;
			case 0XCB04:					Dm = " - 0xCB04 - RLC H";		break;
			case 0XCB05:					Dm = " - 0xCB05 - RLC L";		break;
			case 0XCB06:					Dm = " - 0xCB06 - RLC (HL)";	break;
			case 0XCB07:					Dm = " - 0xCB07 - RLC A";		break;
			case 0XCB08:					Dm = " - 0xCB08 - RRC B";		break;
			case 0XCB09:					Dm = " - 0xCB09 - RRC C";		break;
			case 0XCB0A:					Dm = " - 0xCB0A - RRC D";		break;
			case 0XCB0B:					Dm = " - 0xCB0B - RRC E";		break;
			case 0XCB0C:					Dm = " - 0xCB0C - RRC H";		break;
			case 0XCB0D:					Dm = " - 0xCB0D - RRC L";		break;
			case 0XCB0E:					Dm = " - 0xCB0E - RRC (HL)";	break;
			case 0XCB0F:					Dm = " - 0xCB0F - RRC A";		break;
			case 0XCB10:		RLR8(registers.Rb, "B");			Dm = " - 0xCB10 - RL B";		break;
			case 0XCB11: RLR8(registers.Rc, "C");	//PROX OPCODE			
				Dm = " - 0xCB11 - RL C";		break;
			case 0XCB12:	RLR8(registers.Rd, "C");				Dm = " - 0xCB12 - RL D";		break;
			case 0XCB13:	RLR8(registers.Re, "E");				Dm = " - 0xCB13 - RL E";		break;
			case 0XCB14:		RLR8(registers.Rh, "H");			Dm = " - 0xCB14 - RL H";		break;
			case 0XCB15:		RLR8(registers.Rl, "L");			Dm = " - 0xCB15 - RL L";		break;
			case 0XCB16:		RLR8(memoryA[registers.Rhl],"HL" );			Dm = " - 0xCB16 - RL (HL)";		break;
			case 0XCB17:	RLR8(registers.Ra, "A");				Dm = " - 0xCB17 - RL A";Dissam();		break;
			case 0XCB18:					Dm = " - 0xCB18 - RR B";		break;
			case 0XCB19:					Dm = " - 0xCB19 - RR C";		break;
			case 0XCB1A:					Dm = " - 0xCB1A - RR D";		break;
			case 0XCB1B:					Dm = " - 0xCB1B - RR E";		break;
			case 0XCB1C:					Dm = " - 0xCB1C - RR H";		break;
			case 0XCB1D:					Dm = " - 0xCB1D - RR L";		break;
			case 0XCB1E:					Dm = " - 0xCB1E - RR (HL)";		break;
			case 0XCB1F:					Dm = " - 0xCB1F - RR E";		break;
			case 0XCB20:					Dm = " - 0xCB20 - SLA B";		break;
			case 0XCB21:					Dm = " - 0xCB21 - SLA C";		break;
			case 0XCB22:					Dm = " - 0xCB22 - SLA D";		break;
			case 0XCB23:					Dm = " - 0xCB23 - SLA E";		break;
			case 0XCB24:					Dm = " - 0xCB24 - SLA H";		break;
			case 0XCB25:					Dm = " - 0xCB25 - SLA L";		break;
			case 0XCB26:					Dm = " - 0xCB26 - SLA (HL)";	break;
			case 0XCB27:					Dm = " - 0xCB27 - SLA A";		break;
			case 0XCB28:					Dm = " - 0xCB28 - SRA B";		break;
			case 0XCB29:					Dm = " - 0xCB29 - SRA C";		break;
			case 0XCB2A:					Dm = " - 0xCB2A - SRD D";		break;
			case 0XCB2B:					Dm = " - 0xCB2B - SRA E";		break;
			case 0XCB2C:					Dm = " - 0xCB2C - SRA H";		break;
			case 0XCB2D:					Dm = " - 0xCB2D - SRA L";		break;
			case 0XCB2E:					Dm = " - 0xCB2E - SRA (HL)";	break;
			case 0XCB2F:					Dm = " - 0xCB2F - SRA A";		break;
				/////////////////REVISAR ESTA TRANSICION ////////////
			case 0XCB37:					Dm = " - 0xCB37 - SWAP A";		break;
			case 0XCB38:					Dm = " - 0xCB38 - SRL B";		break;
			case 0XCB39:					Dm = " - 0xCB39 - SRL C";		break;
			case 0XCB3A:					Dm = " - 0xCB3A - SRL D";		break;
			case 0XCB3B:					Dm = " - 0xCB3B - SRL E";		break;
			case 0XCB3C:					Dm = " - 0xCB3C - SRL H";		break;
			case 0XCB3D:					Dm = " - 0xCB3D - SRL L";		break;
			case 0XCB3E:					Dm = " - 0xCB3E - SRL (HL)";	break;
			case 0XCB3F:					Dm = " - 0xCB3F - SRL A";		break;
			case 0XCB40:					Dm = " - 0xCB40 - BIT 0 B";		break;
			case 0XCB41:					Dm = " - 0xCB41 - BIT 0 C";		break;
			case 0XCB42:					Dm = " - 0xCB42 - BIT 0 D";		break;
			case 0XCB43:					Dm = " - 0xCB43 - BIT 0 E";		break;
			case 0XCB44:					Dm = " - 0xCB44 - BIT 0 H";		break;
			case 0XCB45:					Dm = " - 0xCB45 - BIT 0 L";		break;
			case 0XCB46:					Dm = " - 0xCB46 - BIT 0 (HL)";	break;
			case 0XCB47:					Dm = " - 0xCB47 - BIT 0 A";		break;
			case 0XCB48:					Dm = " - 0xCB48 - BIT 1 B";		break;
			case 0XCB49:					Dm = " - 0xCB49 - BIT 1 C";		break;
			case 0XCB4A:					Dm = " - 0xCB4A - BIT 1 D";		break;
			case 0XCB4B:					Dm = " - 0xCB4B - BIT 1 E";		break;
			case 0XCB4C:					Dm = " - 0xCB4C - BIT 1 H";		break;
			case 0XCB4D:					Dm = " - 0xCB4D - BIT 1 L";		break;
			case 0XCB4E:					Dm = " - 0xCB4A - BIT 1 (HL)";	break;
			case 0XCB4F:					Dm = " - 0xCB4F - BIT 1 A";		break;
			case 0XCB50:					Dm = " - 0xCB50 - BIT 2 B";		break;
			case 0XCB51:					Dm = " - 0xCB51 - BIT 2 C";		break;
			case 0XCB52:					Dm = " - 0xCB52 - BIT 2 D";		break;
			case 0XCB53:					Dm = " - 0xCB53 - BIT 2 E";		break;
			case 0XCB54:					Dm = " - 0xCB54 - BIT 2 H";		break;
			case 0XCB55:					Dm = " - 0xCB55 - BIT 2 L";		break;
			case 0XCB56:					Dm = " - 0xCB56 - BIT 2 (HL)";	break;
			case 0XCB57:					Dm = " - 0xCB57 - BIT 2 A";		break;
			case 0XCB58:					Dm = " - 0xCB58 - BIT 3 B";		break;
			case 0XCB59:					Dm = " - 0xCB59 - BIT 3 C";		break;
			case 0XCB5A:					Dm = " - 0xCB5A - BIT 3 D";		break;
			case 0XCB5B:					Dm = " - 0xCB5B - BIT 3 E";		break;
			case 0XCB5C:					Dm = " - 0xCB5C - BIT 3 H";		break;
			case 0XCB5D:					Dm = " - 0xCB5D - BIT 3 L";		break;
			case 0XCB5E:					Dm = " - 0xCB5E - BIT 3 (HL)";	break;
			case 0XCB5F:					Dm = " - 0xCB5F - BIT 3 A";		break;
			case 0XCB60:					Dm = " - 0xCB60 - BIT 4 B";		break;
			case 0XCB61:					Dm = " - 0xCB61 - BIT 4 C";		break;
			case 0XCB62:					Dm = " - 0xCB62 - BIT 4 D";		break;
			case 0XCB63:					Dm = " - 0xCB63 - BIR 4 E";		break;
			case 0XCB64:					Dm = " - 0xCB64 - BIT 4 H";		break;
			case 0XCB65:					Dm = " - 0xCB65 - BIT 4 L";		break;
			case 0XCB66:					Dm = " - 0xCB66 - BIT 4 (HL)";	break;
			case 0XCB67:					Dm = " - 0xCB67 - BIT 4 A";		break;
			case 0XCB68:					Dm = " - 0xCB68 - BIT 5 B";		break;
			case 0XCB69:					Dm = " - 0xCB69 - BIT 5 C";		break;
			case 0XCB6A:					Dm = " - 0xCB6A - BIT 5 D";		break;
			case 0XCB6B:					Dm = " - 0xCB6B - BIT 5 E";		break;
			case 0XCB6C:					Dm = " - 0xCB6C - BIT 5 H";		break;
			case 0XCB6D:					Dm = " - 0xCB6D - BIT 5 L";		break;
			case 0XCB6E:					Dm = " - 0xCB6E - BIT 5 (HL)";	break;
			case 0XCB6F:					Dm = " - 0xCB6F - BIT 5 A";		break;
			case 0XCB70:					Dm = " - 0xCB70 - BIT 6 B";		break;
			case 0XCB71:					Dm = " - 0xCB71 - BIT 6 C";		break;
			case 0XCB72:					Dm = " - 0xCB72 - BIT 6 D";		break;
			case 0XCB73:					Dm = " - 0xCB73 - BIT 6 E";		break;
			case 0XCB74:					Dm = " - 0xCB74 - BIT 6 H";		break;
			case 0XCB75:					Dm = " - 0xCB75 - BIT 6 L";		break;
			case 0XCB76:					Dm = " - 0xCB76 - BIT 6 (HL)";	break;
			case 0XCB77:					Dm = " - 0xCB77 - BIT 6 A";		break;
			case 0XCB78:					Dm = " - 0xCB78 - BIT 7 B";		break;
			case 0XCB79:					Dm = " - 0xCB79 - BIT 7 C";		break;
			case 0XCB7A:					Dm = " - 0xCB7A - BIT 7 D";		break;
			case 0XCB7B:					Dm = " - 0xCB7B - BIT 7 E";		break;
			case 0XCB7C: BIT8R(registers.Rh, 7, "RH");	Dm = " - 0xCB7C - BIT 7 H";	break;
			case 0XCB7D:					Dm = " - 0xCB7D - BIT 7 (HL)";	break;
			case 0XCB7F:					Dm = " - 0xCB7F - BIT 7 A";		break;
			case 0XCB80:					Dm = " - 0xCB80 - RES 0 B";		break;
			case 0XCB81:					Dm = " - 0xCB81 - RES 0 C";		break;
			case 0XCB82:					Dm = " - 0xCB82 - RES 0 D";		break;
			case 0XCB83:					Dm = " - 0xCB83 - RES 0 E";		break;
			case 0XCB84:					Dm = " - 0xCB84 - RES 0 H";		break;
			case 0XCB85:					Dm = " - 0xCB85 - RES 0 L";		break;
			case 0XCB86:					Dm = " - 0xCB86 - RES 0 (HL)";	break;
			case 0XCB87:					Dm = " - 0xCB87 - RES 0 A";		break;
			case 0XCB88:					Dm = " - 0xCB88 - RES 1 B";		break;
			case 0XCB89:					Dm = " - 0xCB89 - RES 1 C";		break;
			case 0XCB8A:					Dm = " - 0xCB8A - RES 1 D";		break;
			case 0XCB8B:					Dm = " - 0xCB8B - RES 1 E";		break;
			case 0XCB8C:					Dm = " - 0xCB8C - RES 1 H";		break;
			case 0XCB8D:					Dm = " - 0xCB8D - RES 1 L";		break;
			case 0XCB8E:					Dm = " - 0xCB8E - RES 1 (HL)";	break;
			case 0XCB8F:					Dm = " - 0xCB8F - RES 1 A";		break;
			case 0XCB90:					Dm = " - 0xCB90 - RES 2 B";		break;
			case 0XCB91:					Dm = " - 0xCB91 - RES 2 C";		break;
			case 0XCB92:					Dm = " - 0xCB92 - RES 2 D";		break;
			case 0XCB93:					Dm = " - 0xCB93 - RES 2 E";		break;
			case 0XCB94:					Dm = " - 0xCB94 - RES 2 H";		break;
			case 0XCB95:					Dm = " - 0xCB95 - RES 2 L";		break;
			case 0XCB96:					Dm = " - 0xCB96 - RES 2 (HL)";	break;
			case 0XCB97:					Dm = " - 0xCB97 - RES 2 A";		break;
			case 0XCB98:					Dm = " - 0xCB98 - RES 3 B";		break;
			case 0XCB99:					Dm = " - 0xCB99 - RES 3 B";		break;
			case 0XCB9A:					Dm = " - 0xCB9A - RES 3 D";		break;
			case 0XCB9B:					Dm = " - 0xCB9B - RES 3 E";		break;
			case 0XCB9C:					Dm = " - 0xCB9C - RES 3 H";		break;
			case 0XCB9D:					Dm = " - 0xCB9D - RES 3 L";		break;
			case 0XCB9E:					Dm = " - 0xCB9E - RES 3 (HL)";	break;
			case 0XCB9F:					Dm = " - 0xCB9F - RES 3 A";		break;
			case 0XCBA0:					Dm = " - 0xCBA0 - RES 4 B";		break;
			case 0XCBA1:					Dm = " - 0xCBA1 - RES 4 C";		break;
			case 0XCBA2:					Dm = " - 0xCBA2 - RES 4 D";		break;
			case 0XCBA3:					Dm = " - 0xCBA3 - RES 4 E";		break;
			case 0XCBA4:					Dm = " - 0xCBA4 - RES 4 H";		break;
			case 0XCBA5:					Dm = " - 0xCBA5 - RES 4 L";		break;
			case 0XCBA6:					Dm = " - 0xCBA6 - RES 4 (HL)";	break;
			case 0XCBA7:					Dm = " - 0xCBA7 - RES 4 A";		break;
			case 0XCBA8:					Dm = " - 0xCBA8 - RES 5 B";		break;
			case 0XCBA9:					Dm = " - 0xCBA9 - RES 5 C";		break;
			case 0XCBAA:					Dm = " - 0xCBAA - RES 5 D";		break;
			case 0XCBAB:					Dm = " - 0xCBAB - RES 5 E";		break;
			case 0XCBAC:					Dm = " - 0xCBAC - RES 5 H";		break;
			case 0XCBAD:					Dm = " - 0xCBAD - RES 5 L";		break;
			case 0XCBAE:					Dm = " - 0xCBAE - RES 5 (HL)";	break;
			case 0XCBAF:					Dm = " - 0xCBAF - RES 5 A";		break;
			case 0XCBB0:					Dm = " - 0xCBB0 - RES 6 B";		break;
			case 0XCBB1:					Dm = " - 0xCBB1 - RES 6 C";		break;
			case 0XCBB2:					Dm = " - 0xCBB2 - RES 6 D";		break;
			case 0XCBB3:					Dm = " - 0xCBB3 - RES 6 E";		break;
			case 0XCBB4:					Dm = " - 0xCBB4 - RES 6 H";		break;
			case 0XCBB5:					Dm = " - 0xCBB5 - RES 6 L";		break;
			case 0XCBB6:					Dm = " - 0xCBB6 - RES 6 (HL)";	break;
			case 0XCBB7:					Dm = " - 0xCBB7 - RES 6 A";		break;
			case 0XCBB8:					Dm = " - 0xCBB8 - RES 7 B";		break;
			case 0XCBB9:					Dm = " - 0xCBB9 - RES 7 C";		break;
			case 0XCBBA:					Dm = " - 0xCBBA - RES 7 D";		break;
			case 0XCBBB:					Dm = " - 0xCBBB - RES 7 E";		break;
			case 0XCBBC:					Dm = " - 0xCBBC - RES 7 H";		break;
			case 0XCBBD:					Dm = " - 0xCBBD - RES 7 L";		break;
			case 0XCBBE:					Dm = " - 0xCBBE - RES 7 (HL)";	break;
			case 0XCBBF:					Dm = " - 0xCBBF - RES 7 A";		break;
			case 0XCBC0:					Dm = " - 0xCBC0 - SET 0 B";		break;
			case 0XCBC1:					Dm = " - 0xCBC1 - SET 0 C";		break;
			case 0XCBC2:					Dm = " - 0xCBC2 - SET 0 D";		break;
			case 0XCBC3:					Dm = " - 0xCBC3 - SET 0 E";		break;
			case 0XCBC4:					Dm = " - 0xCBC4 - SET 0 H";		break;
			case 0XCBC5:					Dm = " - 0xCBC5 - SET 0 L";		break;
			case 0XCBC6:					Dm = " - 0xCBC6 - SET 0 (HL)";	break;
			case 0XCBC7:					Dm = " - 0xCBC7 - SET 0 a";		break;
			case 0XCBC8:					Dm = " - 0xCBC8 - SET 1 B";		break;
			case 0XCBC9:					Dm = " - 0xCBC9 - SET 1 C";		break;
			case 0XCBCA:					Dm = " - 0xCBCA - SET 1 D";		break;
			case 0XCBCB:					Dm = " - 0xCBCB - SET 1 E";		break;
			case 0XCBCC:					Dm = " - 0xCBCC - SET 1 H";		break;
			case 0XCBCD:					Dm = " - 0xCBCD - SET 1 L";		break;
			case 0XCBCE:					Dm = " - 0xCBCE - SET 1 (HL)";	break;
			case 0XCBCF:					Dm = " - 0xCBCF - SET 1 A";		break;
			case 0XCBD0:					Dm = " - 0xCBD0 - SET 2 B";		break;
			case 0XCBD1:					Dm = " - 0xCBD1 - SET 2 C";		break;
			case 0XCBD2:					Dm = " - 0xCBD2 - SET 2 D";		break;
			case 0XCBD3:					Dm = " - 0xCBD3 - SET 2 E";		break;
			case 0XCBD4:					Dm = " - 0xCBD4 - SET 2 H";		break;
			case 0XCBD5:					Dm = " - 0xCBD5 - SET 2 L";		break;
			case 0XCBD6:					Dm = " - 0xCBD6 - SET 2 (HL)";	break;
			case 0XCBD7:					Dm = " - 0xCBD7 - SET 2 A";		break;
			case 0XCBD8:					Dm = " - 0xCBD8 - SET 3 B";		break;
			case 0XCBD9:					Dm = " - 0xCBD9 - SET 3 C";		break;
			case 0XCBDA:					Dm = " - 0xCBDA - SET 3 D";		break;
			case 0XCBDB:	Dm = " - 0xCBD8 - SET 3 E";		break;
			case 0XCBDC:	Dm = " - 0xCBDC - SET 3 H";		break;
			case 0XCBDD:	Dm = " - 0xCBDD - SET 3 L";		break;
			case 0XCBDE:	Dm = " - 0xCBDE - SET 3 (HL)";	break;
			case 0XCBDF:	Dm = " - 0xCBDF - SET 3 A";		break;
			case 0XCBE0:	Dm = " - 0xCBE0 - SET 4 B";		break;
			case 0XCBE1:	Dm = " - 0xCBE1 - SET 4 C";		break;
			case 0XCBE2:	Dm = " - 0xCBE2 - SET 4 D";		break;
			case 0XCBE3:	Dm = " - 0xCBE3 - SET 4 E";		break;
			case 0XCBE4:	Dm = " - 0xCBE4 - SET 4 H";		break;
			case 0XCBE5:	Dm = " - 0xCBE5 - SET 4 L";		break;
			case 0XCBE6:	Dm = " - 0xCBE6 - SET 4 (HL)";	break;
			case 0XCBE7:	Dm = " - 0xCBE7 - SET 4 A";		break;
			case 0XCBE8:	Dm = " - 0xCBE8 - SET 5 B";		break;
			case 0XCBE9:	Dm = " - 0xCBE9 - SET 5 C";		break;
			case 0XCBEA:	Dm = " - 0xCBEA - SET 5 D";		break;
			case 0XCBEB:	Dm = " - 0xCBEB - SET 5 E";		break;
			case 0XCBEC:	Dm = " - 0xCBEC - SET 5 H";		break;
			case 0XCBED:	Dm = " - 0xCBED - SET 5 L";		break;
			case 0XCBEE:	Dm = " - 0xCBEE - SET 5 (HL)";	break;
			case 0XCBEF:	Dm = " - 0xCBEF - SET 5 A";		break;
			case 0XCBF0:	Dm = " - 0xCBF0 - SET 6 B";		break;
			case 0XCBF1:	Dm = " - 0xCBF1 - SET 6 C";		break;
			case 0XCBF2:	Dm = " - 0xCBF2 - SET 6 D";		break;
			case 0XCBF3:	Dm = " - 0xCBF3 - SET 6 E";		break;
			case 0XCBF4:	Dm = " - 0xCBF4 - SET 6 H";		break;
			case 0XCBF5:	Dm = " - 0xCBF5 - SET 6 L";		break;
			case 0XCBF6:	Dm = " - 0xCBF6 - SET 6 (HL)";	break;
			case 0XCBF7:	Dm = " - 0xCBF7 - SET 6 A";		break;
			case 0XCBF8:	Dm = " - 0xCBF8 - SET 7 B";		break;
			case 0XCBF9:	Dm = " - 0xCBF9 - SET 7 C";		break;
			case 0XCBFA:	Dm = " - 0xCBFA - SET 7 D";		break;
			case 0XCBFB:	Dm = " - 0xCBFB - SET 7 E";		break;
			case 0XCBFC:	Dm = " - 0xCBFC - SET 7 H";		break;
			case 0XCBFD:	Dm = " - 0xCBFD - SET 7 L";		break;
			case 0XCBFE:	Dm = " - 0xCBFE - SET 7 (HL)";	break;
			case 0XCBFF:	Dm = " - 0xCBFF - SET 7 A";		break;
			}
			break;
		}

		case 0XCC:	Dm = " - 0xCC - CALL Z $aabb";	break;
		case 0XCD:CALL16BA($aabb );	Dm = " - 0xCD - CALL $aabb";Dissam();	break;
		case 0XCE:	Dm = " - 0xCE - ADC A $xx";		break;
		case 0XCF:	Dm = " - 0xCF - RST $8";		break;
		case 0XD0:	Dm = " - 0xD0 - RET NC";		break;
		case 0XD1: POPr16(registers.Rde); Dissam();	Dm = " - 0xD1 - POP DE";		break;
		case 0XD2:	Dm = " - 0xD2 - JP NC $aabb";	break;
		case 0XD4:	Dm = " - 0xD4 - CALL NC $aabb";	break;
		case 0XD5:	Dm = " - 0xD5 - PUSH DE";		break;
		case 0XD6:	Dm = " - 0xD6 - SUB $xx";		break;
		case 0XD7:	Dm = " - 0xD7 - RST $10";		break;
		case 0XD8:	Dm = " - 0xD8 - RET C";			break;
		case 0XD9:	Dm = " - 0xD9 - RETI";			break;
		case 0XDA:	Dm = " - 0xDA - JP C $aabb";	break;
		case 0XDC:	Dm = " - 0xDC - CALL C $aabb";	break;
		case 0XDE:	Dm = " - 0xDE - SBC A $xx";		break;
		case 0XDF:	Dm = " - 0xDF - RST $18";		break;
		case 0XE0: LOADHAOFF($xx, registers.Ra, "Ra");	 Dm = " - 0xE0 - LDH ($xx) A";Dissam(); break;
		case 0XE1:						Dm = " - 0xE1 - POP HL"; BoxDeb(loopDetected);	Dissam();	break;
		case 0XE2: LOADOFFR8A(registers.Rc, "C");	Dm = " - 0xE2 - LD (C) A"; Dissam();	break;
		case 0XE5:						Dm = " - 0xE5 - PUSH HL"; BoxDeb(loopDetected);		break;
		case 0XE6:						Dm = " - 0xE6 - AND $xx"; BoxDeb(loopDetected);		break;
		case 0XE7:						Dm = " - 0xE7 - RST $20"; BoxDeb(loopDetected);		break;
		case 0XE8:						Dm = " - 0xE8 - ADD SP xx";	BoxDeb(loopDetected);	break;
		case 0XE9:						Dm = " - 0xE9 - JP (HL)";	BoxDeb(loopDetected);	break;
		case 0XEA:						Dm = " - 0xEA - LD ($aabb) A"; BoxDeb(loopDetected);	break;
		case 0XEE:						Dm = " - 0xEE - XOR $xx";	BoxDeb(loopDetected);	break;
		case 0XEF:						Dm = " - 0xEF - RST $28";	BoxDeb(loopDetected);	break;
		case 0XF0:						Dm = " - 0xF0 - LD A ($xx)"; BoxDeb(loopDetected);	break;
		case 0XF1:						Dm = " - 0xF1 - POP AF";	BoxDeb(loopDetected);	break;
		case 0XF2:						Dm = " - 0xF2 - LD A (C)";	BoxDeb(loopDetected);	break;
		case 0XF3: DI();				Dm = " - 0xF3 - DI";					break;
			//is F4 missing?
		case 0XF5:						Dm = " - 0xF5 - PUSH AF";	BoxDeb(loopDetected);	break;
		case 0XF6:						Dm = " - 0xF6 - xx OR $xx";	BoxDeb(loopDetected);	break;
		case 0XF7:						Dm = " - 0xF7 - RST $30";	BoxDeb(loopDetected);	break;
		case 0XF8:						Dm = " - 0xF8 - LD HL SP";	BoxDeb(loopDetected);	break;
		case 0XF9:						Dm = " - 0xF9 - LD SP HL";	BoxDeb(loopDetected);	break;
		case 0XFA:						Dm = " - 0xFA - LD A ($aabb)"; BoxDeb(loopDetected);	break;
		case 0XFB:						Dm = " - 0xFB - EI";	BoxDeb(loopDetected);		break;
		case 0XFE:						Dm = " - 0xFE - CP $xx";	BoxDeb(loopDetected);	break;
		case 0XFF:						Dm = " - 0xFF - RST $38";	BoxDeb(loopDetected);	break;

		default:
			//std::cout << "ERROR CASE NOT FOUND " << "opcode" << std::hex << std::setw(2) << std::setfill('0') << int(opcode) << "\n";
			break;	
		}
		//BoxDeb(loopDetected);
		//std::cout << "Next opcode" << " | " << "Engager: " << std::hex << std::setw(4) << std::setfill('0') << opcode << "-" << Dm << "\n";
		////std::cout << "$aabb: " << std::hex << (int)$aabb << "\n"; // To Dissasemble
		//gb::BoxDeb(loopDetected);
	} //end opDecoder

#pragma endregion

void compareDm()
{
	for (opCounter = 0; opCounter <= 5; opCounter++) {
		if (captureDm) { pastDms[opCounter] = Dm;  }
		else if (captureDm == false) { currentDms[opCounter] = Dm; }
	}
	opCounter = 0;
	captureDm = false;

}
///////////////////////////////////////////////////////////////////////////////////////
	//- Step Debuger and Dissasembler 
	//- This Module is DONE
	///////////////////////////////////////////////////////////////////////////////////////

void Dissam() {
	std::stringstream dissam;
	std::string dissamLine;
	//dTarget y sTarget deben resetearse arriba
	switch (opLen) {
		case 1:
		dissam << "OP: " << std::hex << std::setw(4) << std::setfill('0') << opcode << " - " << Dm;
		break;
		
		case 2:
		dissam << "OP: " << std::hex << std::setw(4) << std::setfill('0') << opcode << " - " << std::hex << (int)$bb << " - " << Dm;;
		break;
		
		case 3:
		dissam << "OP: " << std::hex << std::setw(4) << std::setfill('0') << opcode << " - " << std::hex << (int)$bb << ", " << std::hex << (int)$aa << " - " << Dm;
		break;
}
dissamLine = dissam.str();
SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
TextBlit(dissamLine, 0, 700);
dissam.str(std::string());

}


void BoxDeb( bool& loopDetected) {  
if (opCounter > 24540){  //24515
//if every missing opcode use this i can capture the debug m3ssage with ease. BOXdeb debugger is the not mapped case error
	std::stringstream Debug;
	std::stringstream debugLine_1SS;
	std::string debugLine_1;

	std::stringstream debugLine_2SS;
	std::string debugLine_2;

	std::stringstream debugLine_3SS;
	std::string debugLine_3;

	std::stringstream debugLine_4SS;
	std::string debugLine_4;

	std::stringstream debugLine_5SS;
	std::string debugLine_5;

	std::stringstream debugLine_6SS;
	std::string debugLine_6;
	
	std::bitset<8> debugFlag(registers.Rf);
	std::stringstream MemAccess;
	

		SDL_Rect clearRect;
		
		clearRect.x = 0;
		clearRect.y = 300;
		clearRect.w = 1200 ;
		clearRect.h = 800;
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &clearRect);
		
		Debug << "Executing...:  " << functType << "\n " << "AF:  " << std::hex << std::setw(4) << std::setfill('0') << int(registers.Raf) << "    " << "BC: " << std::hex << std::setw(4) << std::setfill('0') << int(registers.Rbc) << "\n" << "DE: " << std::hex << std::setw(4) << std::setfill('0') << int(registers.Rde) << "    " << "HL: " << std::hex << std::setw(4) << std::setfill('0') << int(registers.Rhl) << "\n"
			<< "PC: " << std::hex << std::setw(4) << std::setfill('0') << int(pc) << "    " << "EI" << EInt << " " << "\n"
			<< "IF: " << std::hex << std::setw(4) << std::setfill('0') << (int)IF << "  " << "DI: " << DInt << "\n"
			<< "SP: " << std::hex << std::setw(4) << std::setfill('0') << (int)sp << "\n"
			<< "Z[" << debugFlag.test(7) << "] N[" << debugFlag.test(6) << "] H[" << debugFlag.test(5) << "] C[" << debugFlag.test(4) << "]\n"
			<< "Current Engager: " << std::hex << std::setw(4) << std::setfill('0') << opcode << "\n"
			<< Dm.c_str();
		
		mergedDebug = Debug.str() + MemAccess.str();
		
		
		std::cout << "//////////////////////////\n"
				  << mergedDebug <<
				//  std::cin.get();
				     "/////////////////////////\n";
				     
				     
				     debugLine_1SS <<  "Executing...:  " << functType;
					 debugLine_3SS << "DE: " << std::hex << std::setw(4) << std::setfill('0') << int(registers.Rde) << "    " << "HL: " << std::hex << std::setw(4) << std::setfill('0') << int(registers.Rhl);
					 debugLine_2SS << "AF:  " << std::hex << std::setw(4) << std::setfill('0') << int(registers.Raf) << "    " << "BC: " << std::hex << std::setw(4) << std::setfill('0') << int(registers.Rbc);
					 debugLine_4SS << "PC: " << std::hex << std::setw(4) << std::setfill('0') << int(pc) << "    " << "EI" << EInt;
					 debugLine_5SS << "IF: " << std::hex << std::setw(4) << std::setfill('0') << (int)IF << "  " << "DI: " << DInt;
					 debugLine_6SS << "Z[" << debugFlag.test(7) << "] N[" << debugFlag.test(6) << "] H[" << debugFlag.test(5) << "] C[" << debugFlag.test(4);
				     debugLine_1 = debugLine_1SS.str();
					 debugLine_2 = debugLine_2SS.str();
					 debugLine_3 = debugLine_3SS.str();
					 debugLine_4 = debugLine_4SS.str();
					 debugLine_5 = debugLine_5SS.str();
					 debugLine_6 = debugLine_6SS.str();
					 
				     
				     debugSDL = mergedDebug;
					 SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
				     TextBlit(debugLine_1, 0, 460);
					 TextBlit(debugLine_2, 0, 500);
					 TextBlit(debugLine_3, 0, 540);
					 TextBlit(debugLine_4, 0, 580);
					 TextBlit(debugLine_5, 0, 620);
					 TextBlit(debugLine_6, 0, 660);
					 //SDL_RenderPresent(renderer);
				     
				     SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Debug M", debugLine_1.c_str(), NULL);
		
					 
				     //SDL_Delay(100);
Debug.str(std::string());
	debugLine_1SS.str(std::string());
	debugLine_2SS.str(std::string());
	debugLine_3SS.str(std::string());
	debugLine_4SS.str(std::string());
	debugLine_5SS.str(std::string());
//	SDL_FreeSurface(blitSurface);
		SDL_DestroyTexture(blitTexture);
		loopDetected = true;
		
		//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Debug M","STOPPING", NULL);

		//

} //end loopDetector
	
}

	void Reset() {
		Ra = 0x1;
		Rb = 0x0;
		Rc = 0x13;
		Rd = 0x0;
		Re = 0xd8;
		Rh = 0x01;
		Rl = 0x4D;
		Rf = 0xb0;
		pc = 0x100;
		sp = 0xFFEE;
		Rm = 0;
		Rt = 0;
		opLen = 1;

		Raf = 0x01;
		Rbc = 0x0013;
		Rde = 0x00d8;
		Rhl = 0x014d;
		//gb::NOP();
	}
	void FlagResearch() {
		
		

		/// Gracias modulo, me ense;aste un monton
		/*
		Rf = 0xB0;
		unsigned char a = 0xf0;
		
		//std::cout << "Rf desde Reg: " << std::hex << (int)Rf << "\n";
		std::bitset<8> x(Rf);

		//std::cout << "Rf transformado: " << std::hex << x << "\n";

		//x.reset(7); // Reset Z
		//x.reset(6); // Reset N
		//x.reset(5); // Reset H
		//x.reset(4); // Reset C

		//x.set(7); // Set Z
		//x.set(6); // Set N
		//x.set(5); // Set H
		//x.set(4); // Set C

		Rf = x.to_ulong();
		//std::cout << "Rf Bitseteado: " << std::hex << (int)Rf << "\n";

		*/
		} // Modulo aprendizaje
		
		
		void Boot() {
		Ra = 0x0;
		Rb = 0x0;
		Rc = 0x00;
		Rd = 0x0;
		Re = 0x0;
		Rh = 0x0;
		Rl = 0x0;
		Rf = 0x0;
		pc = 0;
		sp = 0;
		Rm = 0;
		Rt = 0;
		//opLen = 1;

		Raf = 0x0;
		Rbc = 0x0;
		Rde = 0x0;
		Rhl = 0x0;
		//gb::NOP();
	}
	
	void LoadBios() {
		std::ifstream fin(BIOS, std::ios::binary);
		if (!fin)	{	//std::cout << "File not found " << std::endl;	
		}
		for (int i = 0; i < 0x8000; i++)	{	fin.get(letter);	memoryA[i] = letter;	}
	}; // fin LoadBios
	
	void LoadFile()
	{
		std::ifstream fin(ROM, std::ios::binary);
		if (!fin)	{	//std::cout << "File not found " << std::endl;	
		}
		for (int i = 0; i < 0x8000; i++)	{	fin.get(letter);	memoryA[i] = letter;	}
	}; // fin LoadFile

}; //-- End class GB

//-- Modulo Main
int main(int argc, char *argv[])
{
	//experimental SDL support
	nullRender = false;
	
	if (!nullRender)
	{
		TTF_Init();
		if (!fontLoaded)
		{ 
		TTF_Font * font;
		font = TTF_OpenFont("arial.ttf", 25);
		font1 = TTF_OpenFont("arial.ttf", 40);
		fontLoaded = true;
		if (font == NULL) { std::cout << "FONT NOT LOADED\n"; std::cin.get(); }
		else { std::cout << "SDL_TTF READY TO ROLL\n"; std::cin.get(); }
		}
		
		SDL_Window *window = NULL;
		// SDL_Window* debugger = NULL;

		window = SDL_CreateWindow
			("GBdroid", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	}
	
	///////////////////////////////////////////////////////////////////////////////////////
	//- On Init - Set Base values to Regs, Load Rom from memory, Assign Memory to Mem Array
	//- A little Choppy Atm - Room for improvement
	///////////////////////////////////////////////////////////////////////////////////////
    
    bool animateF = false;
	gb gameboy;
	Buttons stepModule(400,600);
	Buttons animate(800,600);
	//gameboy.Reset();
	gameboy.LoadFile();
	
	
	//-- Creating VRAM peeker all Functionality Disabled ///////////////////// 
	gameboy.LoadBios();
	gameboy.Boot();
	
	///////////////////////////////////////////////////////////////////////////////////////
	//- Main execution loop
	//- Currently Catching Loops Instances
	///////////////////////////////////////////////////////////////////////////////////////
while (1) {
	//gameboy.RegComb();
	gameboy.opDecoder(loopDetected, opCounter);
    
    
    
    //
	opCounter++;
	if ( loopDetected == true) {
	gameboy.Vram2();
	SDL_RenderPresent(renderer);

			switch (event.type)
			{
			case SDL_QUIT:
				{

				//	TTF_CloseFont(font);
					//SDL_DestroyWindow(window);
					// SDL_DestroyWindow(debugger);
					SDL_Quit();
				//	TTF_CloseFont(font);
				}
		
				// done = true;
				break;

			case SDL_FINGERDOWN: {
				stepModule.iddleButton();
				animate.iddleButton();
				
				if (animate.isActive) {
					//animateF = true;
				}
				
				else if (stepModule.isActive) {
					running = true;
				}
			}
					break;
				
					case SDL_FINGERUP:{
						stepModule.activeButton();
						//animate.activeButton();
						//stepModule.isIddle = true;
						//animate.isIddle = true;
						break;
					}
			} //SDL event
		
	
	
    
    } //end LoopDetected = true
    
	} // fi  while
	
  
	
	 
	/*
	if (!animateF) { running = true; }
		

	
	} // fin While 2
*/
		
		

	return 0;
}