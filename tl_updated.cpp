#include <iostream>
#include <string.h>
#include <regex>
#include <vector>
#include <fstream>

using namespace std;

// Generate polynomial basis representation
string getChar(string which, int p, char mode='r')
{
	if (p == 0) return "";
	else if (p == 1) return which;
	else
	{
		if (mode == 'r') return which+"\\^"+to_string(p);
		else return which+"^"+to_string(p);
	}
}

// Generate polynomial basis
vector<string> generate(int n_x, int n_y, char mode='r')
{
	vector<vector<int>> cross;
	vector<string> res;
	for(int i =0;i<n_x+1;i++)
	{
		for(int j = 0;j<n_y+1;j++)
		{
			cross.push_back({i,j});
		}
	}
	for (int i=0;i<(n_x+1)*(n_y+1);i++)
	{
		int p_x = cross[i][0];
		int p_y = cross[i][1];
		if (mode == 'r') res.push_back(R"((^\d{0,}|[+-]\d{0,}))"+getChar("x",p_x)+getChar("y",p_y)+R"((?![\^xy]))");
		else res.push_back(getChar("x",p_x)+getChar("y",p_y));
	}
	if (mode == 'r') res[0] = R"((([+-]|^)\d{1,}\b))";
	else res[0] = "k";
	return res;
}

// Get highest degree of the polynomial
int getDegree(string poly, string which)
{
	int deg = 0;
	regex re(which+"\\^{0,1}(\\d{0,})");
	sregex_iterator next(poly.begin(), poly.end(), re);
  	sregex_iterator end;
	while (next != end) {
    			smatch match = *next;
   			if (match[1].str() == "")
			{
				if (deg < 1) deg = 1;
			}
			else
			{
				if (stoi(match[1].str()) > deg) deg = stoi(match[1].str());
			}
    			next++;
  		}
	return deg;
}

// Get sum of a list
int sum(vector<int> arr)
{
	int sum = 0;
	for(int x: arr) sum = sum + x;
	return sum;
}

// Processing polynomials with regex
vector<int> getMatches(string poly, string regx)
{
	vector<int> res;
	regex re(regx);
	sregex_iterator next(poly.begin(), poly.end(), re);
  	sregex_iterator end;
	while (next != end) {
    			smatch match = *next;
   			if (match[1].str() != "-" && match[1].str() != "+" && match[1].str() != "")
			{
				res.push_back(stoi(match[1].str()));
			}
			else res.push_back(stoi(match[1].str()+"1"));
    			next++;
  		}
	return res;
}

// Used for polynomial multiplication
vector<int> shift(vector<int> arr, int by)
{
	int index;
	vector<int> res;
	for(int i=0;i<arr.size();i++)
	{
		if(i-by < 0) index = arr.size()+i-by;
		else index = i-by;
		res.push_back(arr[index]);
	}
	return res;
}

// Give a structure to input string polynomial
class polynomial
{
	public:	
	string poly;
	int deg_x;
	int deg_y;
	vector<string> coeff_list;
	vector<string> coeff_regex;
	vector<int> coeff;

	polynomial(string p="0x+0y+0")
	{
		poly = regex_replace(p,regex(" "),"");
		poly = regex_replace(poly,regex("\\("),"");
		poly = regex_replace(poly,regex("\\)"),"");
		deg_x = getDegree(poly,"x");
		deg_y = getDegree(poly,"y");
		coeff_list = generate(deg_x,deg_y,'w');
		coeff_regex = generate(deg_x,deg_y);
		for(string regx: coeff_regex)
		{
			coeff.push_back(sum(getMatches(poly,regx)));
		}
	}

	polynomial product(polynomial other)
	{
		int de_x = deg_x + other.deg_x;
		int de_y = deg_y + other.deg_y;
		coeff_list = generate(de_x,de_y,'w');
		coeff_regex = generate(de_x,de_y);
		other.coeff_list = coeff_list;
		other.coeff_regex = coeff_regex;
		vector<int> temp1;
		vector<int> temp2;
		for(string regx: coeff_regex)
		{
			temp1.push_back(sum(getMatches(poly,regx)));
			temp2.push_back(sum(getMatches(other.poly,regx)));
		}
		coeff = temp1;
		other.coeff = temp2;
		vector<vector <int>> M;
		vector<int> W;
		for(int i=0;i<coeff.size();i++)
		{ 
			M.push_back(shift(coeff,i));
		}
		W = other.coeff;
		vector<int> new_coeff;
		int ctr;
		for(int i=0;i<coeff.size();i++)
		{
			ctr = 0;
			for(int j=0;j<coeff.size();j++)
			{
				ctr = ctr + M[j][i]*W[j];
			}
			new_coeff.push_back(ctr);
		}
		polynomial result_poly = polynomial("");
		result_poly.coeff = new_coeff;
		result_poly.coeff_list = coeff_list;
		result_poly.coeff_regex = coeff_regex;
		result_poly.poly = result_poly.repr();
		result_poly.deg_x = de_x;
		result_poly.deg_y = de_y;
		return result_poly;
	}

	string repr()
	{
		string result = "";
		for(int i=0;i<coeff.size();i++)
		{
			string c = "+";
			string r = "";
			
			if (coeff[i] > 0) c = c+to_string(coeff[i]);
			else if(coeff[i] == 0) continue;
			else c = to_string(coeff[i]);
		
			if (coeff_list[i] != "k") r = coeff_list[i];
			
			result = result + c + r;
		}
		return result;
	}

};

//Processing polynomials
vector<polynomial> process(string arg)
{
	vector<polynomial> result;
	vector<string> str;
	char* poly = new char[arg.length()+1];
	strcpy(poly, arg.c_str());
	char *token = strtok(poly,"//");
	while (token != NULL) 
    	{ 
        	str.push_back(string(token));
        	token = strtok(NULL, "//"); 
    	} 
	for(string poly_str: str)
	{ 
		char* p = new char[poly_str.length()+1];
		strcpy(p, poly_str.c_str());
		char *t = strtok(p,"*");
		polynomial c = polynomial("1");
		while (t != NULL) 
    		{
        		c = c.product(polynomial(string(t)));
        		t = strtok(NULL, "*"); 
    		} 
		result.push_back(c);
	}
	return result;
}

// Compare two polynomials
string comparePolynomial(vector<polynomial> one, vector<polynomial> two)
{
	int ctr = 0;
	if(one.size() != two.size()) return "False";
	else
	{
		for(int i=0;i<one.size();i++)
		{
			if (one[i].repr().compare(two[i].repr()) == 0) ctr++;
		}
		if (ctr == one.size()) return "True";
		else return "False";
	}
}

int main(int argc, char** argv)
{
	string filetext;
	ifstream inputFile("input.txt");
	ofstream outputFile("output.txt");
	outputFile<<"rohitduggal rohitduggal21@gmail.com"<<"\n";
	while (getline (inputFile, filetext))
	{
		vector<string> input;
		char* poly = new char[filetext.length()+1];
		strcpy(poly, filetext.c_str());
		char *token = strtok(poly," ");
		while (token != NULL) 
    		{ 
        		input.push_back(string(token));
        		token = strtok(NULL, " "); 
    		} 
		string poly1 = input[0];
		string poly2 = input[1];
		outputFile<<comparePolynomial(process(poly1),process(poly2))<<"\n";
	}
	inputFile.close();
	outputFile.close();
	return 0;
}
