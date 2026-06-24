#pragma once
#include <glfw3.h>
#include "../src/lavish_core.h"

namespace lavish {

	void BeginFrame_GlfwImpl(GLFWwindow* window) {
		
		auto& mouseState = detail::gContext.mMouse;

		double x{}, y{};
		glfwGetCursorPos(window, &x, &y);

		mouseState.mPosition.x = static_cast<float>(x);
		mouseState.mPosition.y = static_cast<float>(y);

		struct ButtonMap { uint8_t mMask{}; int mGlfwBtn{}; };
		static ButtonMap sMaps[ 3 ] = {
			{ LAVISH_MOUSE_BTN_LEFT,	GLFW_MOUSE_BUTTON_LEFT		},
			{ LAVISH_MOUSE_BTN_MIDDLE,	GLFW_MOUSE_BUTTON_MIDDLE	},
			{ LAVISH_MOUSE_BTN_RIGHT,	GLFW_MOUSE_BUTTON_RIGHT		}
		};

		uint8_t lastDown = mouseState.mButtonsDown;

		for (const auto& btn : sMaps) {
			if (glfwGetMouseButton( window, btn.mGlfwBtn ) == GLFW_PRESS) {
				mouseState.mButtonsDown |= btn.mMask;
			} 
			else {
				mouseState.mButtonsDown &= ~btn.mMask;
			}
		}

		mouseState.mButtonsClicked = mouseState.mButtonsDown & ~lastDown;
		mouseState.mButtonsReleased = lastDown & ~mouseState.mButtonsDown;

	}

}