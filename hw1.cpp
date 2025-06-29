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
#include <utility>
using namespace std;


string label_check(string str){
	size_t index = str.find(':');
        if(index != string::npos){
                  return str.substr(0, index);
        }
	else{
		return "nope";
	}
}


bool emptyline(string& str) {
	size_t index = str.find(':');
	if(index != string::npos){
		str = str.substr(index + 1);
	}

	if (str[0] == '\0'||str[0] =='\n' ) {
	        return true;
    	}
	for (char c : str) {
		if (c != ' ' && c != '\t') {
		return false;
		}
	}
	return true;
}

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


string DtB5(int d){
	string B = bitset<5>(d).to_string();
	return B;
}

string DtB16(long long int d){
         string B = bitset<16>(d).to_string();
         return B;
}

string DtB26(long long int d){
           string B = bitset<26>(d).to_string();
           return B;
  }

string DtB32(long long int d){
	string B = bitset<32>(d).to_string();
	return B;
}

string addiu(int rs, int rt, long long int imm){
	string df = "001001";
	string s = DtB5(rs);
	string t = DtB5(rt);
	string m = DtB16(imm);
	string total = df+s+t+m;
	string result = makeH(total); 
	return result;
}

string addu(int rs, int rt, int rd){
          string df = "000000";
          string s = DtB5(rs);
          string t = DtB5(rt);
          string d = DtB5(rd);
          string total = df+s+t+d+"00000"+"100001";
          string result = makeH(total);
          return result;
 }

string And(int rs, int rt, int rd){
             string df = "000000";
             string s = DtB5(rs);
             string t = DtB5(rt);
             string d = DtB5(rd);
             string total = df+s+t+d+"00000100100";
             string result = makeH(total);
             return result;
}

string andi(int rs, int rt, long long int imm){
            string df = "001100";
            string s = DtB5(rs);
            string t = DtB5(rt);
            string m = DtB16(imm);
            string total = df+s+t+m;
            string result = makeH(total);
            return result;
}

string beq(int rs, int rt, long long int offset){
            string df = "000100";
            string s = DtB5(rs);
            string t = DtB5(rt);
            string off = DtB16(offset);
            string total = df+s+t+off;
            string result = makeH(total);
            return result;
}

string bne(int rs, int rt, long long int offset){
            string df = "000101";
            string s = DtB5(rs);
            string t = DtB5(rt);
            string off = DtB16(offset);
            string total = df+s+t+off;
            string result = makeH(total);
            return result;
}

string j(long long int target){
            string df = "000010";
            string t = DtB26(target);
            string total = df+t;
            string result = makeH(total);
            return result;
}

string jal(long long int target){
           string df = "000011";
           string t = DtB26(target);
           string total = df+t;
           string result = makeH(total);
           return result;
}

string jr(int rs){
             string df = "000000";
             string s = DtB5(rs);
             string total = df+s+"000000000000000"+"001000";
             string result = makeH(total);
             return result;
}

string lui(int rt, long long int imm){
             string df = "001111";
             string t = DtB5(rt);
             string m = DtB16(imm);
             string total = df+"00000"+t+m;
             string result = makeH(total);
             return result;
}

string lw(int rs, int rt, long long int offset){
             string df = "100011";
             string s = DtB5(rs);
             string t = DtB5(rt);
             string off = DtB16(offset);
             string total = df+s+t+off;
             string result = makeH(total);
             return result;
}

string lb(int rs, int rt, long long int offset){
             string df = "100000";
             string s = DtB5(rs);
             string t = DtB5(rt);
             string off = DtB16(offset);
             string total = df+s+t+off;
             string result = makeH(total);
             return result;
 }

string nor(int rs, int rt, long long int rd){
             string df = "000000";
             string s = DtB5(rs);
             string t = DtB5(rt);
             string d = DtB5(rd);
             string total = df+s+t+d+"00000"+"100111";
             string result = makeH(total);
             return result;
 }

