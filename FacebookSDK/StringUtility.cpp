#include "pch.h"

int compare_ordinal(std::wstring strA, std::wstring strB) {
	std::transform(strA.begin(), strA.end(), strA.begin(), ::towlower);
	std::transform(strB.begin(), strB.end(), strB.begin(), ::towlower);
	return strA.compare(strB);
}
