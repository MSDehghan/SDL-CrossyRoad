#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include <iostream>
#include <string>
#include <math.h>

using namespace std;

typedef SDL_Texture G_Texture;
typedef SDL_Rect G_Rect;
typedef TTF_Font G_Font;
typedef Mix_Music G_Music;
typedef Mix_Chunk G_Sound;



#define G_Keyboard event.key.keysym.sym
#define G_Mouse event.button.button

#define G_motion event.motion
#define G_motion event.motion



enum
{
	GK_UNKNOWN = 0,

	GK_RETURN = '\r',
	GK_ESCAPE = '\033',
	GK_BACKSPACE = '\b',
	GK_TAB = '\t',
	GK_SPACE = ' ',
	GK_EXCLAIM = '!',
	GK_QUOTEDBL = '"',
	GK_HASH = '#',
	GK_PERCENT = '%',
	GK_DOLLAR = '$',
	GK_AMPERSAND = '&',
	GK_QUOTE = '\'',
	GK_LEFTPAREN = '(',
	GK_RIGHTPAREN = ')',
	GK_ASTERISK = '*',
	GK_PLUS = '+',
	GK_COMMA = ',',
	GK_MINUS = '-',
	GK_PERIOD = '.',
	GK_SLASH = '/',
	GK_0 = '0',
	GK_1 = '1',
	GK_2 = '2',
	GK_3 = '3',
	GK_4 = '4',
	GK_5 = '5',
	GK_6 = '6',
	GK_7 = '7',
	GK_8 = '8',
	GK_9 = '9',
	GK_COLON = ':',
	GK_SEMICOLON = ';',
	GK_LESS = '<',
	GK_EQUALS = '=',
	GK_GREATER = '>',
	GK_QUESTION = '?',
	GK_AT = '@',
	/*
	Skip uppercase letters
	*/
	GK_LEFTBRACKET = '[',
	GK_BACKSLASH = '\\',
	GK_RIGHTBRACKET = ']',
	GK_CARET = '^',
	GK_UNDERSCORE = '_',
	GK_BACKQUOTE = '`',
	GK_a = 'a',
	GK_b = 'b',
	GK_c = 'c',
	GK_d = 'd',
	GK_e = 'e',
	GK_f = 'f',
	GK_g = 'g',
	GK_h = 'h',
	GK_i = 'i',
	GK_j = 'j',
	GK_k = 'k',
	GK_l = 'l',
	GK_m = 'm',
	GK_n = 'n',
	GK_o = 'o',
	GK_p = 'p',
	GK_q = 'q',
	GK_r = 'r',
	GK_s = 's',
	GK_t = 't',
	GK_u = 'u',
	GK_v = 'v',
	GK_w = 'w',
	GK_x = 'x',
	GK_y = 'y',
	GK_z = 'z',

