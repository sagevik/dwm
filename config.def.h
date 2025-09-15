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
static const int showsystray         = 1;        /* 0 means no systray */
static const int showbar             = 1;        /* 0 means no bar */
static const int topbar              = 1;        /* 0 means bottom bar */
static const char *fonts[]           = { "Hack:size=10:style=bold" };
static const char dmenufont[]        = "Hack:size=10:style=bold";
static char normbgcolor[]           = "#222222";
static char normbgcolor2[]           = "#b98d7b";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
	[SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
	[SchemeSel]  = { normbgcolor2,  selbgcolor,  selbordercolor  },

	// [SchemeTitle]  = { selfgcolor,  normbgcolor, normbordercolor  },
	[SchemeStatus]  = { "#888888", normbgcolor,  "#000000"  }, // Statusbar right {text,background,not used but cannot be empty}
	[SchemeTagsSel]  = { selfgcolor, normbgcolor2,  "#000000"  }, // Tagbar left selected {text,background,not used but cannot be empty}
	[SchemeTagsNorm]  = { "#888888", normbgcolor,  "#000000"  }, // Tagbar left unselected {text,background,not used but cannot be empty}
	[SchemeInfoSel]  = { "#999999", normbgcolor,  "#000000"  }, // infobar middle  selected {text,background,not used but cannot be empty}
	[SchemeInfoNorm]  = { "#444444", normbgcolor,  "#000000"  }, // infobar middle  unselected {text,background,not used but cannot be empty}
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
char ghostty[] = "com.mitchellh.ghostty";
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
	{ "Dragon",     NULL,        NULL,	           0,            1,		    1,          0,           1,        -1,      50,50,900,800,	    2,		         0  },
        { "Bitwarden",  NULL,        NULL,	           0,            1,		    1,          0,           0,        -1,      50,50,900,800,	    2,			 0  },
        { "Nsxiv",	"wpicker",   NULL,	           0,            1,		    1,          0,           0,        -1,      360,10,1200,1000,   2,		         0  },
	{ NULL,         NULL,        "webcam",             0,            1,		    1,          1,           0,        -1,      50,50,640,480,	    2,		         0  },
	{ NULL,         NULL,        "nmtui",              0,            1,		    1,          1,           0,        -1,      50,50,450,650,	    2,		         0  },
	{ ghostty,      "ScratchA",  NULL,                 0,            1,		    1,          1,           0,        -1,      50,50,900,800,	    2,		        'a' },
	{ ghostty,      "ScratchS",  NULL,                 0,            1,		    1,          1,           0,        -1,      50,50,900,800,	    2,		        's' },
	{ ghostty,      "ScratchD",  NULL,                 0,            1,		    1,          1,           0,        -1,      360,10,1200,650,    2,		        'd' },
	{ ghostty,      "ScratchF",  NULL,                 0,            1,		    1,          1,           0,        -1,      50,50,900,800,	    2,		        'f' },
	{ ghostty,      "ScratchV",  NULL,                 0,            1,		    1,          1,           0,        -1,      50,50,600,500,	    2,		        'v' },
	// { NULL,         NULL,        "ScratchA",           0,            1,		    1,          1,           0,        -1,      50,50,900,800,	    2,		        'a' },
	// { NULL,         NULL,        "ScratchS",           0,            1,		    1,          1,           0,        -1,      50,50,900,800,	    2,		        's' },
	// { NULL,         NULL,        "ScratchD",           0,            1,		    1,          1,           0,        -1,      360,10,1200,650,    2,		        'd' },
	// { NULL,         NULL,        "ScratchF",           0,            1,		    1,          1,           0,        -1,      50,50,900,800,	    2,		        'f' },
	// { NULL,         NULL,        "ScratchV",           0,            1,		    1,          1,           0,        -1,      50,50,600,500,	    2,		        'v' },

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
static const char *dmenucmd[] = { "dmenu_run", "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", normbgcolor, NULL };
static const char *termcmd[]  = { "ghostty", NULL };
// static const char *termcmd[]  = { "kitty", NULL };

/*First arg only serves to match against key in rules*/
static const char *scratchcmdA[] = {"a", "ghostty", "--x11-instance-name=ScratchA", NULL};
// static const char *scratchcmdA[] = {"a", "kitty", "--title", "ScratchA", NULL};
static const char *scratchcmdS[] = {"s", "ghostty", "--x11-instance-name=ScratchS", NULL};
// static const char *scratchcmdS[] = {"s", "kitty", "--title", "ScratchS", NULL};
static const char *scratchcmdD[] = {"d", "ghostty", "--x11-instance-name=ScratchD", NULL};
// static const char *scratchcmdD[] = {"d", "kitty", "--title", "ScratchD", NULL};
static const char *scratchcmdF[] = {"f", "ghostty", "--x11-instance-name=ScratchF", NULL};
// static const char *scratchcmdF[] = {"f", "kitty", "--title", "ScratchF", NULL};
static const char *scratchcmdV[] = {"v", "ghostty", "--x11-instance-name=ScratchV", NULL};
// static const char *scratchcmdV[] = {"v", "kitty", "--title", "ScratchV", NULL};

#include "movestack.c"
static Keychord *keychords[] = {
       /* Keys                                                              function            argument */
    &((Keychord){1, {{MODKEY, XK_F5}},                                      xrdb,               {.v = NULL } }),
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
    &((Keychord){1, {{MODKEY|ShiftMask,XK_h}},                              setcfact,           {.f = +0.10} }),
    &((Keychord){1, {{MODKEY|ShiftMask,XK_l}},                              setcfact,           {.f = -0.10} }),
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
    &((Keychord){2, {{MODKEY, XK_o}, {0, XK_c}},                            spawn,              {.v = (const char *[]){"qalculate-gtk", NULL}} }),
    &((Keychord){2, {{MODKEY, XK_o}, {0, XK_f}},                            spawn,              {.v = (const char *[]){"nemo", NULL}} }),
    &((Keychord){2, {{MODKEY, XK_o}, {0, XK_v}},                            spawn,              {.v = (const char *[]){"vmselector", NULL}} }),
    &((Keychord){2, {{MODKEY, XK_o}, {0, XK_g}},                            spawn,              {.v = (const char *[]){"gimp", NULL}} }),
    &((Keychord){2, {{MODKEY, XK_o}, {0, XK_b}},                            spawn,              {.v = (const char *[]){"brave", NULL}} }),
    &((Keychord){2, {{MODKEY, XK_o}, {0, XK_j}},                            spawn,              {.v = (const char *[]){"joplin-desktop", NULL}} }),
	// Search
    &((Keychord){2, {{MODKEY, XK_o}, {0, XK_w}},                            spawn,              {.v = (const char *[]){"websearch", NULL}} }),
	// Audio controls
    &((Keychord){1, {{0, XF86XK_AudioLowerVolume}},			    spawn,              {.v = (const char *[]){"volume", "down", NULL}} }),
    &((Keychord){1, {{0, XF86XK_AudioRaiseVolume}},			    spawn,              {.v = (const char *[]){"volume", "up", NULL}} }),
    &((Keychord){1, {{0, XF86XK_AudioMute}},			            spawn,              {.v = (const char *[]){"volume", "mute", NULL}} }),
    &((Keychord){1, {{MODKEY, XF86XK_AudioMute}},			    spawn,              {.v = (const char *[]){"volume", "setvolume", NULL}} }),
	// Screen brightness
    &((Keychord){1, {{0, XF86XK_MonBrightnessDown}},		            spawn,              {.v = (const char *[]){"brightness", "down", NULL}} }),
    &((Keychord){1, {{0, XF86XK_MonBrightnessUp}},			    spawn,              {.v = (const char *[]){"brightness", "up", NULL}} }),
    &((Keychord){1, {{MODKEY, XF86XK_MonBrightnessDown}},		    spawn,              {.v = (const char *[]){"brightness", "setbrightness", NULL}} }),
	// Screenshot
    &((Keychord){1, {{0, 0xff61}},			                    spawn,              {.v = (const char *[]){"scrshot", "selection", NULL}} }),
    &((Keychord){1, {{MODKEY, 0xff61}},		                            spawn,              {.v = (const char *[]){"scrshot", "selectionclip", NULL}} }),
    &((Keychord){1, {{ShiftMask, 0xff61}},		                    spawn,              {.v = (const char *[]){"scrshot", "window", NULL}} }),
    &((Keychord){1, {{ControlMask, 0xff61}},	                            spawn,              {.v = (const char *[]){"scrshot", "fullscreen", NULL}} }),
	// Display setup
    &((Keychord){1, {{MODKEY|Mod1Mask, XK_p}},	                            spawn,              {.v = (const char *[]){"displayselect", NULL}} }),
    &((Keychord){1, {{MODKEY|Mod1Mask|ControlMask, XK_p}},	            spawn,              {.v = (const char *[]){"monitorsetup", NULL}} }),
        // Snippets
    &((Keychord){1, {{MODKEY, XK_Insert}},			            spawn,              {.v = (const char *[]){"snippet", "get", NULL}} }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_Insert}},		            spawn,              {.v = (const char *[]){"snippet", "set", NULL}} }),
    &((Keychord){1, {{MODKEY|Mod1Mask, XK_Insert}},		            spawn,              {.v = (const char *[]){"snippet", NULL}} }),
	// Power menu
    &((Keychord){1, {{MODKEY, XK_Escape}},				    spawn,              {.v = (const char *[]){"pow", NULL}} }),
	// Center window
    &((Keychord){1, {{MODKEY, XK_c}},                                       spawn,	        {.v = (const char *[]){"center_window.sh", NULL}} }), 
    &((Keychord){1, {{MODKEY|ControlMask, XK_c}},                           spawn,	        {.v = (const char *[]){"center_window.sh", "center", NULL}} }), 
    // &((Keychord){1, {{MODKEY|Mod1Mask, XK_BackSpace}},			    spawn,              {.v = (const char *[]){"pow", NULL}} }),
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
