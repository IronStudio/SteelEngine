input_keys = [
	"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
	"1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
	"Escape",
	"Tab",
	"Capslock",
	"LShift", "lctrl", "lalt", "ralt", "lctrl", "rshift",
	"Space",
	"Enter",
	"Backspace",
	"Delete",
	"Insert",
	"Home", "End", "Page_Up", "Page_Down",
	"Up", "Down", "Left", "Right"
]

lower_case_keys = []
upper_case_keys = []
for key in input_keys:
	lower_case_keys.append(key.lower())
	upper_case_keys.append(key.upper())

se_keys = []
for key in lower_case_keys:
	finalKey = key
	while finalKey.find("_") != -1:
		position = finalKey.find("_")
		finalKey = finalKey[:position] + finalKey[position + 1:]
		finalKey = finalKey[:position] + finalKey[position].upper() + finalKey[position + 1:]

	finalKey = finalKey[0].upper() + finalKey[1:]
	finalKey = "se::window::Key::e" + finalKey
	se_keys.append(finalKey)

SDL_keys = []
for key in upper_case_keys:
	SDL_keys.append("SDL_SCANCODE_" + key)


for i in range(len(se_keys)):
	print("{" + SDL_keys[i] + ", " + se_keys[i] + "},")