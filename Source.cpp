#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stack>
#include <functional>
#include <set>
#include <queue>
#include <string>
#include <map>
#include <sstream>

using namespace std;

unsigned int rotate(unsigned int x, int sh) 
{
	return (((x) << (sh)) | ((x) >> (32 - (sh))));
}

vector<unsigned int> quarterround(vector<unsigned int> &arr)
{
	unsigned int z1 = arr[1] ^ rotate((arr[0] + arr[3]), 7);
	unsigned int z2 = arr[2] ^ rotate((z1 + arr[0]), 9);
	unsigned int z3 = arr[3] ^ rotate((z2 + z1), 13);
	unsigned int z0 = arr[0] ^ rotate((z3 + z2), 18);
	return vector<unsigned int> {z0, z1, z2, z3};
}

vector<vector<unsigned int>> rowround(vector<vector<unsigned int>> &arr)
{
	vector<unsigned int> arrZ[4];
	arrZ[0] = quarterround(vector<unsigned int> {arr[0][0], arr[0][1], arr[0][2], arr[0][3]});
	arrZ[1] = quarterround(vector<unsigned int> {arr[1][1], arr[1][2], arr[1][3], arr[1][0]});
	arrZ[2] = quarterround(vector<unsigned int> {arr[2][2], arr[2][3], arr[2][0], arr[2][1]});
	arrZ[3] = quarterround(vector<unsigned int> {arr[3][3], arr[3][0], arr[3][1], arr[3][2]});
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < i; j++)
		{
			unsigned int tmp1 = arrZ[i][3];
			for (int q = 0; q < 4; q++)
			{
				unsigned int tmp2 = arrZ[i][q];
				arrZ[i][q] = tmp1;
				tmp1 = tmp2;
			}
		}
	return vector<vector<unsigned int>> {arrZ[0], arrZ[1], arrZ[2], arrZ[3]};
}
vector<vector<unsigned int>> columnround(vector<vector<unsigned int>> &arr)
{
	vector<unsigned int> arrZ[4];
	arrZ[0] = quarterround(vector<unsigned int> {arr[0][0], arr[1][0], arr[2][0], arr[3][0]});
	arrZ[1] = quarterround(vector<unsigned int> {arr[1][1], arr[2][1], arr[3][1], arr[0][1]});
	arrZ[2] = quarterround(vector<unsigned int> {arr[2][2], arr[3][2], arr[0][2], arr[1][2]});
	arrZ[3] = quarterround(vector<unsigned int> {arr[3][3], arr[0][3], arr[1][3], arr[2][3]});
	return vector < vector<unsigned int> > {{arrZ[0][0], arrZ[1][3], arrZ[2][2], arrZ[3][1]}, { arrZ[0][1], arrZ[1][0], arrZ[2][3], arrZ[3][2] }, { arrZ[0][2], arrZ[1][1], arrZ[2][0], arrZ[3][3]}, { arrZ[0][3], arrZ[1][2], arrZ[2][1], arrZ[3][0]}};
}

vector<vector<unsigned int>> doubleround(vector<vector<unsigned int>> &arr)
{
	return(rowround(columnround(arr)));
}

unsigned int littleendian(vector<unsigned char> &arr)
{
	return arr[0] + arr[1] * (1 << 8) + arr[2] * (1 << 16) + arr[3] * (1 << 24);
}

vector<vector<unsigned int>> sasla20(vector<vector<unsigned char>> &arr)
{
	vector<vector<unsigned int>> res(4);
	for (int i = 0; i < 16; i++)
		res[i / 4].push_back(littleendian(arr[i]));
	vector<vector<unsigned int>> x = res;
	for (int i = 0; i < 10; i++)
		res = doubleround(res);
	for (int i = 0; i < 16; i++)
		res[i / 4][i % 4] += x[i / 4][i % 4];
	return res;
}

vector<vector<unsigned int>> make_salsa(vector<vector<unsigned char>> k0, vector<vector<unsigned char>> k1, vector<vector<unsigned char>> n, vector<vector<unsigned char>> t)
{
	vector<vector<unsigned char>> to_salsa;
	to_salsa.push_back(t[0]);
	for (int i = 0; i < 4; i++)
		to_salsa.push_back(k0[i]);
	to_salsa.push_back(t[1]);
	for (int i = 0; i < 4; i++)
		to_salsa.push_back(n[i]);
	to_salsa.push_back(t[2]);
	for (int i = 0; i < 4; i++)
		to_salsa.push_back(k1[i]);
	to_salsa.push_back(t[3]);
	return sasla20(to_salsa);
}

