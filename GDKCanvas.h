#ifndef _WX_GTK_GDKCANVAS_NEW_H
#define _WX_GTK_GDKCANVAS_NEW_H

#include <GL/gl.h>
#include <gdk/gdk.h>

#include <X11/Xutil.h>

// ----------------------------------------------------------------------------
// wxGLContextGdk
// ----------------------------------------------------------------------------

class wxGLCanvasNew;
class wxGLCanvasGdkNew;

class wxGLContextGdk : public wxGLContextBase
{
public:
    wxGLContextGdk(wxGLCanvasGdkNew *win,
                const wxGLContextGdk *other = NULL,
                const wxGLContextAttrs *ctxAttrs = NULL);
    virtual ~wxGLContextGdk();

    virtual bool SetCurrent(const wxGLCanvasGdkNew& win) const/* wxOVERRIDE*/;
    virtual bool SetCurrent(const wxGLCanvas& win) const {}/* wxOVERRIDE*/;

private:
    mutable GdkGLContext* m_glContext;

    wxDECLARE_CLASS(wxGLContextGdk);
};

// ----------------------------------------------------------------------------
// wxGLCanvasGdkNew
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_GL wxGLCanvasGdkNew : public wxGLCanvasBase
{
public:
    // initialization and dtor
    // -----------------------

    // default ctor doesn't do anything, InitVisual() must be called
    wxGLCanvasGdkNew();

    bool InitVisual(const wxGLAttributes& dispAttrs);

    virtual ~wxGLCanvasGdkNew();
    XVisualInfo *GetXVisualInfo() const { return NULL; }


    // implement wxGLCanvasBase methods
    // --------------------------------

    virtual bool SwapBuffers() wxOVERRIDE;
    gboolean needs_resize;
    void attach_buffers();
private:
    void allocate_buffers();
    void ensure_buffers();
    gboolean have_buffers;
    guint frame_buffer;
    guint render_buffer;
    guint depth_stencil_buffer;

    gboolean needs_render;
    
    int old_w;
    int old_h;
};

#endif 
