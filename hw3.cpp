#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <fstream>
#include <sstream>
#include <bitset>
#include <regex>
#include <cctype>
#include <map>
using namespace std;

string BtH(string binary){
        if (binary == "0000"){
                return "0";
        }
        else if (binary == "0001"){
               return "1";
        }
        else if (binary == "0010"){
               return "2";
        }
        else if (binary == "0011"){
               return "3";
        }
        else if (binary == "0100"){
               return "4";
        }
        else if (binary == "0101"){
               return "5";
        }
        else if (binary == "0110"){
               return "6";
        }
        else if (binary == "0111"){
               return "7";
        }
        else if (binary == "1000"){
              return "8";
        }
        else if (binary == "1001"){
               return "9";
        }
        else if (binary == "1010"){
               return "a";
        }
        else if (binary == "1011"){
              return "b";
        }
        else if (binary == "1100"){
               return "c";
        }
        else if (binary == "1101"){
               return "d";
        }
        else if (binary == "1110"){
               return "e";
        }
        else if (binary == "1111"){
               return "f";
        }
        else {return"z";}
}

string makeH(string b32){
        string total = BtH(b32.substr(0, 4)) + BtH(b32.substr(4, 4)) + BtH(b32.substr(8, 4)) + BtH(b32.substr(12, 4)) + BtH(b32.substr(16, 4)) + BtH(b32.substr(20, 4)) + BtH(b32.substr(24, 4)) + BtH(b32.substr(28, 4));
        return total;
}

string DtB32(long long int d){
        string B = bitset<32>(d).to_string();                    
        return B;
}

long long int BtD(string bit){
	long long int D = stoll(bit, nullptr, 2);
	return D;
}

long long int HtD(string hex){
	long long int d = stoll(hex, nullptr, 16);
	return d;
}

string HtB(string hex){
	long long int d = HtD(hex);
	string b = DtB32(d);
	return b;
}

string make_n(string str){
        for (char& c : str){
                if(isalpha(c)){
                        c = tolower(c);
                }
        }
        for (int i = 0; i < str.length(); ++i){
                if (str[i] != '0'){
                       return str.substr(i);  
                 }
	}
	return "0";
}

string DtH(long long int d){
	string b = DtB32(d);
	string hex = makeH(b);
	string n = "0x" + make_n(hex);
	return n;
}

string BtH2(string bit){
	string hex = makeH(bit);
	string n = "0x"+make_n(hex);
	return n;
}



void re_add(map<long long int, string> &add_map){
	for (const auto& element : add_map) {
       		int i = element.first;
		if (i%4 == 0){
			if(add_map.find(i) == add_map.end()){
				;
			}
			else{
			string first = HtB(add_map[i]);
			string second;
			if(add_map.find(i+4) == add_map.end()){
				second = "00000000000000000000000000000000";
			}
			else{
				second = HtB(add_map[i+4]);
			}
			string new1 = first.substr(8,24) + second.substr(0,8);
			string new2 = first.substr(16,16) + second.substr(0,16);
			string new3 = first.substr(24,8) + second.substr(0,24);
			add_map[i+1] = "0x"+make_n(makeH(new1));
			add_map[i+2] = "0x"+make_n(makeH(new2));
			add_map[i+3] = "0x"+make_n(makeH(new3));
			}
		}
	}
}


