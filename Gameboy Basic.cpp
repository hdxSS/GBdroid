// Gameboy Basic.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <bitset>

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
	unsigned char m = 0;
	unsigned char t = 0;
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
		//opTypes
		// 1 - ADD
		// 2 - SUB
		// 3 - Comp

		// x + y > 255
		unsigned short carrycheck;
		unsigned short borrowcheck;
		carrycheck = mTC1 + mTC2;
		borrowcheck = mTC1 - mTC2;

		switch (opType) {
		case 1: //Add

			if (carrycheck > 0xFF) {
				//carry flag
				Rf |= (1);
			}
			if (carrycheck > 0xF) {
				//half carry flag
				Rf |= (1 << 1);
			}
			break;
		case 2: //Sub
			Rf |= (1 << 2);
			if (borrowcheck < 0) {
				Rf |= (1);
			}
			if (borrowcheck < 0xF) {
				Rf |= (1 << 1);
			}
			break;
		case 3: // Compare
			if (borrowcheck == 0) {
				//zero flag
				Rf |= (1 << 3);
			}
			break;
		}
	} //end flagcalcChar


	void CALCFLG(unsigned short mTS1, unsigned short mTS2, unsigned int opType)
	{
		//overload del modulo CALCFLG
		//opTypes
		// 1 - ADD
		// 2 - SUB
		// 3 - Comp

//probando sync
		// x + y > 255
		unsigned short carrycheck;
		unsigned short borrowcheck;
		carrycheck = mTS1 + mTS2;
		borrowcheck = mTS1 - mTS2;

		switch (opType) {
		case 1: //Add
			if (C != 3) {
				if (carrycheck > 0xFF) {
					//carry flag
					Rf |= (1);
				}
				if (carrycheck > 0xF) {
					//half carry flag
					Rf |= (1 << 1);
				}
			}
			//Do nothing
			break;
		case 2: //Sub
			if (N != 3) {
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
			if (Z != 3) {
				if (borrowcheck == 0) {
					//zero flag
					Rf |= (1 << 3);
				}
			}
			break;
			}
		}
	} //end flagcalcChar


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
	// Instruction mapping ----------
	void ADC8RHLM() {
		// n + carry flag to A
		addTarget = (Rf & (0x1)) + hlM;
		Ra += addTarget;
		//update timers
		FLGH(2, 0, 2, 2);
		CALCFLG(Ra, addTarget, 1);
	}

	void ADCA8INT(unsigned char _8int) {
		addTarget = (Rf & (0x1)) + _8int;
		Ra += addTarget;
		//update timers
		FLGH(2, 0, 2, 2);
		CALCFLG(Ra, addTarget, 1);
	}
	void ADC8R8(unsigned char _8r) {
		addTarget = (Rf & (0x1)) + _8r;
		Ra += addTarget;
		//update timers
		FLGH(2, 0, 2, 2);
		CALCFLG(Ra, addTarget, 1);
	}
	void ADDAHLM() {
		Ra += hlM;
		//update timers
		FLGH(2, 0, 2, 2);
		CALCFLG(Ra, hlM, 1);
	}
	void ADDA8INT(unsigned char _8int) {
		Ra += _8int;
		//update timers
		CALCFLG(Ra, _8int, 1);
	}
	void ADDA8R(unsigned char _8r) {
		Ra += _8r;
		//update timers
		FLGH(2, 0, 2, 2);
		CALCFLG(Ra, _8r, 1);
		FLGH(2, 0, 2, 2);
	}
	void ADDHL16R(unsigned short _R1) {
		Rhl += _R1;
		FLGH(2, 0, 2, 2);
		CALCFLG(Rhl, _R1, 1);

		//update timers
		//update flags
	}
	void ADDSP8OFF(unsigned char _8off) {
		sp = +_8off;
		//update timers
		//update flags
	}
	void ANDAHLM() {
		//update timers
		//update flags
	}
	void ANDA8INT(unsigned char _8int) {
		Ra &= _8int;
		//update timers
		//update flags
	}
	void ANDA8R(unsigned char _8r) {
		Ra &= _8r;
		//update timers
		//update flags
	}
	void BITHLM(unsigned char bitnum) {

		switch (hlM << bitnum) { // refinar 0x1
		case 1:
			std::cout << "tested positive\n";

		case 0:
			std::cout << "tested negative\n";
		}

		//bit test agains
		//update timers
		//update flags
	}
	void BIT8R(unsigned char bitnum, unsigned char _8r) {

		switch (_8r << bitnum) { // refinar 0x1
		case 1:
			std::cout << "tested positive\n";

		case 0:
			std::cout << "tested negative\n";
		}

		//bit test agains
		//update timers
		//update flags
	}
	void CALLPCF16BA(int flagcc, unsigned short _16BA) {

		if (flagcc == 1)
		{
			pc = _16BA;
		}

		//skip
	}
	void CALLPC16BA(unsigned short _16BA) {
		pc = _16BA;
		// mal la dif entre jump y call
		//skip
	}
	void CARRYFLAG() {
		//set carry flag
		//update timers
		//update flags
	}
	void COMPHLM() {
	}
	void COMP8INT(unsigned char _8int) {
	}
	void COMP8R(unsigned char _8r) {
	}
	void CPL() {
	}
	void DAa() {
	}
	void DECHLM() {
		hlM--;
		//update timers
		//update flags
	}
	void DECR16(unsigned short _16r) {
		_16r--;
		//update timers
		//update flags
	}
	void DEC8R(unsigned char _r8) {
		_r8--;
		//update timers
		//update flags
	}
	void DI() {
	}
	void EI() {
	}
	void HALT() {
	}
	void INCHLM() {
		hlM++;
		//update timers
		//update flags
	}
	void INCR16(unsigned short _16r) {
		_16r++;
		//update timers
		//update flags
	}
	void INC8R(unsigned char _r8) {
		_r8++;
		//update timers
		//update flags
	}
	void JUMPHLM() {
		pc = hlM;

		//update timers
		//update flags
	}
	void JUMPCC16BA(int flagcc, unsigned short _16BA) {
		if (flagcc == 1)
			pc = _16BA;
		else
			pc++;
	}
	void JUMP16BA(unsigned short _16BA) {
		pc = _16BA;
		//update timers
		//update flags
	}
	void JRCC8INT(int flagcc, unsigned char _8int) {
		if (flagcc == 1)
			pc = memoryA[_8int];
		else
			//skip
			pc++;
	}
	void JR8INT(unsigned char _8int) {
		pc = memoryA[_8int];
		//skip
	}
	void LOADCMRA() {
		Ra = memoryA[Rc];
		//update timers
		//update flags
	}
	void LOADHLM8INT(unsigned char _8int) {
		hlM = _8int;
		//update timers
		//update flags
	}
	void LOADHLM8R(unsigned char _8r) {
		hlM = _8r;
		//update timers
		//update flags
	}
	void LOAD16BAMRA(unsigned short _16ba) {
		memoryA[_16ba] = Ra;
		//update timers
		//update flags
	}
	void LOAD16BASP(unsigned short _16ba) {
		memoryA[_16ba] = sp;
		//update timers
		//update flags
	}
	void LOAD16RMRA(unsigned short _16r) {
		memoryA[_16r] = Ra;
		//update timers
		//update flags
	}
	void LOADRACM(unsigned char _8r) {
		Ra = memoryA[_8r];
		//update timers
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
		hlM |= Ra;
	}
	void ORRA8INT(unsigned char _8int) {
		Ra |= _8int;
	}
	void ORRA8R(unsigned char _8r) {
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
	void RLHM() {}
	void RLR8(unsigned char _8r, unsigned char _8r2) {}
	void RLA() {}
	void RLCHM() {}
	void RLCR8R8(unsigned char _r8, unsigned char _r82) {}
	void RLCA() {}
	void RRHM() {}
	void RRR8R8(unsigned char _r8, unsigned char _r82) {}
	void RRA() {}
	void RRCHM() {}
	void RRCR8R8(unsigned char _r8, unsigned char _r82) {}
	void RRCA() {}
	void RSTF(unsigned char _byte) {}
	void SBCHM() {}
	void SBC8INT(unsigned char _8int) {}
	void SBC8R(unsigned char _r8) {}
	void SCF() {}
	void SETBHM(unsigned char _bit) {}
	void SETBR8(unsigned char _bit, unsigned char _r8) {}
	void SRAHM() {}
	void SRAR8R8(unsigned char _8r, unsigned char _r82) {}
	void SRLHM() {}
	void SRLR8R8(unsigned char _8r, unsigned char _r82) {}
	void STOP() {}
	void SUBAHLM() {}
	void SUBA8INT(unsigned char _8int) {}
	void SUBAR8R8(unsigned char _r8, unsigned char _r82) {}
	void SWAPHLM() {}
	void SWAPR8R8(unsigned char _r8, unsigned char _r82) {}
	void XORHLM() {}
	void XOR8INT(unsigned char _8int) {}
	void XORR8R8(unsigned char _r8, unsigned char _r82) {}

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
