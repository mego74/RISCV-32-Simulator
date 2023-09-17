#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

map<int, string> Memory;		//holds the address and values of data in the memory
vector<int> registers(32, 0);
int startingPC;
int programcounter;

//Bonus
int binaryToDecimal(string n);
void BinaryTohexa(string binaryNum);
void decToBinary(int n);
void decToHexa(int n);
string bintoHexa(string binary);
string getonehex(string fourbits);
int bintodec(string);

//Code
vector<string> readfile();
void readdatafile();
vector<string> removeemptylines(vector<string>);
vector<string> separatebranch(vector<string>);
map<string, int> getbranchlocations(vector<string>&);
void loadupperimmediate(string);
void UType(string, string&, int&);
int getregister(string);
void printvalues();
void addupperimmtopc(string);
void jumpandlink(string, map<string, int>);
void jumpandlinkr(string);
void addimmediate(string);
void IType(string, string&, string&, int&);
bool branchifequal(string, map<string, int>);
void BType(string, string&, string&, string&);
bool branchifnotequal(string, map<string, int>);
bool branchiflessthan(string, map<string, int>);
bool branchifgreaterthanoreq(string, map<string, int>);
bool branchiflessthanu(string, map<string, int>);
bool branchifgreaterorequ(string, map<string, int>);
void setlessthanimmediate(string);
void setlessthanimmediateu(string);
void xorimmediate(string);
void orimmediate(string);
void andimmediate(string);
void shiftleftlogicalimmediate(string);
void shiftrightlogicalimmediate(string);
void shiftrightarithmeticimmediate(string);
void RType(string, string&, string&, string&);
void add(string);
void subtract(string);
void shiftleftlogical(string);
void setlessthan(string);
void setlessthanu(string);
void exclusiveor(string);
void shiftrightlogical(string);
void shiftrightarithmetic(string);
void orfunc(string);
void andfunc(string);
void storeword(string ins);
void saveword(int, int);
void savehalf(int, int);
void savebyte(int, int);
string tobinary(int word);
void storehalf(string ins);
void storebyte(string ins);
void loadword(string ins);
void ldwrd(int rd, string value);
void loadbyte(string ins);
void ldbyte(int rd, string value);
void loadhalf(string ins);
void ldhalf(int rd, string value);
void loadbyteu(string ins);
void loadhalfu(string ins);
void ldbyteu(int rd, string value);
void ldhalfu(int rd, string value);
void multiply(string ins);
void divide(string ins);


int main()
{
	vector<string> instructions;	//holds instructions to be executed
	map<string, int>  branches;		//holds the name of the branch along with the location of the branch


	instructions = readfile();						//read file line by line to instructions vector

	instructions = removeemptylines(instructions);	//remove empty strings fron vector

	instructions = separatebranch(instructions);	//If there is a branch in the same line as an instruction separate
													//them into two lines for easier computation in the next step

	branches = getbranchlocations(instructions);	//remove branches from the instructions vector and 
													//add them with their location to branches map

	readdatafile();

	cout << "Please enter the program starting address: " << endl;
	cin >> programcounter;
	startingPC = programcounter;




	vector<string>::iterator it = instructions.begin();

	while (it != instructions.end())
	{
		string ins = *it;
		string type = "";
		bool flag = true;
		int i = 0;
		while (ins[i] != ' ' && flag)
		{
			type += ins[i];
			if (type == "ecall" || type == "fence" || type == "ebreak")
				flag = false;
			i++;
		}
		if (flag)
		{
			if (type == "LUI" || type == "lui") //done
			{
				loadupperimmediate(ins);
				programcounter += 4;
			}
			else if (type == "AUIPC" || type == "auipc")//done
			{
				addupperimmtopc(ins);
				programcounter += 4;
			}
			else if (type == "JAL" || type == "jal")//done
			{
				jumpandlink(ins, branches);
				int x = programcounter;
				x -= startingPC;
				x /= 4;
				it = instructions.begin() + x;
				continue;
			}
			else if (type == "JALR" || type == "jalr")//done
			{
				jumpandlinkr(ins);
				int x = programcounter;
				x -= startingPC;
				x /= 4;
				it = instructions.begin() + x;
				continue;
			}
			else if (type == "BEQ" || type == "beq")//done
			{
				if (branchifequal(ins, branches))
					programcounter += 4;
				else
				{
					int x = programcounter;
					x -= startingPC;
					x /= 4;
					it = instructions.begin() + x;
					continue;
				}
			}
			else if (type == "BNE" || type == "bne")//done
			{
				if (branchifnotequal(ins, branches))
					programcounter += 4;
				else
				{
					int x = programcounter;
					x -= startingPC;
					x /= 4;
					it = instructions.begin() + x;
					continue;
				}
			}
			else if (type == "BLT" || type == "blt")//done
			{
				if (branchiflessthan(ins, branches))
					programcounter += 4;
				else
				{
					int x = programcounter;
					x -= startingPC;
					x /= 4;
					it = instructions.begin() + x;
					continue;
				}
			}
			else if (type == "BGE" || type == "bge")//done
			{
				if (branchifgreaterthanoreq(ins, branches))
					programcounter += 4;
				else
				{
					int x = programcounter;
					x -= startingPC;
					x /= 4;
					it = instructions.begin() + x;
					continue;
				}
			}
			else if (type == "BLTU" || type == "bltu")//done
			{
				if (branchiflessthanu(ins, branches))
					programcounter += 4;
				else
				{
					int x = programcounter;
					x -= startingPC;
					x /= 4;
					it = instructions.begin() + x;
					continue;
				}
			}
			else if (type == "BGEU" || type == "bgeu")//done
			{
				if (branchifgreaterorequ(ins, branches))
					programcounter += 4;
				else
				{
					int x = programcounter;
					x -= startingPC;
					x /= 4;
					it = instructions.begin() + x;
					continue;
				}
			}
			else if (type == "LB" || type == "lb")//done
			{
				loadbyte(ins);
				programcounter += 4;
			}
			else if (type == "LH" || type == "lh")//done
			{
				loadhalf(ins);
				programcounter += 4;
			}
			else if (type == "LW" || type == "lw")//done
			{
				loadword(ins);
				programcounter += 4;
			}
			else if (type == "LBU" || type == "lbu")//done
			{
				loadbyteu(ins);
				programcounter += 4;
			}
			else if (type == "LHU" || type == "lhu")//done
			{
				loadhalfu(ins);
				programcounter += 4;
			}
			else if (type == "SB" || type == "sb")//done
			{
				storebyte(ins);
				programcounter += 4;
			}
			else if (type == "SH" || type == "sh")//done
			{
				storehalf(ins);
				programcounter += 4;
			}
			else if (type == "SW" || type == "sw")//done
			{
				storeword(ins);
				programcounter += 4;
			}
			else if (type == "ADDI" || type == "addi")//done
			{
				addimmediate(ins);
				programcounter += 4;
			}
			else if (type == "SLTI" || type == "slti")//done
			{

				setlessthanimmediate(ins);
				programcounter += 4;
			}
			else if (type == "SLTIU" || type == "sltiu")//done
			{
				setlessthanimmediateu(ins);
				programcounter += 4;
			}
			else if (type == "XORI" || type == "xori")//done
			{
				xorimmediate(ins);
				programcounter += 4;
			}
			else if (type == "ORI" || type == "ori")//done
			{
				orimmediate(ins);
				programcounter += 4;
			}
			else if (type == "ANDI" || type == "andi")//done
			{
				andimmediate(ins);
				programcounter += 4;
			}
			else if (type == "SLLI" || type == "slli")//done
			{
				shiftleftlogicalimmediate(ins);
				programcounter += 4;
			}
			else if (type == "SRLI" || type == "srli")//done
			{
				shiftrightlogicalimmediate(ins);
				programcounter += 4;
			}
			else if (type == "SRAI" || type == "srai")//done
			{
				shiftrightarithmeticimmediate(ins);
				programcounter += 4;
			}
			else if (type == "ADD" || type == "add")//done
			{
				add(ins);
				programcounter += 4;
			}
			else if (type == "SUB" || type == "sub")//done
			{
				subtract(ins);
				programcounter += 4;
			}
			else if (type == "SLL" || type == "sll")//done
			{
				shiftleftlogical(ins);
				programcounter += 4;
			}
			else if (type == "SLT" || type == "slt")//done
			{
				setlessthan(ins);
				programcounter += 4;

			}
			else if (type == "SLTU" || type == "sltu")//done
			{
				setlessthanu(ins);
				programcounter += 4;
			}
			else if (type == "XOR" || type == "xor")//done
			{
				exclusiveor(ins);
				programcounter += 4;
			}
			else if (type == "SRL" || type == "srl")//done
			{
				shiftrightlogical(ins);
				programcounter += 4;
			}
			else if (type == "SRA" || type == "sra")//done
			{
				shiftrightarithmetic(ins);
				programcounter += 4;
			}
			else if (type == "OR" || type == "or")//done
			{
				orfunc(ins);
				programcounter += 4;
			}
			else if (type == "AND" || type == "and")//done
			{
				andfunc(ins);
				programcounter += 4;
			}
			else if (type == "MUL" || type == "mul")//done
			{
				multiply(ins);
				programcounter += 4;
			}
			else if (type == "DIV" || type == "div")//done
			{
				divide(ins);
				programcounter += 4;
			}
			else
			{
				cout << "Unidentified instruction " << ins << " Exiting program";
				exit(0);
			}

		}
		else
		{
			break;
		}
		it++;
	}

	return 0;
}



