/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2021, Andreas Merkle
 * http://www.blue-andi.de
 * vscp@blue-andi.de
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

/*******************************************************************************
    DESCRIPTION
*******************************************************************************/
/**
@brief  Platform specific utility functions.
@file   platform.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see platform.h

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "platform.h"
#include <stdio.h>

#ifdef _WIN32

#include <conio.h>

#endif  /* _WIN32 */

#ifdef __linux__

#include <termios.h>
#include <sys/select.h>
#include <unistd.h>

#endif  /* __linux__ */

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/*******************************************************************************
    MACROS
*******************************************************************************/

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

#ifdef _WIN32

static void platform_win_echoOff(void);
static void platform_win_echoOn(void);
static int platform_win_kbhit(void);
static int platform_win_getch(void);
static void platform_win_sleepMS(long valueMS);
static void platform_win_setTextColor(PLATFORM_COLOR color);
static void platform_win_setTextBgColor(PLATFORM_COLOR color);
static uint8_t  platform_win_getTextColor(void);

#endif  /* _WIN32 */

#ifdef __linux__

static void platform_linux_echoOff(void);
static void platform_linux_echoOn(void);
static int platform_linux_kbhit(void);
static int platform_linux_getch(void);
static void platform_linux_sleepMS(long valueMS);
static void platform_linux_setTextColor(PLATFORM_COLOR color);
static void platform_linux_setTextBgColor(PLATFORM_COLOR color);

#endif  /* __linux__ */

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

#ifdef __linux__

/** Terminal settings backup */
static struct termios   platform_terminalOld;

/** Text color mapping table */
static const uint8_t  platform_linux_colors[] =
{
    30, /* Black */
    31, /* Red */
    32, /* Green */
    33, /* Yellow */
    34, /* Blue */
    35, /* Magenta */
    36, /* Cyan */
    37, /* Light grey */
    91, /* Light red */
    92, /* Light green */
    93, /* Light yellow */
    94, /* Light blue */
    95, /* Light magenta */
    96, /* Light cyan */
    97  /* White */
};

/** Background color mapping table */
static const uint8_t  platform_linux_bgColors[] =
{
    40,     /* Black */
    41,     /* Red */
    42,     /* Green */
    43,     /* Yellow */
    44,     /* Blue */
    45,     /* Magenta */
    46,     /* Cyan */
    47,     /* Light grey */
    101,    /* Light red */
    102,    /* Light green */
    103,    /* Light yellow */
    104,    /* Light blue */
    105,    /* Light magenta */
    106,    /* Light cyan */
    107     /* White */
};

#endif  /* __linux__ */

#ifdef _WIN32

/** Text and background color */
static uint8_t      platform_color  = 0;

/** Text color mapping table */
static const uint8_t  platform_win_colors[] =
{
    0,                                                                          /* Black */
    FOREGROUND_RED,                                                             /* Red */
    FOREGROUND_GREEN,                                                           /* Green */
    FOREGROUND_RED + FOREGROUND_GREEN,                                          /* Yellow */
    FOREGROUND_BLUE,                                                            /* Blue */
    FOREGROUND_RED + FOREGROUND_BLUE,                                           /* Magenta */
    FOREGROUND_GREEN + FOREGROUND_BLUE,                                         /* Cyan */
    FOREGROUND_RED + FOREGROUND_GREEN + FOREGROUND_BLUE,                        /* Grey */
    FOREGROUND_INTENSITY + FOREGROUND_RED,                                      /* Light red */
    FOREGROUND_INTENSITY + FOREGROUND_GREEN,                                    /* Light green */
    FOREGROUND_INTENSITY + FOREGROUND_RED + FOREGROUND_GREEN,                   /* Light yellow */
    FOREGROUND_INTENSITY + FOREGROUND_BLUE,                                     /* Light blue */
    FOREGROUND_INTENSITY + FOREGROUND_RED + FOREGROUND_BLUE,                    /* Light magenta */
    FOREGROUND_INTENSITY + FOREGROUND_GREEN + FOREGROUND_BLUE,                  /* Light cyan */
    FOREGROUND_INTENSITY + FOREGROUND_RED + FOREGROUND_GREEN + FOREGROUND_BLUE  /* White */
};

