#pragma once

#include "Gui/DisAsmFrame.h"
#include "Emu/Memory/Memory.h"

enum DisAsmModes
{
	DumpMode,
	InterpreterMode,
	NormalMode,
};

class DisAsm
{
protected:
	DisAsmFrame* disasm_frame;
	const DisAsmModes m_mode;

	virtual void Write(const wxString value)
	{
		switch(m_mode)
		{
			case DumpMode:
			{
				wxString mem = wxString::Format("%08x		", dump_pc);
				for(u8 i=0; i < 4; ++i)
				{
					mem += wxString::Format("%02x", Memory.Read8(dump_pc + i));
					if(i < 3) mem += " ";
				}

				last_opcode = mem + ": " + value + "\n";
			}
			break;

			case InterpreterMode:
			{
				wxString mem = wxString::Format("[%08x]  ", dump_pc);
				for(u8 i=0; i < 4; ++i)
				{
					mem += wxString::Format("%02x", Memory.Read8(dump_pc + i));
					if(i < 3) mem += " ";
				}

				last_opcode = mem + ": " + value;
			}
			break;

			default: disasm_frame->AddLine(value); break;
		};
	}

public:
	wxString last_opcode;
	uint dump_pc;

protected:
	DisAsm(CPUThread& cpu, DisAsmModes mode = NormalMode) : m_mode(mode)
	{
		if(m_mode != NormalMode) return;

		disasm_frame = new DisAsmFrame(cpu);
		disasm_frame->Show();
	}

	virtual u32 DisAsmBranchTarget(const s32 imm)=0;