void pipe_i(string inst, int &irs, int &irt, int &ird, int &iimm, int &ioff, int &ishmt, string &op, int r[32], int pc, int &hrs, int &hrt, int &npc){
	string first = inst.substr(0,6);
	string second = inst.substr(26);
	if (first == "001001"){//addiu
		string rs = inst.substr(6,5);
		string rt = inst.substr(11,5);
		string imm = inst.substr(16,16);
		string check = inst.substr(16,1);
		irs = r[BtD(rs)];
		irt = r[BtD(rt)];
		hrs = BtD(rs);
		hrt = BtD(rt);
		int im = BtD(imm);
		if(check == "1"){
			im = im - 65536;
		}
		iimm = im;
		op = "addiu";
	}
	else if (first == "001100"){//andi
		string rs = inst.substr(6,5);
		string rt = inst.substr(11,5);
		string imm = inst.substr(16,16);
		irs = r[BtD(rs)];
		irt = r[BtD(rt)];
		hrs = BtD(rs);
		hrt = BtD(rt);
		iimm = BtD(imm);
 		op = "andi";
	} 
	else if (first == "000100"){//beq
		string rs = inst.substr(6,5);
		string rt = inst.substr(11,5);
		string off = inst.substr(16,16);
		string check = inst.substr(16,1);
		int ofs = BtD(off);
		if(check == "1"){
			ofs = ofs - 65536;		
		}
		irs = r[BtD(rs)];
		irt = r[BtD(rt)];
		hrs = BtD(rs);
		hrt = BtD(rt);
		ioff = ofs;
		op = "beq";
	}
	else if (first == "000101"){//bne
		string rs = inst.substr(6,5);
		string rt = inst.substr(11,5);
		string off = inst.substr(16,16);
		string check = inst.substr(16,1);
		int ofs = BtD(off);
		if(check == "1"){
			ofs = ofs - 65536;	
		}
		irs = r[BtD(rs)];
		irt = r[BtD(rt)];
		hrs = BtD(rs);
		hrt = BtD(rt);
		ioff = ofs;
		op = "bne";
	}
	else if (first == "000010"){//j
		string target = inst.substr(6,26);
		int add = BtD(target)*4;	
        	string B = bitset<28>(add).to_string();       
		string pcadd = DtB32(pc);
		string newadd = pcadd.substr(0,4) + B;
		npc = BtD(newadd);
		op = "j";
	}
	else if (first == "000011"){//jal
		//삭제
		string target = inst.substr(6,26);
		int add = BtD(target)*4;
		//r[31] =pc +4;
		ird = 31;
        	string B = bitset<28>(add).to_string();                    
		string pcadd = DtB32(pc);
		string newadd = pcadd.substr(0,4) + B;
		npc = BtD(newadd);
		op = "jal";
	}
	else if (first == "001111"){//lui
		string rt = inst.substr(11,5);
		string imm = inst.substr(16,16);
		irt = r[BtD(rt)];
		iimm = BtD(imm);
		hrt = BtD(rt);
		op = "lui";
	}
	else if (first == "100011"){//lw
		string rs = inst.substr(6,5);
		string rt = inst.substr(11,5);
		string off = inst.substr(16,16);
		irs = r[BtD(rs)];
		irt = r[BtD(rt)];
		ioff = BtD(off);
		hrs = BtD(rs);
		hrt = BtD(rt);
		op = "lw";
	}	
	else if (first == "100000"){//lb
		string rs = inst.substr(6,5);
		string rt = inst.substr(11,5);
		string off = inst.substr(16,16);
		irs = r[BtD(rs)];
		irt = r[BtD(rt)];
		hrs = BtD(rs);
		hrt = BtD(rt);
		ioff = BtD(off);
		op = "lb";
	}
	else if (first == "001101"){//ori
		string rs = inst.substr(6,5);
		string rt = inst.substr(11,5);
		string imm = inst.substr(16,16);
		irs = r[BtD(rs)];
		irt = r[BtD(rt)];
		iimm = BtD(imm);
		hrs = BtD(rs);
		hrt = BtD(rt);
		op = "ori";
	}
	else if (first == "001011"){//sltiu
		string rs = inst.substr(6,5);
		string rt = inst.substr(11,5);
		string imm = inst.substr(16,16);
		string check = inst.substr(16,1);
		int im = BtD(imm);
		if(check == "1"){
			im = im - 65536;		
		}
		irs = r[BtD(rs)];
		irt = r[BtD(rt)];
		hrs = BtD(rs);
		hrt = BtD(rt);
		iimm = im;
		op = "sltiu";
	}
	else if (first == "101011"){//sw
		string rs = inst.substr(6,5);
		string rt = inst.substr(11,5);
		string off = inst.substr(16,16);
		irs = r[BtD(rs)];
		irt = r[BtD(rt)];
		hrs = BtD(rs);
		hrt = BtD(rt);
		ioff = BtD(off);
		op = "sw";
	}
	else if (first == "101000"){//sb
		string rs = inst.substr(6,5);
		string rt = inst.substr(11,5);
		string off = inst.substr(16,16);
		irs = r[BtD(rs)];
		irt = r[BtD(rt)];
		hrs = BtD(rs);
		hrt = BtD(rt);
		ioff = BtD(off);
		op = "sb";
	}
	else{
		if (second == "100001"){//addu
			string rs = inst.substr(6,5);
			string rt = inst.substr(11,5);
			string rd = inst.substr(16,5);
			irs = r[BtD(rs)];	
			irt = r[BtD(rt)];
			hrs = BtD(rs);
			hrt = BtD(rt);
			ird = BtD(rd);	
			op = "addu";
		}
		else if (second == "100100"){//and
			string rs = inst.substr(6,5);
			string rt = inst.substr(11,5);
			string rd = inst.substr(16,5);
			irs = r[BtD(rs)];	
			irt = r[BtD(rt)];
			hrs = BtD(rs);
			hrt = BtD(rt);
			ird = BtD(rd);	
			op = "and";
		}
		else if (second == "001000"){//jr
			string rs = inst.substr(6,5);
			irs = r[BtD(rs)];	
			hrs = BtD(rs);
			npc = irs;
			op = "jr";	
		}
		else if (second == "100111"){//nor
			string rs = inst.substr(6,5);
			string rt = inst.substr(11,5);
			string rd = inst.substr(16,5);
			irs = r[BtD(rs)];	
			irt = r[BtD(rt)];
			hrs = BtD(rs);
			hrt = BtD(rt);
			ird = BtD(rd);
			op = "nor";
		}
		else if (second == "100101"){//or
			string rs = inst.substr(6,5);
			string rt = inst.substr(11,5);
			string rd = inst.substr(16,5);
			irs = r[BtD(rs)];	
			irt = r[BtD(rt)];
			hrs = BtD(rs);
			hrt = BtD(rt);
			ird = BtD(rd);		
			op = "or";
		}
		else if (second == "101011"){//sltu
			string rs = inst.substr(6,5);
			string rt = inst.substr(11,5);
			string rd = inst.substr(16,5);
			irs = r[BtD(rs)];	
			irt = r[BtD(rt)];
			hrs = BtD(rs);
			hrt = BtD(rt);
			ird = BtD(rd);
			op = "sltu";	
		}
		else if (second == "000000"){//sll
			string rt = inst.substr(11,5);
			string rd = inst.substr(16,5);
			string shmt = inst.substr(21,5);
			ishmt = BtD(shmt);	
			irt = r[BtD(rt)];
			hrt = BtD(rt);
			ird = BtD(rd);		
			op = "sll";
		}
		else if (second == "000010"){//srl
			string rt = inst.substr(11,5);
			string rd = inst.substr(16,5);
			string shmt = inst.substr(21,5);
			ishmt = BtD(shmt);	
			irt = r[BtD(rt)];
			hrt = BtD(rt);
			ird = BtD(rd);
			op = "srl";
		}
		else if (second == "100011"){//subu
			string rt = inst.substr(11,5);
			string rd = inst.substr(16,5);
			string rs = inst.substr(6,5);
			irs = r[BtD(rs)];	
			irt = r[BtD(rt)];
			hrs = BtD(rs);
			hrt = BtD(rt);
			ird = BtD(rd);		
			op = "subu";
		}
		else{ cout << "nope: " << inst << endl;}
	}
}




