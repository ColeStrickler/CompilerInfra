#include <ostream>
#include "3ac.hpp"

namespace a_lang{

void IRProgram::allocGlobals(){
	for (auto& g: globals)
	{
		
	}
}

void IRProgram::datagenX64(std::ostream& out){

	out << ".data\n";
	
	
	for (auto& s: strings)
	{
		out << s.first->locString() << "\"" << s.second << "\" ";
		//Put this directive after you write out strings
		// so that everything is aligned to a quadword value
		// again
		out << ".align 8\n";
	}
	

}

void IRProgram::toX64(std::ostream& out){
	allocGlobals();
	datagenX64(out);
	// Iterate over each procedure and codegen it
	out << ".text\n";
	out << ".globl main\n";

	for (auto& proc : *procs)
		proc->toX64(out);

	printf("Done!\n");
}

void Procedure::allocLocals(){
	//Allocate space for locals
	// Iterate over each procedure and codegen it
	//TODO(Implement me)
	size_t locOffRBP = 16;
	for (auto local : locals){
		local.second->setMemoryLoc("-" + std::to_string(locOffRBP) +"(%rbp)");
		//locOffRBP += local.second->getWidth();
		locOffRBP += 8;
	}
	for (auto tmp : temps){
		tmp->setMemoryLoc("-" + std::to_string(locOffRBP) +"(%rbp)");
		locOffRBP += 8;
	}
	for (auto formal : formals){
		formal->setMemoryLoc("-" + std::to_string(locOffRBP) +"(%rbp)");
		locOffRBP += 8;
	}
	for (auto loc : addrOpds){
		loc->setMemoryLoc("-" + std::to_string(locOffRBP) +"(%rbp)");
		locOffRBP += 8;
	}
}

void Procedure::toX64(std::ostream& out){
	//Allocate all locals
	

	enter->codegenLabels(out);
	enter->codegenX64(out);
	allocLocals();


	out << "#Fn body " << myName << "\n";
	for (auto quad : *bodyQuads){
		quad->codegenLabels(out);
		out << "#" << quad->toString() << "\n";
		quad->codegenX64(out);
	}
	out << "#Fn epilogue " << myName << "\n";
	leave->codegenLabels(out);
	leave->codegenX64(out);
}

void Quad::codegenLabels(std::ostream& out){
	if (labels.empty()){ return; }

	size_t numLabels = labels.size();
	size_t labelIdx = 0;
	for ( Label * label : labels){
		out << label->getName() << ": \n";
		if (labelIdx != numLabels - 1){ out << "\n"; }
		labelIdx++;
	}
}

void BinOpQuad::codegenX64(std::ostream& out){
	TODO(Implement me binop)
}

void UnaryOpQuad::codegenX64(std::ostream& out){
	TODO(Implement me unaryop)
}

void AssignQuad::codegenX64(std::ostream& out){
	src->genLoadVal(out, A);
	dst->genStoreVal(out, A);
}

void ReadQuad::codegenX64(std::ostream& out){
	TODO(Implement me read)
}

void WriteQuad::codegenX64(std::ostream& out){
	// setup argument
	std::string loc = mySrc->getMemoryLoc();
	out << "movq " << loc << ", %rdi\n";
	if (mySrcType->isInt())
		out << "call printInt\n";
	else
		out << "call printString\n";
	
}

void GotoQuad::codegenX64(std::ostream& out){
	out << "jmp " << tgt->getName() << "\n";
}

void IfzQuad::codegenX64(std::ostream& out){
	TODO(Implement me ifz)
}

void NopQuad::codegenX64(std::ostream& out){
	out << "nop" << "\n";
}

void CallQuad::codegenX64(std::ostream& out){
	out << "call " << calleeOpd->locString() << "\n";
}

void EnterQuad::codegenX64(std::ostream& out){
	out << "pushq %rbp\n";
	out << "movq %rsp, %rbp\n";
	out << "addq $16, %rbp\n";

	std::string stackAllocSize = std::to_string(myProc->arSize());
	out << "subq $" << stackAllocSize << ", %rsp\n";
	//out << "enter " << myProc->toString() << "\n"; 
}

void LeaveQuad::codegenX64(std::ostream& out){
	
	std::string stackAllocSize = std::to_string(myProc->arSize());
	out << "addq $" << stackAllocSize << ", %rsp\n";
	out << "popq %rbp\n";
	out << "ret\n";
}

void SetArgQuad::codegenX64(std::ostream& out){
	TODO(Implement me setarg)
}

void GetArgQuad::codegenX64(std::ostream& out){
	TODO(Implement me getarg)
}

void SetRetQuad::codegenX64(std::ostream& out){
	TODO(Implement me setret)
}

void GetRetQuad::codegenX64(std::ostream& out){
	TODO(Implement me getret)
}

void LocQuad::codegenX64(std::ostream& out){
	TODO(Implement me locquad)
}

void SymOpd::genLoadVal(std::ostream& out, Register reg){
	TODO(Implement me genload)
}

void SymOpd::genStoreVal(std::ostream& out, Register reg){
	out << "movq " << RegUtils::reg64(reg) << ", " << myLoc << "\n";
}

void SymOpd::genLoadAddr(std::ostream& out, Register reg) {
	TODO(Implement me if necessary genloadaddr)
}

void AuxOpd::genLoadVal(std::ostream& out, Register reg){
	TODO(Implement me genloadval)
}

void AuxOpd::genStoreVal(std::ostream& out, Register reg){
	TODO(Implement me genstoreval)
}
void AuxOpd::genLoadAddr(std::ostream& out, Register reg){
	TODO(Implement me genloadaddr)
}


void AddrOpd::genStoreVal(std::ostream& out, Register reg){
	TODO(Implement me genstoreval)
}

void AddrOpd::genLoadVal(std::ostream& out, Register reg){
	TODO(Implement me genloadval)
}

void AddrOpd::genStoreAddr(std::ostream& out, Register reg){
	TODO(Implement me)
}

void AddrOpd::genLoadAddr(std::ostream & out, Register reg){
	TODO(Implement me)
}

void LitOpd::genLoadVal(std::ostream & out, Register reg){
	out << getMovOp() << " $" << val << ", " << getReg(reg) << "\n";
}

}
