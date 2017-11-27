#define GL_GLEXT_PROTOTYPES 1
#undef __glext_h_
#include "GL/gl.h"
#include "GL/glext.h"


#include "wx/glcanvas.h"
#include <gtk-3.0/gtk/gtk.h>
#include <gtk-3.0/gdk/gdk.h>

#include "wx/gtk/glcanvas.h"
#include "glcanvas.h"

#ifdef WX_USE_GDK_GL_CONTEXT

wxIMPLEMENT_CLASS(wxGLContextGdk, wxObject);

wxGLContextGdk::wxGLContextGdk(wxGLCanvasNew* win, const wxGLContextGdk* other, const wxGLContextAttrs* ctxAttrs)
    : m_glContext(NULL)
{
    GtkWidget* widget = win->m_widget;
    if(!gtk_widget_get_realized(widget))
        return;
    GdkWindow* window = win->GTKGetDrawingWindow();
    GError* error = NULL;
    m_glContext = gdk_window_create_gl_context(window, &error);
    gdk_gl_context_realize(m_glContext, &error);
    win->needs_resize = TRUE;
}

wxGLContextGdk::~wxGLContextGdk()
{
    if(!m_glContext)
        return;

    gdk_gl_context_clear_current();
    g_clear_object(&m_glContext);
}

bool wxGLContextGdk::SetCurrent(const wxGLCanvasNew& win) const
{
    if(!m_glContext)
    {
        GtkWidget* widget = win.m_widget;
        if(!gtk_widget_get_realized(widget))
            return false;
        GdkWindow* window = win.GTKGetDrawingWindow();
        GError* error = NULL;
        m_glContext = gdk_window_create_gl_context(window, &error);
        gdk_gl_context_realize(m_glContext, &error);
    }

    gdk_gl_context_make_current(m_glContext);
    GdkWindow* window = win.GTKGetDrawingWindow();
    GdkWindow* ctx_window = gdk_gl_context_get_window(m_glContext);
    if (ctx_window!=window)
    {
        int i=0;
    }
    const_cast<wxGLCanvasNew&>(win).attach_buffers();
}



wxGLCanvasGdkNew::wxGLCanvasGdkNew() :
    have_buffers(FALSE),
    frame_buffer(0),
    render_buffer(0),
    depth_stencil_buffer(0),
    needs_resize(FALSE),
    old_w(0),
    old_h(0)
{
}

wxGLCanvasGdkNew::~wxGLCanvasGdkNew()
{
}

void wxGLCanvasGdkNew::ensure_buffers ()
{
    if (have_buffers)
        return;

    have_buffers = TRUE;

    glGenFramebuffers(1, &frame_buffer);

    /* For non-alpha we use render buffers so we can blit instead of texture the result */
    if (render_buffer == 0)
        glGenRenderbuffers(1, &render_buffer);


    if (depth_stencil_buffer == 0)
        glGenRenderbuffers (1, &depth_stencil_buffer);
    allocate_buffers ();
}


void wxGLCanvasGdkNew::allocate_buffers ()
{
    GdkGLContext* context = gdk_gl_context_get_current();
    if(!context)
        return;

    GdkWindow* window = GTKGetDrawingWindow(); //gdk_gl_context_get_window(context);
    int scale = gdk_window_get_scale_factor(window);
    int width = gdk_window_get_width(window)*scale;
    int height = gdk_window_get_height(window)*scale;

    if (render_buffer) {
        glBindRenderbuffer (GL_RENDERBUFFER, render_buffer);
        glRenderbufferStorage (GL_RENDERBUFFER, GL_RGB8, width, height);
    }

    glBindRenderbuffer (GL_RENDERBUFFER, depth_stencil_buffer);
    glRenderbufferStorage (GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);

    needs_render = TRUE;
}


void wxGLCanvasGdkNew::attach_buffers ()
{
    GdkGLContext* context = gdk_gl_context_get_current();
    if(!context)
        return;
    GdkWindow* window = GTKGetDrawingWindow(); //gdk_gl_context_get_window(context);
    int w = gdk_window_get_width(window);
    int h = gdk_window_get_height(window);
    if (old_w!=w || old_h!=h)
    {
        needs_resize = TRUE;
        old_h = h;
        old_w = w;
    }

    if (!have_buffers)
        ensure_buffers ();
    else if (needs_resize)
        allocate_buffers ();
    needs_resize = FALSE;
    glBindFramebuffer (GL_FRAMEBUFFER, frame_buffer);

    if (render_buffer)
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                  GL_RENDERBUFFER, render_buffer);

    if (depth_stencil_buffer) {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                      GL_RENDERBUFFER, depth_stencil_buffer);
    }
}


bool wxGLCanvasGdkNew::SwapBuffers()
{
    GdkGLContext* context = gdk_gl_context_get_current();
    if(!context)
        return false;
    GLenum status;

    GtkWidget* widget = m_widget;
    if(!gtk_widget_get_realized(widget))
        return false;
    GdkWindow* window = GTKGetDrawingWindow();
    //GdkWindow* window = gdk_gl_context_get_window(context);
    int scale = gdk_window_get_scale_factor(window);
    int w = gdk_window_get_width(window)*scale;
    int h = gdk_window_get_height(window)*scale;

    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status == GL_FRAMEBUFFER_COMPLETE)
    {
        needs_render = FALSE;

//        wxGLCanvasNew *pChild = dynamic_cast<wxGLCanvasNew*>(this);
//        if (!pChild || !pChild->m_cairoPaintContext)
//            return false;
#if 0
        cairo_t* cr;// = pChild->m_cairoPaintContext; //gdk_cairo_create(window);
        GdkWindow* cr_window = gdk_gl_context_get_window(context);
        //window = cr_window;
        cairo_region_t* region = gdk_window_get_clip_region(window);
        GdkDrawingContext* context = gdk_window_begin_draw_frame(window,region);
        cr = gdk_drawing_context_get_cairo_context(context);//gdk_cairo_create(window);
        gdk_cairo_draw_from_gl(cr, cr_window, render_buffer,
                               GL_RENDERBUFFER, scale, 0, 0, w, h);
        gdk_window_end_draw_frame(window,context);
        cairo_region_destroy(region);
        //cairo_destroy(cr);
#else
//        wxGLCanvasNew *pChild = dynamic_cast<wxGLCanvasNew*>(this);
//        if (!pChild || !pChild->m_cairoPaintContext)
//            return false;
        cairo_t* cr = gdk_cairo_create(window);
        GdkWindow* cr_window = gdk_gl_context_get_window(context);
        gdk_cairo_draw_from_gl(cr, cr_window, render_buffer,
                               GL_RENDERBUFFER, scale, 0, 0, w, h);
        cairo_destroy(cr);
#endif
        return true;
    }
    return false;
}


bool wxGLCanvasGdkNew::InitVisual(const wxGLAttributes& dispAttrs)
{
    return true;
}

/* static */
bool wxGLCanvasBase::IsDisplaySupported(const wxGLAttributes& dispAttrs)
{
    return true;
}

/* static */
bool wxGLCanvasBase::IsDisplaySupported(const int* attribList)
{
    return true;
}

#include "GdkGLContextAttrs.cpp"

#endif