vector<unsigned char> decompress(vector<vector<unsigned int>> data)
{
	vector<unsigned char> result;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			for (int q = 0; q < 4; q++)
			{
				unsigned char new_val = 0;
				for (int p = 0; p < 8; p++)
					new_val += int(((data[i][j] & (1 << (q * 8 + p))) > 0)) << p;
				result.push_back(new_val);
			}
	return result;
}

void test_quatrerround()
{
	cout << endl;
	vector<unsigned int> test = {0, 1, 0, 0};
	test = quarterround(test);
	cout << hex;
	for (int i = 0; i < test.size(); i++)
		cout << test[i] << ' ';
	cout << endl;
}

void test_rowround()
{
	vector<vector<unsigned int>> test = { { 1, 0, 0, 0 }, {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0} };
	test = rowround(test);
	cout << hex;
	for (int i = 0; i < test.size(); i++)
	{
		cout << endl;
		for (int j = 0; j < test[i].size(); j++)
			cout << test[i][j] << ' ';
	}
	cout << endl;
}

void test_columnround()
{
	vector<vector<unsigned int>> test = { { 1, 0, 0, 0 }, { 1, 0, 0, 0 }, { 1, 0, 0, 0 }, { 1, 0, 0, 0 } };
	test = columnround(test);
	cout << hex;
	for (int i = 0; i < test.size(); i++)
	{
		cout << endl;
		for (int j = 0; j < test[i].size(); j++)
			cout << test[i][j] << ' ';
	}
	cout << endl;
}

void test_doubleround()
{
	vector<vector<unsigned int>> test = { { 1, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
	test = doubleround(test);
	cout << hex;
	for (int i = 0; i < test.size(); i++)
	{
		cout << endl;
		for (int j = 0; j < test[i].size(); j++)
			cout << test[i][j] << ' ';
	}
	cout << endl;
}


void test_littleendian()
{
	vector<vector<unsigned char>> test = { { 0, 0, 0, 0 }, { 86, 75, 30, 9 }, { 255, 255, 255, 250 }};
	
	cout << hex;
	for (int i = 0; i < test.size(); i++)
	{
		cout << endl;
		cout << littleendian(test[i]) << ' ';
	}
	cout << endl;
}

void test_salsa()
{
	vector<vector<unsigned char>> test = { {211, 159, 13, 115 }, { 76, 55, 82, 183 }, { 3, 117, 222, 37 }, { 191, 187, 234, 136 },
										   {49, 237, 179, 48 }, { 1, 106, 178, 219 }, { 175, 199, 166, 48 }, { 86, 16, 179, 207 },
										   {31, 240, 32, 63 }, { 15, 83, 93, 161 }, { 116, 147, 48, 113 }, { 238, 55, 204, 36 },
										   {79, 201, 235, 79}, { 3, 81, 156, 47}, { 203, 26, 244, 243 }, { 88, 118, 104, 54 } };
	cout << dec;

	cout << endl;
	vector<unsigned char> res = decompress(sasla20(test));
	for (int i = 0; i < res.size(); i++)
		cout << int(res[i]) << ' ';

	cout << endl;
}

void encrypt(vector<vector<unsigned char>> k0, vector<vector<unsigned char>> k1, vector<vector<unsigned char>> n, vector<vector<unsigned char>> t, string s)
{
	cout << dec;
	vector<vector<unsigned int>> SALSA = make_salsa(k0, k1, n, t);
	vector<unsigned char> hash = decompress(SALSA);
	for (int i = 0; i < s.length(); i++)
		cout << char(hash[i % 64] ^ s[i]);
}

void decrypt(vector<vector<unsigned char>> k0, vector<vector<unsigned char>> k1, vector<vector<unsigned char>> n, vector<vector<unsigned char>> t, string s)
{
	encrypt(k0, k1, n, t, s);
}

int main()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	vector<vector<unsigned char>> k0(4), k1(4), n(4);
	vector<vector<unsigned char>> t = {
		{ 101, 120, 112, 97 },
		{ 110, 100, 32, 51 },
		{ 50, 45, 98, 121 },
		{ 116, 101, 32, 107 }
	};

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			k0[i].push_back(rand() % 254 + 1);
			k1[i].push_back(rand() % 254 + 1);
			n[i].push_back(rand() % 254 + 1);
		}
	}

	cout << "********test_quatrerround*******";
	test_quatrerround();
	cout << "********************************" << endl;
	cout << "********rowround*******";
	test_rowround();
	cout << "***********************" << endl;
	cout << "********columnround*******";
	test_columnround();
	cout << "***********************" << endl;
	cout << "********doubleround*******";
	test_doubleround();
	cout << "***********************" << endl;
	cout << "********littleendian*******";
	test_littleendian();
	cout << "***********************" << endl;
	cout << "********Salsa20*******";
	test_salsa();
	cout << "***********************" << endl;
	string s;
	cin >> s;
	encrypt(k0, k1, n, t, s);

}