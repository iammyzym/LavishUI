#pragma once
#include "lavish_core.h"

namespace lavish {

	void BeginWindow( const char* label, Vector2 position, Vector2 size );
	bool Button( const char* label, Vector2 position, Vector2 size );
	void Checkbox( const char* label, bool* value, Vector2 position );

}

#ifndef LAVISH_IMPLEMENTATION
#	error Must define LAVISH_IMPLEMENTATION macro in cpp file
#endif

#ifdef LAVISH_IMPLEMENTATION

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

	bool Button( const char* label, Vector2 position, Vector2 size ) {

		WidgetID id = GetID( label );
		WidgetID& activeID = detail::gContext.mActiveID;

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

	void Checkbox( const char* label, bool* value, Vector2 position ) {

		WidgetID id = GetID( label );
		WidgetID& activeID = detail::gContext.mActiveID;

		const Vector2 checkboxSize = { 25, 25 };
		const Vector2 mousePos = detail::gContext.mMouse.mPosition;
		bool isHovered = CheckAABB( mousePos, position, checkboxSize );

		if (isHovered) {
			detail::gContext.mHoveredID = id;
			if ((detail::gContext.mMouse.mButtonsClicked & LAVISH_MOUSE_BTN_LEFT) && activeID == 0)
				activeID = id;
		}

		if (detail::gContext.mMouse.mButtonsReleased & LAVISH_MOUSE_BTN_LEFT) {
			if (activeID == id) {
				activeID = 0;
				*value = !(*value);
			}
		}

		Color32 finalColor = 0;

		if (*value) {
			finalColor = MakeColor( 0, 255, 0 );
		}
		else {
			finalColor = MakeColor( 255, 0, 0 );
		}

		detail::gContext.mMainDrawList.AddRect( position.x, position.y, checkboxSize.x, checkboxSize.y, finalColor );

	}

}

#endif