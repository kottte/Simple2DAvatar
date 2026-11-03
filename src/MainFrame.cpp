#include "MainFrame.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const AppSettings& s, const wxImage& img1, const wxImage& img2) : wxFrame(NULL, wxID_ANY, "Simple 2D Avatar", wxDefaultPosition, wxDefaultSize,
              wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
        , m_cfg(std::move(s))          // одноразовый перенос — дальше s не нужен
{
    m_panel = new wxPanel(this, wxID_ANY);
    m_panel->SetBackgroundColour(wxColor(0, 255, 0));
    m_panel->SetBackgroundStyle(wxBG_STYLE_PAINT);
    m_panel->Bind(wxEVT_PAINT,            &MainFrame::OnPaint, this);
    m_panel->Bind(wxEVT_ERASE_BACKGROUND, &MainFrame::OnErase, this);
    
    SetClientSize(wxSize(m_cfg.windowWidth, m_cfg.windowHeight));
    m_panel->SetSize(GetClientSize());

    m_bitmap1 = MakeScaledBitmap(img1);   // один раз
    m_bitmap2 = MakeScaledBitmap(img2);   // один раз

    m_audio.Start();
    m_levelTimer.SetOwner(this);
    m_levelTimer.Start(50);
    Bind(wxEVT_TIMER, &MainFrame::OnLevelTimer, this, m_levelTimer.GetId());
}

//-----------------------------------------------------------------------------
// Вписываем изображение в клиентскую область целиком (contain):
// берём min(scaleX, scaleY) — по более узкой стороне.
//-----------------------------------------------------------------------------

wxBitmap MainFrame::MakeScaledBitmap(const wxImage& src)
{
    if (!src.IsOk()) return wxBitmap();

    wxSize ps = m_panel->GetClientSize();
    double fit = std::min(
        static_cast<double>(ps.GetWidth())  / src.GetWidth(),
        static_cast<double>(ps.GetHeight()) / src.GetHeight());
    double eff = fit * m_cfg.scale;
    if (!(eff > 0.0)) eff = fit;

    int w = std::max(1, static_cast<int>(src.GetWidth()  * eff));
    int h = std::max(1, static_cast<int>(src.GetHeight() * eff));

    return wxBitmap(src.Scale(w, h, wxIMAGE_QUALITY_HIGH));
}

void MainFrame::SetSecondImage(bool second)
{
    if (useSecondImage == second) return;
    useSecondImage = second;
    m_panel->Refresh();          // только перерисовка готового битмапа
}

//-----------------------------------------------------------------------------
// Отрисовка: строго по центру клиентской области + offsetX/offsetY.
//-----------------------------------------------------------------------------
void MainFrame::OnPaint(wxPaintEvent&)
{
    wxPaintDC dc(m_panel);
    dc.SetBackground(wxBrush(m_panel->GetBackgroundColour()));
    dc.Clear();

    const wxBitmap& bmp = ActiveBitmap();
    if (!bmp.IsOk()) return;

    wxSize ps = m_panel->GetClientSize();
    int x = (ps.GetWidth()  - bmp.GetWidth())  / 2 + m_cfg.offsetX;
    int y = (ps.GetHeight() - bmp.GetHeight()) / 2 + m_cfg.offsetY;
    dc.DrawBitmap(bmp, x, y, true);
}

void MainFrame::OnLevelTimer(wxTimerEvent&)
{
    if (IsBeingDeleted()) return;

    const float rms = m_audio.GetLevel();

    // rms == 0 -> -inf -> -120 dB, чтобы избежать log10(0)
    const float levelDb = (rms > 1e-6f)
        ? 20.0f * std::log10(rms)
        : -120.0f;

    const float threshold = m_cfg.threshold;
    const float hystDb      = 3.0f;   // гистерезис 3 дБ — без мерцания

    if (!useSecondImage && levelDb > threshold)
        SetSecondImage(true);
    else if (useSecondImage && levelDb < threshold - hystDb)
        SetSecondImage(false);
}

void MainFrame::OnErase(wxEraseEvent&) {}