vector<string> readfile()
{
	ifstream in;
	in.open("D://Assebmly Project//Project.txt");
	vector<string> instructions;
	string x;
	while (!in.eof())
	{
		getline(in, x);
		instructions.push_back(x);
	}
	return instructions;
}

void readdatafile()
{
	ifstream in;
	in.open("D://Assebmly Project//Memory.txt");

	vector<int> values;
	vector<int> addresses;
	int v = -11331, a = -11331;
	while (!in.eof())
	{
		in >> v >> a;
		values.push_back(v);
		addresses.push_back(a);
	}
	if (v != -11331 && a != 11331)
	{
		vector<string> valuesinbinary;
		for (auto y : values)
		{
			string bin = tobinary(y);
			int x = bin.size();
			string save;
			if (bin[0] == '0')
			{
				save = "00000000000000000000000000000000";
			}
			else
			{
				save = "11111111111111111111111111111111";
			}
			for (int i = 0; i < x; i++)
			{
				save[31 - i] = bin[x - i - 1];
			}
			valuesinbinary.push_back(save);
		}
		for (int i = 0; i < valuesinbinary.size(); i++)
		{
			Memory[addresses[i]] = valuesinbinary[i];
		}
	}


	in.close();
}

vector<string> removeemptylines(vector<string> instructions)
{
	vector<string> temp = instructions;
	instructions.clear();
	for (int i = 0; i < temp.size(); i++)
	{
		if (temp[i] != "")
			instructions.push_back(temp[i]);
	}
	return instructions;
}

vector<string> separatebranch(vector<string> instructions)
{
	vector<string> temp = instructions;
	instructions.clear();
	string s = "", x;
	string branch = "";
	for (int i = 0; i < temp.size(); i++)
	{
		if (i > 0)
		{
			if (s != "")
				instructions.push_back(s);

		}
		s = "";
		branch = "";

		x = temp[i];

		bool flag = false, flag2 = false;
		for (int j = 0; j < x.size(); j++)
		{

			if (flag)
			{
				if (flag2)
				{
					s += x[j];
				}
				else
				{
					if (x[j] == ' ')
						continue;
					else
					{
						s += x[j];
						flag2 = true;
					}
				}
			}
			else
			{
				if (x[j] == ':')
				{
					instructions.push_back(branch + ':');
					flag = true;
					s = "";
				}
				else
				{

					branch += x[j];
					s += x[j];
				}
			}

		}
	}
	if (s != "")
		instructions.push_back(s);
	return instructions;
}

map<string, int> getbranchlocations(vector<string>& instructions)
{
	string s = "", branch = "", x;
	int noofbranches = 0;
	map<string, int>  branches;
	vector<string> temp = instructions;
	instructions.clear();
	for (int i = 0; i < temp.size(); i++)
	{
		bool flag = false;
		string branch = "";
		x = temp[i];
		for (int j = 0; j < x.size(); j++)
		{
			if (x[j] == ':')
			{
				branches[branch] = i - noofbranches;
				noofbranches += 1;
				flag = true;
			}
			else
				branch += x[j];
		}
		if (!flag)
			instructions.push_back(x);
	}
	return branches;
}

