/*  dialog.cpp
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

#include "dialog.h"
#include <array>

void Dialog::create_page(int pad, wxNotebook *m_gp_notebook)
{
    /*
     * Define the size and the position of each button :
     * padding[ButtonID] : Width, Height, x position, y position
    */
    std::array<std::array<int, 4>, BUTTONS_LENGTH> padding;

    padding[PAD_L1]         = { 218, 28,  50, 175 };
    padding[PAD_L2]         = { 218, 28,  50, 104 };
    padding[PAD_L3]         = { 218, 28, 268, 641 };

    padding[PAD_R1]         = { 218, 28, 726, 175 };
    padding[PAD_R2]         = { 218, 28, 726, 104 };
    padding[PAD_R3]         = { 218, 28, 498, 641 };

    padding[PAD_TRIANGLE]   = { 218, 28, 726, 246 };
    padding[PAD_CIRCLE]     = { 218, 28, 726, 319 };
    padding[PAD_CROSS]      = { 218, 28, 726, 391 };
    padding[PAD_SQUARE]     = { 218, 28, 726, 463 };

    padding[PAD_START]      = { 218, 28, 503, 34 };
    padding[PAD_SELECT]     = { 218, 28, 273, 34 };

    padding[Analog]         = { 218, 28, 50, 452 };

    // Directional Pad
    padding[PAD_UP]         = { 100, 25, 108, 290 };
    padding[PAD_DOWN]       = { 100, 25, 108, 340 };
    padding[PAD_RIGHT]      = { 109, 25, 159, 315 };
    padding[PAD_LEFT]       = { 109, 25,  50, 315 };

    // Left Joystick
    padding[PAD_L_UP]       = { 100, 25, 325, 527 };
    padding[PAD_L_DOWN]     = { 100, 25, 325, 577 };
    padding[PAD_L_RIGHT]    = { 109, 25, 377, 552 };
    padding[PAD_L_LEFT]     = { 109, 25, 268, 552 };

    // Right Joystick
    padding[PAD_R_UP]       = { 100, 25, 555, 527 };
    padding[PAD_R_DOWN]     = { 100, 25, 555, 577 };
    padding[PAD_R_RIGHT]    = { 109, 25, 607, 552 };
    padding[PAD_R_LEFT]     = { 109, 25, 498, 552 };

    padding[JoyL_config]    = { 180, 28,  50, 550 };
    padding[JoyR_config]    = { 180, 28, 764, 550 };
    padding[Gamepad_config] = { 180, 28,  50, 585 };
    padding[Set_all]        = { 180, 28, 764, 585 };

    // Tabs panels
    m_notebook_page[pad] = new wxPanel(
        m_gp_notebook,
        wxID_ANY,
        wxDefaultPosition,
        wxDefaultSize);

    // New page creation
    m_gp_notebook->AddPage(m_notebook_page[pad], wxString::Format(_("Gamepad %i"), pad));

    wxBoxSizer *note_box = new wxBoxSizer( wxHORIZONTAL );

    // Panel for the actual gamepad picture and gui.
    m_gamepad_panel[pad] = new opPanel(
        m_notebook_page[pad],
        wxID_ANY,
        wxDefaultPosition,
        wxSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    // Create a new ListCtrl
    m_pad_ctrl_list[pad] = new wxListCtrl(m_notebook_page[pad], wxID_ANY, wxDefaultPosition, wxSize(150, DEFAULT_HEIGHT), wxLC_REPORT);

    // Set up a list for our controls. Only shows keys at the moment.
    // To do: add buttons below it. Clear all, Delete, and probably move "Set All Buttons" there.
    m_pad_ctrl_list[pad]->InsertColumn(0, _T("Input"), wxLIST_FORMAT_LEFT, -1);
    m_pad_ctrl_list[pad]->InsertColumn(1, _T("Mapped To"), wxLIST_FORMAT_LEFT, -1);

    for (int j = 0; j < BUTTONS_LENGTH; ++j)
    {
        // Gamepad buttons
        m_btn_gamepad[pad][j] = new wxButton(
        m_gamepad_panel[pad],                         // Parent
            wxID_HIGHEST + j + 1,                  // ID
            _T("Undefined"),                       // Label
            wxPoint(padding[j][2], padding[j][3]), // Position
            wxSize(padding[j][0], padding[j][1])   // Size
            );
    }

    // Set the other buttons labels
    m_btn_gamepad[pad][JoyL_config]->SetLabel(_T("&Left Joystick Config"));
    m_btn_gamepad[pad][JoyR_config]->SetLabel(_T("&Right Joystick Config"));
    m_btn_gamepad[pad][Gamepad_config]->SetLabel(_T("&Gamepad Configuration"));
    m_btn_gamepad[pad][Set_all]->SetLabel(_T("&Set All Buttons"));
    m_btn_gamepad[pad][Analog]->Disable();

    note_box->Add(m_pad_ctrl_list[pad], 0, wxALL | wxEXPAND, 5);
    note_box->Add(m_gamepad_panel[pad], 0, wxALL | wxEXPAND, 5);

    m_notebook_page[pad]->SetSizer(note_box);
}

