#pragma once
#ifndef GWEN_INPUT_VITA_H
#define GWEN_INPUT_VITA_H

#include <psp2/touch.h>
#include <psp2/ctrl.h>

namespace Gwen {
	namespace Input {
		class Vita {
			public:
				Vita();
				void Initialize( Gwen::Controls::Canvas* c );
				void Update();
				void GetCursorPos( Gwen::Point & po );
			protected:
				Gwen::Point cursorPos;
				Gwen::Controls::Canvas*	_canvas;
				// Input variables
				SceCtrlData current_control_data;
				SceCtrlData previous_control_data;
				SceTouchData current_touch_data;
				SceTouchData previous_touch_data;
		};
	}
}

#endif
