/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
#define NUMCOLORS 3
static const char colors[NUMCOLORS][ColLast][8] = {
// border     foreground background
{ "#212121", "#696969", "#121212" }, // 0 = normal
{ "#696969", "#E0E0E0", "#121212" }, // 1 = selected
{ "#212121", "#BF4D80", "#121212" }, // 2 = urgent
};

static const char font[]            = "-*-terminus-*-*-*-*-12-*-*-*-*-*-*-*";
static const unsigned int borderpx  = 1;                /* border pixel of windows */
static const unsigned int snap      = 10;               /* snap pixel */
static const Bool showbar           = True;             /* False means no bar */
static const Bool topbar            = True;             /* False means bottom bar */
static const char scratchpadname[]  = "Scratchpad";
static Bool useicons                = True;             /* False means use ascii symbols */

/* layout(s) */
static const float mfact      = 0.50;     /* factor of master area size [0.05..0.95] */
static const Bool resizehints = False;    /* True means respect size hints in tiled resizals */
static const int nmaster      = 1;        /* default number of clients in the master area */

#include "pidgin-grid.c"
#include "nbstack.c"
static const Layout layouts[] = {
	/* icon                                     symbol     arrange function */
	{ "/home/ok/Build/dwm/icons/tile.xbm",     "[]=",      ntile },
    { "/home/ok/Build/dwm/icons/bstack.xbm",   "TTT",      nbstack },
	{ "/home/ok/Build/dwm/icons/float.xbm",    "><>",      NULL },     /* no layout function means floating behavior */
	{ "/home/ok/Build/dwm/icons/monocle.xbm",  "[ ]",      monocle },
    { "/home/ok/Build/dwm/icons/grid.xbm",     "###",      pidgin },
};

/* tagging */
static const Tag tags[] = {
        /* name       layout           mfact    nmaster */
        { "trm",      &layouts[3],     -1,      -1 },
        { "web",      &layouts[3],     -1,      -1 },
        { "doc",      &layouts[3],     -1,      -1 },
        { "irc",      &layouts[4],     0.18,    -1 },
        { "foo",      &layouts[2],     -1,      -1 },
};

