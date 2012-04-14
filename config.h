/* See LICENSE file for copyright and license details. */
#include "im-grid.c"
#include "push.c"
#include "X11/XF86keysym.h"
#include "togglefullscreen.c"

/* appearance */
static const char font[]            = "-*-termsyn.icons-medium-*-*-*-11-*-*-*-*-*-*-*";

#define NUMCOLORS 9
static const char colors[NUMCOLORS][ColLast][9] = {
    // border     foreground background
    { "#1A1A1A", "#808080", "#020202" },  // gray
    { "#4C4C4C", "#B3B3B3", "#020202" },  // white
    { "#B3354C", "#B3354C", "#020202" },  // light red
    { "#1C678C", "#1C678C", "#020202" },  // blue
    { "#684D80", "#684D80", "#020202" },  // magenta
    { "#877C43", "#877C43", "#020202" },  // yellow
    { "#608040", "#608040", "#020202" },  // green
    { "#337373", "#337373", "#020202" },  // cyan
    { "#802635", "#802635", "#020202" },  // red
};

static const unsigned int borderpx       = 1;                // border pixel of windows
static const unsigned int snap           = 5;                // snap pixel
static const unsigned int systrayspacing = 1;                // space between systray icons
static const Bool showsystray            = True;             // False means no systray
static const Bool showbar                = True;             // False means no bar
static const Bool topbar                 = True;             // False means bottom bar
static Bool useicons                     = True;             // False means use ascii symbols
static const char scratchpadname[]       = "Scratchpad";

/* layout(s) */
static const float mfact      = 0.50;     // factor of master area size [0.05..0.95]
static const Bool resizehints = False;    // True means respect size hints in tiled resizals
static const int nmaster      = 1;        // default number of clients in the master area

static const Layout layouts[] = {
    // icon                                    symbol    arrange function
    { "/home/ok/build/dwm/icons/tile.xbm",     "þ",      tile },
    { "/home/ok/build/dwm/icons/bstack.xbm",   "ü",      bstack },
    { "/home/ok/build/dwm/icons/float.xbm",    "ý",      NULL },     // no layout function means floating behavior
    { "/home/ok/build/dwm/icons/monocle.xbm",  "ÿ",      monocle },
    { "/home/ok/build/dwm/icons/grid.xbm",     "ú",      imgrid },
};

/* tagging */
static const Tag tags[] = {
        // name       layout           mfact    nmaster
        { "¹",        &layouts[3],     -1,      -1 },
        { "Ú",        &layouts[3],     -1,      -1 },
        { "´",        &layouts[3],     -1,      -1 },
        { "²",        &layouts[1],     -1,      -1 },
        { "©",        &layouts[4],   0.22,      -1 },
        { "ê",        &layouts[0],   0.65,      -1 },
        { "º",        &layouts[2],     -1,      -1 },
};

