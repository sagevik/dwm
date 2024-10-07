/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 16;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int gappih    = 4;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 4;       /* vert inner gap between windows */
static const unsigned int gappoh    = 4;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 4;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=15" };
static const char dmenufont[]       = "monospace:size=15";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#3b4252";
static const char col_red[]         = "#dd2f3a";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_red  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class       instance     title                 tags mask     isfloating  isterminal  noswallow  monitor  scratch key */
	{ "Gimp",      NULL,        NULL,                 0,            1,          0,           0,        -1,        0  },
	{ "Firefox",   NULL,        NULL,                 1 << 8,       0,          0,          -1,        -1,        0  },
	{ "st",        NULL,        NULL,                 0,            0,          1,           0,        -1,        0  },
	{ "Alacritty", NULL,        NULL,                 0,            0,          1,           0,        -1,        0  },
	{ NULL,        NULL,        "ScratchA",     	  0,            1,          1,           0,        -1,       'a' },
	{ NULL,        NULL,        "ScratchB",    	  0,            1,          1,           0,        -1,       's' },
	{ NULL,        NULL,        "Event Tester",       0,            0,          0,           1,        -1,        0  }, /* xev */
};

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
	{ "><>",      NULL },			// no layout function means floating behavior
	{ "[M]",      monocle },
	{ "[\\]",     dwindle },
	{ "TTT",      bstack },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

/*First arg only serves to match against key in rules*/
static const char *scratchcmdA[] = {"a", "st", "-t", "ScratchA", NULL}; 
static const char *scratchcmdB[] = {"s", "st", "-t", "ScratchB", NULL}; 


#include "movestack.c"
static const Key keys[] = {
	/* modifier                     key        function           argument */
	{ MODKEY,                       XK_p,      spawn,             {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,             {.v = termcmd } },
	{ MODKEY,                       XK_a,      togglescratch,     {.v = scratchcmdA } },
	{ MODKEY,                       XK_s,      togglescratch,     {.v = scratchcmdB } },
	{ MODKEY|ShiftMask,             XK_u,      scratchpad_show,   {0} },
	{ MODKEY|ControlMask,           XK_u,      scratchpad_hide,   {0} },
	{ MODKEY|Mod1Mask,              XK_u,      scratchpad_remove, {0} },
	{ MODKEY,                       XK_b,      togglebar,         {0} },
	{ MODKEY,                       XK_j,      focusstack,        {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,        {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,        {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,        {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,          {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,          {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,          {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,          {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,      setcfact,          {.f =  0.00} },
	{ MODKEY|ShiftMask,             XK_j,      movestack,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_Return, zoom,              {0} },
	{ MODKEY|Mod1Mask,              XK_0,      togglegaps,        {0} },
	{ MODKEY,                       XK_Tab,    view,              {0} },
	{ MODKEY,                       XK_q,      killclient,        {0} },
	{ MODKEY,                       XK_t,      setlayout,         {.v = &layouts[0]} }, // set layout 0 []= tile
	{ MODKEY,                       XK_f,      setlayout,         {.v = &layouts[1]} }, // set layout 1 ><> floating
	{ MODKEY,                       XK_m,      setlayout,         {.v = &layouts[2]} }, // set layout 2 [M] monocle
	{ MODKEY|ShiftMask,             XK_t,      setlayout,         {.v = &layouts[3]} }, // set layout 3 [\\] dwindle
	{ MODKEY|ControlMask,           XK_t,      setlayout,         {.v = &layouts[4]} }, // set layout 4 TTT bstack
	{ MODKEY|ShiftMask,             XK_m,      setlayout,         {.v = &layouts[5]} }, // set layout 5 |M| centeredmaster
	{ MODKEY|ControlMask,           XK_m,      setlayout,         {.v = &layouts[6]} }, // set layout 6 >M> centeredfloatingmaster
	{ MODKEY,                       XK_space,  setlayout,         {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating,    {0} },
	{ MODKEY,                       XK_0,      view,              {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,               {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,          {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,          {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,            {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,            {.i = +1 } },
	TAGKEYS(                        XK_1,                         0)
	TAGKEYS(                        XK_2,                         1)
	TAGKEYS(                        XK_3,                         2)
	TAGKEYS(                        XK_4,                         3)
	TAGKEYS(                        XK_5,                         4)
	TAGKEYS(                        XK_6,                         5)
	TAGKEYS(                        XK_7,                         6)
	TAGKEYS(                        XK_8,                         7)
	TAGKEYS(                        XK_9,                         8)
	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,              {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
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

