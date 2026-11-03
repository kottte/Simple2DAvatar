#define MINIAUDIO_IMPLEMENTATION
#include "AudioCapture.h"
#include <cmath>
#include <algorithm>


AudioCapture::AudioCapture() = default;
AudioCapture::~AudioCapture() { Stop(); }

bool AudioCapture::Start()
{
    if (m_initialized) return true;

    ma_device_config config = ma_device_config_init(ma_device_type_capture);
    config.capture.format   = ma_format_f32; // Работаем с float для простоты
    config.capture.channels = 1;             // Моно
    config.sampleRate       = 48000;
    config.dataCallback     = DataCallback;
    config.pUserData        = this;          // Передаём указатель на себя

    if (ma_device_init(NULL, &config, &m_device) != MA_SUCCESS) return false;
    if (ma_device_start(&m_device) != MA_SUCCESS) {
        ma_device_uninit(&m_device);
        return false;
    }

    m_initialized = true;
    return true;
}

void AudioCapture::Stop()
{
    if (!m_initialized) return;
    ma_device_uninit(&m_device);
    m_initialized = false;
    m_currentLevel = 0.0f;
}

float AudioCapture::GetLevel() const
{
    return m_currentLevel.load(std::memory_order_relaxed);
}

// Этот колбэк вызывается в отдельном аудиопотоке!
void AudioCapture::DataCallback(ma_device* pDevice, void* pOutput,
                                const void* pInput, ma_uint32 frameCount)
{
    (void)pOutput;
    if (!pInput || frameCount == 0) return;   // <— важно!

    auto* self = static_cast<AudioCapture*>(pDevice->pUserData);
    if (!self) return;

    const float* samples = static_cast<const float*>(pInput);

    float sum = 0.0f;
    for (ma_uint32 i = 0; i < frameCount; ++i)
        sum += samples[i] * samples[i];

    float rms = std::sqrt(sum / frameCount);
    if (!std::isfinite(rms)) rms = 0.0f;      // защита от NaN/Inf
    self->m_currentLevel.store(std::min(rms, 1.0f), std::memory_order_relaxed);
}