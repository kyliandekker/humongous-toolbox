#include "Theme.h"

#include <imgui/imgui.h>
#include <imgui/implot.h>

#include "htb_patcher/imgui/ImGuiSystem.h"

namespace htb::imgui
{
	void ApplyTheme()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		//----------------------------------------------------------
		// Layout
		//----------------------------------------------------------

		style.WindowPadding = ImVec2(10, 10);
		style.FramePadding = ImVec2(17, 17);
		style.CellPadding = ImVec2(8, 6);
		style.ItemSpacing = ImVec2(8, 8);
		style.ItemInnerSpacing = ImVec2(6, 4);

		style.TouchExtraPadding = ImVec2(0, 0);

		style.IndentSpacing = 20.0f;
		style.ScrollbarSize = 13.0f;
		style.GrabMinSize = 10.0f;

		//----------------------------------------------------------
		// Borders
		//----------------------------------------------------------

		style.WindowBorderSize = 1.0f;
		style.ChildBorderSize = 1.0f;
		style.PopupBorderSize = 1.0f;
		style.FrameBorderSize = 1.0f;
		style.TabBorderSize = 0.0f;

		//----------------------------------------------------------
		// Rounding
		//----------------------------------------------------------

		style.WindowRounding = 8.0f;
		style.ChildRounding = 8.0f;
		style.FrameRounding = 6.0f;
		style.PopupRounding = 8.0f;
		style.ScrollbarRounding = 10.0f;
		style.GrabRounding = 6.0f;
		style.TabRounding = 6.0f;

		//----------------------------------------------------------
		// Alignment
		//----------------------------------------------------------

		style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign = ImVec2(0.0f, 0.5f);

		//----------------------------------------------------------
		// Colors
		//----------------------------------------------------------

		ImVec4* colors = style.Colors;

		auto C = [](int r, int g, int b, int a = 255)
			{
				return ImVec4(
					r / 255.0f,
					g / 255.0f,
					b / 255.0f,
					a / 255.0f);
			};

		//==========================================================
		// Text
		//==========================================================

		colors[ImGuiCol_Text] = C(215, 210, 220);
		colors[ImGuiCol_TextDisabled] = C(140, 130, 165);

		//==========================================================
		// Frames
		//==========================================================

		ImColor fr = C(30, 24, 48);
		colors[ImGuiCol_FrameBg] = C(25, 20, 42);
		colors[ImGuiCol_FrameBgHovered] = C(60, 48, 95);
		colors[ImGuiCol_FrameBgActive] = C(85, 60, 145);

		//==========================================================
		// Windows
		//==========================================================

		colors[ImGuiCol_WindowBg] = fr;
		colors[ImGuiCol_ChildBg] = C(34, 28, 52);
		colors[ImGuiCol_PopupBg] = C(38, 30, 58);

		//==========================================================
		// Borders
		//==========================================================

		colors[ImGuiCol_Border] = C(52, 42, 75);
		colors[ImGuiCol_BorderShadow] = C(0, 0, 0, 0);

		//==========================================================
		// Title
		//==========================================================

		colors[ImGuiCol_TitleBg] = fr;
		colors[ImGuiCol_TitleBgActive] = fr;
		colors[ImGuiCol_TitleBgCollapsed] = fr;

		//==========================================================
		// Menu
		//==========================================================

		colors[ImGuiCol_MenuBarBg] = C(30, 24, 46);

		//==========================================================
		// Scrollbars
		//==========================================================

		colors[ImGuiCol_ScrollbarBg] = C(32, 26, 50);
		colors[ImGuiCol_ScrollbarGrab] = C(85, 72, 120);
		colors[ImGuiCol_ScrollbarGrabHovered] = C(120, 100, 160);
		colors[ImGuiCol_ScrollbarGrabActive] = C(150, 125, 195);

		//==========================================================
		// Checkmarks
		//==========================================================

		colors[ImGuiCol_CheckMark] = C(170, 120, 255);

		//==========================================================
		// Sliders
		//==========================================================

		colors[ImGuiCol_SliderGrab] = C(145, 94, 255);
		colors[ImGuiCol_SliderGrabActive] = C(180, 134, 255);

		//==========================================================
		// Buttons
		//==========================================================

