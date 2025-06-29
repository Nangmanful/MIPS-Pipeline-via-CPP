// have to make cache structure
#include <iostream>
#include <string> 
#include <sstream>
#include <bitset>
#include <cstdio>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <random>
#include <iomanip>
#include <fstream>
using namespace std;

int getRandom(int n) {
    std::random_device rd;  
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<> dis(0, n - 1); 
    return dis(gen); // 랜덤 숫자 반환
}

struct Block{
	int valid = 0;
	int tag;
	int dirty = 0;
	int rank =0;
};

struct Set{
	vector<Block> blocks;
};

struct Lcache{
	int capacity;
	int associativity;
	vector<Set> sets;

	Lcache(int cap, int assoc, int numSets) : capacity(cap), associativity(assoc) {
        sets.resize(numSets);
        for (auto& set : sets) {
            set.blocks.resize(associativity);
        }
    }

};

long long int BtD(string bit){                                                                                                                long long int D = stoll(bit, nullptr, 2);
        return D;
}

string DtB32(long long int d){
        string B = bitset<64>(d).to_string();
        return B;
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


int findMax(const Set& set) {
    if (set.blocks.empty()) {
        throw std::invalid_argument("The block vector is empty");
    }

    int maxRankIndex = 0;
    int maxRank = set.blocks[0].rank;

    for (size_t i = 1; i < set.blocks.size(); ++i) {
        if (set.blocks[i].rank > maxRank) {
            maxRank = set.blocks[i].rank;
            maxRankIndex = i;
        }
    }

    return maxRankIndex;
}



int main(int argc, const char* argv[]){       
	int capa;
	int assoc;
	int blocks;
//	Lcache L1;
//	Lcache L2;
	int rand = 0;
	int okay = 0;
	for (int i = 0; i <argc-1; ++i){
		if(string(argv[i]) == "-c"){
			capa = stoi(argv[i+1]);
			okay += 1;
		}
		if(string(argv[i]) == "-a"){
			assoc = stoi(argv[i+1]);
			okay += 1;
		}
		if(string(argv[i]) == "-b"){
			blocks = stoi(argv[i+1]);
			okay += 1;
		}
		if(string(argv[i]) == "-lru"){
			okay += 1;
		}
		if(string(argv[i]) == "-random"){
			rand = 1;
			okay += 1;
		}
	}
	if (okay < 4){
		cout << "parameter error" << endl;
		return 0;
	}
	int set_size1;
	int set_size2 = log2(blocks)+log2(assoc);
	int capacity2 = log2(capa) + 10;
	int capacity1 = log2(capa) + 8;
	int associativity2 = assoc;
	int blockoff = log2(blocks);
	int index_2 = capacity2 - set_size2;
	int index_1;
	int associativity1;
	if(assoc <3){
		associativity1 = assoc;
		set_size1 = log2(assoc)+log2(blocks);
		index_1 = capacity1 - set_size1;
	}	
	else{
		associativity1 = assoc/4;
		set_size1 = log2(assoc)+log2(blocks)-2;
		index_1 = capacity1 - set_size1;
	}
	int result1 = static_cast<int>(std::pow(2, index_1)); // 2의 n승 계산
    	int result2 = static_cast<int>(std::pow(2, index_2)); // 2의 n승 계산
	Lcache L1(capacity1, associativity1, result1);
	Lcache L2(capacity2, associativity2, result2);
	int Racc = 0;
	int Wacc = 0;
	int R1miss = 0;
	int R2miss = 0;
	int W1miss = 0;
	int W2miss = 0;
	int clean1 = 0;
	int clean2 = 0;
	int dirty1 = 0;
	int dirty2 = 0;
	char buffer[256];
	FILE* file1 = fopen(argv[argc-1],"r");
	while (fgets(buffer, sizeof(buffer), file1) != nullptr){
		string word = buffer;
		string address = word.substr(2);
		string badd = HtB(address);
		string l1index = badd.substr(64-blockoff-index_1,index_1);
		int L1ind = BtD(l1index);
		int tag1 = BtD(badd.substr(0, 64-blockoff-index_1));
		int tag2 = BtD(badd.substr(0, 64-blockoff-index_2));
		string l2index = badd.substr(64-blockoff-index_2, index_2);
		int L2ind = BtD(l2index);
		int find = 0;
		int intv = 0;
		int find2 = 0;
		int intv2 = 0;

//zzz		
		if(word[0] == 'R'){
		for(int i = 0; i < L1.associativity; i++){
			if(L1.sets[L1ind].blocks[i].valid == 0){
				//miss
			}
			else{
				if(L1.sets[L1ind].blocks[i].tag == tag1){
					find = 1;//find
					L1.sets[L1ind].blocks[i].rank = 0;
					for(int j = 0; j < L1.associativity; j++){
						if(L1.sets[L1ind].blocks[j].valid == 0){
						}
						else{
							L1.sets[L1ind].blocks[j].rank += 1;
						}	
					}
					for(int k = 0; k < L2.associativity; k++){
						if(L2.sets[L2ind].blocks[k].valid == 1){
							if(L2.sets[L2ind].blocks[k].tag == tag2){
								L2.sets[L2ind].blocks[k].rank = 1;
							}
							else{
								L2.sets[L2ind].blocks[k].rank += 1;
							}
						}
					}
				}
				else{//miss
					intv += 1;
				}
			}
		}
		if (find == 1){//hit
	
		}
		else{	
			if(intv < L1.associativity){//cold miss
				int done = 1;
				for(int i = 0; i < L1.associativity; i++){
					if(L1.sets[L1ind].blocks[i].valid == 0 && done){
						L1.sets[L1ind].blocks[i].valid = 1;
						L1.sets[L1ind].blocks[i].tag = tag1;
						L1.sets[L1ind].blocks[i].rank = 1;
						done = 0;
					}
					else{
						L1.sets[L1ind].blocks[i].rank += 1;
					}	
				}
				int done2 = 1;
				for(int j = 0; j < L2.associativity; j ++){
					if(L2.sets[L2ind].blocks[j].valid == 0 && done2){
						L2.sets[L2ind].blocks[j].valid = 1;
						L2.sets[L2ind].blocks[j].tag = tag2;
						L2.sets[L2ind].blocks[j].rank = 1;
						done2 = 0;
					}
					else{
						L2.sets[L2ind].blocks[j].rank += 1;
					}
				}
			}
			else{
				for(int i = 0; i < L2.associativity; i++){
					if(L2.sets[L2ind].blocks[i].valid == 0){
					//miss
					}
					else{
						if(L2.sets[L2ind].blocks[i].tag == tag2){
							find2 = 1;//find
							L2.sets[L2ind].blocks[i].rank = 0;
							for(int j = 0; j < L2.associativity; j++){
								if(L2.sets[L2ind].blocks[j].valid == 0){
								}
								else{
									L2.sets[L2ind].blocks[j].rank += 1;
								}	
							}
						}
						else{//miss
							intv2 += 1;
						}
					}
				}
				if(find2 == 1){//L1miss, L2hit
					int old1;
					if(rand == 0){//LRU
						old1 = findMax(L1.sets[L1ind]);
					}
					else{//Random
						old1 = getRandom(L1.associativity);
					}
					L1.sets[L1ind].blocks[old1].valid = 1; //old is LRU policy or Random Choice
					L1.sets[L1ind].blocks[old1].tag = tag1;
					L1.sets[L1ind].blocks[old1].rank = 0;
					if(L1.sets[L1ind].blocks[old1].dirty == 1){
						dirty1 += 1;
						L1.sets[L1ind].blocks[old1].dirty = 0;
					}
					else{
						clean1 += 1;
					}
					for(int i = 0; i < L1.associativity; i++){
						if(L1.sets[L1ind].blocks[i].valid == 0){
						}
						else{
							L1.sets[L1ind].blocks[i].rank += 1;
						}	
					}
				}
				else{
					if(intv2 < L2.associativity){//L1 is full but L2 is not, cold
						int done = 1;
						for(int j = 0; j < L2.associativity; j ++){
							if(L2.sets[L2ind].blocks[j].valid == 0 && done){
								L2.sets[L2ind].blocks[j].valid = 1;
								L2.sets[L2ind].blocks[j].tag = tag2;
								L2.sets[L2ind].blocks[j].rank = 1;
								done = 0;
							}
							else{
								L2.sets[L2ind].blocks[j].rank += 1;
							}
						}
						int old1;
						if(rand == 0){//LRU
							old1 = findMax(L1.sets[L1ind]);
						}
						else{//Random
							old1 = getRandom(L1.associativity);
						}

						L1.sets[L1ind].blocks[old1].valid = 1; //old is LRU policy or Random Choice
						L1.sets[L1ind].blocks[old1].tag = tag1;
						L1.sets[L1ind].blocks[old1].rank = 0;
						if(L1.sets[L1ind].blocks[old1].dirty == 1){
							dirty1 += 1;
							L1.sets[L1ind].blocks[old1].dirty = 0;
						}
						else{
							clean1 += 1;
						}

						for(int i = 0; i < L1.associativity; i++){
							if(L1.sets[L1ind].blocks[i].valid == 0){
							}
							else{
								L1.sets[L1ind].blocks[i].rank += 1;
							}	
						}
					}
					else{
						int old1;
						if(rand == 0){//LRU
							old1 = findMax(L1.sets[L1ind]);
						}
						else{//Random
							old1 = getRandom(L1.associativity);
						}
						int old2;
						if(rand == 0){//LRU
							old2 = findMax(L2.sets[L2ind]);
						}
						else{//Random
							old2 = getRandom(L2.associativity);
						}
						L2.sets[L2ind].blocks[old2].valid = 1; //old is LRU policy or Random Choice
						L2.sets[L2ind].blocks[old2].tag = tag2;
						L2.sets[L2ind].blocks[old2].rank = 0;
						L1.sets[L1ind].blocks[old1].valid = 1; //old is LRU policy or Random Choice
						L1.sets[L1ind].blocks[old1].tag = tag1;
						L2.sets[L1ind].blocks[old1].rank = 0;
						
						if(L1.sets[L1ind].blocks[old1].dirty == 1){
							dirty1 += 1;
							L1.sets[L1ind].blocks[old1].dirty = 0;
						}
						else{
							clean1 += 1;
						}
						if(L2.sets[L2ind].blocks[old2].dirty == 1){
							dirty2 += 1;
							L2.sets[L2ind].blocks[old2].dirty = 0;
						}
						else{
							clean2 += 1;
						}

						

						for(int i = 0; i < L1.associativity; i++){
							if(L1.sets[L1ind].blocks[i].valid == 0){
							}
							else{
								L1.sets[L1ind].blocks[i].rank += 1;
							}	
						}
						for(int j = 0; j < L2.associativity; j++){
							if(L2.sets[L2ind].blocks[j].valid == 0){
							}
							else{
								L2.sets[L2ind].blocks[j].rank += 1;
							}	
						}
					}
				}
			}					
		}
		}
		else{//W

			for(int i = 0; i < L1.associativity; i++){
			if(L1.sets[L1ind].blocks[i].valid == 0){
				//miss
			}
			else{
				if(L1.sets[L1ind].blocks[i].tag == tag1){
					find = 1;//find
					L1.sets[L1ind].blocks[i].rank = 0;
					L1.sets[L1ind].blocks[i].dirty = 1;
					for(int j = 0; j < L1.associativity; j++){
						if(L1.sets[L1ind].blocks[j].valid == 0){
						}
						else{
							L1.sets[L1ind].blocks[j].rank += 1;
						}	
					}
					for(int k = 0; k < L2.associativity; k++){
						if(L2.sets[L2ind].blocks[k].valid == 1){
							if(L2.sets[L2ind].blocks[k].tag == tag2){
								L2.sets[L2ind].blocks[k].rank = 1;
								
								L2.sets[L2ind].blocks[k].dirty = 1;
							}
							else{
								L2.sets[L2ind].blocks[k].rank += 1;
							}
						}
					}
				}
				else{//miss
					intv += 1;
				}
			}
		}
		if (find == 1){//hit
	
		}
		else{	
			if(intv < L1.associativity){//cold miss
				int done = 1;
				for(int i = 0; i < L1.associativity; i++){
					if(L1.sets[L1ind].blocks[i].valid == 0 && done){
						L1.sets[L1ind].blocks[i].valid = 1;
						L1.sets[L1ind].blocks[i].tag = tag1;
						L1.sets[L1ind].blocks[i].rank = 1;
						
						L1.sets[L1ind].blocks[i].dirty = 1;
						done = 0;
					}
					else{
						L1.sets[L1ind].blocks[i].rank += 1;
					}	
				}
				int done2 = 1;
				for(int j = 0; j < L2.associativity; j ++){
					if(L2.sets[L2ind].blocks[j].valid == 0 && done2){
						L2.sets[L2ind].blocks[j].valid = 1;
						L2.sets[L2ind].blocks[j].tag = tag2;
						L2.sets[L2ind].blocks[j].rank = 1;
						
						L2.sets[L2ind].blocks[j].dirty = 1;
						done2 = 0;
					}
					else{
						L2.sets[L2ind].blocks[j].rank += 1;
					}
				}
			}
			else{
				for(int i = 0; i < L2.associativity; i++){
					if(L2.sets[L2ind].blocks[i].valid == 0){
					//miss
					}
					else{
						if(L2.sets[L2ind].blocks[i].tag == tag2){
							find2 = 1;//find
							L2.sets[L2ind].blocks[i].rank = 0;
							
							L2.sets[L2ind].blocks[i].dirty = 1;
							for(int j = 0; j < L2.associativity; j++){
								if(L2.sets[L2ind].blocks[j].valid == 0){
								}
								else{
									L2.sets[L2ind].blocks[j].rank += 1;
								}	
							}
						}
						else{//miss
							intv2 += 1;
						}
					}
				}
				if(find2 == 1){//L1miss, L2hit
					int old1;
					if(rand == 0){//LRU
						old1 = findMax(L1.sets[L1ind]);
					}
					else{//Random
						old1 = getRandom(L1.associativity);
					}
					L1.sets[L1ind].blocks[old1].valid = 1; //old is LRU policy or Random Choice
					L1.sets[L1ind].blocks[old1].tag = tag1;
					L1.sets[L1ind].blocks[old1].rank = 0;
					
					if(L1.sets[L1ind].blocks[old1].dirty == 1){
						dirty1 += 1;
					}
					else{
						clean1 += 1;
					}
					
					L1.sets[L1ind].blocks[old1].dirty = 1;
					for(int i = 0; i < L1.associativity; i++){
						if(L1.sets[L1ind].blocks[i].valid == 0){
						}
						else{
							L1.sets[L1ind].blocks[i].rank += 1;
						}	
					}
				}
				else{
					if(intv2 < L2.associativity){//L1 is full but L2 is not, cold
						int done = 1;
						for(int j = 0; j < L2.associativity; j ++){
							if(L2.sets[L2ind].blocks[j].valid == 0 && done){
								L2.sets[L2ind].blocks[j].valid = 1;
								L2.sets[L2ind].blocks[j].tag = tag2;
								L2.sets[L2ind].blocks[j].rank = 1;
								
								L2.sets[L2ind].blocks[j].dirty = 1;
								done = 0;
							}
							else{
								L2.sets[L2ind].blocks[j].rank += 1;
							}
						}
						int old1;
						if(rand == 0){//LRU
							old1 = findMax(L1.sets[L1ind]);
						}
						else{//Random
							old1 = getRandom(L1.associativity);
						}

						L1.sets[L1ind].blocks[old1].valid = 1; //old is LRU policy or Random Choice
						L1.sets[L1ind].blocks[old1].tag = tag1;
						L1.sets[L1ind].blocks[old1].rank = 0;
						if(L1.sets[L1ind].blocks[old1].dirty == 1){
							dirty1 += 1;
						}
						else{
							clean1 += 1;
						}
						
						L1.sets[L1ind].blocks[old1].dirty = 1;
						for(int i = 0; i < L1.associativity; i++){
							if(L1.sets[L1ind].blocks[i].valid == 0){
							}
							else{
								L1.sets[L1ind].blocks[i].rank += 1;
							}	
						}
					}
					else{
						int old1;
						if(rand == 0){//LRU
							old1 = findMax(L1.sets[L1ind]);
						}
						else{//Random
							old1 = getRandom(L1.associativity);
						}
						int old2;
						if(rand == 0){//LRU
							old2 = findMax(L2.sets[L2ind]);
						}
						else{//Random
							old2 = getRandom(L2.associativity);
						}
						L2.sets[L2ind].blocks[old2].valid = 1; //old is LRU policy or Random Choice
						L2.sets[L2ind].blocks[old2].tag = tag2;
						L2.sets[L2ind].blocks[old2].rank = 0;
						L1.sets[L1ind].blocks[old1].valid = 1; //old is LRU policy or Random Choice
						L1.sets[L1ind].blocks[old1].tag = tag1;
						L2.sets[L1ind].blocks[old1].rank = 0;
						
						if(L1.sets[L1ind].blocks[old1].dirty == 1){
							dirty1 += 1;
							L1.sets[L1ind].blocks[old1].dirty = 0;
						}
						else{
							clean1 += 1;
						}
						if(L2.sets[L2ind].blocks[old2].dirty == 1){
							dirty2 += 1;
							L2.sets[L2ind].blocks[old2].dirty = 0;
						}
						else{
							clean2 += 1;
						}

						
						L1.sets[L1ind].blocks[old1].dirty = 1;
							
						L2.sets[L2ind].blocks[old2].dirty = 1;
						for(int i = 0; i < L1.associativity; i++){
							if(L1.sets[L1ind].blocks[i].valid == 0){
							}
							else{
								L1.sets[L1ind].blocks[i].rank += 1;
							}	
						}
						for(int j = 0; j < L2.associativity; j++){
							if(L2.sets[L2ind].blocks[j].valid == 0){
							}
							else{
								L2.sets[L2ind].blocks[j].rank += 1;
							}	
						}
					}
				}
			}					
		}




		}

//zz
		if(word[0] == 'W'){
			Wacc += 1;
			if(find == 0){
				if(find2 == 0){
					W1miss += 1;
					W2miss += 1;	
				}
				else{
					W1miss += 1;
				}
			}
		}
		if(word[0] == 'R'){
			Racc += 1;
			if(find == 0){
				if(find2 == 0){
					R1miss += 1;
					R2miss += 1;
				}
				else{
					R1miss += 1;
				}
			}
		}
	}
	fclose(file1);

    // 통계 출력
	string workloadName = string(argv[argc-1]); // 실제 워크로드 이름으로 변경
	size_t lastDotPosition = workloadName.find_last_of('.');
   	if (lastDotPosition == std::string::npos) {
        // 점이 없으면 전체 파일 이름을 반환
    	}
	else{
    // 점의 위치 앞까지의 부분 문자열을 반환
		workloadName = workloadName.substr(0, lastDotPosition);
	}
	string fileName = workloadName + "_" + std::to_string(capa) + "_" + std::to_string(assoc) + "_" +std::to_string(blocks) + ".out";
	std::ofstream outFile(fileName);
	if (outFile.is_open()) {
        	outFile << "-- General Stats --\n";
        	outFile << "L1 Capacity: " << capa/4 << "\n";
        	outFile << "L1 way: " << L1.associativity << "\n";
		outFile << "L2 Capacity: " << capa << "\n";
        	outFile << "L2 way: " << L2.associativity << "\n";
        	outFile << "Block Size: " << blocks << "\n";
        	outFile << "Total accesses: " << Racc+Wacc << "\n";
        	outFile << "Read accesses: " << Racc << "\n";
        	outFile << "Write accesses: " << Wacc << "\n";
        	outFile << "L1 Read misses: " << R1miss << "\n";
        	outFile << "L2 Read misses: " << R2miss << "\n";
        	outFile << "L1 Write misses: " << W1miss << "\n";
        	outFile << "L2 Write misses: " << W2miss << "\n";
        	outFile << "L1 Read miss rate: " << std::fixed << std::setprecision(2) << (Racc ? (static_cast<double>(R1miss) / Racc) * 100 : 0) << "%\n";
        	outFile << "L2 Read miss rate: " << std::fixed << std::setprecision(2) << (R1miss ? (static_cast<double>(R2miss) / R1miss) * 100 : 0) << "%\n";
        	outFile << "L1 Write miss rate: " << std::fixed << std::setprecision(2) << (W1miss ? (static_cast<double>(W1miss) / Wacc) * 100 : 0) << "%\n";
        	outFile << "L2 Write miss rate: " << std::fixed << std::setprecision(2) << (W2miss ? (static_cast<double>(W2miss) / W1miss) * 100 : 0) << "%\n";
        	outFile << "L1 Clean eviction: " << clean1 << "\n";
        	outFile << "L2 clean eviction: " << clean2 << "\n";
        	outFile << "L1 dirty eviction: " << dirty1 << "\n";
        	outFile << "L2 dirty eviction: " << dirty2 << "\n";
        	outFile.close();
	} else {
        	std::cerr << "Unable to open file";
	}


	return 0;

}
