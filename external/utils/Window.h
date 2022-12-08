// Copyright 2011-2020 the Polygon Mesh Processing Library developers.
// Distributed under a MIT-style license, see LICENSE.txt for details.

#pragma once

#include "gl.h"
#include "glmath.h"
#include <vector>
#include <utility>
#include <GLFW/glfw3.h>


//! A window provided by GLFW
//! \ingroup visualization
class Window
{
public:
    //! constructor
    Window(const char* title, int width, int height);

    //! destructor
    virtual ~Window();

    //! main window loop
    int run();

private:
    static void glfw_error(int error, const char* description);
    static void glfw_keyboard(GLFWwindow* window, int key, int scancode,
                              int action, int mods);
    static void glfw_character(GLFWwindow* window, unsigned int c);
    static void glfw_mouse(GLFWwindow* window, int button, int action,
                           int mods);
    static void glfw_motion(GLFWwindow* window, double xpos, double ypos);
    static void glfw_scroll(GLFWwindow* window, double xoffset, double yoffset);
    static void glfw_resize(GLFWwindow* window, int width, int height);

    static void render_frame();

    static Window* instance_;

protected:
    //! must overload: initialize all OpenGL states
    virtual void initialize() = 0;

    //! this function is called when the scene has to be rendered. it
    //! clears the buffers, calls the render() method, and performs buffer swap
    virtual void render(void) = 0;

    //! this function handles keyboard events
    virtual void keyboard(int /*key*/, int /*code*/, int /*action*/,
                          int /*mods*/);

    //! this function handles unicode character events
    virtual void character(unsigned int);

    //! this function handles mouse button events
    virtual void mouse(int /*button*/, int /*action*/, int /*mods*/) {}

    //! this function handles mouse motion (passive/active position)
    virtual void motion(double /*xpos*/, double /*ypos*/) {}

    //! this function handles mouse scroll events
    virtual void scroll(double /*xoffset*/, double /*yoffset*/) {}

    //! this function is called if the window is resized
    virtual void resize(int /*width*/, int /*height*/) {}

    //! this function is called just before rendering
    virtual void timer() {}

protected:
    //! width of window
    int width() const { return width_; }
    //! height of window
    int height() const { return height_; }

    //! virtual trackball: map 2D screen point to unit sphere. used by rotate().
    bool map_to_sphere(const int point_x, const int point_y, vec3& result);

    //! highDPI scaling
    float high_dpi_scaling() const { return scaling_; }

    //! get position of mouse cursor
    void cursor_pos(double& x, double& y) const;

    //! is left mouse button pressed down?
    bool left_mouse_pressed() const 
    { 
        return button_[GLFW_MOUSE_BUTTON_LEFT]; 
    }
    //! is right mouse button pressed down?
    bool right_mouse_pressed() const
    {
        return button_[GLFW_MOUSE_BUTTON_RIGHT];
    }
    //! is middle mouse button pressed down?
    bool middle_mouse_pressed() const
    {
        return button_[GLFW_MOUSE_BUTTON_MIDDLE];
    }

    //! is CTRL modifier key pressed down?
    bool ctrl_pressed() const { return ctrl_pressed_; }
    //! is ALT modifier key pressed down?
    bool alt_pressed() const { return alt_pressed_; }
    //! is SHIFT modifier key pressed down?
    bool shift_pressed() const { return shift_pressed_; }

private:
    //! GLFW window pointer
    GLFWwindow* window_;

    //! current viewport dimension
    int width_, height_;

    //! highDPI scaling
    float scaling_;
#if __EMSCRIPTEN__
    float pixel_ratio_;
#endif

    // which mouse buttons and modifier keys are pressed down
    bool button_[7], ctrl_pressed_, alt_pressed_, shift_pressed_;

    // fullscreen-related backups
    int backup_xpos_, backup_ypos_, backup_width_, backup_height_;
    bool is_fullscreen() const;
    void enter_fullscreen();
    void exit_fullscreen();
};
