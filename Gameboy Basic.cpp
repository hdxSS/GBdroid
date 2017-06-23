// Gameboy Basic.cpp : Defines the entry point for the console application.
//

#include "stdafx.h" // Windows only
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <bitset>
#include <string>
#include <sstream>
#include <SDL_ttf.h>
#include <SDL.h> //for windows
//#include <SDL2/SDL.h> //for android
#include <random>

//Modules
//MMU - Started Structure - Just Reporting ATM
//CPU -- Function mapping and Opcode mapping - Current TASK
//Timers -- 50% Need to actually update them
//Interrupts - 0%
//FlagHandling - DONE 100%
//Register combining - DONE 100%
//Gfx - Researching SDL Structure Groundwork ready
//Sound - 0%
//Input - 0%

//SDL support

SDL_Renderer* renderer = NULL;

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
int opCounter;
long loopInstances;
bool matchFound = true;
bool loopDetected;
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
int size = 5;
int numCol = 0;
int numRow = 0;


unsigned short _16bitIn;
unsigned long fullOpcode;
unsigned char $bb;
unsigned char $aa;
signed int $xx;
unsigned char _8bitIn3;

unsigned char TempRf;
/////////////

class gb
{
public:
	unsigned short Raf, Rbc, Rde, Rhl;
	unsigned char Ra, Rf, Rb, Rc, Rd, Re, Rl, Rh;
	unsigned short pc, sp;
	unsigned char opclock = 0;
	unsigned long cpuClock = 0;
	unsigned char mathTC1 = 0;
	unsigned char mathTC2 = 0;
	unsigned char mathTS1 = 0;
	unsigned char mathTS2 = 0;
	
	std::string prevM;
	std::string actualM;

	unsigned short IF;
	bool DInt = false;
	bool EInt = true;
	bool memCatchFlag = false;

	unsigned char TIMA, TMA, TAC, NR10, NR11, NR12, NR14, NR21, NR22, NR24, NR30, NR31, NR32, NR33, NR41, NR42, NR43, NR50, NR51, NR52, LCDC, SCY, SCX, LYC, BGP, OBP0, OBP1, WY, WX ;



	unsigned char Z,N,H,C;
	float Rm, Rt;
///////////////////////////////////////////////////////////////////////////////////////
//- Graphics Related code 
//- This section can be enabled or disabled, but is meant for research
///////////////////////////////////////////////////////////////////////////////////////		
	SDL_Rect pixelVram[8192]; // la pantalla es de 256 x 256 pero 144 y 164 son visibles
	
	
	void VRAMmanagerInit() {
		for (int i = 0; i <= 8281; i++)
		{
			std::cout << i << "\n";
			vramGfx[i] = 1;
			//Vram[i] = 1;
		}
	}
	
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
		if ((int)vramGfx[i] == 0x1) {
			
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			//std::cout << "Detected 1" << "\n";
		}
		else if ((int)vramGfx[i] == 0x0) {
			//std::cout << "Detected 0 in " << std::dec << i << " of coordinates:  " << std::dec << (int)numCol << "," << std::dec << (int)numRow << "\n";
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		}
			
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
		SDL_RenderPresent(renderer);
		//}//Video support
	}//gfxHandler

	// reemplazar desde afuera de GFX handler
	

