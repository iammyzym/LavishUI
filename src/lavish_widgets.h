#pragma once
#include "lavish_core.h"
namespace lavish {

	void BeginWindow(const char* label, Vector2 position, Vector2 size) {
		
		WidgetID id = GetID(label);

		detail::gContext.mMainDrawList.AddRect(position.x, position.y, size.x, size.y, MakeColor(255, 255, 255, 255));

		const Vector2& mousePos = detail::gContext.mMousePos;
		static bool bLastFrame = false;

		if (CheckAABB(mousePos, position, size)) {
			detail::gContext.mHoveredID = id;
			if (!bLastFrame) printf("Nachodzi\n");
			bLastFrame = true;
		}
		else bLastFrame = false;


	}

}