// Constructor of Dialog
Dialog::Dialog()
    : wxDialog(NULL,                                  // Parent
               wxID_ANY,                              // ID
               _T("OnePad configuration"),            // Title
               wxDefaultPosition,                     // Position
               wxDefaultSize /*wxSize(DEFAULT_WIDTH, DEFAULT_HEIGHT)*/, // Width + Length
               // Style
               wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN)
{

    wxBoxSizer *main_box = new wxBoxSizer( wxVERTICAL );

    // create a new Notebook
    m_gp_notebook = new wxNotebook(this, wxID_ANY);
    main_box->Add(m_gp_notebook);

    for (int i = 0; i < GAMEPAD_NUMBER; ++i)
    {
        create_page(i, m_gp_notebook);
    }

    main_box->Add(CreateStdDialogButtonSizer(wxOK | wxCANCEL | wxAPPLY), 0, wxALL | wxEXPAND, 5);

    Bind(wxEVT_BUTTON, &Dialog::OnButtonClicked, this);

    main_box->SetSizeHints(this);
    SetSizerAndFit(main_box);
}

void Dialog::InitDialog()
{
    GamePad::EnumerateGamePads(s_vgamePad); // activate gamepads
    LoadConfig();                           // Load configuration from the ini file
    repopulate();                           // Set label and fit simulated key array
}

/****************************************/
/*********** Events functions ***********/
/****************************************/

void Dialog::OnButtonClicked(wxCommandEvent &event)
{
    // Display a message each time the button is clicked
    wxButton *bt_tmp = (wxButton *)event.GetEventObject(); // get the button object
    int bt_id = bt_tmp->GetId() - wxID_HIGHEST - 1;        // get the real ID
    int pad = m_gp_notebook->GetSelection();        // get the tab ID (equivalent to the gamepad id)

    switch (bt_id)
    {
        case Gamepad_config:
        {
            GamepadConfiguration gamepad_config(pad, this);

            gamepad_config.InitGamepadConfiguration();
            gamepad_config.ShowModal();
            break;
        }

        case JoyL_config:
        {
            JoystickConfiguration joystick_config(pad, true, this);

            joystick_config.InitJoystickConfiguration();
            joystick_config.ShowModal();
            break;
        }

        case JoyR_config:
        {
            JoystickConfiguration joystick_config(pad, false, this);

            joystick_config.InitJoystickConfiguration();
            joystick_config.ShowModal();
            break;
        }

        case Set_all:
            for (int i = 0; i < MAX_KEYS; ++i)
            {
                set_tab_img_visible(pad, i, true);
                config_key(pad, i);
                set_tab_img_visible(pad, i, false);

                usleep(500000); // give enough time to the user to release the button
            }
            break;

        case Ok:
        case wxID_OK - wxID_HIGHEST - 1: // Hack. bt_id subtracts this, and we have the raw ids, so...
            SaveConfig();
            Close();
            EndModal(wxID_CANCEL);
            break;

        case Apply:
        case wxID_APPLY - wxID_HIGHEST - 1:
            SaveConfig();
            break;

        case Cancel:
        case wxID_CANCEL - wxID_HIGHEST - 1:
            Close();
            EndModal(wxID_CANCEL);
            break;

        default:
            if (bt_id >= 0 && bt_id <= PAD_R_LEFT) // if the button ID is a gamepad button
            {
                bt_tmp->Disable();
                config_key(pad, bt_id);
                bt_tmp->Enable();
            }
            break;
    }
}

