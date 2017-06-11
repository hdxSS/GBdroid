// Gameboy Basic.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <bitset>
#include <string>
#include <sstream>
//#include <SDL2/SDL.h>
#include <random>

//Modules
//MMU - Done - Not usable under current estructure
//CPU -- ONly functions Needs Mapping
//Timers -- Done
//Interrupts - 0% - Next
//FlagHandling - Done // check logic cases - Rewriten
//Register combining - Done
//Gfx - 0%
//Sound - 0%
//Input - 0%

//SDL support

//SDL_Renderer* renderer = NULL;

int min;
int max;

std::string ROM = "tetris.gb";

unsigned char  memoryT[0xFFFF];
unsigned char memoryA[0xFFFF];
char letter;
unsigned char addr;
unsigned short opcode;
unsigned char opLen;
std::string functType;
unsigned short $aabb;
bool nullRender = true;
std::string tempM;
std::string Dreg;
std::string Sreg;
std::string Starget;
std::string Dtarget;

std::stringstream funcText;

///////////

unsigned short _16bitIn;
unsigned long fullOpcode;
unsigned char $bb;
unsigned char $aa;
unsigned char $xx;
unsigned char _8bitIn3;

unsigned char TempRf;
/////////////

class gb
{
public:
	unsigned short Raf = 0;
	unsigned short Rbc = 0;
	unsigned short Rde = 0;
	unsigned short Rhl = 0;
	unsigned char hlM = 0;
	unsigned char Ra = 0;
	unsigned char Rb = 0;
	unsigned char Rc = 0;
	unsigned char Rd = 0;
	unsigned char Re = 0;
	unsigned char Rl = 0;
	unsigned char Rf = 0;
	unsigned short pc = 0;
	unsigned char Rh = 0;
	unsigned short sp = 0;
	unsigned char opClock = 0;
	unsigned long  cpuClock = 0;
	unsigned char mathTC1 = 0;
	unsigned char mathTC2 = 0;
	unsigned char mathTS1 = 0;
	unsigned char mathTS2 = 0;

	unsigned char N;
	unsigned char H;
	unsigned char C;
	unsigned char Z;



	float Rm = 0;
	float Rt = 0;



	char ROMb0[0x4000];
	char ROMb1[0x4000];

	//MMU manager ---------------1

	//MMU + FLAG 
	//1: read b
	//2: read w
	//3: write b
	//4: write w
	int Mflag = 0;

	// malo... Tengo que tomar el valor siguiente de char de los opcodes 

	// arreglar 

	//
	//SDL_Rect pixel[144][164];
	/*
	void gfxHandler()
	{
	unsigned int rowN;
	unsigned int colN;
	unsigned long gpuClock;
	unsigned int modeClock;
	gpuClock += opClock;
	rowN = 0;
	//cpu clock and times accesing Vram and OAM
	//scanline access OAM - gpuMode 2 - 80 cycles
	//scanline access VRAM - gpuMode 3 - 172 cyc
	//horizontal blank - gpuMode 0 - 204 cycles
	//one line scan and blank - 456 cycles
	//vertical blank - gpuamode 1 - 4560 10lines
	//full frame 70224
	//	144 * 456 + (14 * 4560)?? - 204
	int gpuMode;

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


	for (int z = 0; z<300; z++)
	{
	for (int j = 0; j<145; j++)
	{
	for (int i = 0; i<165; i++) {
	pixel[j][i].h = 5;
	pixel[j][i].w = 5;
	pixel[j][i].x = 140 + (i * 5);
	pixel[j][i].y = 200 + (j * 5);


	//randomGen(0,255);
	int cr, cg, cb;
	//way of selecting RED
	if ((rand() % 100) == 0){
	cr = 96;
	cg = 96;
	cb = 96;
	}
	-	//		 //way of selecting GREEN
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

	SDL_SetRenderDrawColor(renderer, cr, cg, cb, 255);
	SDL_RenderFillRect(renderer, &pixel[j][i]);
	//SDL_RenderPresent(renderer);
	//SDL_Delay(5);

	}

	}
	SDL_RenderPresent(renderer);
	}//Video support
	}//gfxHandler
	*/

	void MMU(unsigned short PC, int Mflag, int iBit) {

		unsigned char _8bitIn = 0;
		unsigned char _8bitInM = 0;
		unsigned short _16bitIn = 0;
		unsigned short _16bitInM = 0;

		if (PC < 0x4000) {

			switch (Mflag) {

			case 1:  //read b
				opcode = memoryA[PC];
				_8bitIn = memoryA[PC + 1];
				_8bitInM = memoryA[_8bitIn];
				break;

			case 2: //read w
				_16bitIn = (memoryA[PC + 2] << 8) | memoryA[PC + 3];
				opcode = (memoryA[PC] << 8) | memoryA[PC + 1];
				_16bitInM = memoryA[_16bitIn];
				break;

			case 3: //write b
				if (iBit == 1) {
					memoryA[_8bitIn] = _8bitInM;
				}
				else
					std::cout << "\n Overflow: on write _8bitIn \n";
				break;

			case 4: //write w
				if (iBit == 2) {
					memoryA[_16bitIn] = _16bitInM;
				}
				else
					std::cout << "\n Overflow: on write _16bitIn \n";
				break;
			}

			std::cout << "MMU ROM bank0";
		}

		else if (PC >= 0x4000 && PC < 0x8000) {



			std::cout << "MMU ROM bank1";
		}

		else if (PC >= 0x8000 && PC < 0xA000) {

			std::cout << "MMU GFX";
		}

		else if (PC >= 0xA000 && PC < 0xC000) {

			std::cout << "MMU EXT RAM bank";
		}

		else if (PC >= 0xC000 && PC < 0xE000) {

			std::cout << "MMU WORKING RAM";
		}

		else if (PC >= 0xE000 && PC < 0xFE00) {

			std::cout << "MMU WORKING RAM SHADOW";
		}

		else if (PC >= 0xFE00 && PC < 0xFF00) {

			std::cout << "MMU GFX SPRITES";
		}

		else if (PC >= 0xFF00 && PC < 0xFF80) {

			std::cout << "MMU INPUT";
		}

		else if (PC >= 0xFF80 && PC < 0xFFFF) {

			std::cout << "MMU ZERO PAGE RAM";
		}
	}

	void rw(unsigned char addr) {}
	void wb(unsigned char addr, short val) {}
	void ww(unsigned char addr, short val) {}


	unsigned short tempComb;

#pragma region RegisterCombine

	void DebugReg(std::string functionType) {

		if (functionType == "LD") { ///// FUNCTION TYPE LD R8
			if (Dreg == "A")	 { functType = "LD | A | - "; }
			else if (Dreg == "B"){ functType = "LD | B | - "; }
			else if (Dreg == "C"){ functType = "LD | C | - "; }
			else if (Dreg == "D"){ functType = "LD | D | - "; }
			else if (Dreg == "E"){ functType = "LD | E | - "; }
			else if (Dreg == "F"){ functType = "LD | F | - "; }
			else if (Dreg == "H"){ functType = "LD | H | - "; }
			else if (Dreg == "L"){ functType = "LD | L | - "; }

			else if (Dreg == "AF"){ functType = "LD | AF | - "; }
			else if (Dreg == "BC"){ functType = "LD | BC | - "; }
			else if (Dreg == "DE"){ functType = "LD | DE | - "; }
			else if (Dreg == "HL"){ functType = "LD | HL | - "; }

			//else functType = "DIDNT WORK :(";
			else if (functionType == "LD A") { ///// FUNCTION TYPE LD R8 to R8
				if (Sreg == "A")	 { functType = "LD | A || A | - "; }
				else if (Sreg == "B"){ functType = "LD | A || B | - "; }
				else if (Sreg == "C"){ functType = "LD | A || C | - "; }
				else if (Sreg == "D"){ functType = "LD | A || D | - "; }
				else if (Sreg == "E"){ functType = "LD | A || E | - "; }
				else if (Sreg == "F"){ functType = "LD | A || F | - "; }
				else if (Sreg == "H"){ functType = "LD | A || H | - "; }
				else if (Sreg == "L"){ functType = "LD | A || L | - "; }

				else functType = "DIDNT WORK :(";
			}
		}
	} // Fin DebugReg

	void Rcomb(unsigned char regPair) {

		switch (regPair) {
		case 0:
			Raf = (Ra << 8) | Rf;
			break;

		case 1:
			Rbc = (Rb << 8) | Rc;
			break;

		case 2:
			Rde = (Rd << 8) | Re;
			break;

		case 3:
			Rhl = (Rh << 8) | Rl;
			hlM = memoryA[Rhl];
			break;

		}
	}
#pragma endregion

#pragma region FlagCalculator
	// Flag handling

	unsigned char addTarget = 0;


	void CALCFLG(unsigned short mTS1, unsigned short mTS2, unsigned int opType)
	{
		//overload del modulo CALCFLG
		//opTypes
		// 1 - ADD
		// 2 - SUB
		// 3 - Comp
		// 4 - AND
		// 5 - OR
		// 6 - XOR

		//probando sync
		// x + y > 255
		unsigned short carrycheck;
		unsigned short borrowcheck;
		unsigned short ANDcheck;
		unsigned short ORcheck;
		unsigned short XORcheck;

		carrycheck = mTS1 + mTS2;
		borrowcheck = mTS1 - mTS2;
		ANDcheck = mTS1 & mTS2;
		ORcheck = mTS1 | mTS2;
		XORcheck = mTS1 ^ mTS2;

		std::bitset<8> Rf_as_bit(Rf);

		//todo tengo que arreglar el overload
		switch (opType) {
		case 1: //Add
			if (C == 2) {
				if (carrycheck > 0xFF) {
					//carry flag
					Rf_as_bit.set(4); // Set C
				}
				if (carrycheck > 0xF) {
					//half carry flag
					Rf_as_bit.set(5); // Set H
				}

				else if (carrycheck < 0xF) { }
			}
			else if (C == 3) {
				std::cout << "Ignore C flag";
			}
			//Do nothing
			break;
		case 2: //Sub
			if (N == 2) {
				{
					

					Rf_as_bit.set(6); // Set N
					if (borrowcheck < 0) {
						Rf_as_bit.set(4); // Set C
					}
					if (borrowcheck < 0xF) {
						Rf_as_bit.set(4); // Set C
					}
				}

			}
			else if (N == 3) {
				//Ignoring
			}
			break;
		

			case 3: // Compare
				if (Z == 2) {
					if (borrowcheck == 0) {
						//zero flag
						Rf_as_bit.set(7); // Set C
					}
				}
				else if (Z == 3) {
					std::cout << "Ignore Z flag";
				}

				break;

			case 4: //optype AND
				if (ANDcheck == 1) {
					Rf_as_bit.set(7); // Set Z
					//SETS FLAG
				}
				else if (ANDcheck == 0) { Rf_as_bit.reset(7); }

				break;

			case 5: //optype OR
				if (ORcheck == 1) {
					Rf_as_bit.set(7); // Set Z
					//SETS FLAG
				}
				else if (ORcheck == 0) { Rf_as_bit.reset(7); }
				break;

			case 6: //optype XOR
				if (XORcheck == 1) {
					Rf_as_bit.reset(7); // Reset Z
					//SETS FLAG
				}
				else if (XORcheck == 0)
				{
					//std::cout << "Te quiero volver loco" << "\n";
					Rf_as_bit.set(7);
				}
				break;
	}
	Rf = Rf_as_bit.to_ulong();
	Rcomb(Raf);
	//std::cout << "IM AN ERROR - How did i get here? ELHP!!" << "AF: " << std::hex << (int)Raf; "n";
	//std::cout << "IM AN ERROR - How did i get here? ELHP!!" << "A: " << std::hex << (int)Ra; "n";
	//std::cout << "IM AN ERROR - How did i get here? ELHP!!" << "F: " << std::hex << (int)Rf; "n";
}//end flagcalcShort

	

	 


