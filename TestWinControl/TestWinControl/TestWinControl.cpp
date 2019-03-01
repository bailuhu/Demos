#include "stdafx.h"

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void output(char val)
{
	cout << val << ' ';
}

int main()
{
	vector<char> vChar;
	vChar.push_back('z');
	vChar.push_back('d');
	vChar.push_back('s');
	vChar.push_back('a');
	vChar.push_back('e');
	vChar.push_back('c');
	vChar.push_back('u');
	vChar.push_back('v');
	for_each (vChar.begin(), vChar.end(), output);
	cout << endl;
	sort(vChar.begin(), vChar.end());
	for_each(vChar.begin(), vChar.end(), output);
}