static const Rule rules[] = {
    	/* class                  instance  title                                   tags mask  isfloating  monitor */
		{  NULL,                  NULL,     "tmux",                                 1 << 0,    False,      -1 },
        { "OperaNext",            NULL,      NULL,                                  1 << 1,    False,      -1 },
        { "Opera",                NULL,      NULL,                                  1 << 1,    False,      -1 },
        { "Comix",                NULL,      NULL,                                  1 << 2,    False,      -1 },
        { "FBReader",             NULL,      NULL,                                  1 << 2,    False,      -1 },
        { "Kchmviewer",           NULL,      NULL,                                  1 << 2,    False,      -1 },
		{  NULL,                  NULL,     "irssi",                                1 << 3,    False,      -1 },
        { "Pidgin",               NULL,      NULL,                                  1 << 3,    False,      -1 },
        { "Skype",                NULL,      NULL,                                  1 << 3,    True,       -1 },
	    { "Gimp",                 NULL,      NULL,                                  1 << 4,    True,       -1 },
        { "Xsane",                NULL,      NULL,                                  1 << 4,    True,       -1 },
        { "WorldOfGoo.bin32",     NULL,      NULL,                                  1 << 4,    True,       -1 },
		{ "Audacious",            NULL,      NULL,                                  0,         True,       -1 },
        { "Gnome-mplayer",        NULL,      NULL,                                  0,         True,       -1 },
        { "Gtk-recordMyDesktop",  NULL,      NULL,                                  0,         True,       -1 },
        { "Keepassx",             NULL,      NULL,                                  0,         True,       -1 },
        { "Lxappearance",         NULL,      NULL,                                  0,         True,       -1 },
        { "MPlayer",              NULL,      NULL,                                  0,         True,       -1 },
        { "Nitrogen",             NULL,      NULL,                                  0,         True,       -1 },
        { "Qalculate-gtk",        NULL,      NULL,                                  0,         True,       -1 },
		{ "Qalculate",            NULL,      NULL,                                  0,         True,       -1 },
        { "Thunar",               NULL,     "Bulk Rename - Rename Multiple Files",  0,         True,       -1 },
        { "Zenity",               NULL,      NULL,                                  0,         True,       -1 },
        {  NULL,                  NULL,     "shutdown-dialog.py",                   0,         True,       -1 },
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
static const char *operacmd[]      = { "/home/ok/Scripts/opera.sh", NULL };
static const char *dmenucmd[]      = { "dmenu-launch", NULL };
static const char *irssicmd[]      = { "urxvtc", "-title", "irssi", "-e", "irssi", NULL };
static const char *logoutcmd[]     = { "sudo", "killall", "X", NULL };
static const char *menucmd[]       = { "/home/ok/Scripts/mygtkmenu.py", NULL };
static const char *monitorcmd[]    = { "/home/ok/Scripts/monitor-dwm.sh", NULL };
static const char *pidgincmd[]     = { "pidgin", NULL };
static const char *reloadcmd[]     = { "/home/ok/Scripts/dwm-reload.sh", NULL };
static const char *runcmd[]        = { "gmrun", NULL };
static const char *scratchpadcmd[] = { "urxvtc", "-title", scratchpadname, "-geometry", "70x9+400+10", NULL };
static const char *screenoffcmd[]  = { "xset", "dpms", "force", "off", NULL };
static const char *shutdowncmd[]   = { "/home/ok/Scripts/shutdown-dialog.py", NULL };
static const char *stardictcmd[]   = { "stardict", NULL };
static const char *termcmd[]       = { "urxvtc", NULL };
static const char *tmuxcmd[]       = { "urxvtc", "-title", "tmux", "-e", "/home/ok/Scripts/tmux.sh", NULL };
static const char *voldowncmd[]    = { "amixer", "-q", "set", "Master", "2dB-",  NULL };
static const char *voltogglecmd[]  = { "amixer", "-q", "set", "Master", "toggle",  NULL };
static const char *volupcmd[]      = { "amixer", "-q", "set", "Master", "2dB+",  NULL };
static const char *wificmd[]       = { "urxvtc", "-e", "sudo", "wifi-select", "wlan0", NULL };

#include "push.c"
static Key keys[] = {
   /* modifier                       key                       function        argument */
    { Mod4Mask,                      XK_o,                     spawn,          {.v = operacmd } },
	{ 0,                             XK_Menu,                  spawn,          {.v = dmenucmd } },
	{ Mod4Mask,                      XK_i,                     spawn,          {.v = irssicmd } },
    { MODKEY|ShiftMask,              XK_e,                     spawn,          {.v = logoutcmd } },
	{ Mod4Mask,                      XK_space,                 spawn,          {.v = menucmd } },
    { 0,                             XF86XK_Display,           spawn,          {.v = monitorcmd } },
    { Mod4Mask,                      XK_p,                     spawn,          {.v = pidgincmd } },
	{ MODKEY|ShiftMask,              XK_r,                     spawn,          {.v = reloadcmd} },
	{ MODKEY,                        XK_F2,                    spawn,          {.v = runcmd} },
    { 0,                             XK_F12,                   togglescratch,  {.v = scratchpadcmd} },
    { 0,                             XF86XK_Launch1,           spawn,          {.v = screenoffcmd } },
    { 0,                             XF86XK_PowerOff,          spawn,          {.v = shutdowncmd } },
	{ Mod4Mask,                      XK_s,                     spawn,          {.v = stardictcmd } },
	{ MODKEY,                        XK_Return,                spawn,          {.v = termcmd } },
    { Mod4Mask,                      XK_t,                     spawn,          {.v = tmuxcmd } },
    { 0,                             XF86XK_AudioLowerVolume,  spawn,          {.v = voldowncmd } },
    { 0,                             XF86XK_AudioMute,         spawn,          {.v = voltogglecmd } },
    { 0,                             XF86XK_AudioRaiseVolume,  spawn,          {.v = volupcmd } },
    { Mod4Mask,                      XK_w,                     spawn,          {.v = wificmd } },

	{ MODKEY,                        XK_Down,                  focusstack,     {.i = +1 } },
	{ MODKEY,                        XK_Up,                    focusstack,     {.i = -1 } },
	{ MODKEY,                        XK_Left,                  setmfact,       {.f = -0.01} },
	{ MODKEY,                        XK_Right,                 setmfact,       {.f = +0.01} },
	{ MODKEY|ShiftMask,              XK_Return,                zoom,           {0} },
	{ Mod1Mask,                      XK_Tab,                   view,           {0} },
	{ Mod1Mask,                      XK_F4,                    killclient,     {0} },
    { MODKEY|ShiftMask,              XK_Left,                  incnmaster,     {.i = +1 } },
    { MODKEY|ShiftMask,              XK_Right,                 incnmaster,     {.i = -1 } },
    { MODKEY|ShiftMask,              XK_Down,                  pushdown,       {0} },
    { MODKEY|ShiftMask,              XK_Up,                    pushup,         {0} },
	{ MODKEY,                        XK_t,                     setlayout,      {.v = &layouts[0]} },
    { MODKEY,                        XK_b,                     setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                        XK_f,                     setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                        XK_m,                     setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                        XK_g,                     setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                        XK_space,                 setlayout,      {0} },
	{ MODKEY|ShiftMask,              XK_space,                 togglefloating, {0} },
	{ MODKEY,                        XK_eacute,                view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,              XK_eacute,                tag,            {.ui = ~0 } },
	TAGKEYS(                         XK_plus,                                  0)
	TAGKEYS(                         XK_ecaron,                                1)
	TAGKEYS(                         XK_scaron,                                2)
	TAGKEYS(                         XK_ccaron,                                3)
	TAGKEYS(                         XK_rcaron,                                4)
	TAGKEYS(                         XK_zcaron,                                5)
	TAGKEYS(                         XK_yacute,                                6)
	TAGKEYS(                         XK_aacute,                                7)
	TAGKEYS(                         XK_iacute,                                8)
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
#include "cycle_layout.c"
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkWinTitle,          0,              Button1,        focusstack,     {.i = +1 } },
	{ ClkWinTitle,          0,              Button3,        focusstack,     {.i = -1 } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkRootWin,	     	0,		        Button3,	    spawn,		    {.v = menucmd } },
};

