#pragma once

#include "../System/SystemInclude.h"

//新しいデータを保存する前に、ランタイムが新しいデータを既存のデータと比較する方法
enum class ComparisonFunc {
    NEVER, //比較しない
    LESS, //src < dest -> pass
    EQUAL, //src == dest -> pass
    LESS_EQUAL, //src <= dest -> pass
    GREATER, //src > dest -> pass
    NOT_EQUAL, //src != dest -> pass
    GREATER_EQUAL, //src >= dest -> pass
    ALWAYS //常に成功
};

D3D11_COMPARISON_FUNC toComparisonFunc(ComparisonFunc func);