/** Background color mapping table */
static const uint8_t  platform_win_bgColors[] =
{
    0,                                                                          /* Black */
    BACKGROUND_RED,                                                             /* Red */
    BACKGROUND_GREEN,                                                           /* Green */
    BACKGROUND_RED + BACKGROUND_GREEN,                                          /* Yellow */
    BACKGROUND_BLUE,                                                            /* Blue */
    BACKGROUND_RED + BACKGROUND_BLUE,                                           /* Magenta */
    BACKGROUND_GREEN + BACKGROUND_BLUE,                                         /* Cyan */
    BACKGROUND_RED + BACKGROUND_GREEN + BACKGROUND_BLUE,                        /* Grey */
    BACKGROUND_INTENSITY + BACKGROUND_RED,                                      /* Light red */
    BACKGROUND_INTENSITY + BACKGROUND_GREEN,                                    /* Light green */
    BACKGROUND_INTENSITY + BACKGROUND_RED + BACKGROUND_GREEN,                   /* Light yellow */
    BACKGROUND_INTENSITY + BACKGROUND_BLUE,                                     /* Light blue */
    BACKGROUND_INTENSITY + BACKGROUND_RED + BACKGROUND_BLUE,                    /* Light magenta */
    BACKGROUND_INTENSITY + BACKGROUND_GREEN + BACKGROUND_BLUE,                  /* Light cyan */
    BACKGROUND_INTENSITY + BACKGROUND_RED + BACKGROUND_GREEN + BACKGROUND_BLUE  /* White */
};

#endif  /* _WIN32 */

/*******************************************************************************
    GLOBAL VARIABLES
*******************************************************************************/

/*******************************************************************************
    GLOBAL FUNCTIONS
*******************************************************************************/

/**
 * This function initializes the module.
 */
extern void platform_init(void)
{
#ifdef __linux__

    /* Remember the terminal settings */
    (void)tcgetattr(STDIN_FILENO, &platform_terminalOld);

#endif  /* __linux__ */

#ifdef _WIN32

    /* Remember the console color */
    platform_color = platform_win_getTextColor();

#endif  /* _WIN32 */

    return;
}

/**
 * This function de-initializes the module.
 */
extern void platform_deInit(void)
{
#ifdef __linux__

    /* Restore terminal settings */
    (void)(void)tcsetattr(STDIN_FILENO, TCSANOW, &platform_terminalOld);

#endif  /* __linux__ */

    platform_restoreTextColor();
    platform_restoreTextBgColor();

    return;
}

/**
 * Disable the console echo.
 */
extern void platform_echoOff(void)
{
#ifdef _WIN32

    platform_win_echoOff();

#endif  /* _WIN32 */

#ifdef __linux__

    platform_linux_echoOff();

#endif  /* __linux__ */

    return;
}

/**
 * Enable the console echo.
 */
extern void platform_echoOn(void)
{
#ifdef _WIN32

    platform_win_echoOn();

#endif  /* _WIN32 */

#ifdef __linux__

    platform_linux_echoOn();

#endif  /* __linux__ */

    return;
}

/**
 * This function is not defined as part of the ANSI C/C++ standard.
 * It is generally used by Borland's family of compilers. It returns a positive
 * non-zero integer if a key is in the keyboard buffer. It will not wait for a
 * key to be pressed.
 *
 * @return Status
 * @retval 0    No key pressed
 * @retval > 0  Key pressed
 * @retval < 0  Error
 */
extern int platform_kbhit(void)
{
#ifdef _WIN32

    return platform_win_kbhit();

#endif  /* _WIN32 */

#ifdef __linux__

    return platform_linux_kbhit();

#endif  /* __linux__ */
}

/**
 * This function is not defined as part of the ANSI C/C++ standard.
 * It is generally used by Borland's family of compilers. It waits until a key is
 * pressed and returns its key value.
 *
 * @return Key value
 * @retval 0    Aborted
 * @retval < 0  Key value
 * @retval -1   Error
 */
extern int platform_getch(void)
{
#ifdef _WIN32

    return platform_win_getch();

#endif  /* _WIN32 */

#ifdef __linux__

    return platform_linux_getch();

#endif  /* __linux__ */
}