/****************************************/
/*********** Methods functions **********/
/****************************************/

bool Dialog::capture_key(int pad, int key)
{
    u32 key_pressed = 0;

    if (PollX11KeyboardMouseEvent(key_pressed))
    {
        // special case for keyboard/mouse to handle multiple keys
        // Note: key_pressed == 0 when ESC is hit to abort the capture
        if (key_pressed > 0)
        {
            g_conf.k_map[pad].set_key(key_pressed, key);
        }
        return true;
    }
    return false;
}

void Dialog::config_key(int pad, int key)
{
    bool captured = false;

    while (!captured)
    {
        if (capture_key(pad, key))
        {
            m_btn_gamepad[pad][key]->SetLabel(g_conf.k_map[pad].get_name_from_key(key).c_str());
            captured = true;
        }
    }

    repopulate();
}

// Set button values.
// Needs to be changed to handle keyboard, mouse, *and* gamepads.
void Dialog::repopulate()
{
    for (int pad = 0; pad < GAMEPAD_NUMBER; ++pad)
    {
        // Clear list before repopulating.
        m_pad_ctrl_list[pad]->DeleteAllItems();

        // keyboard/mouse key
        for (int key = 0; key < MAX_KEYS; key++)
        {
            if (g_conf.k_map[pad].key_exists(key))
            {
                const std::string name = g_conf.k_map[pad].get_name_from_key(key);
                m_btn_gamepad[pad][key]->SetLabel(name);

                long itemIndex = m_pad_ctrl_list[pad]->InsertItem(0, wxString(gamePadBtnNames[key]));
                m_pad_ctrl_list[pad]->SetItem(itemIndex, 1, wxString(name));
                //fprintf(stderr, "Gamepad %i: Button %s = %s.\n", pad, gamePadBtnNames[key], name.c_str());
            }
        }

        // Now that we actually have text in the list, resize the columns to fit it.
        m_pad_ctrl_list[pad]->SetColumnWidth(0, wxLIST_AUTOSIZE);
        m_pad_ctrl_list[pad]->SetColumnWidth(1, wxLIST_AUTOSIZE);
    }
}

void Dialog::set_tab_img_visible(int pad, int i, bool visible)
{
    if (visible)
    {
        m_gamepad_panel[pad]->ShowImg(gamePadimages[i]);
    }
    else
    {
        m_gamepad_panel[pad]->HideImg(gamePadimages[i]);
    }
    m_gamepad_panel[pad]->Refresh();
    m_gamepad_panel[pad]->Update();
}

// Main
void DisplayDialog()
{
    if (g_conf.ftw)
    {
        // We need all the functionality in *one* plugin!!!
        wxString info("The OnePad GUI is provided to map the keyboard/mouse to the virtual PS2 pad.\n\n"
                      "Gamepads/Joysticks are plug and play. The active gamepad can be selected in the 'Gamepad Configuration' panel.\n\n"
                      "If you prefer to manually map your gamepad, you should use the 'onepad-legacy' plugin instead.");

        wxMessageDialog ftw(nullptr, info);
        ftw.ShowModal();

        g_conf.ftw = 0;
        SaveConfig();
    }

    Dialog dialog;

    dialog.InitDialog();
    dialog.ShowModal();
}
