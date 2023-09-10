#pragma once

#define bit64data unsigned long long
#define bit32data unsigned int
#define bit16data unsigned short
#define bit08data unsigned char

#define workBit bit64data
#define CharType unsigned char
#define OnceBitChars (sizeof(workBit) / sizeof(CharType))

#define GetJCQ(addr) JCQ.GetNativeJCQ(addr)

#define codelong (sizeof(char) * 64)

#define threadClass thread

#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <thread>
#include <cstdio>
#include <functional>
#include <fstream>

using namespace std;

constexpr chrono::milliseconds operator ""ms(unsigned long long ms) {
	return chrono::milliseconds(ms);
}

workBit CharToWorkBit(array<CharType,OnceBitChars> d);
array<CharType,OnceBitChars> WorkBitToChar(workBit d);

struct CpuStatusFlag {
	private:
		static const CharType Bit16Mode = 0;
		static const CharType Bit32Mode = 1;
		static const CharType Bit64Mode = 2;
		static const CharType RealMode  = 3;
		static const CharType SafeMode  = 4;
		static const CharType PageMode  = 5;
	public:
		workBit Data = 0b1001;
	public:
		bool Get(const char po[]);
		void operator[](const char po[]);
	private:
		void HelpFunc(CharType s);
		bool HelpGetFunc(CharType s);
};

struct CPUJCQs {
	private://64b
	workBit rax = 0;
	workBit rbx = 0;
	workBit rcx = 0;
	workBit rdx = 0;
	workBit rex = 0;
	workBit rfx = 0;
	workBit r7 = 0;
	workBit r8 = 0;
	workBit r9 = 0;
	workBit r10 = 0;
	workBit r11 = 0;
	workBit r12 = 0;
	workBit r13 = 0;
	workBit r14 = 0;
	workBit r15 = 0;
	workBit r16 = 0;

	//ts&ro
	workBit tr = 0;//stack root address

	workBit ts = 0;//stack shift address

	workBit ca = 0;//code address

	public:
	CpuStatusFlag flag;

	workBit* GetNativeJCQ(string name);
};

struct Memory {
	workBit* MemoryStart = nullptr;
	workBit* MemoryEnd = nullptr;
	unsigned long long MemoryLong = 1024 * 1024 * 64 / 8;//B 64MB

	Memory();
	~Memory();

	workBit* GetMemory(workBit pi);

	operator unsigned long long();
};

struct Disk {
	string DiskFolderPath = ".\\Disk0\\";

	Disk() {}
	Disk(string path) {DiskFolderPath=path;}

	void Read(int No,workBit* writBuff,int datas = 4096);
	void Write(int No,workBit* data,int datas = 4096);
};

enum CPUStatus {
	Working,//Working
	Wait,//
	IO,//IO Disk NET...
	HLT,//HLT Command
	Shutdown,//No Start
	Ukdown
};

enum CPUAuthority {
	System,
	Service,
	Admin,
	User
};

struct ChannelAtThread {
	bool HadData = false;
	int ID = 0;
	workBit Data = 0;

	bool Throw = false;
	int ThrowID = 0;
	int ThrowTo = 0;
	workBit ThrowData = 0;
};

struct Thread {
	CPUJCQs JCQ;
	CPUStatus Status = CPUStatus::Shutdown;
	CPUAuthority Authority = CPUAuthority::User;
	Memory* MemoryPoint = nullptr;
	Disk* DiskPoint = nullptr;
	ChannelAtThread channel;
	char (*CPUInfo)[7][65] = nullptr;

	threadClass runThread;

	workBit MemoryGetMax = 0;
	workBit MemoryGetMix = 0;

	workBit* LetsNumberBuffer = nullptr;
	int LetsNumberBufferLength = 512;
	int HadLets = 0;

	Thread();
	Thread(workBit code,workBit sr);
	~Thread();

	void Work();

	void Join() {
		runThread.join();
	}

	void Help(function<workBit* (workBit)> GetMemory,const char* DisplayFlag);

	string GetCode(int length=16);

	vector<workBit*> GetArgs(string con,int conlen,int args,function<workBit* (workBit)> GetMemory,vector<CharType> mid = {','},vector<CharType> end = {' ','\0','\n','\r'});

	void Init(workBit code,workBit sr);
};

struct Core {
	array<Thread,2> Data;

	operator int();
	Thread& operator[](int ID);
};

struct ThreadChannel {
	threadClass ListenThread = threadClass([this]{
		while (true) {
			bool exit = true;
			for (int i = 0;i < ThreadNumber;++i) {
				if ((Threads+i)->channel.Throw) {
					if ((Threads+i)->channel.ThrowTo == -1) {
						(Threads+i)->channel.HadData = true;
						(Threads+i)->channel.ID = (Threads+i)->channel.ThrowID;
						(Threads+i)->channel.Data = (Threads+i)->channel.ThrowData;
						(Threads+i)->channel.Throw = false;
					} else {
						int tt = (Threads+i)->channel.ThrowTo;
						(Threads+tt)->channel.HadData = true;
						(Threads+tt)->channel.ID = (Threads+i)->channel.ThrowID;
						(Threads+tt)->channel.Data = (Threads+i)->channel.ThrowData;
						(Threads+i)->channel.Throw = false;
					}
				}
				exit = exit && ((Threads+i)->Status == CPUStatus::Shutdown);
			}
			if (exit) {
				break;
			}
			this_thread::sleep_for(13ms);
		}
	});
	Thread* Threads = nullptr;
	Memory* memory = nullptr;
	int ThreadNumber = 0;

	~ThreadChannel() {
		ListenThread.join();
	}
};

template <int Cores>
struct CPU {
	array<Core,Cores> Data;
	Memory* memoryp = nullptr;
	Disk* diskp = nullptr;
	ThreadChannel threadc;
	char CPUInfo[7][65] = {
		"Copyright(c) 2023 ChunHuiStudio(r)                             \n",
		"WinterSun(r) 64Bit CPU G9-1900F @ 1.10GHz                      \n",
		"1*Core 2*Thread 1.10GHz                                        \n",
		"Instruction Set Version:ChillyWinter 1.1.1.0_3                 \n",
		"Architecture Version   :ChunHui      1.1.1.0_6                 \n",
		"Made in China/PRC                                              \n",
		"2023-09-10                                                     \n"
	};

	void Init();

	operator int();
	Thread& operator[](int ID);
};

struct PC {
	CPU<1> cpu;
	Memory memory = Memory();
	Disk disk = Disk();

	void Powar();
};

void InsertToMemory(PC* pc,workBit add,char d[64]);
void InsertToMemory(Memory* mem,workBit add,char d[64]);

bool starts_with(string d,string ser) {
	for (int i = 0;i < ser.size();++i) {
		if (d[i] != ser[i]) {
			return false;
		}
	}
	return true;
}