/**
 * This function suspends the execution of the current thread until the time-out
 * interval elapses.
 *
 * @param[in] valueMS   Timeout value in ms
 */
extern void platform_delay(long valueMS)
{
#ifdef _WIN32

    return platform_win_sleepMS(valueMS);

#endif  /* _WIN32 */

#ifdef __linux__

    return platform_linux_sleepMS(valueMS);

#endif  /* __linux__ */

    return;
}

/**
 * Set text foreground color.
 *
 * @param[in] color Color
 */
extern void platform_setTextColor(PLATFORM_COLOR color)
{
#ifdef _WIN32

    return platform_win_setTextColor(color);

#endif  /* _WIN32 */

#ifdef __linux__

    return platform_linux_setTextColor(color);

#endif  /* __linux__ */

    return;
}

/**
 * Set text background color.
 *
 * @param[in] color Color
 */
extern void platform_setTextBgColor(PLATFORM_COLOR color)
{
#ifdef _WIN32

    return platform_win_setTextBgColor(color);

#endif  /* _WIN32 */

#ifdef __linux__

    return platform_linux_setTextBgColor(color);

#endif  /* __linux__ */

    return;
}

/**
 * Restore the text color to the default value.
 */
extern void platform_restoreTextColor(void)
{
#ifdef __linux__

    const uint8_t   defaultForegroundColor  = 39;

    printf("\033[%um", defaultForegroundColor);

#endif  /* __linux__ */

#ifdef _WIN32

    platform_win_setTextColor((platform_color >> 0)& 0x0f);

#endif  /* _WIN32 */

    return;
}

/**
 * Restore the text background color to the default value.
 */
