#pragma once

#include "IEffectCreater.h"
#include "IEffectParameter.h"
#include "ISoundEffect.h"
#include "../Voice/IVoice.h"
#include <xaudio2.h>
#include <memory>
#include <vector>

class SoundEffectCollection;

//サウンドエフェクト専用クラス
class SoundEffect : public IEffectCreater, public IEffectParameter {
public:
    SoundEffect(IVoice& voice);
    ~SoundEffect();

    /// <summary>
    /// エフェクトにパラメータを設定する
    /// </summary>
    /// <param name="effectID">設定するエフェクトのID</param>
    /// <param name="parameters">設定するデータの先頭アドレス</param>
    /// <param name="parametersByteSize">データ(parameters)のサイズ</param>
    virtual void setEffectParameters(int effectID, const void* parameters, unsigned parametersByteSize) override;

    /// <summary>
    /// エフェクトのパラメータを取得する
    /// </summary>
    /// <param name="effectID">取得するエフェクトのID</param>
    /// <param name="parameters">パラメータ格納先の先頭アドレス</param>
    /// <param name="parametersByteSize">取得するパラメータのサイズ</param>
    virtual void getEffectParameters(int effectID, void* parameters, unsigned parametersByteSize) const override;

    /// <summary>
    /// エフェクトを有効化する
    /// </summary>
    /// <param name="effectID">有効化したいエフェクトのID</param>
    void enable(int effectID);

    /// <summary>
    /// エフェクトを無効化する
    /// </summary>
    /// <param name="effectID">無効化したいエフェクトのID</param>
    void disable(int effectID);

    /// <summary>
    /// エフェクトの有効化状態を返す
    /// </summary>
    /// <param name="effectID">状態を取得したいエフェクトのID</param>
    /// <returns>有効化状態</returns>
    bool getEnabled(int effectID);

    /// <summary>
    /// エフェクト集にアクセスする
    /// </summary>
    /// <returns></returns>
    SoundEffectCollection& getEffectCollection() const;

private:
    SoundEffect(const SoundEffect&) = delete;
    SoundEffect& operator=(const SoundEffect&) = delete;

    //エフェクトを生成する
    virtual int createEffect(ISoundEffect* target, bool isApply = true) override;
    virtual int createEffect(IUnknown* target, bool isApply = true) override;
    //作成した全エフェクトを適用する
    void apply();
    //IDが有効か
    bool isValidID(int effectID) const;
    //パラメータが有効か
    bool isValidParameters(const void* parameters) const;
    //エフェクトにアクセスできるか
    bool canAccessEffect(int effectID, const void* parameters) const;

private:
    IVoice& mVoice;
    std::unique_ptr<SoundEffectCollection> mEffectCollection;
    std::vector<XAUDIO2_EFFECT_DESCRIPTOR> mDescs;
    bool mIsApplied;
};
