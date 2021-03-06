// ConsoleApplication1.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <fstream>

using namespace std;

int n, flag = 0, m;
int shudu[10][10];//存数独的数组
bool line[10][10], orl[10][10], block[10][10]; //line, orl表示每一行, 列是否出现1 - 9中的数。将9 * 9宫格分成9个小宫格, block表示每一个宫格是否出现1 - 9的数

bool judge(char *s);
bool judge_shudu(int x, int y, int n);
void output();
void get_init();
void dfs(int x, int y);
void creat_sudoku();
int getblocknum(int x, int y);
ofstream Output;

void creat_sudoku()//生成数独终局函数
{
	m = 0;//记录当前生成多少个数独,n是要生成的数独数
	int tmp[10] = { 0,9,1,2,3,4,5,6,7,8 };//tmp 1-9是第一行要生成的数
	int moveleft[10] = { 0,0,3,6,1,4,7,2,5,8 };	//moveleft表示2-9行在第1行基础上整体左移位数
	for (int i = 1; i <= 40320; i++)	//8!=40320
	{
		for (int j = 1; j <= 9; j++)
			shudu[1][j] = tmp[j];

		for (int j = 2; j <= 9; j++)
		{
			for (int k = 1; k <= 9; k++)
			{
				int pos = k - moveleft[j];
				if (pos <= 0) pos += 9;
				shudu[j][k] = shudu[1][pos];
			}
		}//生成一个数独模板

		int fol[10] = { 0,1,2,3,4,5,6,7,8,9 };//fol函数是接下来输出的1-9行对应数独模板的fol行
		for (int j = 1; j <= 6; j++)
		{
			if (j != 1)next_permutation(fol + 4, fol + 7);//此函数是求4-7行的下一个排列
			for (int k = 1; k <= 6; k++)
			{
				if (k != 1)next_permutation(fol + 7, fol + 10);
				for (int p = 1; p <= 9; p++)
				{
					for (int q = 1; q <= 9; q++)
					{
						if (q == 1)Output << shudu[fol[p]][q];
						else Output << " " << shudu[fol[p]][q];
					}
					Output << endl;
				}
				Output << endl;
				if (++m >= n)
					return;
			}//完成一次数独的输出
			fol[7] = 7, fol[8] = 8, fol[9] = 9;
		}//每种数独模板可以输出36种子数独,所以最多能输出8!*36=1451520种
		next_permutation(tmp + 2, tmp + 10);//对tmp函数进行一次全排列
	}
	return;
}

bool judge(char *s)//判断输入的数是否有效
{
	int len = strlen(s);
	bool flag1 = 0;
	for (int i = 0; i < len; i++)
	{
		if (s[i] >= '0'&&s[i] <= '9')
			continue;
		flag1 = 1;
	}//判断是否全为数字
	if (flag1 == 1)return 0;
	return 1;
}

bool judge_shudu(int x, int y, int n)//判断x行y列这个位置是否可以放置数字n
{
	return !line[x][n] && !orl[y][n] && !block[getblocknum(x, y)][n];
}

void output()//打印函数,将数独输出到指定的txt文件中
{
	for (int i = 1; i <= 9; i++)
	{
		for (int j = 1; j <= 9; j++)
		{
			if (j == 1) Output << shudu[i][j];
			else Output << " " << shudu[i][j];
		}
		Output << endl;
	}
	Output << endl;
}

void dfs(int x, int y)//解数独函数,用dfs深度优先搜索
{
	if (flag) return;
	if (y > 9) {
		x++; y = 1;
	}
	if (x == 10 && flag == 0) {
		output();
		flag = 1;
		return;
	}//此时每一位都放置了数字,则输出

	if (!shudu[x][y])
	{
		for (int i = 1; i <= 9 && !flag; i++)
		{
			if (judge_shudu(x, y, i))
			{
				line[x][i] = 1;
				orl[y][i] = 1;
				block[getblocknum(x, y)][i] = 1;
				shudu[x][y] = i;
				dfs(x, y + 1);
				line[x][i] = 0;
				orl[y][i] = 0;
				block[getblocknum(x, y)][i] = 0;
				shudu[x][y] = 0;
			}
		}
	}
	else dfs(x, y + 1);
}//判断x行y列要放置1-9中的哪一位数字,递归实现

int getblocknum(int x, int y)//计算出x行y列的数字在第几个小格子里
{
	return ((x - 1) / 3) * 3 + ((y - 1) / 3) + 1;
}

void get_init()//在解数独中对应给定的数独对三个状态数组进行初始化
{
	for (int i = 1; i <= 9; i++)
	{
		for (int j = 1; j <= 9; j++)
		{
			if (shudu[i][j])
			{
				line[i][shudu[i][j]] = 1;
				orl[j][shudu[i][j]] = 1;
				block[getblocknum(i, j)][shudu[i][j]] = 1;
			}
		}
	}
}


int main(int argc, char *argv[])
{
	ifstream input;
	Output.open("./BIN/sudoku.txt");//结果输出到指定文件
	if (argv[1][0] == '-'&&argv[1][1] == 'c')
	{
		if (judge(argv[2]))
			n = atoi(argv[2]);//如果输入是有效的,则运用ascii to integer函数转化为整数
		else n = 0;
		if (n <= 0)cout << "Wrong input!" << endl;
		else creat_sudoku();//有效,调用生成数独函数
	}
	else if (argv[1][0] == '-'&&argv[1][1] == 's')
	{
		input.open(argv[2]);
		int i = 1, j = 1;
		while (!input.eof())//输入至文件结尾
		{
			input >> shudu[i][j];
			j++;
			if (j > 9) {
				j = 1;
				i++;
			}
			if (i > 9) {
				flag = 0;
				memset(line, 0, sizeof line);
				memset(orl, 0, sizeof orl);
				memset(block, 0, sizeof block);
				get_init();//对行，列，九宫格得到初始数组
				dfs(1, 1);//解给定的数独
				i = 1; j = 1;
				memset(shudu, 0, sizeof shudu);
			}
		}
	}
	else cout << "Wrong input!" << endl;
	system("pause");
	return 0;
}