#include "input.h"
#include <unordered_map>

void ProcessInputAfter(Input& input)
{
}

void ResetInput(Input& input)
{
	for (std::unordered_map<const char*, Button>::iterator itr = input.KeyboardMap.begin(); itr != input.KeyboardMap.end(); itr++)
	{
		Button a = itr->second;
		itr->second.
		a = static_cast<>
	}
}

void processEventButton(Button& b, bool newState)
{
}
