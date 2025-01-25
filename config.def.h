/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 16;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int gappih    = 4;        /* horiz inner gap between windows */
static const unsigned int gappiv    = 4;        /* vert inner gap between windows */
static const unsigned int gappoh    = 4;        /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 4;        /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Hack:size=12" };
static const char dmenufont[]       = "Hack:size=12";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_red[]         = "#dd2f3a";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_red  },
	[SchemeTitle]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class        instance     title                 tags mask     iscentered	isfloating  isterminal  noswallow  monitor  float x,y,w,h	    floatborderpx	scratch key */
	{ "Gimp",       NULL,        NULL,                 0,            1,		    1,          0,           0,        -1,      50,50,1700,1000,    2,		         0  },
	{ "Firefox",    NULL,        NULL,                 1 << 1,       0,		    0,          0,          -1,        -1,      50,50,500,500,	    2,		         0  },
	{ "st",         NULL,        NULL,                 0,            1,		    0,          1,           0,        -1,      50,50,500,500,	    2,		         0  },
	{ "Qalculate",  NULL,        NULL,                 0,            1,		    1,          0,           0,        -1,      50,50,720,500,	    2,		         0  },
	{ NULL,         NULL,        "webcam",             0,            1,		    1,          1,           0,        -1,      50,50,640,480,	    2,		         0  },
	{ NULL,         NULL,        "nmtui",              0,            1,		    1,          1,           0,        -1,      50,50,450,650,	    2,		         0  },
	{ NULL,         NULL,        "ScratchA",           0,            1,		    1,          1,           0,        -1,      50,50,900,800,	    2,		        'a' },
	{ NULL,         NULL,        "ScratchS",           0,            1,		    1,          1,           0,        -1,      50,50,900,800,	    2,		        's' },
	{ NULL,         NULL,        "ScratchD",           0,            1,		    1,          1,           0,        -1,      360,10,1200,650,	2,		        'd' },
	{ NULL,         NULL,        "ScratchF",           0,            1,		    1,          1,           0,        -1,      50,50,900,800,	    2,		        'f' },
	{ NULL,         NULL,        "ScratchV",           0,            1,		    1,          1,           0,        -1,      50,50,600,500,	    2,		        'v' },

	{ NULL,         NULL,        "Event Tester",       0,            0,		    0,          0,           1,        -1,      50,50,500,500,	    2,		         0  }, /* xev */
	{ NULL,         NULL,        "Unlock Keyring",     0,            1,		    0,          0,           1,        -1,      50,50,500,500,	    2,		         0  },
};

/* window following */
#define WFACTIVE '>'
#define WFINACTIVE 'v'
#define WFDEFAULT WFINACTIVE

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },			// first entry is default
	{ "[\\]",     dwindle },
	{ "TTT",      bstack },
    { "###",      nrowgrid },
    { "[M]",      monocle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
    { "===",      bstackhoriz },
    { "><>",      NULL },			// no layout function means floating behavior
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG)                                                                                               \
       &((Keychord){1, {{MODKEY, KEY}},                                        view,           {.ui = 1 << TAG} }), \
       &((Keychord){1, {{MODKEY|ControlMask, KEY}},                            toggleview,     {.ui = 1 << TAG} }), \
       &((Keychord){1, {{MODKEY|ShiftMask, KEY}},                              tag,            {.ui = 1 << TAG} }), \
       &((Keychord){1, {{MODKEY|ControlMask|ShiftMask, KEY}},                  toggletag,      {.ui = 1 << TAG} }),

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
//static const char *termcmd[]  = { "st", NULL };
static const char *termcmd[]  = { "kitty", NULL };

/*First arg only serves to match against key in rules*/
//static const char *scratchcmdA[] = {"a", "st", "-t", "ScratchA", NULL};
static const char *scratchcmdA[] = {"a", "kitty", "--title", "ScratchA", NULL};
//static const char *scratchcmdS[] = {"s", "st", "-t", "ScratchS", NULL};
static const char *scratchcmdS[] = {"s", "kitty", "--title", "ScratchS", NULL};
//static const char *scratchcmdD[] = {"d", "st", "-t", "ScratchD", NULL};
static const char *scratchcmdD[] = {"d", "kitty", "--title", "ScratchD", NULL};
//static const char *scratchcmdF[] = {"f", "st", "-t", "ScratchF", NULL};
static const char *scratchcmdF[] = {"f", "kitty", "--title", "ScratchF", NULL};
//static const char *scratchcmdV[] = {"v", "st", "-t", "ScratchV", NULL};
static const char *scratchcmdV[] = {"v", "kitty", "--title", "ScratchV", NULL};

