#include<bits/stdc++.h>

using namespace std;

unordered_map<string, string> registers = {
    {"x0", "00000"}, {"x1", "00001"}, {"x2", "00010"}, {"x3", "00011"},
    {"x4", "00100"}, {"x5", "00101"}, {"x6", "00110"}, {"x7", "00111"},
    {"x8", "01000"}, {"x9", "01001"}, {"x10", "01010"}, {"x11", "01011"},
    {"x12", "01100"}, {"x13", "01101"}, {"x14", "01110"}, {"x15", "01111"},
    {"x16", "10000"}, {"x17", "10001"}, {"x18", "10010"}, {"x19", "10011"},
    {"x20", "10100"}, {"x21", "10101"}, {"x22", "10110"}, {"x23", "10111"},
    {"x24", "11000"}, {"x25", "11001"}, {"x26", "11010"}, {"x27", "11011"},
    {"x28", "11100"}, {"x29", "11101"}, {"x30", "11110"}, {"x31", "11111"}
};

unordered_map<string, pair<string, string>> R_type_ops = {
    {"ADD", {"0000000", "000"}}, 
    {"SUB", {"0100000", "000"}}, 
    {"AND", {"0000000", "111"}}, 
    {"OR",  {"0000000", "110"}}  
};

unordered_map<string, pair<string, string>> I_type_ops = {
    {"ADDI", {"0010011", "000"}}, 
    {"LW",   {"0000011", "010"}}
};

unordered_map<string, pair<string, string>> S_type_ops = {
    {"SW", {"0100011", "010"}}
};

unordered_map<string,string>U_type_ops = {
    {"LUI", "0110111"},
    {"AUIPC", "0010111"}
};

unordered_map<string, string>B_type_ops = {
    {"BEQ", "1100011"},
    {"BNE", "1100011"}
};

unordered_map<string,string> J_type_ops = {
    {"JAL", "1101111"}
};



string int_to_binary(int deci){
    if(deci == 0) return "0";

    string binary= "";
    while(deci > 0){
        binary += to_string(deci % 2);
        deci /=2;
    }
    for(int i = binary.size(); i < 12; i++){
        binary = "0" + binary;
    }
    return binary;
}



string binarytoHex(const string& binary){
    if(binary.length() != 32){
        return "invalid input size . The size should be 32";
    }
    unsigned int decimal = bitset<32>(binary).to_ulong();

    stringstream ss;
    ss << hex << uppercase<< decimal;
    return "0x" + ss.str();
}

vector<string> parse(string int_reg){
    string num_str = "";
    string reg_str = "";

    bool flag = false;

    for(int i= 0 ; i< int_reg.size(); i++){
        if(flag == false && int_reg[i] != '(') num_str += int_reg[i];
        else{
            if(int_reg[i] == '(') {
                flag = true;
                continue;}
            if(int_reg[i] == ')')
            break;
            reg_str += int_reg[i];
        }

    }
    vector<string> vec = {num_str, reg_str};
    //cout<<vec[0]<<" "<<vec[1]<<endl;
    return vec;

}

string conv_r_type(string& op , string& rd, string& rs1, string& rs2){
    string func7 = R_type_ops[op].first;
    string func3 = R_type_ops[op].second;
    return func7 + registers[rs2] + registers[rs1] + func3 + registers[rd] + "0110011";
}

string conv_i_type(string& op, string& rd, string& rs1, string& imm){
    string opcode = I_type_ops[op].first;
    string func3 = I_type_ops[op].second;
    return imm + registers[rs1] + func3 + registers[rd] +opcode;
}

string conv_s_type(string& op, string& rs1, string& imm, string& mem){
    string opcode = S_type_ops[op].first;
    string func3 = S_type_ops[op].second;
    int offset = atoi(imm.c_str());
    imm = bitset<32>(offset).to_string().substr(20);

    cout<< registers[mem] <<" "<< registers[rs1] <<" "<< imm.substr(0, 12)<<endl;
    return imm.substr(0, 7) + registers[mem] + registers[rs1] + func3 + imm.substr(7, 5) + opcode;

}


string conv_u_type(string& op, string& rd, string& imm){
    string opcode = U_type_ops[op];
    return  imm + registers[rd] + opcode;
}

