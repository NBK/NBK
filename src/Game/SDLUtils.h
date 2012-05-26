#ifndef SDL_UTILS_H
#define SDL_UTILS_H
#ifndef WIN32

#include <SDL.h>
#include <SDL_image.h>

#define VK_RETURN   SDLK_RETURN
#define VK_BACK     SDLK_BACKSPACE
#define VK_PRIOR    SDLK_PAGEUP
#define VK_LEFT     SDLK_LEFT
#define VK_RIGHT    SDLK_RIGHT
#define VK_UP       SDLK_UP
#define VK_DOWN     SDLK_DOWN
#define VK_ESCAPE   SDLK_ESCAPE
#define VK_SPACE    SDLK_SPACE
#define VK_SHIFT    SDLK_RSHIFT // SDLK_LSHIFT
#define VK_TAB      SDLK_TAB
#define VK_CONTROL  SDLK_RCTRL // SDLK_LCTRL
#define VK_ADD      SDLK_PLUS
#define VK_SUBTRACT SDLK_MINUS
#define VK_F1       SDLK_F1
#define VK_F2       SDLK_F2
#define VK_F3       SDLK_F3
#define VK_F4       SDLK_F4
#define VK_F5       SDLK_F5
#define VK_F6       SDLK_F6
#define VK_F7       SDLK_F7
#define VK_F8       SDLK_F8
#define VK_F9       SDLK_F9
#define VK_F10      SDLK_F10
#define VK_F11      SDLK_F11
#define VK_F12      SDLK_F12
#define VK_INSERT   SDLK_INSERT
#define VK_HOME     SDLK_HOME
#define VK_END      SDLK_END

#define itoa(x,y,z) SDL_itoa(x,y,z)

#endif
#endif // SDL_UTILS_H
