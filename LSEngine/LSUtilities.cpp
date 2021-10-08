#include "LSUtilities.h"

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

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

bool LSUtilities::LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height) {
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	*out_texture = image_texture;
	*out_width = image_width;
	*out_height = image_height;

	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

unsigned int LSUtilities::LoadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

unsigned char* LSUtilities::LoadTextureThreaded(const char* filename, int* image_width, int* image_height)
{
	unsigned char* image_data = stbi_load(filename, image_width, image_height, NULL, 4);
	return image_data;
}

std::tuple<float, float, float> LSUtilities::GetGPUMemoryUsage() {
	GLint nTotalMemoryInKB = 0;
	glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX, &nTotalMemoryInKB);

	GLint nCurAvailMemoryInKB = 0;
	glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &nCurAvailMemoryInKB);

	return std::make_tuple((nTotalMemoryInKB - nCurAvailMemoryInKB) / 1000.0f, nTotalMemoryInKB / 1000.0f, nCurAvailMemoryInKB / 1000.0f);
}