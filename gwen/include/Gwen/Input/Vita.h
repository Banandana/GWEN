#pragma once
#ifndef GWEN_INPUT_VITA_H
#define GWEN_INPUT_VITA_H

#include "Gwen/InputHandler.h"
#include "Gwen/Gwen.h"
#include "Gwen/Controls/Canvas.h"

#include <psp2/touch.h>
#include <psp2/ctrl.h>

namespace Gwen
{
	namespace Input
	{
		class Vita
		{
			public:

				Vita() {}

				void Initialize( Gwen::Controls::Canvas* c )
				{
					_canvas = c;

					// Set touch sampling mode
					SceUInt32 samplingState = -1;
					sceTouchGetSamplingState(SCE_TOUCH_PORT_FRONT, &samplingState);
					if (samplingState != SCE_TOUCH_SAMPLING_STATE_START) {
						sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START);
					}

					int pmode = -1;
					sceCtrlGetSamplingMode(&pmode);
					// Set the control sampling mode
					if (pmode != SCE_CTRL_MODE_ANALOG_WIDE) {
						sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG_WIDE);
					}
				}

				// Math scaling for touch X
				float calculateX(int input_x) {
					return (
						((float)input_x / 1919.0f)
						* 960.0f
					 );
				}

				// Math scaling for touch Y
				float calculateY(int input_y) {
					return (
						((float)input_y / 1087.0f)
						* 544.0f
					 );
				}

				void Update() {

					// Set previous control data to current one before updating
					previous_control_data = current_control_data;

					// Poll for control data
					sceCtrlPeekBufferPositive(0, &current_control_data, 1);

					// Set previous touch data to current one
					previous_touch_data = current_touch_data;

					// Poll for touch data
					sceTouchRead(SCE_TOUCH_PORT_FRONT, &current_touch_data, SCE_TOUCH_MAX_REPORT);

					// Mouse movement
					if (current_touch_data.reportNum > 0) {
						// Report controls and mouse movement
						_canvas->InputMouseMoved(
							(int)calculateX(current_touch_data.report[0].x),
							(int)calculateY(current_touch_data.report[0].y),
							(int)(calculateX(current_touch_data.report[0].x) - calculateX(previous_touch_data.report[0].x)),
							(int)(calculateY(current_touch_data.report[0].y) - calculateY(previous_touch_data.report[0].y))
						);
					}

					_canvas->InputMouseButton( 0, (current_touch_data.reportNum > 0));

					// Register untouches as clicks
					//_canvas->InputMouseButton( 0, (current_control_data.buttons & SCE_CTRL_LTRIGGER) );
					//_canvas->InputMouseButton( 1, (current_control_data.buttons & SCE_CTRL_RTRIGGER) );

					_canvas->InputKey(Gwen::Key::Up, current_control_data.buttons & SCE_CTRL_UP);
					_canvas->InputKey(Gwen::Key::Down, current_control_data.buttons & SCE_CTRL_DOWN);
					_canvas->InputKey(Gwen::Key::Left, current_control_data.buttons & SCE_CTRL_LEFT);
					_canvas->InputKey(Gwen::Key::Right, current_control_data.buttons & SCE_CTRL_RIGHT);

					_canvas->InputKey(Gwen::Key::Space, current_control_data.buttons & SCE_CTRL_CROSS);
				}

			protected:

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
