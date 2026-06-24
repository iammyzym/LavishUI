#pragma once
#include <vector>
#include <cstdint>
#include "lavish_math.h"
#include "lavish_types.h"
#include <assert.h>

namespace lavish {

	namespace detail {

		inline constexpr const char* gVertexShaderSource = R"(
			#version 450 core
			layout(location = 0) in vec2 inPos;
			layout(location = 1) in vec2 inUv;
			layout(location = 2) in vec4 inColor;
			
			out vec2 outFragUv;
			out vec4 outFragColor;

			layout(location = 0) uniform vec2 uScreenSize;

			void main() {
				outFragUv = inUv;
				outFragColor = inColor;
				
				float x = (inPos.x / uScreenSize.x) * 2.0 - 1.0;
				float y = 1.0 - (inPos.y / uScreenSize.y) * 2.0;

				gl_Position = vec4(x, y, 0.0, 1.0);
			}
		)";

		inline constexpr const char* gFragmentShaderSource = R"(
			#version 450 core

			in vec2 outFragUv;
			in vec4 outFragColor;

			out vec4 fragColor;

			void main() {
				fragColor = outFragColor;
			}

		)";

		struct Vertex {
			float mX{}, mY{};
			float mU{}, mV{};
			uint32_t mColor;
		};

		class DrawList {
		public:

			std::vector<Vertex> mVertexBuffer;
			std::vector<uint32_t> mIndexBuffer;

			void Clear( ) {
				mVertexBuffer.clear( );
				mIndexBuffer.clear( );
			}

			void AddRect( float x, float y, float width, float height, uint32_t color ) {

				uint32_t baseIndex = static_cast< uint32_t >(mVertexBuffer.size( ));

				Vertex topLeft		= { x,			y,			0.0f, 0.0f, color };
				Vertex topRight		= { x + width,	y,			1.0f, 0.0f, color };
				Vertex bottomLeft	= { x,			y + height, 0.0f, 1.0f, color };
				Vertex bottomRight	= { x + width,	y + height, 1.0f, 1.0f, color };

				mVertexBuffer.push_back(topLeft);
				mVertexBuffer.push_back(topRight);
				mVertexBuffer.push_back(bottomLeft);
				mVertexBuffer.push_back(bottomRight);

				mIndexBuffer.push_back(baseIndex + 0);
				mIndexBuffer.push_back(baseIndex + 1);
				mIndexBuffer.push_back(baseIndex + 2);

				mIndexBuffer.push_back(baseIndex + 1);
				mIndexBuffer.push_back(baseIndex + 3);
				mIndexBuffer.push_back(baseIndex + 2);

			}

		};

		struct ContextIO {

			detail::DrawList mMainDrawList{};
			WidgetID mHoveredID = 0;
			WidgetID mActivatedID = 0;

			struct MouseState {

#				define LAVISH_MOUSE_BTN_LEFT	(1 << 0) // 0000`0001
#				define LAVISH_MOUSE_BTN_MIDDLE	(1 << 1) // 0000`0010
#				define LAVISH_MOUSE_BTN_RIGHT	(1 << 2) // 0000`0100

				Vector2 mPosition{};
				uint8_t mButtonsDown = 0;
				uint8_t mButtonsClicked = 0;
				uint8_t mButtonsReleased = 0;

			} mMouse;


			void( *mBackendPollEventsFn )(float, float) = nullptr;

		};

		ContextIO gContext;

	}

	void PollDrawList( float screenWidth, float screenHeight ) {
		
		auto& drawList = detail::gContext.mMainDrawList;

		if (drawList.mVertexBuffer.empty()) return;
		
		assert( detail::gContext.mBackendPollEventsFn );
		detail::gContext.mBackendPollEventsFn( screenWidth, screenHeight );

		drawList.Clear();

	}

	constexpr uint32_t MakeColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
		return
			static_cast<uint32_t>(r) |
			(static_cast<uint32_t>(g) << 8) |
			(static_cast<uint32_t>(b) << 16) |
			(static_cast<uint32_t>(a) << 24);
	}

	WidgetID GetID(const char* label) {
		size_t hash = 2166136261u;
		while (*label) {
			hash ^= (unsigned char)*label++;
			hash *= 16777619u;
		}
		return static_cast<WidgetID>(hash);
	}

	bool CheckAABB(Vector2 mousePos, Vector2 rectPosition, Vector2 rectSize) {

		bool overlapX = ((mousePos.x >= rectPosition.x) && (mousePos.x <= rectPosition.x + rectSize.x));
		bool overlapY = ((mousePos.y >= rectPosition.y) && (mousePos.y <= rectPosition.y + rectSize.y));
		return (overlapX && overlapY);

	}

	inline detail::DrawList& GetDrawList( ) {
		return detail::gContext.mMainDrawList;
	}

}