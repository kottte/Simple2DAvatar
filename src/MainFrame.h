#include <wx/wx.h>
#include <wx/image.h>
#include "AudioCapture.h"

struct AppSettings
{
    int      windowWidth    = 800;
    int      windowHeight   = 600;
    int      offsetX        = 0;
    int      offsetY        = 0;
    double   scale          = 1.0;
    float  threshold    = 0.05f;
};

class MainFrame : public wxFrame
{
public:
    MainFrame(const AppSettings& s, const wxImage& img1, const wxImage& img2);
    ~MainFrame() = default;

    void SetSecondImage(bool second);

private:
    wxBitmap MakeScaledBitmap(const wxImage& src);   // <— вместо UpdateScaledBitmap

    const wxBitmap& ActiveBitmap() const             // <— вместо ActiveImage
    {
        return useSecondImage ? m_bitmap2 : m_bitmap1;
    }

    void OnPaint(wxPaintEvent& event);
    void OnErase(wxEraseEvent& event);
    void OnLevelTimer(wxTimerEvent& event);

    const AppSettings m_cfg;
    bool     useSecondImage = false;
    wxBitmap m_bitmap1;
    wxBitmap m_bitmap2;
    wxPanel* m_panel = nullptr;
    wxTimer  m_levelTimer;
    AudioCapture m_audio;

    wxDECLARE_EVENT_TABLE();
};