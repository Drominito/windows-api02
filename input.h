#pragma once

#include <Windows.h>
#include <unordered_map>
#include <string>
#include <utility>
#define WIN32_LEAN_AND_MEAN



void ProcessInputAfter(Input& input);
void ResetInput(Input& input);

//newstate == 1 means pressed else released
void processEventButton(Button& b, bool newState);

struct MyBit
{
	unsigned char Focused  : 1;
};

struct Input
{
	std::unordered_map<const char*, Button> KeyboardMap;

	MyBit mb;
private:
	Input()
	{
		mb.Focused = 0;
	}
};

struct Button
{
	UCHAR pressed  = 0,
		  held     = 0,
		  released = 0;

	enum inputEnum
	{
		A = 0,
		B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		NR0, NR1, NR2, NR3, NR4, NR5, NR6, NR7, NR8, NR9,
		Space,
		Enter,
		Escape,
		Up,
		Down,
		Left,
		Right,
		Shift,
		BackSpace, //next line 
		Plus_Equal,
		Period_RightArrow,
		Minus_Underscore,
		Comma_LeftArrow,
		SemiColon,
		Question_BackSlash,
		Tilde,
		Quotes,
		Slash,
		SquareBracketsOpen,
		SquareBracketsClose,
		BUTTONS_COUNT, //
	};

	std::pair<int, int> buttonValues[BUTTONS_COUNT] =
	{
		
		{A, 'A'}, {B, 'B'}, {C, 'C'}, {D, 'D'}, {E, 'E'}, {F, 'F'}, {G, 'G'}, {H, 'H'}, {I, 'H'}, {J, 'G'}, {K, 'K'}, {L, 'L'},
		{M, 'M'}, {N, 'N'}, {O, 'O'}, {P, 'P'}, {Q, 'Q'}, {R, 'R'}, {S, 'S'}, {T, 'T'}, {U, 'U'}, {V, 'V'}, {W, 'W'}, {X, 'X'}, {Y, 'Y'}, {Z, 'Z'},
		{NR0, '0'}, {NR1, '1'}, {NR2, '2'}, {NR3, '3'}, {NR4, '4'}, {NR5, '5'}, {NR6, '6'}, {NR7, '7'}, {NR8, '8'}, {NR9, '9'},
		{Space, VK_SPACE}, {Enter, VK_RETURN}, {Up, VK_UP}, {Down, VK_DOWN}, {Left, VK_LEFT}, {Right, VK_RIGHT}, {Shift, VK_SHIFT}, {BackSpace, VK_BACK}, {Plus_Equal, VK_OEM_PLUS},
		{Period_RightArrow, VK_OEM_PERIOD}, {Minus_Underscore, VK_OEM_MINUS}, {Comma_LeftArrow, VK_OEM_COMMA}, {SemiColon, 48}, {Question_BackSlash, 49}, {Tilde, 50}, {, 51},
	};

};