///////////////////////////////////////////////////////////////////////////////////////
//- Im using the MMU as a BP for when the CPU is mature enough to start accesing VRAM 
//- This will be a Major milestone in the project
///////////////////////////////////////////////////////////////////////////////////////
	void MMUexp(unsigned short toTarget, unsigned short fromSource, std::string sourceReg, int WriteType) {

		switch (WriteType)
		{
		case 1: // Read 8 bit from memory (I need a source[Mem] and a Target[Reg])
			if (fromSource < 0x4000) { std::cout << "Reading from ROM bank0" << "\n"; }
			else if (fromSource >= 0x4000 && fromSource < 0x8000) { std::cout << "Reading MMU ROM bank1" << "\n"; }
			else if (fromSource >= 0x8000 && fromSource < 0xA000) { std::cout << "Reading MMU GFX" << "\n"; 
			
			}		
			else if (fromSource >= 0xA000 && fromSource < 0xC000) { std::cout << "Reading MMU EXT RAM bank" << "\n"; }
			else if (fromSource >= 0xC000 && fromSource < 0xE000) { std::cout << "Reading MMU WORKING RAM" << "\n"; }
			else if (fromSource >= 0xE000 && fromSource < 0xFE00) { std::cout << "Reading MMU WORKING RAM SHADOW" << "\n"; }
			else if (fromSource >= 0xFE00 && fromSource < 0xFF00) { std::cout << "Reading MMU GFX SPRITES" << "\n"; }
			else if (fromSource >= 0xFF00 && fromSource < 0xFF80) { std::cout << "Reading MMU INPUT" << "\n"; }
			else if (fromSource >= 0xFF80 && fromSource < 0xFFFF) { std::cout << "Reading MMU ZERO PAGE RAM" << "\n"; }
			break;
		

	case 2: // Write 8 bit to memory (I need a source[Reg] and a Target[Mem])
		if (toTarget < 0x4000) { std::cout << "Writing from ROM bank0" << "\n"; }
		else if (toTarget >= 0x4000 && toTarget < 0x8000) { std::cout << "Writing MMU ROM bank1" << "\n"; }
		else if (toTarget >= 0x8000 && toTarget < 0xA000) { std::cout << "Writing MMU GFX" << "\n"; }
		else if (toTarget >= 0xA000 && toTarget < 0xC000) { std::cout << "Writing MMU EXT RAM bank" << "\n"; }
		else if (toTarget >= 0xC000 && toTarget < 0xE000) { std::cout << "Writing MMU WORKING RAM" << "\n"; }
		else if (toTarget >= 0xE000 && toTarget < 0xFE00) { std::cout << "Writing MMU WORKING RAM SHADOW" << "\n"; }
		else if (toTarget >= 0xFE00 && toTarget < 0xFF00) { std::cout << "Writing MMU GFX SPRITES" << "\n"; }
		else if (toTarget >= 0xFF00 && toTarget < 0xFF80) { std::cout << "Writing MMU INPUT" << "\n"; }
		else if (toTarget >= 0xFF80 && toTarget < 0xFFFF) { std::cout << "Writing MMU ZERO PAGE RAM" << "\n"; }
		break;

	case 3: // Read 16 bit from memory (I need a source[Mem] and a Target[Reg]
		if (fromSource < 0x4000) { std::cout << "Reading from ROM bank0" << "\n"; }
		else if (fromSource >= 0x4000 && fromSource < 0x8000) { std::cout << "Reading MMU ROM bank1" << "\n"; }
		else if (fromSource >= 0x8000 && fromSource < 0xA000) { std::cout << "Reading MMU GFX" << "\n"; }
		else if (fromSource >= 0xA000 && fromSource < 0xC000) { std::cout << "Reading MMU EXT RAM bank" << "\n"; }
		else if (fromSource >= 0xC000 && fromSource < 0xE000) { std::cout << "Reading MMU WORKING RAM" << "\n"; }
		else if (fromSource >= 0xE000 && fromSource < 0xFE00) { std::cout << "Reading MMU WORKING RAM SHADOW" << "\n"; }
		else if (fromSource >= 0xFE00 && fromSource < 0xFF00) { std::cout << "Reading MMU GFX SPRITES" << "\n"; }
		else if (fromSource >= 0xFF00 && fromSource < 0xFF80) { std::cout << "Reading MMU INPUT" << "\n"; }
		else if (fromSource >= 0xFF80 && fromSource < 0xFFFF) { std::cout << "Reading MMU ZERO PAGE RAM" << "\n"; }
		break;

	case 4: // Write 16 bit to memory (I need a source[Reg] and a Target[Mem]
		if (toTarget < 0x4000) { std::cout << "Writing from ROM bank0" << "\n"; }
		else if (toTarget >= 0x4000 && toTarget < 0x8000) { std::cout << "Writing MMU ROM bank1" << "\n"; }
		else if (toTarget >= 0x8000 && toTarget < 0xA000) { std::cout << "Writing MMU GFX" << "\n";
		
		vramGfx[toTarget - 0x8000] = fromSource;
		//std::cout << "Writing " << sourceReg << std::hex << (int)fromSource << "] to Mem region [0x" << std::hex << (int)toTarget << "]\n";
		//std::cout << "Parsing mem to VramGfx Pos = " << std::dec << int(toTarget - 0x8000) << "\n";
		}
		else if (toTarget >= 0xA000 && toTarget < 0xC000) { std::cout << "Writing MMU EXT RAM bank" << "\n"; }
		else if (toTarget >= 0xC000 && toTarget < 0xE000) { std::cout << "Writing MMU WORKING RAM" << "\n"; }
		else if (toTarget >= 0xE000 && toTarget < 0xFE00) { std::cout << "Writing MMU WORKING RAM SHADOW" << "\n"; }
		else if (toTarget >= 0xFE00 && toTarget < 0xFF00) { std::cout << "Writing MMU GFX SPRITES" << "\n"; }
		else if (toTarget >= 0xFF00 && toTarget < 0xFF80) { std::cout << "Writing MMU INPUT" << "\n"; }
		else if (toTarget >= 0xFF80 && toTarget < 0xFFFF) { std::cout << "Writing MMU ZERO PAGE RAM" << "\n"; }
		break;
		break;
	}
	
	} // MMU EXP 

	void MMU() {
		//THIS WONT WORK. the program counter does not  "GO" Into a memory address
		//Registers write and access memory from their own functions
	
		//How could i capure a certain memory section?
		//For gfx i could say... if GFX range is != 0 that would mean there is something written in one of the mems
		if (pc < 0x4000) {
			//std::cout << "MMU ROM bank0" << "\n";
		}
		else if (pc >= 0x4000 && pc < 0x8000) { //std::cout << "MMU ROM bank1" << "\n"; 
		}
		else if (pc >= 0x8000 && pc < 0xA000) { //std::cout << "MMU GFX" << "\n"; std::cin.get();			
		} //Break point GFX
		else if (pc >= 0xA000 && pc < 0xC000) { //std::cout << "MMU EXT RAM bank" << "\n";				
		}
		else if (pc >= 0xC000 && pc < 0xE000) { //std::cout << "MMU WORKING RAM" << "\n";					
		}
		else if (pc >= 0xE000 && pc < 0xFE00) { //std::cout << "MMU WORKING RAM SHADOW" << "\n";			
		}
		else if (pc >= 0xFE00 && pc < 0xFF00) { //std::cout << "MMU GFX SPRITES" << "\n"; std::cin.get(); 
		} //Break point GFX
		else if (pc >= 0xFF00 && pc < 0xFF80) { //std::cout << "MMU INPUT" << "\n";						
		}
		else if (pc >= 0xFF80 && pc < 0xFFFF) { //std::cout << "MMU ZERO PAGE RAM" << "\n";				
		}
	}

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

		std::bitset<8> Rf_as_bit(Rf);
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
		Rf = Rf_as_bit.to_ulong();
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
		if (pc != 0x100) {
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
		opDeb();
		pc += opLen;
		////-- Instruction preset

	*/ //Hidden Instruction Preset //
	//Instruction Preset

	void ADC8RhlM(unsigned short &$any, std::string Dtarget, std::string Starget) { // n + carry flag to A
		////-- Instruction preset
		opLen = 1;
		FLGH(2, 0, 2, 2, $any, Ra, 1 );
		Ra = ($any + TempRf);
		funcText << "ADC " << Dtarget << ", " << Starget << "(" << std::hex << (int)$any << ")";
		functType = funcText.str();
		opDeb();
		pc += opLen;
		////-- Instruction preset
	}

	void ADCA$xx(unsigned short $any, std::string Dtarget, std::string Starget)  {
		////-- Instruction preset
		opLen = 2;
		FLGH(2, 0, 2, 2, $any, Ra, 1);
		Ra = ($any + TempRf);
		funcText << "ADC " << Dtarget << ", " << Starget << "(" << std::hex << (int)$any << ")";
		functType = funcText.str();
		opDeb();
		pc += opLen;
		////-- Instruction preset
	}
	void ADCR8(unsigned short &$any, std::string Dtarget, std::string Starget) {
		////-- Instruction preset
		opLen = 1;
		FLGH(2, 0, 2, 2, $any, Ra, 1);
		Ra = ($any + TempRf);
		funcText << "ADC " << Dtarget << ", " << Starget << "(" << std::hex << (int)$any << ")";
		functType = funcText.str();
		opDeb();
		pc += opLen;
		////-- Instruction preset
	}
	void ADDAhlM(std::string Dtarget, std::string Starget) {
		
		////-- Instruction preset
		opLen = 1;
		opDeb();
		FLGH(2, 0, 2, 2, Ra, memoryA[Rhl], 1);
		Ra += memoryA[Rhl];
		funcText << "ADD " << Dtarget << ", " << Starget << "(" << std::hex << (int)memoryA[Rhl] << ")";
		functType = funcText.str();
		pc += opLen;
		////-- Instruction preset

	}
	void ADDA$xx(unsigned char _8int) {
		//FLGH(2, 0, 2, 2);
		//CALCFLG(Ra, _8int, 1);
		Ra += _8int;
		opclock = 2;
		functType = "ADD $xx ";

	}
	void ADDA8R(unsigned char &_8r, std::string Dtarget, std::string Starget) {
		////-- Instruction preset
		opLen = 1;
		FLGH(2, 0, 1, 0, Ra, _8r, 1);
		Ra += _8r;
		funcText << "AND " << Dtarget << ", " << Starget;
		functType = funcText.str();
		opDeb();
		pc += opLen;
		////-- Instruction preset
	}
	void ADDHL16R(unsigned short &_R16) {
		//FLGH(3, 0, 2, 2);
		//CALCFLG(Rhl, _R1, 1);
		Rhl += _R16;
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
		Ra &= _8int;
		opclock = 2;
		functType = "AND A $xx ";
	}
	void ANDA8R(unsigned char &_8r, std::string Dtarget, std::string Starget) {
		////-- Instruction preset
		opLen = 1;
		FLGH(2, 0, 1, 0, Ra, _8r, 4);
		Ra &= _8r;
		funcText << "AND " << Dtarget << ", " << Starget;
		functType = funcText.str();
		opDeb();
		pc += opLen;
		////-- Instruction preset
	}
	void BIThlM(unsigned char bitnum) {

		switch (memoryA[Rhl] << bitnum) { // refinar 0x1
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
			opDeb();
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
	void CALLpc16BA(unsigned short _16BA) {
		pc = _16BA;
		// mal la dif entre jump y call
		//skip
		opclock = 3;
		functType = "CALL pc $aabb ";
		//FLGH(3, 3, 3, 3);
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
		memoryA[Rhl]--;
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
		opDeb();
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
		memoryA[Rhl]++;
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
		opDeb();
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
		opDeb();
		pc += opLen;
		////-- Instruction preset
	}
	void JUMPhlM() {
		pc = memoryA[Rhl];

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
		opDeb();
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
		opDeb();
		std::bitset<8> flagChecker(Rf);
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
		Ra = memoryA[Rc];
		//update timers
		opclock = 1;
		//update flags
		functType = "LOAD C (A) ";
		//FLGH(3, 3, 3, 3);
	}
	void LOADhlM$xx(unsigned char _8int) {
		memoryA[Rhl] = _8int;
		//update timers
		opclock = 2;
		//update flags
		functType = "LOAD (HL) $xx ";
		//FLGH(3, 3, 3, 3);
	}
	void LOADhlM8R(unsigned char &_8r) {
		memoryA[Rhl] = _8r;
		//update timers
		opclock = 1;
		//update flags
		functType = "LOAD (HL) R8 ";
		///FLGH(3, 3, 3, 3);
	}
	void LOAD$aabbMRA(unsigned short _16ba) {
		memoryA[_16ba] = Ra;
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
		memoryA[_16r] = Ra;
		//update timers
		opclock = 1;
		//update flags
		functType = "LOAD (16R) A ";
		//FLGH(3, 3, 3, 3);
	}
	void LOADRACM(unsigned char &_8r) {
		Ra = memoryA[_8r];
		//update timers
		opclock = 1;
		//update flags
		functType = "LOAD A (C) - REVISAR ";
		//FLGH(3, 3, 3, 3);
		///////////////////REVISAR PUEDE ESTAR MALO
	}
	void LOADRA$aabb(unsigned short _16ba) {
		Ra = memoryA[_16ba];
		//update timers
		opclock = 3;
		//update flags
		functType = "LOAD A $aabb ";
		//FLGH(3, 3, 3, 3);
	}
	void LOADRA16RM(unsigned short _16r, std::string Starget) {
		////-- Instruction preset
		opLen = 2;
		Ra = memoryA[_16r];
		FLGH(3, 3, 3, 3, NULL, NULL, NULL);
		funcText << "LOAD A, (" << Starget << ") |" << std::hex << std::setw(4) << std::setfill('0') << (int)memoryA[_16r] << "|";
		functType = funcText.str();
		opDeb();
		pc = opLen;
		////-- Instruction preset
	}
	void LOADRHLSPOFF(unsigned char _8boff) {
		//FLGH(0, 0, 2, 2);
		Rhl = (sp + _8boff);
		//update timers
		opclock = 2;
		//update flags
		functType = "LOAD HL SPOFF";
	}
	void LOADR16$aabb(unsigned short &_16r, unsigned short $_aabb, std::string Dtarget) {
		////-- Instruction preset
		opLen = 3;
		FLGH(3, 3, 3, 3, NULL, NULL, NULL);
		_16r = $_aabb;
		MMUexp($_aabb, NULL,Dtarget, 3);
		funcText << "LOAD " << Dtarget << ", "  << std::hex << std::setw(4) << std::setfill('0') << (int)$_aabb;
		functType = funcText.str();
		opDeb();
		if (Dtarget == "HL") { Rh = $aa; Rl = $bb; }
		else if (Dtarget == "AF") { Ra = $aa; Rf = $bb; }
		else if (Dtarget == "BC") { Rb = $aa; Rc = $bb; }
		else if (Dtarget == "DE") { Rd = $aa; Re = $bb; }
		memCatch =$_aabb;
		memCatchFlag = true;
		pc += opLen;
		////-- Instruction preset
	}
	void LOADR8hlM(unsigned char &_8r) {
		_8r = memoryA[Rhl];
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
		_8r = _$xx;
		funcText << "LOAD " << Dtarget << ", " << std::hex << std::setw(4) << std::setfill('0') << (int)_$xx;
		functType = funcText.str();
		opDeb();
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
		opDeb();
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
		Ra = memoryA[Rhl];
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
		memoryA[Rhl] = _R8;
		MMUexp(Rhl,_R8,Starget, 4);
		funcText << "LOADD " << "(HL) ," << Starget;
		functType = funcText.str();
		//std::cout << "MEMORY RHL DESP: " << std::dec << (int)vramGfx[(8191)] << "\n";
		opDeb();
		if (Dtarget == "(HL)") {
			if (Rl == 0) { Rh--; Rl = 0xFF; }
			else if (Rl > -1) { Rl--; }
		}
			
		pc += opLen;
		////-- Instruction preset

	}
	void LOADHAOFF(signed int $_xx, unsigned char R8, std::string Starget) { //LDH
		////-- Instruction preset
		opLen = 2;
		FLGH(3, 3, 3, 3, NULL, NULL, NULL);
		memoryA[0xff00 + $_xx] = Ra;
		funcText << "LDH " << std::hex << (int)$_xx << ", " << Starget;
		functType = funcText.str();
		opDeb();
		pc += opLen;
		////-- Instruction preset
	}
	void LOADOFFR8A(unsigned char _R8, std::string Starget) { //LOAD A into R8 + 0xff00
		//It would be wise to know which part of memory is trying to access
		memoryA[0xff00 + _R8] = Ra;
		////-- Instruction preset
		opLen = 2;
		FLGH(3, 3, 3, 3, NULL, NULL, NULL);
		funcText << "LOAD ( 0xff00 + " << Starget << "|" << std::hex << (int)_R8 << "| ) ,A";
		functType = funcText.str();
		opDeb();
		pc += opLen;
		////-- Instruction preset
	}
	void LDIhlMRA() {
		memoryA[Rhl] = Ra;
		Ra++;
		//update timers
		opclock = 1;
		//update flags
		functType = " LDI (HL) A ";
		//FLGH(3, 3, 3, 3);
	}
	void LDIRAhlM() {
		Ra = memoryA[Rhl];
		memoryA[Rhl]++;
		//update timers
		opclock = 1;
		//update flags
		functType = "LDI A (HL) ";
	//	FLGH(3, 3, 3, 3);
	}
	void NOP() {
		opDeb();
		//FLGH(3, 3, 3, 3);
		//pc += opLen;
		functType = "NOP ";
	}
	void ORhlMRA() {
	//	FLGH(2, 0, 0, 0);
		memoryA[Rhl] |= Ra;
		opclock = 1;
		functType = "OR (HL) A ";
	}
	void ORRA$xx(unsigned char _8int) {
	//	FLGH(2, 0, 0, 0);
		Ra |= _8int;
		opclock = 2;
		functType = "OR ADC R8 ";
	}
	void ORRA8R(unsigned char &_r8, std::string Dtarget, std::string Starget) {
		////-- Instruction preset
		opLen = 1;
		FLGH(2, 0, 0, 0, Ra, _r8, 5);
		Ra |= _r8;
		funcText << "OR " << Dtarget << ", " << Starget;
		functType = funcText.str();
		opDeb();
		pc += opLen;
		////-- Instruction preset
	}
	void POP16R(unsigned &_16r) {
		memoryA[sp] = _16r;
		sp--;
		opclock = 1;
		functType = "POP 16R ";
	//	FLGH(3, 3, 3, 3);
	}
	void PUSH() {
		memoryA[pc] = sp;
		sp++;
		opclock = 1;
		functType = "PUSH ";
	//	FLGH(3, 3, 3, 3);
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
	void RLR8(unsigned char &_8r, unsigned char &_8r2) {
	//	FLGH(2, 0, 0, 2);
		opclock = 2;
		functType = "RL 8R ";
	}
	void RLA() {
	//	FLGH(0, 0, 0, 2);
		opclock = 1;
		functType = "RL A ";
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
	void RLCA() {
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
		FLGH(2, 1, 2, 2, Ra, _8r, 2);
		Ra -= _8r;
		funcText << "SUB " << Dtarget << ", " << Starget;
		functType = funcText.str();
		opDeb();
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
		FLGH(2, 0, 0, 0, Ra, _r8, 6);
		Ra ^= _r8;
		funcText << "XOR " << std::hex << std::setw(4) << std::setfill('0') << Ra << ", " << std::hex << std::setw(4) << std::setfill('0') << _r8;
		functType = funcText.str();
		opDeb();
		pc += opLen;
		////-- Instruction preset
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
	//- Step Debuger and Dissasembler 
	//- This Module is DONE
	///////////////////////////////////////////////////////////////////////////////////////

	void opDeb() {

		if (pc == 0x100)
		{
			MMU();
			//std::cout << "Executing line: " << std::hex << "pc:" << pc << " -> " << opcode << " " << "NOP" << "\n";
			//FLGH(3, 3, 3, 3);
			pc += 1;
			opLen = 3;
		}
		else {

			if (opLen == 1) {
				//std::cout << "--------------------------------------------------------" << "\n";
				MMU();
				//std::cout << "--------------------------------------------------------" << "\n";
				//std::cout << "Executing line: " << std::hex << "pc: " << std::setw(4) << std::setfill('0') << pc << " -> " << std::setw(4) << std::setfill('0') << opcode << " | " << functType << "\n";
				//std::cout << "--------------------------------------------------------" << "\n";
				////std::cout << "$aabb: " << std::hex << (int)$aabb << "\n";
			}

			else if (opLen == 2) {
				//std::cout << "--------------------------------------------------------" << "\n";
				MMU();
				//std::cout << "--------------------------------------------------------" << "\n";
				//std::cout << "Executing line: " << std::hex << "pc: " << std::setw(4) << std::setfill('0') << pc << " -> " << std::setw(4) << std::setfill('0') << opcode << " " << std::hex << std::setw(2) << std::setfill('0') << (int)$bb << " | " << functType << "\n";
				//std::cout << "--------------------------------------------------------" << "\n";
				////std::cout << "$aabb: " << std::hex << (int)$aabb << "\n";
			}

			else if (opLen == 3) {
				//std::cout << "--------------------------------------------------------" << "\n";
				MMU();
				//std::cout << "--------------------------------------------------------" << "\n";
				//std::cout << "Executing line: " << std::hex << "pc: " << std::setw(4) << std::setfill('0') << pc << " -> " << std::setw(4) << std::setfill('0') << opcode << " " << std::hex << std::setw(2) << std::setfill('0') << (int)$bb << " " << std::setw(2) << std::setfill('0') << (int)$aa << " | " << functType << "\n";
				//std::cout << "--------------------------------------------------------" << "\n";
				////std::cout << "$aabb: " << std::hex << (int)$aabb << "\n";
			}
			else
			{
				//std::cout << "Como diablos me sali del ciclo???" << "Cuanto vale opLen?: " << (int(opLen)) << "\n";
			}
		}
		funcText.str(std::string());
		//functType = "";
	}

	///////////////////////////////////////////////////////////////////////////////////////
	//- Opcode table with debug info - Started
	//- This will be a Major milestone in the project (Current TASK)
	///////////////////////////////////////////////////////////////////////////////////////

	void opDecoder() {
		
		_16bitIn = (memoryA[pc + 2] << 8) | memoryA[pc + 3];
		fullOpcode = memoryA[pc] << 8 | memoryA[pc + 1];
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
		switch (opcode & 0x00FF) {
		case 0X00:	NOP();				Dm = " - 0x00 - NOP";			break;
		case 0X01:						Dm = " - 0x01 - LD BC $aabb";	break;
		case 0X02:						Dm = " - 0x02 - LD (BC) $aabb";	break;
		case 0X03:INC16R(Rbc, "BC");	Dm = " - 0x03 - INC BC";		break;
		case 0X04:INC8R(Rb, "B");		Dm = " - 0x04 - INC B";			break;
		case 0X05:DEC8R(Rb, "B");		Dm = " - 0x05 - DEC B";			break;
		case 0X06:LDR8$xx(Rb, (int)$xx, "B"); Dm = " - 0x06 - LD B $xx";		break;
		case 0X07:						Dm = " - 0x07 - RLCA";			break;
		case 0X08:						Dm = " - 0x08 - LD ($aabb) SP";	break;
		case 0X09:						Dm = " - 0x09 - ADD HL BC";		break;
		case 0X0A:						Dm = " - 0x0A - LD A (BC)";		break;
		case 0X0B:						Dm = " - 0x0B - DEC BC";		break;
		case 0X0C:INC8R(Rc, "C");		Dm = " - 0x0C - INC C";			break;
		case 0X0D:DEC8R(Rc, "C");		Dm = " - 0x0D - DEC C";			break;
		case 0X0E:LDR8$xx(Rc, (int)$xx, "C"); Dm = " - 0x0E - LD C $xx";		break;
		case 0X0F:						Dm = " - 0x0F - RRCA";			break;
		case 0X10:						Dm = " - 0x10 - STOP";			break;
		case 0X11:LOADR16$aabb(Rde, $aabb, "DE");  Dm = " - 0x11 - LD DE $aabb";	break;
		case 0X12:						Dm = " - 0x12 - LD (DE) A";		break;
		case 0X13:INC16R(Rde, "DE");	Dm = " - 0x13 - INC DE";		break;
		case 0X14:INC8R(Rd, "D");		Dm = " - 0x14 - INC D";			break;
		case 0X15:DEC8R(Rd, "D");		Dm = " - 0x15 - DEC D";			break;
		case 0X16:LDR8$xx(Rd, (int)$xx, "D");	Dm = " - 0x15 - LD D $xx ";		break;
		case 0X17:						Dm = " - 0x17 - RLA";			break;
		case 0X18:						Dm = " - 0x18 - JR $xx";		break;
		case 0X19:						Dm = " - 0x19 - ADD HL DE";		break;
		case 0X1A:LOADRA16RM(Rde, "DE");Dm = " - 0x1A - LD A (DE)";		break;
		case 0X1B:						Dm = " - 0x1B - DEC DE";		break;
		case 0X1C:INC8R(Re, "E");		Dm = " - 0x1C - INC E";			break;
		case 0X1D:DEC8R(Re, "E");		Dm = " - 0x1D - DEC E";			break;
		case 0X1E:LDR8$xx(Re, (int)$xx, "E");	Dm = " - 0x1E - LD E $xx";		break;
		case 0X1F:						Dm = " - 0x1F - RRA";			break;
		case 0X20:JRCC$xx("NZ", (int)$xx);	Dm = " - 0x20 - JR NZ $xx";		break;
		case 0X21:LOADR16$aabb(Rhl, $aabb , "HL");	Dm = " - 0x21 - LD HL $aabb";	break;
		case 0X22:						Dm = " - 0x22 - LD (HLI) A";	break;
		case 0X23:INC16R(Rhl, "HL");	Dm = " - 0x23 - INC HL";		break;
		case 0X24:INC8R(Rh, "H");		Dm = " - 0x24 - INC H";			break;
		case 0X25:DEC8R(Rh, "H");		Dm = " - 0x25 - DEC H";			break;
		case 0X26:LDR8$xx(Rh, (int)$xx, "H");	Dm = " - 0x26 - LD H $xx";		break;
		case 0X27:						Dm = " - 0x27 - DAA";			break;
		case 0X28:						Dm = " - 0x28 - JR Z $xx";		break;
		case 0X29:						Dm = " - 0x29 - ADD HL HL";		break;
		case 0X2A:						Dm = " - 0x2A - LD A (HLI)";	break;
		case 0X2B:						Dm = " - 0x2B - DEC HL";		break;
		case 0X2C:INC8R(Rl, "L");		Dm = " - 0x2C - INC L";			break;
		case 0X2D:DEC8R(Rl, "L");		Dm = " - 0x2D - DEC L";			break;
		case 0X2E:LDR8$xx(Rl, (int)$xx, "L");	Dm = " - 0x2E - LD L $xx";		break;
		case 0X2F:						Dm = " - 0x2F - CPL";			break;
		case 0X30:						Dm = " - 0x30 - JR NC $xx";		break;
		case 0X31:LOADR16$aabb(sp, $aabb,"SP");				Dm = " - 0x31 - LD SP $aabb";	break;
		case 0X32:LOADDhlMR8(Ra, "(HL)", "A");	Dm = " - 0x32 - LDD (HLD) A";	break;
		case 0X33:INC16R(sp, "SP");		Dm = " - 0x33 - INC SP";		break;
		case 0X34:						Dm = " - 0x34 - INC (HL)";		break;
		case 0X35:						Dm = " - 0x2B - DEC HL";		break;
		case 0X36:						Dm = " - 0x36 - LD (HL) $xx";	break;
		case 0X37:						Dm = " - 0x37 - SCF";			break;
		case 0X38:						Dm = " - 0x38 - JR C $xx";		break;
		case 0X39:						Dm = " - 0x39 - ADD HL SP";		break;
		case 0X3A:						Dm = " - 0x3A - LD A (HLD)";	break;
		case 0X3B:						Dm = " - 0x3B - DEC SP";		break;
		case 0X3C:INC8R(Ra, "A");		Dm = " - 0x3C - INC A";			break;
		case 0X3D:DEC8R(Ra, "A");		Dm = " - 0x3D - DEC A";			break;
		case 0X3E:LDR8$xx(Ra, $xx, "A");	Dm = " - 0x3E - LD A $xx";		break;
		case 0X3F:						Dm = " - 0x3F - CCF";			break;
		case 0X40:LOADR8R8(Rb, Rb, "B", "B");	Dm = " - 0x40 - LD B B";		break;
		case 0X41:LOADR8R8(Rb, Rc, "B", "C");	Dm = " - 0x41 - LD B C";		break;
		case 0X42:LOADR8R8(Rb, Rd, "B", "D");	Dm = " - 0x42 - LD B D";		break;
		case 0X43:LOADR8R8(Rb, Re, "B", "E");	Dm = " - 0x43 - LD B E";		break;
		case 0X44:LOADR8R8(Rb, Rh, "B", "H");	Dm = " - 0x44 - LD B H";		break;
		case 0X45:LOADR8R8(Rb, Rl, "B", "L");	Dm = " - 0x45 - LD B L";		break;
		case 0X46:LOADR8R8(Rb, memoryA[Rhl], "B", "(HL)");	Dm = " - 0x46 - LD B (HL)";		break;
		case 0X47:LOADR8R8(Rb, Ra, "B", "A");	Dm = " - 0x47 - LD B A";		break;
		case 0X48:LOADR8R8(Rc, Rb, "C", "B");	Dm = " - 0x48 - LD C B";		break;
		case 0X49:LOADR8R8(Rc, Rc, "C", "C");	Dm = " - 0x49 - LD C C";		break;
		case 0X4A:LOADR8R8(Rc, Rd, "C", "D");	Dm = " - 0x4A - LD C D";		break;
		case 0X4B:LOADR8R8(Rc, Re, "C", "E");	Dm = " - 0x4B - LD C E";		break;
		case 0X4C:LOADR8R8(Rc, Rh, "C", "H");	Dm = " - 0x4C - LD C H";		break;
		case 0X4D:LOADR8R8(Rc, Rl, "C", "L");	Dm = " - 0x4D - LD C L";		break;
		case 0X4E:LOADR8R8(Rc, memoryA[Rhl], "C", "(HL)");	Dm = " - 0x4E - LD C (HL)";		break;
		case 0X4F:LOADR8R8(Rc, Ra, "C", "A");	Dm = " - 0x4F - LD C A";		break;
		case 0X50:LOADR8R8(Rd, Rb, "D", "B");	Dm = " - 0x50 - LD D B";		break;
		case 0X51:LOADR8R8(Rd, Rc, "D", "C");	Dm = " - 0x51 - LD D C";		break;
		case 0X52:LOADR8R8(Rd, Rd, "D", "D");	Dm = " - 0x52 - LD D D";		break;
		case 0X53:LOADR8R8(Rd, Re, "D", "E");	Dm = " - 0x53 - LD D E";		break;
		case 0X54:LOADR8R8(Rd, Rh, "D", "H");	Dm = " - 0x54 - LD D H";		break;
		case 0X55:LOADR8R8(Rd, Rl, "D", "L");	Dm = " - 0x55 - LD D L";		break;
		case 0X56:LOADR8R8(Rd, memoryA[Rhl], "D", "(HL)");	Dm = " - 0x56 - LD D (HL)";		break;
		case 0X57:LOADR8R8(Rd, Ra, "D", "A");	Dm = " - 0x57 - LD D A";		break;
		case 0X58:LOADR8R8(Re, Rb, "E", "B");	Dm = " - 0x58 - LD E B";		break;
		case 0X59:LOADR8R8(Re, Rc, "E", "C");	Dm = " - 0x59 - LD E C";		break;
		case 0X5A:LOADR8R8(Re, Rd, "E", "D");	Dm = " - 0x5A - LD E D";		break;
		case 0X5B:LOADR8R8(Re, Re, "E", "E");	Dm = " - 0x5B - LD E E";		break;
		case 0X5C:LOADR8R8(Re, Rh, "E", "H");	Dm = " - 0x5C - LD E H";		break;
		case 0X5D:LOADR8R8(Re, Rl, "E", "L");	Dm = " - 0x5D - LD E L";		break;
		case 0X5E:LOADR8R8(Re, memoryA[Rhl], "E", "(HL)");	Dm = " - 0x5E - LD E (HL)";		break;
		case 0X5F:LOADR8R8(Re, Ra, "E", "A");	Dm = " - 0x5F - LD E A";		break;
		case 0X60:LOADR8R8(Rh, Rb, "H", "B");	Dm = " - 0x60 - LD H B";		break;
		case 0X61:LOADR8R8(Rh, Rc, "H", "C");	Dm = " - 0x61 - LD H C";		break;
		case 0X62:LOADR8R8(Rh, Rd, "H", "D");	Dm = " - 0x62 - LD H D";		break;
		case 0X63:LOADR8R8(Rh, Re, "H", "E");	Dm = " - 0x63 - LD H E";		break;
		case 0X64:LOADR8R8(Rh, Rh, "H", "H");	Dm = " - 0x64 - LD H H";		break;
		case 0X65:LOADR8R8(Rh, Rl, "H", "L");	Dm = " - 0x65 - LD H L";		break;
		case 0X66:LOADR8R8(Rh, memoryA[Rhl], "H", "(HL)");	Dm = " - 0x66 - LD H (HL)";		break;
		case 0X67:LOADR8R8(Re, memoryA[Rhl], "E", "(HL)");	Dm = " - 0x67 - LD E (HL)";		break;
		case 0X68:LOADR8R8(Rl, Rb, "L", "B");	Dm = " - 0x68 - LD L B";		break;
		case 0X69:LOADR8R8(Rl, Rc, "L", "C");	Dm = " - 0x69 - LD L C";		break;
		case 0X6A:LOADR8R8(Rl, Rd, "L", "D");	Dm = " - 0x6A - LD L D";		break;
		case 0X6B:LOADR8R8(Rl, Re, "L", "E");	Dm = " - 0x6B - LD L E";		break;
		case 0X6C:LOADR8R8(Rl, Rh, "L", "H");	Dm = " - 0x6C - LD L H";		break;
		case 0X6D:LOADR8R8(Rl , Rl, "L", "L");	Dm = " - 0x6D - LD L L";		break;
		case 0X6E:LOADR8R8(Rl, memoryA[Rhl], "L", "(HL)");	Dm = " - 0x6E - LD L (HL)";		break;
		case 0X6F:LOADR8R8(Rh, Ra, "H", "A");	Dm = " - 0x6F - LD H A";		break;
		case 0X70:LOADR8R8(memoryA[Rhl], Rb, "(HL)", "B");	Dm = " - 0x70 - LD (HL) B";		break;
		case 0X71:LOADR8R8(memoryA[Rhl], Rc, "(HL)", "C");	Dm = " - 0x71 - LD (HL) C";		break;
		case 0X72:LOADR8R8(memoryA[Rhl], Rd, "(HL)", "D");	Dm = " - 0x72 - LD (HL) D";		break;
		case 0X73:LOADR8R8(memoryA[Rhl], Re, "(HL)", "E");	Dm = " - 0x73 - LD (HL) E";		break;
		case 0X74:LOADR8R8(memoryA[Rhl], Rh, "(HL)", "H");	Dm = " - 0x74 - LD (HL) H";		break;
		case 0X75:LOADR8R8(memoryA[Rhl], Rl, "(HL)", "L");	Dm = " - 0x75 - LD (HL) L";		break;
		case 0X76:						Dm = " - 0x76 - HALT";			break;
		case 0X77:LOADR8R8(memoryA[Rhl], Ra, "(HL)", "A");	Dm = " - 0x77 - LD (HL) A";		break;
		case 0X78:LOADR8R8(Ra, Rb, "A", "B");	Dm = " - 0x78 - LD A B";		break;
		case 0X79:LOADR8R8(Ra, Rc, "A", "C");	Dm = " - 0x79 - LD A C";		break;
		case 0X7A:LOADR8R8(Ra, Rd, "A", "D");	Dm = " - 0x7A - LD A D";		break;
		case 0X7B:LOADR8R8(Ra, Re, "A", "E");	Dm = " - 0x7B - LD A E";		break;
		case 0X7C:LOADR8R8(Ra, Rh, "A", "H");	Dm = " - 0x7C - LD A H";		break;
		case 0X7D:LOADR8R8(Ra, Rl, "A", "L");	Dm = " - 0x7D - LD A L";		break;
		case 0X7E:LOADR8R8(Ra, memoryA[Rhl], "A", "(HL)");Dm = " - 0x7E - LD A (HL)";		break;
		case 0X7F:LOADR8R8(Ra, Ra, "A", "A");	Dm = " - 0x7F - LD A A";		break;
		case 0X80:ADDA8R(Rb, "A", "B");	Dm = " - 0x80 - ADD A B";		break;
		case 0X81:ADDA8R(Rc, "A", "C");	Dm = " - 0x81 - ADD A C";		break;
		case 0X82:ADDA8R(Rd, "A", "D");	Dm = " - 0x82 - ADD A D";		break;
		case 0X83:ADDA8R(Re, "A", "E");	Dm = " - 0x83 - ADD A E";		break;
		case 0X84:ADDA8R(Rh, "A", "H");	Dm = " - 0x84 - ADD A H";		break;
		case 0X85:ADDA8R(Rl, "A", "L");	Dm = " - 0x85 - ADD A L";		break;
		case 0X86:ADDA8R(memoryA[Rhl], "A", "(HL)");	Dm = " - 0x86 - ADD A (HL)";	break;
		case 0X87:ADDA8R(Ra, "A", "A");	Dm = " - 0x87 - ADD A A";		break;
		case 0X88:						Dm = " - 0x88 - ADC A B";		break;
		case 0X89:						Dm = " - 0x89 - ADC A C";		break;
		case 0X8A:						Dm = " - 0x8A - ADC A D";		break;
		case 0X8B:						Dm = " - 0x8B - ADC A E";		break;
		case 0X8C:						Dm = " - 0x8C - ADC A H";		break;
		case 0X8D:						Dm = " - 0x8D - ADC A L";		break;
		case 0X8E:						Dm = " - 0x8E - ADC A (HL)";	break;
		case 0X8F:						Dm = " - 0x8F - ADC A A";		break;
		case 0X90:SUBAR8(Rb, "A", "B");	Dm = " - 0x90 - SUB B";			break;
		case 0X91:SUBAR8(Rc, "A", "C");	Dm = " - 0x91 - SUB C";			break;
		case 0X92:SUBAR8(Rd, "A", "D");	Dm = " - 0x92 - SUB D";			break;
		case 0X93:SUBAR8(Re, "A", "E");	Dm = " - 0x93 - SUB E";			break;
		case 0X94:SUBAR8(Rh, "A", "H");	Dm = " - 0x94 - SUB H";			break;
		case 0X95:SUBAR8(Rl, "A", "L");	Dm = " - 0x95 - SUB L";			break;
		case 0X97:SUBAR8(Ra, "A", "A");	Dm = " - 0x97 - SUB A";			break;
		case 0X98:						Dm = " - 0x98 - SBC A B";		break;
		case 0X99:						Dm = " - 0x99 - SBC A C";		break;
		case 0X9A:						Dm = " - 0x9A - SBC A D";		break;
		case 0X9B:						Dm = " - 0x9B - SBC A E";		break;
		case 0X9C:						Dm = " - 0x9C - SBC A H";		break;
		case 0X9D:						Dm = " - 0x9D - SBC A L";		break;
		case 0X9E:						Dm = " - 0x9E - SBC A (HL)";	break;
		case 0X9F:						Dm = " - 0x9F - SBC A A";		break;
		case 0XA0:ANDA8R(Rb, "A", "B");	Dm = " - 0xA0 - AND B";			break;
		case 0XA1:ANDA8R(Rc, "A", "C");	Dm = " - 0xA1 - AND C";			break;
		case 0XA2:ANDA8R(Rd, "A", "D");	Dm = " - 0xA2 - AND D";			break;
		case 0XA3:ANDA8R(Re, "A", "E");	Dm = " - 0xA3 - AND E";			break;
		case 0XA4:ANDA8R(Rh, "A", "H");	Dm = " - 0xA4 - AND H";			break;
		case 0XA5:ANDA8R(Rl, "A", "L");	Dm = " - 0xA5 - AND L";			break;
		case 0XA6:ANDA8R(memoryA[Rhl], "A", "(HL)");	Dm = " - 0xA6 - AND (HL)";		break;
		case 0XA7:ANDA8R(Ra, "A", "A");	Dm = " - 0xA7 - AND A";			break;
		case 0XA8:XORAR8(Rb, "A", "B");	Dm = " - 0xA8 - XOR B";			break;
		case 0XA9:XORAR8(Rc, "A", "C");	Dm = " - 0xA9 - XOR C";			break;
		case 0XAA:XORAR8(Rd, "A", "D");	Dm = " - 0xAA - XOR D";			break;
		case 0XAB:XORAR8(Re, "A", "E");	Dm = " - 0xAB - XOR E";			break;
		case 0XAC:XORAR8(Rh, "A", "H");	Dm = " - 0xAC - XOR H";			break;
		case 0XAD:XORAR8(Rl, "A", "L");	Dm = " - 0xAD - XOR L";			break;
		case 0XAE:XORAR8(memoryA[Rhl], "A", "(HL)");	Dm = " - 0xAE - XOR (HL)";		break;
		case 0XAF:XORAR8(Ra, "A", "A");	Dm = " - 0xAF - XOR A";			break;
		case 0XB0:ORRA8R(Rb, "A", "B");	Dm = " - 0xB0 - OR B";			break;
		case 0XB1:ORRA8R(Rc, "A", "C");	Dm = " - 0xB1 - OR C";			break;
		case 0XB2:ORRA8R(Rd, "A", "D");	Dm = " - 0xB2 - OR D";			break;
		case 0XB3:ORRA8R(Re, "A", "E");	Dm = " - 0xB3 - OR E";			break;
		case 0XB4:ORRA8R(Rh, "A", "H");	Dm = " - 0xB4 - OR H";			break;
		case 0XB5:ORRA8R(Rl, "A", "L");	Dm = " - 0xB5 - OR L";			break;
		case 0XB6:ORRA8R(memoryA[Rhl], "A", "(HL)");	Dm = " - 0xB6 - OR (HL)";		break;
		case 0XB7:ORRA8R(Rb, "A", "A");	Dm = " - 0xB7 - OR A";			break;
		case 0XB8:						Dm = " - 0xB8 - CP B";			break;
		case 0XB9:						Dm = " - 0xB9 - CP C";			break;
		case 0XBA:						Dm = " - 0xBA - CP D";			break;
		case 0XBB:						Dm = " - 0xBB - CP E";			break;
		case 0XBC:						Dm = " - 0xBC - CP H";			break;
		case 0XBD:						Dm = " - 0xBD - CP L";			break;
		case 0XBE:						Dm = " - 0xBE - CP (HL)";		break;
		case 0XBF:						Dm = " - 0xBF - CP A";			break;
		case 0XC0:						Dm = " - 0xC0 - RET NZ";		break;
		case 0XC1:						Dm = " - 0xC1 - POP BC";		break;
		case 0XC2:						Dm = " - 0xC2 - JP NZ $aabb";	break;
		case 0XC3:	JP$aabb($aabb);		Dm = " - 0xC3 - JUMP $aabb";	break;
		case 0XC4:						Dm = " - 0xC4 - CALL NZ $aabb";	break;
		case 0XC5:						Dm = " - 0xC5 - PUSH BC";		break;
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
			case 0XCB10:					Dm = " - 0xCB10 - RL B";		break;
			case 0XCB11:					Dm = " - 0xCB11 - RL C";		break;
			case 0XCB12:					Dm = " - 0xCB12 - RL D";		break;
			case 0XCB13:					Dm = " - 0xCB13 - RL E";		break;
			case 0XCB14:					Dm = " - 0xCB14 - RL H";		break;
			case 0XCB15:					Dm = " - 0xCB15 - RL L";		break;
			case 0XCB16:					Dm = " - 0xCB16 - RL (HL)";		break;
			case 0XCB17:					Dm = " - 0xCB17 - RL A";		break;
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
			case 0XCB7C: BIT8R(Rh, 7, "RH");	Dm = " - 0xCB7C - BIT 7 H";		break;
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
		case 0XCD:	Dm = " - 0xCD - CALL $aabb";	break;
		case 0XCE:	Dm = " - 0xCE - ADC A $xx";		break;
		case 0XCF:	Dm = " - 0xCF - RST $8";		break;
		case 0XD0:	Dm = " - 0xD0 - RET NC";		break;
		case 0XD1:	Dm = " - 0xD1 - POP DE";		break;
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
		case 0XE0: LOADHAOFF($xx, Ra, "Ra");	 Dm = " - 0xE0 - LDH ($xx) A"; break;
		case 0XE1:						Dm = " - 0xE1 - POP HL"; BoxDeb();		break;
		case 0XE2: LOADOFFR8A(Rc, "C");	Dm = " - 0xE2 - LD (C) A"; BoxDeb();		break;
		case 0XE5:						Dm = " - 0xE5 - PUSH HL"; BoxDeb();		break;
		case 0XE6:						Dm = " - 0xE6 - AND $xx"; BoxDeb();		break;
		case 0XE7:						Dm = " - 0xE7 - RST $20"; BoxDeb();		break;
		case 0XE8:						Dm = " - 0xE8 - ADD SP xx";	BoxDeb();	break;
		case 0XE9:						Dm = " - 0xE9 - JP (HL)";	BoxDeb();	break;
		case 0XEA:						Dm = " - 0xEA - LD ($aabb) A"; BoxDeb();	break;
		case 0XEE:						Dm = " - 0xEE - XOR $xx";	BoxDeb();	break;
		case 0XEF:						Dm = " - 0xEF - RST $28";	BoxDeb();	break;
		case 0XF0:						Dm = " - 0xF0 - LD A ($xx)"; BoxDeb();	break;
		case 0XF1:						Dm = " - 0xF1 - POP AF";	BoxDeb();	break;
		case 0XF2:						Dm = " - 0xF2 - LD A (C)";	BoxDeb();	break;
		case 0XF3: DI();				Dm = " - 0xF3 - DI";					break;
			//is F4 missing?
		case 0XF5:						Dm = " - 0xF5 - PUSH AF";	BoxDeb();	break;
		case 0XF6:						Dm = " - 0xF6 - xx OR $xx";	BoxDeb();	break;
		case 0XF7:						Dm = " - 0xF7 - RST $30";	BoxDeb();	break;
		case 0XF8:						Dm = " - 0xF8 - LD HL SP";	BoxDeb();	break;
		case 0XF9:						Dm = " - 0xF9 - LD SP HL";	BoxDeb();	break;
		case 0XFA:						Dm = " - 0xFA - LD A ($aabb)"; BoxDeb();	break;
		case 0XFB:						Dm = " - 0xFB - EI";	BoxDeb();		break;
		case 0XFE:						Dm = " - 0xFE - CP $xx";	BoxDeb();	break;
		case 0XFF:						Dm = " - 0xFF - RST $38";	BoxDeb();	break;

		default:
			//std::cout << "ERROR CASE NOT FOUND " << "opcode" << std::hex << std::setw(2) << std::setfill('0') << int(opcode) << "\n";
			break;	
		}
		BoxDeb();
		//std::cout << "Next opcode" << " | " << "Engager: " << std::hex << std::setw(4) << std::setfill('0') << opcode << "-" << Dm << "\n";
		////std::cout << "$aabb: " << std::hex << (int)$aabb << "\n"; // To Dissasemble
		
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

void BoxDeb() {  
//if every missing opcode use this i can capture the debug m3ssage with ease. BOXdeb debugger is the not mapped case error
	std::stringstream Debug;
	Debug.str(std::string());
	std::bitset<8> debugFlag(Rf);
	std::stringstream MemAccess;
	
	if (loopDetected = true) {

		if ($aabb > 0X4000)
		{
			if (memCatchFlag == true)
			{
				MemAccess << "   $aabb not ROM -> " << std::hex << (int)memCatch;
				memCatchFlag = false;
			}
		}


		Debug << "Executing...:  " << functType << "\n " << "AF:  " << std::hex << std::setw(4) << std::setfill('0') << int(Raf) << "    " << "BC: " << std::hex << std::setw(4) << std::setfill('0') << int(Rbc) << "\n" << "DE: " << std::hex << std::setw(4) << std::setfill('0') << int(Rde) << "    " << "HL: " << std::hex << std::setw(4) << std::setfill('0') << int(Rhl) << "\n"
			<< "PC: " << std::hex << std::setw(4) << std::setfill('0') << int(pc) << "    " << "EI" << EInt << " " << "\n"
			<< "IF: " << std::hex << std::setw(4) << std::setfill('0') << (int)IF << "  " << "DI: " << DInt << "\n"
			<< "SP: " << std::hex << std::setw(4) << std::setfill('0') << (int)sp << "\n"
			<< "Z[" << debugFlag.test(7) << "] N[" << debugFlag.test(6) << "] H[" << debugFlag.test(5) << "] C[" << debugFlag.test(4) << "]\n"
			<< "Current Engager: " << std::hex << std::setw(4) << std::setfill('0') << opcode << "\n"
			<< Dm.c_str();
		std::string mergedDebug;
		mergedDebug = Debug.str() + MemAccess.str();



		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Debug M", mergedDebug.c_str(), NULL);


	}
	
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
		Rh = 0x9f;
		Rl = 0xf5;
		Rf = 0x0;
		pc = 0;
		sp = 0;
		Rm = 0;
		Rt = 0;
		//opLen = 1;

		Raf = 0x0;
		Rbc = 0x0;
		Rde = 0x0;
		Rhl = 0x9ff5;
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
		font = TTF_OpenFont("arial.ttf", 40);
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

	gb gameboy;
	//gameboy.Reset();
	gameboy.LoadFile();
	
	
	//-- Creating VRAM peeker all Functionality Disabled ///////////////////// 
	gameboy.LoadBios();
	gameboy.Boot();
	gameboy.VRAMmanagerInit();
	
	memValue << "mem: " <<  std::hex << (int)memoryA[gameboy.Rhl];
		instamem = memValue.str();
		
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Vram Peek", instamem.c_str(), NULL);
	///////////////////////////////////////////////////////////////////////////////////////
	//- Main execution loop
	//- Currently Catching Loops Instances
	///////////////////////////////////////////////////////////////////////////////////////
	while (1)	{

		////VRAM PEEK
		//gameboy.VRAMmanagerInit();
		//gameboy.VRAMmanager();
		//gameboy.gfxHandler();
		/// VRAM PEEK
		memValue.str(std::string());
		
		
		
		//memoryA[0x9F96] = 1;
		
		for (opCounter = 0; opCounter <= 5; opCounter++) {
		
		
		gameboy.RegComb();
		gameboy.BoxDeb();
		gameboy.opDecoder();
		
	//	gameboy.VRAMpeek();
	gameboy.Vram2();
		//
		
		
		if (captureDm) { pastDms[opCounter] = Dm; }
		else if (captureDm == false) { currentDms[opCounter] = Dm; }
		}
		opCounter = 0;
		captureDm = false;

		for (int i = 0; i <= 5; i++)
		{
			
			if (pastDms[i] == currentDms[i]) {
				numberMatchs = 0;
				loopInstances++;
				std::cout << currentDms[i] << "\n";
				matchFound = true;	
				if (matchFound) { numberMatchs++; matchFound = false; }
			}
			
			if (numberMatchs > 1)
			{
				loopInstances++;
				std::cout << "Detected loop of [" << numberMatchs << "] Instructions \n";
				
			}
			
		}
		std::cout << "- 5 Instructions Evaluated - [" << loopInstances << "] Found\n";
		//Break Point QUICK ACCESS - 
		//First you put a huge number here like 20000
		//the program will most likely show the number of matchs in the loop getting stuck way before the counter finishes
		//if i match the loop counter to this number. I can quickly jump into the last instruction that was read before going into a loop //most likely unimplemented
		if (loopInstances > 0) { loopDetected = true; SDL_RenderPresent(renderer); }
		// TO RESTORE FUNCTIONALITY AFTER VRAM PEEK
	//	std::cin.get();
		//-- Creating VRAM peeker all Functionality Disabled /////////////////////
		
		
}
	return 0;
}