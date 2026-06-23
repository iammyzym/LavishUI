#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>
#include "src/lavish_core.h"
#include "backend/lavish_impl_glfw.h"
#include "src/lavish_widgets.h"

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const uint32_t kWindowWidth = 1000;
	const uint32_t kWindowHeight = 500;
	GLFWwindow* window = glfwCreateWindow(kWindowWidth, kWindowHeight, "test", nullptr, nullptr);

	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	lavish::Initialize();

	while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT);

		glfwPollEvents();
		lavish::ImplGlfwBeginFrame(window);

		lavish::BeginWindow("fiut", {100, 100}, { 250, 100 } );

		lavish::PollDrawList(kWindowWidth, kWindowHeight);

		glfwSwapBuffers(window);
	}
}