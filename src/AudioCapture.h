#pragma once
#include <atomic>
#include "miniaudio.h"

class AudioCapture
{
public:
    AudioCapture();
    ~AudioCapture();

    bool Start();
    void Stop();

    // Возвращает текущий уровень громкости (RMS) от 0.0 до 1.0
    // Потокобезопасно, можно вызывать из главного потока.
    float GetLevel() const;

private:
    static void DataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

    ma_device m_device;
    bool m_initialized = false;
    std::atomic<float> m_currentLevel{0.0f}; // Потокобезопасный уровень
};