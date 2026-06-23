#pragma once
#include <glfw3.h>
#include "../src/lavish_core.h"

namespace lavish {

	void ImplGlfwBeginFrame(GLFWwindow* window) {
		
		double x{}, y{};
		glfwGetCursorPos(window, &x, &y);

		detail::gContext.mMousePos.x = static_cast<float>(x);
		detail::gContext.mMousePos.y = static_cast<float>(y);

	}

}