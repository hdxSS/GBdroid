// Gameboy Basic.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <bitset>

//Modules
//MMU - Done
//CPU -- Done + flags
//Timers -- Starting timers
//Interrupts - 0% - Next
//FlagHandling - Done // check logic cases
//Register combining - Done
//Gfx - 0%
//Sound - 0%
//Input - 0%



unsigned char  memoryT[0xFFFF];
unsigned char memoryA[0xFFFF];
char letter;
unsigned char addr;
unsigned short opcode;

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
	unsigned char sp = 0;
	unsigned char opClock = 0;
	unsigned char emulatedClock = 0;
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


	void Rcomb(unsigned char regPair) {

		switch (regPair) {
		case 0:
			Raf = (Ra << 4) | Rf;

			break;

		case 1:
			Rbc = (Rb << 4) | Rc;
			break;

		case 2:
			Rde = (Rd << 4) | Re;
			break;

		case 3:
			Rhl = (Rh << 4) | Rl;
			hlM = memoryA[Rhl];
			break;

		}
	}
#pragma endregion

#pragma region FlagCalculator
	// Flag handling

	unsigned char addTarget = 0;
	
		void CALCFLG(unsigned char mTC1, unsigned char mTC2, unsigned int opType)
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
		unsigned char carrycheck;
		unsigned char borrowcheck;
		unsigned char ANDcheck;
		unsigned char ORcheck;
		unsigned char XORcheck;
		
		carrycheck = mTC1 + mTC2;
		borrowcheck = mTC1 - mTC2;
		ANDcheck = mTC1 && mTC2;
		ORcheck = mTC1 || mTC2;
		XORcheck = mTC1 ^ mTC2;
		

//todo tengo que arreglar el overload
		switch (opType) {
		case 1: //Add
			if (C == 2) {
				if (carrycheck > 0xFF) {
					//carry flag
					Rf |= (1);
				}
				if (carrycheck > 0xF) {
					//half carry flag
					Rf |= (1 << 1);
				}
			}
			else if ( C == 3 ) {
				std::cout << "Ignore C flag";
			}
			//Do nothing
			break;
		case 2: //Sub
			if (N == 2) {
				{

					Rf |= (1 << 2);
					if (borrowcheck < 0) {
						Rf |= (1);
					}
					if (borrowcheck < 0xF) {
						Rf |= (1 << 1);
					}
				}
					
			
				break;
		case 3: // Compare
			if (Z == 2) {
				if (borrowcheck == 0) {
					//zero flag
					Rf |= (1 << 3);
				}
			}
			else if (Z == 3 ) {
				std::cout << "Ignore Z flag";
			}
		
			break;

			case 4: //optype AND
			if (ANDcheck == 0) {
				//SETS FLAG
			}
			
			break;
			
			case 5: //optype OR
				if (ORcheck == 0) {
				//SETS FLAG
			}
			break;
			
			case 6: //optype XOR
				if (XORcheck == 0) {
				//SETS FLAG
			}
			break;
			
			
			}
			
		}
	} //end flagcalcChar



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
		ANDcheck = mTS1 && mTS2;
		ORcheck = mTS1 || mTS2;
		XORcheck = mTS1 ^ mTS2;
		

//todo tengo que arreglar el overload
		switch (opType) {
		case 1: //Add
			if (C == 2) {
				if (carrycheck > 0xFF) {
					//carry flag
					Rf |= (1);
				}
				if (carrycheck > 0xF) {
					//half carry flag
					Rf |= (1 << 1);
				}
			}
			else if ( C == 3 ) {
				std::cout << "Ignore C flag";
			}
			//Do nothing
			break;
		case 2: //Sub
			if (N == 2) {
				{

					Rf |= (1 << 2);
					if (borrowcheck < 0) {
						Rf |= (1);
					}
					if (borrowcheck < 0xF) {
						Rf |= (1 << 1);
					}
				}
					
			
				break;
		case 3: // Compare
			if (Z == 2) {
				if (borrowcheck == 0) {
					//zero flag
					Rf |= (1 << 3);
				}
			}
			else if (Z == 3 ) {
				std::cout << "Ignore Z flag";
			}
		
			break;

			case 4: //optype AND
			if (ANDcheck == 0) {
				//SETS FLAG
			}
			
			break;
			
			case 5: //optype OR
				if (ORcheck == 0) {
				//SETS FLAG
			}
			break;
			
			case 6: //optype XOR
				if (XORcheck == 0) {
				//SETS FLAG
			}
			break;
			
			
			}
			
		}
	} //end flagcalcShort


	void FLGH(unsigned char Z, unsigned char N, unsigned char H, unsigned char C) {


		switch (Z) {
		case 0:
			Rf &= ~(1 << 3);
			break;

		case 1:
			Rf |= (1 << 3);
			break;

		case 2: //calc
			break;

		case 3:
			break;
		}

		switch (N) {
		case 0:
			Rf &= ~(1 << 2);
			break;

		case 1:
			Rf |= (1 << 2);
			break;

		case 2: //calc
			break;

		case 3:
			break;
		}

		switch (H) {
		case 0:
			Rf &= ~(1 << 1);
			break;

		case 1:
			Rf |= (1 << 1);
			break;

		case 2: //calc
			break;

		case 3:
			break;
		}

		switch (C) {
		case 0:
			Rf &= ~(1);
			break;

		case 1:
			Rf |= (1);
			break;

		case 2: //calc
			break;

		case 3:
			break;
		}
	}

#pragma endregion

#pragma region RegisterAndFlagsUpdate	
	// Register Combine method
	void RegComb() {
		gb::Rcomb(0);
		gb::Rcomb(1);
		gb::Rcomb(2);
		gb::Rcomb(3);

		//-Done
		std::cout << "\nAF: " << std::hex << (int)Raf << "\n";
		std::cout << "BC: " << std::hex << (int)Rbc << "\n";
		std::cout << "DE: " << std::hex << (int)Rde << "\n";
		std::cout << "HL: " << std::hex << (int)Rhl << "\n";
	}

	void FlagState() {
		std::cout << "Z: " << std::hex << (int)(Rf & (0x8 >> 3)) << "\n";
		std::cout << "N: " << std::hex << (int)(Rf & (0x4 >> 2)) << "\n";
		std::cout << "H: " << std::hex << (int)(Rf & (0x2 >> 1)) << "\n";
		std::cout << "C: " << std::hex << (int)(Rf & (0x1)) << "\n";
	}