	void FLGH(unsigned char Z, unsigned char N, unsigned char H, unsigned char C, unsigned short calcT1, unsigned short calcT2, unsigned char Optype) {

		//opTypes
		// 1 - ADD
		// 2 - SUB
		// 3 - Comp
		// 4 - AND
		// 5 - OR
		// 6 - XOR

		unsigned short TT1;
		unsigned short TT2;
		unsigned short Tresult;

		TT1 = calcT1;
		TT2 = calcT2;

		std::bitset<8> Rf_as_bit(Rf);
		switch (Z) {
		case 0:	Rf_as_bit.reset(7); break;
		case 1:	Rf_as_bit.set(7);	break;
		case 2: //Calculate Flag based on result
			switch (Optype) {			
			//case 0: break; // 2 possible x+y > 0xFF or x+y > 0xF Carry and Halfcarry - Non  exclusive - Affects [H][C]
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
				if (Tresult > 0xF) { Rf_as_bit.set(5); }	else { Rf_as_bit.reset(5); }
				break;
				case 2:
				Rf_as_bit.set(6);
				Tresult = TT1 - TT2;
				if (Tresult < 0xF) { Rf_as_bit.set(5); }	else { Rf_as_bit.reset(5); }
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
		std::cout << "Z[" << Rf_as_bit.test(7) << "] N[" << Rf_as_bit.test(6) << "] H[" << Rf_as_bit.test(5) << "] C[" << Rf_as_bit.test(4) << "]\n";
		Rf = Rf_as_bit.to_ulong();
	}

	void FlagHelper(){
		//gb::Rcomb(0);
		std::cout << "A: " << std::hex << (int)Ra << "\n";
		std::cout << "F: " << std::hex << (int)Rf << "\n";
		std::cout << "Rf: " << std::hex << (int)Raf << "\n";
		//Rf = (Raf << 4);
		std::bitset<8> Rf_as_bit(Rf);
		std::cout << "Z[" << Rf_as_bit.test(7) << "] N[" << Rf_as_bit.test(6) << "] H[" << Rf_as_bit.test(5) << "] C[" << Rf_as_bit.test(7) << "]\n";

		Rf = Rf_as_bit.to_ulong();
		gb::Rcomb(0);
		
		
	}

#pragma endregion

#pragma region RegisterAndFlagsUpdate	
	// Register Combine method
	void RegComb() {
		if (pc != 0x100) {
		gb::Rcomb(0);
		gb::Rcomb(1);
		gb::Rcomb(2);
		gb::Rcomb(3);

		}
		//-Done
		
		std::cout << "\nAF: " << std::hex << (int)Raf << "\n";
		std::cout << "BC: " << std::hex << (int)Rbc << "\n";
		std::cout << "DE: " << std::hex << (int)Rde << "\n";
		std::cout << "HL: " << std::hex << (int)Rhl << "\n";
	
	}


	void FlagState() {
		/*
		std::cout << "Z: " << std::hex << (int)(Rf & (0x8 >> 3)) << "\n";
		std::cout << "N: " << std::hex << (int)(Rf & (0x4 >> 2)) << "\n";
		std::cout << "H: " << std::hex << (int)(Rf & (0x2 >> 1)) << "\n";
		std::cout << "C: " << std::hex << (int)(Rf & (0x1)) << "\n";
	*/
	}

#pragma endregion

#pragma region InstructionMapping
	//probablemente ningun calculo de flag funciona porque se calcula sobre el resultado
	// Instruction mapping ----------
	void ADC8RHLM(unsigned short $any, std::string Dtarget, std::string Starget) { // n + carry flag to A
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
	void ADCR8(unsigned short $any, std::string Dtarget, std::string Starget) {
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
	void ADDAHLM(std::string Dtarget, std::string Starget) {
		
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
		CALCFLG(Ra, _8int, 1);
		Ra += _8int;
		opClock = 2;
		functType = "ADD $xx ";

	}
	void ADDA8R(unsigned char _8r) {
		//FLGH(2, 0, 2, 2);
		CALCFLG(Ra, _8r, 1);
		Ra += _8r;
		opClock = 1;
		functType = "ADD R8 ";
	}
	void ADDHL16R(unsigned short _R1) {
		//FLGH(3, 0, 2, 2);
		CALCFLG(Rhl, _R1, 1);
		Rhl += _R1;
		opClock = 1;
		functType = "ADD HL 16R ";
	}
	void ADDSP8OFF(unsigned char _8off) {
		//FLGH(0, 0, 2, 2);
		CALCFLG(sp, _8off, 1);
		sp = +_8off;
		opClock = 2;
		functType = "ADD SP 8off";
	}
	void ANDAHLM() {
		//FLGH(2, 0, 1, 0);
		CALCFLG(Ra, hlM, 1);
		Ra &= hlM;
		opClock = 1;
		functType = "AND A (HL) ";
	}
	void ANDA$xx(unsigned char _8int) {
		//FLGH(2, 0, 1, 0);
		CALCFLG(Ra, _8int, 3); //set optype3 as AND
		Ra &= _8int;
		opClock = 2;
		functType = "AND A $xx ";
	}
	void ANDA8R(unsigned char _8r) {
		//FLGH(2, 0, 1, 0);
		CALCFLG(Ra, _8r, 3);  //set optype3 as AND
		Ra &= _8r;
		opClock = 1;
		functType = "AND A 8R ";
	}
	void BITHLM(unsigned char bitnum) {

		switch (hlM << bitnum) { // refinar 0x1
		case 1:
			std::cout << "tested positive\n";

		case 0:
			std::cout << "tested negative\n";
		}
		//FLGH(2, 0, 1, 3);
		opClock = 2;
		functType = "BIT (HL) ";
		//CALCFLG(Ra, _8int, 3) //set optype4 op de revision valor hlM o hacer directo aca

		//bit test agains
		//update timers
	}
	void BIT8R(unsigned char bitnum, unsigned char _8r) {

		switch (_8r << bitnum) { // refinar 0x1
		case 1:
			std::cout << "tested positive\n";

		case 0:
			std::cout << "tested negative\n";
		}
		//FLGH(2, 0, 1, 3);
		opClock = 2;
		functType = "BIT 8R ";
		//CALCFLG(Ra, _8int, 3) //set optype4 op de revision valor hlM o hacer directo aca
		//bit test agains
		//update timers

	}
	void CALLPCF$aabb(int flagcc, unsigned short _16BA) {

		if (flagcc == 1)
		{
			pc = _16BA;
		}
		//FLGH(3, 3, 3, 3);
		opClock = 3;
		functType = "CALL PC IF F  $aabb ";
	}
	void CALLPC16BA(unsigned short _16BA) {
		pc = _16BA;
		// mal la dif entre jump y call
		//skip
		opClock = 3;
		functType = "CALL PC $aabb ";
		//FLGH(3, 3, 3, 3);
	}
	void CARRYFLAG() {
		//FLGH(3, 0, 0, 2);
		//CALCFLG(Ra, _8int, 3) //set optype4 op de revision valor hlM o hacer directo aca
		opClock = 1;
		functType = "CARRY FLAG ";
		//FLGH(3, 3, 3, 1);
	}
	void COMPHLM() {
		//FLGH(2, 1, 2, 2);
		opClock = 1;
		functType = "COMPLEMENT (HL) ";

	}
	void COMP8INT(unsigned char _8int) {
		//FLGH(2, 1, 2, 2);
		opClock = 2;
		functType = "COMPLEMENT $xx ";
	}
	void COMP8R(unsigned char _8r) {
		//FLGH(2, 1, 2, 2);
		opClock = 1;
		functType = "COMPLEMENT 8R ";
	}
	void CPL() {
		//FLGH(3, 1, 1, 3);
		opClock = 1;
		functType = "COMPLEMENT L";
	}
	void DAA() {
		//FLGH(2, 3, 0, 2);
		opClock = 1;
		functType = "DAA ";
	}
	void DECHLM() {
		//FLGH(2, 1, 2, 3);
		hlM--;
		opClock = 1;
		functType = "DEC (HL) ";

	}
	void DECR16(unsigned short _16r) {
		_16r--;
		opClock = 1;
		//update flags
		functType = "DEC R16 ";
		//FLGH(3, 3, 3, 3);
	}
	void DEC8R(unsigned char _r8) {
		//FLGH(2, 1, 2, 3);
		_r8--;
		opClock = 1;
		functType = "DEC 8R ";
		//update timers
		//update flags
	}
	void DI() {
		opClock = 1;
		opLen = 1;
		pc += opLen;
		functType = "DISABLE INTERRUPTS ";
		//FLGH(3, 3, 3, 3);
	}
	void EI() {
		opClock = 1;
		opLen = 1;
		pc += opClock;
		//FLGH(3, 3, 3, 3);
	}
	void HALT() {
		opClock = 1;
		//FLGH(3, 3, 3, 3);
	}
	void INCHLM() {
		//FLGH(2, 0, 2, 3);
		hlM++;
		opClock = 1;
		//update timers
		//update flags
		functType = "INC (HL) ";
	}
	void INCR16(unsigned short _16r) {
		_16r++;
		//update timers
		opClock = 1;
		functType = "INC R16 ";
		//FLGH(3, 3, 3, 3);
		//update flags
	}
	void INC8R(unsigned char _r8) {
		//FLGH(2, 0, 2, 3);
		_r8++;
		//update timers
		opClock = 1;
		//update flags
		functType = "INC R8 ";
		//FLGH(3, 3, 3, 3);
	}
	void JUMPHLM() {
		pc = hlM;

		//update timers
		opClock = 1;
		//update flags
		functType = "JP (HL) ";
		//FLGH(3, 3, 3, 3);
	}
	void JUMPCC$aabb(int flagcc, unsigned short _16BA) {
		if (flagcc == 1)
			pc = _16BA;
		else
			pc++;
		opClock = 3;
		functType = "JUMP CC $aabb ";
		//FLGH(3, 3, 3, 3);
	}
	void JP$aabb(unsigned short _$aabb) {
		////-- Instruction preset
		opLen = 3;
		FLGH(3, 3, 3, 3, NULL, NULL, NULL);
		funcText << "JUMP PC -> " << std::hex << std::setw(4) << std::setfill('0') << (int)_$aabb;
		functType = funcText.str();
		opDeb();
		pc = _$aabb;
		////-- Instruction preset
	}
	void JRCC$xx(int flagcc, unsigned char _$xx) {
		if (flagcc == 1)
			pc = memoryA[_$xx];
		else
			//skip
			pc++;
		opClock = 2;
		functType = "JR IF C $xx ";
		//FLGH(3, 3, 3, 3);
	}
	void JR$xx(unsigned char _8int) {
		pc = memoryA[_8int];
		//skip
		opClock = 2;
		functType = "JR $xx ";
		//FLGH(3, 3, 3, 3);
	}
	void LOADCMRA() {
		Ra = memoryA[Rc];
		//update timers
		opClock = 1;
		//update flags
		functType = "LOAD C (A) ";
		//FLGH(3, 3, 3, 3);
	}
	void LOADHLM$xx(unsigned char _8int) {
		hlM = _8int;
		//update timers
		opClock = 2;
		//update flags
		functType = "LOAD (HL) $xx ";
		//FLGH(3, 3, 3, 3);
	}
	void LOADHLM8R(unsigned char _8r) {
		hlM = _8r;
		//update timers
		opClock = 1;
		//update flags
		functType = "LOAD (HL) R8 ";
		///FLGH(3, 3, 3, 3);
	}
	void LOAD$aabbMRA(unsigned short _16ba) {
		memoryA[_16ba] = Ra;
		//update timers
		opClock = 3;
		//update flags
		functType = "LOAD $aabb (A) ";
		//FLGH(3, 3, 3, 3);
	}
	void LOAD$aabbSP(unsigned short _16ba) {
		memoryA[_16ba] = sp;
		//update timers
		opClock = 3;
		//update flags
		functType = "LOAD $aabb SP ";
		//FLGH(3, 3, 3, 3);
	}
	void LOAD16RMRA(unsigned short _16r) {
		memoryA[_16r] = Ra;
		//update timers
		opClock = 1;
		//update flags
		functType = "LOAD (16R) A ";
		//FLGH(3, 3, 3, 3);
	}
	void LOADRACM(unsigned char _8r) {
		Ra = memoryA[_8r];
		//update timers
		opClock = 1;
		//update flags
		functType = "LOAD A (C) - REVISAR ";
		//FLGH(3, 3, 3, 3);
		///////////////////REVISAR PUEDE ESTAR MALO
	}
	void LOADRA$aabb(unsigned short _16ba) {
		Ra = memoryA[_16ba];
		//update timers
		opClock = 3;
		//update flags
		functType = "LOAD A $aabb ";
		//FLGH(3, 3, 3, 3);
	}
	void LOADRA16RM(unsigned short _16r) {
		Ra = memoryA[_16r];
		//update timers
		opClock = 1;
		//update flags
		functType = "LOAD A (16R) ";
		//FLGH(3, 3, 3, 3);
	}
	void LOADRHLSPOFF(unsigned char _8boff) {
		//FLGH(0, 0, 2, 2);
		Rhl = (sp + _8boff);
		//update timers
		opClock = 2;
		//update flags
		functType = "LOAD HL SPOFF";
	}
	void LOADR16$aabb(unsigned short _16r, unsigned short $aabb) {
		_16r = $aabb;
		//update timers
		opClock = 3;
		opLen = 3;
		//functType = "LD R16 $aabb - ";
		//FLGH(3, 3, 3, 3);
		functType = "LD";
		gb::DebugReg(functType);
		//update flags
	}
	void LOADR8HLM(unsigned char _8r) {
		_8r = hlM;
		functType = "LD (HL)";
		gb::DebugReg(functType);
		//update timers
		opClock = 1;
		opLen = 1;
		pc += opLen;
	//	FLGH(3, 3, 3, 3);
		//update flags
	}
	void LDR8$xx(unsigned char _8r, unsigned char _$xx) {
		
		//update timers
		opClock = 2;
		//FLGH(3, 3, 3, 3);
		opLen = 2;
		functType = "LD R8 $xx ";
		//DebugReg(functType);
		//_8r = _$xx;

		//update flags
	}
	void LOADR8R8(unsigned char _8r, unsigned char  _8r2) {
		_8r = _8r2;
		//update timers
		opClock = 1;
		//update flags
		functType = "LOAD R8 R8 ";
		//FLGH(3, 3, 3, 3);
	}
	void LOADSPHL(unsigned short SP, unsigned short HL) {
		SP = HL;
		//update timers
		opClock = 1;
		//update flags
		functType = "SP HL ";
		//FLGH(3, 3, 3, 3);
	}
	void LOADDR8HLM() {
		Ra = hlM;
		Rhl--;
		//update timers
		opClock = 1;
		//update flags
		functType = "R8 (HL) ";
		//FLGH(3, 3, 3, 3);
		opLen = 1;
	}
	void LOADDHLMR8() {
		//FLGH(3, 3, 3, 3);
		//update timers
		opClock = 1;
		//update flags
		opLen = 1;
		functType = "LDD (HL) R8 ";
		
	}
	void LOADHAOFF(unsigned char off) { //LDH

		memoryA[0xff00 + off] = Ra;
		//update timers
		opClock = 2;
		opLen = 2;
		pc += opLen;
		//update flags
		functType = "LD";
		DebugReg(functType);
		//FLGH(3, 3, 3, 3);
	}

	void LOADAHOFF(unsigned char off) { //LDH

		Ra = memoryA[0xff00 + off];
		//update timers
		opClock = 2;
		//update flags
		functType = "LOAD A OFF ";
		//FLGH(3, 3, 3, 3);
	}
	void LDIHLMRA() {
		hlM = Ra;
		Ra++;
		//update timers
		opClock = 1;
		//update flags
		functType = " LDI (HL) A ";
		//FLGH(3, 3, 3, 3);
	}
	void LDIRAHLM() {
		Ra = hlM;
		hlM++;
		//update timers
		opClock = 1;
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
	void ORHLMRA() {
	//	FLGH(2, 0, 0, 0);
		hlM |= Ra;
		opClock = 1;
		functType = "OR (HL) A ";
	}
	void ORRA$xx(unsigned char _8int) {
	//	FLGH(2, 0, 0, 0);
		Ra |= _8int;
		opClock = 2;
		functType = "OR ADC R8 ";
	}
	void ORRA8R(unsigned char _8r) {
	//	FLGH(2, 0, 0, 0);
		Ra |= _8r;
		opClock = 1;
		functType = "OR A R8 ";
	}
	void POP16R(unsigned _16r) {
		memoryA[sp] = _16r;
		sp--;
		opClock = 1;
		functType = "POP 16R ";
	//	FLGH(3, 3, 3, 3);
	}
	void PUSH() {
		memoryA[pc] = sp;
		sp++;
		opClock = 1;
		functType = "PUSH ";
	//	FLGH(3, 3, 3, 3);
	}
	void RESBHLM(unsigned char _bit) {
		//bit de hlM a cero
		opClock = 2;
		functType = "RES BIT (HL) ";
	//	FLGH(3, 3, 3, 3);
	}
	void RESBR8(unsigned char _bit, unsigned  char _8r){
		//bit de _8r a cero
		opClock = 2;
		functType = "RES BIT R8 ";
	//	FLGH(3, 3, 3, 3);
	}
	void RET() {
		opClock = 1;
		functType = "RET ";
	//	FLGH(3, 3, 3, 3);
	}

	void RETCC(unsigned char flagcc) {
		opClock = 1;
		functType = "RET CC ";
	//	FLGH(3, 3, 3, 3);
	}
	void RETI() {
		opClock = 1;
		functType = "RETI ";
	//	FLGH(3, 3, 3, 3);
	}
	void RLHM() {
	//	FLGH(2, 0, 0, 2);
		opClock = 2;
		functType = "RL (H)";
	}
	void RLR8(unsigned char _8r, unsigned char _8r2) {
	//	FLGH(2, 0, 0, 2);
		opClock = 2;
		functType = "RL 8R ";
	}
	void RLA() {
	//	FLGH(0, 0, 0, 2);
		opClock = 1;
		functType = "RL A ";
	}
	void RLCHM() {
	//	FLGH(2, 0, 0, 2);
		opClock = 2;
		functType = "RLC (H) ";
	}
	void RLCR8R8(unsigned char _r8, unsigned char _r82) {
	//	FLGH(2, 0, 0, 2);
		opClock = 2;
		functType = "RLC R8 R8 ";
	}
	void RLCA() {
	//	FLGH(0, 0, 0, 2);
		opClock = 1;
		functType = "RLC A ";
	}
	void RRHM() {
	//	FLGH(2, 0, 0, 2);
		opClock = 2;
		functType = "RR (H) ";
	}
	void RRR8R8(unsigned char _r8, unsigned char _r82) {
	//	FLGH(2, 0, 0, 2);
		opClock = 2;
		functType = "RR R8 R8 ";

	}
	void RRA() {
	//	FLGH(0, 0, 0, 2);
		opClock = 1;
		functType = "RR A ";

	}
	void RRCHM() {
	//	FLGH(2, 0, 0, 2);
		opClock = 2;
		functType = "RRC (H) ";
	}
	void RRCR8R8(unsigned char _r8, unsigned char _r82) {
	//	FLGH(2, 0, 0, 2);
		opClock = 2;
		functType = "RRC R8 R8 ";
	}
	void RRCA() {
	//	FLGH(0, 0, 0, 2);
		opClock = 1;
		functType = "RRC A ";
	}
	void RSTF(unsigned char _byte) {
		functType = "RST F ";
	}
	void SBCHM() {
	//	FLGH(2, 1, 2, 2);
		opClock = 1;
		functType = "SBC (H) ";
	}
	void SBC$xx(unsigned char _8int) {
	//	FLGH(2, 1, 2, 2);
		opClock = 2;
		functType = "SBC $xx ";
	}
	void SBC8R(unsigned char _r8) {
	//	FLGH(2, 1, 2, 2);
		opClock = 1;
		functType = "SBC 8R ";
	}
	void SCF() {
	//	FLGH(3, 0, 0, 1);
		opClock = 1;
		functType = "SCF ";
	}
	void SETBHM(unsigned char _bit) {
		opClock = 2;
		functType = "SET B (H) ";
	//	FLGH(3, 3, 3, 3);
	}
	void SETBR8(unsigned char _bit, unsigned char _r8) {
		opClock = 2;
		functType = "SET B R8 ";
	//	FLGH(3, 3, 3, 3);
	}
	void SLAHLM() {
	//	FLGH(2, 0, 0, 2);
		opClock = 2;
		functType = "SLA (HL) ";
	}
	void SLAR8(unsigned char _r8) {
	//	FLGH(2, 0, 0, 2);
		opClock = 2;
		functType = "SLA R8 ";
	}
	void SRAHM() {
	//	FLGH(2, 0, 0, 2);
		opClock = 2;
		functType = "SRA (H) ";
	}
	void SRAR8R8(unsigned char _8r, unsigned char _r82) {
	//	FLGH(2, 0, 0, 2);
		opClock = 2;
		functType = "SRA R8 R8 ";
	}
	void SRLHM() {
	//	FLGH(2, 0, 0, 2);
		opClock = 2;
		functType = "SRL (H) ";
	}
	void SRLR8R8(unsigned char _8r, unsigned char _r82) {
	//	FLGH(2, 0, 0, 2);
		opClock = 2;
		functType = "SRL R8 R8 ";
	}
	void STOP() {
		opClock = 2;
		functType = "STOP ";
	//	FLGH(3, 3, 3, 3);
	}
	void SUBAHLM() {
//		FLGH(2, 1, 2, 2);
		opClock = 1;
		functType = "SUB A (HL) ";
	}
	void SUBA$xx(unsigned char _8int) {
	//	FLGH(2, 1, 2, 2);
		opClock = 2;
		functType = "SUB A $xx ";
	}
	void SUBAR8R8(unsigned char _r8, unsigned char _r82) {
	//	FLGH(2, 1, 2, 2);
		opClock = 1;
		functType = "SUBA R8 R8 - REVISAR ";
	}
	void SWAPHLM() {
	//	FLGH(2, 0, 0, 0);
		opClock = 2;
		functType = "SWAP (HL) ";
	}
	void SWAPR8R8(unsigned char _r8, unsigned char _r82) {
	//	FLGH(2, 0, 0, 0);
		opClock = 2;
		functType = "SWAP R8 R8 ";
	}
	void XORHLM() {
	//	FLGH(2, 0, 0, 0);
		opClock = 1;
		functType = "XOR (HL) - MAYBE A? ";
	}
	void XOR$xx(unsigned char _8int) {
	//	FLGH(2, 0, 0, 0);
		opClock = 2;
		functType = "XOR $xx ";
	}
	void XORAR8(unsigned char _r8, std::string Dtarget, std::string Starget) {		
		////-- Instruction preset
		opLen = 1;
		FLGH(2, 0, 0, 0, Ra, Ra, 6);
		Ra ^= Ra;
		funcText << "XOR " << Dtarget << ", " << Starget;
		functType = funcText.str();
		opDeb();
		pc += opLen;
		////-- Instruction preset

	}

	//Interrupts
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

	void opDeb() {

		if (pc == 0x100)
		{
			std::cout << "Executing line: " << std::hex << "Pc:" << pc << " -> " << opcode << " " << "NOP" << "\n";
			//FLGH(3, 3, 3, 3);
			pc += 1;
			opLen = 3;
		}
		else {

			if (opLen == 1) {
				std::cout << "Executing line: " << std::hex << "Pc: " << std::setw(4) << std::setfill('0') << pc << " -> " << std::setw(4) << std::setfill('0') << opcode << " | " << functType << "\n";
				//std::cout << "$aabb: " << std::hex << (int)$aabb << "\n";
			}

			else if (opLen == 2) {
				std::cout << "Executing line: " << std::hex << "Pc: " << std::setw(4) << std::setfill('0') << pc << " -> " << std::setw(4) << std::setfill('0') << opcode << " " << std::hex << std::setw(2) << std::setfill('0') << (int)$bb << " | " << functType << "\n";
				//std::cout << "$aabb: " << std::hex << (int)$aabb << "\n";
			}

			else if (opLen == 3) {
				std::cout << "Executing line: " << std::hex << "Pc: " << std::setw(4) << std::setfill('0') << pc << " -> " << std::setw(4) << std::setfill('0') << opcode << " " << std::hex << std::setw(2) << std::setfill('0') << (int)$bb << " " << std::setw(2) << std::setfill('0') << (int)$aa << " | " << functType << "\n";
				//std::cout << "$aabb: " << std::hex << (int)$aabb << "\n";
			}
			else
			{
				std::cout << "Como diablos me sali del ciclo???" << "Cuanto vale opLen?: " << (int(opLen)) << "\n";
			}
		}
		funcText.str(std::string());
		//functType = "";
	}

	void opDecoder() {
		
		_16bitIn = (memoryA[pc + 2] << 8) | memoryA[pc + 3];
		fullOpcode = (memoryA[pc] << 16 | memoryA[pc + 1] << 8 | memoryA[pc + 2]);
		opcode = memoryA[pc];
		$bb = memoryA[pc + 1];
		$xx = memoryA[pc + 1];
		$aa = memoryA[pc + 2];
		hlM = memoryA[Rhl];
		_8bitIn3 = memoryA[pc + 3];
		$aabb = (memoryA[pc + 2] << 8) | (memoryA[pc + 1]);
		
		//std::cout << "Como diablos me sali del ciclo???" << "Cuanto vale opLen?: " << (int(opLen)) << "opcode" << std::hex << (int)opcode << "\n";

		//_8bitInM = memoryA[_8bitIn];

		

		// aa bb = 16bit integer
		switch (opcode) {
		case	0X00:	//			NOP
			NOP();
			tempM = " - 0x00 - NOP";
			break;
		case	0X01:	//	bb	aa	LD	BC	$aabb		{}
			
			tempM = " - 0x01 -LD BC $aabb";
			break;
		case	0X02:	//			LD	(BC)	A		{}
			tempM = " - 0x02 - LD (BC) $aabb";
			break;
		case	0X03:	//			INC	BC			{}
			tempM = " - 0x03 - INC BC";
			break;
		case	0X04:	//			INC	B	
			tempM = " - 0x04 - INC B";
			break;
		case	0X05:	//			DEC	B	
			//opDeb();
			tempM = " - 0x05 - DEC B";
			break;
		case	0X06:	//	xx		LD	B	$xx	
			
			//Dreg = "B";
			//LDR8$xx(Rb, $xx);
			//opDeb();
			//Rb = $xx;
			//pc += opLen;
			tempM = " - 0x06 - LD B $xx";
			break;
		case	0X07:	//			RLCA		.
			tempM = " - 0x07 - RLCA";
			break;
		case	0X08:	//	bb	aa	LD	($aabb)	SP
			tempM = " - 0x08 - LD ($aabb) SP";
			break;
		case	0X09:	//			ADD	HL	BC	
			tempM = " - 0x09 - ADD HL BC";
			break;
		case	0X0A:	//			LD	A	(BC)		
			tempM = " - 0x0A - LD A (BC)";
			break;
		case	0X0B:	//			DEC	BC	
			tempM = " - 0x0B - DEC BC";
			break;
		case	0X0C:	//			INC	C	
			tempM = " - 0x0C INC C";
			break;
		case	0X0D:	//			DEC	C	
			tempM = " - 0x0D - DEC C";
			break;
		case	0X0E:	//	xx		LD	C	$xx	
			//opLen = 2;
			//Dreg = "C";
			//LDR8$xx(Rc, $xx);
			//Rc = $xx;
			//opDeb();
			//pc += opLen;
			tempM = " - 0x0E - LD C $xx";
			break;
		case	0X0F:	//			RRCA		
			tempM = " - 0x0F - RRCA";
			break;
		case	0X10:	//			STOP	
			tempM = " - 0x10 - STOP";
			break;
		case	0X11:	//	bb	aa	LD	DE	$aabb
			tempM = " - 0x11 - LD DE $aabb";
			break;
		case	0X12:	//			LD	(DE)	A	
			tempM = " - 0x12 - LD (DE) A";
			break;
		case	0X13:	//			INC	DE
			tempM = " - 0x13 - INC DE";
			break;
		case	0X14:	//			INC	D	
			tempM = " - 0x14 - INC D";
			break;
		case	0X15:	//			DEC	D	
			tempM = " - 0x015 - DEC D";
			break;
		case	0X16:	//	xx		LD	D	$xx	
			tempM = " - 0x15 - LD D $xx ";
			break;
		case	0X17:	//			RLA	
			tempM = " - 0x17 - RLA";
			break;
		case	0X18:	//	xx		JR	$xx	
			tempM = " - 0x18 - JR $xx";
			break;
		case	0X19:	//			ADD	HL	DE	
			tempM = " - 0x19 - ADD HL DE";
			break;
		case	0X1A:	//			LD	A	(DE)	
			tempM = " - 0x1A - LD A (DE)";
			break;
		case	0X1B:	//			DEC	DE		
			tempM = " - 0x1B - DEC DE";
			break;
		case	0X1C:	//			INC	E	
			tempM = " - 0x1C - INC E";
			break;
		case	0X1D:	//			DEC	E
			tempM = " - 0x1D - DEC E";
			break;
		case	0X1E:	//	xx		LD	E	$xx	
			tempM = " - 0x1E - LD E $xx";
			break;
		case	0X1F:	//			RRA		
			tempM = " - 0x1F - RRA";
			break;
		case	0X20:	//	xx		JR	NZ	$xx
			tempM = " - 0x20 - JR NZ $xx";
			break;
		case	0X21:	//	bb	aa	LD	HL	$aabb
			
			//Dreg = "HL";
			//LOADR16$aabb(Rhl, $aabb);
			//opDeb();
			//Rh = $aa;
			//Rl = $bb;
			//pc += opLen;
			tempM = " - 0x21 - LD HL $aabb";
			break;
		case	0X22:	//			LD	(HLI)	A
			tempM = " - 0x22 - LD (HLI) A";
			break;
		case	0X23:	//			INC	HL
			tempM = " - 0x23 - INC HL";
			break;
		case	0X24:	//			INC	H	
			tempM = " - 0x24 - INC H";
			break;
		case	0X25:	//			DEC	H	
			tempM = " - 0x25 - DEC H";
			break;
		case	0X26:	//	xx		LD	H	$xx	
			tempM = " - 0x26 - LD H $xx";
			break;
		case	0X27:	//			DAA	
			tempM = " - 0x27 - DAA";
			break;
		case	0X28:	//	xx		JR	Z	$xx	
			tempM = " - 0x28 - JR Z $xx";
			break;
		case	0X29:	//			ADD	HL	HL	
			tempM = " - 0x29 - ADD HL HL";
			break;
		case	0X2A:	//			LD	A	(HLI)	
			tempM = " - 0x2A - LD A (HLI)";
			break;
		case	0X2B:	//			DEC	HL	
			tempM = " - 0x2B - DEC HL";
			break;
		case	0X2C:	//			INC	L	
			tempM = " - 0x2C - INC L";
			break;
		case	0X2D:	//			DEC	L		
			tempM = " - 0x2D - DEC L";
			break;
		case	0X2E:	//	xx		LD	L	$xx
			tempM = " - 0x2E - LD L $xx";
			break;
		case	0X2F:	//			CPL				
			tempM = " - 0x2F - CPL";
			break;
		case	0X30:	//	xx		JR	NC	$xx	
			tempM = " - 0x30 - JR NC $xx";
			break;
		case	0X31:	//	bb	aa	LD	SP	$aabb
			tempM = " - 0x31 - LD SP $aabb";
			break;
		case	0X32:	//			LD	(HLD)	A	
			//LOADDHLMR8();
			//opDeb();
			//opLen = 1;
			//pc += 1;
			//std::cout << pc;
			//memoryA[Rhl] = Ra;
			//Rl--;


			//std::cout << "Rhl" << std::hex << int(Rhl) << "\n";
			tempM = " - 0x32 - LD (HLD) A";
			break;
		case	0X33:	//			INC	SP	
			tempM = " - 0x33 - INC SP";
			break;
		case	0X34:	//			INC	(HL)	
			tempM = " - 0x34 - INC (HL)";
			break;
		case	0X35:	//			DEC	(HL)	
			tempM = " - 0x2B - DEC HL";
			break;
		case	0X36:	//	xx		LD	(HL)	$xx
			tempM = " - 0x36 - LD (HL) $xx";
			break;
		case	0X37:	//			SCF		
			tempM = " - 0x37 - SCF";
			break;
		case	0X38:	//	xx		JR	C	$xx	
			tempM = " - 0x38 - JR C $xx";
			break;
		case	0X39:	//			ADD	HL	SP	
			tempM = " - 0x39 - ADD HL SP";
			break;
		case	0X3A:	//			LD	A	(HLD)	
			tempM = " - 0x3A - LD A (HLD)";
			break;
		case	0X3B:	//			DEC	SP		
			tempM = " - 0x3B - DEC SP";
			break;
		case	0X3C:	//			INC	A	
			tempM = " - 0x3C - INC A";
			break;
		case	0X3D:	//			DEC	A		
			tempM = " - 0x3D - DEC A";
			break;
		case	0X3E:	//	xx		LD	A	$xx	
			tempM = " - 0x3E - LD A $xx";
			break;
		case	0X3F:	//			CCF		
			tempM = " - 0x3F - CCF";
			break;
		case	0X40:	//			LD	B	B	
			tempM = " - 0x40 - LD B B";
			break;
		case	0X41:	//			LD	B	C		
			tempM = " - 0x41 - LD B C";
			break;
		case	0X42:	//			LD	B	D		
			tempM = " - 0x42 - LD B D";
			break;
		case	0X43:	//			LD	B	E		
			tempM = " - 0x43 - LD B E";
			break;
		case	0X44:	//			LD	B	H	
			tempM = " - 0x44 - LD B H";
			break;
		case	0X45:	//			LD	B	L		
			tempM = " - 0x45 - LD B L";
			break;
		case	0X46:	//			LD	B	(HL)	
			tempM = " - 0x46 - LD B (HL)";
			break;
		case	0X47:	//			LD	B	A		
			tempM = " - 0x47 - LD B A";
			break;
		case	0X48:	//			LD	C	B	
			tempM = " - 0x48 - LD C B";
			break;
		case	0X49:	//			LD	C	C	
			tempM = " - 0x49 - LD C C";
			break;
		case	0X4A:	//			LD	C	D	
			tempM = " - 0x4A - LD C D";
			break;
		case	0X4B:	//			LD	C	E	
			tempM = " - 0x4B - LD C E";
			break;
		case	0X4C:	//			LD	C	H	
			tempM = " - 0x4C - LD C H";
			break;
		case	0X4D:	//			LD	C	L	
			tempM = " - 0x4D - LD C L";
			break;
		case	0X4E:	//			LD	C	(HL)	
			tempM = " - 0x4E - LD C (HL)";
			break;
		case	0X4F:	//			LD	C	A	
			tempM = " - 0x4F - LD C A";
			break;
		case	0X50:	//			LD	D	B	
			tempM = " - 0x50 - LD D B";
			break;
		case	0X51:	//			LD	D	C		
			tempM = " - 0x51 - LD D C";
			break;
		case	0X52:	//			LD	D	D
			tempM = " - 0x52 - LD D D";
			break;
		case	0X53:	//			LD	D	E	
			tempM = " - 0x53 - LD D E";
			break;
		case	0X54:	//			LD	D	H	
			tempM = " - 0x54 - LD D H";
			break;
		case	0X55:	//			LD	D	L	
			tempM = " - 0x55 - LD D L";
			break;
		case	0X56:	//			LD	D	(HL)	
			tempM = " - 0x56 - LD D (HL)";
			break;
		case	0X57:	//			LD	D	A	
			tempM = " - 0x57 - LD D A";
			break;
		case	0X58:	//			LD	E	B		
			tempM = " - 0x58 - LD E B";
			break;
		case	0X59:	//			LD	E	C	
			tempM = " - 0x59 - LD E C";
			break;
		case	0X5A:	//			LD	E	D
			tempM = " - 0x5A - LD E D";
			break;
		case	0X5B:	//			LD	E	E
			tempM = " - 0x5B - LD E E";
			break;
		case	0X5C:	//			LD	E	H
			tempM = " - 0x5C - LD E H";
			break;
		case	0X5D:	//			LD	E	L	
			tempM = " - 0x5D - LD E L";
			break;
		case	0X5E:	//			LD	E	(HL)	
			tempM = " - 0x5E - LD E (HL)";
			break;
		case	0X5F:	//			LD	E	A		
			tempM = " - 0x5F - LD E A";
			break;
		case	0X60:	//			LD	H	B		
			tempM = " - 0x60 - LD H B";
			break;
		case	0X61:	//			LD	H	C	
			tempM = " - 0x61 - LD H C";
			break;
		case	0X62:	//			LD	H	D	
			tempM = " - 0x62 - LD H D";
			break;
		case	0X63:	//			LD	H	E	
			tempM = " - 0x63 - LD H E";
			break;
		case	0X64:	//			LD	H	H		
			tempM = " - 0x64 - LD H H";
			break;
		case	0X65:	//			LD	H	L		
			tempM = " - 0x65 - LD H L";
			break;
		case	0X66:	//			LD	H	(HL)	
			tempM = " - 0x66 - LD H (HL)";
			break;
		case	0X67:	//			LD	H	A	
			tempM = " - 0x67 - LD E (HL)";
			break;
		case	0X68:	//			LD	L	B		
			tempM = " - 0x68 - LD L B";
			break;
		case	0X69:	//			LD	L	C
			tempM = " - 0x69 - LD L C";
			break;
		case	0X6A:	//			LD	L	D	
			tempM = " - 0x6A - LD L D";
			break;
		case	0X6B:	//			LD	L	E		
			tempM = " - 0x6B - LD L E";
			break;
		case	0X6C:	//			LD	L	H	
			tempM = " - 0x6C - LD L H";
			break;
		case	0X6D:	//			LD	L	L	
			tempM = " - 0x6D - LD L L";
			break;
		case	0X6E:	//			LD	L	(HL)		
			tempM = " - 0x6E - LD L (HL)";
			break;
		case	0X6F:	//			LD	H	A		
			tempM = " - 0x6F - LD H A";
			break;
		case	0X70:	//			LD	(HL)	B		
			tempM = " - 0x70 - LD (HL) B";
			break;
		case	0X71:	//			LD	(HL)	C		
			tempM = " - 0x71 - LD (HL) C";
			break;
		case	0X72:	//			LD	(HL)	D	
			tempM = " - 0x72 - LD (HL) D";
			break;
		case	0X73:	//			LD	(HL)	E		
			tempM = " - 0x73 - LD (HL) E";
			break;
		case	0X74:	//			LD	(HL)	H	
			tempM = " - 0x74 - LD (HL) H";
			break;
		case	0X75:	//			LD	(HL)	L		
			tempM = " - 0x75 - LD (HL) L";
			break;
		case	0X76:	//			HALT			
			tempM = " - 0x76 - HALT";
			break;
		case	0X77:	//			LD	(HL)	A		
			tempM = " - 0x77 - LD (HL) A";
			break;
		case	0X78:	//			LD	A	B		
			tempM = " - 0x78 - LD A B";
			break;
		case	0X79:	//			LD	A	C	
			tempM = " - 0x79 - LD A C";
			break;
		case	0X7A:	//			LD	A	D		
			tempM = " - 0x7A - LD A D";
			break;
		case	0X7B:	//			LD	A	E		
			tempM = " - 0x7B - LD A E";
			break;
		case	0X7C:	//			LD	A	H	
			tempM = " - 0x7C - LD A H";
			break;
		case	0X7D:	//			LD	A	L	
			tempM = " - 0x7D - LD A L";
			break;
		case	0X7E:	//			LD	A	(HL)	
			tempM = " - 0x7E - LD A (HL)";
			break;
		case	0X7F:	//			LD	A	A		
			tempM = " - 0x7F - LD A A";
			break;
		case	0X80:	//			ADD	A	B		
			tempM = " - 0x80 - LD A B";
			break;
		case	0X81:	//			ADD	A	C	
			tempM = " - 0x81 - LD A C";
			break;
		case	0X82:	//			ADD	A	D	
			tempM = " - 0x82 - ADD A D";
			break;
		case	0X83:	//			ADD	A	E	
			tempM = " - 0x83 - ADD A E";
			break;
		case	0X84:	//			ADD	A	H	
			tempM = " - 0x84 - ADD A H";
			break;
		case	0X85:	//			ADD	A	L		
			tempM = " - 0x85 - ADD A L";
			break;
		case	0X86:	//			ADD	A	(HL)	
			tempM = " - 0x86 - ADD A (HL)";
			break;
		case	0X87:	//			ADD	A	A		
			tempM = " - 0x87 - ADD A A";
			break;
		case	0X88:	//			ADC	A	B		
			tempM = " - 0x88 - ADC A B";
			break;
		case	0X89:	//			ADC	A	C		
			tempM = " - 0x89 - ADC A C";
			break;
		case	0X8A:	//			ADC	A	D	
			tempM = " - 0x8A - ADC A D";
			break;
		case	0X8B:	//			ADC	A	E		
			tempM = " - 0x8B - ADC A E";
			break;
		case	0X8C:	//			ADC	A	H	
			tempM = " - 0x8C - ADC A H";
			break;
		case	0X8D:	//			ADC	A	L	
			tempM = " - 0x8D - ADC A L";
			break;
		case	0X8E:	//			ADC	A	(HL)	
			tempM = " - 0x8E - ADC A (HL)";
			break;
		case	0X8F:	//			ADC	A	A		
			tempM = " - 0x8F - ADC A A";
			break;
		case	0X90:	//			SUB	B			
			tempM = " - 0x90 - SUB B";
			break;
		case	0X91:	//			SUB	C			
			tempM = " - 0x91 - SUB C";
			break;
		case	0X92:	//			SUB	D	
			tempM = " - 0x92 - SUB D";
			break;
		case	0X93:	//			SUB	E			
			tempM = " - 0x93 - SUB E";
			break;
		case	0X94:	//			SUB	H		
			tempM = " - 0x94 - SUB H";
			break;
		case	0X95:	//			SUB	L	
			tempM = " - 0x95 - SUB L";
			break;
		case	0X97:	//			SUB	A		
			tempM = " - 0x97 - SUB A";
			break;
		case	0X98:	//			SBC	A	B	
			tempM = " - 0x98 - SBC A B";
			break;
		case	0X99:	//			SBC	A	C		
			tempM = " - 0x99 - SBC A C";
			break;
		case	0X9A:	//			SBC	A	D	
			tempM = " - 0x9A - SBC A D";
			break;
		case	0X9B:	//			SBC	A	E		
			tempM = " - 0x9B - SBC A E";
			break;
		case	0X9C:	//			SBC	A	H	
			tempM = " - 0x9C - SBC A H";
			break;
		case	0X9D:	//			SBC	A	L	
			tempM = " - 0x9D - SBC A L";
			break;
		case	0X9E:	//			SBC	A	(HL)	
			tempM = " - 0x9E - SBC A (HL)";
			break;
		case	0X9F:	//			SBC	A	A		
			tempM = " - 0x9F - SBC A A";
			break;
		case	0XA0:	//			AND	B		
			tempM = " - 0xA0 - AND B";
			break;
		case	0XA1:	//			AND	C		
			tempM = " - 0xA1 - AND C";
			break;
		case	0XA2:	//			AND	D		
			tempM = " - 0xA2 - AND D";
			break;
		case	0XA3:	//			AND	E		
			tempM = " - 0xA3 - AND E";
			break;
		case	0XA4:	//			AND	H		
			tempM = " - 0xA4 - AND H";
			break;
		case	0XA5:	//			AND	L			
			tempM = " - 0xA5 - AND L";
			break;
		case	0XA6:	//			AND	(HL)	
			tempM = " - 0xA6 - AND (HL)";
			break;
		case	0XA7:	//			AND	A			
			tempM = " - 0xA7 - AND A";
			break;
		case	0XA8:	//			XOR	B			
			tempM = " - 0xA8 - XOR B";
			break;
		case	0XA9:	//			XOR	C			
			tempM = " - 0xA9 - XOR C";
			break;
		case	0XAA:	//			XOR	D			
			tempM = " - 0xAA - XOR D";
			break;
		case	0XAB:	//			XOR	E			
			tempM = " - 0xAB - XOR E";
			break;
		case	0XAC:	//			XOR	H			
			tempM = " - 0xAC - XOR H";
			break;
		case	0XAD:	//			XOR	L			
			tempM = " - 0xAD - XOR L";
			break;
		case	0XAE:	//			XOR	(HL)			
			tempM = " - 0xAE - XOR (HL)";
			break;
		case	0XAF:	//			XOR	A
			XORAR8(Ra, "A", "A");
			tempM = " - 0xAF - XOR A";
			break;
		case	0XB0:	//			OR	B			
			tempM = " - 0xB0 - OR B";
			break;
		case	0XB1:	//			OR	C			
			tempM = " - 0xB1 - OR C";
			break;
		case	0XB2:	//			OR	D			
			tempM = " - 0xB2 - OR D";
			break;
		case	0XB3:	//			OR	E			
			tempM = " - 0xB3 - OR E";
			break;
		case	0XB4:	//			OR	H			
			tempM = " - 0xB4 - OR H";
			break;
		case	0XB5:	//			OR	L			
			tempM = " - 0xB5 - OR L";
			break;
		case	0XB6:	//			OR	(HL)			
			tempM = " - 0xB6 - OR (HL)";
			break;
		case	0XB7:	//			OR	A			
			tempM = " - 0xB7 - OR A";
			break;
		case	0XB8:	//			CP	B			
			tempM = " - 0xB8 - CP B";
			break;
		case	0XB9:	//			CP	C			
			tempM = " - 0xB9 - CP C";
			break;
		case	0XBA:	//			CP	D			
			tempM = " - 0xBA - CP D";
			break;
		case	0XBB:	//			CP	E			
			tempM = " - 0xBB - CP E";
			break;
		case	0XBC:	//			CP	H			
			tempM = " - 0xBC - CP H";
			break;
		case	0XBD:	//			CP	L			
			tempM = " - 0xBD - CP L";
			break;
		case	0XBE:	//			CP	(HL)			
			tempM = " - 0xBE - CP (HL)";
			break;
		case	0XBF:	//			CP	A		
			tempM = " - 0xBF - CP A";
			break;
		case	0XC0:	//			RET	NZ		
			tempM = " - 0xC0 - RET NZ";
			break;
		case	0XC1:	//			POP	BC		
			tempM = " - 0xC1 - POP BC";
			break;
		case	0XC2:	//	bb	aa	JP	NZ	$aabb	
			tempM = " - 0xC2 - JP NZ $aabb";
			break;
		case	0XC3:	//	bb	aa	JP	$aabb			
			JP$aabb($aabb);
	
			tempM = " - 0xC3 - JUMP $aabb";
			break;
		case	0XC4:	//	bb	aa	CALL	NZ	$aabb	
			tempM = " - 0xC4 - CALL NZ $aabb";
			break;
		case	0XC5:	//			PUSH	BC			
			tempM = " - 0xC5 - PUSH BC";
			break;
		case	0XC6:	//	xx		ADD	A	$xx	
			tempM = " - 0xC6 - ADD A $xx";
			break;
		case	0XC7:	//			RST	$ 0		
			tempM = " - 0xC7 - RST $ 0";
			break;
		case	0XC8:	//			RET	Z			
			tempM = " - 0xC8 - RET Z";
			break;
		case	0XC9:	//			RET				
			tempM = " - 0xC9 - RET";
			break;
		case	0XCA:	//	bb	aa	JP	Z	$aabb		
			tempM = " - 0xC2A - JP Z $aabb";
			break;

		case	0XCB00:	//			RLC	B		
			tempM = " - 0xCB00 - RLC B";
			break;
		case	0XCB01:	//			RLC	C	
			tempM = " - 0xCB01 - RLC C";
			break;
		case	0XCB02:	//			RLC	D		
			tempM = " - 0xCB02 - RLC D";
			break;
		case	0XCB03:	//			RLC	E			
			tempM = " - 0xCB03 - RLC E";
			break;
		case	0XCB04:	//			RLC	H			
			tempM = " - 0xCB04 - RLC H";
			break;
		case	0XCB05:	//			RLC	L			
			tempM = " - 0xCB05 - RLC L";
			break;
		case	0XCB06:	//			RLC	(HL)			
			tempM = " - 0xCB06 - RLC (HL)";
			break;
		case	0XCB07:	//			RLC	A			
			tempM = " - 0xCB07 - RLC A";
			break;
		case	0XCB08:	//			RRC	B			
			tempM = " - 0xCB08 - RRC B";
			break;
		case	0XCB09:	//			RRC	C			
			tempM = " - 0xCB09 - RRC C";
			break;
		case	0XCB0A:	//			RRC	D			
			tempM = " - 0xCB0A - RRC D";
			break;
		case	0XCB0B:	//			RRC	E			
			tempM = " - 0xCB0B - RRC E";
			break;
		case	0XCB0C:	//			RRC	H			
			tempM = " - 0xCB0C - RRC H";
			break;
		case	0XCB0D:	//			RRC	L			
			tempM = " - 0xCB0D - RRC L";
			break;
		case	0XCB0E:	//			RRC	(HL)			
			tempM = " - 0xCB0E - RRC (HL)";
			break;
		case	0XCB0F:	//			RRC	A			
			tempM = " - 0xCB0F - RRC A";
			break;
		case	0XCB10:	//			RL	B			
			tempM = " - 0xCB10 - RL B";
			break;
		case	0XCB11:	//			RL	C			
			tempM = " - 0xCB11 - RL C";
			break;
		case	0XCB12:	//			RL	D			
			tempM = " - 0xCB12 - RL D";
			break;
		case	0XCB13:	//			RL	E			
			tempM = " - 0xCB13 - RL E";
			break;
		case	0XCB14:	//			RL	H			
			tempM = " - 0xCB14 - RL H";
			break;
		case	0XCB15:	//			RL	L			
			tempM = " - 0xCB15 - RL L";
			break;
		case	0XCB16:	//			RL	(HL)			
			tempM = " - 0xCB16 - RL (HL)";
			break;
		case	0XCB17:	//			RL	A			
			tempM = " - 0xCB17 - RL A";
			break;
		case	0XCB18:	//			RR	B			
			tempM = " - 0xCB18 - RR B";
			break;
		case	0XCB19:	//			RR	C			
			tempM = " - 0xCB19 - RR C";
			break;
		case	0XCB1A:	//			RR	D			
			tempM = " - 0xCB1A - RR D";
			break;
		case	0XCB1B:	//			RR	E			
			tempM = " - 0xCB1B - RR E";
			break;
		case	0XCB1C:	//			RR	H			
			tempM = " - 0xCB1C - RR H";
			break;
		case	0XCB1D:	//			RR	L			
			tempM = " - 0xCB1D - RR L";
			break;
		case	0XCB1E:	//			RR	(HL)			
			tempM = " - 0xCB1E - RR (HL)";
			break;
		case	0XCB1F:	//			RR	A			
			tempM = " - 0xCB1F - RR E";
			break;
		case	0XCB20:	//			SLA	B			
			tempM = " - 0xCB20 - SLA B";
			break;
		case	0XCB21:	//			SLA	C			
			tempM = " - 0xCB21 - SLA C";
			break;
		case	0XCB22:	//			SLA	D			
			tempM = " - 0xCB22 - SLA D";
			break;
		case	0XCB23:	//			SLA	E			
			tempM = " - 0xCB23 - SLA E";
			break;
		case	0XCB24:	//			SLA	H			
			tempM = " - 0xCB24 - SLA H";
			break;
		case	0XCB25:	//			SLA	L			
			tempM = " - 0xCB25 - SLA L";
			break;
		case	0XCB26:	//			SLA	(HL)			
			tempM = " - 0xCB26 - SLA (HL)";
			break;
		case	0XCB27:	//			SLA	A			
			tempM = " - 0xCB27 - SLA A";
			break;
		case	0XCB28:	//			SRA	B			
			tempM = " - 0xCB28 - SRA B";
			break;
		case	0XCB29:	//			SRA	C			
			tempM = " - 0xCB29 - SRA C";
			break;
		case	0XCB2A:	//			SRA	D		
			tempM = " - 0xCB2A - SRD D";
			break;
		case	0XCB2B:	//			SRA	E		
			tempM = " - 0xCB2B - SRA E";
			break;
		case	0XCB2C:	//			SRA	H			
			tempM = " - 0xCB2C - SRA H";
			break;
		case	0XCB2D:	//			SRA	L		
			tempM = " - 0xCB2D - SRA L";
			break;
		case	0XCB2E:	//			SRA	(HL)			
			tempM = " - 0xCB2E - SRA (HL)";
			break;
		case	0XCB2F:	//			SRA	A			
			tempM = " - 0xCB2F - SRA A";
			break;
			/////////////////REVISAR ESTA TRANSICION ////////////
		case	0XCB37:	//			SWAP	A			
			tempM = " - 0xCB37 - SWAP A";
			break;
		case	0XCB38:	//			SRL	B			
			tempM = " - 0xCB38 - SRL B";
			break;
		case	0XCB39:	//			SRL	C		
			tempM = " - 0xCB39 - SRL C";
			break;
		case	0XCB3A:	//			SRL	D			
			tempM = " - 0xCB3A - SRL D";
			break;
		case	0XCB3B:	//			SRL	E		
			tempM = " - 0xCB3B - SRL E";
			break;
		case	0XCB3C:	//			SRL	H			
			tempM = " - 0xCB3C - SRL H";
			break;
		case	0XCB3D:	//			SRL	L		
			tempM = " - 0xCB3D - SRL L";
			break;
		case	0XCB3E:	//			SRL	(HL)		
			tempM = " - 0xCB3E - SRL (HL)";
			break;
		case	0XCB3F:	//			SRL	A			
			tempM = " - 0xCB3F - SRL A";
			break;
		case	0XCB40:	//			BIT	0	B		
			tempM = " - 0xCB40 - BIT 0 B";
			break;
		case	0XCB41:	//			BIT	0	C		
			tempM = " - 0xCB41 - BIT 0 C";
			break;
		case	0XCB42:	//			BIT	0	D		
			tempM = " - 0xCB42 - BIT 0 D";
			break;
		case	0XCB43:	//			BIT	0	E	
			tempM = " - 0xCB43 - BIT 0 E";
			break;
		case	0XCB44:	//			BIT	0	H	
			tempM = " - 0xCB44 - BIT 0 H";
			break;
		case	0XCB45:	//			BIT	0	L		
			tempM = " - 0xCB45 - BIT 0 L";
			break;
		case	0XCB46:	//			BIT	0	(HL)	
			tempM = " - 0xCB46 - BIT 0 (HL)";
			break;
		case	0XCB47:	//			BIT	0	A		
			tempM = " - 0xCB47 - BIT 0 A";
			break;
		case	0XCB48:	//			BIT	1	B	
			tempM = " - 0xCB48 - BIT 1 B";
			break;
		case	0XCB49:	//			BIT	1	C		
			tempM = " - 0xCB49 - BIT 1 C";
			break;
		case	0XCB4A:	//			BIT	1	D		
			tempM = " - 0xCB4A - BIT 1 D";
			break;
		case	0XCB4B:	//			BIT	1	E		
			tempM = " - 0xCB4B - BIT 1 E";
			break;
		case	0XCB4C:	//			BIT	1	H		
			tempM = " - 0xCB4C - BIT 1 H";
			break;
		case	0XCB4D:	//			BIT	1	L		
			tempM = " - 0xCB4D - BIT 1 L";
			break;
		case	0XCB4E:	//			BIT	1	(HL)	
			tempM = " - 0xCB4A - BIT 1 (HL)";
			break;
		case	0XCB4F:	//			BIT	1	A		
			tempM = " - 0xCB4F - BIT 1 A";
			break;
		case	0XCB50:	//			BIT	2	B		
			tempM = " - 0xCB50 - BIT 2 B";
			break;
		case	0XCB51:	//			BIT	2	C		
			tempM = " - 0xCB51 - BIT 2 C";
			break;
		case	0XCB52:	//			BIT	2	D		
			tempM = " - 0xCB52 - BIT 2 D";
			break;
		case	0XCB53:	//			BIT	2	E	
			tempM = " - 0xCB53 - BIT 2 E";
			break;
		case	0XCB54:	//			BIT	2	H	
			tempM = " - 0xCB54 - BIT 2 H";
			break;
		case	0XCB55:	//			BIT	2	L		
			tempM = " - 0xCB55 - BIT 2 L";
			break;
		case	0XCB56:	//			BIT	2	(HL)		
			tempM = " - 0xCB56 - BIT 2 (HL)";
			break;
		case	0XCB57:	//			BIT	2	A		
			tempM = " - 0xCB57 - BIT 2 A";
			break;
		case	0XCB58:	//			BIT	3	B	
			tempM = " - 0xCB58 - BIT 3 B";
			break;
		case	0XCB59:	//			BIT	3	C	
			tempM = " - 0xCB59 - BIT 3 C";
			break;
		case	0XCB5A:	//			BIT	3	D	
			tempM = " - 0xCB5A - BIT 3 D";
			break;
		case	0XCB5B:	//			BIT	3	E		
			tempM = " - 0xCB5B - BIT 3 E";
			break;
		case	0XCB5C:	//			BIT	3	H	
			tempM = " - 0xCB5C - BIT 3 H";
			break;
		case	0XCB5D:	//			BIT	3	L		
			tempM = " - 0xCB5D - BIT 3 L";
			break;
		case	0XCB5E:	//			BIT	3	(HL)	
			tempM = " - 0xCB5E - BIT 3 (HL)";
			break;
		case	0XCB5F:	//			BIT	3	A		
			tempM = " - 0xCB5F - BIT 3 A";
			break;
		case	0XCB60:	//			BIT	4	B		
			tempM = " - 0xCB60 - BIT 4 B";
			break;
		case	0XCB61:	//			BIT	4	C		
			tempM = " - 0xCB61 - BIT 4 C";
			break;
		case	0XCB62:	//			BIT	4	D		
			tempM = " - 0xCB62 - BIT 4 D";
			break;
		case	0XCB63:	//			BIT	4	E	
			tempM = " - 0xCB63 - BIR 4 E";
			break;
		case	0XCB64:	//			BIT	4	H		
			tempM = " - 0xCB64 - BIT 4 H";
			break;
		case	0XCB65:	//			BIT	4	L		
			tempM = " - 0xCB65 - BIT 4 L";
			break;
		case	0XCB66:	//			BIT	4	(HL)	
			tempM = " - 0xCB66 - BIT 4 (HL)";
			break;
		case	0XCB67:	//			BIT	4	A		
			tempM = " - 0xCB67 - BIT 4 A";
			break;
		case	0XCB68:	//			BIT	5	B		
			tempM = " - 0xCB68 - BIT 5 B";
			break;
		case	0XCB69:	//			BIT	5	C		
			tempM = " - 0xCB69 - BIT 5 C";
			break;
		case	0XCB6A:	//			BIT	5	D		
			tempM = " - 0xCB6A - BIT 5 D";
			break;
		case	0XCB6B:	//			BIT	5	E		
			tempM = " - 0xCB6B - BIT 5 E";
			break;
		case	0XCB6C:	//			BIT	5	H		
			tempM = " - 0xCB6C - BIT 5 H";
			break;
		case	0XCB6D:	//			BIT	5	L		
			tempM = " - 0xCB6D - BIT 5 L";
			break;
		case	0XCB6E:	//			BIT	5	(HL)	
			tempM = " - 0xCB6E - BIT 5 (HL)";
			break;
		case	0XCB6F:	//			BIT	5	A		
			tempM = " - 0xCB6F - BIT 5 A";
			break;
		case	0XCB70:	//			BIT	6	B		
			tempM = " - 0xCB70 - BIT 6 B";
			break;
		case	0XCB71:	//			BIT	6	C		
			tempM = " - 0xCB71 - BIT 6 C";
			break;
		case	0XCB72:	//			BIT	6	D		
			tempM = " - 0xCB72 - BIT 6 D";
			break;
		case	0XCB73:	//			BIT	6	E		
			tempM = " - 0xCB73 - BIT 6 E";
			break;
		case	0XCB74:	//			BIT	6	H		
			tempM = " - 0xCB74 - BIT 6 H";
			break;
		case	0XCB75:	//			BIT	6	L	
			tempM = " - 0xCB75 - BIT 6 L";
			break;
		case	0XCB76:	//			BIT	6	(HL)	
			tempM = " - 0xCB76 - BIT 6 (HL)";
			break;
		case	0XCB77:	//			BIT	6	A	
			tempM = " - 0xCB77 - BIT 6 A";
			break;
		case	0XCB78:	//			BIT	7	B	
			tempM = " - 0xCB78 - BIT 7 B";
			break;
		case	0XCB79:	//			BIT	7	C	
			tempM = " - 0xCB79 - BIT 7 C";
			break;
		case	0XCB7A:	//			BIT	7	D	
			tempM = " - 0xCB7A - BIT 7 D";
			break;
		case	0XCB7B:	//			BIT	7	E	
			tempM = " - 0xCB7B - BIT 7 E";
			break;
		case	0XCB7C:	//			BIT	7	H	
			tempM = " - 0xCB7C - BIT 7 H";
			break;
		case	0XCB7D:	//			BIT	7	(HL)	
			tempM = " - 0xCB7D - BIT 7 (HL)";
			break;
		case	0XCB7F:	//			BIT	7	A	
			tempM = " - 0xCB7F - BIT 7 A";
			break;
		case	0XCB80:	//			RES	0	B	
			tempM = " - 0xCB80 - RES 0 B";
			break;
		case	0XCB81:	//			RES	0	C	
			tempM = " - 0xCB81 - RES 0 C";
			break;
		case	0XCB82:	//			RES	0	D	
			tempM = " - 0xCB82 - RES 0 D";
			break;
		case	0XCB83:	//			RES	0	E	
			tempM = " - 0xCB83 - RES 0 E";
			break;
		case	0XCB84:	//			RES	0	H	
			tempM = " - 0xCB84 - RES 0 H";
			break;
		case	0XCB85:	//			RES	0	L	
			tempM = " - 0xCB85 - RES 0 L";
			break;
		case	0XCB86:	//			RES	0	(HL)	
			tempM = " - 0xCB86 - RES 0 (HL)";
			break;
		case	0XCB87:	//			RES	0	A	
			tempM = " - 0xCB87 - RES 0 A";
			break;
		case	0XCB88:	//			RES	1	B	
			tempM = " - 0xCB88 - RES 1 B";
			break;
		case	0XCB89:	//			RES	1	C	
			tempM = " - 0xCB89 - RES 1 C";
			break;
		case	0XCB8A:	//			RES	1	D	
			tempM = " - 0xCB8A - RES 1 D";
			break;
		case	0XCB8B:	//			RES	1	E	
			tempM = " - 0xCB8B - RES 1 E";
			break;
		case	0XCB8C:	//			RES	1	H	
			tempM = " - 0xCB8C - RES 1 H";
			break;
		case	0XCB8D:	//			RES	1	L	
			tempM = " - 0xCB8D - RES 1 L";
			break;
		case	0XCB8E:	//			RES	1	(HL)	
			tempM = " - 0xCB8E - RES 1 (HL)";
			break;
		case	0XCB8F:	//			RES	1	A	
			tempM = " - 0xCB8F - RES 1 A";
			break;
		case	0XCB90:	//			RES	2	B	
			tempM = " - 0xCB90 - RES 2 B";
			break;
		case	0XCB91:	//			RES	2	C	
			tempM = " - 0xCB91 - RES 2 C";
			break;
		case	0XCB92:	//			RES	2	D	
			tempM = " - 0xCB92 - RES 2 D";
			break;
		case	0XCB93:	//			RES	2	E	
			tempM = " - 0xCB93 - RES 2 E";
			break;
		case	0XCB94:	//			RES	2	H	
			tempM = " - 0xCB94 - RES 2 H";
			break;
		case	0XCB95:	//			RES	2	L	
			tempM = " - 0xCB95 - RES 2 L";
			break;
		case	0XCB96:	//			RES	2	(HL)	
			tempM = " - 0xCB96 - RES 2 (HL)";
			break;
		case	0XCB97:	//			RES	2	A	
			tempM = " - 0xCB97 - RES 2 A";
			break;
		case	0XCB98:	//			RES	3	B	
			tempM = " - 0xCB98 - RES 3 B";
			break;
		case	0XCB99:	//			RES	3	C	
			tempM = " - 0xCB99 - RES 3 B";
			break;
		case	0XCB9A:	//			RES	3	D	
			tempM = " - 0xCB9A - RES 3 D";
			break;
		case	0XCB9B:	//			RES	3	E	
			tempM = " - 0xCB9B - RES 3 E";
			break;
		case	0XCB9C:	//			RES	3	H	
			tempM = " - 0xCB9C - RES 3 H";
			break;
		case	0XCB9D:	//			RES	3	L	
			tempM = " - 0xCB9D - RES 3 L";
			break;
		case	0XCB9E:	//			RES	3	(HL)	
			tempM = " - 0xCB9E - RES 3 (HL)";
			break;
		case	0XCB9F:	//			RES	3	A	
			tempM = " - 0xCB9F - RES 3 A";
			break;
		case	0XCBA0:	//			RES	4	B	
			tempM = " - 0xCBA0 - RES 4 B";
			break;
		case	0XCBA1:	//			RES	4	C	
			tempM = " - 0xCBA1 - RES 4 C";
			break;
		case	0XCBA2:	//			RES	4	D		
			tempM = " - 0xCBA2 - RES 4 D";
			break;
		case	0XCBA3:	//			RES	4	E	
			tempM = " - 0xCBA3 - RES 4 E";
			break;
		case	0XCBA4:	//			RES	4	H	
			tempM = " - 0xCBA4 - RES 4 H";
			break;
		case	0XCBA5:	//			RES	4	L	
			tempM = " - 0xCBA5 - RES 4 L";
			break;
		case	0XCBA6:	//			RES	4	(HL)	
			tempM = " - 0xCBA6 - RES 4 (HL)";
			break;
		case	0XCBA7:	//			RES	4	A	
			tempM = " - 0xCBA7 - RES 4 A";
			break;
		case	0XCBA8:	//			RES	5	B	
			tempM = " - 0xCBA8 - RES 5 B";
			break;
		case	0XCBA9:	//			RES	5	C	
			tempM = " - 0xCBA9 - RES 5 C";
			break;
		case	0XCBAA:	//			RES	5	D	
			tempM = " - 0xCBAA - RES 5 D";
			break;
		case	0XCBAB:	//			RES	5	E	
			tempM = " - 0xCBAB - RES 5 E";
			break;
		case	0XCBAC:	//			RES	5	H	
			tempM = " - 0xCBAC - RES 5 H";
			break;
		case	0XCBAD:	//			RES	5	L	
			tempM = " - 0xCBAD - RES 5 L";
			break;
		case	0XCBAE:	//			RES	5	(HL)	
			tempM = " - 0xCBAE - RES 5 (HL)";
			break;
		case	0XCBAF:	//			RES	5	A	
			tempM = " - 0xCBAF - RES 5 A";
			break;
		case	0XCBB0:	//			RES	6	B	
			tempM = " - 0xCBB0 - RES 6 B";
			break;
		case	0XCBB1:	//			RES	6	C	
			tempM = " - 0xCBB1 - RES 6 C";
			break;
		case	0XCBB2:	//			RES	6	D	
			tempM = " - 0xCBB2 - RES 6 D";
			break;
		case	0XCBB3:	//			RES	6	E	
			tempM = " - 0xCBB3 - RES 6 E";
			break;
		case	0XCBB4:	//			RES	6	H	
			tempM = " - 0xCBB4 - RES 6 H";
			break;
		case	0XCBB5:	//			RES	6	L	
			tempM = " - 0xCBB5 - RES 6 L";
			break;
		case	0XCBB6:	//			RES	6	(HL)	
			tempM = " - 0xCBB6 - RES 6 (HL)";
			break;
		case	0XCBB7:	//			RES	6	A	
			tempM = " - 0xCBB7 - RES 6 A";
			break;
		case	0XCBB8:	//			RES	7	B	
			tempM = " - 0xCBB8 - RES 7 B";
			break;
		case	0XCBB9:	//			RES	7	C	
			tempM = " - 0xCBB9 - RES 7 C";
			break;
		case	0XCBBA:	//			RES	7	D	
			tempM = " - 0xCBBA - RES 7 D";
			break;
		case	0XCBBB:	//			RES	7	E	
			tempM = " - 0xCBBB - RES 7 E";
			break;
		case	0XCBBC:	//			RES	7	H	
			tempM = " - 0xCBBC - RES 7 H";
			break;
		case	0XCBBD:	//			RES	7	L	
			tempM = " - 0xCBBD - RES 7 L";
			break;
		case	0XCBBE:	//			RES	7	(HL)	
			tempM = " - 0xCBBE - RES 7 (HL)";
			break;
		case	0XCBBF:	//			RES	7	A	
			tempM = " - 0xCBBF - RES 7 A";
			break;
		case	0XCBC0:	//			SET	0	B	
			tempM = " - 0xCBC0 - SET 0 B";
			break;
		case	0XCBC1:	//			SET	0	C	
			tempM = " - 0xCBC1 - SET 0 C";
			break;
		case	0XCBC2:	//			SET	0	D	
			tempM = " - 0xCBC2 - SET 0 D";
			break;
		case	0XCBC3:	//			SET	0	E	
			tempM = " - 0xCBC3 - SET 0 E";
			break;
		case	0XCBC4:	//			SET	0	H	
			tempM = " - 0xCBC4 - SET 0 H";
			break;
		case	0XCBC5:	//			SET	0	L	
			tempM = " - 0xCBC5 - SET 0 L";
			break;
		case	0XCBC6:	//			SET	0	(HL)	
			tempM = " - 0xCBC6 - SET 0 (HL)";
			break;
		case	0XCBC7:	//			SET	0	A	
			tempM = " - 0xCBC7 - SET 0 a";
			break;
		case	0XCBC8:	//			SET	1	B	
			tempM = " - 0xCBC8 - SET 1 B";
			break;
		case	0XCBC9:	//			SET	1	C	
			tempM = " - 0xCBC9 - SET 1 C";
			break;
		case	0XCBCA:	//			SET	1	D	
			tempM = " - 0xCBCA - SET 1 D";
			break;
		case	0XCBCB:	//			SET	1	E	
			tempM = " - 0xCBCB - SET 1 E";
			break;
		case	0XCBCC:	//			SET	1	H	
			tempM = " - 0xCBCC - SET 1 H";
			break;
		case	0XCBCD:	//			SET	1	L	
			tempM = " - 0xCBCD - SET 1 L";
			break;
		case	0XCBCE:	//			SET	1	(HL)	
			tempM = " - 0xCBCE - SET 1 (HL)";
			break;
		case	0XCBCF:	//			SET	1	A	
			tempM = " - 0xCBCF - SET 1 A";
			break;
		case	0XCBD0:	//			SET	2	B	
			tempM = " - 0xCBD0 - SET 2 B";
			break;
		case	0XCBD1:	//			SET	2	C	
			tempM = " - 0xCBD1 - SET 2 C";
			break;
		case	0XCBD2:	//			SET	2	D	
			tempM = " - 0xCBD2 - SET 2 D";
			break;
		case	0XCBD3:	//			SET	2	E	
			tempM = " - 0xCBD3 - SET 2 E";
			break;
		case	0XCBD4:	//			SET	2	H	
			tempM = " - 0xCBD4 - SET 2 H";
			break;
		case	0XCBD5:	//			SET	2	L	
			tempM = " - 0xCBD5 - SET 2 L";
			break;
		case	0XCBD6:	//			SET	2	(HL)	
			tempM = " - 0xCBD6 - SET 2 (HL)";
			break;
		case	0XCBD7:	//			SET	2	A	
			tempM = " - 0xCBD7 - SET 2 A";
			break;
		case	0XCBD8:	//			SET	3	B	
			tempM = " - 0xCBD8 - SET 3 B";
			break;
		case	0XCBD9:	//			SET	3	C	
			tempM = " - 0xCBD9 - SET 3 C";
			break;
		case	0XCBDA:	//			SET	3	D	
			tempM = " - 0xCBDA - SET 3 D";
			break;
		case	0XCBDB:	//			SET	3	E	
			tempM = " - 0xCBD8 - SET 3 E";
			break;
		case	0XCBDC:	//			SET	3	H	
			tempM = " - 0xCBDC - SET 3 H";
			break;
		case	0XCBDD:	//			SET	3	L	
			tempM = " - 0xCBDD - SET 3 L";
			break;
		case	0XCBDE:	//			SET	3	(HL)	
			tempM = " - 0xCBDE - SET 3 (HL)";
			break;
		case	0XCBDF:	//			SET	3	A	
			tempM = " - 0xCBDF - SET 3 A";
			break;
		case	0XCBE0:	//			SET	4	B	
			tempM = " - 0xCBE0 - SET 4 B";
			break;
		case	0XCBE1:	//			SET	4	C	
			tempM = " - 0xCBE1 - SET 4 C";
			break;
		case	0XCBE2:	//			SET	4	D	
			tempM = " - 0xCBE2 - SET 4 D";
			break;
		case	0XCBE3:	//			SET	4	E	
			tempM = " - 0xCBE3 - SET 4 E";
			break;
		case	0XCBE4:	//			SET	4	H	
			tempM = " - 0xCBE4 - SET 4 H";
			break;
		case	0XCBE5:	//			SET	4	L	
			tempM = " - 0xCBE5 - SET 4 L";
			break;
		case	0XCBE6:	//			SET	4	(HL)	
			tempM = " - 0xCBE6 - SET 4 (HL)";
			break;
		case	0XCBE7:	//			SET	4	A	
			tempM = " - 0xCBE7 - SET 4 A";
			break;
		case	0XCBE8:	//			SET	5	B	
			tempM = " - 0xCBE8 - SET 5 B";
			break;
		case	0XCBE9:	//			SET	5	C	
			tempM = " - 0xCBE9 - SET 5 C";
			break;
		case	0XCBEA:	//			SET	5	D	
			tempM = " - 0xCBEA - SET 5 D";
			break;
		case	0XCBEB:	//			SET	5	E	
			tempM = " - 0xCBEB - SET 5 E";
			break;
		case	0XCBEC:	//			SET	5	H	
			tempM = " - 0xCBEC - SET 5 H";
			break;
		case	0XCBED:	//			SET	5	L	
			tempM = " - 0xCBED - SET 5 L";
			break;
		case	0XCBEE:	//			SET	5	(HL)	
			tempM = " - 0xCBEE - SET 5 (HL)";
			break;
		case	0XCBEF:	//			SET	5	A	
			tempM = " - 0xCBEF - SET 5 A";
			break;
		case	0XCBF0:	//			SET	6	B	
			tempM = " - 0xCBF0 - SET 6 B";
			break;
		case	0XCBF1:	//			SET	6	C	
			tempM = " - 0xCBF1 - SET 6 C";
			break;
		case	0XCBF2:	//			SET	6	D	
			tempM = " - 0xCBF2 - SET 6 D";
			break;
		case	0XCBF3:	//			SET	6	E	
			tempM = " - 0xCBF3 - SET 6 E";
			break;
		case	0XCBF4:	//			SET	6	H	
			tempM = " - 0xCBF4 - SET 6 H";
			break;
		case	0XCBF5:	//			SET	6	L	
			tempM = " - 0xCBF5 - SET 6 L";
			break;
		case	0XCBF6:	//			SET	6	(HL)	
			tempM = " - 0xCBF6 - SET 6 (HL)";
			break;
		case	0XCBF7:	//			SET	6	A	
			tempM = " - 0xCBF7 - SET 6 A";
			break;
		case	0XCBF8:	//			SET	7	B	
			tempM = " - 0xCBF8 - SET 7 B";
			break;
		case	0XCBF9:	//			SET	7	C	
			tempM = " - 0xCBF9 - SET 7 C";
			break;
		case	0XCBFA:	//			SET	7	D	
			tempM = " - 0xCBFA - SET 7 D";
			break;
		case	0XCBFB:	//			SET	7	E	
			tempM = " - 0xCBFB - SET 7 E";
			break;
		case	0XCBFC:	//			SET	7	H	
			tempM = " - 0xCBFC - SET 7 H";
			break;
		case	0XCBFD:	//			SET	7	L	
			tempM = " - 0xCBFD - SET 7 L";
			break;
		case	0XCBFE:	//			SET	7	(HL)	
			tempM = " - 0xCBFE - SET 7 (HL)";
			break;
		case	0XCBFF:	//			SET	7	A
			tempM = " - 0xCBFF - SET 7 A";
			break;

		case	0XCC:	//	bb	aa	CALL	Z	$aabb	
			tempM = " - 0xCC - CALL Z $aabb";
			break;
		case	0XCD:	//	bb	aa	CALL	$aabb		
			tempM = " - 0xCD - CALL $aabb";
			break;
		case	0XCE:	//	xx		ADC	A	$xx		
			tempM = " - 0xCE - ADC A $xx";
			break;
		case	0XCF:	//			RST	$ 8
			tempM = " - 0xCF - RST $8";
			break;
		case	0XD0:	//			RET	NC			
			tempM = " - 0xD0 - RET NC";
			break;
		case	0XD1:	//			POP	DE			
			tempM = " - 0xD1 - POP DE";
			break;
		case	0XD2:	//	bb	aa	JP	NC	$aabb	
			tempM = " - 0xD2 - JP NC $aabb";
			break;
		case	0XD4:	//	bb	aa	CALL	NC	$aabb
			tempM = " - 0xD4 - CALL NC $aabb";
			break;
		case	0XD5:	//			PUSH	DE
			tempM = " - 0xD5 - PUSH DE";
			break;
		case	0XD6:	//	xx		SUB	$xx			
			tempM = " - 0xD6 - SUB $xx";
			break;
		case	0XD7:	//			RST	$ 10			
			tempM = " - 0xD7 - RST $10";
			break;
		case	0XD8:	//			RET	C
			tempM = " - 0xD8 - RET C";
			break;
		case	0XD9:	//			RETI
			tempM = " - 0xD9 - RETI";
			break;
		case	0XDA:	//	bb	aa	JP	C	$aabb		
			tempM = " - 0xDA - JP C $aabb";
			break;
		case	0XDC:	//	bb	aa	CALL	C	$aabb
			tempM = " - 0xDC - CALL C $aabb";
			break;
		case	0XDE:	//	xx		SBC	A	$xx		
			tempM = " - 0xDE - SBC A $xx";
			break;
		case	0XDF:	//			RST	$ 18
			tempM = " - 0xDF - RST $18";
			break;
		case	0XE0:	//	xx		LD	($xx)	A
			tempM = " - 0xE0 - LD ($xx) A";
			break;
		case	0XE1:	//			POP	HL			
			tempM = " - 0xE1 - POP HL";
			break;
		case	0XE2:	//			LD	(C)	A
			tempM = " - 0xE2 - LD (C) A";
			break;
		case	0XE5:	//			PUSH	HL
			tempM = " - 0xE5 - PUSH HL";
		case	0XE6:	//	xx		AND	$xx		
			tempM = " - 0xE6 - AND $xx";
			break;
		case	0XE7:	//			RST	$ 20
			tempM = " - 0xE7 - RST $20";
			break;
		case	0XE8:	//	xx		ADD	SP	xx		
			tempM = " - 0xE8 - ADD SP xx";
			break;
		case	0XE9:	//			JP	(HL)
			tempM = " - 0xE9 - JP (HL)";
			break;
		case	0XEA:	//	bb	aa	LD	($aabb)	A
			tempM = " - 0xEA - LD ($aabb) A";
			break;
		case	0XEE:	//	xx		XOR	$xx			
			tempM = " - 0xEE - XOR $xx";
			break;
		case	0XEF:	//			RST	$ 28
			tempM = " - 0xEF - RST $28";
			break;
		case	0XF0:	//	xx		LD	A	($xx)
			tempM = " - 0xF0 - LD A ($xx)";
			break;
		case	0XF1:	//			POP	AF			
			tempM = " - 0xF1 - POP AF";
			break;
		case	0XF2:	//			LD	A	(C)
			tempM = " - 0xF2 - LD A (C)";
			break;
		case	0XF3:	//			DI	
			tempM = " - 0xF3 - DI";
			// Y EL F4!!!???/
			break;
		case	0XF5:	//			PUSH	AF
			tempM = " - 0xF5 - PUSH AF";
			break;
		case	0XF6:	//	xx		OR	$xx		
			tempM = " - 0xF6 - xx OR $xx";
			break;
		case	0XF7:	//			RST	$ 30
			tempM = " - 0xF7 - RST $30";
			break;
		case	0XF8:	//			LD	HL	SP		
			tempM = " - 0xF8 - LD HL SP";
			break;
		case	0XF9:	//			LD	SP	HL
			tempM = " - 0xF9 - LD SP HL";
			break;
		case	0XFA:	//	bb	aa	LD	A	($aabb)
			tempM = " - 0xFA - LD A ($aabb)";
			break;
		case	0XFB:	//			EI	
			tempM = " - 0xFB - EI";
			break;
		case	0XFE:	//	xx		CP	$xx			
			tempM = " - 0xFE - CP $xx";
			break;
		case	0XFF:	//			RST	$ 38			
			tempM = " - 0xFF - RST $38";
			break;

		default:
			std::cout << "ERROR CASE NOT FOUND " << "opcode" << std::hex << std::setw(2) << std::setfill('0') << int(opcode) << "\n";
			break;	
		}

		
		std::cout << "Next opcode" << " | " << "Engager: " << std::hex << std::setw(4) << std::setfill('0') << opcode << "-" << tempM << "\n";
		//std::cout << "$aabb: " << std::hex << (int)$aabb << "\n";
		


	} //end opDecoder

#pragma endregion

	//flag handler
	//el register F mantiene 4 flags
	//2 sera usado como caso de error indistinto



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

	void FlagController() {

	}


	void dispatch() {
		//		unsigned short opcode;
		// retrieve opcode
		//	opcode =   ( gb::mmu.rb (memoryT[reg.pc])  << 8 ) || ( gb::mmu.rb(memoryA[reg.pc + 1]));
		// assign registers

		//std::cout << "\nPC: " << std::hex << (int)pc << "\n";
		//gb::MMU(pc, 1, 0);

		//	opcode = memoryA[reg.pc];
		//	std::cout << std::hex << (int)memoryA[0x101] << "prueba";

		//std::cout << "\nOpcode: " << std::hex << (int)opcode;
		//		std::cout << "\nOpcodeINd: " << std::hex << (int)memoryA[pc];
		//Ra = 0xC;
		//Rf = 0x8;
		RegComb();
		FlagState();

		/// Gracias modulo, me ense;aste un monton
		/*
		Rf = 0xB0;
		unsigned char a = 0xf0;
		
		std::cout << "Rf desde Reg: " << std::hex << (int)Rf << "\n";
		std::bitset<8> x(Rf);

		std::cout << "Rf transformado: " << std::hex << x << "\n";

		//x.reset(7); // Reset Z
		//x.reset(6); // Reset N
		//x.reset(5); // Reset H
		//x.reset(4); // Reset C

		//x.set(7); // Set Z
		//x.set(6); // Set N
		//x.set(5); // Set H
		//x.set(4); // Set C

		Rf = x.to_ulong();
		std::cout << "Rf Bitseteado: " << std::hex << (int)Rf << "\n";


		std::cout << x << "\n";
		
		unsigned char testbit = 0xf0;
		unsigned char chartest = 0;
		unsigned char zt;
		unsigned char nt;
		unsigned char ht;
		unsigned char ct;

		ct = (testbit & 0x16) >> 13; // 1 2 4 8 16 32 64 128
		ht = (testbit & 0x32) >> 14;
		nt = (testbit & 0x64) >> 15;
		zt = (testbit & 0x8F) >> 8;

		std::cout << "\ncharTest: 0x" << std::setw(2) << std::setfill('0')  << std::hex << (int)testbit;
		std::cout << "\nZ FLAG: " << std::hex << (int)zt << "\n";
		std::cout << "N FLAG: " << std::hex << (int)nt << "\n";
		std::cout << "H FLAG: " << std::hex << (int)ht << "\n";
		std::cout << "C FLAG: " << std::hex << (int)ct << "\n";

		//testbit &= ~(0x2 << 1); // set 2nd bit to 1
		//clears Z N H C
		//testbit &= ~(1 << 3);
		//testbit &= ~(1 << 2);
		//testbit &= ~(1 << 1);
		//testbit &= ~(1);

		// set Z N H C
		testbit |= (1 << 3);
		testbit |= (1 << 2);
		testbit |= (1 << 1);
		testbit |= (1);

		std::cout << "\nbittest mod: " << std::hex << (int)testbit;

		zt = (testbit & 0x8) >> 3;
		nt = (testbit & 0x4) >> 2;
		ht = (testbit & 0x2) >> 1;
		ct = (testbit & 0x1);

		std::cout << "\nZ FLAG: " << std::hex << (int)zt << "\n";
		std::cout << "N FLAG: " << std::hex << (int)nt << "\n";
		std::cout << "H FLAG: " << std::hex << (int)ht << "\n";
		std::cout << "C FLAG: " << std::hex << (int)ct << "\n";

		std::cout << "\ncharTest: 0x" << std::setw(2) << std::setfill('0') << std::hex << (int)testbit;


		

		//supongamos que tengo stored 1100

		*/

		// modify clocks
		//gb::Rm += gb::Rm;
		//gb::Rt += gb::Rt;


	}


	void LoadFile()
	{
		std::ifstream fin(ROM, std::ios::binary);
		if (!fin)
		{
			std::cout << "no se puede abrir archivo" << std::endl;
		}

		for (int i = 0; i < 0x8000; i++)
		{
			fin.get(letter);
			memoryT[i] = letter;

		}


		for (int i = 0; i<0x8000; i++)
		{
			//std::cout << std::hex << (int)memoryT[i];





			//	std::cout << std::hex << (int)memoryT[0x100];
			//	std::cout << std::hex << (int)memoryT[0x102];

		}
	}; // fin LoadFile

	void MemAssign()
	{
		for (int i = 0; i <= 0xFFFE; i++)
		{
			if (i >= 0 && i < 0x4000)
			{
				memoryA[i] = memoryT[i];
			}

		}


	}; //-- Fin MA



}; //-- Fin class GB
//-- Modulo Main




int main()
{
	//experimental SDL support
	nullRender = true;
	/*
	if (!nullRender)
	{
		//TTF_Init();
		SDL_Window *window = NULL;
		// SDL_Window* debugger = NULL;

		window = SDL_CreateWindow
			("Gb++AV", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, 1080, 1920, SDL_WINDOW_SHOWN);

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	}
	*/

	gb gameboy;
	gameboy.Reset();
	//gameboy.Init();
	gameboy.LoadFile();
	gameboy.MemAssign();
	
	unsigned short intType;
	intType = memoryA[0xFFFF];
	
	//	gameboy.gfxHandler();
	while (1)
	{
		gameboy.dispatch();
		//std::cout << tempM << "\n";
		gameboy.opDecoder();
		
		//gameboy.dispatch();
		std::cin.get();
		//gameboy.RegUpdate();

	}


	return 0;
}
/*
int _tmain(int argc, _TCHAR* argv[])
{



return 0;
}
*/