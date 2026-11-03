#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/fileconf.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include "MainFrame.h"

class App : public wxApp
{
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    wxImage::AddHandler(new wxPNGHandler);

    AppSettings s;
    wxString img1Path = "image.png";
    wxString img2Path = "image2.png";                          // дефолты уже заданы
        {
            wxString exePath = wxStandardPaths::Get().GetExecutablePath();
            wxString baseDir;
                    
            #ifdef __APPLE__
                wxFileName f(exePath);
                f.RemoveLastDir();   // MacOS     -> Contents
                f.RemoveLastDir();   // Contents  -> MyApp.app
                f.RemoveLastDir();   // MyApp.app -> <родитель бандла>
                baseDir = f.GetPath();
            #else
                baseDir = wxFileName(exePath).GetPath();
            #endif
                    
            wxSetWorkingDirectory(baseDir);
            wxString cfgPath = wxFileName(baseDir, "config.txt").GetFullPath();

            wxFileConfig cfg(wxEmptyString, wxEmptyString, cfgPath, wxEmptyString,
                             wxCONFIG_USE_LOCAL_FILE);

            cfg.Read("/Window/Width",         &s.windowWidth,    s.windowWidth);
            cfg.Read("/Window/Height",        &s.windowHeight,   s.windowHeight);
            cfg.Read("/Image/Image1",         &img1Path,         img1Path);
            cfg.Read("/Image/Image2",         &img2Path,         img2Path);
            cfg.Read("/Image/OffsetX",        &s.offsetX,        s.offsetX);
            cfg.Read("/Image/OffsetY",        &s.offsetY,        s.offsetY);
            cfg.Read("/Image/Scale",          &s.scale,          s.scale);
            cfg.Read("/Audio/Threshold", &s.threshold,  s.threshold);
        }
    
    wxImage img1, img2;
    if (wxFileExists(img1Path)) img1.LoadFile(img1Path, wxBITMAP_TYPE_PNG);
    if (wxFileExists(img2Path)) img2.LoadFile(img2Path, wxBITMAP_TYPE_PNG);
    
    MainFrame* main_frame = new MainFrame(s, img1, img2);
    main_frame->Show(true);

    return true;
}