extern void platform_restoreTextBgColor(void)
{
#ifdef __linux__

    const uint8_t   defaultBackgroundColor  = 49;

    printf("\033[%um", defaultBackgroundColor);

#endif  /* __linux__ */

#ifdef _WIN32

    platform_win_setTextBgColor((platform_color >> 4) & 0x0f);

#endif  /* _WIN32 */

    return;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

#ifdef _WIN32

/**
 * Disable the console echo.
 */
static void platform_win_echoOff(void)
{
    /* Not supported */
    return;
}

/**
 * Enable the console echo.
 */
static void platform_win_echoOn(void)
{
    /* Not supported */
    return;
}

/**
 * This function is not defined as part of the ANSI C/C++ standard.
 * It is generally used by Borland's family of compilers. It returns a positive
 * non-zero integer if a key is in the keyboard buffer. It will not wait for a
 * key to be pressed.
 *
 * @return Status
 * @retval 0    No key pressed
 * @retval 1    Key pressed
 * @retval -1   Error
 */
static int platform_win_kbhit(void)
{
    return kbhit();
}

/**
 * This function is not defined as part of the ANSI C/C++ standard.
 * It is generally used by Borland's family of compilers. It waits until a key is
 * pressed and returns its key value.
 *
 * @return Key value
 * @retval 0    Aborted
 * @retval < 0  Key value
 * @retval -1   Error
 */
static int platform_win_getch(void)
{
    return getch();
}

/**
 * This function suspends the execution of the current thread until the time-out
 * interval elapses.
 *
 * @param[in] valueMS   Timeout value in ms
 */
static void platform_win_sleepMS(long valueMS)
{
    Sleep(valueMS);
    return;
}

/**
 * Set text foreground color.
 *
 * @param[in] color Color
 */
static void platform_win_setTextColor(PLATFORM_COLOR color)
{
    if (PLATFORM_COLOR_WHITE >= color)
    {
        HANDLE  hStdout     = GetStdHandle(STD_OUTPUT_HANDLE);
        uint8_t foreground  = platform_win_colors[color];
        uint8_t background  = platform_win_getTextColor() & 0xf0;

        if (hStdout == INVALID_HANDLE_VALUE)
        {
            return;
        }

        (void)SetConsoleTextAttribute(hStdout, foreground | background);
    }

    return;
}

/**
 * Set text background color.
 *
 * @param[in] color Color
 */
static void platform_win_setTextBgColor(PLATFORM_COLOR color)
{
    if (PLATFORM_COLOR_WHITE >= color)
    {
        HANDLE  hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        uint8_t foreground  = platform_win_getTextColor() & 0x0f;
        uint8_t background  = platform_win_bgColors[color];

        if (hStdout == INVALID_HANDLE_VALUE)
        {
            return;
        }

        (void)SetConsoleTextAttribute(hStdout, foreground | background);
    }

    return;
}

/**
 * Get the current console text and background color.
 *
 * @return Color
 */
static uint8_t  platform_win_getTextColor(void)
{
    uint8_t                     color   = 0;
    HANDLE                      hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO  consoleScreenBufferInfo;

    if (hStdout == INVALID_HANDLE_VALUE)
    {
        return 0;
    }

    if (0 != GetConsoleScreenBufferInfo(hStdout, &consoleScreenBufferInfo))
    {
        color = (uint8_t)(consoleScreenBufferInfo.wAttributes & 0xff);
    }

    return color;
}

#endif  /* _WIN32 */

#ifdef __linux__

/**
 * Disable the console echo.
 */
static void platform_linux_echoOff(void)
{
    struct termios  terminal;

    terminal = platform_terminalOld;
    terminal.c_lflag &= ~(ECHO | ICANON);   /* Disable echo and disable canonical mode */
    terminal.c_cc[VMIN]     = 1;            /* Always 1 byte */
    terminal.c_cc[VTIME]    = 0;            /* No timer */

    (void)tcsetattr(STDIN_FILENO, TCSANOW, &terminal);
    return;
}

/**
 * Enable the console echo.
 */
static void platform_linux_echoOn(void)
{
    (void)tcsetattr(STDIN_FILENO, TCSANOW, &platform_terminalOld);
    return;
}

/**
 * This function is not defined as part of the ANSI C/C++ standard.
 * It is generally used by Borland's family of compilers. It returns a positive
 * non-zero integer if a key is in the keyboard buffer. It will not wait for a
 * key to be pressed.
 *
 * @return Status
 * @retval 0    No key pressed
 * @retval 1    Key pressed
 * @retval -1   Error
 */
static int platform_linux_kbhit(void)
{
    int             status = 0;
    fd_set          fdSet;
    struct timeval  timeout;

    /* Watch STDIN to see when it has input */
    FD_ZERO(&fdSet);
    FD_SET(STDIN_FILENO, &fdSet);

    /* Wait up to 100us */
    timeout.tv_sec  = 0;
    timeout.tv_usec = 100;

    status = select(1, &fdSet, NULL, NULL, &timeout);

    return status;
}

/**
 * This function is not defined as part of the ANSI C/C++ standard.
 * It is generally used by Borland's family of compilers. It waits until a key is
 * pressed and returns its key value.
 *
 * @return Key value
 * @retval 0    Aborted
 * @retval < 0  Key value
 * @retval -1   Error
 */
static int platform_linux_getch(void)
{
    int keyValue    = 0;

    if (1 == read(STDIN_FILENO, &keyValue, 1))
    {
        keyValue &= 0xff;
    }

    return keyValue;
}

/**
 * This function suspends the execution of the current thread until the time-out
 * interval elapses.
 *
 * @param[in] valueMS   Timeout value in ms
 */
static void platform_linux_sleepMS(long valueMS)
{
    struct timeval  timeout;

    timeout.tv_sec  = valueMS / 1000;
    timeout.tv_usec = (valueMS % 1000) * 1000;

    (void)select(0, NULL, NULL, NULL, &timeout);

    return;
}

/**
 * Set text foreground color.
 *
 * @param[in] color Color
 */
static void platform_linux_setTextColor(PLATFORM_COLOR color)
{
    if (PLATFORM_COLOR_WHITE >= color)
    {
        printf("\033[%um", platform_linux_colors[color]);
    }

    return;
}

/**
 * Set text background color.
 *
 * @param[in] color Color
 */
static void platform_linux_setTextBgColor(PLATFORM_COLOR color)
{
    if (PLATFORM_COLOR_WHITE >= color)
    {
        printf("\033[%um", platform_linux_bgColors[color]);
    }

    return;
}

#endif  /* __linux__ */
