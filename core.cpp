#include <cstdio>
#include <algorithm>
#include <cstring>
#include <fstream>

using namespace std;

const int MAXSTR = 200;
const int MAXSTATE = 200;
const int INVALID = MAXSTATE;

char dp[MAXSTR][MAXSTATE];
int dp2[MAXSTR][MAXSTR][MAXSTATE];
int nextIndex[MAXSTR][MAXSTATE];

int transition[MAXSTATE][256]; // ascii chars only for transition
bool isFinal[MAXSTATE];

string str;

FILE *inp = fopen("input.txt","r");
FILE *fs = fopen("test_str.txt","r");
FILE *fp = fopen("result.txt","w");

FILE *test = fopen("test.txt","w");

int getNext(int tar, int cur, int state)
{
	int &ret = dp2[tar][cur][state];
	if(ret != -1) return ret;

	int nextState = transition[state][str[cur]];
	if(tar == cur) ret = (nextState == -1 ? INVALID : nextState);
	else
	{
		if(nextState == -1) ret = INVALID;
		else ret = getNext(tar, cur - 1, nextState);
	}

	return ret;
}

char isAcceptable(int ind, int state)
{
	if(ind == str.size()) return isFinal[state];

	char &ret = dp[ind][state];
	if(ret != -1) return ret;

	ret = false;
	for(int i = ind; i < str.size(); i++)
	{
		int nextState = getNext(ind, i, state);
		if(nextState == INVALID) continue;

		if(isAcceptable(i + 1, nextState))
		{
			ret = true;
			nextIndex[ind][state] = i + 1;
			break;
		}
	}

	return ret;
}

int main(void)
{
	memset(dp, -1, sizeof(dp));
	memset(dp2, -1, sizeof(dp2));
	memset(transition, -1, sizeof(transition));

	int startState;
	fscanf(inp,"%d", &startState);

	int n; // number of accept states;
	fscanf(inp,"%d", &n);
	for(int i = 0; i < n; i++)
	{
		int cur;
		fscanf(inp,"%d", &cur);
		isFinal[cur] = true;
	}

	int m; // number of transition
	fscanf(inp,"%d", &m);
	for(int i = 0; i < m; i++)
	{
		int from, to;
		char alphabet[2];
		fscanf(inp,"%d %d %s", &from, &to, alphabet);
		transition[from][alphabet[0]] = to;
	}

	char temp[MAXSTR + 1];
	fscanf(fs,"%s", temp);
	fprintf(test,"%s", temp);;
	str = temp;

	ofstream ofs("trace.csv", ofstream::out);

	if(isAcceptable(0, startState) == 0) 
		fprintf(fp,"NO\n");
		//printf("NO\n");
	else 
	{
		fprintf(fp,"YES\n");
		//ofs << "YES" << endl;
		ofs << "str,invNum,stateNum" << endl;
		
		int ind = 0, state = startState;
		while(ind < str.size())
		{
			int nInd = nextIndex[ind][state];

			string curstr = str.substr(ind);
			ofs << curstr << "," << (nInd - ind) << "," << state << endl;
			
			string reversed = curstr.substr(0, nInd - ind);
			reverse(reversed.begin(), reversed.end());
			curstr = reversed + curstr.substr(nInd - ind);

			state = transition[state][curstr[0]];
			curstr = curstr.substr(1);
			
			for(int i = nInd - 2; i >= ind; i--)
			{
				ofs << curstr << ",0," << state << endl;
				state = transition[state][curstr[0]];
				curstr = curstr.substr(1);
			}

			ind = nInd;
		}

		ofs << ",0," << state << endl;
		ofs.close();
	}	

	return 0;
}