	GK_CAPSLOCK = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CAPSLOCK),

	GK_F1 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F1),
	GK_F2 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F2),
	GK_F3 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F3),
	GK_F4 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F4),
	GK_F5 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F5),
	GK_F6 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F6),
	GK_F7 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F7),
	GK_F8 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F8),
	GK_F9 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F9),
	GK_F10 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F10),
	GK_F11 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F11),
	GK_F12 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F12),

	GK_PRINTSCREEN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PRINTSCREEN),
	GK_SCROLLLOCK = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SCROLLLOCK),
	GK_PAUSE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAUSE),
	GK_INSERT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_INSERT),
	GK_HOME = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_HOME),
	GK_PAGEUP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAGEUP),
	GK_DELETE = '\177',
	GK_END = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_END),
	GK_PAGEDOWN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAGEDOWN),
	GK_RIGHT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RIGHT),
	GK_LEFT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LEFT),
	GK_DOWN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DOWN),
	GK_UP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_UP),

	GK_NUMLOCKCLEAR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMLOCKCLEAR),
	GK_KP_DIVIDE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DIVIDE),
	GK_KP_MULTIPLY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MULTIPLY),
	GK_KP_MINUS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MINUS),
	GK_KP_PLUS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PLUS),
	GK_KP_ENTER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_ENTER),
	GK_KP_1 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_1),
	GK_KP_2 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_2),
	GK_KP_3 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_3),
	GK_KP_4 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_4),
	GK_KP_5 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_5),
	GK_KP_6 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_6),
	GK_KP_7 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_7),
	GK_KP_8 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_8),
	GK_KP_9 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_9),
	GK_KP_0 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_0),
	GK_KP_PERIOD = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PERIOD),

	GK_APPLICATION = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_APPLICATION),
	GK_POWER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_POWER),
	GK_KP_EQUALS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EQUALS),
	GK_F13 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F13),
	GK_F14 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F14),
	GK_F15 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F15),
	GK_F16 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F16),
	GK_F17 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F17),
	GK_F18 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F18),
	GK_F19 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F19),
	GK_F20 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F20),
	GK_F21 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F21),
	GK_F22 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F22),
	GK_F23 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F23),
	GK_F24 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F24),
	GK_EXECUTE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EXECUTE),
	GK_HELP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_HELP),
	GK_MENU = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MENU),
	GK_SELECT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SELECT),
	GK_STOP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_STOP),
	GK_AGAIN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AGAIN),
	GK_UNDO = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_UNDO),
	GK_CUT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CUT),
	GK_COPY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_COPY),
	GK_PASTE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PASTE),
	GK_FIND = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_FIND),
	GK_MUTE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MUTE),
	GK_VOLUMEUP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_VOLUMEUP),
	GK_VOLUMEDOWN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_VOLUMEDOWN),
	GK_KP_COMMA = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_COMMA),
	GK_KP_EQUALSAS400 =
	SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EQUALSAS400),

	GK_ALTERASE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_ALTERASE),
	GK_SYSREQ = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SYSREQ),
	GK_CANCEL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CANCEL),
	GK_CLEAR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CLEAR),
	GK_PRIOR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PRIOR),
	GK_RETURN2 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RETURN2),
	GK_SEPARATOR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SEPARATOR),
	GK_OUT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_OUT),
	GK_OPER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_OPER),
	GK_CLEARAGAIN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CLEARAGAIN),
	GK_CRSEL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CRSEL),
	GK_EXSEL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EXSEL),

	GK_KP_00 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_00),
	GK_KP_000 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_000),
	GK_THOUSANDSSEPARATOR =
	SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_THOUSANDSSEPARATOR),
	GK_DECIMALSEPARATOR =
	SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DECIMALSEPARATOR),
	GK_CURRENCYUNIT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CURRENCYUNIT),
	GK_CURRENCYSUBUNIT =
	SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CURRENCYSUBUNIT),
	GK_KP_LEFTPAREN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LEFTPAREN),
	GK_KP_RIGHTPAREN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_RIGHTPAREN),
	GK_KP_LEFTBRACE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LEFTBRACE),
	GK_KP_RIGHTBRACE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_RIGHTBRACE),
	GK_KP_TAB = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_TAB),
	GK_KP_BACKSPACE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_BACKSPACE),
	GK_KP_A = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_A),
	GK_KP_B = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_B),
	GK_KP_C = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_C),
	GK_KP_D = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_D),
	GK_KP_E = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_E),
	GK_KP_F = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_F),
	GK_KP_XOR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_XOR),
	GK_KP_POWER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_POWER),
	GK_KP_PERCENT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PERCENT),
	GK_KP_LESS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LESS),
	GK_KP_GREATER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_GREATER),
	GK_KP_AMPERSAND = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_AMPERSAND),
	GK_KP_DBLAMPERSAND =
	SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DBLAMPERSAND),
	GK_KP_VERTICALBAR =
	SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_VERTICALBAR),
	GK_KP_DBLVERTICALBAR =
	SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DBLVERTICALBAR),
	GK_KP_COLON = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_COLON),
	GK_KP_HASH = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_HASH),
	GK_KP_SPACE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_SPACE),
	GK_KP_AT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_AT),
	GK_KP_EXCLAM = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EXCLAM),
	GK_KP_MEMSTORE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMSTORE),
	GK_KP_MEMRECALL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMRECALL),
	GK_KP_MEMCLEAR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMCLEAR),
	GK_KP_MEMADD = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMADD),
	GK_KP_MEMSUBTRACT =
	SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMSUBTRACT),
	GK_KP_MEMMULTIPLY =
	SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMMULTIPLY),
	GK_KP_MEMDIVIDE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMDIVIDE),
	GK_KP_PLUSMINUS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PLUSMINUS),
	GK_KP_CLEAR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_CLEAR),
	GK_KP_CLEARENTRY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_CLEARENTRY),
	GK_KP_BINARY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_BINARY),
	GK_KP_OCTAL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_OCTAL),
	GK_KP_DECIMAL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DECIMAL),
	GK_KP_HEXADECIMAL =
	SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_HEXADECIMAL),

	GK_LCTRL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LCTRL),
	GK_LSHIFT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LSHIFT),
	GK_LALT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LALT),
	GK_LGUI = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LGUI),
	GK_RCTRL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RCTRL),
	GK_RSHIFT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RSHIFT),
	GK_RALT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RALT),
	GK_RGUI = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RGUI),

	GK_MODE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MODE),

	GK_AUDIONEXT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIONEXT),
	GK_AUDIOPREV = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOPREV),
	GK_AUDIOSTOP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOSTOP),
	GK_AUDIOPLAY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOPLAY),
	GK_AUDIOMUTE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOMUTE),
	GK_MEDIASELECT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIASELECT),
	GK_WWW = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_WWW),
	GK_MAIL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MAIL),
	GK_CALCULATOR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CALCULATOR),
	GK_COMPUTER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_COMPUTER),
	GK_AC_SEARCH = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_SEARCH),
	GK_AC_HOME = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_HOME),
	GK_AC_BACK = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_BACK),
	GK_AC_FORWARD = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_FORWARD),
	GK_AC_STOP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_STOP),
	GK_AC_REFRESH = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_REFRESH),
	GK_AC_BOOKMARKS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_BOOKMARKS),

	GK_BRIGHTNESSDOWN =
	SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_BRIGHTNESSDOWN),
	GK_BRIGHTNESSUP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_BRIGHTNESSUP),
	GK_DISPLAYSWITCH = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DISPLAYSWITCH),
	GK_KBDILLUMTOGGLE =
	SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KBDILLUMTOGGLE),
	GK_KBDILLUMDOWN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KBDILLUMDOWN),
	GK_KBDILLUMUP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KBDILLUMUP),
	GK_EJECT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EJECT),
	GK_SLEEP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SLEEP)
};
typedef enum
{
	G_FIRSTEVENT = 0,     /**< Unused (do not remove) */

	/* Application events */
	G_QUIT = 0x100, /**< User-requested quit */

	/* These application events have special meaning on iOS, see README-ios.txt for details */
	G_APP_TERMINATING,        /**< The application is being terminated by the OS
	Called on iOS in applicationWillTerminate()
	Called on Android in onDestroy()
	*/
	G_APP_LOWMEMORY,          /**< The application is low on memory, free memory if possible.
	Called on iOS in applicationDidReceiveMemoryWarning()
	Called on Android in onLowMemory()
	*/
	G_APP_WILLENTERBACKGROUND, /**< The application is about to enter the background
	Called on iOS in applicationWillResignActive()
	Called on Android in onPause()
	*/
	G_APP_DIDENTERBACKGROUND, /**< The application did enter the background and may not get CPU for some time
	Called on iOS in applicationDidEnterBackground()
	Called on Android in onPause()
	*/
	G_APP_WILLENTERFOREGROUND, /**< The application is about to enter the foreground
	Called on iOS in applicationWillEnterForeground()
	Called on Android in onResume()
	*/
	G_APP_DIDENTERFOREGROUND, /**< The application is now interactive
	Called on iOS in applicationDidBecomeActive()
	Called on Android in onResume()
	*/

	/* Window events */
	G_WINDOWEVENT = 0x200, /**< Window state change */
	G_SYSWMEVENT,             /**< System specific event */

	/* Keyboard events */
	G_KEYDOWN = 0x300, /**< Key pressed */
	G_KEYUP,                  /**< Key released */
	G_TEXTEDITING,            /**< Keyboard text editing (composition) */
	G_TEXTINPUT,              /**< Keyboard text input */

	/* Mouse events */
	G_MOUSEMOTION = 0x400, /**< Mouse moved */
	G_MOUSEBUTTONDOWN,        /**< Mouse button pressed */
	G_MOUSEBUTTONUP,          /**< Mouse button released */
	G_MOUSEWHEEL,             /**< Mouse wheel motion */

	/* Joystick events */
	G_JOYAXISMOTION = 0x600, /**< Joystick axis motion */
	G_JOYBALLMOTION,          /**< Joystick trackball motion */
	G_JOYHATMOTION,           /**< Joystick hat position change */
	G_JOYBUTTONDOWN,          /**< Joystick button pressed */
	G_JOYBUTTONUP,            /**< Joystick button released */
	G_JOYDEVICEADDED,         /**< A new joystick has been inserted into the system */
	G_JOYDEVICEREMOVED,       /**< An opened joystick has been removed */

	/* Game controller events */
	G_CONTROLLERAXISMOTION = 0x650, /**< Game controller axis motion */
	G_CONTROLLERBUTTONDOWN,          /**< Game controller button pressed */
	G_CONTROLLERBUTTONUP,            /**< Game controller button released */
	G_CONTROLLERDEVICEADDED,         /**< A new Game controller has been inserted into the system */
	G_CONTROLLERDEVICEREMOVED,       /**< An opened Game controller has been removed */
	G_CONTROLLERDEVICEREMAPPED,      /**< The controller mapping was updated */

	/* Touch events */
	G_FINGERDOWN = 0x700,
	G_FINGERUP,
	G_FINGERMOTION,

	/* Gesture events */
	G_DOLLARGESTURE = 0x800,
	G_DOLLARRECORD,
	G_MULTIGESTURE,

	/* Clipboard events */
	G_CLIPBOARDUPDATE = 0x900, /**< The clipboard changed */

	/* Drag and drop events */
	G_DROPFILE = 0x1000, /**< The system requests a file open */

	/* Render events */
	G_RENDER_TARGETS_RESET = 0x2000, /**< The render targets have been reset */

	/** Events ::G_USEREVENT through ::G_LASTEVENT are for your use,
	*  and should be allocated with G_RegisterEvents()
	*/
	G_USEREVENT = 0x8000,

	/**
	*  This last event is only for bounding internal arrays
	*/
	G_LASTEVENT = 0xFFFF
} G_EventType;

