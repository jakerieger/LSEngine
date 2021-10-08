#include "LSGUI.h"

void LSGUI::InitImGUI(GLFWwindow* window, unsigned int glslVerMajor, unsigned int glslVerMinor) {
	std::string glsl_version = fmt::format("#version {}{}0 core", glslVerMajor, glslVerMinor);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//ImGui::StyleColorsDark();

	io.Fonts->AddFontFromFileTTF("Y:\\Code\\_GAMEENGINE\\LSENGINE_MAIN\\LSEngine\\resources\\fonts\\TitilliumWeb-Regular.ttf", 18);

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version.c_str());
}

void LSGUI::ApplyTheme() {
	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	colors[ImGuiCol_Text] = ImVec4(169.f / 255.f, 177.f / 255.f, 213.f / 255.f, 1.000f);
	colors[ImGuiCol_TextDisabled] = ImVec4(103.f / 255.f, 108.f / 255.f, 130.f / 255.f, 1.000f);
	colors[ImGuiCol_WindowBg] = ImVec4(20.f / 255.f, 20.f / 255.f, 27.f / 255.f, 1.000f);
	colors[ImGuiCol_ChildBg] = ImVec4(20.f / 255.f, 20.f / 255.f, 27.f / 255.f, 1.000f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
	colors[ImGuiCol_Border] = ImVec4(25.f / 255.f, 25.f / 255.f, 32.f / 255.f, 1.000f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
	colors[ImGuiCol_FrameBg] = ImVec4(26.f / 255.f, 27.f / 255.f, 38.f / 255.f, 1.000f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(32.f / 255.f, 33.f / 255.f, 44.f / 255.f, 1.000f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(32.f / 255.f, 33.f / 255.f, 44.f / 255.f, 1.000f);
	colors[ImGuiCol_TitleBg] = ImVec4(26.f / 255.f, 27.f / 255.f, 38.f / 255.f, 1.000f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(26.f / 255.f, 27.f / 255.f, 38.f / 255.f, 1.000f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(26.f / 255.f, 27.f / 255.f, 38.f / 255.f, 1.000f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(20.f / 255.f, 20.f / 255.f, 27.f / 255.f, 1.000f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(26.f / 255.f, 27.f / 255.f, 38.f / 255.f, 1.000f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(62.f / 255.f, 89.f / 255.f, 159.f / 255.f, 1.000f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(62.f / 255.f, 89.f / 255.f, 159.f / 255.f, 0.800f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(62.f / 255.f, 89.f / 255.f, 159.f / 255.f, 1.000f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
	colors[ImGuiCol_SliderGrab] = ImVec4(62.f / 255.f, 89.f / 255.f, 159.f / 255.f, 0.800f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(62.f / 255.f, 89.f / 255.f, 159.f / 255.f, 1.000f);
	colors[ImGuiCol_Button] = ImVec4(62.f / 255.f, 89.f / 255.f, 159.f / 255.f, 1.000f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(62.f / 255.f, 89.f / 255.f, 159.f / 255.f, 0.800f);
	colors[ImGuiCol_ButtonActive] = ImVec4(62.f / 255.f, 89.f / 255.f, 159.f / 255.f, 1.000f);
	colors[ImGuiCol_Header] = ImVec4(20.f / 255.f, 20.f / 255.f, 27.f / 255.f, 1.000f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(40.f / 255.f, 40.f / 255.f, 47.f / 255.f, 1.000f);
	colors[ImGuiCol_HeaderActive] = ImVec4(35.f / 255.f, 35.f / 255.f, 42.f / 255.f, 1.000f);
	colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
	colors[ImGuiCol_SeparatorHovered] = ImVec4(62.f / 255.f, 89.f / 255.f, 159.f / 255.f, 0.800f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(62.f / 255.f, 89.f / 255.f, 159.f / 255.f, 1.000f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(26.f / 255.f, 27.f / 255.f, 38.f / 255.f, 1.000f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(62.f / 255.f, 89.f / 255.f, 159.f / 255.f, 0.800f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(62.f / 255.f, 89.f / 255.f, 159.f / 255.f, 1.000f);
	colors[ImGuiCol_Tab] = ImVec4(35.f / 255.f, 35.f / 255.f, 42.f / 255.f, 1.000f);
	colors[ImGuiCol_TabHovered] = ImVec4(20.f / 255.f, 20.f / 255.f, 27.f / 255.f, 0.800f);
	colors[ImGuiCol_TabActive] = ImVec4(20.f / 255.f, 20.f / 255.f, 27.f / 255.f, 1.000f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(35.f / 255.f, 35.f / 255.f, 42.f / 255.f, 1.000f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(20.f / 255.f, 20.f / 255.f, 27.f / 255.f, 1.000f);
	// colors[ImGuiCol_DockingPreview]         = ImVec4(1.000f, 0.391f, 0.000f, 0.781f);
	// colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(169.f / 255.f, 177.f / 255.f, 213.f / 255.f, 0.200f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
	colors[ImGuiCol_PopupBg] = ImVec4(28.f / 255.f, 28.f / 255.f, 35.f / 255.f, 1.000f);

	style->ChildRounding = 4.0f;
	style->FrameBorderSize = 0.0f;
	style->FrameRounding = 2.0f;
	style->GrabMinSize = 7.0f;
	style->PopupRounding = 2.0f;
	style->ScrollbarRounding = 12.0f;
	style->ScrollbarSize = 13.0f;
	style->TabBorderSize = 1.0f;
	style->TabRounding = 4.0f;
	style->WindowRounding = 0.0f;
}

bool LSGUI::DragFloatN_Colored(const char* label, float* v, int components, float v_speed, float v_min, float v_max, const char* display_format, float power)
{
  ImGuiWindow* window = ImGui::GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext& g = *GImGui;
  bool value_changed = false;
  ImGui::BeginGroup();
  ImGui::PushID(label);
  ImGui::PushMultiItemsWidths(components, ImGui::CalcItemWidth());

  for (int i = 0; i < components; i++) {
    static const ImU32 colors[] = {
      0xBB0000FF,
      0xBB00FF00,
      0xBBFF0000,
      0xBBFFFFFF,
    };

    ImGui::PushID(i);
    value_changed |= ImGui::DragFloat("##v", &v[i], v_speed, v_min, v_max, display_format, power);

    const ImVec2 min = ImGui::GetItemRectMin();
    const ImVec2 max = ImGui::GetItemRectMax();
    const float spacing = g.Style.FrameRounding;
    const float halfSpacing = spacing / 2;

    window->DrawList->AddLine({ min.x + spacing, max.y - halfSpacing }, { max.x - spacing, max.y - halfSpacing }, colors[i], 1);

    ImGui::SameLine(0, g.Style.ItemInnerSpacing.x);
    ImGui::PopID();
    ImGui::PopItemWidth();
  }

  ImGui::PopID();
  ImGui::TextUnformatted(label, ImGui::FindRenderedTextEnd(label));
  ImGui::EndGroup();

  return value_changed;
}

bool LSGUI::ButtonColored(const char* label, ImVec4 color, const ImVec2& size) {
	bool button_clicked = false;

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4({ 1.0f, 1.0f, 1.0f, 1.0f }));
	ImGui::PushStyleColor(ImGuiCol_Button, color);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4({ color.x, color.y, color.z, 0.8f }));

	button_clicked |= ImGui::Button(label, size);

	ImGui::PopStyleColor(4);

	return button_clicked;
}

bool LSGUI::Button(const char* label, const ImVec2& size)
{
	bool button_clicked = false;

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4({ 1.0f, 1.0f, 1.0f, 1.0f }));

	button_clicked |= ImGui::Button(label, size);

	ImGui::PopStyleColor(1);

	return button_clicked;
}