string Or(int rs, int rt, int rd){
             string df = "000000";
             string s = DtB5(rs);
             string t = DtB5(rt);
             string d = DtB5(rd);
             string total = df+s+t+d+"00000"+"100101";
             string result = makeH(total);
             return result;
 }

string ori(int rs, int rt, long long int imm){
             string df = "001101";
             string s = DtB5(rs);
             string t = DtB5(rt);
             string m = DtB16(imm);
             string total = df+s+t+m;
             string result = makeH(total);
             return result;
 }

string sltiu(int rs, int rt, long long int imm){
             string df = "001011";
             string s = DtB5(rs);
             string t = DtB5(rt);
             string off = DtB16(imm);
             string total = df+s+t+off;
             string result = makeH(total);
             return result;
}

string sltu(int rs, int rt, int rd){
            string df = "000000";
            string s = DtB5(rs);
            string t = DtB5(rt);
            string d = DtB5(rd);
            string total = df+s+t+d+"00000101011";
            string result = makeH(total);
            return result;
}


string sll(int rt, int rd, int shamt){
              string df = "00000000000";
              string d = DtB5(rd);
              string t = DtB5(rt);
              string s = DtB5(shamt);
              string total = df+t+d+s+"000000";
              string result = makeH(total);
              return result;
}

string srl(int rt, int rd, int shamt){
               string df = "00000000000";
               string d = DtB5(rd);
               string t = DtB5(rt);
               string s = DtB5(shamt);
               string total = df+t+d+s+"000010";
               string result = makeH(total);
               return result;
}

string sw(int rs, int rt, long long int offset){
               string df = "101011";
               string s = DtB5(rs);
               string t = DtB5(rt);
               string off = DtB16(offset);
               string total = df+s+t+off;
               string result = makeH(total);
               return result;
}

string sb(int rs, int rt, long long int offset){
                string df = "101000";
                string s = DtB5(rs);
                string t = DtB5(rt);
                string off = DtB16(offset);
                string total = df+s+t+off;
                string result = makeH(total);
                return result;
}

string subu(int rs, int rt, int rd){
                string df = "000000";
                string s = DtB5(rs);
                string t = DtB5(rt);
                string d = DtB5(rd);
                string total = df+s+t+d+"00000100011";
                string result = makeH(total);
                return result;
}

string get_add(map<string, int>& myMap, string label){
	int txt = myMap[".text"];
	int lab = myMap[label];
	long long int num;
	if(lab<txt){
		num = 268435456+(lab-1)*4;
	}
	else{
		num = 4194304+(lab-txt-1)*4;
	}
	stringstream ss;
    	ss << hex << num;
	string add = ss.str();
	return add;
}

bool la_check(map<string, int>& myMap, string buffer,int now){
	regex pattern("\\S+");
	smatch match;
	string label;
	size_t index = buffer.find(',');
	string want = buffer.substr(index+1);
	if(regex_search(want, match, pattern)){
		label = match[0].str();
	}
	string add = get_add(myMap, label);
	int len = add.length();
	if (add.substr(len-4) == "fffc"){
                 if(myMap[label]>now){
                         return false;
                 }
        }
	if (add.substr(len-4) != "0000"){
		return true;	
	}
	else{
		return false;
	}
}