#define G_BUTTON(X)       (1 << ((X)-1))
#define G_BUTTON_LEFT     1
#define G_BUTTON_MIDDLE   2
#define G_BUTTON_RIGHT    3
#define G_BUTTON_X1       4
#define G_BUTTON_X2       5


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event event;

//update sceen
void G_Update()
{
	SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);
}
//event handling
int G_Event()
{
	if (SDL_PollEvent(&event) != 0)
	{
		return event.type;
	}
	return 0;
}

//init all SDL
bool G_InitSDL()
{
	if ((SDL_Init(SDL_INIT_EVERYTHING) == 0) && (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == 0) && (TTF_Init() == 0))
		return true;
	return false;
}
//creat window and set renderer
bool G_CreateWindow(char* title, G_Rect win, Uint8 r, Uint8 g, Uint8 b, int flag = 0)
{
	window = SDL_CreateWindow(title, win.x, win.y, win.w, win.h, flag);
	if (window != 0)
	{
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer != 0)
		{
			SDL_SetRenderDrawColor(renderer, r, g, b, 255);
			return true;
		}
	}
	return false;
}
//load image with any format and overloaded to remove rgb color from image
SDL_Texture* G_LoadImage(const char* file)
{
	SDL_Surface* surface = IMG_Load(file);
	if (surface != 0)
	{
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		if (texture != 0)
		{
			return texture;
		}
	}
	cout << "ah shit" << file << endl;
	return NULL;
}
SDL_Texture* G_LoadImage(const char *file, Uint8 r, Uint8 g, Uint8 b)
{
	SDL_Surface* surface = IMG_Load(file);
	if (surface != 0)
	{
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, r, g, b));
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		if (texture != 0)
		{
			return texture;
		}
	}
	return NULL;
}
//draw functions(over loaded) based on source and destination kinds (all 4 possible cases)
void G_Draw(G_Texture* texture, G_Rect *src, G_Rect *dst, bool fullscreen = false)
{
	if (!fullscreen)
		SDL_RenderCopy(renderer, texture, src, dst);
	else
		SDL_RenderCopy(renderer, texture, src, 0);
}
void G_Draw(G_Texture* texture, G_Rect *dst, bool fullscreen = false)
{
	if (!fullscreen)
		SDL_RenderCopy(renderer, texture, 0, dst);
	else
		SDL_RenderCopy(renderer, texture, 0, 0);
}
void G_DrawEx(G_Texture* texture, G_Rect *src, G_Rect *dst, SDL_RendererFlip flip, bool fullscreen = false)
{
	if (!fullscreen)
		SDL_RenderCopyEx(renderer, texture, src, dst, 0, 0, flip);
	else
		SDL_RenderCopyEx(renderer, texture, src, 0, 0, 0, flip);
}
void G_DrawEx(G_Texture* texture, G_Rect *dst, SDL_RendererFlip flip, bool fullscreen = false)
{
	if (!fullscreen)
		SDL_RenderCopyEx(renderer, texture, 0, dst, 0, 0, flip);
	else
		SDL_RenderCopyEx(renderer, texture, 0, 0, 0, 0, flip);
}



