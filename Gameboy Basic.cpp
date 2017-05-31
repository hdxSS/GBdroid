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
		//update flags
	}
	void LOADRA16RM(unsigned short _16r) {
		Ra = memoryA[_16r];
		//update timers
		//update flags
	}
	void LOADRHLSPOFF(unsigned char _8boff) {
		FLGH(0,0,2,2);
		Rhl = (sp + _8boff);
		//update timers
		//update flags
	}
	void LOADR1616INT(unsigned short _16r, unsigned short _16int) {
		_16r = _16int;
		//update timers
		//update flags
	}
	void LOADR8HLM(unsigned char _8r) {
		_8r = hlM;
		//update timers
		//update flags
	}
	void LOADR88INT(unsigned char _8r, unsigned char _8int) {
		_8r = _8int;
		//update timers
		//update flags
	}
	void LOADR8R8(unsigned char _8r, unsigned char  _8r2) {
		_8r = _8r2;
		//update timers
		//update flags
	}
	void LOADSPHL(unsigned short SP, unsigned short HL) {
		SP = HL;
		//update timers
		//update flags
	}
	void LOADDR8HLM() {
		Ra = hlM;
		Rhl--;
		//update timers
		//update flags
	}
	void LOADDHLMR8() {
		hlM = Ra;
		Ra--;
		//update timers
		//update flags
	}
	void LOADHAOFF(unsigned char off) { //LDH

		memoryA[0xff00 + off] = Ra;
		//update timers
		//update flags
	}

	void LOADAHOFF(unsigned char off) { //LDH

		Ra = memoryA[0xff00 + off];
		//update timers
		//update flags
	}
	void LDIHLMRA() {
		hlM = Ra;
		Ra++;
		//update timers
		//update flags
	}
	void LDIRAHMM() {
		Ra = hlM;
		hlM++;
		//update timers
		//update flags
	}
	void NOP() {
	}
	void ORHLMRA() {
		FLGH(2,0,0,0);
		hlM |= Ra;
	}
	void ORRA8INT(unsigned char _8int) {
		FLGH(2,0,0,0);
		Ra |= _8int;
	}
	void ORRA8R(unsigned char _8r) {
		FLGH(2,0,0,0);
		Ra |= _8r;
	}
	void POP16R(unsigned _16r) {
		memoryA[sp] = _16r;
		sp--;
	}
	void PUSH() {
		memoryA[pc] = sp;
		sp++;
	}
	void RESBHLM(unsigned char _bit) {
		//bit de hlM a cero
	}
	void RESBR8(unsigned char _bit, unsigned  char _8r){
		//bit de _8r a cero
	}
	void RET() {}
	void RETCC(unsigned char flagcc) {}
	void RETI() {}
	void RLHM() {
		FLGH(2,0,0,2);
		}
	void RLR8(unsigned char _8r, unsigned char _8r2) {
		FLGH(2,0,0,2);
		}
	void RLA() {
		FLGH(0,0,0,2);
		}
	void RLCHM() {
		FLGH(2,0,0,2);
		}
	void RLCR8R8(unsigned char _r8, unsigned char _r82) {
		FLGH(2,0,0,2);
		}
	void RLCA() {
		FLGH(0,0,0,2);
		}
	void RRHM() {
		FLGH(2,0,0,2);
		}
	void RRR8R8(unsigned char _r8, unsigned char _r82) {
		FLGH(2,0,0,2);
		}
	void RRA() {
		FLGH(0,0,0,2);
		}
	void RRCHM() {
		FLGH(2,0,0,2);
		}
	void RRCR8R8(unsigned char _r8, unsigned char _r82) {
		FLGH(2,0,0,2);
		}
	void RRCA() {
		FLGH(0,0,0,2);
		}
	void RSTF(unsigned char _byte) {}
	void SBCHM() {
		FLGH(2,1,2,2);
		}
	void SBC8INT(unsigned char _8int) {
		FLGH(2,1,2,2);
		}
	void SBC8R(unsigned char _r8) {
		FLGH(2,1,2,2);
		}
	void SCF() {
		FLGH(3,0,0,1);
		}
	void SETBHM(unsigned char _bit) {}
	void SETBR8(unsigned char _bit, unsigned char _r8) {}
	void SLAHLM() {
		FLGH(2,0,0,2);
	}
	void SLAR8(unsigned char _r8) {
		FLGH(2,0,0,2);
	}
	void SRAHM() {
		FLGH(2,0,0,2);
		}
	void SRAR8R8(unsigned char _8r, unsigned char _r82) {
		FLGH(2,0,0,2);
		}
	void SRLHM() {
		FLGH(2,0,0,2);
		}
	void SRLR8R8(unsigned char _8r, unsigned char _r82) {
		FLGH(2,0,0,2);
		}
	void STOP() {}
	void SUBAHLM() {
		FLGH(2,1,2,2);
		}
	void SUBA8INT(unsigned char _8int) {
		FLGH(2,1,2,2);
		}
	void SUBAR8R8(unsigned char _r8, unsigned char _r82) {
		FLGH(2,1,2,2);
		}
	void SWAPHLM() {
		FLGH(2,0,0,0);
		}
	void SWAPR8R8(unsigned char _r8, unsigned char _r82) {
		FLGH(2,0,0,0);
		}
	void XORHLM() {
		FLGH(2,0,0,0);
		}
	void XOR8INT(unsigned char _8int) {
		FLGH(2,0,0,0);
		}
	void XORR8R8(unsigned char _r8, unsigned char _r82) {
		FLGH(2,0,0,0);
		}

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

		gb::MMU(pc, 3, 3);

		//	opcode = memoryA[reg.pc];
		//	std::cout << std::hex << (int)memoryA[0x101] << "prueba";

		std::cout << " " << std::hex << (int)opcode;
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
	//gameboy.Init();
	gameboy.LoadFile();
	gameboy.Reset();
	gameboy.MemAssign();
	gameboy.dispatch();
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