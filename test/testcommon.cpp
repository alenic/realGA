#include "testcommon.h"

void coutColor(const char * s, TextColor color) {
	switch(color) {
		case TEXT_RED:
			cout << "\033[31m" << s << "\033[0m";
		break;
		case TEXT_GREEN:
			cout << "\033[32m" << s << "\033[0m";
		break;
		case TEXT_YELLOW:
			cout << "\033[33m" << s << "\033[0m";
		break;
	}
}