#pragma endregion

#pragma region InstructionMapping
//probablemente ningun calculo de flag funciona porque se calcula sobre el resultado
	// Instruction mapping ----------
	void ADC8RHLM() {
		// n + carry flag to A
		addTarget = (Rf & (0x1)) + hlM;
		FLGH(2, 0, 2, 2);
		CALCFLG(Ra, addTarget, 1);
		Ra += addTarget;
		opClock = 1;
		
		
	}

	void ADCA8INT(unsigned char _8int) {
		addTarget = (Rf & (0x1)) + _8int;
		FLGH(2, 0, 2, 2);
		CALCFLG(Ra, addTarget, 1);
		Ra += addTarget;
		opClock = 2;
		
	}
	void ADC8R8(unsigned char _8r) {
		addTarget = (Rf & (0x1)) + _8r;
		FLGH(2, 0, 2, 2);
		CALCFLG(Ra, addTarget, 1);
		Ra += addTarget;
		opClock = 1;
		
	}
	void ADDAHLM() {
		FLGH(2, 0, 2, 2);
		CALCFLG(Ra, hlM, 1);
		Ra += hlM;
		opClock = 1;
		
	}
	void ADDA8INT(unsigned char _8int) {
		FLGH(2, 0, 2, 2);
		CALCFLG(Ra, _8int, 1);
		Ra += _8int;
		opClock = 2;
		
	}
	void ADDA8R(unsigned char _8r) {
		FLGH(2, 0, 2, 2);
		CALCFLG(Ra, _8r, 1);
		Ra += _8r;
		opClock = 1;
		
	}
	void ADDHL16R(unsigned short _R1) {
		FLGH(3, 0, 2, 2);
		CALCFLG(Rhl, _R1, 1);
		Rhl += _R1;
		opClock = 1;
	}
	void ADDSP8OFF(unsigned char _8off) {
		FLGH(0, 0, 2, 2);
		CALCFLG(sp,  _8off, 1);
		sp = +_8off;
		opClock = 2;
	}
	void ANDAHLM() {
		FLGH(2, 0, 1, 0);
		CALCFLG(Ra, hlM, 1);
		Ra &= hlM;
		opClock = 1;
	}
	void ANDA8INT(unsigned char _8int) {
		FLGH(2,0,1,0);
		CALCFLG(Ra, _8int, 3); //set optype3 as AND
		Ra &= _8int;
		opClock = 2;
	}
	void ANDA8R(unsigned char _8r) {
		FLGH(2,0,1,0);
		CALCFLG(Ra, _8r, 3);  //set optype3 as AND
		Ra &= _8r;
		opClock = 1;
	}
	void BITHLM(unsigned char bitnum) {

		switch (hlM << bitnum) { // refinar 0x1
		case 1:
			std::cout << "tested positive\n";

		case 0:
			std::cout << "tested negative\n";
		}
		FLGH(2,0,1,3);
		opClock = 2;
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
FLGH(2,0,1,3);
opClock = 2;
		//CALCFLG(Ra, _8int, 3) //set optype4 op de revision valor hlM o hacer directo aca
		//bit test agains
		//update timers
	
	}
	void CALLPCF16BA(int flagcc, unsigned short _16BA) {

		if (flagcc == 1)
		{
			pc = _16BA;
		}

		opClock = 3;
	}
	void CALLPC16BA(unsigned short _16BA) {
		pc = _16BA;
		// mal la dif entre jump y call
		//skip
		opClock = 3;
	}
	void CARRYFLAG() {
		FLGH(3,0,0,2);
		//CALCFLG(Ra, _8int, 3) //set optype4 op de revision valor hlM o hacer directo aca
		opClock = 1;
	}
	void COMPHLM() {
		FLGH(2,1,2,2);
		opClock = 1;
	}
	void COMP8INT(unsigned char _8int) {
		FLGH(2,1,2,2);
		opClock = 2;
	}
	void COMP8R(unsigned char _8r) {
		FLGH(2,1,2,2);
		opClock = 1;
	}
	void CPL() {
		FLGH(3,1,1,3);
		opClock = 1;
	}
	void DAA() {
		FLGH(2,3,0,2);
		opClock = 1;
	}
	void DECHLM() {
		FLGH(2,1,2,3);
		hlM--;
		opClock = 1;
	
	}
	void DECR16(unsigned short _16r) {
		_16r--;
		opClock = 1;
		//update flags
	}
	void DEC8R(unsigned char _r8) {
		FLGH(2,1,2,3);
		_r8--;
		opClock = 1;
		//update timers
		//update flags
	}
	void DI() {
		opClock = 1;
	}
	void EI() {
		opClock = 1;
	}
	void HALT() {
		opClock = 1;
	}
	void INCHLM() {
		FLGH(2,0,2,3);
		hlM++;
		opClock = 1;
		//update timers
		//update flags
	}
	void INCR16(unsigned short _16r) {
		_16r++;
		//update timers
		opClock = 1;
		//update flags
	}
	void INC8R(unsigned char _r8) {
		FLGH(2,0,2,3);
		_r8++;
		//update timers
		opClock = 1;
		//update flags
	}
	void JUMPHLM() {
		pc = hlM;

		//update timers
		opClock = 1;
		//update flags
	}
	void JUMPCC16BA(int flagcc, unsigned short _16BA) {
		if (flagcc == 1)
			pc = _16BA;
		else
			pc++;
			opClock = 3;
	}
	void JUMP16BA(unsigned short _16BA) {
		pc = _16BA;
		opClock = 3;
		//update timers
		//update flags
	}
	void JRCC8INT(int flagcc, unsigned char _8int) {
		if (flagcc == 1)
			pc = memoryA[_8int];
		else
			//skip
			pc++;
			opClock = 2;
	}
	void JR8INT(unsigned char _8int) {
		pc = memoryA[_8int];
		//skip
		opClock = 2;
	}
	void LOADCMRA() {
		Ra = memoryA[Rc];
		//update timers
		opClock = 1;
		//update flags
	}
	void LOADHLM8INT(unsigned char _8int) {
		hlM = _8int;
		//update timers
		opClock = 2;
		//update flags
	}
	void LOADHLM8R(unsigned char _8r) {
		hlM = _8r;
		//update timers
		opClock = 1;
		//update flags
	}
	void LOAD16BAMRA(unsigned short _16ba) {
		memoryA[_16ba] = Ra;
		//update timers
		opClock = 3;
		//update flags
	}
	void LOAD16BASP(unsigned short _16ba) {
		memoryA[_16ba] = sp;
		//update timers
		opClock = 3;
		//update flags
	}
	void LOAD16RMRA(unsigned short _16r) {
		memoryA[_16r] = Ra;
		//update timers
		opClock = 1;
		//update flags
	}
	void LOADRACM(unsigned char _8r) {
		Ra = memoryA[_8r];
		//update timers
		opClock = 1;
		//update flags
	}
	void LOADRA16BAM(unsigned short _16ba) {
		Ra = memoryA[_16ba];
		//update timers
		opClock = 3;
		//update flags
	}
	void LOADRA16RM(unsigned short _16r) {
		Ra = memoryA[_16r];
		//update timers
		opClock = 1;
		//update flags
	}
	void LOADRHLSPOFF(unsigned char _8boff) {
		FLGH(0,0,2,2);
		Rhl = (sp + _8boff);
		//update timers
		opClock = 2;
		//update flags
	}
	void LOADR1616INT(unsigned short _16r, unsigned short _16int) {
		_16r = _16int;
		//update timers
		opClock = 3;
		//update flags
	}
	void LOADR8HLM(unsigned char _8r) {
		_8r = hlM;
		//update timers
		opClock = 1;
		//update flags
	}
	void LOADR88INT(unsigned char _8r, unsigned char _8int) {
		_8r = _8int;
		//update timers
		opClock = 2;
		//update flags
	}
	void LOADR8R8(unsigned char _8r, unsigned char  _8r2) {
		_8r = _8r2;
		//update timers
		opClock = 1;
		//update flags
	}
	void LOADSPHL(unsigned short SP, unsigned short HL) {
		SP = HL;
		//update timers
		opClock = 1;
		//update flags
	}
	void LOADDR8HLM() {
		Ra = hlM;
		Rhl--;
		//update timers
		opClock = 1;
		//update flags
	}
	void LOADDHLMR8() {
		hlM = Ra;
		Ra--;
		//update timers
		opClock = 1;
		//update flags
	}
	void LOADHAOFF(unsigned char off) { //LDH

		memoryA[0xff00 + off] = Ra;
		//update timers
		opClock = 2;
		//update flags
	}

	void LOADAHOFF(unsigned char off) { //LDH

		Ra = memoryA[0xff00 + off];
		//update timers
		opClock = 2;
		//update flags
	}
	void LDIHLMRA() {
		hlM = Ra;
		Ra++;
		//update timers
		opClock = 1;
		//update flags
	}
	void LDIRAHMM() {
		Ra = hlM;
		hlM++;
		//update timers
		opClock = 1;
		//update flags
	}
	void NOP() {
	}
	void ORHLMRA() {
		FLGH(2,0,0,0);
		hlM |= Ra;
		opClock = 1;
	}
	void ORRA8INT(unsigned char _8int) {
		FLGH(2,0,0,0);
		Ra |= _8int;
		opClock = 2;
	}
	void ORRA8R(unsigned char _8r) {
		FLGH(2,0,0,0);
		Ra |= _8r;
		opClock = 1;
	}
	void POP16R(unsigned _16r) {
		memoryA[sp] = _16r;
		sp--;
		opClock = 1;
	}
	void PUSH() {
		memoryA[pc] = sp;
		sp++;
		opClock = 1;
	}
	void RESBHLM(unsigned char _bit) {
		//bit de hlM a cero
		opClock = 2;
	}
	void RESBR8(unsigned char _bit, unsigned  char _8r){
		//bit de _8r a cero
		opClock = 2;
	}
	void RET() {
		opClock = 1;
		}
	void RETCC(unsigned char flagcc) {
		opClock = 1;
		}
	void RETI() {
		opClock = 1;
		}
	void RLHM() {
		FLGH(2,0,0,2);
		opClock = 2;
		}
	void RLR8(unsigned char _8r, unsigned char _8r2) {
		FLGH(2,0,0,2);
		opClock = 2;
		}
	void RLA() {
		FLGH(0,0,0,2);
		opClock = 1;
		}
	void RLCHM() {
		FLGH(2,0,0,2);
		opClock = 2;
		}
	void RLCR8R8(unsigned char _r8, unsigned char _r82) {
		FLGH(2,0,0,2);
		opClock = 2;
		}
	void RLCA() {
		FLGH(0,0,0,2);
		opClock = 1;
		}
	void RRHM() {
		FLGH(2,0,0,2);
		opClock = 2;
		}
	void RRR8R8(unsigned char _r8, unsigned char _r82) {
		FLGH(2,0,0,2);
		opClock = 2;
		
		}
	void RRA() {
		FLGH(0,0,0,2);
		opClock = 1;
		}
	void RRCHM() {
		FLGH(2,0,0,2);
		opClock = 2;
		}
	void RRCR8R8(unsigned char _r8, unsigned char _r82) {
		FLGH(2,0,0,2);
		opClock = 2;
		}
	void RRCA() {
		FLGH(0,0,0,2);
		opClock = 1;
		}
	void RSTF(unsigned char _byte) {}
	void SBCHM() {
		FLGH(2,1,2,2);
		opClock = 1;
		}
	void SBC8INT(unsigned char _8int) {
		FLGH(2,1,2,2);
		opClock = 2;
		}
	void SBC8R(unsigned char _r8) {
		FLGH(2,1,2,2);
		opClock = 1;
		}
	void SCF() {
		FLGH(3,0,0,1);
		opClock = 1;
		}
	void SETBHM(unsigned char _bit) {
		opClock = 2;}
	void SETBR8(unsigned char _bit, unsigned char _r8) {
		opClock = 2;
		}
	void SLAHLM() {
		FLGH(2,0,0,2);
		opClock = 2;
	}
	void SLAR8(unsigned char _r8) {
		FLGH(2,0,0,2);
		opClock = 2;
	}
	void SRAHM() {
		FLGH(2,0,0,2);
		opClock = 2;
		}
	void SRAR8R8(unsigned char _8r, unsigned char _r82) {
		FLGH(2,0,0,2);
		opClock = 2;
		}
	void SRLHM() {
		FLGH(2,0,0,2);
		opClock = 2;
		}
	void SRLR8R8(unsigned char _8r, unsigned char _r82) {
		FLGH(2,0,0,2);
		opClock = 2;
		}
	void STOP() {
		opClock = 2;
		}
	void SUBAHLM() {
		FLGH(2,1,2,2);
		opClock = 1;
		}
	void SUBA8INT(unsigned char _8int) {
		FLGH(2,1,2,2);
		opClock = 2;
		}
	void SUBAR8R8(unsigned char _r8, unsigned char _r82) {
		FLGH(2,1,2,2);
		opClock = 1;
		}
	void SWAPHLM() {
		FLGH(2,0,0,0);
		opClock = 2;
		}
	void SWAPR8R8(unsigned char _r8, unsigned char _r82) {
		FLGH(2,0,0,0);
		opClock = 2;
		}
	void XORHLM() {
		FLGH(2,0,0,0);
		opClock = 1;
		}
	void XOR8INT(unsigned char _8int) {
		FLGH(2,0,0,0);
		opClock = 2;
		}
	void XORR8R8(unsigned char _r8, unsigned char _r82) {
		FLGH(2,0,0,0);
		opClock = 1;
		}
		
		// Opcodes 
		
		void opDecoder() {
			switch (opcode) {
case	0X00	:	//			NOP
break;				
case	0X01	:	//	bb	aa	LD	BC	$aabb		{}
break;
case	0X02	:	//			LD	(BC)	A		{}
break;
case	0X03	:	//			INC	BC			{}
break;
case	0X04	:	//			INC	B	
break;		
case	0X05	:	//			DEC	B	
break;		
case	0X06	:	//	xx		LD	B	$xx		
break;
case	0X07	:	//			RLCA		
break;		
case	0X08	:	//	bb	aa	LD	($aabb)	SP
break;		
case	0X09	:	//			ADD	HL	BC		
break;
case	0X0A	:	//			LD	A	(BC)		
case	0X0B	:	//			DEC	BC			
case	0X0C	:	//			INC	C			
case	0X0D	:	//			DEC	C			
case	0X0E	:	//	xx		LD	C	$xx		
case	0X0F	:	//			RRCA				
case	0X10	:	//			STOP				
case	0X11	:	//	bb	aa	LD	DE	$aabb		
case	0X12	:	//			LD	(DE)	A		
case	0X13	:	//			INC	DE			
case	0X14	:	//			INC	D			
case	0X15	:	//			DEC	D			
case	0X16	:	//	xx		LD	D	$xx		
case	0X17	:	//			RLA				
case	0X18	:	//	xx		JR	$xx			
case	0X19	:	//			ADD	HL	DE		
case	0X1A	:	//			LD	A	(DE)		
case	0X1B	:	//			DEC	DE			
case	0X1C	:	//			INC	E			
case	0X1D	:	//			DEC	E			
case	0X1E	:	//	xx		LD	E	$xx		
case	0X1F	:	//			RRA				
case	0X20	:	//	xx		JR	NZ	$xx		
case	0X21	:	//	bb	aa	LD	HL	$aabb		
case	0X22	:	//			LD	(HLI)	A		
case	0X23	:	//			INC	HL			
case	0X24	:	//			INC	H			
case	0X25	:	//			DEC	H			
case	0X26	:	//	xx		LD	H	$xx		
case	0X27	:	//			DAA				
case	0X28	:	//	xx		JR	Z	$xx		
case	0X29	:	//			ADD	HL	HL		
case	0X2A	:	//			LD	A	(HLI)		
case	0X2B	:	//			DEC	HL			
case	0X2C	:	//			INC	L			
case	0X2D	:	//			DEC	L			
case	0X2E	:	//	xx		LD	L	$xx		
case	0X2F	:	//			CPL				
case	0X30	:	//	xx		JR	NC	$xx		
case	0X31	:	//	bb	aa	LD	SP	$aabb		
case	0X32	:	//			LD	(HLD)	A		
case	0X33	:	//			INC	SP			
case	0X34	:	//			INC	(HL)			
case	0X35	:	//			DEC	(HL)			
case	0X36	:	//	xx		LD	(HL)	$xx		
case	0X37	:	//			SCF				
case	0X38	:	//	xx		JR	C	$xx		
case	0X39	:	//			ADD	HL	SP		
case	0X3A	:	//			LD	A	(HLD)		
case	0X3B	:	//			DEC	SP			
case	0X3C	:	//			INC	A			
case	0X3D	:	//			DEC	A			
case	0X3E	:	//	xx		LD	A	$xx		
case	0X3F	:	//			CCF				
case	0X40	:	//			LD	B	B		
case	0X41	:	//			LD	B	C		
case	0X42	:	//			LD	B	D		
case	0X43	:	//			LD	B	E		
case	0X44	:	//			LD	B	H		
case	0X45	:	//			LD	B	L		
case	0X46	:	//			LD	B	(HL)		
case	0X47	:	//			LD	B	A		
case	0X48	:	//			LD	C	B		
case	0X49	:	//			LD	C	C		
case	0X4A	:	//			LD	C	D		
case	0X4B	:	//			LD	C	E		
case	0X4C	:	//			LD	C	H		
case	0X4D	:	//			LD	C	L		
case	0X4E	:	//			LD	C	(HL)		
case	0X4F	:	//			LD	C	A		
case	0X50	:	//			LD	D	B		
case	0X51	:	//			LD	D	C		
case	0X52	:	//			LD	D	D		
case	0X53	:	//			LD	D	E		
case	0X54	:	//			LD	D	H		
case	0X55	:	//			LD	D	L		
case	0X56	:	//			LD	D	(HL)		
case	0X57	:	//			LD	D	A		
case	0X58	:	//			LD	E	B		
case	0X59	:	//			LD	E	C		
case	0X5A	:	//			LD	E	D		
case	0X5B	:	//			LD	E	E		
case	0X5C	:	//			LD	E	H		
case	0X5D	:	//			LD	E	L		
case	0X5E	:	//			LD	E	(HL)		
case	0X5F	:	//			LD	E	A		
case	0X60	:	//			LD	H	B		
case	0X61	:	//			LD	H	C		
case	0X62	:	//			LD	H	D		
case	0X63	:	//			LD	H	E		
case	0X64	:	//			LD	H	H		
case	0X65	:	//			LD	H	L		
case	0X66	:	//			LD	H	(HL)		
case	0X67	:	//			LD	H	A		
case	0X68	:	//			LD	L	B		
case	0X69	:	//			LD	L	C		
case	0X6A	:	//			LD	L	D		
case	0X6B	:	//			LD	L	E		
case	0X6C	:	//			LD	L	H		
case	0X6D	:	//			LD	L	L		
case	0X6E	:	//			LD	L	(HL)		
case	0X6F	:	//			LD	H	A		
case	0X70	:	//			LD	(HL)	B		
case	0X71	:	//			LD	(HL)	C		
case	0X72	:	//			LD	(HL)	D		
case	0X73	:	//			LD	(HL)	E		
case	0X74	:	//			LD	(HL)	H		
case	0X75	:	//			LD	(HL)	L		
case	0X76	:	//			HALT				
case	0X77	:	//			LD	(HL)	A		
case	0X78	:	//			LD	A	B		
case	0X79	:	//			LD	A	C		
case	0X7A	:	//			LD	A	D		
case	0X7B	:	//			LD	A	E		
case	0X7C	:	//			LD	A	H		
case	0X7D	:	//			LD	A	L		
case	0X7E	:	//			LD	A	(HL)		
case	0X7F	:	//			LD	A	A		
case	0X80	:	//			ADD	A	B		
case	0X81	:	//			ADD	A	C		
case	0X82	:	//			ADD	A	D		
case	0X83	:	//			ADD	A	E		
case	0X84	:	//			ADD	A	H		
case	0X85	:	//			ADD	A	L		
case	0X86	:	//			ADD	A	(HL)		
case	0X87	:	//			ADD	A	A		
case	0X88	:	//			ADC	A	B		
case	0X89	:	//			ADC	A	C		
case	0X8A	:	//			ADC	A	D		
case	0X8B	:	//			ADC	A	E		
case	0X8C	:	//			ADC	A	H		
case	0X8D	:	//			ADC	A	L		
case	0X8E	:	//			ADC	A	(HL)		
case	0X8F	:	//			ADC	A	A		
case	0X90	:	//			SUB	B			
case	0X91	:	//			SUB	C			
case	0X92	:	//			SUB	D			
case	0X93	:	//			SUB	E			
case	0X94	:	//			SUB	H			
case	0X95	:	//			SUB	L			
case	0X97	:	//			SUB	A			
case	0X98	:	//			SBC	A	B		
case	0X99	:	//			SBC	A	C		
case	0X9A	:	//			SBC	A	D		
case	0X9B	:	//			SBC	A	E		
case	0X9C	:	//			SBC	A	H		
case	0X9D	:	//			SBC	A	L		
case	0X9E	:	//			SBC	A	(HL)		
case	0X9F	:	//			SBC	A	A		
case	0XA0	:	//			AND	B			
case	0XA1	:	//			AND	C			
case	0XA2	:	//			AND	D			
case	0XA3	:	//			AND	E			
case	0XA4	:	//			AND	H			
case	0XA5	:	//			AND	L			
case	0XA6	:	//			AND	(HL)			
case	0XA7	:	//			AND	A			
case	0XA8	:	//			XOR	B			
case	0XA9	:	//			XOR	C			
case	0XAA	:	//			XOR	D			
case	0XAB	:	//			XOR	E			
case	0XAC	:	//			XOR	H			
case	0XAD	:	//			XOR	L			
case	0XAE	:	//			XOR	(HL)			
case	0XAF	:	//			XOR	A			
case	0XB0	:	//			OR	B			
case	0XB1	:	//			OR	C			
case	0XB2	:	//			OR	D			
case	0XB3	:	//			OR	E			
case	0XB4	:	//			OR	H			
case	0XB5	:	//			OR	L			
case	0XB6	:	//			OR	(HL)			
case	0XB7	:	//			OR	A			
case	0XB8	:	//			CP	B			
case	0XB9	:	//			CP	C			
case	0XBA	:	//			CP	D			
case	0XBB	:	//			CP	E			
case	0XBC	:	//			CP	H			
case	0XBD	:	//			CP	L			
case	0XBE	:	//			CP	(HL)			
case	0XBF	:	//			CP	A			
case	0XC0	:	//			RET	NZ			
case	0XC1	:	//			POP	BC			
case	0XC2	:	//	bb	aa	JP	NZ	$aabb		
case	0XC3	:	//	bb	aa	JP	$aabb			
case	0XC4	:	//	bb	aa	CALL	NZ	$aabb		
case	0XC5	:	//			PUSH	BC			
case	0XC6	:	//	xx		ADD	A	$xx		
case	0XC7	:	//			RST	$ 0			
case	0XC8	:	//			RET	Z			
case	0XC9	:	//			RET				
case	0XCA	:	//	bb	aa	JP	Z	$aabb		
						
case	0XCB00	:	//			RLC	B			
case	0XCB01	:	//			RLC	C			
case	0XCB02	:	//			RLC	D			
case	0XCB03	:	//			RLC	E			
case	0XCB04	:	//			RLC	H			
case	0XCB05	:	//			RLC	L			
case	0XCB06	:	//			RLC	(HL)			
case	0XCB07	:	//			RLC	A			
case	0XCB08	:	//			RRC	B			
case	0XCB09	:	//			RRC	C			
case	0XCB0A	:	//			RRC	D			
case	0XCB0B	:	//			RRC	E			
case	0XCB0C	:	//			RRC	H			
case	0XCB0D	:	//			RRC	L			
case	0XCB0E	:	//			RRC	(HL)			
case	0XCB0F	:	//			RRC	A			
case	0XCB10	:	//			RL	B			
case	0XCB11	:	//			RL	C			
case	0XCB12	:	//			RL	D			
case	0XCB13	:	//			RL	E			
case	0XCB14	:	//			RL	H			
case	0XCB15	:	//			RL	L			
case	0XCB16	:	//			RL	(HL)			
case	0XCB17	:	//			RL	A			
case	0XCB18	:	//			RR	B			
case	0XCB19	:	//			RR	C			
case	0XCB1A	:	//			RR	D			
case	0XCB1B	:	//			RR	E			
case	0XCB1C	:	//			RR	H			
case	0XCB1D	:	//			RR	L			
case	0XCB1E	:	//			RR	(HL)			
case	0XCB1F	:	//			RR	A			
case	0XCB20	:	//			SLA	B			
case	0XCB21	:	//			SLA	C			
case	0XCB22	:	//			SLA	D			
case	0XCB23	:	//			SLA	E			
case	0XCB24	:	//			SLA	H			
case	0XCB25	:	//			SLA	L			
case	0XCB26	:	//			SLA	(HL)			
case	0XCB27	:	//			SLA	A			
case	0XCB28	:	//			SRA	B			
case	0XCB29	:	//			SRA	C			
case	0XCB2A	:	//			SRA	D			
case	0XCB2B	:	//			SRA	E			
case	0XCB2C	:	//			SRA	H			
case	0XCB2D	:	//			SRA	L			
case	0XCB2E	:	//			SRA	(HL)			
case	0XCB2F	:	//			SRA	A			
case	0XCB37	:	//			SWAP	A			
case	0XCB38	:	//			SRL	B			
case	0XCB39	:	//			SRL	C			
case	0XCB3A	:	//			SRL	D			
case	0XCB3B	:	//			SRL	E			
case	0XCB3C	:	//			SRL	H			
case	0XCB3D	:	//			SRL	L			
case	0XCB3E	:	//			SRL	(HL)			
case	0XCB3F	:	//			SRL	A			
case	0XCB40	:	//			BIT	0	B		
case	0XCB41	:	//			BIT	0	C		
case	0XCB42	:	//			BIT	0	D		
case	0XCB43	:	//			BIT	0	E		
case	0XCB44	:	//			BIT	0	H		
case	0XCB45	:	//			BIT	0	L		
case	0XCB46	:	//			BIT	0	(HL)		
case	0XCB47	:	//			BIT	0	A		
case	0XCB48	:	//			BIT	1	B		
case	0XCB49	:	//			BIT	1	C		
case	0XCB4A	:	//			BIT	1	D		
case	0XCB4B	:	//			BIT	1	E		
case	0XCB4C	:	//			BIT	1	H		
case	0XCB4D	:	//			BIT	1	L		
case	0XCB4E	:	//			BIT	1	(HL)		
case	0XCB4F	:	//			BIT	1	A		
case	0XCB50	:	//			BIT	2	B		
case	0XCB51	:	//			BIT	2	C		
case	0XCB52	:	//			BIT	2	D		
case	0XCB53	:	//			BIT	2	E		
case	0XCB54	:	//			BIT	2	H		
case	0XCB55	:	//			BIT	2	L		
case	0XCB56	:	//			BIT	2	(HL)		
case	0XCB57	:	//			BIT	2	A		
case	0XCB58	:	//			BIT	3	B		
case	0XCB59	:	//			BIT	3	C		
case	0XCB5A	:	//			BIT	3	D		
case	0XCB5B	:	//			BIT	3	E		
case	0XCB5C	:	//			BIT	3	H		
case	0XCB5D	:	//			BIT	3	L		
case	0XCB5E	:	//			BIT	3	(HL)		
case	0XCB5F	:	//			BIT	3	A		
case	0XCB60	:	//			BIT	4	B		
case	0XCB61	:	//			BIT	4	C		
case	0XCB62	:	//			BIT	4	D		
case	0XCB63	:	//			BIT	4	E		
case	0XCB64	:	//			BIT	4	H		
case	0XCB65	:	//			BIT	4	L		
case	0XCB66	:	//			BIT	4	(HL)		
case	0XCB67	:	//			BIT	4	A		
case	0XCB68	:	//			BIT	5	B		
case	0XCB69	:	//			BIT	5	C		
case	0XCB6A	:	//			BIT	5	D		
case	0XCB6B	:	//			BIT	5	E		
case	0XCB6C	:	//			BIT	5	H		
case	0XCB6D	:	//			BIT	5	L		
case	0XCB6E	:	//			BIT	5	(HL)		
case	0XCB6F	:	//			BIT	5	A		
case	0XCB70	:	//			BIT	6	B		
case	0XCB71	:	//			BIT	6	C		
case	0XCB72	:	//			BIT	6	D		
case	0XCB73	:	//			BIT	6	E		
case	0XCB74	:	//			BIT	6	H		
case	0XCB75	:	//			BIT	6	L		
case	0XCB76	:	//			BIT	6	(HL)		
case	0XCB77	:	//			BIT	6	A		
case	0XCB78	:	//			BIT	7	B		
case	0XCB79	:	//			BIT	7	C		
case	0XCB7A	:	//			BIT	7	D		
case	0XCB7B	:	//			BIT	7	E		
case	0XCB7C	:	//			BIT	7	H		
case	0XCB7D	:	//			BIT	7	(HL)		
case	0XCB7F	:	//			BIT	7	A		
case	0XCB80	:	//			RES	0	B		
case	0XCB81	:	//			RES	0	C		
case	0XCB82	:	//			RES	0	D		
case	0XCB83	:	//			RES	0	E		
case	0XCB84	:	//			RES	0	H		
case	0XCB85	:	//			RES	0	L		
case	0XCB86	:	//			RES	0	(HL)		
case	0XCB87	:	//			RES	0	A		
case	0XCB88	:	//			RES	1	B		
case	0XCB89	:	//			RES	1	C		
case	0XCB8A	:	//			RES	1	D		
case	0XCB8B	:	//			RES	1	E		
case	0XCB8C	:	//			RES	1	H		
case	0XCB8D	:	//			RES	1	L		
case	0XCB8E	:	//			RES	1	(HL)		
case	0XCB8F	:	//			RES	1	A		
case	0XCB90	:	//			RES	2	B		
case	0XCB91	:	//			RES	2	C		
case	0XCB92	:	//			RES	2	D		
case	0XCB93	:	//			RES	2	E		
case	0XCB94	:	//			RES	2	H		
case	0XCB95	:	//			RES	2	L		
case	0XCB96	:	//			RES	2	(HL)		
case	0XCB97	:	//			RES	2	A		
case	0XCB98	:	//			RES	3	B		
case	0XCB99	:	//			RES	3	C		
case	0XCB9A	:	//			RES	3	D		
case	0XCB9B	:	//			RES	3	E		
case	0XCB9C	:	//			RES	3	H		
case	0XCB9D	:	//			RES	3	L		
case	0XCB9E	:	//			RES	3	(HL)		
case	0XCB9F	:	//			RES	3	A		
case	0XCBA0	:	//			RES	4	B		
case	0XCBA1	:	//			RES	4	C		
case	0XCBA2	:	//			RES	4	D		
case	0XCBA3	:	//			RES	4	E		
case	0XCBA4	:	//			RES	4	H		
case	0XCBA5	:	//			RES	4	L		
case	0XCBA6	:	//			RES	4	(HL)		
case	0XCBA7	:	//			RES	4	A		
case	0XCBA8	:	//			RES	5	B		
case	0XCBA9	:	//			RES	5	C		
case	0XCBAA	:	//			RES	5	D		
case	0XCBAB	:	//			RES	5	E		
case	0XCBAC	:	//			RES	5	H		
case	0XCBAD	:	//			RES	5	L		
case	0XCBAE	:	//			RES	5	(HL)		
case	0XCBAF	:	//			RES	5	A		
case	0XCBB0	:	//			RES	6	B		
case	0XCBB1	:	//			RES	6	C		
case	0XCBB2	:	//			RES	6	D		
case	0XCBB3	:	//			RES	6	E		
case	0XCBB4	:	//			RES	6	H		
case	0XCBB5	:	//			RES	6	L		
case	0XCBB6	:	//			RES	6	(HL)		
case	0XCBB7	:	//			RES	6	A		
case	0XCBB8	:	//			RES	7	B		
case	0XCBB9	:	//			RES	7	C		
case	0XCBBA	:	//			RES	7	D		
case	0XCBBB	:	//			RES	7	E		
case	0XCBBC	:	//			RES	7	H		
case	0XCBBD	:	//			RES	7	L		
case	0XCBBE	:	//			RES	7	(HL)		
case	0XCBBF	:	//			RES	7	A		
case	0XCBC0	:	//			SET	0	B		
case	0XCBC1	:	//			SET	0	C		
case	0XCBC2	:	//			SET	0	D		
case	0XCBC3	:	//			SET	0	E		
case	0XCBC4	:	//			SET	0	H		
case	0XCBC5	:	//			SET	0	L		
case	0XCBC6	:	//			SET	0	(HL)		
case	0XCBC7	:	//			SET	0	A		
case	0XCBC8	:	//			SET	1	B		
case	0XCBC9	:	//			SET	1	C		
case	0XCBCA	:	//			SET	1	D		
case	0XCBCB	:	//			SET	1	E		
case	0XCBCC	:	//			SET	1	H		
case	0XCBCD	:	//			SET	1	L		
case	0XCBCE	:	//			SET	1	(HL)		
case	0XCBCF	:	//			SET	1	A		
case	0XCBD0	:	//			SET	2	B		
case	0XCBD1	:	//			SET	2	C		
case	0XCBD2	:	//			SET	2	D		
case	0XCBD3	:	//			SET	2	E		
case	0XCBD4	:	//			SET	2	H		
case	0XCBD5	:	//			SET	2	L		
case	0XCBD6	:	//			SET	2	(HL)		
case	0XCBD7	:	//			SET	2	A		
case	0XCBD8	:	//			SET	3	B		
case	0XCBD9	:	//			SET	3	C		
case	0XCBDA	:	//			SET	3	D		
case	0XCBDB	:	//			SET	3	E		
case	0XCBDC	:	//			SET	3	H		
case	0XCBDD	:	//			SET	3	L		
case	0XCBDE	:	//			SET	3	(HL)		
case	0XCBDF	:	//			SET	3	A		
case	0XCBE0	:	//			SET	4	B		
case	0XCBE1	:	//			SET	4	C		
case	0XCBE2	:	//			SET	4	D		
case	0XCBE3	:	//			SET	4	E		
case	0XCBE4	:	//			SET	4	H		
case	0XCBE5	:	//			SET	4	L		
case	0XCBE6	:	//			SET	4	(HL)		
case	0XCBE7	:	//			SET	4	A		
case	0XCBE8	:	//			SET	5	B		
case	0XCBE9	:	//			SET	5	C		
case	0XCBEA	:	//			SET	5	D		
case	0XCBEB	:	//			SET	5	E		
case	0XCBEC	:	//			SET	5	H		
case	0XCBED	:	//			SET	5	L		
case	0XCBEE	:	//			SET	5	(HL)		
case	0XCBEF	:	//			SET	5	A		
case	0XCBF0	:	//			SET	6	B		
case	0XCBF1	:	//			SET	6	C		
case	0XCBF2	:	//			SET	6	D		
case	0XCBF3	:	//			SET	6	E		
case	0XCBF4	:	//			SET	6	H		
case	0XCBF5	:	//			SET	6	L		
case	0XCBF6	:	//			SET	6	(HL)		
case	0XCBF7	:	//			SET	6	A		
case	0XCBF8	:	//			SET	7	B		
case	0XCBF9	:	//			SET	7	C		
case	0XCBFA	:	//			SET	7	D		
case	0XCBFB	:	//			SET	7	E		
case	0XCBFC	:	//			SET	7	H		
case	0XCBFD	:	//			SET	7	L		
case	0XCBFE	:	//			SET	7	(HL)		
case	0XCBFF	:	//			SET	7	A		
				
case	0XCC	:	//	bb	aa	CALL	Z	$aabb		
case	0XCD	:	//	bb	aa	CALL	$aabb			
case	0XCE	:	//	xx		ADC	A	$xx		
case	0XCF	:	//			RST	$ 8			
case	0XD0	:	//			RET	NC			
case	0XD1	:	//			POP	DE			
case	0XD2	:	//	bb	aa	JP	NC	$aabb		
case	0XD4	:	//	bb	aa	CALL	NC	$aabb		
case	0XD5	:	//			PUSH	DE			
case	0XD6	:	//	xx		SUB	$xx			
case	0XD7	:	//			RST	$ 10			
case	0XD8	:	//			RET	C			
case	0XD9	:	//			RETI				
case	0XDA	:	//	bb	aa	JP	C	$aabb		
case	0XDC	:	//	bb	aa	CALL	C	$aabb		
case	0XDE	:	//	xx		SBC	A	$xx		
case	0XDF	:	//			RST	$ 18			
case	0XE0	:	//	xx		LD	($xx)	A		
case	0XE1	:	//			POP	HL			
case	0XE2	:	//			LD	(C)	A		
case	0XE5	:	//			PUSH	HL			
case	0XE6	:	//	xx		AND	$xx			
case	0XE7	:	//			RST	$ 20			
case	0XE8	:	//	xx		ADD	SP	xx		
case	0XE9	:	//			JP	(HL)			
case	0XEA	:	//	bb	aa	LD	($aabb)	A		
case	0XEE	:	//	xx		XOR	$xx			
case	0XEF	:	//			RST	$ 28			
case	0XF0	:	//	xx		LD	A	($xx)		
case	0XF1	:	//			POP	AF			
case	0XF2	:	//			LD	A	(C)		
case	0XF3	:	//			DI				
case	0XF5	:	//			PUSH	AF			
case	0XF6	:	//	xx		OR	$xx			
case	0XF7	:	//			RST	$ 30			
case	0XF8	:	//			LD	HL	SP		
case	0XF9	:	//			LD	SP	HL		
case	0XFA	:	//	bb	aa	LD	A	($aabb)		
case	0XFB	:	//			EI				
case	0XFE	:	//	xx		CP	$xx			
case	0XFF	:	//			RST	$ 38			

				
				
		}
		} //end opDecoder

