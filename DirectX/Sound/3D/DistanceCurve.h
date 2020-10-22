#pragma once

//特定の正規化距離でのDSP設定を定義する構造体
struct DistanceCurvePoint {
    //正規化された距離 [0.f, 1.f]
    float distance;
    //DSPコントロール設定
    float dspSettings;
};

//線形セグメントで構成される明示的な区分的曲線を定義し、正規化された距離に関するDSPの動作を直接定義する構造体
struct DistanceCurve {
    //DistanceCurvePoint配列
    //距離に関して昇順でソートされている必要がある
    DistanceCurvePoint* points;
    //距離曲線ポイントの数
    //正規化された端点が最低2つ必要
    unsigned pointCount;
};
