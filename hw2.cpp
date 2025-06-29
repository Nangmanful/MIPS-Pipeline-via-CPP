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
	string n = "0x"+make_n(hex);
	return n;
}

void check_i(string inst, int (&r)[32], int &pc, map<long long int, string> &add_map){
	string first = inst.substr(0,6);
	string second = inst.substr(26);
	if (first == "001001"){//addiu
		string rs = inst.substr(6,5);
		string rt = inst.substr(11,5);
		string imm = inst.substr(16,16);
		string check = inst.substr(16,1);
		int im = BtD(imm);
		if(check == "1"){
			im = im - 65536;		
		}
		r[BtD(rt)] = r[BtD(rs)] + im; 
		pc += 4;
	}
	else if (first == "001100"){
		string rs = inst.substr(6,5);
		string rt = inst.substr(11,5);
		string imm = inst.substr(16,16);
		r[BtD(rt)] = r[BtD(rs)] & BtD(imm);
		pc += 4; 
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
		if (r[BtD(rs)] == r[BtD(rt)]){
			pc = ofs*4 + pc + 4;
		}
		else{pc += 4;} 
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
		if (r[BtD(rs)] != r[BtD(rt)]){
			pc = ofs*4 + pc + 4;
		}
		else{pc += 4;} 
	}
	else if (first == "000010"){//j
		string target = inst.substr(6,26);
		int add = BtD(target)*4;	
        	string B = bitset<28>(add).to_string();       
		string pcadd = DtB32(pc);
		string newadd = pcadd.substr(0,4) + B;
		pc = BtD(newadd);
	}
	else if (first == "000011"){//jal
		string target = inst.substr(6,26);
		int add = BtD(target)*4;
		r[31] = pc+4;	
        	string B = bitset<28>(add).to_string();                    
		string pcadd = DtB32(pc);
		string newadd = pcadd.substr(0,4) + B;
		pc = BtD(newadd);
	}
	else if (first == "001111"){//lui
		string rt = inst.substr(11,5);
		string imm = inst.substr(16,16);
		string total = imm + "0000000000000000";
		r[BtD(rt)] = BtD(total); 
		pc += 4;
	}
	else if (first == "100011"){//lw
		string rs = inst.substr(6,5);
		string rt = inst.substr(11,5);
		string off = inst.substr(16,16);
		r[BtD(rt)] = HtD(add_map[r[BtD(rs)] + BtD(off)]); 
		pc += 4;
	}	
	else if (first == "100000"){//lb
		string rs = inst.substr(6,5);
		string rt = inst.substr(11,5);
		string imm = inst.substr(16,16);
		//r[BtD(rt)] = BtD(DtB32(HtD(add_map[r[BtD(rs)]+BtD(imm)])).substr(0,8));
		string id = DtB32(HtD(add_map[r[BtD(rs)]+BtD(imm)])).substr(0,8);
		string check = id.substr(0,1);
		int im = BtD(id);
		if(check == "1"){
			im = im - 256;		
		}
		r[BtD(rt)] = im;
		pc += 4;
	}
	else if (first == "001101"){//ori
		string rs = inst.substr(6,5);
		string rt = inst.substr(11,5);
		string imm = inst.substr(16,16);
		r[BtD(rt)] = r[BtD(rs)] | BtD(imm);
		pc += 4; 	
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
		if(r[BtD(rs)] < im){
			r[BtD(rt)] = 1;
		}
		else{
			r[BtD(rt)] = 0;
		}
		pc += 4; 
	}
	else if (first == "101011"){//sw
		string rs = inst.substr(6,5);
		string rt = inst.substr(11,5);
		string imm = inst.substr(16,16);
		add_map[r[BtD(rs)] + BtD(imm)] = DtH(r[BtD(rt)]); 
		pc += 4;

	}
	else if (first == "101000"){//sb
		string rs = inst.substr(6,5);
		string rt = inst.substr(11,5);
		string imm = inst.substr(16,16);
		if((r[BtD(rs)] + BtD(imm))%4 == 0){
			if(add_map.find(r[BtD(rs)] + BtD(imm)) != add_map.end()){
				add_map[r[BtD(rs)] + BtD(imm)] = DtH(BtD(DtB32(r[BtD(rt)]).substr(24,8) + DtB32(HtD(add_map[r[BtD(rs)] + BtD(imm)+1])).substr(0,24)));  
			}
			else{
				add_map[r[BtD(rs)] + BtD(imm)] = DtH(BtD(DtB32(r[BtD(rt)]).substr(24,8)+"000000000000000000000000"));
			}
		}
		else if((r[BtD(rs)] + BtD(imm))%4 == 1){
			if(add_map.find(r[BtD(rs)] + BtD(imm)) != add_map.end()){
				 add_map[r[BtD(rs)] + BtD(imm)-1] = DtH(BtD(DtB32(HtD(add_map[r[BtD(rs)] + BtD(imm)-1])).substr(0,8)+DtB32(r[BtD(rt)]).substr(24,8)+DtB32(HtD(add_map[r[BtD(rs)] + BtD(imm)-1])).substr(16,16)));
			}
			else{
				add_map[r[BtD(rs)] + BtD(imm)-1] = DtH(BtD(DtB32(r[BtD(rt)]).substr(24,8)+"0000000000000000")); 
			}
		}
		else if((r[BtD(rs)] + BtD(imm))%4 == 2){
                        if(add_map.find(r[BtD(rs)] + BtD(imm)) != add_map.end()){
				add_map[r[BtD(rs)] + BtD(imm)-2] = DtH(BtD(DtB32(HtD(add_map[r[BtD(rs)] + BtD(imm)-2])).substr(0,16)+DtB32(r[BtD(rt)]).substr(24,8)+DtB32(HtD(add_map[r[BtD(rs)] + BtD(imm)-2])).substr(24,8)));
                        }
                        else{
                               add_map[r[BtD(rs)] + BtD(imm)-2] = DtH(BtD(DtB32(r[BtD(rt)]).substr(24,8)+"00000000"));
                        }
                }		
		else if((r[BtD(rs)] + BtD(imm))%4 == 3){
                        if(add_map.find(r[BtD(rs)] + BtD(imm)) != add_map.end()){
				add_map[r[BtD(rs)] + BtD(imm)-3] = DtH(BtD(DtB32(HtD(add_map[r[BtD(rs)] + BtD(imm)-3])).substr(0,24)+DtB32(r[BtD(rt)]).substr(24,8)));
			}
                        else{
                               add_map[r[BtD(rs)] + BtD(imm)-3] = DtH(BtD(DtB32(r[BtD(rt)]).substr(24,8)));
                        }
                }
		pc += 4;

	}
	else{
		if (second == "100001"){//addu
			string rs = inst.substr(6,5);
			string rt = inst.substr(11,5);
			string rd = inst.substr(16,5);
			r[BtD(rd)] = r[BtD(rs)] + r[BtD(rt)];
			pc += 4;
		}
		else if (second == "100100"){//and
			string rs = inst.substr(6,5);
			string rt = inst.substr(11,5);
			string rd = inst.substr(16,5);
			r[BtD(rd)] = r[BtD(rs)] & r[BtD(rt)];
			pc += 4; 
		}
		else if (second == "001000"){//jr
			string rs = inst.substr(6,5);
			pc = r[BtD(rs)];
		}
		else if (second == "100111"){//nor
			string rs = inst.substr(6,5);
			string rt = inst.substr(11,5);
			string rd = inst.substr(16,5);
			r[BtD(rd)] = ~(r[BtD(rs)] | r[BtD(rt)]);
			pc += 4; 	
		}
		else if (second == "100101"){//or
			string rs = inst.substr(6,5);
			string rt = inst.substr(11,5);
			string rd = inst.substr(16,5);
			r[BtD(rd)] = r[BtD(rs)] | r[BtD(rt)];
			pc += 4; 	
		}
		else if (second == "101011"){//sltu
			string rs = inst.substr(6,5);
			string rt = inst.substr(11,5);
			string rd = inst.substr(16,5);
			if(r[BtD(rs)] < r[BtD(rt)]){
				r[BtD(rd)] = 1;
			}
			else{
				r[BtD(rd)] = 0;
			}
			pc += 4; 
		}
		else if (second == "000000"){//sll
			string rt = inst.substr(11,5);
			string rd = inst.substr(16,5);
			string shmt = inst.substr(21,5);
			r[BtD(rd)] = r[BtD(rt)] << BtD(shmt);
			pc += 4; 	
		}
		else if (second == "000010"){//srl
			string rt = inst.substr(11,5);
			string rd = inst.substr(16,5);
			string shmt = inst.substr(21,5);
			r[BtD(rd)] = r[BtD(rt)] >> BtD(shmt);
			pc += 4; 	
		}
		else if (second == "100011"){//subu
			string rt = inst.substr(11,5);
			string rd = inst.substr(16,5);
			string rs = inst.substr(6,5);
			r[BtD(rd)] = r[BtD(rs)] - r[BtD(rt)];
			pc += 4; 	
		}
		else{ cout << "nope: " << inst << endl;}
	}
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


int main(int argc, const char* argv[]){
	int PC = 4194304;
	int R[32] {0};
	bool n = false;
	bool m = false;
	bool d = false;
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
	while (add_map.find(PC) != add_map.end()) {
		if(n){
			if(check_n == num_n){break;}
		}
		check_i(HtB(add_map[PC]), R, PC, add_map);
		check_n += 1;
		re_add(add_map);
		if (d){
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
					string new1 = DtB32(HtD(add_map[m1i])).substr(0,8);
					for (int i = 0; i < 3*8; ++i){
						new1 += "0";
					}
					string plz = makeH(new1);
					for (int i = 0; i < 3*2; ++i){
						plz.pop_back();
					}	
					cout << DtH(m1i) << ": " << "0x"+make_n(plz) << endl;
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
			}
		}
	}
	if (!d){
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
				string new1 = DtB32(HtD(add_map[m1i])).substr(0,8);
				for (int i = 0; i < 3*8; ++i){
					new1 += "0";
				}
				string plz = makeH(new1);
				for (int i = 0; i < 3*2; ++i){
					plz.pop_back();
				}	
				cout << DtH(m1i) << ": " << "0x"+make_n(plz) << endl;
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
