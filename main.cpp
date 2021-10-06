/******************************************************************************

Cynthia Rios 

The goal of this project is to design and implement a C/C++ cycle-accurate simulator of a 5-stage LC3-like integer pipelined processor.

Upcoming Work 
- Accepting text files
- Adding code for more operations
- Incorporate PC and ctrl operations
- Skip memory step if ctrl operation 
- Code for if certain steps take more time (ex, memory takes 3 clock cycles)
- Code for more hazards
- How to handle hazards with global vars instead of class vars
- Refactoring 

*******************************************************************************/

#include <iostream>
#include <queue>
#include <thread>

using namespace std;

int REG[7];

class process{
    public: 
        int stage = 0; 
        long int inst; 
        int opcode; 
        int src_1 = -1; 
        int src_2 = -1; 
        int dst = -1; 
        int val; 
        char opcode_type[3]; 
        bool fetch_done = false; 
        bool decode_done = false; 
        bool ex_done = false; 
        bool mem_done = false; 
        bool wb_done = false; 
        bool raw = false; 
        /*
        RAW: instruction refers to a result that has not yet been calculated or retrieved
             - src param cannot be used until dest WB
             - assuming one pipeline: only must check with next thread
        */
        bool war = false; 
        
        void run_inst(){
            printf("INST: %lu\n", inst); 
            if(raw){
                printf("bubble\n"); 
                stage = stage - 1; 
            }
            else if(!fetch_done){
                fetch(); 
                fetch_done = true; 
            }
            else if(!decode_done){
                decode(); 
                decode_done = true; 
            }
            else if(ex_done){
                ex(); 
                ex_done = true; 
            }
            else if(mem_done){
                mem(); 
                mem_done = true; 
            }
            stage = stage + 1; 
            printf("- stage: %d\n", stage); 
        }
        void fetch(){
            opcode = inst>>12 & 15;
            if(opcode == 1 || opcode == 9){
                opcode_type == "ALU";
            }
            // is there an equivilant to "in" 
            else if(opcode == 2 || opcode == 10 || opcode == 6 || opcode == 13 || opcode == 3 || opcode == 11){
                opcode_type == "MEM";
            }
            else if(opcode == 0 || opcode == 12){
                opcode_type == "CTR";
            }
            else{
                opcode_type == "INV"; 
            }
        }
        void decode(){ 
            // assume add with 2 registers
            if(opcode == 1){
                dst = (inst>>9&7)-1; 
                src_1 = (inst>>6&7)-1;  
                src_2 = (inst&7)-1;   
            }
            else if(opcode == 3){
                src_1 = (inst>>9&7)-1; 
            }
            
        }
        void ex(){  
            switch(opcode) {
                case 1:
                    val = REG[src_1] + REG[src_2];
            
            }
        }
        
        void mem(){
            REG[dst] = val;
        }
        
        void wb(){
        }

}; 

void run_prog(int& clk, vector<process> &vectorOfProcesses){
    if(!vectorOfProcesses.empty()){
        vectorOfProcesses.at(0).run_inst(); 
        clk = clk + 1; 
    }
    while(!vectorOfProcesses.empty()){
        printf("------------------\n"); 
        std::vector<std::thread> threads;
        for(int i = 0; i <= vectorOfProcesses.begin()->stage; i++) {
            // bubble check: raw: src param cannot be used until dest WB
            if(vectorOfProcesses.size() > i+1){ // if there is a next item
                if(vectorOfProcesses.at(i).dst == vectorOfProcesses.at(i+1).src_1 || vectorOfProcesses.at(i).dst == vectorOfProcesses.at(i+1).src_2){
                    if(vectorOfProcesses.at(i).stage > 2){
                       vectorOfProcesses.at(i+1).raw = true; 
                    }
                }
                else{
                    vectorOfProcesses.at(i+1).raw = false; 
                }
            }
            threads.push_back(std::thread(&process::run_inst, &vectorOfProcesses.at(i)));
            if(vectorOfProcesses.size()==threads.size()){
                break; 
            }
        }
        for (auto &th : threads) {
            th.join(); 
        }
        if(vectorOfProcesses.begin()->stage == 5){
            if(vectorOfProcesses.size() >= 2){
                if(vectorOfProcesses.at(1).raw && vectorOfProcesses.at(0).dst == vectorOfProcesses.at(1).src_1 || vectorOfProcesses.at(0).dst == vectorOfProcesses.at(1).src_2){ // if there is a next item
                    vectorOfProcesses.at(1).raw = false; 
                }
            }
            vectorOfProcesses.erase(vectorOfProcesses.begin());
            //~curr_process; 
        }
        clk = clk + 1; 
    }
    printf("clk: %d\n", clk); 
}

int main()
{
    int clk = 0;
    std::queue<long int> inst_from_file;
    // need file i/o 
    int inst_1 = std::stoi("0001010010000001", nullptr, 2);
    int inst_2 = std::stoi("0011010010000001", nullptr, 2);
    inst_from_file.push(inst_1);
    inst_from_file.push(inst_2);
    
    long int curr_inst; 
    // need to add instructions to queue
    // I probably don't need a queue AND a vector

    vector<process> vectorOfProcesses;
    while(!inst_from_file.empty()){
        process newProcessObject;
        newProcessObject.inst = inst_from_file.front(); 
        vectorOfProcesses.push_back(newProcessObject);
        inst_from_file.pop(); 
    }
    run_prog(clk, vectorOfProcesses); 
    
    return 0;
}
