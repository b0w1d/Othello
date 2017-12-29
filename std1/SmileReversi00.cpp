#include <vector>
#include <cstdlib>
using namespace std;

namespace s_RANDOM
{
	const int NN=10;
}

pair<int,int> I2P_RANDOM(int Board[][s_RANDOM::NN],int player,vector<pair<int,int>> ValidPoint)
{
	size_t VPsize=ValidPoint.size();
	int choice=rand()%VPsize;
	
	/*
	your code write here
	*/
	return ValidPoint[choice];
}