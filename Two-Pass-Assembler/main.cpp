#include <bits/stdc++.h>
using namespace std;

map<string, string> OPTAB;
map<string, string> SYMTAB;
ifstream in;
ofstream out;
char c[100];
string LOCCTR;
string LABLE;
string OPCODE;
string OPERAND;
string startingAddress;
string programLength;
vector<string> line;
vector<string> opcode;
stringstream ss;

void InitOpcode() {
    in.open("OPCODE.txt", ios::in);
    while (in.getline(c, 100)) {
        opcode.push_back(c);
    }
    in.close();
}

void BuildOPTAB() {
    string key, value;
    in.open("OPTAB.txt", ios::in);
    while (in.getline(c, 100)) {
        ss.clear();
        ss << c;
        ss >> key >> value;
        OPTAB[key] = value;
    }
    in.close();
}

void GetNewLine(char c[100]) {
    line.clear();
    ss.clear();
    ss << c;
    string s;
    while (ss >> s) {
        line.push_back(s);
    }
}

string GetOPCODE() {
    for (auto i : line) {
        for (auto u : opcode) {
            if (i == u) return i;
        }
    }
    return "\t";
}

bool IsComment() {
    if (line[0][0] == '.') return true;
    else return false;
}

void SetLOCCTR(int val) {
    ss.clear();
    int newLOCCTR;
    ss << hex << LOCCTR;
    ss >> newLOCCTR;
    newLOCCTR += val;
    ss.clear();
    ss << newLOCCTR;
    ss >> hex >> LOCCTR;
}

int GetOperandValue() {
    int val;
    OPERAND = line.back();
    ss.clear();
    ss << OPERAND;
    ss >> dec >> val;
    return val;
}

int GetByteOperandVal() {
    OPERAND = line.back();
    char type = OPERAND[0];
    string temp;
    temp.clear();
    if (type == 'C') {
        for (int i = 1; i < OPERAND.size(); i++) {
            if (OPERAND[i] == '\'') continue;
            temp += OPERAND[i];
        }
        return temp.size();
    }
    else if (type == 'X') {
        for (int i = 1; i < OPERAND.size(); i++) {
            if (OPERAND[i] == '\'') continue;
            temp += OPERAND[i];
        }
        return (temp.size() * 4) / 8;
    }
    return 0;
}

void GetProgramLength() {
    ss.clear();
    int LCTCTR, starAdd;
    ss << hex << LOCCTR;
    ss >> LCTCTR;
    ss.clear();
    ss << hex << startingAddress;
    ss >> starAdd;
    ss << hex << (LCTCTR - starAdd);
    ss >> programLength;
}

void Pass1() {
    in.open("sourceProgram.txt", ios::in);
    out.open("intermediateFile.txt", ios::out);

    // START
    in.getline(c, 100);
    GetNewLine(c);
    OPCODE = GetOPCODE();
    if (OPCODE == "START") {
        OPERAND = line.back();
        LOCCTR = line.back();
        startingAddress = LOCCTR;
        out << LOCCTR << '\t' << LABLE << '\t' << OPCODE << '\t' << OPERAND << '\n';
    }
    else LOCCTR = "0000";

    in.getline(c, 100);
    GetNewLine(c);
    OPCODE = GetOPCODE();
    while (OPCODE != "END") {
        if (!IsComment()) {
            if (line.size() == 3) {
                LABLE = line[0];
                SYMTAB[LABLE] = LOCCTR;
            }
            else LABLE = '\t';
            out << LOCCTR << '\t';
            if (OPCODE == "WORD") {
                OPERAND = line.back();
                SetLOCCTR(3);
            }
            else if (OPCODE == "RESW") {
                int val = GetOperandValue();
                SetLOCCTR(val * 3);
            }
            else if (OPCODE == "RESB") {
                int val = GetOperandValue();
                SetLOCCTR(val);
            }
            else if (OPCODE == "BYTE") {
                int val = GetByteOperandVal();
                SetLOCCTR(val);
            }
            else {
                if (line.size() > 1) {
                    OPERAND = line.back();
                }
                else {
                    OPERAND = '\t';
                }
                SetLOCCTR(3);
            }
            out << LABLE << '\t' << OPCODE << '\t' << OPERAND << '\n';
        }
        else {
            out << line[0] << '\n';
        }
        in.getline(c, 100);
        GetNewLine(c);
        OPCODE = GetOPCODE();
    }
    OPERAND = line.back();
    out << LOCCTR << '\t' << OPCODE << '\t' << OPERAND;
    GetProgramLength();
    in.close();
    out.close();
}

int main() {
    BuildOPTAB();
    InitOpcode();
    Pass1();
    return 0;
}