/* window rules */
static const Rule rules[] = {
        // class                  instance  title                  tags mask  isfloating  iscentred   monitor
        {  NULL,                  NULL,     "tmux",                1 << 0,    False,                  -1 },
        { "Firefox",              NULL,      NULL,                 1 << 1,    False,                  -1 },
        { "luakit",               NULL,      NULL,                 1 << 1,    False,                  -1 },
        { "OperaNext",            NULL,      NULL,                 1 << 1,    False,                  -1 },
        { "Opera",                NULL,      NULL,                 1 << 1,    False,                  -1 },
        { "Cr3",                  NULL,      NULL,                 1 << 2,    False,                  -1 },
        { "Lazarus",              NULL,      NULL,                 1 << 3,    True,       False,      -1 },
        { "Skype",                NULL,      NULL,                 1 << 4,    False,                  -1 },
        {  NULL,                  NULL,     "Weechat",             1 << 4,    False,                  -1 },
        { "Gimp",                 NULL,      NULL,                 1 << 6,    True,                   -1 },
        { "Xsane",                NULL,      NULL,                 1 << 6,    True,       False,      -1 },
        { "Gsimplecal",           NULL,      NULL,                 0,         True,       False,      -1 },
        { "Lxappearance",         NULL,      NULL,                 0,         True,                   -1 },
        { "mplayer2",             NULL,      NULL,                 0,         True,                   -1 },
        { "Nitrogen",             NULL,      NULL,                 0,         True,                   -1 },
        { "Qalculate-gtk",        NULL,      NULL,                 0,         True,                   -1 },
        { "Qalculate",            NULL,      NULL,                 0,         True,                   -1 },
        { "Qpass",                NULL,      NULL,                 0,         True,                   -1 },
        { "URxvt",                NULL,      NULL,                 0,         False,                  -1 },
        { "Stardict",             NULL,      NULL,                 0,         True,                   -1 },
        { "Zenity",               NULL,      NULL,                 0,         True,                   -1 },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *addresscmd[]    = { "urxvtc", "-title", "abook", "-e", "/home/ok/bin/abook-autoexport", NULL };
static const char *browsercmd[]    = { "/home/ok/bin/browser", NULL };
static const char *calendarcmd[]   = { "gsimplecal", NULL };
static const char *cpucmd[]        = { "/home/ok/bin/cpu_control.sh", NULL };
static const char *dictcmd[]       = { "stardict", NULL };
static const char *dmenucmd[]      = { "dmenu_run", "-i", "-fn", font, "-nb", colors[0][ColBG], "-nf", colors[0][ColFG],
                                       "-sb", colors[1][ColBG], "-sf", colors[1][ColFG], NULL };
static const char *fancmd[]        = { "/home/ok/bin/fan_control.sh", NULL };
static const char *imcmd[]         = { "pidgin", NULL };
static const char *irccmd[]        = {"urxvtc", "-title", "Weechat", "-e", "weechat-curses", NULL };
static const char *logoutcmd[]     = { "sudo", "killall", "X", NULL };
static const char *menucmd[]       = { "/home/ok/bin/mygtkmenu.py", NULL };
static const char *monitorcmd[]    = { "/home/ok/bin/monitor-dwm.sh", NULL };
static const char *passcmd[]       = { "qpass", NULL };
static const char *reloadcmd[]     = { "/home/ok/bin/dwm-reload.sh", NULL };
static const char *scratchpadcmd[] = { "urxvtc", "-title", scratchpadname, "-geometry", "70x9+400+10", NULL };
static const char *screenoffcmd[]  = { "xset", "dpms", "force", "off", NULL };
static const char *shutdowncmd[]   = { "/home/ok/bin/dmenu-powerbutton", NULL };
static const char *termcmd[]       = { "urxvtc", NULL };
static const char *tmuxcmd[]       = { "urxvtc", "-title", "tmux", "-e", "/home/ok/bin/tmux.sh", NULL };
static const char *voldowncmd[]    = { "amixer", "-q", "set", "Master", "2dB-",  NULL };
static const char *voltogglecmd[]  = { "amixer", "-q", "set", "Master", "toggle",  NULL };
static const char *volupcmd[]      = { "amixer", "-q", "set", "Master", "2dB+",  NULL };
static const char *wificmd[]       = { "urxvtc", "-e", "sudo", "wifi-select", "wlan0", NULL };

static const char *playcmd[]       = { "player_control", "pause", NULL };
static const char *prevcmd[]       = { "player_control", "prev", NULL };
static const char *nextcmd[]       = { "player_control", "next", NULL };

static Key keys[] = {
   // modifier                       key                       function          argument
    { Mod4Mask,                      XK_a,                     spawn,            {.v = addresscmd } },
    { Mod4Mask,                      XK_b,                     spawn,            {.v = browsercmd } },
    { Mod4Mask,                      XK_d,                     spawn,            {.v = dictcmd } },
    { 0,                             XK_Menu,                  spawn,            {.v = dmenucmd } },
    { Mod4Mask,                      XK_p,                     spawn,            {.v = imcmd } },
    { Mod4Mask,                      XK_i,                     spawn,            {.v = irccmd } },
    { MODKEY|ShiftMask,              XK_e,                     spawn,            {.v = logoutcmd } },
    { Mod4Mask,                      XK_space,                 spawn,            {.v = menucmd } },
    { 0,                             XF86XK_Display,           spawn,            {.v = monitorcmd } },
    { Mod4Mask,                      XK_h,                     spawn,            {.v = passcmd} },
    { MODKEY|ShiftMask,              XK_r,                     spawn,            {.v = reloadcmd} },
    { 0,                             XK_F12,                   togglescratch,    {.v = scratchpadcmd} },
    { 0,                             XF86XK_Launch1,           spawn,            {.v = screenoffcmd } },
    { 0,                             XF86XK_PowerOff,          spawn,            {.v = shutdowncmd } },
    { MODKEY,                        XK_Return,                spawn,            {.v = termcmd } },
    { Mod4Mask,                      XK_t,                     spawn,            {.v = tmuxcmd } },
    { 0,                             XF86XK_AudioLowerVolume,  spawn,            {.v = voldowncmd } },
    { 0,                             XF86XK_AudioMute,         spawn,            {.v = voltogglecmd } },
    { 0,                             XF86XK_AudioRaiseVolume,  spawn,            {.v = volupcmd } },
    { Mod4Mask,                      XK_w,                     spawn,            {.v = wificmd } },

    { Mod4Mask,                      XK_Down,                  spawn,            {.v = playcmd } },
    { Mod4Mask,                      XK_Left,                  spawn,            {.v = prevcmd } },
    { Mod4Mask,                      XK_Right,                 spawn,            {.v = nextcmd } },

    { MODKEY,                        XK_Down,                  focusstack,       {.i = +1 } },
    { MODKEY,                        XK_Up,                    focusstack,       {.i = -1 } },
    { MODKEY,                        XK_Left,                  setmfact,         {.f = -0.01} },
    { MODKEY,                        XK_Right,                 setmfact,         {.f = +0.01} },
    { MODKEY|ShiftMask,              XK_Return,                zoom,             {0} },
    { Mod1Mask,                      XK_Tab,                   view,             {0} },
    { Mod1Mask,                      XK_F4,                    killclient,       {0} },
    { MODKEY|ShiftMask,              XK_Left,                  incnmaster,       {.i = +1 } },
    { MODKEY|ShiftMask,              XK_Right,                 incnmaster,       {.i = -1 } },
    { MODKEY|ShiftMask,              XK_Down,                  pushdown,         {0} },
    { MODKEY|ShiftMask,              XK_Up,                    pushup,           {0} },
    { MODKEY,                        XK_t,                     setlayout,        {.v = &layouts[0]} },
    { MODKEY,                        XK_b,                     setlayout,        {.v = &layouts[1]} },
    { MODKEY,                        XK_f,                     setlayout,        {.v = &layouts[2]} },
    { MODKEY,                        XK_m,                     setlayout,        {.v = &layouts[3]} },
    { MODKEY,                        XK_g,                     setlayout,        {.v = &layouts[4]} },
    { MODKEY,                        XK_space,                 setlayout,        {0} },
    { MODKEY|ShiftMask,              XK_space,                 togglefloating,   {0} },
    { MODKEY,                        XK_eacute,                view,             {.ui = ~0 } },
    { MODKEY|ShiftMask,              XK_eacute,                tag,              {.ui = ~0 } },
    { MODKEY|ShiftMask,              XK_f,                     togglefullscreen, {0} },
    TAGKEYS(                         XK_plus,                                    0)
    TAGKEYS(                         XK_ecaron,                                  1)
    TAGKEYS(                         XK_scaron,                                  2)
    TAGKEYS(                         XK_ccaron,                                  3)
    TAGKEYS(                         XK_rcaron,                                  4)
    TAGKEYS(                         XK_zcaron,                                  5)
    TAGKEYS(                         XK_yacute,                                  6)
//    TAGKEYS(                         XK_aacute,                                  7)
//    TAGKEYS(                         XK_iacute,                                  8)
};

#include "tilemovemouse.c"
/* button definitions */
// click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin
static Button buttons[] = {
    // click                event mask      button          function        argument
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    { ClkWinTitle,          0,              Button1,        focusstack,     {.i = +1 } },
    { ClkWinTitle,          0,              Button3,        focusstack,     {.i = -1 } },
    { ClkClientWin,         MODKEY,         Button1,        tilemovemouse,  {0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
    { ClkRootWin,           0,              Button3,        spawn,          {.v = menucmd } },
    { ClkStatusText,        0,              Button1,        spawn,          {.v = calendarcmd } },
    { ClkStatusText,        0,              Button3,        spawn,          {.v = cpucmd } },
    { ClkStatusText,        MODKEY,         Button3,        spawn,          {.v = fancmd } },
};

