#include "LSUtilities.h"

void LSUtilities::InitGLAD() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::runtime_error("[!] Failed to initialize GLAD!\n");
	}
}

void LSUtilities::Shader::ShaderCheckCompile(unsigned int shader) {
	int success;
	char infoLog[512];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::string msg = fmt::format("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{}", infoLog);
		throw std::runtime_error(msg);
	}
}

void LSUtilities::Shader::ShaderCheckLink(unsigned int ID) {
	int success;
	char infoLog[512];

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::string msg = fmt::format("ERROR::SHADER::PROGRAM::LINKING_FAILED\n{}", infoLog);
		throw std::runtime_error(msg);
	}
}

void LSUtilities::GUI::InitImGUI(GLFWwindow* window, unsigned int glslVerMajor, unsigned int glslVerMinor) {
	std::string glsl_version = fmt::format("#version {}{}0 core", glslVerMajor, glslVerMinor);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version.c_str());
}

void LSUtilities::GUI::ApplyTheme() {
	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	colors[ImGuiCol_Text] = ImVec4(169.f / 255.f, 177.f / 255.f, 213.f / 255.f, 1.000f);
	colors[ImGuiCol_TextDisabled] = ImVec4(103.f / 255.f, 108.f / 255.f, 130.f / 255.f, 1.000f);
	colors[ImGuiCol_WindowBg] = ImVec4(20.f / 255.f, 20.f / 255.f, 27.f / 255.f, 1.000f);
	colors[ImGuiCol_ChildBg] = ImVec4(1.280f, 0.280f, 0.280f, 0.000f);
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
	colors[ImGuiCol_Header] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
	colors[ImGuiCol_SeparatorHovered] = ImVec4(62.f / 255.f, 89.f / 255.f, 159.f / 255.f, 0.800f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(62.f / 255.f, 89.f / 255.f, 159.f / 255.f, 1.000f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(26.f / 255.f, 27.f / 255.f, 38.f / 255.f, 1.000f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(62.f / 255.f, 89.f / 255.f, 159.f / 255.f, 0.800f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(62.f / 255.f, 89.f / 255.f, 159.f / 255.f, 1.000f);
	colors[ImGuiCol_Tab] = ImVec4(35.f / 255.f, 35.f / 255.f, 42.f / 255.f, 1.000f);
	colors[ImGuiCol_TabHovered] = ImVec4(35.f / 255.f, 35.f / 255.f, 42.f / 255.f, 0.800f);
	colors[ImGuiCol_TabActive] = ImVec4(35.f / 255.f, 35.f / 255.f, 42.f / 255.f, 1.000f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(25.f / 255.f, 25.f / 255.f, 32.f / 255.f, 1.000f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(25.f / 255.f, 25.f / 255.f, 32.f / 255.f, 1.000f);
	// colors[ImGuiCol_DockingPreview]         = ImVec4(1.000f, 0.391f, 0.000f, 0.781f);
	// colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);

	style->ChildRounding = 4.0f;
	style->FrameBorderSize = 1.0f;
	style->FrameRounding = 2.0f;
	style->GrabMinSize = 7.0f;
	style->PopupRounding = 2.0f;
	style->ScrollbarRounding = 12.0f;
	style->ScrollbarSize = 13.0f;
	style->TabBorderSize = 1.0f;
	style->TabRounding = 0.0f;
	style->WindowRounding = 4.0f;
}

/**
 * Returns timestamp as string in format HH:MM P
 */
std::string LSUtilities::GetTimestamp() {
	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, 80, "%I:%M %p", timeinfo);
	puts(buffer);

	std::string timestamp;
	timestamp += buffer;

	return timestamp;
}

void LSUtilities::SetWindowIcon(GLFWwindow* window, const char* iconPath) {
	int iconWidth, iconHeight, iconChannel;
	stbi_uc* icon = stbi_load(iconPath, &iconWidth, &iconHeight, &iconChannel, 0);
	if (icon == NULL) throw std::runtime_error("Application icon could not be loaded!");
	GLFWimage appIcon[1];
	appIcon->height = iconHeight;
	appIcon->width = iconWidth;
	appIcon[0].pixels = icon;
	glfwSetWindowIcon(window, 1, appIcon);
}

std::string LSUtilities::ReadFileToString(const char* filePath, const char* category) {
	std::string fileContents;
	std::ifstream fileStream;

	fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		fileStream.open(filePath);
		std::stringstream fileStringStream;
		fileStringStream << fileStream.rdbuf();
		fileStream.close();
		fileContents = fileStringStream.str();
	}
	catch (std::ifstream::failure& e) {
		std::string msg = fmt::format("ERROR::{}::FILE_NOT_SUCCESSFULLY_READ", category);
		throw std::runtime_error(msg.c_str());
	}

	return fileContents;
}

int LSUtilities::OutputLog(std::vector<std::string> logContents) {
	std::string logString = "[DEBUG LOG]\n-----------\n";

	for (auto& logentry : logContents) {
		logString += logentry + "\n";
	}

	time_t timestamp = std::time(nullptr);
	std::string filename = fmt::format("logs/lsengine_debug_log_{}.txt", timestamp);

	try {
		std::ofstream out(filename.c_str());
		out << logString;
		out.close();

		return 0;
	}
	catch (std::ofstream::failure& e) {
		std::cout << "ERROR::LOG::COULD_NOT_EXPORT" << std::endl;

		return 1;
	}
}