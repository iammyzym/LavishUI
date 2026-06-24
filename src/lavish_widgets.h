#pragma once
#include "lavish_core.h"
namespace lavish {

	void BeginWindow( const char* label, Vector2 position, Vector2 size ) {

		WidgetID id = GetID( label );

		const Vector2& mousePos = detail::gContext.mMouse.mPosition;

		Vector2 finalPosition = position;

		if (CheckAABB( mousePos, position, size )) {
			detail::gContext.mHoveredID = id;
			if (detail::gContext.mMouse.mButtonsDown & LAVISH_MOUSE_BTN_LEFT) {
				finalPosition = mousePos;
			}
		}

		detail::gContext.mMainDrawList.AddRect( finalPosition.x, finalPosition.y, size.x, size.y, MakeColor( 255, 255, 255, 255 ) );


	}

	bool Button( const char* label, Vector2 position, Vector2 size = { 100, 100 } ) {

		WidgetID id = GetID( label );
		WidgetID& activeID = detail::gContext.mActivatedID;

		const Vector2& mousePos = detail::gContext.mMouse.mPosition;
		bool isHovered = CheckAABB( mousePos, position, size );

		detail::gContext.mMainDrawList.AddRect( position.x, position.y, size.x, size.y, MakeColor( 255, 255, 255, 255 ) );

		if (isHovered) {
			detail::gContext.mHoveredID = id;
			if ((detail::gContext.mMouse.mButtonsClicked & LAVISH_MOUSE_BTN_LEFT) && activeID == 0)
				activeID = id;
		}

		if (detail::gContext.mMouse.mButtonsReleased & LAVISH_MOUSE_BTN_LEFT) {
			if (activeID == id) {
				activeID = 0;
				return isHovered;
			}
		}

		if (activeID == id && !(detail::gContext.mMouse.mButtonsDown & LAVISH_MOUSE_BTN_LEFT)) {
			activeID = 0;
		}

		return false;

	}

	bool Checkbox( const char* label, bool* value, Vector2 position ) {

		const Vector2 mousePos = detail::gContext.mMouse.mPosition;
		bool isHovered = CheckAABB( mousePos, position, { 20, 20 } );

	}

}