void pipe_ex(string inst, int pc, int irs, int irt, int iimm, int ioff, int ishmt, int &alu, int &tar){
	string first = inst.substr(0,6);
	string second = inst.substr(26);
	if (first == "001001"){//addiu
		alu = irs + iimm; 
	}
	else if (first == "001100"){
		alu = irs & iimm;
	} 
	else if (first == "000100"){//beq
		if (irs == irt){
			tar = ioff*4 + pc + 4;
		}
		else{tar = 0;} 
	}
	else if (first == "000101"){//bne
		if (irs != irt){
			tar = ioff*4 + pc + 4;
		}
		else{tar = 0;} 
	}
	else if (first == "000010"){//j
	}
	else if (first == "000011"){//jal
		alu = pc +4;
	}
	else if (first == "001111"){//lui
		string total = DtB32(iimm).substr(16,16) + "0000000000000000";
		alu = BtD(total); 
	}
	else if (first == "100011"){//lw
		alu = irs + ioff; 
	}	
	else if (first == "100000"){//lb
		alu = irs+ioff;
	}
	else if (first == "001101"){//ori
		alu = irs | iimm;
	}
	else if (first == "001011"){//sltiu
		if(irs < iimm){
			alu = 1;
		}
		else{
			alu = 0;
		}
	}
	else if (first == "101011"){//sw
		alu = irs + ioff; 
	}
	else if (first == "101000"){//sb
		alu = irs + ioff;
	}
	else{
		if (second == "100001"){//addu
			alu = irs + irt;
		}
		else if (second == "100100"){//and
			alu = irs & irt;
		}
		else if (second == "001000"){//jr
		}
		else if (second == "100111"){//nor
			alu = ~(irs | irt);
		}
		else if (second == "100101"){//or
			alu = irs | irt; 	
		}
		else if (second == "101011"){//sltu
			if(irs < irt){
				alu = 1;
			}
			else{
				alu = 0;
			}
		}
		else if (second == "000000"){//sll
			alu = irt << ishmt; 	
		}
		else if (second == "000010"){//srl
			alu = irt >> ishmt; 	
		}
		else if (second == "100011"){//subu
			alu = irs - irt; 	
		}
		else{ cout << "nope: " << inst << endl;}
	}
}

void pipe_mem(string op, int rs, int rt, int rd, int off, int alu, map<long long int, string> &add_map, int (&r)[32], int hrs, int hrt, int &memout){
	if (op == "lw"){//lw
		if(add_map.find(alu) != add_map.end()){
			r[hrt] = HtD(add_map[alu]);
			memout = r[hrt];
		} 
	}	
	else if (op == "lb"){//lb
		if(add_map.find(alu) != add_map.end()){
			string id = DtB32(HtD(add_map[alu])).substr(0,8);
			string check = id.substr(0,1);
			int im = BtD(id);
			if(check == "1"){
				im = im - 256;		
			}
			r[hrt] = im;
			memout = im;
		}
	}
	else if (op == "sw"){//sw
		if (alu<268435456 && alu>4194304){}
		else{
			add_map[alu] = DtH(rt); 
		}
	}
	else if (op == "sb"){//sb
		if(alu<268435456 && alu > 4194304){}
		else{
		if((alu)%4 == 0){
			if(add_map.find(alu) != add_map.end()){
				add_map[alu] = DtH(BtD(DtB32(rt).substr(24,8) + DtB32(HtD(add_map[alu+1])).substr(0,24)));  
			}
			else{
				add_map[alu] = DtH(BtD(DtB32(rt).substr(24,8)+"000000000000000000000000"));
			}
		}
		else if((alu)%4 == 1){
			if(add_map.find(alu) != add_map.end()){
				 add_map[alu-1] = DtH(BtD(DtB32(HtD(add_map[alu-1])).substr(0,8)+DtB32(rt).substr(24,8)+DtB32(HtD(add_map[alu-1])).substr(16,16)));
			}
			else{
				add_map[alu-1] = DtH(BtD(DtB32(rt).substr(24,8)+"0000000000000000")); 
			}
		}
		else if((alu)%4 == 2){
                        if(add_map.find(alu) != add_map.end()){
				add_map[alu-2] = DtH(BtD(DtB32(HtD(add_map[alu-2])).substr(0,16)+DtB32(rt).substr(24,8)+DtB32(HtD(add_map[alu-2])).substr(24,8)));
                        }
                        else{
                               add_map[alu-2] = DtH(BtD(DtB32(rt).substr(24,8)+"00000000"));
                        }
                }		
		else if((alu)%4 == 3){
                        if(add_map.find(alu) != add_map.end()){
				add_map[alu-3] = DtH(BtD(DtB32(HtD(add_map[alu-3])).substr(0,24)+DtB32(rt).substr(24,8)));
			}
                        else{
                               add_map[alu-3] = DtH(BtD(DtB32(rt).substr(24,8)));
                        }
                }
		}
	}
}


