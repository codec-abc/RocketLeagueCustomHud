#pragma once
class IMGUIFont
{
public:
	enum FontName {
		RobotoMono_Bold = 101,
		RobotoMono_BoldItalic = 102,
		RobotoMono_ExtraLight = 103,
		RobotoMono_ExtraLightItalic = 104,
		RobotoMono_Italic = 105,
		RobotoMono_Light = 106,
		RobotoMono_LightItalic = 107,
		RobotoMono_Medium = 108,
		RobotoMono_MediumItalic = 109,
		RobotoMono_Regular = 110,
		RobotoMono_SemiBold = 111,
		RobotoMono_SemiBoldItalic = 112,
		RobotoMono_Thin = 113,
		RobotoMono_ThinItalic = 114,
	};

	static ImFont* LoadFont(FontName fontName, float fontSize);
};