string conv_b_type(string& op, string& rs1, string& rs2, string& imm){
    string opcode = B_type_ops[op];
    string func3 = (op == "BEQ") ? "000" : "001";
    return imm[0] + imm.substr(2, 6) + registers[rs2] + registers[rs1] + func3 + imm.substr(8, 4) + imm[1] + opcode;
}

string conv_j_type(string& op , string& rd, string& imm){
    string opcode = J_type_ops[op];
    return imm + imm.substr(10, 8) + imm[9] + imm.substr(1, 8) + registers[rd] + opcode;
}
void parse_instruction(string& sample_instruction){
    istringstream ss(sample_instruction);
    string opcode, dest, src1, src2, imm;

    ss >> opcode;

    if(R_type_ops.find(opcode) != R_type_ops.end()) {

        ss >> dest >> src1 >> src2;

        dest = dest.substr(0, dest.size()-1);
        src1 = src1.substr(0, src1.size()-1);

        cout << "Operation: " << opcode << endl;
        cout << "Destination: " << dest << endl;
        cout << "Source 1: " << src1 << endl;
        cout << "Source 2: " << src2 << endl; 

        cout<< "Instruction Type :- R-type"<<endl;
        string r_binary = conv_r_type(opcode, dest, src1, src2);
        cout<<"Binary Instruction: "<< r_binary<<endl;
        cout<<"Hexa representation is : "<<binarytoHex(r_binary);
    }

    else if(I_type_ops.find(opcode) != I_type_ops.end()){

        ss >> dest >> src1 >> src2;

        dest = dest.substr(0, dest.size()-1);
        src1 = src1.substr(0, src1.size()-1);

        cout << "Operation: " << opcode << endl;
        cout << "Destination: " << dest << endl;
        cout << "Source 1: " << src1 << endl;
        cout << "Source 2: " << src2 << endl; 

        cout<<"Instruction Type:- I-type"<<endl;
        imm = int_to_binary(stoi(src2));
        string i_binary = conv_i_type(opcode, dest, src1, imm);
        cout<<"Binary Instruction: "<<i_binary<<endl;
        cout<<"Hexa representation is : "<<binarytoHex(i_binary);
    }

    else if(S_type_ops.count(opcode)){

        ss >> dest >> src1;
        //cout<<dest<<src1<<endl;
        vector<string> mem = parse(src1);
        
        cout<< "Destination: "<< dest<<endl;
        cout<< "Source: "<<src1<<endl;

        cout<<"Instruction Type:- S-type"<<endl;
        dest = dest.substr(0, dest.size()-1);
    
        string s_binary = conv_s_type(opcode, dest, mem[0], mem[1]);
        cout<<"Binary Instruction: "<<s_binary<<endl;
        cout<<"Hexa representation is : "<<binarytoHex(s_binary);
    }

    else if(U_type_ops.count(opcode)){
        ss >> dest >> imm;
        cout<<"Instruction type:- U_type"<<endl;

        cout<<"Destination: "<<endl;
        cout<<"Immediate: "<<endl;

        int number = stoi(imm);
        bitset<19> binary(number);
        imm = binary.to_string();
        string u_binary = conv_u_type(opcode, dest, imm);
        cout<<"binary Instruction: "<<u_binary<<endl;
        cout<<"Hexa representation is : "<<binarytoHex(u_binary);
    }
    else if(B_type_ops.count(opcode)){
        cout<<"Instruction type:- B_type"<<endl;
        string b_binary = conv_b_type(opcode, src1, src2, imm);
        cout<<"binary Instruction: "<<b_binary<<endl;
    }
    else if(J_type_ops.count(opcode)){
        cout<<"Instruction type:- J_type"<<endl;
        string j_binary = conv_j_type(opcode, dest, imm);
        cout<<"Binary Instruction: "<<j_binary<<endl;
    }

    else{
        cout<<"Invalid type"<<endl;
    }


}


int main(){

    vector<string> instructsions;
    string instruct;

    while(getline(cin, instruct)){
        if(instruct.empty()) break;
        instructsions.push_back(instruct);
    }

    for(int i =0; i< instructsions.size(); i++){
        parse_instruction(instructsions[i]);
        
    }


}