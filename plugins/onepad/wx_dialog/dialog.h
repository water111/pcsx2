/*  dialog.h
 *  PCSX2 Dev Team
 *  Copyright (C) 2015
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#pragma once

#ifndef __DIALOG_H__
#define __DIALOG_H__

#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/frame.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/effects.h>
#include <wx/rawbmp.h>
#include <wx/graphics.h>
#include <wx/timer.h>
#include <wx/listctrl.h>

#include <string>
#include <sstream>

#include "../GamePad.h"
#include "../keyboard.h"
#include "../onepad.h"
#include "opPanel.h"

#include "GamepadConfiguration.h"
#include "JoystickConfiguration.h"

// Allow to found quickly button id
// e.g L2 → 0, triangle → 4, ...
// see onepad.h for more details about gamepad button id

enum gui_buttons {
    Analog = PAD_R_LEFT + 1, // Analog button (not yet supported ?)
    JoyL_config,             // Left Joystick Configuration
    JoyR_config,             // Right Joystick Configuration
    Gamepad_config,          // Gamepad Configuration
    Set_all,                 // Set all buttons
    Apply,                   // Apply modifications without exit
    Ok,                      // Apply modifications and exit
    Cancel                   // Exit without apply modificatons
};

#define BUTTONS_LENGTH 29 // numbers of buttons on the gamepad
#define UPDATE_TIME 5
#define DEFAULT_WIDTH 1000
#define DEFAULT_HEIGHT 700

static const char *gamePadBtnNames[PAD_R_LEFT + 1] [[gnu::unused]] = {
    "L2",
    "R2",
    "L1",
    "R1",
    "Triangle",
    "Circle",
    "Cross",
    "Square",
    "Select",
    "L3",
    "R3",
    "Start",
    "Up",
    "Right",
    "Down",
    "Left",
    "L Joy Up",
    "L Joy Right",
    "L Joy Left",
    "L Joy Up",
    "R Joy Up",
    "R Joy Right",
    "R Joy Down",
    "R Joy Left"
};

static const gui_img gamePadimages[PAD_R_LEFT + 1] [[gnu::unused]] = {
    img_l2,   // L2 button
    img_r2,       // R2 button
    img_l1,       // L1 button
    img_r1,       // R1 button
    img_triangle, // Triangle button ▲
    img_circle,   // Circle button ●
    img_cross,    // Cross button ✖
    img_square,   // Square button ■
    img_select,   // Select button
    img_l3,       // Left joystick button (L3)
    img_r3,       // Right joystick button (R3)
    img_start,    // Start button
    img_dp_up,       // Directional pad ↑
    img_dp_right,    // Directional pad →
    img_dp_bottom,     // Directional pad ↓
    img_dp_left,     // Directional pad ←
    img_l_arrow_up,     // Left joystick (Up) ↑
    img_l_arrow_right,  // Left joystick (Right) →
    img_l_arrow_bottom,   // Left joystick (Down) ↓
    img_l_arrow_left,   // Left joystick (Left) ←
    img_r_arrow_up,     // Right joystick (Up) ↑
    img_r_arrow_right,  // Right joystick (Right) →
    img_r_arrow_bottom,   // Right joystick (Down) ↓
    img_r_arrow_left    // Right joystick (Left) ←
};

class Dialog : public wxDialog
{
    // Panels
    opPanel *m_gamepad_panel[GAMEPAD_NUMBER]; // Gamepad Tabs box
    wxPanel *m_notebook_page[GAMEPAD_NUMBER];
    // Lists
    wxListCtrl *m_pad_ctrl_list[GAMEPAD_NUMBER];
    // Notebooks
    wxNotebook *m_gp_notebook; // Joysticks Tabs
    // Buttons
    wxButton *m_btn_gamepad[GAMEPAD_NUMBER][BUTTONS_LENGTH]; // Joystick button use to modify the button mapping
    // Timer
    wxTimer m_time_update_gui;

    // methods
    void config_key(int, int);
    bool capture_key(int pad, int key);
    void repopulate();
    void set_tab_img_visible(int pad, int i, bool visible);
    void create_page(int i, wxNotebook *m_gp_notebook);

    // Events
    void OnButtonClicked(wxCommandEvent &);
    void JoystickEvent(wxTimerEvent &);

public:
    Dialog();
    void InitDialog();
    void show();
};

extern void DisplayDialog(); // Main function

#endif // __DIALOG_H__
