/*  OnePAD - author: arcum42(@gmail.com)
 *  Copyright (C) 2009
 *
 *  Based on ZeroPAD, author zerofrog@gmail.com
 *  Copyright (C) 2006-2007
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
#include <string.h> // for memset
#define MAX_KEYS 24

extern bool IsAnalogKey(int index);

class KeyMap
{
    private:
        std::map<u32, u32> keysym_map;

        // Contains all simulated keys
        std::array<u32, MAX_KEYS> m_simulatedKeys;

    public:
        KeyMap ()
        {
            clear();
        }

        void clear()
        {
            keysym_map.clear();
            for (int i = 0; i < MAX_KEYS; i++)
            {
                m_simulatedKeys[i] = 0;
            }
        }

        void clear_key(int key)
        {
            // Erase the keyboard binded key
            u32 keysim = m_simulatedKeys[key];
            m_simulatedKeys[key] = 0;

            // erase gamepad entry (keysim map)
            keysym_map.erase(keysim);
        }

        __forceinline void set_key(int keysym, int index)
        {
            clear_key(index);
            keysym_map[keysym] = index;
            m_simulatedKeys[index] = keysym;
        }

        __forceinline int get_key(int keysym)
        {
            // You must use find instead of []
            // [] will create an element if the key does not exist and return 0
            std::map<u32, u32>::iterator it = keysym_map.find(keysym);
            if (it != keysym_map.end())
                return it->second;
            else
                return -1;
        }

        __forceinline bool key_exists(int key)
        {
            return (m_simulatedKeys[key] > 0);
        }

        __forceinline u32 get_sym(int key)
        {
            return m_simulatedKeys[key];
        }

        __forceinline void set_defaults()
        {
            clear();
#if defined(__unix__)
            set_key(XK_a, PAD_L2);
            set_key(XK_semicolon, PAD_R2);
            set_key(XK_w, PAD_L1);
            set_key(XK_p, PAD_R1);
            set_key(XK_i, PAD_TRIANGLE);
            set_key(XK_l, PAD_CIRCLE);
            set_key(XK_k, PAD_CROSS);
            set_key(XK_j, PAD_SQUARE);
            set_key(XK_v, PAD_SELECT);
            set_key(XK_n, PAD_START);
            set_key(XK_e, PAD_UP);
            set_key(XK_f, PAD_RIGHT);
            set_key(XK_d, PAD_DOWN);
            set_key(XK_s, PAD_LEFT);
#else
#endif
        }

        std::string get_name_from_sym(int keysym)
        {
            // Mouse
            if (keysym < 10)
            {
                switch (keysym)
                {
                    case 0:
                        return "";
                    case 1:
                        return "Mouse Left";
                    case 2:
                        return "Mouse Middle";
                    case 3:
                        return "Mouse Right";
                    default: // Use only number for extra button
                        return "Mouse " + std::to_string(keysym);
                }
            }

            return std::string(XKeysymToString(keysym));
        }

        std::string get_name_from_key(int key)
        {
            return get_name_from_sym(get_sym(key));
        }
};

class PADconf
{
    u32 ff_intensity;
    u32 sensibility;

public:
    union
    {
        struct
        {
            u16 forcefeedback : 1;
            u16 reverse_lx : 1;
            u16 reverse_ly : 1;
            u16 reverse_rx : 1;
            u16 reverse_ry : 1;
            u16 mouse_l : 1;
            u16 mouse_r : 1;
            u16 _free : 9;             // The 9 remaining bits are unused, do what you wish with them ;)
        } pad_options[GAMEPAD_NUMBER]; // One for each pads
        u32 packed_options;            // Only first 8 bits of each 16 bits series are really used, rest is padding
    };

    u32 log;
    u32 ftw;
    KeyMap k_map[GAMEPAD_NUMBER];
    std::array<size_t, GAMEPAD_NUMBER> unique_id;
    std::vector<std::string> sdl2_mapping;

    PADconf() { init(); }

    void init()
    {
        log = 0;
        ftw = 1;
        packed_options = 0;
        ff_intensity = 0x7FFF; // set it at max value by default
        sensibility = 500;
        for (int pad = 0; pad < GAMEPAD_NUMBER; pad++)
        {
            k_map[pad].clear();
        }
        unique_id.fill(0);
        sdl2_mapping.clear();
    }

    void set_joy_uid(u32 pad, size_t uid)
    {
        if (pad < GAMEPAD_NUMBER)
            unique_id[pad] = uid;
    }

    size_t get_joy_uid(u32 pad)
    {
        if (pad < GAMEPAD_NUMBER)
            return unique_id[pad];
        else
            return 0;
    }

    /**
	 * Return (a copy of) private member ff_intensity
	 **/
    u32 get_ff_intensity()
    {
        return ff_intensity;
    }

    /**
	 * Set intensity while checking that the new value is within valid range,
	 * more than 0x7FFF will cause pad not to rumble, and less than 0 is obviously bad.
	 **/
    void set_ff_intensity(u32 new_intensity)
    {
        if (new_intensity <= 0x7FFF)
        {
            ff_intensity = new_intensity;
        }
    }

    /**
	 * Set sensibility value.
	 * There will be an upper range, and less than 0 is obviously wrong.
	 * We are doing object oriented code, so members are definitely not supposed to be public.
	 **/
    void set_sensibility(u32 new_sensibility)
    {
        if (new_sensibility > 0)
        {
            sensibility = new_sensibility;
        }
        else
        {
            sensibility = 1;
        }
    }

    u32 get_sensibility()
    {
        return sensibility;
    }
};
extern PADconf g_conf;
