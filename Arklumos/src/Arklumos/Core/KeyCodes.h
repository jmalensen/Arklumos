#pragma once

namespace Arklumos
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44,			 /* , */
		Minus = 45,			 /* - */
		Period = 46,		 /* . */
		Slash = 47,			 /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61,			/* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,	 /* [ */
		Backslash = 92,		 /* \ */
		RightBracket = 93, /* ] */
		GraveAccent = 96,	 /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream &operator<<(std::ostream &os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

// From glfw3.h
#define AK_KEY_SPACE ::Arklumos::Key::Space
#define AK_KEY_APOSTROPHE ::Arklumos::Key::Apostrophe /* ' */
#define AK_KEY_COMMA ::Arklumos::Key::Comma						/* , */
#define AK_KEY_MINUS ::Arklumos::Key::Minus						/* - */
#define AK_KEY_PERIOD ::Arklumos::Key::Period					/* . */
#define AK_KEY_SLASH ::Arklumos::Key::Slash						/* / */
#define AK_KEY_0 ::Arklumos::Key::D0
#define AK_KEY_1 ::Arklumos::Key::D1
#define AK_KEY_2 ::Arklumos::Key::D2
#define AK_KEY_3 ::Arklumos::Key::D3
#define AK_KEY_4 ::Arklumos::Key::D4
#define AK_KEY_5 ::Arklumos::Key::D5
#define AK_KEY_6 ::Arklumos::Key::D6
#define AK_KEY_7 ::Arklumos::Key::D7
#define AK_KEY_8 ::Arklumos::Key::D8
#define AK_KEY_9 ::Arklumos::Key::D9
#define AK_KEY_SEMICOLON ::Arklumos::Key::Semicolon /* ; */
#define AK_KEY_EQUAL ::Arklumos::Key::Equal					/* = */
#define AK_KEY_A ::Arklumos::Key::A
#define AK_KEY_B ::Arklumos::Key::B
#define AK_KEY_C ::Arklumos::Key::C
#define AK_KEY_D ::Arklumos::Key::D
#define AK_KEY_E ::Arklumos::Key::E
#define AK_KEY_F ::Arklumos::Key::F
#define AK_KEY_G ::Arklumos::Key::G
#define AK_KEY_H ::Arklumos::Key::H
#define AK_KEY_I ::Arklumos::Key::I
#define AK_KEY_J ::Arklumos::Key::J
#define AK_KEY_K ::Arklumos::Key::K
#define AK_KEY_L ::Arklumos::Key::L
#define AK_KEY_M ::Arklumos::Key::M
#define AK_KEY_N ::Arklumos::Key::N
#define AK_KEY_O ::Arklumos::Key::O
#define AK_KEY_P ::Arklumos::Key::P
#define AK_KEY_Q ::Arklumos::Key::Q
#define AK_KEY_R ::Arklumos::Key::R
#define AK_KEY_S ::Arklumos::Key::S
#define AK_KEY_T ::Arklumos::Key::T
#define AK_KEY_U ::Arklumos::Key::U
#define AK_KEY_V ::Arklumos::Key::V
#define AK_KEY_W ::Arklumos::Key::W
#define AK_KEY_X ::Arklumos::Key::X
#define AK_KEY_Y ::Arklumos::Key::Y
#define AK_KEY_Z ::Arklumos::Key::Z
#define AK_KEY_LEFT_BRACKET ::Arklumos::Key::LeftBracket	 /* [ */
#define AK_KEY_BACKSLASH ::Arklumos::Key::Backslash				 /* \ */
#define AK_KEY_RIGHT_BRACKET ::Arklumos::Key::RightBracket /* ] */
#define AK_KEY_GRAVE_ACCENT ::Arklumos::Key::GraveAccent	 /* ` */
#define AK_KEY_WORLD_1 ::Arklumos::Key::World1						 /* non-US #1 */
#define AK_KEY_WORLD_2 ::Arklumos::Key::World2						 /* non-US #2 */

/* Function keys */
#define AK_KEY_ESCAPE ::Arklumos::Key::Escape
#define AK_KEY_ENTER ::Arklumos::Key::Enter
#define AK_KEY_TAB ::Arklumos::Key::Tab
#define AK_KEY_BACKSPACE ::Arklumos::Key::Backspace
#define AK_KEY_INSERT ::Arklumos::Key::Insert
#define AK_KEY_DELETE ::Arklumos::Key::Delete
#define AK_KEY_RIGHT ::Arklumos::Key::Right
#define AK_KEY_LEFT ::Arklumos::Key::Left
#define AK_KEY_DOWN ::Arklumos::Key::Down
#define AK_KEY_UP ::Arklumos::Key::Up
#define AK_KEY_PAGE_UP ::Arklumos::Key::PageUp
#define AK_KEY_PAGE_DOWN ::Arklumos::Key::PageDown
#define AK_KEY_HOME ::Arklumos::Key::Home
#define AK_KEY_END ::Arklumos::Key::End
#define AK_KEY_CAPS_LOCK ::Arklumos::Key::CapsLock
#define AK_KEY_SCROLL_LOCK ::Arklumos::Key::ScrollLock
#define AK_KEY_NUM_LOCK ::Arklumos::Key::NumLock
#define AK_KEY_PRINT_SCREEN ::Arklumos::Key::PrintScreen
#define AK_KEY_PAUSE ::Arklumos::Key::Pause
#define AK_KEY_F1 ::Arklumos::Key::F1
#define AK_KEY_F2 ::Arklumos::Key::F2
#define AK_KEY_F3 ::Arklumos::Key::F3
#define AK_KEY_F4 ::Arklumos::Key::F4
#define AK_KEY_F5 ::Arklumos::Key::F5
#define AK_KEY_F6 ::Arklumos::Key::F6
#define AK_KEY_F7 ::Arklumos::Key::F7
#define AK_KEY_F8 ::Arklumos::Key::F8
#define AK_KEY_F9 ::Arklumos::Key::F9
#define AK_KEY_F10 ::Arklumos::Key::F10
#define AK_KEY_F11 ::Arklumos::Key::F11
#define AK_KEY_F12 ::Arklumos::Key::F12
#define AK_KEY_F13 ::Arklumos::Key::F13
#define AK_KEY_F14 ::Arklumos::Key::F14
#define AK_KEY_F15 ::Arklumos::Key::F15
#define AK_KEY_F16 ::Arklumos::Key::F16
#define AK_KEY_F17 ::Arklumos::Key::F17
#define AK_KEY_F18 ::Arklumos::Key::F18
#define AK_KEY_F19 ::Arklumos::Key::F19
#define AK_KEY_F20 ::Arklumos::Key::F20
#define AK_KEY_F21 ::Arklumos::Key::F21
#define AK_KEY_F22 ::Arklumos::Key::F22
#define AK_KEY_F23 ::Arklumos::Key::F23
#define AK_KEY_F24 ::Arklumos::Key::F24
#define AK_KEY_F25 ::Arklumos::Key::F25

/* Keypad */
#define AK_KEY_KP_0 ::Arklumos::Key::KP0
#define AK_KEY_KP_1 ::Arklumos::Key::KP1
#define AK_KEY_KP_2 ::Arklumos::Key::KP2
#define AK_KEY_KP_3 ::Arklumos::Key::KP3
#define AK_KEY_KP_4 ::Arklumos::Key::KP4
#define AK_KEY_KP_5 ::Arklumos::Key::KP5
#define AK_KEY_KP_6 ::Arklumos::Key::KP6
#define AK_KEY_KP_7 ::Arklumos::Key::KP7
#define AK_KEY_KP_8 ::Arklumos::Key::KP8
#define AK_KEY_KP_9 ::Arklumos::Key::KP9
#define AK_KEY_KP_DECIMAL ::Arklumos::Key::KPDecimal
#define AK_KEY_KP_DIVIDE ::Arklumos::Key::KPDivide
#define AK_KEY_KP_MULTIPLY ::Arklumos::Key::KPMultiply
#define AK_KEY_KP_SUBTRACT ::Arklumos::Key::KPSubtract
#define AK_KEY_KP_ADD ::Arklumos::Key::KPAdd
#define AK_KEY_KP_ENTER ::Arklumos::Key::KPEnter
#define AK_KEY_KP_EQUAL ::Arklumos::Key::KPEqual

#define AK_KEY_LEFT_SHIFT ::Arklumos::Key::LeftShift
#define AK_KEY_LEFT_CONTROL ::Arklumos::Key::LeftControl
#define AK_KEY_LEFT_ALT ::Arklumos::Key::LeftAlt
#define AK_KEY_LEFT_SUPER ::Arklumos::Key::LeftSuper
#define AK_KEY_RIGHT_SHIFT ::Arklumos::Key::RightShift
#define AK_KEY_RIGHT_CONTROL ::Arklumos::Key::RightControl
#define AK_KEY_RIGHT_ALT ::Arklumos::Key::RightAlt
#define AK_KEY_RIGHT_SUPER ::Arklumos::Key::RightSuper
#define AK_KEY_MENU ::Arklumos::Key::Menu
