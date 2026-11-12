#pragma once

#include <array>

//属性
enum class ELEMENT
{
	None = -1,

	Physics,	//物理

	Fire,		//火

	Ice,		//氷

	Wind,		//風

	Thunder,	//雷

	Max

};

//属性耐性
enum class ELEMENT_RESIST
{
	None = -1,

	Normal,		//普通	

	Weak,		//弱点
};


//属性ごとの耐性テーブル
using ElementResistTable = std::array<ELEMENT_RESIST, (int)ELEMENT::Max>;