void pipe_wb(string inst, int (&r)[32], int pc, int rs, int rt, int rd, int alu){
	string first = inst.substr(0,6);
	string second = inst.substr(26);
	if (first == "001001"){//addiu
		r[rt] = alu; 
	}
	else if (first == "001100"){
		r[rt] = alu;
	} 
	else if (first == "000011"){//jal
		r[rd] = alu;	
	}
	else if (first == "001111"){//lui
		r[rt] = alu; 
	}
	else if (first == "001101"){//ori
		r[rt] = alu; 	
	}
	else if (first == "001011"){//sltiu
		r[rt] = alu;
	}
	else if (first =="000000"){
		if (second == "100001"){//addu
			r[rd] = alu;
		}
		else if (second == "100100"){//and
			r[rd] = alu;
		}
		else if (second == "100111"){//nor
			r[rd] = alu; 	
		}
		else if (second == "100101"){//or
			r[rd] = alu;
		}
		else if (second == "101011"){//sltu
			r[rd] = alu;
		}
		else if (second == "000000"){//sll
			r[rd] = alu; 	
		}
		else if (second == "000010"){//srl
			r[rd] = alu; 	
		}
		else if (second == "100011"){//subu
			r[rd] = alu; 	
		}
		else{}
	}
	else{}
}


void IF(int pc, string &inst, int &npc, map<long long int, string> add_map){
	if(pc == 0){
	}
	else if(add_map.find(pc) == add_map.end()){

	}
	else{
		inst = HtB(add_map[pc]);
		npc = pc + 4;
	}
}
void ID(string inst, int &rs, int &rt, int &rd, int &imm, int &off, int &shmt, int r[32], int pc, string &op, int &jpc, int &hrs, int &hrt, int &npc){
	if(pc == 0){
	}
	else if(inst == ""){
		
	}
	else{
		pipe_i(inst, rs, rt, rd, imm, off, shmt, op, r, jpc, hrs, hrt, npc);
	}
}

void EX(string inst, int pc, int rs, int rt, int imm, int off, int shmt, int &alu, int &tar){
	if(pc == 0){
	}
	else if(inst == ""){
		
	}
	else{
		pipe_ex(inst, pc, rs,rt, imm, off, shmt, alu, tar); 
	}
}
void MEM(string op, int rs, int rt, int rd, int off, int alu, map<long long int, string> &add_map, int(&r)[32], int pc, int tar, int hrs, int hrt, int &npc, int &memout){
	if(pc == 0){
	}
	else if(add_map.find(pc) == add_map.end()){
	}
	else{
		if(op == "lw" || op == "lb" || op == "sw" || op == "sb"){
			pipe_mem(op, rs, rt, rd, off,alu,add_map,r,hrs,hrt,memout);
		}
		else if(op == "bne" || op == "beq"){
			
		}
		else{
		}
	}
}
void WB(string inst, int (&r)[32], int pc, int rs, int rt, int rd, int alu){
	if(pc == 0){
	}
	else if(inst == ""){
		
	}
	else{
		pipe_wb(inst, r, pc, rs, rt, rd, alu);
	}
}

void data_hazard(string op1, string op2, string op3, int rs, int rss, int rt, int rtt, int xrd, int xalu, int malu, int mrt, map<long long int, string> add_map, int &vrs, int &vrss, int &vrt, int &vrtt, int mrd, int &npc, int memout){
	if (op1=="lw"){// mem/wb to ex
		if(mrt == rs){
			vrs = memout;
			if(op3 == "jr"){
				npc = vrs;
			}
		}
		if(mrt == rt){
			vrt = memout;
		}
		if(op2 == "sw" || op2 == "sb"){// mem/wb to mem
			if(mrt == rtt){
				vrtt = memout;
			}
		}
		else if(op2 == "lw" || op2 == "lb"){
			if(mrt == rss){
				vrss = memout;
			}
		}
	}
	else if (op1=="lb"){// mem/wb to ex
		if(mrt == rs){
			vrs = memout;
		}
		if(mrt == rt){
			vrt = memout;
		}
		if(op2 == "sw" || op2 == "sb"){// mem/wb to mem
			if(mrt == rtt){
				vrtt = memout;
			}
		}
		else if(op2 == "lw" || op2 == "lb"){
			if(mrt == rss){
				vrss = memout;
			}
		}
	}
	else if(op1 == "addiu" || op1 == "andi" || op1 =="lui" || op1 =="ori" || op1 == "sltiu"){//rt mem/wb to ex
		if(mrt == rs){
			vrs = malu;
		}
		if(mrt == rt){
			vrt = malu;
		}
	}
	else if(op1 == "addu" || op1 == "and" || op1 == "nor" || op1 == "or" || op1 == "sltu" || op1 == "sll" || op1 == "srl" || op1 == "subu"){//rd mem/wb to ex
		if(mrd == rs){
			vrs = malu;
		}
		if(mrd == rt){
			vrt = malu;
		}
	}

	if(op2 == "addiu" || op2 == "andi" || op2 =="lui" || op2 =="ori" || op2 == "sltiu"){//rt write ex/mem to ex
		if(rtt == rs){
			vrs = xalu;
		}
		if(rtt == rt){
			vrt = xalu;
		}
	}
	else if(op2 == "addu" || op2 == "and" || op2 == "nor" || op2 == "or" || op2 == "sltu" || op2 == "sll" || op2 == "srl" || op2 == "subu"){//rd write ex/mem to ex
		if(xrd == rs){
			vrs = xalu;
		}
		if(xrd == rt){
			vrt = xalu;
		}
	}
}

