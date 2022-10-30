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
string H, T, E;
string fileName;
vector<string> line;
vector<string> opcode;
vector<string> tRecord;
stringstream ss;

void InitOpcode() {
    in.open("OPCODE.txt", ios::in);
    while (in.getline(c, 100)) {
        opcode.push_back(c);
    }
    in.close();
}

string BitExtension(string h) {
    string zero;
    int left = 0;
    if (h.size() < 6) left = 6 - h.size();
    while (left--)  {
        zero += '0';
    }
    zero += h;
    return zero;
}


void InitText() {
    T.clear();
    line[0] = BitExtension(line[0]);
    T = "T^" + line[0] + '^';
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
    if (line.empty()) return false;
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

void ByteObjCode() {
    OPERAND = line.back();
    char type = OPERAND[0];
    string temp;
    
    if (type == 'C') {
        temp.clear();
        string TR;
        TR.clear();
        for (int i = 1; i < OPERAND.size(); i++) {
            if (OPERAND[i] == '\'') continue;
            else {
                int ASCII = OPERAND[i];
                ss.clear();
                ss << hex << ASCII;
                ss >> temp;
                TR += temp;
            }
        }
        tRecord.push_back(TR);
    }
    else if (type == 'X') {
        temp.clear();
        for (int i = 1; i < OPERAND.size(); i++) {
            if (OPERAND[i] == '\'') continue;
            else {
                temp += OPERAND[i];
            }
        }
        if (tRecord.empty()) InitText();
        tRecord.push_back(temp);
    }
}

void WordObjCode() {
    int val = GetOperandValue();
    string s;
    ss.clear();
    ss << hex << val;
    ss >> s;
    s = BitExtension(s);
    if (tRecord.empty()) InitText();
    tRecord.push_back(s);
}

void GetProgramLength() {
    ss.clear();
    string s;
    int LCTCTR, starAdd;
    int result;
    ss << hex << LOCCTR;
    ss >> LCTCTR;
    ss.clear();
    ss << hex << startingAddress;
    ss >> starAdd;
    ss.clear();
    ss << hex << (LCTCTR - starAdd);
    ss >> programLength;
}

string BinToHex(string x) {
    string h;
    int base = 1;
    int sum = 0;
    for (int i = x.size() - 1; i >= 0; i--) {
        sum += (x[i] - '0') * base;
        base *= 2;
    }
    h.clear();
    if (sum >= 10) {
        h = (sum - 10) + 'A';
    }
    else {
        h = sum + '0';
    }
    return h;
}

void ObjCodeCal() {
    OPERAND = line.back();
    int n = OPERAND.size();
    string x;
    x.clear();
    if (OPERAND[n - 2] == ',' && OPERAND[n - 1] == 'X') {
        x = "1";
        OPERAND.resize(n - 2);
    }
    else x = "0";
    string Add = SYMTAB[OPERAND];
    bitset<3> b(Add[0]);
    x += b.to_string();
    x = BinToHex(x);
    string temp;
    temp = OPTAB[OPCODE] + x + Add[1] + Add[2] + Add[3];
    if (tRecord.empty()) InitText();
    tRecord.push_back(temp);
}

void WriteTextRecord() {
    int bytecnt = 0;
    string temp;
    for (auto i : tRecord) {
        if (i.size() < 6) {
            bytecnt++;
        }
    }
    int length = (3 * (tRecord.size() - bytecnt)) + bytecnt;
    ss.clear();
    ss << hex << length;
    temp.clear();
    ss >> temp;

    if (temp.size() < 2) {
        temp = '0' + temp;
    }

    T += temp;
    for (auto i : tRecord) {
        T += '^' + i;
    }
    transform(T.begin(), T.end(), T.begin(), ::toupper);
    out << T << '\n';
    tRecord.clear();
}

void Pass1() {
    cin >> fileName;
    in.open(fileName, ios::in);
    out.open("intermediateFile.txt", ios::out);

    // START
    in.getline(c, 100);
    GetNewLine(c);
    OPCODE = GetOPCODE();
    
    if (OPCODE == "START") {
        LABLE = line[0];
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

void Pass2() {
    in.open("intermediateFile.txt", ios::in);
    out.open(fileName + ".obj", ios::out);

    // START
    in.getline(c, 100);
    GetNewLine(c);
    OPCODE = GetOPCODE();
    if (OPCODE == "START") {
        LABLE = line[1];
        startingAddress = line[3];
        in.getline(c, 100);
        GetNewLine(c);
        OPCODE = GetOPCODE();
    }
    startingAddress = BitExtension(startingAddress);
    H.clear();
    programLength = BitExtension(programLength);
    H = "H^" + LABLE + "  " + '^' + startingAddress + '^' + programLength;
    transform(H.begin(), H.end(), H.begin(), ::toupper);
    out << H << '\n';
    InitText();

    while (OPCODE != "END") {
        string test = OPCODE;
        if (!IsComment()) {
            if (OPCODE == "BYTE" || OPCODE == "WORD") {
                if (OPCODE == "BYTE") {
                    ByteObjCode();
                }
                else {
                    WordObjCode();
                }
            }
            else if (OPCODE == "RESW" || OPCODE == "RESB"){
                if (!tRecord.empty()) {
                    WriteTextRecord();
                }
            }
            else {
                if (line.size() > 2) {
                    ObjCodeCal();
                }
                else {
                    string op = OPTAB[OPCODE];
                    string Add = "0000";
                    op += Add;
                    tRecord.push_back(op);
                }
            }
            if (tRecord.size() == 10) {
                WriteTextRecord();
            }
        }
        in.getline(c, 100);
        GetNewLine(c);
        OPCODE = GetOPCODE();
    }
    if (!tRecord.empty())
        WriteTextRecord();
    E.clear();
    E = "E^" + startingAddress;
    transform(E.begin(), E.end(), E.begin(), ::toupper);
    out << E << '\n';

    in.close();
    out.close();
}

int main() {
    BuildOPTAB();
    InitOpcode();
    Pass1();
    Pass2();
    return 0;
}