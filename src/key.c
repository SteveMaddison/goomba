/*
 * Goomba GUI/menu system
 * Copyright (c) 2010 Steve Maddison
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <SDL/SDL.h>
#include <goomba/key.h>

const char *goomba_key_name( int key ) {
	switch( key ) {
#ifdef PANDORA
		case SDLK_HOME: return "(A)";
		case SDLK_END: return "(B)";
		case SDLK_PAGEUP: return "(Y)";
		case SDLK_PAGEDOWN: return "(X)";
		case SDLK_RSHIFT: return "(L)";
		case SDLK_RCTRL: return "(R)";
		case SDLK_LCTRL: return "Select";
		case SDLK_LALT: return "Start";
#else
		case SDLK_HOME: return "Home";
		case SDLK_END: return "End";
		case SDLK_PAGEUP: return "PgUp";
		case SDLK_PAGEDOWN: return "PgDn";
		case SDLK_RSHIFT: return "RShift";
		case SDLK_RCTRL: return "RCtrl";
		case SDLK_LCTRL: return "LCtrl";
		case SDLK_LALT: return "LAlt";
#endif
		case SDLK_BACKSPACE: return "Backspace";
		case SDLK_TAB: return "Tab";
		case SDLK_CLEAR: return "Clear";
		case SDLK_RETURN: return "Return";
		case SDLK_PAUSE: return "Pause";
		case SDLK_ESCAPE: return "Esc";
		case SDLK_SPACE: return "Space";
		case SDLK_EXCLAIM: return "!";
		case SDLK_QUOTEDBL: return "\"";
		case SDLK_HASH: return "#";
		case SDLK_DOLLAR: return "$";
		case SDLK_AMPERSAND: return "&";
		case SDLK_QUOTE: return "\'";
		case SDLK_LEFTPAREN: return "(";
		case SDLK_RIGHTPAREN: return ")";
		case SDLK_ASTERISK: return "*";
		case SDLK_PLUS: return "+";
		case SDLK_COMMA: return ",";
		case SDLK_MINUS: return "-";
		case SDLK_PERIOD: return ".";
		case SDLK_SLASH: return "/";
		case SDLK_0: return "0";
		case SDLK_1: return "1";
		case SDLK_2: return "2";
		case SDLK_3: return "3";
		case SDLK_4: return "4";
		case SDLK_5: return "5";
		case SDLK_6: return "6";
		case SDLK_7: return "7";
		case SDLK_8: return "8";
		case SDLK_9: return "9";
		case SDLK_COLON: return ":";
		case SDLK_SEMICOLON: return ";";
		case SDLK_LESS: return "<";
		case SDLK_EQUALS: return "=";
		case SDLK_GREATER: return ">";
		case SDLK_QUESTION: return "?";
		case SDLK_AT: return "@";
		case SDLK_LEFTBRACKET: return "[";
		case SDLK_BACKSLASH: return "\\";
		case SDLK_RIGHTBRACKET: return "]";
		case SDLK_CARET: return "^";
		case SDLK_UNDERSCORE: return "_";
		case SDLK_BACKQUOTE: return "`";
		case SDLK_a: return "A";
		case SDLK_b: return "B";
		case SDLK_c: return "C";
		case SDLK_d: return "D";
		case SDLK_e: return "E";
		case SDLK_f: return "F";
		case SDLK_g: return "G";
		case SDLK_h: return "H";
		case SDLK_i: return "I";
		case SDLK_j: return "J";
		case SDLK_k: return "K";
		case SDLK_l: return "L";
		case SDLK_m: return "M";
		case SDLK_n: return "N";
		case SDLK_o: return "O";
		case SDLK_p: return "P";
		case SDLK_q: return "Q";
		case SDLK_r: return "R";
		case SDLK_s: return "S";
		case SDLK_t: return "T";
		case SDLK_u: return "U";
		case SDLK_v: return "V";
		case SDLK_w: return "W";
		case SDLK_x: return "X";
		case SDLK_y: return "Y";
		case SDLK_z: return "Z";
		case SDLK_DELETE: return "Del";
		case SDLK_WORLD_0: return "World 0";
		case SDLK_WORLD_1: return "World_1";
		case SDLK_WORLD_2: return "World_2";
		case SDLK_WORLD_3: return "World_3";
		case SDLK_WORLD_4: return "World 4";
		case SDLK_WORLD_5: return "World 5";
		case SDLK_WORLD_6: return "World 6";
		case SDLK_WORLD_7: return "World 7";
		case SDLK_WORLD_8: return "World 8";
		case SDLK_WORLD_9: return "World 9";
		case SDLK_WORLD_10: return "World 10";
		case SDLK_WORLD_11: return "World 11";
		case SDLK_WORLD_12: return "World 12";
		case SDLK_WORLD_13: return "World 13";
		case SDLK_WORLD_14: return "World 14";
		case SDLK_WORLD_15: return "World 15";
		case SDLK_WORLD_16: return "World 16";
		case SDLK_WORLD_17: return "World 17";
		case SDLK_WORLD_18: return "World 18";
		case SDLK_WORLD_19: return "World 19";
		case SDLK_WORLD_20: return "World 20";
		case SDLK_WORLD_21: return "World 21";
		case SDLK_WORLD_22: return "World 22";
		case SDLK_WORLD_23: return "World 23";
		case SDLK_WORLD_24: return "World 24";
		case SDLK_WORLD_25: return "World 25";
		case SDLK_WORLD_26: return "World 26";
		case SDLK_WORLD_27: return "World 27";
		case SDLK_WORLD_28: return "World 28";
		case SDLK_WORLD_29: return "World 29";
		case SDLK_WORLD_30: return "World 30";
		case SDLK_WORLD_31: return "World 31";
		case SDLK_WORLD_32: return "World 32";
		case SDLK_WORLD_33: return "World 33";
		case SDLK_WORLD_34: return "World 34";
		case SDLK_WORLD_35: return "World 35";
		case SDLK_WORLD_36: return "World 36";
		case SDLK_WORLD_37: return "World 37";
		case SDLK_WORLD_38: return "World 38";
		case SDLK_WORLD_39: return "World 39";
		case SDLK_WORLD_40: return "World 40";
		case SDLK_WORLD_41: return "World 41";
		case SDLK_WORLD_42: return "World 42";
		case SDLK_WORLD_43: return "World 43";
		case SDLK_WORLD_44: return "World 44";
		case SDLK_WORLD_45: return "World 45";
		case SDLK_WORLD_46: return "World 46";
		case SDLK_WORLD_47: return "World 47";
		case SDLK_WORLD_48: return "World 48";
		case SDLK_WORLD_49: return "World 49";
		case SDLK_WORLD_50: return "World 50";
		case SDLK_WORLD_51: return "World 51";
		case SDLK_WORLD_52: return "World 52";
		case SDLK_WORLD_53: return "World 53";
		case SDLK_WORLD_54: return "World 54";
		case SDLK_WORLD_55: return "World 55";
		case SDLK_WORLD_56: return "World 56";
		case SDLK_WORLD_57: return "World 57";
		case SDLK_WORLD_58: return "World 58";
		case SDLK_WORLD_59: return "World 59";
		case SDLK_WORLD_60: return "World 60";
		case SDLK_WORLD_61: return "World 61";
		case SDLK_WORLD_62: return "World 62";
		case SDLK_WORLD_63: return "World 63";
		case SDLK_WORLD_64: return "World 64";
		case SDLK_WORLD_65: return "World 65";
		case SDLK_WORLD_66: return "World 66";
		case SDLK_WORLD_67: return "World 67";
		case SDLK_WORLD_68: return "World 68";
		case SDLK_WORLD_69: return "World 69";
		case SDLK_WORLD_70: return "World 70";
		case SDLK_WORLD_71: return "World 71";
		case SDLK_WORLD_72: return "World 72";
		case SDLK_WORLD_73: return "World 73";
		case SDLK_WORLD_74: return "World 74";
		case SDLK_WORLD_75: return "World 75";
		case SDLK_WORLD_76: return "World 76";
		case SDLK_WORLD_77: return "World 77";
		case SDLK_WORLD_78: return "World 78";
		case SDLK_WORLD_79: return "World 79";
		case SDLK_WORLD_80: return "World 80";
		case SDLK_WORLD_81: return "World 81";
		case SDLK_WORLD_82: return "World 82";
		case SDLK_WORLD_83: return "World 83";
		case SDLK_WORLD_84: return "World 84";
		case SDLK_WORLD_85: return "World 85";
		case SDLK_WORLD_86: return "World 86";
		case SDLK_WORLD_87: return "World 87";
		case SDLK_WORLD_88: return "World 88";
		case SDLK_WORLD_89: return "World 89";
		case SDLK_WORLD_90: return "World 90";
		case SDLK_WORLD_91: return "World 91";
		case SDLK_WORLD_92: return "World 92";
		case SDLK_WORLD_93: return "World 93";
		case SDLK_WORLD_94: return "World 94";
		case SDLK_WORLD_95: return "World 95";
		case SDLK_KP0: return "Num0";
		case SDLK_KP1: return "Num1";
		case SDLK_KP2: return "Num2";
		case SDLK_KP3: return "Num3";
		case SDLK_KP4: return "Num4";
		case SDLK_KP5: return "Num5";
		case SDLK_KP6: return "Num6";
		case SDLK_KP7: return "Num7";
		case SDLK_KP8: return "Num8";
		case SDLK_KP9: return "Num9";
		case SDLK_KP_PERIOD: return "Num.";
		case SDLK_KP_DIVIDE: return "Num/";
		case SDLK_KP_MULTIPLY: return "Num*";
		case SDLK_KP_MINUS: return "Num-";
		case SDLK_KP_PLUS: return "Num+";
		case SDLK_KP_ENTER: return "Num Enter";
		case SDLK_KP_EQUALS: return "Num=";
		case SDLK_UP: return "Up";
		case SDLK_DOWN: return "Down";
		case SDLK_RIGHT: return "Right";
		case SDLK_LEFT: return "Left";
		case SDLK_INSERT: return "Ins";
		case SDLK_F1: return "F1";
		case SDLK_F2: return "F2";
		case SDLK_F3: return "F3";
		case SDLK_F4: return "F4";
		case SDLK_F5: return "F5";
		case SDLK_F6: return "F6";
		case SDLK_F7: return "F7";
		case SDLK_F8: return "F8";
		case SDLK_F9: return "F9";
		case SDLK_F10: return "F10";
		case SDLK_F11: return "F11";
		case SDLK_F12: return "F12";
		case SDLK_F13: return "F13";
		case SDLK_F14: return "F14";
		case SDLK_F15: return "F15";
		case SDLK_NUMLOCK: return "Numlock";
		case SDLK_CAPSLOCK: return "Capslock";
		case SDLK_SCROLLOCK: return "Scrollock";
		case SDLK_LSHIFT: return "LShift";
		case SDLK_RALT: return "RAlt";
		case SDLK_RMETA: return "RMeta";
		case SDLK_LMETA: return "LMeta";
		case SDLK_LSUPER: return "LSuper";
		case SDLK_RSUPER: return "RSuper";
		case SDLK_MODE: return "Mode";
		case SDLK_COMPOSE: return "Compose";
		case SDLK_HELP: return "Help";
		case SDLK_PRINT: return "PrintScrn";
		case SDLK_SYSREQ: return "SysRq";
		case SDLK_BREAK: return "Break";
		case SDLK_MENU: return "Menu";
		case SDLK_POWER: return "Power";
		case SDLK_EURO: return "Euro";
		case SDLK_UNDO: return "Undo";
	}
	return "";
}

