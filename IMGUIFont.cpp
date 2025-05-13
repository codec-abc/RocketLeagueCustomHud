#include "pch.h"
#include "IMGUIFont.h"
#include "resource.h"
#include <wtypes.h>


ImFont* IMGUIFont::LoadFont(FontName fontname, float fontSize)
{
	HMODULE hModule = NULL;

	bool isModuleFound = GetModuleHandleEx(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
		GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPCTSTR)&IMGUIFont::LoadFont, &hModule);

	HRSRC myResource = ::FindResource(hModule, MAKEINTRESOURCE(fontname), RT_RCDATA);
	if (myResource == nullptr)
	{
		return nullptr;
	}

	unsigned int myResourceSize = ::SizeofResource(hModule, myResource);
	HGLOBAL myResourceData = ::LoadResource(hModule, myResource);
	if (myResourceData == nullptr)
	{
		return nullptr;
	}
	void* pMyBinaryData = ::LockResource(myResourceData);
	ImGuiIO& io = ImGui::GetIO();
	ImFontConfig config;
	config.FontDataOwnedByAtlas = false;
	auto returned = io.Fonts->AddFontFromMemoryTTF(pMyBinaryData, myResourceSize, fontSize, &config);
	io.Fonts->Build();
	return returned;
}