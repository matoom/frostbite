#ifndef DEFAULTVALUES_H
#define DEFAULTVALUES_H

#define SPEECH_COLOR_HEX "#4A956E"
#define SPEECH_COLOR_RGB 74, 149, 110
#define WHISPER_COLOR_HEX "#00AAFF"
#define WHISPER_COLOR_RGB 0, 170, 255
#define ROOM_NAME_COLOR_HEX "#AAAA7F"
#define ROOM_NAME_COLOR_RGB 60, 64, 170
#define GAME_MESSAGE_COLOR_HEX "#FF5500"
#define GAME_MESSAGE_COLOR_RGB 255, 85, 07
#define THINKING_COLOR_HEX "#CD6900"
#define THINKING_COLOR_RGB 205, 105, 0
#define BOOST_COLOR_HEX "#55AA7F"
#define BOOST_COLOR_RGB 85, 170, 127
#define PENALTY_COLOR_HEX "#FF557F"
#define PENALTY_COLOR_RGB 255, 85, 127
#define ECHO_COLOR_HEX "#FFAA00"
#define ECHO_COLOR_RGB 255, 170, 0
#define SCRIPT_COLOR_HEX "#970080"
#define SCRIPT_COLOR_RGB 151, 0, 128
#define DAMAGE_COLOR_HEX "#FF557F"
#define DAMAGE_COLOR_RGB 255, 85, 127

#define ROOM_NAME "a_roomName"
#define GAME_MESSAGE "b_gameMessage"
#define DAMAGE "c_damage"
#define SPEECH "d_speech"
#define WHISPER "e_whisper"
#define THINKING "f_thinking"
#define BONUS "g_bonus"
#define PENALTY "h_penalty"
#define SCRIPT "i_script"
#define ECHO "j_echo"

#ifdef Q_OS_WIN
#define DEFAULT_FONT "Consolas"
#elif defined(Q_OS_LINUX)
#define DEFAULT_FONT "Monospace"
#elif defined(Q_OS_MAC)
#define DEFAULT_FONT "Monaco"
#endif

#define DEFAULT_MAIN_FONT DEFAULT_FONT
#define DEFAULT_MAIN_FONT_SIZE 12
#define DEFAULT_MAIN_FONT_COLOR "#000000"
#define DEFAULT_MAIN_BACKGROUND "#FFFFFF"

#define DEFAULT_DOCK_FONT DEFAULT_FONT
#define DEFAULT_DOCK_FONT_SIZE 11
#define DEFAULT_DOCK_FONT_COLOR "#000000"
#define DEFAULT_DOCK_BACKGROUND "#FFFFFF"

#define DOCK_TITLE_DEATHS "Deaths"
#define DOCK_TITLE_ARRIVALS "Arrivals"
#define DOCK_TITLE_THOUGHTS "Thoughts"
#define DOCK_TITLE_ROOM "Room"
#define DOCK_TITLE_CONVERSATIONS "Conversations"
#define DOCK_TITLE_FAMILIAR "Familiar"
#define DOCK_TITLE_EXP "Experience"
#define DOCK_TITLE_MAP "Map"

#endif // DEFAULTVALUES_H