#pragma endregion

	//flag handler
	//el register F mantiene 4 flags
	//2 sera usado como caso de error indistinto



	void Reset() {
		Ra = 0;
		Rb = 0;
		Rc = 0;
		Rd = 0;
		Re = 0;
		Rl = 0;
		Rf = 0;
		pc = 0x101;
		sp = 0;
		Rm = 0;
		Rt = 0;
	}


	void dispatch() {
		//		unsigned short opcode;
		// retrieve opcode
		//	opcode =   ( gb::mmu.rb (memoryT[reg.pc])  << 8 ) || ( gb::mmu.rb(memoryA[reg.pc + 1]));
		// assign registers
		
std::cout << "\nPC: " << std::hex << (int)pc << "\n";
		gb::MMU(pc, 1, 0);

		//	opcode = memoryA[reg.pc];
		//	std::cout << std::hex << (int)memoryA[0x101] << "prueba";

		std::cout << "\nOpcode: " << std::hex << (int)opcode;
//		std::cout << "\nOpcodeINd: " << std::hex << (int)memoryA[pc];
		Ra = 0xC;
		Rf = 0x8;
		RegComb();
		FlagState();
		/*
		unsigned char testbit = 0xf;
		unsigned char chartest = 0;
		unsigned char zt;
		unsigned char nt;
		unsigned char ht;
		unsigned char ct;

		ct = (testbit & 0x8) >> 3;
		ht = (testbit & 0x4) >> 2;
		nt = (testbit & 0x2) >> 1;
		zt = (testbit & 0x1);

		std::cout << "\ncharTest: " << std::hex << (int)testbit;
		std::cout << "\nZ FLAG: " << std::hex << (int)zt << "\n";
		std::cout << "N FLAG: " << std::hex << (int)nt << "\n";
		std::cout << "H FLAG: " << std::hex << (int)ht << "\n";
		std::cout << "C FLAG: " << std::hex << (int)ct << "\n";

		//testbit &= ~(0x2 << 1); // set 2nd bit to 1
		//clears Z N H C
		testbit &= ~(1 << 3);
		testbit &= ~(1 << 2);
		testbit &= ~(1 << 1);
		testbit &= ~(1);

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




		*/

		//supongamos que tengo stored 1100



		// modify clocks
		gb::Rm += gb::Rm;
		gb::Rt += gb::Rt;


	}


	void LoadFile()
	{
		std::ifstream fin("plot.gb", std::ios::binary);
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
			std::cout << std::hex << (int)memoryT[i];





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
	gb gameboy;
	gameboy.Reset();
	//gameboy.Init();
	gameboy.LoadFile();
	gameboy.MemAssign();
	gameboy.dispatch();
	gameboy.opDecoder();
	while (1)
	{
		std::cin.get();
		//gameboy.OpcodeDecoder();
		//	gameboy.RegUpdate();

	}


	return 0;
}
/*
int _tmain(int argc, _TCHAR* argv[])
{



	return 0;
}
*/