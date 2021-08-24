#pragma once

#include <iostream>
#include <utility>

#define DEBUG

namespace LM 
{
	
	class ConsoleLog
	{
	public:
#ifdef _WIN32
		enum class ConsoleColorType
		{
			Black        = 0,
			Blue         = 1,
			Green        = 2,
			Cyan         = 3,
			Red          = 4,
			Magenta      = 5,
			//Brown        = 6, // No bash Color!
			LightGray    = 7,
			DarkGray     = 8,
			LightBlue    = 9,
			LightGreen   = 10,
			LightCyan    = 11,
			LightRed     = 12,
			LightMagenta = 13,
			Yellow       = 14,
			White        = 15
		};

		//typedef ConsoleTxtColor ConsoleBgColor;
		typedef ConsoleColorType ConsoleTxtColor;
		typedef ConsoleColorType ConsoleBgColor;
#else
		struct ConsoleTxtColor
		{
			static inline std::string Black        = "30";
			static inline std::string Blue         = "34";
			static inline std::string Green        = "32";
			static inline std::string Cyan         = "36";
			static inline std::string Red          = "31";
			static inline std::string Magenta      = "35";
			static inline std::string LightGray    = "37";
			static inline std::string DarkGray     = "90";
			static inline std::string LightBlue    = "94";
			static inline std::string LightGreen   = "92";
			static inline std::string LightCyan    = "96";
			static inline std::string LightRed     = "91";
			static inline std::string LightMagenta = "95";
			static inline std::string Yellow       = "33";
			static inline std::string White        = "97";
		};

		struct ConsoleBgColor
		{
			static inline std::string Black = "30";
			static inline std::string Blue = "34";
			static inline std::string Green = "32";
			static inline std::string Cyan = "36";
			static inline std::string Red = "31";
			static inline std::string Magenta = "35";
			static inline std::string LightGray = "37";
			static inline std::string DarkGray = "90";
			static inline std::string LightBlue = "94";
			static inline std::string LightGreen = "92";
			static inline std::string LightCyan = "96";
			static inline std::string LightRed = "91";
			static inline std::string LightMagenta = "95";
			static inline std::string Yellow = "33";
			static inline std::string White = "97";
		};

		typedef std::string ConsoleColorType;
#endif
	
		ConsoleLog() = delete;
		
		static void SetColor(ConsoleColorType txt = ConsoleTxtColor::White, ConsoleColorType bg = ConsoleBgColor::Black);

		static void Init() 
		{ 
			std::system("cls"); 
		}

		template<typename ...Args>
		static void LogInfo(Args && ...args)
		{
			SetColor(ConsoleTxtColor::Green);
			std::cerr << "[ INFO ]: ";
			(std::cerr << ... << args) << ' ';
			std::cerr << std::endl;
			SetColor();
		}

		template<typename ...Args>
		static void LogWarning(Args && ...args)
		{
			SetColor(ConsoleTxtColor::Yellow);
			std::cerr << "[ WARNING ]: ";
			(std::cerr << ... << args) << ' ';
			std::cerr << std::endl;
			SetColor();
		}

		template<typename ...Args>
		static void LogError(Args && ...args)
		{
			SetColor(ConsoleTxtColor::Red);
			std::cerr << "[ ERROR ]: ";
			(std::cerr << ... << args) << ' ';
			std::cerr << std::endl;
			SetColor();
		}

		template<typename ...Args>
		static void LogDecorate(Args&& ...args)
		{
			SetColor(ConsoleTxtColor::LightGray);
			printf("========================================\n");
			SetColor();
		}
	};

}

#ifdef DEBUG

#define LOG_INIT() ::LM::ConsoleLog::Init()

#define LOGI(...)  ::LM::ConsoleLog::LogInfo(__VA_ARGS__)
				   
#define LOGW(...)  ::LM::ConsoleLog::LogWarning(__VA_ARGS__)
				   
#define LOGE(...)  ::LM::ConsoleLog::LogError(__VA_ARGS__)

// Decorate your code!
#define LOGD()	   ::LM::ConsoleLog::LogDecorate();

//#define CORE_ASSERT(x, s) { if(!(x)) { LOGE("Assertion Failed: " << s); __debugbreak(); } }

#else
#define LOG_INIT() 
#define LOGI(x)
#define LOGW(x)
#define LOGE(x)
#define LOGD()
//#define CORE_ASSERT(x, s)
//#define CORE_ASSERT(x, ...) 

#endif // DEBUG