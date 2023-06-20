#include "Shake.h"
#include <random>
using namespace std;

void Shake::ShakePos(float& pos, const int shakeMax, const int shakeMin, const int TargetTime, const int Division)
{
	if (shakeStart) {
		if (shakeTime < TargetTime) {
			//minstd_rand->整数に対して、乗算、加算、剰余算を一回ずつ行う
			//minstd_randやminstd_rand0より4倍ほど速い。
			mt19937 mt{ std::random_device{}() };
			//指定された範囲の値が等確率で発生するよう離散分布する
			uniform_int_distribution<int> l_dist(shakeMin, shakeMax);
			pos = float(l_dist(mt)) / Division;
			shakeTime++;
		}
		else {
			shakeStart = false;
			shakeTime = 0;
			pos = 0.0f;
		}
	}
}