	void DisAsm_F4_RC(const wxString& op, OP_REG f0, OP_REG f1, OP_REG f2, OP_REG f3, bool rc)
	{
		Write(wxString::Format("%s%s f%d,f%d,f%d,f%d", op, rc ? "." : "", f0, f1, f2, f3));
	}
	void DisAsm_F3_RC(const wxString& op, OP_REG f0, OP_REG f1, OP_REG f2, bool rc)
	{
		Write(wxString::Format("%s%s f%d,f%d,f%d", op, rc ? "." : "", f0, f1, f2));
	}
	void DisAsm_F3(const wxString& op, OP_REG f0, OP_REG f1, OP_REG f2)
	{
		DisAsm_F3_RC(op, f0, f1, f2, false);
	}
	void DisAsm_F2_RC(const wxString& op, OP_REG f0, OP_REG f1, bool rc)
	{
		Write(wxString::Format("%s%s f%d,f%d", op, rc ? "." : "", f0, f1));
	}
	void DisAsm_F2(const wxString& op, OP_REG f0, OP_REG f1)
	{
		DisAsm_F2_RC(op, f0, f1, false);
	}
	void DisAsm_F1_RC(const wxString& op, OP_REG f0, bool rc)
	{
		Write(wxString::Format("%s%s f%d", op, rc ? "." : "", f0));
	}
	void DisAsm_R1(const wxString& op, OP_REG r0)
	{
		Write(wxString::Format("%s r%d", op, r0));
	}
	void DisAsm_R2_OE_RC(const wxString& op, OP_REG r0, OP_REG r1, OP_REG oe, bool rc)
	{
		Write(wxString::Format("%s%s%s r%d,r%d", op, oe ? "o" : "", rc ? "." : "", r0, r1));
	}
	void DisAsm_R2_RC(const wxString& op, OP_REG r0, OP_REG r1, bool rc)
	{
		DisAsm_R2_OE_RC(op, r0, r1, false, rc);
	}
	void DisAsm_R2(const wxString& op, OP_REG r0, OP_REG r1)
	{
		DisAsm_R2_RC(op, r0, r1, false);
	}
	void DisAsm_R3_OE_RC(const wxString& op, OP_REG r0, OP_REG r1, OP_REG r2, OP_REG oe, bool rc)
	{
		Write(wxString::Format("%s%s%s r%d,r%d,r%d", op, oe ? "o" : "", rc ? "." : "", r0, r1, r2));
	}
	void DisAsm_R3_RC(const wxString& op, OP_REG r0, OP_REG r1, OP_REG r2, bool rc)
	{
		DisAsm_R3_OE_RC(op, r0, r1, r2, false, rc);
	}
	void DisAsm_R3(const wxString& op, OP_REG r0, OP_REG r1, OP_REG r2)
	{
		DisAsm_R3_RC(op, r0, r1, r2, false);
	}
	void DisAsm_R2_INT3_RC(const wxString& op, OP_REG r0, OP_REG r1, OP_sIMM i0, OP_sIMM i1, OP_sIMM i2, bool rc)
	{
		Write(wxString::Format("%s%s r%d,r%d,%d,%d,%d", op, rc ? "." : "", r0, r1, i0, i1, i2));
	}
	void DisAsm_R2_INT3(const wxString& op, OP_REG r0, OP_REG r1, OP_sIMM i0, OP_sIMM i1, OP_sIMM i2)
	{
		DisAsm_R2_INT3_RC(op, r0, r1, i0, i1, i2, false);
	}
	void DisAsm_R2_INT2_RC(const wxString& op, OP_REG r0, OP_REG r1, OP_sIMM i0, OP_sIMM i1, bool rc)
	{
		Write(wxString::Format("%s%s r%d,r%d,%d,%d", op, rc ? "." : "", r0, r1, i0, i1));
	}
	void DisAsm_R2_INT2(const wxString& op, OP_REG r0, OP_REG r1, OP_sIMM i0, OP_sIMM i1)
	{
		DisAsm_R2_INT2_RC(op, r0, r1, i0, i1, false);
	}
	void DisAsm_R2_IMM(const wxString& op, OP_REG r0, OP_REG r1, OP_sIMM imm0)
	{
		Write(wxString::Format("%s r%d,r%d,%d  #%x", op, r0, r1, imm0, imm0));
	}
	void DisAsm_R1_IMM(const wxString& op, OP_REG r0, OP_sIMM imm0)
	{
		Write(wxString::Format("%s r%d,%d  #%x", op, r0, imm0, imm0));
	}
	void DisAsm_CR_R_IMM(const wxString& op, OP_REG cr0, OP_REG r0, OP_sIMM imm0)
	{
		Write(wxString::Format("%s cr%d,r%d,%d  #%x", op, cr0, r0, imm0, imm0));
	}
	void DisAsm_CR1_R2_RC(const wxString& op, OP_REG cr0, OP_REG r0, OP_REG r1, bool rc)
	{
		Write(wxString::Format("%s%s cr%d,r%d,r%d", op, rc ? "." : "", cr0, r0, r1 ));
	}
	void DisAsm_CR1_R2(const wxString& op, OP_REG cr0, OP_REG r0, OP_REG r1)
	{
		DisAsm_CR1_R2_RC(op, cr0, r0, r1, false);
	}
	void DisAsm_INT3(const wxString& op, const int i0, const int i1, const int i2)
	{
		Write(wxString::Format("%s %d,%d,%d", op, i0, i1, i2 ));
	}
	void DisAsm_BRANCH(const wxString& op, const int pc)
	{
		Write(wxString::Format("%s 0x%x", op, DisAsmBranchTarget(pc)));
	}
	void DisAsm_B2_BRANCH(const wxString& op, OP_REG b0, OP_REG b1, const int pc)
	{
		Write(wxString::Format("%s %d,%d,0x%x ", op, b0, b1, DisAsmBranchTarget(pc)));
	}
	void DisAsm_CR_BRANCH(const wxString& op, OP_REG cr, const int pc)
	{
		Write(wxString::Format("%s cr%d,0x%x ", op, cr/4, DisAsmBranchTarget(pc)));
	}
};