		colors[ImGuiCol_Button] = C(55, 45, 85, 0);
		colors[ImGuiCol_ButtonHovered] = C(115, 82, 215, 40);
		colors[ImGuiCol_ButtonActive] = C(145, 94, 255, 0);

		//==========================================================
		// Headers
		//==========================================================

		colors[ImGuiCol_Header] = C(100, 68, 180);
		colors[ImGuiCol_HeaderHovered] = C(130, 92, 230);
		colors[ImGuiCol_HeaderActive] = C(160, 120, 255);

		//==========================================================
		// Tabs
		//==========================================================

		colors[ImGuiCol_Tab] = fr;
		colors[ImGuiCol_TabHovered] = fr;
		colors[ImGuiCol_TabActive] = fr;
		colors[ImGuiCol_TabSelected] = fr;
		colors[ImGuiCol_TabDimmed] = fr;
		colors[ImGuiCol_TabDimmedSelected] = fr;
		colors[ImGuiCol_TabDimmedSelectedOverline] = C(0, 0, 0, 0);
		colors[ImGuiCol_TabSelectedOverline] = C(0, 0, 0, 0);

		//==========================================================
		// Resize grips
		//==========================================================

		colors[ImGuiCol_ResizeGrip] = C(125, 88, 225, 120);
		colors[ImGuiCol_ResizeGripHovered] = C(150, 108, 245);
		colors[ImGuiCol_ResizeGripActive] = C(175, 132, 255);

		//==========================================================
		// Separators
		//==========================================================

		colors[ImGuiCol_Separator] = C(62, 52, 90);
		colors[ImGuiCol_SeparatorHovered] = C(125, 92, 230);
		colors[ImGuiCol_SeparatorActive] = C(150, 112, 250);

		//==========================================================
		// Tables
		//==========================================================

		colors[ImGuiCol_TableHeaderBg] = C(38, 32, 56);
		colors[ImGuiCol_TableBorderStrong] = C(0, 0, 0, 0);
		colors[ImGuiCol_TableBorderLight] = C(0, 0, 0, 0);

		colors[ImGuiCol_TableRowBg] = C(30, 25, 50);
		colors[ImGuiCol_TableRowBgAlt] = C(28, 23, 48);

		//==========================================================
		// Selection
		//==========================================================

		colors[ImGuiCol_TextSelectedBg] = C(125, 92, 230, 120);

		//==========================================================
		// Docking
		//==========================================================

		colors[ImGuiCol_DockingPreview] = C(145, 94, 255, 180);
		colors[ImGuiCol_DockingEmptyBg] = fr;

		//==========================================================
		// Navigation
		//==========================================================

		colors[ImGuiCol_NavCursor] = C(175, 132, 255);
		colors[ImGuiCol_NavWindowingHighlight] = C(175, 132, 255);

		//----------------------------------------------------------
		// Make everything slightly larger
		//----------------------------------------------------------

		style.ScaleAllSizes(1.05f);

		//----------------------------------------------------------
		// Extra colors
		//----------------------------------------------------------

		ImVec4* extraColors = imgui::ExtraColors;
		extraColors[imgui::ImGuiExtraCol_AccentButton] = C(0, 129, 252, 255);
		extraColors[imgui::ImGuiExtraCol_AccentButtonHovered] = C(107, 181, 249, 255);
		extraColors[imgui::ImGuiExtraCol_AccentButtonActive] = C(25, 73, 117, 255);
		extraColors[imgui::ImGuiExtraCol_CancelButton] = C(168, 58, 58, 255);
		extraColors[imgui::ImGuiExtraCol_CancelButtonHovered] = C(211, 74, 74, 255);
		extraColors[imgui::ImGuiExtraCol_CancelButtonActive] = C(130, 24, 24, 255);
		extraColors[imgui::ImGuiExtraCol_TabInactive] = C(24, 20, 40, 255);

		ImPlotStyle& implotStyle = ImPlot::GetStyle();
		implotStyle.LabelPadding = ImVec2(12, 12);

		ImVec4* implotColors = implotStyle.Colors;
		ImPlot::GetStyle().Colors[ImPlotCol_Line] = C(118, 82, 175, 255);

		// Override colors.
		colors[ImGuiCol_PlotHistogram] = extraColors[imgui::ImGuiExtraCol_AccentButton];
	}
}