int load_use(string op1, string op2, int rs, int rt, int rtt){
	if (op1 == "lw" || op1 == "lb"){
		if(op2 == "addiu" || op2 == "andi" || op2 =="lui" || op2 =="ori" || op2 == "sltiu" || op2 =="jr"){//rt not use
			if(rs == rtt){
				return 1;
			}
		}	
		else if(op2 == "addu" || op2 == "and" || op2 == "nor" || op2 == "or" || op2 == "sltu" || op2 == "sll" || op2 == "srl" || op2 == "subu" || op2 == "beq" || op2 == "bne"){ //rt use
			if(rs == rtt){
				return 1;
			}
			else if(rt == rtt){
				return 1;
			}
		}
	}
	return 0;
}

int control_jump(string op){
	if(op == "jal" || op == "j" || op == "jr"){
			return 1;
		}
		return 0;
	}

int branch_check(int tar){//ex/mem 
	if(tar != 0){
		return 1;	
	}
	return 0;
}

int main(int argc, const char* argv[]){
	int PC = 4194304;
	int pc[5] {0};

	int hrs[5] {0};		
	int hrt[5] {0};

	int rs[5] {0};
	int rt[5] {0};
	int rd[5] {0};
	int off[5] {0};
	int imm[5] {0};
	int shmt[5] {0};
	int alu[5] {0};
	int tar[5] {0};	

	string inst[5];	
	string op[5];
	
	int memout = 0;

	int R[32] {0};
	bool n = false;
	bool m = false;
	bool d = false;
	bool p = false;
	bool taken = false;
	string M;
	string m1;
	string m2;
	int num_n;
	for (int i = 0; i <argc-1; ++i){
		if(string(argv[i]) == "-n"){
			n = true;
			num_n = stoi(argv[i+1]);	
		}
		if(string(argv[i]) == "-d"){
			d = true;
		}
		if(string(argv[i]) == "-m"){
			m = true;
			M = argv[i+1];
			size_t pos = M.find(":");
			m1 = M.substr(0,pos);
			m2 = M.substr(pos+1);			
		}
		if(string(argv[i]) == "-p"){
			p = true;
		}
		if(string(argv[i]) == "-atp"){
			taken = true;
		}
		if(string(argv[i]) == "-antp"){
			taken = false;
		}

	}
	map<long long int, string> add_map;
	int j = -1;
	int num_t;
	int num_d;
	long long int t_add = 4194304;
	long long int d_add = 268435456;
	char buffer[256];
	FILE* file1 = fopen(argv[argc-1],"r");
	while (fgets(buffer, sizeof(buffer), file1) != nullptr){
		string word = buffer;
		if (j == -1){
			num_t = HtD(word)/4;
		}
		else if(j == 0){
			num_d = HtD(word)/4;
		}
		else if (j< num_t+1){
			if (word.back() == '\n') {
        			word.pop_back();
    			}
			add_map[t_add] = word;
			t_add += 4;
		}
		else{
			if (word.back() == '\n') {
        			word.pop_back();
    			}
			add_map[d_add] = word;
			d_add += 4;
		}
		j += 1;
	}
	fclose(file1);
	re_add(add_map);
	int check_n = 0;
	int cycle = 1;
	pc[0] = PC;
	while (add_map.find(pc[0]) != add_map.end() || add_map.find(pc[1]) != add_map.end() || add_map.find(pc[2]) != add_map.end() || add_map.find(pc[3]) != add_map.end() || add_map.find(pc[4]) != add_map.end()) {//기존의 mips
		if(n){
			if(check_n == num_n){break;}
		}

		//pipeline pc출력
		if(p){
		cout << "===== Cycle " << cycle <<" =====" << endl;
		cout << "Current pipeline PC state:" << endl <<"{"; 
		for (int i = 0; i<4; ++i){
			if(pc[i] == 0 || add_map.find(pc[i]) == add_map.end()){
				cout << "|";
			}
			else{
				cout << DtH(pc[i]) << "|"; 
			}
		}
		if(pc[4] == 0 || add_map.find(pc[4]) == add_map.end()){ cout << "}";}
		else{
			cout << DtH(pc[4]) << "}";
		}
		cout <<endl<<endl;
		}
		//pipeline
		IF(pc[0],inst[0], PC ,add_map);
		EX(inst[2], pc[2], rs[2], rt[2], imm[2], off[2], shmt[2], alu[2], tar[2]);
		MEM(op[3],rs[3],rt[3],rd[3],off[3],alu[3],add_map,R, pc[3], tar[3], hrs[3], hrt[3], PC, memout);
		WB(inst[4],R,pc[4],hrs[4],hrt[4],rd[4],alu[4]); //structural Hazard clear
		ID(inst[1],rs[1],rt[1],rd[1],imm[1],off[1],shmt[1],R, pc[1], op[1], pc[1], hrs[1], hrt[1], PC); //structural Hazard clear
	
		if ((pc[0] == 0 || add_map.find(pc[0]) == add_map.end()) && (pc[1] == 0 || add_map.find(pc[1]) == add_map.end()) && (pc[2] == 0 || add_map.find(pc[2]) == add_map.end()) && (pc[3] == 0 || add_map.find(pc[3]) == add_map.end())){
			break;
		}
				

		//check_n
		if(pc[4] != 0 && inst[4] != ""){
			check_n += 1;
		}		
		if(n){
			if(check_n == num_n){break;}
		}
	
		if(taken && op[3] == "bne" && !branch_check(tar[3])){//predict false
			PC = pc[3] + 4;	
			pc[0] = PC;	
			for (int i = 4; i > 2; --i) {
     				rs[i] = rs[i - 1];
       	 			rt[i] = rt[i - 1];
        			rd[i] = rd[i - 1];
        			imm[i] = imm[i - 1];
        			inst[i] = inst[i - 1];
        			off[i] = off[i - 1];
				op[i] = op[i-1];
				shmt[i] = shmt[i-1];
				alu[i] = alu[i-1];
				tar[i] = tar[i-1];
				hrt[i] = hrt[i-1];
				hrs[i] = hrs[i-1];
    			}
			pc[4] = pc[3];
			pc[3] = pc[2];

			pc[2] = 0;
			rs[2] = 0;
       	 		rt[2] = 0;
       			rd[2] = 0;
       			imm[2] = 0;
       			inst[2] = "";
       			off[2] = 0;
			op[2] = "";
			shmt[2] = 0;
			alu[2] = 0;
			tar[2] = 0;
			hrt[2] = 0;
			hrs[2] = 0;

			pc[1] = 0;
			rs[1] = 0;
       	 		rt[1] = 0;
       			rd[1] = 0;
       			imm[1] = 0;
       			inst[1] = "";
       			off[1] = 0;
			op[1] = "";
			shmt[1] = 0;
			alu[1] = 0;
			tar[1] = 0;
			hrt[1] = 0;
			hrs[1] = 0;

		}		
		else if(taken && op[3] == "beq" && !branch_check(tar[3])){//predict false
			PC = pc[3] + 4;	
			pc[0] = PC;	
			for (int i = 4; i > 2; --i) {
     				rs[i] = rs[i - 1];
       	 			rt[i] = rt[i - 1];
        			rd[i] = rd[i - 1];
        			imm[i] = imm[i - 1];
        			inst[i] = inst[i - 1];
        			off[i] = off[i - 1];
				op[i] = op[i-1];
				shmt[i] = shmt[i-1];
				alu[i] = alu[i-1];
				tar[i] = tar[i-1];
				hrt[i] = hrt[i-1];
				hrs[i] = hrs[i-1];
    			}
			pc[4] = pc[3];
			pc[3] = pc[2];

			pc[2] = 0;
			rs[2] = 0;
       	 		rt[2] = 0;
       			rd[2] = 0;
       			imm[2] = 0;
       			inst[2] = "";
       			off[2] = 0;
			op[2] = "";
			shmt[2] = 0;
			alu[2] = 0;
			tar[2] = 0;
			hrt[2] = 0;
			hrs[2] = 0;

			pc[1] = 0;
			rs[1] = 0;
       	 		rt[1] = 0;
       			rd[1] = 0;
       			imm[1] = 0;
       			inst[1] = "";
       			off[1] = 0;
			op[1] = "";
			shmt[1] = 0;
			alu[1] = 0;
			tar[1] = 0;
			hrt[1] = 0;
			hrs[1] = 0;

		}

		else if(!taken && branch_check(tar[3])){//predict false 2
			PC = tar[3];
			pc[0] = PC;	
			for (int i = 4; i > 3; --i) {
     				rs[i] = rs[i - 1];
       	 			rt[i] = rt[i - 1];
        			rd[i] = rd[i - 1];
        			imm[i] = imm[i - 1];
        			inst[i] = inst[i - 1];
        			off[i] = off[i - 1];
				op[i] = op[i-1];
				shmt[i] = shmt[i-1];
				alu[i] = alu[i-1];
				tar[i] = tar[i-1];
				hrt[i] = hrt[i-1];
				hrs[i] = hrs[i-1];
    			}
			pc[4] = pc[3];

			pc[3] = 0;
			rs[3] = 0;
       	 		rt[3] = 0;
       			rd[3] = 0;
       			imm[3] = 0;
       			inst[3] = "";
       			off[3] = 0;
			op[3] = "";
			shmt[3] = 0;
			alu[3] = 0;
			tar[3] = 0;
			hrt[3] = 0;
			hrs[3] = 0;

			pc[2] = 0;
			rs[2] = 0;
       	 		rt[2] = 0;
       			rd[2] = 0;
       			imm[2] = 0;
       			inst[2] = "";
       			off[2] = 0;
			op[2] = "";
			shmt[2] = 0;
			alu[2] = 0;
			tar[2] = 0;
			hrt[2] = 0;
			hrs[2] = 0;

			pc[1] = 0;
			rs[1] = 0;
       	 		rt[1] = 0;
       			rd[1] = 0;
       			imm[1] = 0;
       			inst[1] = "";
       			off[1] = 0;
			op[1] = "";
			shmt[1] = 0;
			alu[1] = 0;
			tar[1] = 0;
			hrt[1] = 0;
			hrs[1] = 0;
		}
	
		else{
		//load-use detect
		if(load_use(op[2], op[1], hrs[1], hrt[1], hrt[2])){
			for (int i = 4; i > 2; --i) {
     				rs[i] = rs[i - 1];
       	 			rt[i] = rt[i - 1];
        			rd[i] = rd[i - 1];
        			imm[i] = imm[i - 1];
        			inst[i] = inst[i - 1];
        			off[i] = off[i - 1];
				op[i] = op[i-1];
				shmt[i] = shmt[i-1];
				alu[i] = alu[i-1];
				tar[i] = tar[i-1];
				hrt[i] = hrt[i-1];
				hrs[i] = hrs[i-1];
    			}
			pc[4] = pc[3];
			pc[3] = pc[2];

			pc[2] = 0;
			rs[2] = 0;
       	 		rt[2] = 0;
       			rd[2] = 0;
       			imm[2] = 0;
       			inst[2] = "";
       			off[2] = 0;
			op[2] = "";
			shmt[2] = 0;
			alu[2] = 0;
			tar[2] = 0;
			hrt[2] = 0;
			hrs[2] = 0;
		
		}
		else{
		// hazard detect
		data_hazard(op[3], op[2], op[1], hrs[1], hrs[2], hrt[1], hrt[2], rd[2], alu[2], alu[3], hrt[3], add_map, rs[1], rs[2], rt[1], rt[2], rd[3], PC, memout);

		// control hazard jump
		if (control_jump(op[1])){
			for (int i = 4; i > 1; --i) {
     				rs[i] = rs[i - 1];
       	 			rt[i] = rt[i - 1];
        			rd[i] = rd[i - 1];
        			imm[i] = imm[i - 1];
        			inst[i] = inst[i - 1];
        			off[i] = off[i - 1];
				op[i] = op[i-1];
				shmt[i] = shmt[i-1];
				alu[i] = alu[i-1];
				tar[i] = tar[i-1];
				hrt[i] = hrt[i-1];
				hrs[i] = hrs[i-1];
    			}
			pc[4] = pc[3];
			pc[3] = pc[2];
			pc[2] = pc[1];
			pc[0] = PC;
		
			pc[1] = 0;
			rs[1] = 0;
       	 		rt[1] = 0;
       			rd[1] = 0;
       			imm[1] = 0;
       			inst[1] = "";
       			off[1] = 0;
			op[1] = "";
			shmt[1] = 0;
			alu[1] = 0;
			tar[1] = 0;
			hrt[1] = 0;
			hrs[1] = 0;
		}
		else if(taken && op[1] == "beq"){ // atp
			PC = off[1]*4 + pc[1] + 4;
			for (int i = 4; i > 1; --i) {
     				rs[i] = rs[i - 1];
       	 			rt[i] = rt[i - 1];
        			rd[i] = rd[i - 1];
        			imm[i] = imm[i - 1];
        			inst[i] = inst[i - 1];
        			off[i] = off[i - 1];
				op[i] = op[i-1];
				shmt[i] = shmt[i-1];
				alu[i] = alu[i-1];
				tar[i] = tar[i-1];
				hrt[i] = hrt[i-1];
				hrs[i] = hrs[i-1];
    			}
			pc[4] = pc[3];
			pc[3] = pc[2];
			pc[2] = pc[1];
			pc[0] = PC;
		
			pc[1] = 0;
			rs[1] = 0;
       	 		rt[1] = 0;
       			rd[1] = 0;
       			imm[1] = 0;
       			inst[1] = "";
       			off[1] = 0;
			op[1] = "";
			shmt[1] = 0;
			alu[1] = 0;
			tar[1] = 0;
			hrt[1] = 0;
			hrs[1] = 0;
	
		}
		else if(taken && op[1] =="bne"){ // atp
			PC = off[1]*4 + pc[1] + 4;
			for (int i = 4; i > 1; --i) {
     				rs[i] = rs[i - 1];
       	 			rt[i] = rt[i - 1];
        			rd[i] = rd[i - 1];
        			imm[i] = imm[i - 1];
        			inst[i] = inst[i - 1];
        			off[i] = off[i - 1];
				op[i] = op[i-1];
				shmt[i] = shmt[i-1];
				alu[i] = alu[i-1];
				tar[i] = tar[i-1];
				hrt[i] = hrt[i-1];
				hrs[i] = hrs[i-1];
    			}
			pc[4] = pc[3];
			pc[3] = pc[2];
			pc[2] = pc[1];
			pc[0] = PC;
		
			pc[1] = 0;
			rs[1] = 0;
       	 		rt[1] = 0;
       			rd[1] = 0;
       			imm[1] = 0;
       			inst[1] = "";
       			off[1] = 0;
			op[1] = "";
			shmt[1] = 0;
			alu[1] = 0;
			tar[1] = 0;
			hrt[1] = 0;
			hrs[1] = 0;

		}
	
		else{
		// data fetch
		for (int i = 4; i > 0; --i) {
     			rs[i] = rs[i - 1];
       	 		rt[i] = rt[i - 1];
        		rd[i] = rd[i - 1];
        		imm[i] = imm[i - 1];
        		inst[i] = inst[i - 1];
        		off[i] = off[i - 1];
			op[i] = op[i-1];
			shmt[i] = shmt[i-1];
			alu[i] = alu[i-1];
			tar[i] = tar[i-1];
			hrt[i] = hrt[i-1];
			hrs[i] = hrs[i-1];
    		}
		pc[4] = pc[3];
		pc[3] = pc[2];
		pc[2] = pc[1];
		pc[1] = pc[0];
		pc[0] = PC;

		}
		}
		}
		cycle += 1;
		re_add(add_map);
		
		//출력형태 
		if (d){
			cout << "Current register values:" <<  endl;
			cout << "--------------------------" << endl;
			cout << "PC: " <<DtH(PC) << endl;
			cout << "Registers:" << endl;
			for(int i = 0; i <32; ++i){
				cout << "R" <<i <<": " <<DtH(R[i]) << endl; 
			}
			cout << endl;
		/*	if(m){
				cout << "Memory content [" << m1 << ".." << m2 <<"]:" <<endl;
				cout <<"------------------------------------------------------" <<endl;
				int m1i = HtD(m1);
				int m2i = HtD(m2);
				if(m1i == m2i){
					if (add_map.find(m1i) == add_map.end()){
						cout << DtH(m1i) << ": " << "0x0" << endl;
					}
					else{
						cout << DtH(m1i) << ": " << add_map[m1i] << endl;
					}
				}
				else{
				for(m1i; m1i <= m2i; m1i+=4){
					if(m2i-m1i>0 && m2i-m1i<4){
						if (add_map.find(m1i) == add_map.end()){
							cout << DtH(m1i) << ": " << "0x0" << endl;
						}
						else{
							int num = m2i-m1i+1;
							string new1 = DtB32(HtD(add_map[m1i])).substr(0,num*8);
							for (int i = 0; i < (4-num)*8; ++i){
								new1 += "0";
							}
							string plz = makeH(new1);
							for (int i = 0; i < (4-num)*2; ++i){
								plz.pop_back();
							}	
							cout << DtH(m1i) << ": " << "0x"+make_n(plz) << endl;
						}
					}
					else{
						if (add_map.find(m1i) == add_map.end()){
							cout << DtH(m1i) << ": " << "0x0" << endl;
						}
						else{
							cout << DtH(m1i) << ": " << add_map[m1i] << endl;
						}
					}
				}
				}
				cout << endl;
			} */
		}
	}
	if (1){
		cout << "===== Completion cycle: " << cycle <<" =====" << endl;
		cout << "Current pipeline PC state:" << endl <<"{"; 
		for (int i = 0; i<4; ++i){
			if(pc[i] == 0 || add_map.find(pc[i]) == add_map.end()){
				cout << "|";
			}
			else{
				cout << DtH(pc[i]) << "|"; 
			}
		}
		if(pc[4] == 0 || add_map.find(pc[4]) == add_map.end()){ cout << "}";}
		else{
			cout << DtH(pc[4]) << "}";
		}
		cout <<endl<<endl;
		


		cout << "Current register values:" <<  endl;
		cout << "--------------------------" << endl;
		cout << "PC: " <<DtH(PC) << endl;
		cout << "Registers:" << endl;
		for(int i = 0; i <32; ++i){
			cout << "R" <<i <<": " <<DtH(R[i]) << endl; 
		}
		cout << endl;
		if(m){
			cout << "Memory content [" << m1 << ".." << m2 <<"]:" <<endl;
			cout <<"------------------------------------------------------" <<endl;
			int m1i = HtD(m1);
			int m2i = HtD(m2);
			if(m1i == m2i){
				if (add_map.find(m1i) == add_map.end()){
						cout << DtH(m1i) << ": " << "0x0" << endl;
					}
				else{
					cout << DtH(m1i) << ": " << add_map[m1i] << endl;
				}
			}
			else{
			for(m1i; m1i <= m2i; m1i+=4){
				if(m2i-m1i>0 && m2i-m1i<4){
						if (add_map.find(m1i) == add_map.end()){
							cout << DtH(m1i) << ": " << "0x0" << endl;
						}
						else{
							int num = m2i-m1i+1;
							string new1 = DtB32(HtD(add_map[m1i])).substr(0,num*8);
							for (int i = 0; i < (4-num)*8; ++i){
								new1 += "0";
							}
							string plz = makeH(new1);
							for (int i = 0; i < (4-num)*2; ++i){
								plz.pop_back();
							}	
							cout << DtH(m1i) << ": " << "0x"+make_n(plz) << endl;
						}
					}
				else{
					if (add_map.find(m1i) == add_map.end()){
					cout << DtH(m1i) << ": " << "0x0" << endl;
					}
					else{
					cout << DtH(m1i) << ": " << add_map[m1i]<<endl;
					}
				}
			}
			}
			cout << endl;
		}
	}
	//for (const auto& element : add_map) {
        //	std::cout << dth(element.first) << ": " << element.second << std::endl;
    	//}
	return 0;
}