//font
TTF_Font* G_OpenFont(char* file, int fontSize)
{
	return TTF_OpenFont(file, fontSize);
}
SDL_Texture* G_LoadFont(G_Font* font, char* title, Uint8 r, Uint8 g, Uint8 b)
{
	SDL_Color color;
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = 255;
	SDL_Surface* surface = TTF_RenderText_Solid(font, title, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}

//music
Mix_Music* G_LoadMusic(char* file)
{
	return Mix_LoadMUS(file);
}
void G_PlayMusic(G_Music* music, int loop = -1)
{
	Mix_PlayMusic(music, loop);
}
void G_PauseMusic()
{
	Mix_PauseMusic();
}
void G_ResumeMusic()
{
	Mix_ResumeMusic();
}
void G_StopMusic()
{
	Mix_HaltMusic();
}
void G_FreeMusic(G_Music* music)
{
	Mix_FreeMusic(music);
}

//sound
Mix_Chunk* G_LoadSound(char* file)
{
	return Mix_LoadWAV(file);
}
void G_PlaySound(G_Sound* sound, int loop)
{
	Mix_PlayChannel(-1, sound, loop);
}
void G_PauseSound()
{
	Mix_Pause(-1);
}
void G_FreeSound(G_Sound* sound)
{
	Mix_FreeChunk(sound);
}

//delay and getticks
void G_Delay(Uint32 second)
{
	SDL_Delay(second);
}
Uint32 G_GetTicks()
{
	return SDL_GetTicks();
}

//quit
void G_DestroyTexture(G_Texture *texture)
{
	SDL_DestroyTexture(texture);
}
void G_QuitSDL()
{
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

