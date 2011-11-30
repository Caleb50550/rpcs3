#include "stdafx.h"
#include "Emu/Cell/PPU.h"
#include "Emu/Cell/PPUDecoder.h"
#include "Emu/Cell/PPUInterpreter.h"
#include "Emu/Cell/PPUDisAsm.h"

PPUThread::PPUThread(const u8 core) : CPUThread(false, core)
{
	Reset();
}

PPUThread::~PPUThread()
{
	//~CPUThread();
}

void PPUThread::DoReset()
{
	//reset regs
	memset(FPR,  0, sizeof(FPR));
	memset(GPR,  0, sizeof(GPR));
	memset(SPRG, 0, sizeof(SPRG));
	
	CR.CR	= 0;
	LR		= 0;
	CTR		= 0;
	USPRG	= 0;
	TB		= 0;
	XER		= 0;
	FPSCR	= 0;
}

void PPUThread::_InitStack()
{
	GPR[1] = Stack.GetEndAddr();
}

u64 PPUThread::GetFreeStackSize() const
{
	return (GetStackAddr() + GetStackSize()) - GPR[1];
}

void PPUThread::DoRun()
{
	switch(Ini.m_DecoderMode.GetValue())
	{
	case 0: m_dec = new PPU_Decoder(*new PPU_DisAsm(*this)); break;
	case 1:
	case 2:
		m_dec = new PPU_Decoder(*new PPU_Interpreter(*this)); break;
	}
}

void PPUThread::DoResume()
{
}

void PPUThread::DoPause()
{
}

void PPUThread::DoStop()
{
	if(m_dec)
	{
		(*(PPU_Decoder*)m_dec).~PPU_Decoder();
		safe_delete(m_dec);
	}
}

void PPUThread::DoSysResume()
{
}

void PPUThread::DoCode(const s32 code)
{
	(*(PPU_Decoder*)m_dec).Decode(code);
}