void loadupperimmediate(string ins)
{
	string temp = ins;
	ins = "";
	int imm, r;
	string rd;
	for (int i = 3; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	UType(ins, rd, imm);

	r = getregister(rd);

	int num = imm;
	num = num << 12;

	registers[r] = num;

	printvalues();

}

void UType(string ins, string& rd, int& imm)
{
	bool flag = false;
	string dest, im;
	for (auto x : ins)
	{
		if (flag)
		{
			if (im == "")
			{
				if (x == ' ' || x == ',')
					continue;
				else
					im += x;
			}
			else
			{
				if (x == ' ')
					break;
				else
					im += x;
			}
		}
		else
		{
			if (x == ' ' || x == ',')
			{
				flag = true;
				rd = dest;
			}
			else
				dest += x;
		}
	}

	imm = stoi(im);
}

int getregister(string reg)
{
	if (reg == "zero" || reg == "x0")
		return 0;
	if (reg == "ra" || reg == "x1")
		return 1;
	if (reg == "sp" || reg == "x2")
		return 2;
	if (reg == "gp" || reg == "x3")
		return 3;
	if (reg == "tp" || reg == "x4")
		return 4;
	if (reg == "t0" || reg == "x5")
		return 5;
	if (reg == "t1" || reg == "x6")
		return 6;
	if (reg == "t2" || reg == "x7")
		return 7;
	if (reg == "s0" || reg == "x8")
		return 8;
	if (reg == "s1" || reg == "x9")
		return 9;
	if (reg == "a0" || reg == "x10")
		return 10;
	if (reg == "a1" || reg == "x11")
		return 11;
	if (reg == "a2" || reg == "x12")
		return 12;
	if (reg == "a3" || reg == "x13")
		return 13;
	if (reg == "a4" || reg == "x14")
		return 14;
	if (reg == "a5" || reg == "x15")
		return 15;
	if (reg == "a6" || reg == "x16")
		return 16;
	if (reg == "a7" || reg == "x17")
		return 17;
	if (reg == "s2" || reg == "x18")
		return 18;
	if (reg == "s3" || reg == "x19")
		return 19;
	if (reg == "s4" || reg == "x20")
		return 20;
	if (reg == "s5" || reg == "x21")
		return 21;
	if (reg == "s6" || reg == "x22")
		return 22;
	if (reg == "s7" || reg == "x23")
		return 23;
	if (reg == "s8" || reg == "x24")
		return 24;
	if (reg == "s9" || reg == "x25")
		return 25;
	if (reg == "s10" || reg == "x26")
		return 26;
	if (reg == "s11" || reg == "x27")
		return 27;
	if (reg == "t3" || reg == "x28")
		return 28;
	if (reg == "t4" || reg == "x29")
		return 29;
	if (reg == "t5" || reg == "x30")
		return 30;
	if (reg == "t6" || reg == "x31")
		return 31;
}

void printvalues()
{
	cout << "Program counter: " << programcounter << endl;
	for (int i = 0; i < 32; i++)
	{
		cout << "Register x" << i << ": " << registers[i] << endl;

		string bin = tobinary(registers[i]), binary;

		if (bin[0] == '0')
		{
			binary = "00000000000000000000000000000000";
		}
		else
		{
			binary = "11111111111111111111111111111111";
		}
		for (int i = 0; i < bin.size(); i++)
		{
			binary[31 - i] = bin[bin.size() - i - 1];
		}
		cout << "The binary value is: " << binary << endl;

		string hexa = bintoHexa(binary);
		cout << "The Hexa value is: 0x" << hexa << endl << endl;
	}
	cout << endl;
	cout << "Memory:" << endl;
	for (auto x : Memory)
	{
		cout << "Location: " << x.first << ", Value: " << x.second << endl;
		string hexa = bintoHexa(x.second);
		int decimal = bintodec(x.second);
		cout << "The Hexa value is: 0x" << hexa << endl;
		cout << "The decimal value is: " << decimal << endl << endl;
	}
	cout << endl;

}


void addupperimmtopc(string ins)
{
	string temp = ins;
	ins = "";
	int imm, r;
	string rd;
	for (int i = 5; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	UType(ins, rd, imm);

	r = getregister(rd);

	int num = imm;
	num = num << 12;

	num += programcounter;
	registers[r] = num;

	printvalues();
}

void jumpandlink(string ins, map<string, int> branches)
{
	string temp = ins;
	ins = "";
	string rd, label;
	for (int i = 3; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	bool flag = false;
	string dest, im = "";
	for (auto x : ins)
	{
		if (flag)
		{
			if (im == "")
			{
				if (x == ' ' || x == ',')
					continue;
				else
					im += x;
			}
			else
			{
				if (x == ' ')
					break;
				else
					im += x;
			}
		}
		else
		{
			if (x == ' ' || x == ',')
			{
				flag = true;
				rd = dest;
			}
			else
				dest += x;
		}
	}

	int r = getregister(rd);
	int place = branches[im];
	place = place * 4 + startingPC;
	int currentplace = programcounter + 4;

	if (r != 0)
		registers[r] = currentplace;
	programcounter = place;

	printvalues();
}

void jumpandlinkr(string ins)
{
	string temp = ins;
	ins = "";
	string rd, rs1, imm;

	for (int i = 4; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	bool isrd = false, isimm = false;
	string dest, im = "", placetogo = "";
	for (auto x : ins)
	{
		if (isrd)
		{
			if (isimm)
			{
				if (placetogo == "")
				{
					if (x == ' ' || x == ',')
						continue;
					else if (x == ')')
					{
						rs1 = placetogo;
						break;
					}
					else
						placetogo += x;
				}
				else
				{
					if (x == ' ' || x == ',')
						continue;
					else if (x == ')')
					{
						rs1 = placetogo;
						break;
					}
					else
						placetogo += x;
				}
			}
			else
			{
				if (im == "")
				{
					if (x == ' ' || x == ',')
						continue;
					else
						im += x;
				}
				else
				{
					if (x == '(')
					{
						isimm = true;
						imm = im;
					}
					else
						im += x;
				}
			}

		}
		else
		{
			if (x == ' ' || x == ',')
			{
				isrd = true;
				rd = dest;
			}
			else
				dest += x;
		}
	}


	int r = getregister(rd);
	int r1 = getregister(rs1);
	int offset = stoi(imm);
	int place = registers[r1] + offset;
	int currentplace = programcounter + 4;

	if (r != 0)
		registers[r] = currentplace;
	programcounter = place;

	printvalues();
}

void addimmediate(string ins)
{
	string temp = ins;
	ins = "";
	int imm, r;
	string rd, rs1;
	for (int i = 4; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	IType(ins, rd, rs1, imm);

	r = getregister(rd);
	int r1 = getregister(rs1);

	if (r != 0)
		registers[r] = registers[r1] + imm;

	printvalues();
}

void IType(string ins, string& rd, string& rs1, int& imm)
{
	bool isdest = false, issource = false;
	string dest, source = "", im = "";
	for (auto x : ins)
	{
		if (isdest)
		{
			if (issource)
			{
				if (im == "")
				{
					if (x == ' ' || x == ',')
						continue;
					else
						im += x;
				}
				else
				{
					if (x == ' ')
						break;
					else
						im += x;
				}
			}
			else
			{
				if (source == "")
				{
					if (x == ' ' || x == ',')
						continue;
					else
						source += x;
				}
				else
				{
					if (x == ' ' || x == ',')
					{
						issource = true;
						rs1 = source;
					}
					else
						source += x;
				}
			}
		}
		else
		{
			if (x == ' ' || x == ',')
			{
				isdest = true;
				rd = dest;
			}
			else
				dest += x;
		}
	}

	imm = stoi(im);

}

bool branchifequal(string ins, map<string, int> branches)
{
	string temp = ins;
	ins = "";
	int r;
	string rd, rs1, label;
	for (int i = 3; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	BType(ins, rd, rs1, label);

	r = getregister(rd);
	int r1 = getregister(rs1);

	if (registers[r] == registers[r1])
	{
		int place = branches[label];
		place = place * 4 + startingPC;
		programcounter = place;
		printvalues();
		return false;
	}
	else
	{
		printvalues();
		return true;
	}



}

void BType(string ins, string& rd, string& rs1, string& label)
{
	bool isdest = false, issource = false;
	string dest, source = "", im = "";
	for (auto x : ins)
	{
		if (isdest)
		{
			if (issource)
			{
				if (im == "")
				{
					if (x == ' ' || x == ',')
						continue;
					else
						im += x;
				}
				else
				{
					if (x == ' ')
						break;
					else
						im += x;
				}
			}
			else
			{
				if (source == "")
				{
					if (x == ' ' || x == ',')
						continue;
					else
						source += x;
				}
				else
				{
					if (x == ' ' || x == ',')
					{
						issource = true;
						rs1 = source;
					}
					else
						source += x;
				}
			}
		}
		else
		{
			if (x == ' ' || x == ',')
			{
				isdest = true;
				rd = dest;
			}
			else
				dest += x;
		}
	}
	label = im;
}

bool branchifnotequal(string ins, map<string, int> branches)
{
	string temp = ins;
	ins = "";
	int r;
	string rd, rs1, label;
	for (int i = 3; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	BType(ins, rd, rs1, label);

	r = getregister(rd);
	int r1 = getregister(rs1);

	if (registers[r] != registers[r1])
	{
		int place = branches[label];
		place = place * 4 + startingPC;
		programcounter = place;
		printvalues();
		return false;
	}
	else
	{
		printvalues();
		return true;
	}

}

bool branchiflessthan(string ins, map<string, int> branches)
{
	string temp = ins;
	ins = "";
	int r;
	string rd, rs1, label;
	for (int i = 3; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	BType(ins, rd, rs1, label);

	r = getregister(rd);
	int r1 = getregister(rs1);

	if (registers[r] < registers[r1])
	{
		int place = branches[label];
		place = place * 4 + startingPC;
		programcounter = place;
		printvalues();
		return false;
	}
	else
	{
		printvalues();
		return true;
	}
}

bool branchifgreaterthanoreq(string ins, map<string, int> branches)
{
	string temp = ins;
	ins = "";
	int r;
	string rd, rs1, label;
	for (int i = 3; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	BType(ins, rd, rs1, label);

	r = getregister(rd);
	int r1 = getregister(rs1);

	if (registers[r] >= registers[r1])
	{
		int place = branches[label];
		place = place * 4 + startingPC;
		programcounter = place;
		printvalues();
		return false;
	}
	else
	{
		printvalues();
		return true;
	}
}

bool branchiflessthanu(string ins, map<string, int> branches)
{
	string temp = ins;
	ins = "";
	unsigned int r;
	string rd, rs1, label;
	for (int i = 4; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	BType(ins, rd, rs1, label);

	r = getregister(rd);
	unsigned int r1 = getregister(rs1);

	if (registers[r] < registers[r1])
	{
		int place = branches[label];
		place = place * 4 + startingPC;
		programcounter = place;
		printvalues();
		return false;
	}
	else
	{
		printvalues();
		return true;
	}
}

bool branchifgreaterorequ(string ins, map<string, int> branches)
{
	string temp = ins;
	ins = "";
	unsigned int r;
	string rd, rs1, label;
	for (int i = 4; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	BType(ins, rd, rs1, label);

	r = getregister(rd);
	unsigned int r1 = getregister(rs1);

	if (registers[r] >= registers[r1])
	{
		int place = branches[label];
		place = place * 4 + startingPC;
		programcounter = place;
		printvalues();
		return false;
	}
	else
	{
		printvalues();
		return true;
	}
}

void setlessthanimmediate(string ins)
{
	string temp = ins;
	ins = "";
	int imm, r;
	string rd, rs1;
	for (int i = 4; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	IType(ins, rd, rs1, imm);

	r = getregister(rd);
	int r1 = getregister(rs1);

	if (r != 0)
	{
		if (registers[r1] < imm)
		{
			registers[r] = 1;
		}
		else
			registers[r] = 0;
	}

	printvalues();
}

void xorimmediate(string ins)
{
	string temp = ins;
	ins = "";
	int imm, r;
	string rd, rs1;
	for (int i = 4; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	IType(ins, rd, rs1, imm);

	r = getregister(rd);
	int r1 = getregister(rs1);
	int x = registers[r1];
	if (r != 0)
	{
		registers[r] = x ^ imm;
	}

	printvalues();
}

void orimmediate(string ins)
{
	string temp = ins;
	ins = "";
	int imm, r;
	string rd, rs1;
	for (int i = 3; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	IType(ins, rd, rs1, imm);

	r = getregister(rd);
	int r1 = getregister(rs1);
	int x = registers[r1];
	if (r != 0)
	{
		registers[r] = x | imm;
	}

	printvalues();
}

void andimmediate(string ins)
{
	string temp = ins;
	ins = "";
	int imm, r;
	string rd, rs1;
	for (int i = 4; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	IType(ins, rd, rs1, imm);

	r = getregister(rd);
	int r1 = getregister(rs1);
	int x = registers[r1];
	if (r != 0)
	{
		registers[r] = x & imm;
	}

	printvalues();
}

void shiftleftlogicalimmediate(string ins)
{
	string temp = ins;
	ins = "";
	int imm, r;
	string rd, rs1;
	for (int i = 4; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	IType(ins, rd, rs1, imm);

	r = getregister(rd);
	int r1 = getregister(rs1);
	int x = registers[r1];
	if (r != 0)
	{
		registers[r] = x << imm;
	}

	printvalues();
}

void shiftrightlogicalimmediate(string ins)
{
	string temp = ins;
	ins = "";
	int imm, r;
	string rd, rs1;
	for (int i = 4; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	IType(ins, rd, rs1, imm);

	r = getregister(rd);
	int r1 = getregister(rs1);
	unsigned int x = registers[r1];
	if (r != 0)
	{
		registers[r] = x >> imm;
	}

	printvalues();
}

void shiftrightarithmeticimmediate(string ins)
{
	string temp = ins;
	ins = "";
	int imm, r;
	string rd, rs1;
	for (int i = 4; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	IType(ins, rd, rs1, imm);

	r = getregister(rd);
	int r1 = getregister(rs1);
	int x = registers[r1];
	if (r != 0)
	{
		registers[r] = x >> imm;
	}

	printvalues();
}

void RType(string ins, string& rd, string& rs1, string& rs2)
{
	bool isdest = false, issource = false;
	string dest, source = "", im = "";
	for (auto x : ins)
	{
		if (isdest)
		{
			if (issource)
			{
				if (im == "")
				{
					if (x == ' ' || x == ',')
						continue;
					else
						im += x;
				}
				else
				{
					if (x == ' ')
						break;
					else
						im += x;
				}
			}
			else
			{
				if (source == "")
				{
					if (x == ' ' || x == ',')
						continue;
					else
						source += x;
				}
				else
				{
					if (x == ' ' || x == ',')
					{
						issource = true;
						rs1 = source;
					}
					else
						source += x;
				}
			}
		}
		else
		{
			if (x == ' ' || x == ',')
			{
				isdest = true;
				rd = dest;
			}
			else
				dest += x;
		}
	}
	rs2 = im;
}

void add(string ins)
{
	string temp = ins;
	ins = "";
	int r;
	string rd, rs1, rs2;
	for (int i = 3; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	RType(ins, rd, rs1, rs2);

	r = getregister(rd);
	int r1 = getregister(rs1), r2 = getregister(rs2);

	if (r != 0)
		registers[r] = registers[r1] + registers[r2];

	printvalues();
}

void subtract(string ins)
{
	string temp = ins;
	ins = "";
	int r;
	string rd, rs1, rs2;
	for (int i = 3; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	RType(ins, rd, rs1, rs2);

	r = getregister(rd);
	int r1 = getregister(rs1), r2 = getregister(rs2);

	if (r != 0)
		registers[r] = registers[r1] - registers[r2];

	printvalues();
}

void shiftleftlogical(string ins)
{
	string temp = ins;
	ins = "";
	int r;
	string rd, rs1, rs2;
	for (int i = 3; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	RType(ins, rd, rs1, rs2);

	r = getregister(rd);
	int r1 = getregister(rs1), r2 = getregister(rs2);

	if (r != 0)
	{
		registers[r] = registers[r1] << registers[r2];
	}

	printvalues();
}

void setlessthan(string ins)
{
	string temp = ins;
	ins = "";
	int r;
	string rd, rs1, rs2;
	for (int i = 3; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	RType(ins, rd, rs1, rs2);

	r = getregister(rd);
	int r1 = getregister(rs1), r2 = getregister(rs2);

	if (r != 0)
	{
		if (registers[r1] < registers[r2])
		{
			registers[r] = 1;
		}
		else
			registers[r] = 0;
	}

	printvalues();
}

void setlessthanu(string ins)
{
	string temp = ins;
	ins = "";
	int r;
	string rd, rs1, rs2;
	for (int i = 4; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	RType(ins, rd, rs1, rs2);

	r = getregister(rd);
	int r1 = getregister(rs1), r2 = getregister(rs2);
	unsigned int x = registers[r1], y = registers[r2];
	if (r != 0)
	{
		if (x < y)
		{
			registers[r] = 1;
		}
		else
			registers[r] = 0;
	}

	printvalues();
}

void exclusiveor(string ins)
{
	string temp = ins;
	ins = "";
	int r;
	string rd, rs1, rs2;
	for (int i = 3; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	RType(ins, rd, rs1, rs2);

	r = getregister(rd);
	int r1 = getregister(rs1), r2 = getregister(rs2);

	if (r != 0)
	{
		registers[r] = registers[r1] ^ registers[r2];
	}

	printvalues();
}

void shiftrightlogical(string ins)
{
	string temp = ins;
	ins = "";
	int r;
	string rd, rs1, rs2;
	for (int i = 3; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	RType(ins, rd, rs1, rs2);

	r = getregister(rd);
	int r1 = getregister(rs1), r2 = getregister(rs2);
	unsigned int x = registers[r1];
	if (r != 0)
	{
		registers[r] = x >> registers[r2];
	}

	printvalues();
}

void shiftrightarithmetic(string ins)
{
	string temp = ins;
	ins = "";
	int r;
	string rd, rs1, rs2;
	for (int i = 3; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	RType(ins, rd, rs1, rs2);

	r = getregister(rd);
	int r1 = getregister(rs1), r2 = getregister(rs2);
	if (r != 0)
	{
		registers[r] = registers[r1] >> registers[r2];
	}

	printvalues();
}

void orfunc(string ins)
{
	string temp = ins;
	ins = "";
	int r;
	string rd, rs1, rs2;
	for (int i = 2; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	RType(ins, rd, rs1, rs2);

	r = getregister(rd);
	int r1 = getregister(rs1), r2 = getregister(rs2);
	if (r != 0)
	{
		registers[r] = registers[r1] | registers[r2];
	}

	printvalues();
}

void andfunc(string ins)
{
	string temp = ins;
	ins = "";
	int r;
	string rd, rs1, rs2;
	for (int i = 3; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	RType(ins, rd, rs1, rs2);

	r = getregister(rd);
	int r1 = getregister(rs1), r2 = getregister(rs2);
	if (r != 0)
	{
		registers[r] = registers[r1] & registers[r2];
	}

	printvalues();
}

void storeword(string ins)
{
	string temp = ins;
	ins = "";
	string rd, rs1, imm;

	for (int i = 2; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	bool isrd = false, isimm = false;
	string dest, im = "", placetogo = "";
	for (auto x : ins)
	{
		if (isrd)
		{
			if (isimm)
			{
				if (placetogo == "")
				{
					if (x == ' ' || x == ',')
						continue;
					else if (x == ')')
					{
						rs1 = placetogo;
						break;
					}
					else
						placetogo += x;
				}
				else
				{
					if (x == ' ' || x == ',')
						continue;
					else if (x == ')')
					{
						rs1 = placetogo;
						break;
					}
					else
						placetogo += x;
				}
			}
			else
			{
				if (im == "")
				{
					if (x == ' ' || x == ',')
						continue;
					else
						im += x;
				}
				else
				{
					if (x == '(')
					{
						isimm = true;
						imm = im;
					}
					else
						im += x;
				}
			}

		}
		else
		{
			if (x == ' ' || x == ',')
			{
				isrd = true;
				rd = dest;
			}
			else
				dest += x;
		}
	}
	int r = getregister(rd);
	int r1 = getregister(rs1);
	int offset = stoi(imm);
	int place = registers[r1] + offset;

	saveword(registers[r], place);

}

void saveword(int word, int location)
{
	string bin = tobinary(word);
	int x = bin.size();
	string save;
	if (bin[0] == '0')
	{
		save = "00000000000000000000000000000000";
	}
	else
	{
		save = "11111111111111111111111111111111";
	}
	for (int i = 0; i < x; i++)
	{
		save[31 - i] = bin[x - i - 1];
	}


	Memory[location] = save;

	printvalues();
}

void savehalf(int word, int location)
{
	string bin = tobinary(word);
	int x = bin.size();
	string save;
	if (bin[0] == '0')
	{
		save = "00000000000000000000000000000000";
	}
	else
	{
		save = "11111111111111111111111111111111";
	}
	for (int i = 0; i < x; i++)
	{
		save[31 - i] = bin[x - i - 1];
	}
	string full = "0000000000000000";
	string secondhalf = save.substr(16, 16);
	full = full + secondhalf;
	Memory[location] = full;

	printvalues();
}

void savebyte(int word, int location)
{
	string bin = tobinary(word);
	int x = bin.size();
	string save;
	if (bin[0] == '0')
	{
		save = "00000000000000000000000000000000";
	}
	else
	{
		save = "11111111111111111111111111111111";
	}
	for (int i = 0; i < x; i++)
	{
		save[31 - i] = bin[x - i - 1];
	}
	string full = "000000000000000000000000";
	string lastbyte = save.substr(24, 16);
	full = full + lastbyte;
	Memory[location] = full;

	printvalues();
}

string tobinary(int word)
{
	string bin = "";
	vector<int> vec;
	int x = word;
	if (x == 0)
		return "0";
	if (x > 0)
	{
		while (x > 0)
		{
			vec.push_back(x % 2);
			x /= 2;
		}
		vec.push_back(0);
		reverse(vec.begin(), vec.end());

		for (auto y : vec)
		{
			bin += to_string(y);
		}
	}
	else
	{
		x = 0 - x;
		x = x - 1;

		while (x > 0)
		{
			vec.push_back(x % 2);
			x /= 2;
		}
		for (int i = 0; i < vec.size(); i++)
		{
			if (vec[i] == 0)
				vec[i] = 1;
			else
				vec[i] = 0;
		}
		vec.push_back(1);
		reverse(vec.begin(), vec.end());

		for (auto y : vec)
		{
			bin += to_string(y);
		}

	}



	return bin;
}

void storehalf(string ins)
{
	string temp = ins;
	ins = "";
	string rd, rs1, imm;

	for (int i = 2; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	bool isrd = false, isimm = false;
	string dest, im = "", placetogo = "";
	for (auto x : ins)
	{
		if (isrd)
		{
			if (isimm)
			{
				if (placetogo == "")
				{
					if (x == ' ' || x == ',')
						continue;
					else if (x == ')')
					{
						rs1 = placetogo;
						break;
					}
					else
						placetogo += x;
				}
				else
				{
					if (x == ' ' || x == ',')
						continue;
					else if (x == ')')
					{
						rs1 = placetogo;
						break;
					}
					else
						placetogo += x;
				}
			}
			else
			{
				if (im == "")
				{
					if (x == ' ' || x == ',')
						continue;
					else
						im += x;
				}
				else
				{
					if (x == '(')
					{
						isimm = true;
						imm = im;
					}
					else
						im += x;
				}
			}

		}
		else
		{
			if (x == ' ' || x == ',')
			{
				isrd = true;
				rd = dest;
			}
			else
				dest += x;
		}
	}
	int r = getregister(rd);
	int r1 = getregister(rs1);
	int offset = stoi(imm);
	int place = registers[r1] + offset;

	savehalf(registers[r], place);
}

void storebyte(string ins)
{
	string temp = ins;
	ins = "";
	string rd, rs1, imm;

	for (int i = 2; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	bool isrd = false, isimm = false;
	string dest, im = "", placetogo = "";
	for (auto x : ins)
	{
		if (isrd)
		{
			if (isimm)
			{
				if (placetogo == "")
				{
					if (x == ' ' || x == ',')
						continue;
					else if (x == ')')
					{
						rs1 = placetogo;
						break;
					}
					else
						placetogo += x;
				}
				else
				{
					if (x == ' ' || x == ',')
						continue;
					else if (x == ')')
					{
						rs1 = placetogo;
						break;
					}
					else
						placetogo += x;
				}
			}
			else
			{
				if (im == "")
				{
					if (x == ' ' || x == ',')
						continue;
					else
						im += x;
				}
				else
				{
					if (x == '(')
					{
						isimm = true;
						imm = im;
					}
					else
						im += x;
				}
			}

		}
		else
		{
			if (x == ' ' || x == ',')
			{
				isrd = true;
				rd = dest;
			}
			else
				dest += x;
		}
	}
	int r = getregister(rd);
	int r1 = getregister(rs1);
	int offset = stoi(imm);
	int place = registers[r1] + offset;

	savebyte(registers[r], place);
}

void loadword(string ins)
{
	string temp = ins;
	ins = "";
	string rd, rs1, imm;

	for (int i = 2; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	bool isrd = false, isimm = false;
	string dest, im = "", placetogo = "";
	for (auto x : ins)
	{
		if (isrd)
		{
			if (isimm)
			{
				if (placetogo == "")
				{
					if (x == ' ' || x == ',')
						continue;
					else if (x == ')')
					{
						rs1 = placetogo;
						break;
					}
					else
						placetogo += x;
				}
				else
				{
					if (x == ' ' || x == ',')
						continue;
					else if (x == ')')
					{
						rs1 = placetogo;
						break;
					}
					else
						placetogo += x;
				}
			}
			else
			{
				if (im == "")
				{
					if (x == ' ' || x == ',')
						continue;
					else
						im += x;
				}
				else
				{
					if (x == '(')
					{
						isimm = true;
						imm = im;
					}
					else
						im += x;
				}
			}

		}
		else
		{
			if (x == ' ' || x == ',')
			{
				isrd = true;
				rd = dest;
			}
			else
				dest += x;
		}
	}
	int r = getregister(rd);
	int r1 = getregister(rs1);
	int offset = stoi(imm);
	int place = registers[r1] + offset;

	ldwrd(r, Memory[place]);


	printvalues();
}

void ldwrd(int rd, string value)
{
	int ans = 0, i = 0;
	if (value[0] == '0')
	{
		for (auto x : value)
		{
			ans += pow(2, 31 - i) * (x - '0');

			i++;
		}
	}
	else
	{
		string temp = "";
		for (auto x : value)
		{
			if (x == '0')
			{
				temp.append(1, '1');

			}
			else
			{
				temp.append(1, '0');
			}
		}
		for (auto x : temp)
		{
			ans += pow(2, 31 - i) * (x - '0');

			i++;
		}
		ans += 1;
		ans = 0 - ans;

	}
	registers[rd] = ans;
}

void loadbyte(string ins)
{
	string temp = ins;
	ins = "";
	string rd, rs1, imm;

	for (int i = 2; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	bool isrd = false, isimm = false;
	string dest, im = "", placetogo = "";
	for (auto x : ins)
	{
		if (isrd)
		{
			if (isimm)
			{
				if (placetogo == "")
				{
					if (x == ' ' || x == ',')
						continue;
					else if (x == ')')
					{
						rs1 = placetogo;
						break;
					}
					else
						placetogo += x;
				}
				else
				{
					if (x == ' ' || x == ',')
						continue;
					else if (x == ')')
					{
						rs1 = placetogo;
						break;
					}
					else
						placetogo += x;
				}
			}
			else
			{
				if (im == "")
				{
					if (x == ' ' || x == ',')
						continue;
					else
						im += x;
				}
				else
				{
					if (x == '(')
					{
						isimm = true;
						imm = im;
					}
					else
						im += x;
				}
			}

		}
		else
		{
			if (x == ' ' || x == ',')
			{
				isrd = true;
				rd = dest;
			}
			else
				dest += x;
		}
	}
	int r = getregister(rd);
	int r1 = getregister(rs1);
	int offset = stoi(imm);
	int place = registers[r1] + offset;

	ldbyte(r, Memory[place]);


	printvalues();
}

void ldbyte(int rd, string value)
{
	int ans = 0, i = 0;
	if (value[24] == '0')
	{
		for (int i = 24; i < 32; i++)
		{
			ans += pow(2, 31 - i) * (value[i] - '0');
		}
	}
	else
	{
		string temp = "";
		for (int i = 24; i < 32; i++)
		{
			if (value[i] == '0')
			{
				temp.append(1, '1');

			}
			else
			{
				temp.append(1, '0');
			}
		}
		for (auto x : temp)
		{
			ans += pow(2, 7 - i) * (x - '0');

			i++;
		}
		ans += 1;
		ans = 0 - ans;

	}
	registers[rd] = ans;
}

void loadhalf(string ins)
{
	string temp = ins;
	ins = "";
	string rd, rs1, imm;

	for (int i = 2; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	bool isrd = false, isimm = false;
	string dest, im = "", placetogo = "";
	for (auto x : ins)
	{
		if (isrd)
		{
			if (isimm)
			{
				if (placetogo == "")
				{
					if (x == ' ' || x == ',')
						continue;
					else if (x == ')')
					{
						rs1 = placetogo;
						break;
					}
					else
						placetogo += x;
				}
				else
				{
					if (x == ' ' || x == ',')
						continue;
					else if (x == ')')
					{
						rs1 = placetogo;
						break;
					}
					else
						placetogo += x;
				}
			}
			else
			{
				if (im == "")
				{
					if (x == ' ' || x == ',')
						continue;
					else
						im += x;
				}
				else
				{
					if (x == '(')
					{
						isimm = true;
						imm = im;
					}
					else
						im += x;
				}
			}

		}
		else
		{
			if (x == ' ' || x == ',')
			{
				isrd = true;
				rd = dest;
			}
			else
				dest += x;
		}
	}
	int r = getregister(rd);
	int r1 = getregister(rs1);
	int offset = stoi(imm);
	int place = registers[r1] + offset;

	ldhalf(r, Memory[place]);


	printvalues();
}

void ldhalf(int rd, string value)
{
	int ans = 0, i = 0;
	if (value[16] == '0')
	{
		for (int i = 16; i < 32; i++)
		{
			ans += pow(2, 31 - i) * (value[i] - '0');
		}
	}
	else
	{
		string temp = "";
		for (int i = 16; i < 32; i++)
		{
			if (value[i] == '0')
			{
				temp.append(1, '1');

			}
			else
			{
				temp.append(1, '0');
			}
		}
		for (auto x : temp)
		{
			ans += pow(2, 15 - i) * (x - '0');

			i++;
		}
		ans += 1;
		ans = 0 - ans;

	}
	registers[rd] = ans;
}

void loadbyteu(string ins)
{
	string temp = ins;
	ins = "";
	string rd, rs1, imm;

	for (int i = 3; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	bool isrd = false, isimm = false;
	string dest, im = "", placetogo = "";
	for (auto x : ins)
	{
		if (isrd)
		{
			if (isimm)
			{
				if (placetogo == "")
				{
					if (x == ' ' || x == ',')
						continue;
					else if (x == ')')
					{
						rs1 = placetogo;
						break;
					}
					else
						placetogo += x;
				}
				else
				{
					if (x == ' ' || x == ',')
						continue;
					else if (x == ')')
					{
						rs1 = placetogo;
						break;
					}
					else
						placetogo += x;
				}
			}
			else
			{
				if (im == "")
				{
					if (x == ' ' || x == ',')
						continue;
					else
						im += x;
				}
				else
				{
					if (x == '(')
					{
						isimm = true;
						imm = im;
					}
					else
						im += x;
				}
			}

		}
		else
		{
			if (x == ' ' || x == ',')
			{
				isrd = true;
				rd = dest;
			}
			else
				dest += x;
		}
	}
	int r = getregister(rd);
	int r1 = getregister(rs1);
	int offset = stoi(imm);
	int place = registers[r1] + offset;

	ldbyteu(r, Memory[place]);


	printvalues();
}

void loadhalfu(string ins)
{
	string temp = ins;
	ins = "";
	string rd, rs1, imm;

	for (int i = 3; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	bool isrd = false, isimm = false;
	string dest, im = "", placetogo = "";
	for (auto x : ins)
	{
		if (isrd)
		{
			if (isimm)
			{
				if (placetogo == "")
				{
					if (x == ' ' || x == ',')
						continue;
					else if (x == ')')
					{
						rs1 = placetogo;
						break;
					}
					else
						placetogo += x;
				}
				else
				{
					if (x == ' ' || x == ',')
						continue;
					else if (x == ')')
					{
						rs1 = placetogo;
						break;
					}
					else
						placetogo += x;
				}
			}
			else
			{
				if (im == "")
				{
					if (x == ' ' || x == ',')
						continue;
					else
						im += x;
				}
				else
				{
					if (x == '(')
					{
						isimm = true;
						imm = im;
					}
					else
						im += x;
				}
			}

		}
		else
		{
			if (x == ' ' || x == ',')
			{
				isrd = true;
				rd = dest;
			}
			else
				dest += x;
		}
	}
	int r = getregister(rd);
	int r1 = getregister(rs1);
	int offset = stoi(imm);
	int place = registers[r1] + offset;

	ldhalfu(r, Memory[place]);


	printvalues();
}

void ldbyteu(int rd, string value)
{
	int ans = 0, i = 0;

	for (int i = 24; i < 32; i++)
	{
		ans += pow(2, 31 - i) * (value[i] - '0');
	}

	registers[rd] = ans;
}

void ldhalfu(int rd, string value)
{
	int ans = 0, i = 0;

	for (int i = 16; i < 32; i++)
	{
		ans += pow(2, 31 - i) * (value[i] - '0');
	}
	registers[rd] = ans;
}

void multiply(string ins)
{
	string temp = ins;
	ins = "";
	int r;
	string rd, rs1, rs2;
	for (int i = 3; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	RType(ins, rd, rs1, rs2);

	r = getregister(rd);
	int r1 = getregister(rs1), r2 = getregister(rs2);

	if (r != 0)
		registers[r] = registers[r1] * registers[r2];

	printvalues();
}

void divide(string ins)
{
	string temp = ins;
	ins = "";
	int r;
	string rd, rs1, rs2;
	for (int i = 3; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	RType(ins, rd, rs1, rs2);

	r = getregister(rd);
	int r1 = getregister(rs1), r2 = getregister(rs2);

	if (r != 0)
		registers[r] = registers[r1] / registers[r2];

	printvalues();
}


void setlessthanimmediateu(string ins)
{
	string temp = ins;
	ins = "";
	int r;
	int imm;
	string rd, rs1;
	for (int i = 5; i < temp.size(); i++)
	{
		if (ins != "")
		{
			ins += temp[i];
		}
		else
		{
			if (temp[i] != ' ')
			{
				ins += temp[i];
			}
		}
	}


	IType(ins, rd, rs1, imm);
	unsigned int imm2 = imm;
	r = getregister(rd);
	int r1 = getregister(rs1);
	unsigned int x = registers[r1];
	if (r != 0)
	{
		if (x < imm2)
		{
			registers[r] = 1;
		}
		else
			registers[r] = 0;
	}

	printvalues();
}


int binaryToDecimal(int n)
{
	int num = n;
	int dec_value = 0;

	// Initializing base value to 1, i.e 2^0
	int base = 1;

	int temp = num;
	while (temp) {
		int last_digit = temp % 10;
		temp = temp / 10;

		dec_value += last_digit * base;

		base = base * 2;
	}

	return dec_value;
}

int  binaryToDecimal(string bin_string)
{
	int number = 0;
	number = stoi(bin_string, 0, 2);

	cout << "The decimal value is: " << number << endl;
	return number;
}

void BinaryTohexa(string binarynum)
{
	int decimalnum = binaryToDecimal(binarynum);
	decToHexa(decimalnum);

}
void decToBinary(int n)
{
	// array to store binary number
	int binaryNum[32];
	bool flag;
	if (n == 0)
		flag = true;
	else
		flag = false;
	// counter for binary array
	int i = 0;
	while (n > 0) {

		// storing remainder in binary array
		binaryNum[i] = n % 2;
		n = n / 2;
		i++;
	}
	cout << "The binary value is: ";
	if (flag)
		cout << "0";
	// printing binary array in reverse order
	for (int j = i - 1; j >= 0; j--)
		cout << binaryNum[j];
}



void decToHexa(int n)
{
	// char array to store hexadecimal number
	char hexaDeciNum[100];
	bool flag;
	if (n == 0)
		flag = true;
	else
		flag = false;
	// counter for hexadecimal number array
	int i = 0;
	while (n != 0) {
		// temporary variable to store remainder
		int temp = 0;

		// storing remainder in temp variable.
		temp = n % 16;

		// check if temp < 10
		if (temp < 10) {
			hexaDeciNum[i] = temp + 48;
			i++;
		}
		else {
			hexaDeciNum[i] = temp + 55;
			i++;
		}

		n = n / 16;
	}
	cout << "The hexa value is: ";
	if (flag)
		cout << "0";
	// printing hexadecimal number array in reverse order
	for (int j = i - 1; j >= 0; j--)
		cout << hexaDeciNum[j];
}

string bintoHexa(string binary)
{
	string one, two, three, four, five, six, seven, eight;
	one = binary.substr(0, 4);
	two = binary.substr(4, 4);
	three = binary.substr(8, 4);
	four = binary.substr(12, 4);
	five = binary.substr(16, 4);
	six = binary.substr(20, 4);
	seven = binary.substr(24, 4);
	eight = binary.substr(28, 4);
	one = getonehex(one);
	two = getonehex(two);
	three = getonehex(three);
	four = getonehex(four);
	five = getonehex(five);
	six = getonehex(six);
	seven = getonehex(seven);
	eight = getonehex(eight);
	string ans = one + two + three + four + five + six + seven + eight;
	return ans;
}

string getonehex(string fourbits)
{
	string ans;
	if (fourbits == "0000")
	{
		ans = "0";
	}
	else if (fourbits == "0001")
	{
		ans = "1";
	}
	else if (fourbits == "0010")
	{
		ans = "2";
	}
	else if (fourbits == "0011")
	{
		ans = "3";
	}
	else if (fourbits == "0100")
	{
		ans = "4";
	}
	else if (fourbits == "0101")
	{
		ans = "5";
	}
	else if (fourbits == "0110")
	{
		ans = "6";
	}
	else if (fourbits == "0111")
	{
		ans = "7";
	}
	else if (fourbits == "1000")
	{
		ans = "8";
	}
	else if (fourbits == "1001")
	{
		ans = "9";
	}
	else if (fourbits == "1010")
	{
		ans = "a";
	}
	else if (fourbits == "1011")
	{
		ans = "b";
	}
	else if (fourbits == "1100")
	{
		ans = "c";
	}
	else if (fourbits == "1101")
	{
		ans = "d";
	}
	else if (fourbits == "1110")
	{
		ans = "e";
	}
	else if (fourbits == "1111")
	{
		ans = "f";
	}

	return ans;
}

int bintodec(string value)
{
	int ans = 0, i = 0;
	if (value[0] == '0')
	{
		for (auto x : value)
		{
			ans += pow(2, 31 - i) * (x - '0');

			i++;
		}
	}
	else
	{
		string temp = "";
		for (auto x : value)
		{
			if (x == '0')
			{
				temp.append(1, '1');

			}
			else
			{
				temp.append(1, '0');
			}
		}
		for (auto x : temp)
		{
			ans += pow(2, 31 - i) * (x - '0');

			i++;
		}
		ans += 1;
		ans = 0 - ans;

	}
	return ans;
}