string find_d(string data){
         regex pattern(".word\\s*([a-zA-Z0-9]+)");
         smatch match;
         if(regex_search(data, match, pattern)){
                 string data_num = match.str(1);
                 if(data_num.substr(0,2) == "0x"){
                         return data_num.substr(2);
 }
                  else{
                         long long int num = stoi(data_num);
                         string num32 = DtB32(num);
                         string final_num = makeH(num32);
                         return final_num;
                 }
 }
         else{
                 return "nope";
         }
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




string instruction(string str, int now, map<string, int>& myMap){
	string inst;
	regex pattern(R"(\S+)");
        smatch match;
        if(regex_search(str,match,pattern)){
        	inst = match.str();
                }
	else {inst = "none";
	}
	regex pattern1("\\d+");
        smatch match1;
	smatch match10;
	if(inst == "addiu"){
		int rs;
		int rt;
		long long int imm;
		size_t index = str.find('$');
		size_t new_index = str.substr(index+1).find('$');
		new_index += index + 1;
        	string str1 = str.substr(index+3);
		if(index != string::npos){
			if(regex_search(str,match1,pattern1)){
				rt = stoi(match1.str());
	
			}
			if(regex_search(str1,match10,pattern1)){
               			rs = stoi(match10.str());
           
			}
			regex pattern2("[a-zA-Z0-9-]+");
			smatch match2;
			size_t hex = str.substr(new_index+1).find("0x");
			if(hex != string::npos){
				hex += new_index + 1;
				string str2 = str.substr(hex);			
				if(regex_search(str2,match2,pattern2)){
					imm = stoi(match2.str(), nullptr, 16);
				}
			}
			else{
				string str3 = str.substr(new_index+3);
				if(regex_search(str3,match2,pattern2)){
					imm = stoi(match2.str());
                		}
			}	
		}
		return addiu(rs, rt, imm);
	}
	else if(inst == "addu"){ 
		int rs;
		int rt;
                int rd;
                size_t index = str.find('$');
                size_t new_index = str.substr(index+1).find('$');
		new_index += index + 1;
                if(index != string::npos){
                         if(regex_search(str,match1,pattern1)){
                                 rd = stoi(match1.str());
                         }
			string str1 =str.substr(index+3);
                         if(regex_search(str1,match1,pattern1)){
                                 rs = stoi(match1.str());
                         }
			string str2 = str.substr(new_index+3);
			 if(regex_search(str2,match1,pattern1)){
				rt = stoi(match1.str());
                 	}
                 }
		return addu(rs,rt,rd);
	}
	else if(inst == "and"){
		int rs;
                int rt;
                int rd;
                size_t index = str.find('$');
                size_t new_index = str.substr(index+1).find('$');
		new_index += index + 1;
                if(index != string::npos){
                         if(regex_search(str,match1,pattern1)){
                                 rd = stoi(match1.str());
                         }
			string str1 = str.substr(index+3);
                         if(regex_search(str1,match1,pattern1)){
                                 rs = stoi(match1.str());
                         }
			string str2 = str.substr(new_index+3);
                         if(regex_search(str2,match1,pattern1)){
                                rt = stoi(match1.str());
                        }
                 }
                 return And(rs,rt,rd);
	}
	else if(inst == "andi"){      
		int rs;
		int rt;
                long long int imm;
                size_t index = str.find('$');
                size_t new_index = str.substr(index+1).find('$');
		new_index += index + 1;
                if(index != string::npos){
                	 if(regex_search(str,match1,pattern1)){
                                 rt = stoi(match1.str());
                         }
			string str1 = str.substr(index+3);
                         if(regex_search(str1,match1,pattern1)){
                                 rs = stoi(match1.str());
                         }
                         regex pattern2("[a-zA-Z0-9-]+");
                         smatch match2;
                         size_t hex = str.substr(new_index+1).find("0x");
                         if(hex != string::npos){
				hex += new_index + 1;
				string str2 = str.substr(hex);
                                 if(regex_search(str2,match2,pattern2)){
                                         imm = stoi(match2.str(), nullptr, 16);
                                 }
                         }
                         else{
				string str3 = str.substr(new_index+3);
                                 if(regex_search(str3,match2,pattern2)){
                                         imm = stoi(match2.str());
                                 }
                         }
                 }
                 return andi(rs, rt, imm);

	}
	else if(inst == "beq"){
		regex pattern12("\\S+");
 	        smatch match12;
 	        string label;
 	        size_t index1 = str.find(',');
 	        size_t index2 = str.substr(index1+1).find(',');
		index2 += index1 + 1;
		string want = str.substr(index2+1);
 	        if(regex_search(want, match12, pattern12)){
 	        	label = match12[0].str();
 	     	}
 	        int ln = myMap[label];
		long long int off = ln - now - 1;
                int rs;
                int rt;
                size_t index = str.find('$');
                size_t new_index = str.substr(index+1).find('$');
                new_index += index + 1;
                if(index != string::npos){
                         if(regex_search(str,match1,pattern1)){
                                 rs = stoi(match1.str());
                         }
                        string str1 = str.substr(index+3);
                         if(regex_search(str1,match1,pattern1)){
                                 rt = stoi(match1.str());
                         }
		}
		return beq(rs, rt, off);
//offset 함수 만들어야 함
	}
	else if(inst == "bne"){
		regex pattern12("\\S+");
                smatch match12;
                string label;
                size_t index1 = str.find(',');
                size_t index2 = str.substr(index1+1).find(',');
                index2 += index1 + 1;
                string want = str.substr(index2+1);
                if(regex_search(want, match12, pattern12)){
                        label = match12[0].str();
                }
                int ln = myMap[label];
                long long int off = ln - now - 1;
                int rs;
                int rt;
                size_t index = str.find('$');
                size_t new_index = str.substr(index+1).find('$');
                new_index += index + 1;
                if(index != string::npos){
                         if(regex_search(str,match1,pattern1)){
                                 rs = stoi(match1.str());
                         }
                        string str1 = str.substr(index+3);
                        if(regex_search(str1,match1,pattern1)){
                                 rt = stoi(match1.str());
                        }
                }
                return bne(rs, rt, off);
	}
	else if(inst == "j"){   
		regex pattern12("\\S+");
                smatch match12;
                string label;
                size_t index1 = str.find('j');
                index1 += 1;
                string want = str.substr(index1);
                if(regex_search(want, match12, pattern12)){
                        label = match12.str();
                }
		string add = get_add(myMap, label);
		int len = add.length();
		if (len<7){
		}
		else {
			int check = len-7;
			add = add.substr(check);
		}
		long long int decimalValue;
		decimalValue = stoi(add, nullptr, 16);
    		decimalValue /= 4;
		return j(decimalValue);		
	} //target 함수 만들어야 함
	else if(inst == "jal"){
		regex pattern12("\\S+");
                smatch match12;
                string label;
                size_t index1 = str.find("jal");
                string want = str.substr(index1+3);
                if(regex_search(want, match12, pattern12)){
                        label = match12.str();
                }
                string add = get_add(myMap, label);
                int len = add.length();
                if (len<7){
                }
                else {
                        int check = len-7;
                        add = add.substr(check);
                }
                long long int decimalValue;
		decimalValue = stoi(add, nullptr, 16);
                decimalValue /= 4;
                return jal(decimalValue);
	}
	else if(inst == "jr"){
	     int rs;
             if(regex_search(str,match1,pattern1)){
             	rs = stoi(match1.str());
             }
	return jr(rs);

	}
	else if(inst == "lui"){
    		int rs = 0;
		int rt;
                long long int imm;
                size_t index = str.find('$');
                if(index != string::npos){
                         if(regex_search(str,match1,pattern1)){
                                 rt = stoi(match1.str());
                         }
                         regex pattern2("[a-zA-Z0-9-]+");
                         smatch match2;
                         size_t hex = str.substr(index+1).find("0x");
                         if(hex != string::npos){
				hex += index + 1;
				string str1 = str.substr(hex);
                                 if(regex_search(str1,match2,pattern2)){
                                         imm = stoi(match2.str(), nullptr, 16);
                                 }
                         }
                         else{
				string str2 = str.substr(index+3);
                                 if(regex_search(str2,match2,pattern2)){
                                         imm = stoi(match2.str());
                                 }
                         }
                 }
                 return lui(rt, imm); 
       }
	else if(inst == "lw"){
		int rs;
		int rt;
                long long int imm;
                size_t index = str.find('$');
		size_t new_index = str.substr(index+1).find('$');
		new_index += index +1;
                if(index != string::npos){
                         if(regex_search(str,match1,pattern1)){
                                 rt = stoi(match1.str());
                         }
			string str1 = str.substr(new_index);
                         if(regex_search(str1,match1,pattern1)){
                                 rs = stoi(match1.str());
                         }
                         regex pattern2("\\w+(?=\\()");
                         smatch match2;
                         size_t hex = str.substr(index+1).find("0x");
                         if(hex != string::npos){
				hex += index + 1;
				string str2 = str.substr(hex);
                                 if(regex_search(str2,match2,pattern2)){
                                         imm = stoi(match2.str(), nullptr, 16);
                                 }
                         }
                         else{
                                 if(regex_search(str,match2,pattern2)){
                                         imm = stoi(match2.str());
                                 }
                         }
                 }
                 return lw(rs, rt, imm);
	}
	else if(inst == "lb"){
		int rs;
                int rt;
                long long int imm;
                size_t index = str.find('$');
		size_t new_index = str.substr(index+1).find('$');
		new_index += index + 1;
                if(index != string::npos){
                         if(regex_search(str,match1,pattern1)){
                                 rt = stoi(match1.str());
                         }
			string str1 = str.substr(new_index);
                         if(regex_search(str1,match1,pattern1)){
                                 rs = stoi(match1.str());
                          }
                         regex pattern2("\\w+(?=\\()");
                         smatch match2;
                         size_t hex = str.substr(index+1).find("0x");
                         if(hex != string::npos){
				hex += index + 1;
                                string str2 = str.substr(hex); 
				if(regex_search(str2,match2,pattern2)){
                                         imm = stoi(match2.str(), nullptr, 16);
                                 }
                         }
                         else{
                                 if(regex_search(str,match2,pattern2)){
                                         imm = stoi(match2.str());
                                 }
                         }
                 }
                 return lb(rs, rt, imm);
	}
	else if(inst == "nor"){		
		int rs;
                int rt;
                int rd;
                size_t index = str.find('$');
                size_t new_index = str.substr(index+1).find('$');
		new_index += index + 1;
                if(index != string::npos){
                         if(regex_search(str,match1,pattern1)){
                                 rd = stoi(match1.str());
                         }
			string str1 = str.substr(index+3);
                         if(regex_search(str1,match1,pattern1)){
                                 rs = stoi(match1.str());
                         }
			string str2 = str.substr(new_index+3);
                         if(regex_search(str2,match1,pattern1)){
                                rt = stoi(match1.str());
                        }
                 }
                 return nor(rs,rt,rd);
	}	
	else if(inst == "or"){
		int rs;
                int rt;
                int rd;
                size_t index = str.find('$');
                size_t new_index = str.substr(index+1).find('$');
		new_index += index + 1;
                if(index != string::npos){
                         if(regex_search(str,match1,pattern1)){
                                 rd = stoi(match1.str());
                         }
			string str1 = str.substr(index+3);
                         if(regex_search(str1,match1,pattern1)){
                                 rs = stoi(match1.str());
                         }
			string str2 = str.substr(new_index+3);
                         if(regex_search(str2,match1,pattern1)){
                                rt = stoi(match1.str());
                        }
                 }
                 return Or(rs,rt,rd);
	}
	else if(inst == "ori"){
		int rs;
		int rt;
                long long int imm;
                size_t index = str.find('$');
                size_t new_index = str.substr(index+1).find('$');
		new_index += index + 1;
                if(index != string::npos){
                         if(regex_search(str,match1,pattern1)){
                                 rt = stoi(match1.str());
                         }
			string str1 = str.substr(index+3);
                         if(regex_search(str1,match1,pattern1)){
                                 rs = stoi(match1.str());
                         }
                         regex pattern2("[a-zA-Z0-9-]+");
                         smatch match2;
                         size_t hex = str.substr(new_index+1).find("0x");
                         if(hex != string::npos){
				hex += new_index + 1;
				string str2 = str.substr(hex);
                                 if(regex_search(str2,match2,pattern2)){
                                         imm = stoi(match2.str(), nullptr, 16);
                                 }
                         }
                         else{
				string str3 = str.substr(new_index+3);
                                 if(regex_search(str3,match2,pattern2)){
                                         imm = stoi(match2.str());
                                 }
                         }
                 }
                 return ori(rs, rt, imm);
	}
	else if(inst == "sltiu"){      
	     int rs;
	     int rt;
             long long int imm;
             size_t index = str.find('$');
             size_t new_index = str.substr(index+1).find('$');
             new_index += index + 1;
             if(index != string::npos){
                   if(regex_search(str,match1,pattern1)){
                          rt = stoi(match1.str());
                   }
                   string str1 = str.substr(index+3);
                   if(regex_search(str1,match1,pattern1)){
                              rs = stoi(match1.str());
                      }
                      regex pattern2("[a-zA-Z0-9-]+");
                        smatch match2;
                      size_t hex = str.substr(new_index+1).find("0x");
                       if(hex != string::npos){
                               hex += new_index + 1;
                             string str2 = str.substr(hex);
                                if(regex_search(str2,match2,pattern2)){
                                     imm = stoi(match2.str(), nullptr, 16);
                            }
                        }
                        else{
                             string str3 = str.substr(new_index+3);
                             if(regex_search(str3,match2,pattern2)){
                                        imm = stoi(match2.str());
                              }
                      }
             }
            return sltiu(rs, rt, imm);
      }
	else if(inst == "sltu"){
		int rs;
                int rt;
                int rd;
                size_t index = str.find('$');
                size_t new_index = str.substr(index+1).find('$');
		new_index += index + 1;
                if(index != string::npos){
                         if(regex_search(str,match1,pattern1)){
                                 rd = stoi(match1.str());
                         }
			string str1 = str.substr(index+3);
                         if(regex_search(str1,match1,pattern1)){
                                 rs = stoi(match1.str());
                         }
			string str2 = str.substr(new_index+3);
                         if(regex_search(str2,match1,pattern1)){
                                rt = stoi(match1.str());
                        }
                 }
                 return sltu(rs,rt,rd);
	}
	else if(inst == "sll"){
		int rd;
		int rt;
                int shmt;
                size_t index = str.find('$');
                size_t new_index = str.substr(index+1).find('$');
		new_index += index + 1;
                if(index != string::npos){
                         if(regex_search(str,match1,pattern1)){
                                 rd = stoi(match1.str());
                         }
			string str1 = str.substr(index+3);
                         if(regex_search(str1,match1,pattern1)){
                                 rt = stoi(match1.str());
                         }
                         regex pattern2("[a-zA-Z0-9-]+");
                         smatch match2;
                         size_t hex = str.substr(new_index+1).find("0x");
                         if(hex != string::npos){
				hex += new_index + 1;
				string str2 = str.substr(hex);
                                 if(regex_search(str2,match2,pattern2)){
                                         shmt = stoi(match2.str(), nullptr, 16);
                                 }
                         }
                         else{
				string str3 = str.substr(new_index+3);
                                 if(regex_search(str3,match2,pattern2)){
                                         shmt = stoi(match2.str());
                                 }
                         }
                 }
                 return sll(rt,rd, shmt);
	}
	else if(inst == "srl"){
		int rd;
		int rt;
                int imm;
                size_t index = str.find('$');
                size_t new_index = str.substr(index+1).find('$');
		new_index += index + 1;
                if(index != string::npos){
                         if(regex_search(str,match1,pattern1)){
                                 rd = stoi(match1.str());
                         }
			string str1 = str.substr(index+3);
                         if(regex_search(str1,match1,pattern1)){
                                 rt = stoi(match1.str());
                         }
                         regex pattern2("[a-zA-Z0-9-]+");
                         smatch match2;
                         size_t hex = str.substr(new_index+1).find("0x");
                         if(hex != string::npos){
				hex += new_index + 1;
				string str2 = str.substr(hex);
                                 if(regex_search(str2,match2,pattern2)){
                                         imm = stoi(match2.str(), nullptr, 16);
                                 }
                         }
                         else{
				string str3 = str.substr(new_index+3);
                                 if(regex_search(str3,match2,pattern2)){
                                         imm = stoi(match2.str());
                                 }
                         }
                 }
                 return srl(rt, rd, imm);
	}
	else if(inst == "sw"){
		int rs;
                int rt;
                long long int imm;
                size_t index = str.find('$');
		size_t new_index = str.substr(index+1).find('$');
		new_index += index + 1;
                if(index != string::npos){
                         if(regex_search(str,match1,pattern1)){
                                 rt = stoi(match1.str());
                         }
			string str1 = str.substr(new_index);
                         if(regex_search(str1,match1,pattern1)){
                                 rs = stoi(match1.str());
                          }
                         regex pattern2("\\w+(?=\\()");
                         smatch match2;
                         size_t hex = str.substr(index+1).find("0x");
                         if(hex != string::npos){
				hex += index + 1;
				string str2 = str.substr(hex);
                                 if(regex_search(str2,match2,pattern2)){
                                         imm = stoi(match2.str(), nullptr, 16);
                                 }
                         }
                         else{
                                 if(regex_search(str,match2,pattern2)){
                                         imm = stoi(match2.str());
                                 }
                         }
                 }
                 return sw(rs, rt, imm);
	}
	else if(inst == "sb"){
		int rs;
                int rt;
                long long int imm;
                size_t index = str.find('$');
		size_t new_index = str.substr(index+1).find('$');
		new_index += index + 1;
                if(index != string::npos){
                         if(regex_search(str,match1,pattern1)){
                                 rt = stoi(match1.str());
                         }
			string str1 = str.substr(new_index);
                         if(regex_search(str1,match1,pattern1)){
                                 rs = stoi(match1.str());
                          }
                         regex pattern2("\\w+(?=\\()");
                         smatch match2;
                         size_t hex = str.substr(index+1).find("0x");
                         if(hex != string::npos){
				hex += index + 1;
				string str2 = str.substr(hex);
                                 if(regex_search(str2,match2,pattern2)){
                                         imm = stoi(match2.str(), nullptr, 16);
                                 }
                         }
                         else{
                                 if(regex_search(str,match2,pattern2)){
                                         imm = stoi(match2.str());
                                 }
                         }
                 }
                 return sb(rs, rt, imm);
	}
	else if(inst == "subu"){
		int rs;
                int rt;
                int rd;
                size_t index = str.find('$');
                size_t new_index = str.substr(index+1).find('$');
		new_index += index + 1;
                if(index != string::npos){
                         if(regex_search(str,match1,pattern1)){
                                 rd = stoi(match1.str());
                         }
			string str1 = str.substr(index+3);
                         if(regex_search(str1,match1,pattern1)){
                                 rs = stoi(match1.str());
                         }
			string str2 = str.substr(new_index+3);
                         if(regex_search(str2,match1,pattern1)){
                                rt = stoi(match1.str());
                        }
                 }
                 return subu(rs,rt,rd);
	}
	else {}
	return "nope";
}

pair<string, string> la(map<string, int>& myMap, string word, int now){
	int rt;
	size_t index = word.find('$');
	regex pattern("\\d+");
	smatch match;
	if(regex_search(word, match, pattern)){
		rt = stoi(match.str());
	}

	regex pattern1("\\S+");
        smatch match1;
        string label;
        size_t index1 = word.find(',');
        string want = word.substr(index1+1);
        if(regex_search(want, match1, pattern1)){
                label = match1[0].str();
        }
        string add = get_add(myMap, label);
        int len = add.length();
	long long int ad1 = stoi(add.substr(0,len-4), nullptr, 16);
	if (add.substr(len-4) == "fffc"){
		if(myMap[label]>now){
			return make_pair(lui(rt,ad1), "nope");		
		}
	}
        if (add.substr(len-4) != "0000"){
                 long long int ad = stoi(add.substr(len-4),nullptr,16);
		 if(myMap[label]>now){
			ad += 4;
			if(ad>65535){
				ad -= 65535;
			}
		 }
		for (auto& pair : myMap) {
			if(pair.second>now){
				pair.second += 1;
			}
		  }
                 return make_pair(lui(rt,ad1), ori(rt, rt, ad)); 
	}
         return make_pair(lui(rt,ad1), "nope");
}

bool is_la(string buffer){
	string inst;
        regex pattern(R"(\S+)");
        smatch match;
        if(regex_search(buffer,match,pattern)){
                inst = match.str();
       	}
	if (inst == "la"){
		return true;
	}
	else{
		return	false;
	}
}


int main(int argc, const char* argv[]){

FILE* file = fopen(argv[1],"r");



string data = ".data";
string text = ".text";
string datalist = "";
string textlist = "";
map<string, int> label_map;
int id = 0;
int it = 0;
int total_i = 0;
int check = 0;
int check2 = 0;
char buffer[256];
char buffer2[256];

while (fgets(buffer, sizeof(buffer), file) != nullptr) { //fgets call only one sentence. so use while

string word = buffer;

if(label_check(word) != "nope"){
	label_map[label_check(word)] = total_i; 
}

if(word.find(data) != string::npos){
        check = 1;
        }// data line
if(word.find(text) != string::npos) {
        if (check == 1){
                 id -= 1;
        }
        check = 2;
	label_map[".text"] = total_i;
        } // text line


if(check == 1){
	if(emptyline(word)){
 		id -= 1;
		total_i -= 1;
}
	else{
		if (find_d(word) != "nope"){
		datalist += "0x" + make_n(find_d(word));
		datalist += "\n";
		}
}
        id += 1;
}
else if (check == 2){
	if(emptyline(word)){
		it -= 1;
		total_i -= 1;
}
        it += 1;
}
total_i += 1;
}

it -= 1;

fclose(file);

FILE* file2 = fopen(argv[1],"r");
pair<string, string> twotext;
int now=-1;
while (fgets(buffer2, sizeof(buffer2), file2) != nullptr) { //fgets call only one sentence. so use while
	string word2 = buffer2;
	if(not emptyline(word2)){
		now += 1;
		if(check2 == 1){
			if(is_la(word2)){
				if(la_check(label_map, word2, now)){
					it += 1;
					twotext = la(label_map, word2, now);
					string some1 = twotext.first;
					string some2 = twotext.second;
					textlist += "0x" + make_n(some1);
					textlist += "\n";
					textlist += "0x" + make_n(some2);
					textlist += "\n";
					now += 1; 
				}
				else {
					twotext = la(label_map, word2, now);
					string some1 = twotext.first;
					textlist += "0x" + make_n(some1);
					textlist += "\n";
				}
				
			}
			else{
				string some = instruction(word2, now, label_map);
				if(some != "nope"){
					textlist += "0x" + make_n(some);
					textlist += "\n";
				}
				else{
					it -= 1;
					now -= 1;
				}
			}
		
		}
	}
	if(word2.find(text) != string::npos) {
                check2 = 1;
        } // text line

}

fclose(file2);

int h_d = id*4;
int h_t = it*4;
string name = argv[1];
size_t dot = name.find('.');
name = name.substr(0,dot);
string filename = name+".o";
ofstream outputFile(filename);

if (outputFile.is_open()){
	outputFile << "0x" << hex << h_t << endl;
	outputFile << "0x" << hex << h_d << endl;
	outputFile << textlist;
	outputFile << datalist;
	outputFile.close();
}

return 0;

}
