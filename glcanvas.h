/////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk/glcanvas.h
// Purpose:     wxGLCanvasNew, for using OpenGL/Mesa with wxWidgets and GTK
// Author:      Robert Roebling
// Modified by:
// Created:     17/8/98
// Copyright:   (c) Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GLCANVAS_NEW_H_
#define _WX_GLCANVAS_NEW_H_

#undef WX_USE_GDK_GL_CONTEXT
#define WX_USE_GDK_GL_CONTEXT

#ifndef WX_USE_GDK_GL_CONTEXT
#include "glx11.h"
#else
#include "GDKCanvas.h"
#include "glEgl.h"
#endif

//---------------------------------------------------------------------------
// wxGLCanvasNew
//---------------------------------------------------------------------------

#ifndef WX_USE_GDK_GL_CONTEXT
typedef wxGLContextX11_ wxGLContextNew;
class wxGLCanvasNew : public wxGLCanvasX11_
#else
typedef wxGLContextEgl wxGLContextNew;
class wxGLCanvasNew : public wxGLCanvasEgl
#endif
{
public:
    wxGLCanvasNew(wxWindow *parent,
               const wxGLAttributes& dispAttrs,
               wxWindowID id = wxID_ANY,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = 0,
               const wxString& name = wxGLCanvasName,
               const wxPalette& palette = wxNullPalette);

    explicit // avoid implicitly converting a wxWindow* to wxGLCanvasNew
    wxGLCanvasNew(wxWindow *parent,
               wxWindowID id = wxID_ANY,
               const int *attribList = NULL,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = 0,
               const wxString& name = wxGLCanvasName,
               const wxPalette& palette = wxNullPalette);

    bool Create(wxWindow *parent,
                const wxGLAttributes& dispAttrs,
                wxWindowID id = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxGLCanvasName,
                const wxPalette& palette = wxNullPalette);

    bool Create(wxWindow *parent,
                wxWindowID id = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxGLCanvasName,
                const int *attribList = NULL,
                const wxPalette& palette = wxNullPalette);

    virtual bool SetBackgroundStyle(wxBackgroundStyle style) wxOVERRIDE;

    // implement wxGLCanvasX11 methods
    // --------------------------------
#ifndef WX_USE_GDK_GL_CONTEXT
    virtual Window GetXWindow() const wxOVERRIDE;
#endif

    // deprecated methods
    // ------------------

#if WXWIN_COMPATIBILITY_2_8
    wxDEPRECATED(
    wxGLCanvasNew(wxWindow *parent,
               wxWindowID id = wxID_ANY,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = 0,
               const wxString& name = wxGLCanvasName,
               const int *attribList = NULL,
               const wxPalette& palette = wxNullPalette)
    );

    wxDEPRECATED(
    wxGLCanvasNew(wxWindow *parent,
               const wxGLContextNew *shared,
               wxWindowID id = wxID_ANY,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = 0,
               const wxString& name = wxGLCanvasName,
               const int *attribList = NULL,
               const wxPalette& palette = wxNullPalette)
    );

    wxDEPRECATED(
    wxGLCanvasNew(wxWindow *parent,
               const wxGLCanvasNew *shared,
               wxWindowID id = wxID_ANY,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = 0,
               const wxString& name = wxGLCanvasName,
               const int *attribList = NULL,
               const wxPalette& palette = wxNullPalette)
    );

    // called from "realized" callback to create the implicit context if needed
    void GTKInitImplicitContext();
#endif // WXWIN_COMPATIBILITY_2_8

    // implementation from now on
    void OnInternalIdle() wxOVERRIDE;

    bool              m_exposed;
#ifdef __WXGTK3__
    cairo_t* m_cairoPaintContext;
#endif

#if WXWIN_COMPATIBILITY_2_8
    wxGLContextNew      *m_sharedContext;
    wxGLCanvasNew       *m_sharedContextOf;
    const bool        m_createImplicitContext;
#endif // WXWIN_COMPATIBILITY_2_8

private:
    wxDECLARE_CLASS(wxGLCanvasNew);
};

#endif // _WX_GLCANVAS_H_