// Application launcher commands
static const char *qalculate[]    = {"qalculate-gtk", NULL};
static const char *fileexplorer[] = {"nemo", NULL};
static const char *vmselector[]   = {"vmselector", NULL};
static const char *gimp[]         = {"gimp", NULL};
static const char *browser[]      = {"brave", NULL};
static const char *joplin[]       = {"joplin-desktop", NULL};
// Media control
static const char *voldown[]      = {"volume", "down", NULL};
static const char *volup[]        = {"volume", "up", NULL};
static const char *volmute[]      = {"volume", "mute", NULL};
static const char *volset[]       = {"volume", "setvolume", NULL};
// Brightness control
static const char *brightdown[]   = {"brightness", "down", NULL};
static const char *brightup[]     = {"brightness", "up", NULL};
// Screenshot
static const char *scrshotsel[]   = {"scrshot", "selection", NULL};
static const char *scrshotwin[]   = {"scrshot", "window", NULL};
static const char *scrshotscr[]   = {"scrshot", "fullscreen", NULL};
// Display
static const char *displaysel[]   = {"displayselect", NULL};
static const char *displayset[]   = {"monitorsetup", NULL};
// Power menu
static const char *powermenu[]    = {"pow", NULL};


#include "movestack.c"
static Keychord *keychords[] = {
       /* Keys                                                              function            argument */
    &((Keychord){1, {{MODKEY, XK_p}},                                       spawn,              {.v = dmenucmd } }),
    &((Keychord){1, {{MODKEY, XK_Return}},                                  spawn,              {.v = termcmd } }),
    &((Keychord){1, {{MODKEY|ShiftMask,XK_a}},                              togglescratch,      {.v = scratchcmdA } }),
    &((Keychord){1, {{MODKEY|ShiftMask,XK_s}},                              togglescratch,      {.v = scratchcmdS } }),
    &((Keychord){1, {{MODKEY|ShiftMask,XK_d}},                              togglescratch,      {.v = scratchcmdD } }),
    &((Keychord){1, {{MODKEY|ShiftMask,XK_f}},                              togglescratch,      {.v = scratchcmdF } }),
    &((Keychord){1, {{MODKEY|ShiftMask,XK_v}},                              togglescratch,      {.v = scratchcmdV } }),
    &((Keychord){1, {{MODKEY|ShiftMask,XK_u}},                              scratchpad_show,    {0} }),
    &((Keychord){1, {{MODKEY|ControlMask,XK_u}},                            scratchpad_hide,    {0} }),
    &((Keychord){1, {{MODKEY|Mod1Mask,XK_u}},                               scratchpad_remove,  {0} }),
    &((Keychord){1, {{MODKEY, XK_b}},                                       togglebar,          {0} }),
	&((Keychord){1, {{MODKEY,XK_n}},                                        togglefollow,       {0} }),
    &((Keychord){1, {{MODKEY, XK_j}},                                       focusstack,         {.i = +1 } }),
    &((Keychord){1, {{MODKEY, XK_k}},                                       focusstack,         {.i = -1 } }),
    &((Keychord){1, {{MODKEY, XK_i}},                                       incnmaster,         {.i = +1 } }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_i}},                             incnmaster,         {.i = -1 } }),
    &((Keychord){1, {{MODKEY, XK_h}},                                       setmfact,           {.f = -0.05} }),
    &((Keychord){1, {{MODKEY, XK_l}},                                       setmfact,           {.f = +0.05} }),
    &((Keychord){1, {{MODKEY|ShiftMask,XK_h}},                              setcfact,           {.f = +0.25} }),
	&((Keychord){1, {{MODKEY|ShiftMask,XK_l}},                              setcfact,           {.f = -0.25} }),
	&((Keychord){1, {{MODKEY|ShiftMask,XK_o}},                              setcfact,           {.f =  0.00} }),
    &((Keychord){1, {{MODKEY|ShiftMask,XK_j}},                              movestack,          {.i = +1 } }),
    &((Keychord){1, {{MODKEY|ShiftMask,XK_k}},                              movestack,          {.i = -1 } }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_Return}},                        zoom,               {0} }),
    &((Keychord){1, {{MODKEY|Mod1Mask,XK_plus}},                            incrgaps,           {.i = +4 } }),
    &((Keychord){1, {{MODKEY|Mod1Mask,XK_minus}},                           incrgaps,           {.i = -4 } }),
    &((Keychord){1, {{MODKEY|Mod1Mask|ShiftMask,XK_0}},                     defaultgaps,        {0} }),
    &((Keychord){1, {{MODKEY|Mod1Mask,XK_0}},                               togglegaps,         {0} }),
    &((Keychord){1, {{MODKEY, XK_Tab}},                                     view,               {0} }),
    &((Keychord){1, {{MODKEY, XK_q}},                                       killclient,         {0} }),
    &((Keychord){1, {{MODKEY,XK_t}},                                        setlayout,          {.v = &layouts[0]} }), // set layout 0 []= tile
    &((Keychord){1, {{MODKEY|ShiftMask,XK_t}},                              setlayout,          {.v = &layouts[1]} }), // set layout 1 [\\] dwindle
    &((Keychord){1, {{MODKEY|ControlMask,XK_t}},                            setlayout,          {.v = &layouts[2]} }), // set layout 2 TTT bstack
    &((Keychord){1, {{MODKEY|ShiftMask|ControlMask,XK_t}},                  setlayout,          {.v = &layouts[3]} }), // set layout 3 ### nrowgrid
	&((Keychord){1, {{MODKEY,XK_m}},                                        setlayout,          {.v = &layouts[4]} }), // set layout 4 [M] monocle
	&((Keychord){1, {{MODKEY|ShiftMask,XK_m}},                              setlayout,          {.v = &layouts[5]} }), // set layout 5 |M| centeredmaster
	&((Keychord){1, {{MODKEY|ControlMask,XK_m}},                            setlayout,          {.v = &layouts[6]} }), // set layout 6 >M> centeredfloatingmaster
	&((Keychord){1, {{MODKEY|ShiftMask|ControlMask,XK_m}},                  setlayout,          {.v = &layouts[7]} }), // set layout 7 === bstackhoriz
    &((Keychord){1, {{MODKEY,XK_f}},                                        setlayout,          {.v = &layouts[8]} }), // set layout 8 ><> floating
	&((Keychord){1, {{MODKEY,XK_space}},                                    setlayout,          {0} }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_space}},                         togglefloating,     {0} }),
    &((Keychord){1, {{MODKEY, XK_0}},                                       view,               {.ui = ~0 } }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_0}},                             tag,                {.ui = ~0 } }),
    &((Keychord){1, {{MODKEY, XK_comma}},                                   focusmon,           {.i = -1 } }),
    &((Keychord){1, {{MODKEY, XK_period}},                                  focusmon,           {.i = +1 } }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_comma}},                         tagmon,             {.i = -1 } }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_period}},                        tagmon,             {.i = +1 } }),
    &((Keychord){1, {{MODKEY|ControlMask|ShiftMask, XK_q}},                 quit,               {0} }),
	// Application launcher keys
    &((Keychord){2, {{MODKEY, XK_o}, {0, XK_c}},                            spawn,              {.v = qalculate } }),
	&((Keychord){2, {{MODKEY, XK_o}, {0, XK_f}},                            spawn,              {.v = fileexplorer } }),
	&((Keychord){2, {{MODKEY, XK_o}, {0, XK_v}},                            spawn,              {.v = vmselector } }),
	&((Keychord){2, {{MODKEY, XK_o}, {0, XK_g}},                            spawn,              {.v = gimp } }),
	&((Keychord){2, {{MODKEY, XK_o}, {0, XK_b}},                            spawn,              {.v = browser } }),
	&((Keychord){2, {{MODKEY, XK_o}, {0, XK_j}},                            spawn,              {.v = joplin } }),
	// Audio controls
	&((Keychord){1, {{0, XF86XK_AudioLowerVolume}},			                spawn,              {.v = voldown } }),
	&((Keychord){1, {{0, XF86XK_AudioRaiseVolume}},			                spawn,              {.v = volup } }),
	&((Keychord){1, {{0, XF86XK_AudioMute}},			                    spawn,              {.v = volmute } }),
	&((Keychord){1, {{MODKEY, XF86XK_AudioMute}},			                spawn,              {.v = volset } }),
	// Screen brightness
	&((Keychord){1, {{0, XF86XK_MonBrightnessDown}},		                spawn,              {.v = brightdown } }),
	&((Keychord){1, {{0, XF86XK_MonBrightnessUp}},			                spawn,              {.v = brightup } }),
	// Screenshot
	&((Keychord){1, {{0, 0xff61}},			                                spawn,              {.v = scrshotsel } }),
	&((Keychord){1, {{ShiftMask, 0xff61}},		                            spawn,              {.v = scrshotwin } }),
	&((Keychord){1, {{ControlMask, 0xff61}},	                            spawn,              {.v = scrshotscr } }),
	// Display setup
	&((Keychord){1, {{MODKEY|Mod1Mask, XK_p}},	                            spawn,              {.v = displaysel } }),
	&((Keychord){1, {{MODKEY|Mod1Mask|ControlMask, XK_p}},	                spawn,              {.v = displayset } }),
	// Power menu
	&((Keychord){1, {{MODKEY|Mod1Mask, XK_BackSpace}},	                    spawn,              {.v = powermenu } }),
   TAGKEYS(                        XK_1,                      0)
   TAGKEYS(                        XK_2,                      1)
   TAGKEYS(                        XK_3,                      2)
   TAGKEYS(                        XK_4,                      3)
   TAGKEYS(                        XK_5,                      4)
   TAGKEYS(                        XK_6,                      5)
   TAGKEYS(                        XK_7,                      6)
   TAGKEYS(                        XK_8,                      7)
   TAGKEYS(                        XK_9,                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkFollowSymbol,      0,              Button1,        togglefollow,   {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

