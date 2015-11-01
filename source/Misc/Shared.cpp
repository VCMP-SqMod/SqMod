#include "Misc/Shared.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstring>
#include <algorithm>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const SQChar * GetKeyCodeName(SQInt32 keycode)
{
    switch (keycode)
    {
        case SQMOD_KEYCODE_ABNT_C1:             return _SC("Abnt C1");
        case SQMOD_KEYCODE_ABNT_C2:             return _SC("Abnt C2");
        case SQMOD_KEYCODE_ADD:                 return _SC("Numpad +");
        case SQMOD_KEYCODE_ATTN:                return _SC("Attn");
        case SQMOD_KEYCODE_BACK:                return _SC("Backspace");
        case SQMOD_KEYCODE_CANCEL:              return _SC("Break");
        case SQMOD_KEYCODE_CLEAR:               return _SC("Clear");
        case SQMOD_KEYCODE_CRSEL:               return _SC("Cr Sel");
        case SQMOD_KEYCODE_DECIMAL:             return _SC("Numpad .");
        case SQMOD_KEYCODE_DIVIDE:              return _SC("Numpad /");
        case SQMOD_KEYCODE_EREOF:               return _SC("Er Eof");
        case SQMOD_KEYCODE_ESCAPE:              return _SC("Esc");
        case SQMOD_KEYCODE_EXECUTE:             return _SC("Execute");
        case SQMOD_KEYCODE_EXSEL:               return _SC("Ex Sel");
        case SQMOD_KEYCODE_ICO_CLEAR:           return _SC("IcoClr");
        case SQMOD_KEYCODE_ICO_HELP:            return _SC("IcoHlp");
        case SQMOD_KEYCODE_KEY_0:               return _SC("0");
        case SQMOD_KEYCODE_KEY_1:               return _SC("1");
        case SQMOD_KEYCODE_KEY_2:               return _SC("2");
        case SQMOD_KEYCODE_KEY_3:               return _SC("3");
        case SQMOD_KEYCODE_KEY_4:               return _SC("4");
        case SQMOD_KEYCODE_KEY_5:               return _SC("5");
        case SQMOD_KEYCODE_KEY_6:               return _SC("6");
        case SQMOD_KEYCODE_KEY_7:               return _SC("7");
        case SQMOD_KEYCODE_KEY_8:               return _SC("8");
        case SQMOD_KEYCODE_KEY_9:               return _SC("9");
        case SQMOD_KEYCODE_KEY_A:               return _SC("A");
        case SQMOD_KEYCODE_KEY_B:               return _SC("B");
        case SQMOD_KEYCODE_KEY_C:               return _SC("C");
        case SQMOD_KEYCODE_KEY_D:               return _SC("D");
        case SQMOD_KEYCODE_KEY_E:               return _SC("E");
        case SQMOD_KEYCODE_KEY_F:               return _SC("F");
        case SQMOD_KEYCODE_KEY_G:               return _SC("G");
        case SQMOD_KEYCODE_KEY_H:               return _SC("H");
        case SQMOD_KEYCODE_KEY_I:               return _SC("I");
        case SQMOD_KEYCODE_KEY_J:               return _SC("J");
        case SQMOD_KEYCODE_KEY_K:               return _SC("K");
        case SQMOD_KEYCODE_KEY_L:               return _SC("L");
        case SQMOD_KEYCODE_KEY_M:               return _SC("M");
        case SQMOD_KEYCODE_KEY_N:               return _SC("N");
        case SQMOD_KEYCODE_KEY_O:               return _SC("O");
        case SQMOD_KEYCODE_KEY_P:               return _SC("P");
        case SQMOD_KEYCODE_KEY_Q:               return _SC("Q");
        case SQMOD_KEYCODE_KEY_R:               return _SC("R");
        case SQMOD_KEYCODE_KEY_S:               return _SC("S");
        case SQMOD_KEYCODE_KEY_T:               return _SC("T");
        case SQMOD_KEYCODE_KEY_U:               return _SC("U");
        case SQMOD_KEYCODE_KEY_V:               return _SC("V");
        case SQMOD_KEYCODE_KEY_W:               return _SC("W");
        case SQMOD_KEYCODE_KEY_X:               return _SC("X");
        case SQMOD_KEYCODE_KEY_Y:               return _SC("Y");
        case SQMOD_KEYCODE_KEY_Z:               return _SC("Z");
        case SQMOD_KEYCODE_MULTIPLY:            return _SC("Numpad *");
        case SQMOD_KEYCODE_NONAME:              return _SC("NoName");
        case SQMOD_KEYCODE_NUMPAD0:             return _SC("Numpad 0");
        case SQMOD_KEYCODE_NUMPAD1:             return _SC("Numpad 1");
        case SQMOD_KEYCODE_NUMPAD2:             return _SC("Numpad 2");
        case SQMOD_KEYCODE_NUMPAD3:             return _SC("Numpad 3");
        case SQMOD_KEYCODE_NUMPAD4:             return _SC("Numpad 4");
        case SQMOD_KEYCODE_NUMPAD5:             return _SC("Numpad 5");
        case SQMOD_KEYCODE_NUMPAD6:             return _SC("Numpad 6");
        case SQMOD_KEYCODE_NUMPAD7:             return _SC("Numpad 7");
        case SQMOD_KEYCODE_NUMPAD8:             return _SC("Numpad 8");
        case SQMOD_KEYCODE_NUMPAD9:             return _SC("Numpad 9");
        case SQMOD_KEYCODE_OEM_1:               return _SC("OEM_1 (: ;)");
        case SQMOD_KEYCODE_OEM_102:             return _SC("OEM_102 (> <)");
        case SQMOD_KEYCODE_OEM_2:               return _SC("OEM_2 (? /)");
        case SQMOD_KEYCODE_OEM_3:               return _SC("OEM_3 (~ `)");
        case SQMOD_KEYCODE_OEM_4:               return _SC("OEM_4 ({ [)");
        case SQMOD_KEYCODE_OEM_5:               return _SC("OEM_5 (| \\)");
        case SQMOD_KEYCODE_OEM_6:               return _SC("OEM_6 (} ])");
        case SQMOD_KEYCODE_OEM_7:               return _SC("OEM_7 (\" ')");
        case SQMOD_KEYCODE_OEM_8:               return _SC("OEM_8 (ยง !)");
        case SQMOD_KEYCODE_OEM_ATTN:            return _SC("Oem Attn");
        case SQMOD_KEYCODE_OEM_AUTO:            return _SC("Auto");
        case SQMOD_KEYCODE_OEM_AX:              return _SC("Ax");
        case SQMOD_KEYCODE_OEM_BACKTAB:         return _SC("Back Tab");
        case SQMOD_KEYCODE_OEM_CLEAR:           return _SC("OemClr");
        case SQMOD_KEYCODE_OEM_COMMA:           return _SC("OEM_COMMA (< ,)");
        case SQMOD_KEYCODE_OEM_COPY:            return _SC("Copy");
        case SQMOD_KEYCODE_OEM_CUSEL:           return _SC("Cu Sel");
        case SQMOD_KEYCODE_OEM_ENLW:            return _SC("Enlw");
        case SQMOD_KEYCODE_OEM_FINISH:          return _SC("Finish");
        case SQMOD_KEYCODE_OEM_FJ_LOYA:         return _SC("Loya");
        case SQMOD_KEYCODE_OEM_FJ_MASSHOU:      return _SC("Mashu");
        case SQMOD_KEYCODE_OEM_FJ_ROYA:         return _SC("Roya");
        case SQMOD_KEYCODE_OEM_FJ_TOUROKU:      return _SC("Touroku");
        case SQMOD_KEYCODE_OEM_JUMP:            return _SC("Jump");
        case SQMOD_KEYCODE_OEM_MINUS:           return _SC("OEM_MINUS (_ -)");
        case SQMOD_KEYCODE_OEM_PA1:             return _SC("OemPa1");
        case SQMOD_KEYCODE_OEM_PA2:             return _SC("OemPa2");
        case SQMOD_KEYCODE_OEM_PA3:             return _SC("OemPa3");
        case SQMOD_KEYCODE_OEM_PERIOD:          return _SC("OEM_PERIOD (> .)");
        case SQMOD_KEYCODE_OEM_PLUS:            return _SC("OEM_PLUS (+ =)");
        case SQMOD_KEYCODE_OEM_RESET:           return _SC("Reset");
        case SQMOD_KEYCODE_OEM_WSCTRL:          return _SC("WsCtrl");
        case SQMOD_KEYCODE_PA1:                 return _SC("Pa1");
        case SQMOD_KEYCODE_PACKET:              return _SC("Packet");
        case SQMOD_KEYCODE_PLAY:                return _SC("Play");
        case SQMOD_KEYCODE_PROCESSKEY:          return _SC("Process");
        case SQMOD_KEYCODE_RETURN:              return _SC("Enter");
        case SQMOD_KEYCODE_SELECT:              return _SC("Select");
        case SQMOD_KEYCODE_SEPARATOR:           return _SC("Separator");
        case SQMOD_KEYCODE_SPACE:               return _SC("Space");
        case SQMOD_KEYCODE_SUBTRACT:            return _SC("Num -");
        case SQMOD_KEYCODE_TAB:                 return _SC("Tab");
        case SQMOD_KEYCODE_ZOOM:                return _SC("Zoom");
        case SQMOD_KEYCODE_ACCEPT:              return _SC("Accept");
        case SQMOD_KEYCODE_APPS:                return _SC("Context Menu");
        case SQMOD_KEYCODE_BROWSER_BACK:        return _SC("Browser Back");
        case SQMOD_KEYCODE_BROWSER_FAVORITES:   return _SC("Browser Favorites");
        case SQMOD_KEYCODE_BROWSER_FORWARD:     return _SC("Browser Forward");
        case SQMOD_KEYCODE_BROWSER_HOME:        return _SC("Browser Home");
        case SQMOD_KEYCODE_BROWSER_REFRESH:     return _SC("Browser Refresh");
        case SQMOD_KEYCODE_BROWSER_SEARCH:      return _SC("Browser Search");
        case SQMOD_KEYCODE_BROWSER_STOP:        return _SC("Browser Stop");
        case SQMOD_KEYCODE_CAPITAL:             return _SC("Caps Lock");
        case SQMOD_KEYCODE_CONVERT:             return _SC("Convert");
        case SQMOD_KEYCODE_DELETE:              return _SC("Delete");
        case SQMOD_KEYCODE_DOWN:                return _SC("Arrow Down");
        case SQMOD_KEYCODE_END:                 return _SC("End");
        case SQMOD_KEYCODE_F1:                  return _SC("F1");
        case SQMOD_KEYCODE_F10:                 return _SC("F10");
        case SQMOD_KEYCODE_F11:                 return _SC("F11");
        case SQMOD_KEYCODE_F12:                 return _SC("F12");
        case SQMOD_KEYCODE_F13:                 return _SC("F13");
        case SQMOD_KEYCODE_F14:                 return _SC("F14");
        case SQMOD_KEYCODE_F15:                 return _SC("F15");
        case SQMOD_KEYCODE_F16:                 return _SC("F16");
        case SQMOD_KEYCODE_F17:                 return _SC("F17");
        case SQMOD_KEYCODE_F18:                 return _SC("F18");
        case SQMOD_KEYCODE_F19:                 return _SC("F19");
        case SQMOD_KEYCODE_F2:                  return _SC("F2");
        case SQMOD_KEYCODE_F20:                 return _SC("F20");
        case SQMOD_KEYCODE_F21:                 return _SC("F21");
        case SQMOD_KEYCODE_F22:                 return _SC("F22");
        case SQMOD_KEYCODE_F23:                 return _SC("F23");
        case SQMOD_KEYCODE_F24:                 return _SC("F24");
        case SQMOD_KEYCODE_F3:                  return _SC("F3");
        case SQMOD_KEYCODE_F4:                  return _SC("F4");
        case SQMOD_KEYCODE_F5:                  return _SC("F5");
        case SQMOD_KEYCODE_F6:                  return _SC("F6");
        case SQMOD_KEYCODE_F7:                  return _SC("F7");
        case SQMOD_KEYCODE_F8:                  return _SC("F8");
        case SQMOD_KEYCODE_F9:                  return _SC("F9");
        case SQMOD_KEYCODE_FINAL:               return _SC("Final");
        case SQMOD_KEYCODE_HELP:                return _SC("Help");
        case SQMOD_KEYCODE_HOME:                return _SC("Home");
        case SQMOD_KEYCODE_ICO_00:              return _SC("Ico00 *");
        case SQMOD_KEYCODE_INSERT:              return _SC("Insert");
        case SQMOD_KEYCODE_JUNJA:               return _SC("Junja");
        case SQMOD_KEYCODE_KANA:                return _SC("Kana");
        case SQMOD_KEYCODE_KANJI:               return _SC("Kanji");
        case SQMOD_KEYCODE_LAUNCH_APP1:         return _SC("App1");
        case SQMOD_KEYCODE_LAUNCH_APP2:         return _SC("App2");
        case SQMOD_KEYCODE_LAUNCH_MAIL:         return _SC("Mail");
        case SQMOD_KEYCODE_LAUNCH_MEDIA_SELECT: return _SC("Media");
        case SQMOD_KEYCODE_LBUTTON:             return _SC("Left Button **");
        case SQMOD_KEYCODE_LCONTROL:            return _SC("Left Ctrl");
        case SQMOD_KEYCODE_LEFT:                return _SC("Arrow Left");
        case SQMOD_KEYCODE_LMENU:               return _SC("Left Alt");
        case SQMOD_KEYCODE_LSHIFT:              return _SC("Left Shift");
        case SQMOD_KEYCODE_LWIN:                return _SC("Left Win");
        case SQMOD_KEYCODE_MBUTTON:             return _SC("Middle Button **");
        case SQMOD_KEYCODE_MEDIA_NEXT_TRACK:    return _SC("Next Track");
        case SQMOD_KEYCODE_MEDIA_PLAY_PAUSE:    return _SC("Play / Pause");
        case SQMOD_KEYCODE_MEDIA_PREV_TRACK:    return _SC("Previous Track");
        case SQMOD_KEYCODE_MEDIA_STOP:          return _SC("Stop");
        case SQMOD_KEYCODE_MODECHANGE:          return _SC("Mode Change");
        case SQMOD_KEYCODE_NEXT:                return _SC("Page Down");
        case SQMOD_KEYCODE_NONCONVERT:          return _SC("Non Convert");
        case SQMOD_KEYCODE_NUMLOCK:             return _SC("Num Lock");
        case SQMOD_KEYCODE_OEM_FJ_JISHO:        return _SC("Jisho");
        case SQMOD_KEYCODE_PAUSE:               return _SC("Pause");
        case SQMOD_KEYCODE_PRINT:               return _SC("Print");
        case SQMOD_KEYCODE_PRIOR:               return _SC("Page Up");
        case SQMOD_KEYCODE_RBUTTON:             return _SC("Right Button **");
        case SQMOD_KEYCODE_RCONTROL:            return _SC("Right Ctrl");
        case SQMOD_KEYCODE_RIGHT:               return _SC("Arrow Right");
        case SQMOD_KEYCODE_RMENU:               return _SC("Right Alt");
        case SQMOD_KEYCODE_RSHIFT:              return _SC("Right Shift");
        case SQMOD_KEYCODE_RWIN:                return _SC("Right Win");
        case SQMOD_KEYCODE_SCROLL:              return _SC("Scrol Lock");
        case SQMOD_KEYCODE_SLEEP:               return _SC("Sleep");
        case SQMOD_KEYCODE_SNAPSHOT:            return _SC("Print Screen");
        case SQMOD_KEYCODE_UP:                  return _SC("Arrow Up");
        case SQMOD_KEYCODE_VOLUME_DOWN:         return _SC("Volume Down");
        case SQMOD_KEYCODE_VOLUME_MUTE:         return _SC("Volume Mute");
        case SQMOD_KEYCODE_VOLUME_UP:           return _SC("Volume Up");
        case SQMOD_KEYCODE_XBUTTON1:            return _SC("X Button 1 **");
        case SQMOD_KEYCODE_XBUTTON2:            return _SC("X Button 2 **");
        case SQMOD_KEYCODE_NONE:                return _SC("no mapping");
        default:                                return _SC("");
    }
}


// ------------------------------------------------------------------------------------------------
const SQChar * GetModelName(SQInt32 id)
{
    switch (id)
    {
        case 258: return _SC("Mobile CellPhone");
        case 259: return _SC("Brass Knuckles");
        case 260: return _SC("Screwdriver");
        case 261: return _SC("Golf Club");
        case 262: return _SC("Nigtht Stick");
        case 263: return _SC("Knife");
        case 264: return _SC("Baseball");
        case 265: return _SC("Hammer");
        case 266: return _SC("Meath Cleaver");
        case 267: return _SC("Machete");
        case 268: return _SC("Katana");
        case 269: return _SC("Chainsaw");
        case 270: return _SC("Grenades");
        case 271: return _SC("Tear Gas");
        case 272: return _SC("Molotovs");
        case 273: return _SC("Rocket");
        case 274: return _SC("Colt 45");
        case 275: return _SC("Python");
        case 276: return _SC("Ruger");
        case 277: return _SC("Chrome Shotgun");
        case 278: return _SC("Spaz Shotgun");
        case 279: return _SC("Stubby Shotgun");
        case 280: return _SC("M4");
        case 281: return _SC("Tec9");
        case 282: return _SC("Uzi");
        case 283: return _SC("Silenced Ingram");
        case 284: return _SC("MP5");
        case 285: return _SC("Sniper Rifle");
        case 286: return _SC("Laser Sniper Rifle");
        case 287: return _SC("Rocket Launcher");
        case 288: return _SC("Flamethrower");
        case 289: return _SC("M60");
        case 290: return _SC("Minigun");
        case 291: return _SC("Detonator + Grenades");
        case 292: return _SC("Camera");
        case 293: return _SC("Fists");
        case 294: return _SC("Helicannon");
        case 300: return _SC("bar_gatebar01");
        case 301: return _SC("bar_gatebox01");
        case 302: return _SC("barrier_turn");
        case 303: return _SC("Electricgate");
        case 304: return _SC("fence_small");
        case 305: return _SC("subwaygate");
        case 306: return _SC("tunnel_entrance");
        case 307: return _SC("sub_roadbarrier");
        case 308: return _SC("tall_fence");
        case 309: return _SC("Columbian_gate");
        case 310: return _SC("phils_compnd_gate");
        case 311: return _SC("towerdoor1");
        case 312: return _SC("sub_roadright");
        case 313: return _SC("sub_roadleft");
        case 314: return _SC("airport_gate");
        case 315: return _SC("helix_barrier");
        case 316: return _SC("bar_barrier12");
        case 317: return _SC("bar_barrier16");
        case 318: return _SC("bar_barriergate1");
        case 319: return _SC("bar_barrier10b");
        case 320: return _SC("lhouse_barrier1");
        case 321: return _SC("lhouse_barrier2");
        case 322: return _SC("lhouse_barrier3");
        case 323: return _SC("Gdyn_barrier17");
        case 324: return _SC("bar_barrier10");
        case 330: return _SC("tiny_rock");
        case 331: return _SC("washing_machane");
        case 332: return _SC("junk_tyre");
        case 333: return _SC("cooker1");
        case 334: return _SC("vending_machane");
        case 335: return _SC("briefcase");
        case 336: return _SC("fire_hydrant");
        case 337: return _SC("Money");
        case 338: return _SC("mine");
        case 339: return _SC("bollard");
        case 340: return _SC("bollard_light");
        case 341: return _SC("phonebooth1");
        case 342: return _SC("barrel2");
        case 343: return _SC("barrel1");
        case 344: return _SC("palette");
        case 345: return _SC("cardboardbox2");
        case 346: return _SC("cardboardbox4");
        case 347: return _SC("barrel3");
        case 348: return _SC("lampost_coast");
        case 349: return _SC("wooden_box");
        case 350: return _SC("barrel4");
        case 351: return _SC("lamppost3");
        case 352: return _SC("bin1");
        case 353: return _SC("dump1");
        case 354: return _SC("roadworkbarrier1");
        case 355: return _SC("bussign1");
        case 356: return _SC("cardboard_box");
        case 357: return _SC("Streetlamp2");
        case 358: return _SC("Streetlamp1");
        case 359: return _SC("noparkingsign1");
        case 360: return _SC("phone_sign");
        case 361: return _SC("waste_bin");
        case 362: return _SC("rcyclbank01");
        case 363: return _SC("strtbarrier01");
        case 364: return _SC("traffic_cone");
        case 365: return _SC("info");
        case 366: return _SC("health");
        case 367: return _SC("adrenaline");
        case 368: return _SC("bodyarmour");
        case 369: return _SC("bouy");
        case 370: return _SC("petrolpump");
        case 371: return _SC("newramp");
        case 372: return _SC("line");
        case 373: return _SC("rockpatch1");
        case 374: return _SC("rockpatch03");
        case 375: return _SC("bribe");
        case 376: return _SC("bonus");
        case 377: return _SC("faketarget");
        case 378: return _SC("smashbarpost");
        case 379: return _SC("smashbar");
        case 380: return _SC("barrelexpos");
        case 381: return _SC("glassfx_composh");
        case 382: return _SC("camerapickup");
        case 383: return _SC("killfrenzy");
        case 384: return _SC("telgrphpole02");
        case 385: return _SC("lounger");
        case 386: return _SC("Stonebench1");
        case 387: return _SC("miami_phone");
        case 388: return _SC("miami_hydrant");
        case 389: return _SC("bustop");
        case 390: return _SC("Mpostbox1");
        case 391: return _SC("BillBd1");
        case 392: return _SC("BillBd3");
        case 393: return _SC("LODlBd2");
        case 394: return _SC("MTraffic2");
        case 395: return _SC("Miamibin");
        case 396: return _SC("MTraffic4");
        case 397: return _SC("MTraffic3");
        case 398: return _SC("BlackBag1");
        case 399: return _SC("BlackBag2");
        case 400: return _SC("LODlBd3");
        case 401: return _SC("BillBd2");
        case 402: return _SC("LODlBd1");
        case 403: return _SC("parkingmeter");
        case 404: return _SC("parkingmeterg");
        case 405: return _SC("gunbox");
        case 406: return _SC("property_locked");
        case 407: return _SC("property_foresale");
        case 408: return _SC("bigdollar");
        case 409: return _SC("clothesp");
        case 410: return _SC("package1");
        case 411: return _SC("pickup_save");
        case 412: return _SC("postbox1");
        case 413: return _SC("newsstand1");
        case 414: return _SC("parkbench1");
        case 415: return _SC("papermachn01");
        case 416: return _SC("parktable1");
        case 417: return _SC("lamppost2");
        case 418: return _SC("garden_bench");
        case 419: return _SC("Barrier");
        case 420: return _SC("newstandnew1");
        case 421: return _SC("newstandnew4");
        case 422: return _SC("newstandnew3");
        case 423: return _SC("newstandnew5");
        case 424: return _SC("newstandnew2");
        case 425: return _SC("lamppost1");
        case 426: return _SC("doublestreetlght1");
        case 427: return _SC("trafficlight1");
        case 428: return _SC("MTraffic1");
        case 429: return _SC("lamp_post");
        case 430: return _SC("sea_light");
        case 431: return _SC("craig_package");
        case 432: return _SC("music_headsets");
        case 440: return _SC("veg_tree3");
        case 441: return _SC("veg_treea1");
        case 442: return _SC("veg_treeb1");
        case 443: return _SC("veg_treea3");
        case 444: return _SC("veg_palwee03");
        case 445: return _SC("veg_palm04");
        case 446: return _SC("veg_palm02");
        case 447: return _SC("veg_palm03");
        case 448: return _SC("veg_palwee01");
        case 449: return _SC("veg_palwee02");
        case 450: return _SC("veg_palmkb1");
        case 451: return _SC("veg_palmkb2");
        case 452: return _SC("veg_palmkb3");
        case 453: return _SC("veg_palmkb4");
        case 454: return _SC("veg_palmkb5");
        case 455: return _SC("veg_palmkb8");
        case 456: return _SC("veg_palmkb9");
        case 457: return _SC("veg_palmkb7");
        case 458: return _SC("veg_palmkb10");
        case 459: return _SC("veg_palmkbb11");
        case 460: return _SC("veg_fern_balcny_kb1");
        case 461: return _SC("veg_fern_balcny_kb2");
        case 462: return _SC("kb_pot_1");
        case 463: return _SC("kb_planterbush2");
        case 464: return _SC("kb_planterbox");
        case 465: return _SC("kb_planter+bush");
        case 466: return _SC("veg_ivy_balcny_kb3");
        case 467: return _SC("new_bushtest");
        case 468: return _SC("kb_planter+bush2");
        case 469: return _SC("veg_palmkb13");
        case 470: return _SC("kb_canopy_test");
        case 471: return _SC("kb_chr_tbl_test");
        case 472: return _SC("pot_02");
        case 473: return _SC("pot_01");
        case 474: return _SC("veg_palmbig14");
        case 475: return _SC("veg_palmkb14");
        case 476: return _SC("pot_03");
        case 477: return _SC("new_bushsm");
        case 478: return _SC("veg_palm01");
        case 500: return _SC("lf_mediastage");
        case 501: return _SC("trashcan");
        case 502: return _SC("drug_white");
        case 503: return _SC("drug_orange");
        case 504: return _SC("drug_yellow");
        case 505: return _SC("drug_green");
        case 506: return _SC("drug_blue");
        case 507: return _SC("drug_red");
        case 508: return _SC("keycard");
        case 509: return _SC("lf_banner");
        case 510: return _SC("pizzabox");
        case 511: return _SC("tar_gun2");
        case 512: return _SC("tar_gun1");
        case 513: return _SC("tar_civ2");
        case 514: return _SC("tar_civ1");
        case 515: return _SC("tar_frame");
        case 516: return _SC("tar_top");
        case 517: return _SC("tar_upright");
        case 518: return _SC("tar_upleft");
        case 519: return _SC("tar_downleft");
        case 520: return _SC("tar_downright");
        case 521: return _SC("plc_stinger");
        case 522: return _SC("chairsntable");
        case 523: return _SC("satdishbig");
        case 524: return _SC("satdishsml");
        case 525: return _SC("cntrlrsac1");
        case 526: return _SC("beachball");
        case 527: return _SC("fish1single");
        case 528: return _SC("fish2single");
        case 529: return _SC("fish3s");
        case 530: return _SC("jellyfish");
        case 531: return _SC("jellyfish01");
        case 532: return _SC("fish3single");
        case 533: return _SC("fish1s");
        case 534: return _SC("fish2s");
        case 535: return _SC("dolphin");
        case 536: return _SC("shark");
        case 537: return _SC("turtle");
        case 538: return _SC("sandcastle1");
        case 539: return _SC("sandcastle2");
        case 540: return _SC("submarine");
        case 541: return _SC("nt_firehose_01");
        case 542: return _SC("nt_alarm1_01");
        case 543: return _SC("nt_alarm2_01");
        case 544: return _SC("nt_securecam1_01");
        case 545: return _SC("nt_aircon1_01");
        case 546: return _SC("nt_aircon1_02");
        case 547: return _SC("nt_vent1_01");
        case 548: return _SC("nt_vent2_01");
        case 549: return _SC("nt_vent3_01");
        case 550: return _SC("nt_securecam2_01");
        case 551: return _SC("nt_aircon3_01");
        case 552: return _SC("nt_cablebox1_01");
        case 553: return _SC("nt_cablebox2_01");
        case 554: return _SC("nt_cablebox3_01");
        case 555: return _SC("nt_alarm3_01");
        case 556: return _SC("nt_cablebox4_01");
        case 557: return _SC("nt_cablebox5_01");
        case 558: return _SC("nt_cablebox6_01");
        case 559: return _SC("waterjump2");
        case 560: return _SC("waterjump1");
        case 561: return _SC("landjump");
        case 562: return _SC("landjump2");
        case 563: return _SC("nt_aircon1dbl");
        case 564: return _SC("rcbomb");
        case 565: return _SC("od_pat_hutb");
        case 566: return _SC("od_pat_hut");
        case 567: return _SC("od_vbnet");
        case 568: return _SC("beachtowel04");
        case 569: return _SC("beachtowel03");
        case 570: return _SC("beachtowel02");
        case 571: return _SC("beachtowel01");
        case 572: return _SC("lotion");
        case 573: return _SC("lounge_wood_up");
        case 574: return _SC("lounge_towel_up");
        case 575: return _SC("lounge_wood_dn");
        case 576: return _SC("groyne01");
        case 577: return _SC("wglasssmash");
        case 578: return _SC("petrolcan");
        case 579: return _SC("copwindows");
        case 580: return _SC("fence_haiti");
        case 581: return _SC("fence_haitis");
        case 582: return _SC("dynamite");
        case 583: return _SC("waterjumpx2");
        case 584: return _SC("Esculator_step");
        case 587: return _SC("htl_fan_static_dy");
        case 588: return _SC("ramp");
        case 589: return _SC("htl_fan_rotate_dy");
        case 590: return _SC("nt_roadblockCI");
        case 591: return _SC("swivel_chair_B");
        case 592: return _SC("propwinebotl2");
        case 593: return _SC("propashtray1");
        case 594: return _SC("propbeerglass1");
        case 595: return _SC("propwineglass1");
        case 596: return _SC("propvodkabotl1");
        case 597: return _SC("propwinebotl1");
        case 598: return _SC("propcollecttable");
        case 599: return _SC("swivelchair_A");
        case 600: return _SC("Gasgrenade");
        case 601: return _SC("roadsign");
        case 602: return _SC("lodxrefhse1");
        case 603: return _SC("wshxrefhse1");
        case 604: return _SC("wash_gaspump");
        case 605: return _SC("lodxrefhirise1");
        case 606: return _SC("wshxrefhirise1");
        case 607: return _SC("wshxrefhse2");
        case 608: return _SC("lodxrefhse2");
        case 609: return _SC("xod_beacon_dy");
        case 611: return _SC("xod_starlite_dy");
        case 613: return _SC("xod_leslie_nt");
        case 614: return _SC("xod_leslie_dy");
        case 615: return _SC("xod_majestic_dy");
        case 617: return _SC("xod_macalpin_dy");
        case 620: return _SC("xod_tides_dy");
        case 633: return _SC("jw_coffin");
        case 634: return _SC("chairsntableml");
        case 635: return _SC("LODjumbo_01");
        case 636: return _SC("ap_learjet1_01");
        case 637: return _SC("ap_radar1_01");
        case 638: return _SC("ap_jumbo_01");
        default: return _SC("");
    }
}

bool IsModelWeapon(SQInt32 id)
{
    switch (id)
    {
        case 258:
        case 259:
        case 260:
        case 261:
        case 262:
        case 263:
        case 264:
        case 265:
        case 266:
        case 267:
        case 268:
        case 269:
        case 270:
        case 271:
        case 272:
        case 273:
        case 274:
        case 275:
        case 276:
        case 277:
        case 278:
        case 279:
        case 280:
        case 281:
        case 282:
        case 283:
        case 284:
        case 285:
        case 286:
        case 287:
        case 288:
        case 289:
        case 290:
        case 291:
        case 292:
        case 293:
        case 294: return true;
        default: return false;
    }
}

bool IsModelActuallyWeapon(SQInt32 id)
{
    switch (id)
    {
        case 259:
        case 260:
        case 261:
        case 262:
        case 263:
        case 264:
        case 265:
        case 266:
        case 267:
        case 268:
        case 269:
        case 270:
        case 271:
        case 272:
        case 274:
        case 275:
        case 276:
        case 277:
        case 278:
        case 279:
        case 280:
        case 281:
        case 282:
        case 283:
        case 284:
        case 285:
        case 286:
        case 287:
        case 288:
        case 289:
        case 290:
        case 291: return true;
        default: return false;
    }
}

bool IsModelValid(SQInt32 id)
{
    return (strcmp(GetModelName(id), _SC("")) != 0) ? true : false;
}

// ------------------------------------------------------------------------------------------------
const SQChar * GetSkinName(SQInt32 id)
{
    switch(id)
    {
        case SQMOD_SKIN_TOMMY_VERCETTI:         return _SC("Tommy Vercetti");
        case SQMOD_SKIN_COP:                    return _SC("Cop");
        case SQMOD_SKIN_SWAT:                   return _SC("SWAT");
        case SQMOD_SKIN_FBI:                    return _SC("FBI");
        case SQMOD_SKIN_ARMY:                   return _SC("Army");
        case SQMOD_SKIN_PARAMEDIC:              return _SC("Paramedic");
        case SQMOD_SKIN_FIREMAN:                return _SC("Firefighter");
        case SQMOD_SKIN_GOLF_GUY_A:             return _SC("Golf Guy #1");
        case SQMOD_SKIN_BUM_LADY_A:             return _SC("Bum Lady #1");
        case SQMOD_SKIN_BUM_LADY_B:             return _SC("Bum Lady #2");
        case SQMOD_SKIN_PUNK_A:                 return _SC("Punk #1");
        case SQMOD_SKIN_LAWYER:                 return _SC("Lawyer");
        case SQMOD_SKIN_SPANISH_LADY_A:         return _SC("Spanish Lady #1");
        case SQMOD_SKIN_SPANISH_LADY_B:         return _SC("Spanish Lady #2");
        case SQMOD_SKIN_COOL_GUY_A:             return _SC("Cool Guy #1");
        case SQMOD_SKIN_ARABIC_GUY:             return _SC("Arabic Guy");
        case SQMOD_SKIN_BEACH_LADY_A:           return _SC("Beach Lady #1");
        case SQMOD_SKIN_BEACH_LADY_B:           return _SC("Beach Lady #2");
        case SQMOD_SKIN_BEACH_GUY_A:            return _SC("Beach Guy #1");
        case SQMOD_SKIN_BEACH_GUY_B:            return _SC("Beach Guy #2");
        case SQMOD_SKIN_OFFICE_LADY_A:          return _SC("Office Lady #1");
        case SQMOD_SKIN_WAITRESS_A:             return _SC("Waitress #1");
        case SQMOD_SKIN_FOOD_LADY:              return _SC("Food Lady");
        case SQMOD_SKIN_PROSTITUTE_A:           return _SC("Prostitute #1");
        case SQMOD_SKIN_BUM_LADY_C:             return _SC("Bum Lady #3");
        case SQMOD_SKIN_BUM_GUY_A:              return _SC("Bum Guy #1");
        case SQMOD_SKIN_GARBAGEMAN_A:           return _SC("Garbageman #1");
        case SQMOD_SKIN_TAXI_DRIVER_A:          return _SC("Taxi Driver #1");
        case SQMOD_SKIN_HATIAN_A:               return _SC("Haitian #1");
        case SQMOD_SKIN_CRIMINAL_A:             return _SC("Criminal #1");
        case SQMOD_SKIN_HOOD_LADY:              return _SC("Hood Lady");
        case SQMOD_SKIN_GRANNY_A:               return _SC("Granny #1");
        case SQMOD_SKIN_BUSINESS_MAN_A:         return _SC("Businessman #1");
        case SQMOD_SKIN_CHURCH_GUY:             return _SC("Church Guy");
        case SQMOD_SKIN_CLUB_LADY:              return _SC("Club Lady");
        case SQMOD_SKIN_CHURCH_LADY:            return _SC("Church Lady");
        case SQMOD_SKIN_PIMP:                   return _SC("Pimp");
        case SQMOD_SKIN_BEACH_LADY_C:           return _SC("Beach Lady #3");
        case SQMOD_SKIN_BEACH_GUY_C:            return _SC("Beach Guy #3");
        case SQMOD_SKIN_BEACH_LADY_D:           return _SC("Beach Lady #4");
        case SQMOD_SKIN_BEACH_GUY_D:            return _SC("Beach Guy #4");
        case SQMOD_SKIN_BUSINESS_MAN_B:         return _SC("Businessman #2");
        case SQMOD_SKIN_PROSTITUTE_B:           return _SC("Prostitute #2");
        case SQMOD_SKIN_BUM_LADY_D:             return _SC("Bum Lady #4");
        case SQMOD_SKIN_BUM_GUY_B:              return _SC("Bum Guy #2");
        case SQMOD_SKIN_HATIAN_B:               return _SC("Haitian #2");
        case SQMOD_SKIN_CONSTRUCTION_WORKER_A:  return _SC("Construction Worker #1");
        case SQMOD_SKIN_PUNK_B:                 return _SC("Punk #2");
        case SQMOD_SKIN_PROSTITUTE_C:           return _SC("Prostitute #3");
        case SQMOD_SKIN_GRANNY_B:               return _SC("Granny #2");
        case SQMOD_SKIN_PUNK_C:                 return _SC("Punk #3");
        case SQMOD_SKIN_BUSINESS_MAN_C:         return _SC("Businessman #3");
        case SQMOD_SKIN_SPANISH_LADY_C:         return _SC("Spanish Lady #3");
        case SQMOD_SKIN_SPANISH_LADY_D:         return _SC("Spanish Lady #4");
        case SQMOD_SKIN_COOL_GUY_B:             return _SC("Cool Guy #2");
        case SQMOD_SKIN_BUSINESS_MAN_D:         return _SC("Businessman #4");
        case SQMOD_SKIN_BEACH_LADY_E:           return _SC("Beach Lady #5");
        case SQMOD_SKIN_BEACH_GUY_E:            return _SC("Beach Guy #5");
        case SQMOD_SKIN_BEACH_LADY_F:           return _SC("Beach Lady #6");
        case SQMOD_SKIN_BEACH_GUY_F:            return _SC("Beach Guy #6");
        case SQMOD_SKIN_CONSTRUCTION_WORKER_B:  return _SC("Construction Worker #2");
        case SQMOD_SKIN_GOLF_GUY_B:             return _SC("Golf Guy #2");
        case SQMOD_SKIN_GOLF_LADY:              return _SC("Golf Lady");
        case SQMOD_SKIN_GOLF_GUY_C:             return _SC("Golf Guy #3");
        case SQMOD_SKIN_BEACH_LADY_G:           return _SC("Beach Lady #7");
        case SQMOD_SKIN_BEACH_GUY_G:            return _SC("Beach Guy #7");
        case SQMOD_SKIN_OFFICE_LADY_B:          return _SC("Office Lady #2");
        case SQMOD_SKIN_BUSINESS_MAN_E:         return _SC("Businessman #5");
        case SQMOD_SKIN_BUSINESS_MAN_F:         return _SC("Businessman #6");
        case SQMOD_SKIN_PROSTITUTE_D:           return _SC("Prostitute #2");
        case SQMOD_SKIN_BUM_LADY_E:             return _SC("Bum Lady #4");
        case SQMOD_SKIN_BUM_GUY_C:              return _SC("Bum Guy #3");
        case SQMOD_SKIN_SPANISH_GUY:            return _SC("Spanish Guy");
        case SQMOD_SKIN_TAXI_DRIVER_B:          return _SC("Taxi Driver #2");
        case SQMOD_SKIN_GYM_LADY:               return _SC("Gym Lady");
        case SQMOD_SKIN_GYM_GUY:                return _SC("Gym Guy");
        case SQMOD_SKIN_SKATE_LADY:             return _SC("Skate Lady");
        case SQMOD_SKIN_SKATE_GUY:              return _SC("Skate Guy");
        case SQMOD_SKIN_SHOPPER_A:              return _SC("Shopper #1");
        case SQMOD_SKIN_SHOPPER_B:              return _SC("Shopper #2");
        case SQMOD_SKIN_TOURIST_A:              return _SC("Tourist #1");
        case SQMOD_SKIN_TOURIST_B:              return _SC("Tourist #2");
        case SQMOD_SKIN_CUBAN_A:                return _SC("Cuban #1");
        case SQMOD_SKIN_CUBAN_B:                return _SC("Cuban #2");
        case SQMOD_SKIN_HATIAN_C:               return _SC("Haitian #3");
        case SQMOD_SKIN_HATIAN_D:               return _SC("Haitian #4");
        case SQMOD_SKIN_SHARK_A:                return _SC("Shark #1");
        case SQMOD_SKIN_SHARK_B:                return _SC("Shark #2");
        case SQMOD_SKIN_DIAZ_GUY_A:             return _SC("Diaz Guy #1");
        case SQMOD_SKIN_DIAZ_GUY_B:             return _SC("Diaz Guy #2");
        case SQMOD_SKIN_DBP_SECURITY_A:         return _SC("DBP Security #1");
        case SQMOD_SKIN_DBP_SECURITY_B:         return _SC("DBP Security #2");
        case SQMOD_SKIN_BIKER_A:                return _SC("Biker #1");
        case SQMOD_SKIN_BIKER_B:                return _SC("Biker #2");
        case SQMOD_SKIN_VERCETTI_GUY_A:         return _SC("Vercetti Guy #1");
        case SQMOD_SKIN_VERCETTI_GUY_B:         return _SC("Vercetti Guy #2");
        case SQMOD_SKIN_UNDERCOVER_COP_A:       return _SC("Undercover Cop #1");
        case SQMOD_SKIN_UNDERCOVER_COP_B:       return _SC("Undercover Cop #2");
        case SQMOD_SKIN_UNDERCOVER_COP_C:       return _SC("Undercover Cop #3");
        case SQMOD_SKIN_UNDERCOVER_COP_D:       return _SC("Undercover Cop #4");
        case SQMOD_SKIN_UNDERCOVER_COP_E:       return _SC("Undercover Cop #5");
        case SQMOD_SKIN_UNDERCOVER_COP_F:       return _SC("Undercover Cop #6");
        case SQMOD_SKIN_RICH_GUY:               return _SC("Rich Guy");
        case SQMOD_SKIN_COOL_GUY_C:             return _SC("Cool Guy #3");
        case SQMOD_SKIN_PROSTITUTE_E:           return _SC("Prostitute #3");
        case SQMOD_SKIN_PROSTITUTE_F:           return _SC("Prostitute #4");
        case SQMOD_SKIN_LOVE_FIST_A:            return _SC("Love Fist #1");
        case SQMOD_SKIN_KEN_ROSENBURG:          return _SC("Ken Rosenburg");
        case SQMOD_SKIN_CANDY_SUXX:             return _SC("Candy Suxx");
        case SQMOD_SKIN_HILARY:                 return _SC("Hilary");
        case SQMOD_SKIN_LOVE_FIST_B:            return _SC("Love Fist #2");
        case SQMOD_SKIN_PHIL:                   return _SC("Phil");
        case SQMOD_SKIN_ROCKSTAR_GUY:           return _SC("Rockstar Guy");
        case SQMOD_SKIN_SONNY:                  return _SC("Sonny");
        case SQMOD_SKIN_LANCE_A:                return _SC("Lance");
        case SQMOD_SKIN_MERCADES_A:             return _SC("Mercedes");
        case SQMOD_SKIN_LOVE_FIST_C:            return _SC("Love Fist #3");
        case SQMOD_SKIN_ALEX_SRUB:              return _SC("Alex Shrub");
        case SQMOD_SKIN_LANCE_COP:              return _SC("Lance (Cop)");
        case SQMOD_SKIN_LANCE_B:                return _SC("Lance");
        case SQMOD_SKIN_CORTEZ:                 return _SC("Cortez");
        case SQMOD_SKIN_LOVE_FIST_D:            return _SC("Love Fist #4");
        case SQMOD_SKIN_COLUMBIAN_GUY_A:        return _SC("Columbian Guy #1");
        case SQMOD_SKIN_HILARY_ROBBER:          return _SC("Hilary (Robber)");
        case SQMOD_SKIN_MERCADES_B:             return _SC("Mercedes");
        case SQMOD_SKIN_CAM:                    return _SC("Cam");
        case SQMOD_SKIN_CAM_ROBBER:             return _SC("Cam (Robber)");
        case SQMOD_SKIN_PHIL_ONE_ARM:           return _SC("Phil (One Arm)");
        case SQMOD_SKIN_PHIL_ROBBER:            return _SC("Phil (Robber)");
        case SQMOD_SKIN_COOL_GUY_D:             return _SC("Cool Guy #4");
        case SQMOD_SKIN_PIZZAMAN:               return _SC("Pizza Man");
        case SQMOD_SKIN_TAXI_DRIVER_C:          return _SC("Taxi Driver #1");
        case SQMOD_SKIN_TAXI_DRIVER_D:          return _SC("Taxi Driver #2");
        case SQMOD_SKIN_SAILOR_A:               return _SC("Sailor #1");
        case SQMOD_SKIN_SAILOR_B:               return _SC("Sailor #2");
        case SQMOD_SKIN_SAILOR_C:               return _SC("Sailor #3");
        case SQMOD_SKIN_CHEF:                   return _SC("Chef");
        case SQMOD_SKIN_CRIMINAL_B:             return _SC("Criminal #2");
        case SQMOD_SKIN_FRENCH_GUY:             return _SC("French Guy");
        case SQMOD_SKIN_GARBAGEMAN_B:           return _SC("Garbageman #2");
        case SQMOD_SKIN_HATIAN_E:               return _SC("Haitian #5");
        case SQMOD_SKIN_WAITRESS_B:             return _SC("Waitress #2");
        case SQMOD_SKIN_SONNY_GUY_A:            return _SC("Sonny Guy #1");
        case SQMOD_SKIN_SONNY_GUY_B:            return _SC("Sonny Guy #2");
        case SQMOD_SKIN_SONNY_GUY_C:            return _SC("Sonny Guy #3");
        case SQMOD_SKIN_COLUMBIAN_GUY_B:        return _SC("Columbian Guy #2");
        case SQMOD_SKIN_THUG_A:                 return _SC("Haitian #6");
        case SQMOD_SKIN_BEACH_GUY_H:            return _SC("Beach Guy #8");
        case SQMOD_SKIN_GARBAGEMAN_C:           return _SC("Garbageman #3");
        case SQMOD_SKIN_GARBAGEMAN_D:           return _SC("Garbageman #4");
        case SQMOD_SKIN_GARBAGEMAN_E:           return _SC("Garbageman #5");
        case SQMOD_SKIN_TRANNY:                 return _SC("Tranny");
        case SQMOD_SKIN_THUG_B:                 return _SC("Thug #5");
        case SQMOD_SKIN_SPANDEX_GUY_A:          return _SC("SpandEx Guy #1");
        case SQMOD_SKIN_SPANDEX_GUY_B:          return _SC("SpandEx Guy #2");
        case SQMOD_SKIN_STRIPPER_A:             return _SC("Stripper #1");
        case SQMOD_SKIN_STRIPPER_B:             return _SC("Stripper #2");
        case SQMOD_SKIN_STRIPPER_C:             return _SC("Stripper #3");
        case SQMOD_SKIN_STORE_CLERK:            return _SC("Store Clerk");
        default:                                return _SC("");
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 GetSkinID(const SQChar * name)
{
    // Clone the string into an editable version
    String str(name);
    // Strip non alphanumeric characters from the name
    str.erase(std::remove_if(str.begin(), str.end(), std::not1(std::ptr_fun(::isalnum))), str.end());
    // Convert the string to lowercase
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    // See if we still have a valid name after the cleanup
    if(str.empty()) return SQMOD_UNKNOWN;
    // Grab the actual length of the string
    unsigned len = str.length();
    // Get the most significant characters used to identify a skin
    SQChar a = str[0], b = 0, c = 0, d = str[len-1];
    // Look for deeper specifiers
    if (str.length() >= 3)
    {
        b = str[1];
        c = str[2];
    }
    else if (str.length() >= 2)
    {
        b = str[1];
    }
    // Search for a pattern in the name
    switch (a)
    {
        // [A]lex Srub, [A]rabic guy, [A]rmy
        case 'a':
            switch (b)
            {
                // [Al]ex [S]rub
                case 'l':
                case 's': return SQMOD_SKIN_ALEX_SRUB;
                // [A]rabic [g]uy
                case 'g': return SQMOD_SKIN_ARABIC_GUY;
                // [Ara]bic guy, [Arm]y
                case 'r':
                    if (c == 'a') return SQMOD_SKIN_ARABIC_GUY;
                    else if (c == 'm') return SQMOD_SKIN_ARMY;
            }
        // [B]each guy (#1|A)/(#2|B)/(#3|C)/(#4|D)/(#5|E)/(#6|F)/(#7|G)/(#8|H)
        // [B]each lady (#1|A)/(#2|B)/(#3|C)/(#4|D)/(#5|E)/(#6|F)/(#7|G)
        // [B]iker (#1|A)/(#2|B)
        // [B]um guy (#1|A)/(#2|B)/(#3|C)
        // [B]um lady (#1|A)/(#2|B)/(#2|C)/(#3|D)/(#4|E)
        // [B]usiness man (#1|A)/(#2|B)/(#3|C)/(#4|D)/(#5|E)/(#6|F)
        case 'b':
            // [Be]ach [g]uy (#1|A)/(#2|B)/(#3|C)/(#4|D)/(#5|E)/(#6|F)/(#7|G)/(#8|H)
            if (b == 'e' && (c == 'g' || (len > 4 && str[5] == 'g')))
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_BEACH_GUY_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_BEACH_GUY_B;
                    case '3':
                    case 'c': return SQMOD_SKIN_BEACH_GUY_C;
                    case '4':
                    case 'd': return SQMOD_SKIN_BEACH_GUY_D;
                    case '5':
                    case 'e': return SQMOD_SKIN_BEACH_GUY_E;
                    case '6':
                    case 'f': return SQMOD_SKIN_BEACH_GUY_F;
                    case '7':
                    case 'g': return SQMOD_SKIN_BEACH_GUY_G;
                    case '8':
                    case 'h': return SQMOD_SKIN_BEACH_GUY_H;
                }
            }
            // [Be]ach [l]ady (#1|A)/(#2|B)/(#3|C)/(#4|D)/(#5|E)/(#6|F)/(#7|G)
            else if (b == 'e' && (c == 'l' || (len > 4 && str[5] == 'l')))
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_BEACH_LADY_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_BEACH_LADY_B;
                    case '3':
                    case 'c': return SQMOD_SKIN_BEACH_LADY_C;
                    case '4':
                    case 'd': return SQMOD_SKIN_BEACH_LADY_D;
                    case '5':
                    case 'e': return SQMOD_SKIN_BEACH_LADY_E;
                    case '6':
                    case 'f': return SQMOD_SKIN_BEACH_LADY_F;
                    case '7':
                    case 'g': return SQMOD_SKIN_BEACH_LADY_G;
                }
            }
            // [Bi]ker (#1|A)/(#2|B)
            else if (b == 'i' && (d == '1' || d == 'a')) return SQMOD_SKIN_BIKER_A;
            else if (b == 'i' && (d == '2' || d == 'b')) return SQMOD_SKIN_BIKER_B;
            // [Bum] [g]uy (#1|A)/(#2|B)/(#3|C)
            // [Bum] [l]ady (#1|A)/(#2|B)/(#2|C)/(#3|D)/(#4|E)
            else if (b == 'u' && (c && (c == 'm' || c == 'g' || c == 'l')))
            {
                // [Bum] [g]uy (#1|A)/(#2|B)/(#3|C)
                if (c == 'g' || (len > 2 && str[3] == 'g'))
                {
                    if (d == '1' || d == 'a') return SQMOD_SKIN_BUM_GUY_A;
                    else if (d == '2' || d == 'b') return SQMOD_SKIN_BUM_GUY_B;
                    else if (d == '3' || d == 'c') return SQMOD_SKIN_BUM_GUY_C;
                }
                // [Bum] [l]ady (#1|A)/(#2|B)/(#2|C)/(#3|D)/(#4|E)
                else if (c == 'l' || (len > 2 && str[3] == 'l'))
                {
                    if (d == '1' || d == 'a') return SQMOD_SKIN_BUM_LADY_A;
                    else if (d == '2' || d == 'b') return SQMOD_SKIN_BUM_LADY_B;
                    else if (d == '2' || d == 'c') return SQMOD_SKIN_BUM_LADY_C;
                    else if (d == '3' || d == 'd') return SQMOD_SKIN_BUM_LADY_D;
                    else if (d == '4' || d == 'e') return SQMOD_SKIN_BUM_LADY_E;
                }
            }
            // [Bus]iness [m]an (#1|A)/(#2|B)/(#3|C)/(#4|D)/(#5|E)/(#6|F)
            else if (b == 'u' && (c == 's' || (len > 8 && str[9] == 'm')))
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_BUSINESS_MAN_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_BUSINESS_MAN_B;
                    case '3':
                    case 'c': return SQMOD_SKIN_BUSINESS_MAN_C;
                    case '4':
                    case 'd': return SQMOD_SKIN_BUSINESS_MAN_D;
                    case '5':
                    case 'e': return SQMOD_SKIN_BUSINESS_MAN_E;
                    case '6':
                    case 'f': return SQMOD_SKIN_BUSINESS_MAN_F;
                }
            }
        // [C]am, [C]am (Robber), [C]andy Suxx, [C]hef
        // [C]hurch guy, [C]hurch lady, [C]lub lady
        // [C]olumbian guy (#1|A)/(#2|B),
        // [C]onstruction worker (#1|A)/(#2|B)
        // [C]ool guy (#1|A)/(#2|B)/(#3|C)/(#4|D)
        // [C]op, [C]ortez
        // [C]riminal (#1|A)/(#2|B)
        // [C]uban (#1|A)/(#2|B)
        case 'c':
            // [Ca]m, [Ca]m (Robber), [Ca]ndy Suxx
            if (b == 'a')
            {
                // [Cam] ([R]obbe[r])
                if (c && (c == 'm' || c == 'r') && d == 'r') return SQMOD_SKIN_CAM_ROBBER;
                // [Cam]
                else if (c == 'm') return SQMOD_SKIN_CAM;
                // [Can]dy [S]ux[x]
                else if (c && (c == 'n' || c == 's' || d == 'x')) return SQMOD_SKIN_CANDY_SUXX;
            }
            // [Ch]ef, [Ch]urch guy, [Ch]urch lady
            else if (b == 'h')
            {
                // [Che][f]
                if (c && (c == 'e' || d == 'f')) return SQMOD_SKIN_CHEF;
                // [Chu]rch [g]uy
                else if (c && ((c == 'u' && len > 5 && str[6] == 'g') || (c == 'g')))
                    return SQMOD_SKIN_CHURCH_GUY;
                // [Chu]rch [l]ady
                else if (c && ((c == 'u' && len > 5 && str[6] == 'l') || (c == 'l')))
                    return SQMOD_SKIN_CHURCH_LADY;
            }
            // [Cl]ub [l]ady
            else if (b == 'l') return SQMOD_SKIN_CLUB_LADY;
            // [Co]lumbian guy (#1|A)/(#2|B)
            // [Co]nstruction worker (#1|A)/(#2|B)
            // [Co]ol guy (#1|A)/(#2|B)/(#3|C)/(#4|D)
            // [Co]p, [Co]rtez
            else if (b == 'o')
            {
                // [Col]umbian [g]uy (#1|A)/(#2|B)
                if (c && ((c == 'l' && len > 8 && str[9] == 'g') || (c == 'g')))
                {
                    if (d == '1' || d == 'a') return SQMOD_SKIN_COLUMBIAN_GUY_A;
                    else if (d == '2' || d == 'b') return SQMOD_SKIN_COLUMBIAN_GUY_B;
                }
                // [Con]struction [w]orker (#1|A)/(#2|B)
                else if (c && (c == 'n' || (len > 11 && str[12] == 'g')))
                {
                    if (d == '1' || d == 'a') return SQMOD_SKIN_CONSTRUCTION_WORKER_A;
                    else if (d == '2' || d == 'b') return SQMOD_SKIN_CONSTRUCTION_WORKER_B;
                }
                // [Coo]l guy (#1|A)/(#2|B)/(#3|C)/(#4|D)
                else if (c == 'o')
                {
                    switch (d)
                    {
                        case '1':
                        case 'a': return SQMOD_SKIN_COOL_GUY_A;
                        case '2':
                        case 'b': return SQMOD_SKIN_COOL_GUY_B;
                        case '3':
                        case 'c': return SQMOD_SKIN_COOL_GUY_C;
                        case '4':
                        case 'd': return SQMOD_SKIN_COOL_GUY_D;
                    }
                }
                // [Cop]
                else if (c == 'p') return SQMOD_SKIN_COP;
                // [Cor]te[z]
                else if (c && (c == 'r' || c == 'z' || d == 'z')) return SQMOD_SKIN_CORTEZ;
            }
            // [Cr]iminal (#1|A)/(#2|B)
            else if (b == 'r' && (d == '1' || d == 'a')) return SQMOD_SKIN_CRIMINAL_A;
            else if (b == 'r' && (d == '2' || d == 'b')) return SQMOD_SKIN_CRIMINAL_B;
            // [Cu]ban (#1|A)/(#2|B)
            else if (b == 'u' && (d == '1' || d == 'a')) return SQMOD_SKIN_CUBAN_A;
            else if (b == 'u' && (d == '2' || d == 'b')) return SQMOD_SKIN_CUBAN_B;
        // [D]BP security (#1|A)/(#2|B)
        // [D]iaz guy (#1|A)/(#2|B)
        case 'd':
            switch (b)
            {
                // [DB]P [s]ecurity (#1|A)/(#2|B)
                case 'b':
                case 's':
                    if (d == '1' || d == 'a') return SQMOD_SKIN_DBP_SECURITY_A;
                    else if (d == '2' || d == 'b') return SQMOD_SKIN_DBP_SECURITY_B;
                // [Di]a[z] [g]uy (#1|A)/(#2|B)
                case 'i':
                case 'z':
                case 'g':
                    if (d == '1' || d == 'a') return SQMOD_SKIN_DIAZ_GUY_A;
                    else if (d == '2' || d == 'b') return SQMOD_SKIN_DIAZ_GUY_B;
            }
        // [F]BI, [F]ireman, [F]ood lady, [F]rench guy
        case 'f':
            switch (b)
            {
                // [FB]I
                case 'b': return SQMOD_SKIN_FBI;
                // [Fi]re[m]an
                case 'i':
                case 'm': return SQMOD_SKIN_FIREMAN;
                // [Fo]od [l]ady
                case 'o':
                case 'l': return SQMOD_SKIN_FOOD_LADY;
                // [Fr]ench [g]uy
                case 'r':
                case 'g': return SQMOD_SKIN_FRENCH_GUY;
            }
        // [G]arbageman (#1|A)/(#2|B)/(#3|C)/(#4|D)/(#5|E)
        // [G]olf guy (#1|A)/(#2|B)/(#3|C)
        // [G]olf lady
        // [G]ranny (#1|A)/(#2|B)
        // [G]ym guy, [G]ym lady
        case 'g':
            // [Ga]rbage[m]an (#1|A)/(#2|B)/(#3|C)/(#4|D)/(#5|E)
            if (b && (b == 'a' || b == 'm'))
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_GARBAGEMAN_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_GARBAGEMAN_B;
                    case '3':
                    case 'c': return SQMOD_SKIN_GARBAGEMAN_C;
                    case '4':
                    case 'd': return SQMOD_SKIN_GARBAGEMAN_D;
                    case '5':
                    case 'e': return SQMOD_SKIN_GARBAGEMAN_E;
                }
            }
            // [Go]lf [g]uy (#1|A)/(#2|B)/(#3|C)
            else if (b == 'o' && ((c == 'g') || (len > 3 && str[4] == 'g')))
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_GOLF_GUY_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_GOLF_GUY_B;
                    case '3':
                    case 'c': return SQMOD_SKIN_GOLF_GUY_C;
                }
            }
            // [Go]lf [l]ady
            else if (b == 'o' && ((c == 'l') || (len > 3 && str[4] == 'l')))
                return SQMOD_SKIN_GOLF_LADY;
            // [Gr]anny (#1|A)/(#2|B)
            else if (b == 'r')
            {
                if (d == '1' || d == 'a') return SQMOD_SKIN_GRANNY_A;
                else if (d == '2' || d == 'b') return SQMOD_SKIN_GRANNY_B;
            }
            // [Gy]m [g]uy
            else if (b && (b == 'g' || (b == 'y' && len > 2 && str[3] == 'g')))
                return SQMOD_SKIN_GYM_GUY;
            // [Gy]m [l]ady
            else if (b && (b == 'l' || (b == 'y' && len > 2 && str[3] == 'l')))
                return SQMOD_SKIN_GYM_LADY;
        // [H]atian (#1|A)/(#2|B)/(#3|C)/(#4|D)/(#5|E)
        // [H]ilary, [H]ilary (Robber), [H]ood lady
        case 'h':
            // [H]atian (#1|A)/(#2|B)/(#3|C)/(#4|D)/(#5|E)
            if (b == 'a')
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_HATIAN_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_HATIAN_B;
                    case '3':
                    case 'c': return SQMOD_SKIN_HATIAN_C;
                    case '4':
                    case 'd': return SQMOD_SKIN_HATIAN_D;
                    case '5':
                    case 'e': return SQMOD_SKIN_HATIAN_E;
                }
            }
            // [Hi]lary ([R]obbe[r])
            else if (b && (b == 'i' || b == 'r') && d == 'r') return SQMOD_SKIN_HILARY_ROBBER;
            // [Hi]lary
            else if (b == 'i') return SQMOD_SKIN_HILARY;
            // [Ho]od [l]ady
            if (b && (b == 'o' || b == 'l')) return SQMOD_SKIN_HOOD_LADY;
        // [K]en Rosenburg
        case 'k':
            return SQMOD_SKIN_KEN_ROSENBURG;
        // [L]ance (#1|A)/(#1|B)
        // [L]ance (Cop)
        // [L]awyer
        // [L]ove Fist (#1|A)/(#2|B)/(#3|C)/(#3|D)
        case 'l':
            //[Lan]ce ([C]o[p])
            if ((b == 'a') && (c == 'n') && ((len > 4 && str[5] == 'c') || d == 'p'))
                return SQMOD_SKIN_LANCE_COP;
            else if (b && (b == 'c' ||  (b == 'a' && (c == 'n'))))
                return SQMOD_SKIN_LANCE_COP;
            // [La]nce (#1|A)/(#1|B)
            else if (b == 'a' && c == 'n')
            {
                if (d == '1' || d == 'a') return SQMOD_SKIN_LANCE_A;
                else if (d == '2' || d == 'b') return SQMOD_SKIN_LANCE_B;
            }
            // [Law]yer
            else if (b && (b == 'w' || (b == 'a' && c == 'w'))) return SQMOD_SKIN_LAWYER;
            // [Lo]ve [F]ist (#1|A)/(#2|B)/(#3|C)/(#3|D)
            else if (b && (b == 'o' || b == 'f'))
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_LOVE_FIST_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_LOVE_FIST_B;
                    case '3':
                    case 'c': return SQMOD_SKIN_LOVE_FIST_C;
                    case 'd': return SQMOD_SKIN_LOVE_FIST_D;
                }
            }
        // [M]ercades
        case 'm':
            if (d == 'b') return SQMOD_SKIN_MERCADES_B;
            else return SQMOD_SKIN_MERCADES_A;
        // [O]ffice lady (#1|A)/(#2|B)
        case 'o':
            if (d == '1' || d == 'a') return SQMOD_SKIN_OFFICE_LADY_A;
            else if (d == '2' || d == 'b') return SQMOD_SKIN_OFFICE_LADY_B;
        // [P]aramedic, [P]hil,  [P]hil (One arm), [P]hil (Robber)
        // [P]imp, [P]izzaman
        // [P]rostitute (#1|A)/(#2|B)/(#2|C)/(#2|D)/(#3|D)/(#4|D)
        // [P]unk (#1|A)/(#2|B)/(#3|C)
        case 'p':
            // [Pa]ramedic
            if (b == 'a') return SQMOD_SKIN_PARAMEDIC;
            // [Ph]il (One arm), [Ph]il (Robber)
            else if (b == 'h')
            {
                // [Ph]il ([O]ne ar[m])
                if (b == 'o' || (c == 'o') || (len > 3 && str[4] == 'o') || d == 'm')
                    return SQMOD_SKIN_PHIL_ONE_ARM;
                // [Ph]il ([R]obbe[r])
                else if (c && (c == 'r' || d == 'r' || (len > 3 && str[4] == 'r')))
                    return SQMOD_SKIN_PHIL_ROBBER;
                // [Phi]l
                else if (c == 'i') return SQMOD_SKIN_PHIL;
            }
            // [Pim][p]
            else if (b == 'i' && ((c == 'm') || d == 'p')) return SQMOD_SKIN_PIMP;
            // [Piz]zama[n]
            else if (b == 'i' && ((c == 'z') || d == 'n')) return SQMOD_SKIN_PIZZAMAN;
            // [Pr]ostitute (#1|A)/(#2|B)/(#2|C)/(#2|D)/(#3|D)/(#4|D)
            else if (b == 'r')
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_PROSTITUTE_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_PROSTITUTE_B;
                    case 'c': return SQMOD_SKIN_PROSTITUTE_C;
                    case 'd': return SQMOD_SKIN_PROSTITUTE_D;
                    case '3':
                    case 'e': return SQMOD_SKIN_PROSTITUTE_E;
                    case '4':
                    case 'f': return SQMOD_SKIN_PROSTITUTE_F;
                }
            }
            // [Pu]nk (#1|A)/(#2|B)/(#3|C)
            else if (b == 'u')
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_PUNK_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_PUNK_B;
                    case '3':
                    case 'c': return SQMOD_SKIN_PUNK_C;
                }
            }
        // [R]ich guy, [R]ockstar guy
        case 'r':
            // [Ri]ch guy
            if (b == 'i') return SQMOD_SKIN_RICH_GUY;
            // [Ro]ckstar guy
            else if (b == 'o') return SQMOD_SKIN_ROCKSTAR_GUY;
        // [S]ailor (#1|A)/(#2|B)/(#3|C)
        // [S]hark (#1|A)/(#2|B)
        // [S]hopper (#1|A)/(#2|B)
        // [S]kate guy, [S]kate lady, [S]onny
        // [S]onny guy (#1|A)/(#2|B)/(#3|C)
        // [S]pandEx (#1|A)/(#2|B)
        // [S]panish guy
        // [S]panish lady (#1|A)/(#2|B)/(#3|C)/(#4|D)
        // [S]tore clerk
        // [S]tripper (#1|A)/(#2|B)/(#3|C)
        // [S]wat
        case 's':
            // [Sa]ilor (#1|A)/(#2|B)/(#3|C)
            if (b == 'a')
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_SAILOR_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_SAILOR_B;
                    case '3':
                    case 'c': return SQMOD_SKIN_SAILOR_C;
                }
            }
            // [S]hark (#1|A)/(#2|B)
            else if (b == 'h' && c == 'a')
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_SHARK_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_SHARK_B;
                }
            }
            // [S]hopper (#1|A)/(#2|B)
            else if (b == 'h' && c == 'o')
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_SHOPPER_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_SHOPPER_B;
                }
            }
            // [Sk]ate [g]uy
            else if (b == 'k' && ((c == 'g') || (len > 4 && str[5] == 'g')))
                return SQMOD_SKIN_SKATE_GUY;
            // [Sk]ate [l]ady
            else if (b == 'k' && ((c == 'l') || (len > 4 && str[5] == 'l')))
                return SQMOD_SKIN_SKATE_LADY;
            // [So]nny
            // [So]nny guy (#1|A)/(#2|B)/(#3|C)
            else if (b == 'o')
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_SONNY_GUY_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_SONNY_GUY_B;
                    case '3':
                    case 'c': return SQMOD_SKIN_SONNY_GUY_C;
                }
            }
            else if (b == 'g')
            {
                                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_SONNY_GUY_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_SONNY_GUY_B;
                    case '3':
                    case 'c': return SQMOD_SKIN_SONNY_GUY_C;
                }
            }
            // [Sp]andE[x] (#1|A)/(#2|B)
            else if (b == 'p' && ((c == 'x') || (len > 5 && str[6] == 'x')))
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_SPANDEX_GUY_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_SPANDEX_GUY_B;
                }
            }
            // [Sp]anish [g]uy
            else if (b == 'p' && ((c == 'g') || (len > 6 && str[7] == 'g')))
                return SQMOD_SKIN_SPANISH_GUY;
            // [Sp]anish [l]ady (#1|A)/(#2|B)/(#3|C)/(#4|D)
            else if (b == 'p' && ((c == 'l') || (len > 6 && str[7] == 'l')))
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_SPANISH_LADY_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_SPANISH_LADY_B;
                    case '3':
                    case 'c': return SQMOD_SKIN_SPANISH_LADY_C;
                    case '4':
                    case 'd': return SQMOD_SKIN_SPANISH_LADY_D;
                }
            }
            // [Sto]re clerk
            else if ((b == 't') && (c == 'o')) return SQMOD_SKIN_STORE_CLERK;
            // [Str]ipper (#1|A)/(#2|B)/(#3|C)
            else if ((b == 't') && (c == 'r'))
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_STRIPPER_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_STRIPPER_B;
                    case '3':
                    case 'c': return SQMOD_SKIN_STRIPPER_C;
                }
            }
            // [Sw]at
            else if (b == 'w') return SQMOD_SKIN_SWAT;
        // [T]axi driver (#1|A)/(#1|B)/(#2|C)/(#2|D)
        // [T]hug (#1|A)/(#2|B)
        // [T]ommy Vercetti
        // [T]ourist (#1|A)/(#2|B)
        // [T]ranny
        case 't':
            switch (b)
            {
                // [Ta]xi driver (#1|A)/(#1|B)/(#2|C)/(#2|D)
                case 'a':
                    switch (d)
                    {
                        case '1':
                        case 'a': return SQMOD_SKIN_TAXI_DRIVER_A;
                        case '2':
                        case 'b': return SQMOD_SKIN_TAXI_DRIVER_B;
                        case 'c': return SQMOD_SKIN_TAXI_DRIVER_C;
                        case 'd': return SQMOD_SKIN_TAXI_DRIVER_D;
                    }
                // [Th]ug (#1|A)/(#2|B)
                case 'h':
                    switch (d)
                    {
                        case '1':
                        case 'a': return SQMOD_SKIN_THUG_A;
                        case '5':
                        case 'b': return SQMOD_SKIN_THUG_B;
                    }
                // [To]mmy [V]ercetti
                // [To]urist (#1|A)/(#2|B)
                case 'v': return SQMOD_SKIN_TOMMY_VERCETTI;
                case 'o':
                    if (c == 'm') return SQMOD_SKIN_TOMMY_VERCETTI;
                    else if (c == 'u' && (d == '1' || d == 'a')) return SQMOD_SKIN_TOURIST_A;
                    else if (c == 'u' && (d == '2' || d == 'b')) return SQMOD_SKIN_TOURIST_B;
                case 'r': return SQMOD_SKIN_TRANNY;
            }
        // [U]ndercover cop (#1|A)/(#2|B)/(#3|C)/(#4|D)/(#5|E)/(#6|F)
        case 'u':
            switch (d)
            {
                case '1':
                case 'a': return SQMOD_SKIN_UNDERCOVER_COP_A;
                case '2':
                case 'b': return SQMOD_SKIN_UNDERCOVER_COP_B;
                case '3':
                case 'c': return SQMOD_SKIN_UNDERCOVER_COP_C;
                case '4':
                case 'd': return SQMOD_SKIN_UNDERCOVER_COP_D;
                case '5':
                case 'e': return SQMOD_SKIN_UNDERCOVER_COP_E;
                case '6':
                case 'f': return SQMOD_SKIN_UNDERCOVER_COP_F;
            }
        // [V]ercetti guy (#1|A)/(#2|B)
        case 'v':
            switch (d)
            {
                case '1':
                case 'a': return SQMOD_SKIN_VERCETTI_GUY_A;
                case '2':
                case 'b': return SQMOD_SKIN_VERCETTI_GUY_B;
            }
        // [W]aitress (#1|A)/(#2|B)
        case 'w':
            switch (d)
            {
                case '1':
                case 'a': return SQMOD_SKIN_WAITRESS_A;
                case '2':
                case 'b': return SQMOD_SKIN_WAITRESS_B;
            }
        // Default to unknown
        default: return SQMOD_UNKNOWN;
    }
}

// ------------------------------------------------------------------------------------------------
bool IsSkinValid(SQInt32 id)
{
    return (strcmp(GetSkinName(id), _SC("")) != 0) ? true : false;
}

// ------------------------------------------------------------------------------------------------
const SQChar * GetAutomobileName(SQInt32 id)
{
    switch(id)
    {
        case SQMOD_VEHICLE_LANDSTALKER:         return _SC("Landstalker");
        case SQMOD_VEHICLE_IDAHO:               return _SC("Idaho");
        case SQMOD_VEHICLE_STINGER:             return _SC("Stinger");
        case SQMOD_VEHICLE_LINERUNNER:          return _SC("Linerunner");
        case SQMOD_VEHICLE_PERENNIAL:           return _SC("Perennial");
        case SQMOD_VEHICLE_SENTINEL:            return _SC("Sentinel");
        case SQMOD_VEHICLE_RIO:                 return _SC("Rio");
        case SQMOD_VEHICLE_FIRETRUCK:           return _SC("Firetruck");
        case SQMOD_VEHICLE_TRASHMASTER:         return _SC("Trashmaster");
        case SQMOD_VEHICLE_STRETCH:             return _SC("Stretch");
        case SQMOD_VEHICLE_MANANA:              return _SC("Manana");
        case SQMOD_VEHICLE_INFERNUS:            return _SC("Infernus");
        case SQMOD_VEHICLE_VOODOO:              return _SC("Voodoo");
        case SQMOD_VEHICLE_PONY:                return _SC("Pony");
        case SQMOD_VEHICLE_MULE:                return _SC("Mule");
        case SQMOD_VEHICLE_CHEETAH:             return _SC("Cheetah");
        case SQMOD_VEHICLE_AMBULANCE:           return _SC("Ambulance");
        case SQMOD_VEHICLE_FBIWASHINGTON:       return _SC("FBI Washington");
        case SQMOD_VEHICLE_MOONBEAM:            return _SC("Moonbeam");
        case SQMOD_VEHICLE_ESPERANTO:           return _SC("Esperanto");
        case SQMOD_VEHICLE_TAXI:                return _SC("Taxi");
        case SQMOD_VEHICLE_WASHINGTON:          return _SC("Washington");
        case SQMOD_VEHICLE_BOBCAT:              return _SC("Bobcat");
        case SQMOD_VEHICLE_MRWHOOPEE:           return _SC("Mr. Whoopee");
        case SQMOD_VEHICLE_BFINJECTION:         return _SC("BF Injection");
        case SQMOD_VEHICLE_HUNTER:              return _SC("Hunter");
        case SQMOD_VEHICLE_POLICE:              return _SC("Police");
        case SQMOD_VEHICLE_ENFORCER:            return _SC("Enforcer");
        case SQMOD_VEHICLE_SECURICAR:           return _SC("Securicar");
        case SQMOD_VEHICLE_BANSHEE:             return _SC("Banshee");
        case SQMOD_VEHICLE_PREDATOR:            return _SC("Predator");
        case SQMOD_VEHICLE_BUS:                 return _SC("Bus");
        case SQMOD_VEHICLE_RHINO:               return _SC("Rhino");
        case SQMOD_VEHICLE_BARRACKSOL:          return _SC("Barracks OL");
        case SQMOD_VEHICLE_CUBANHERMES:         return _SC("Cuban Hermes");
        case SQMOD_VEHICLE_HELICOPTER:          return _SC("Helicopter");
        case SQMOD_VEHICLE_ANGEL:               return _SC("Angel");
        case SQMOD_VEHICLE_COACH:               return _SC("Coach");
        case SQMOD_VEHICLE_CABBIE:              return _SC("Cabbie");
        case SQMOD_VEHICLE_STALLION:            return _SC("Stallion");
        case SQMOD_VEHICLE_RUMPO:               return _SC("Rumpo");
        case SQMOD_VEHICLE_RCBANDIT:            return _SC("RC Bandit");
        case SQMOD_VEHICLE_HEARSE:              return _SC("Romero's Hearse");
        case SQMOD_VEHICLE_PACKER:              return _SC("Packer");
        case SQMOD_VEHICLE_SENTINELXS:          return _SC("Sentinel XS");
        case SQMOD_VEHICLE_ADMIRAL:             return _SC("Admiral");
        case SQMOD_VEHICLE_SQUALO:              return _SC("Squalo");
        case SQMOD_VEHICLE_SEASPARROW:          return _SC("Sea Sparrow");
        case SQMOD_VEHICLE_PIZZABOY:            return _SC("Pizza Boy");
        case SQMOD_VEHICLE_GANGBURRITO:         return _SC("Gang Burrito");
        case SQMOD_VEHICLE_AIRTRAIN:            return _SC("Airtrain");
        case SQMOD_VEHICLE_DEADDODO:            return _SC("Deaddodo");
        case SQMOD_VEHICLE_SPEEDER:             return _SC("Speeder");
        case SQMOD_VEHICLE_REEFER:              return _SC("Reefer");
        case SQMOD_VEHICLE_TROPIC:              return _SC("Tropic");
        case SQMOD_VEHICLE_FLATBED:             return _SC("Flatbed");
        case SQMOD_VEHICLE_YANKEE:              return _SC("Yankee");
        case SQMOD_VEHICLE_CADDY:               return _SC("Caddy");
        case SQMOD_VEHICLE_ZEBRACAB:            return _SC("Zebra Cab");
        case SQMOD_VEHICLE_TOPFUN:              return _SC("Top Fun");
        case SQMOD_VEHICLE_SKIMMER:             return _SC("Skimmer");
        case SQMOD_VEHICLE_PCJ600:              return _SC("PCJ-600");
        case SQMOD_VEHICLE_FAGGIO:              return _SC("Faggio");
        case SQMOD_VEHICLE_FREEWAY:             return _SC("Freeway");
        case SQMOD_VEHICLE_RCBARON:             return _SC("RC Baron");
        case SQMOD_VEHICLE_RCRAIDER:            return _SC("RC Raider");
        case SQMOD_VEHICLE_GLENDALE:            return _SC("Glendale");
        case SQMOD_VEHICLE_OCEANIC:             return _SC("Oceanic");
        case SQMOD_VEHICLE_SANCHEZ:             return _SC("Sanchez");
        case SQMOD_VEHICLE_SPARROW:             return _SC("Sparrow");
        case SQMOD_VEHICLE_PATRIOT:             return _SC("Patriot");
        case SQMOD_VEHICLE_LOVEFIST:            return _SC("Love Fist");
        case SQMOD_VEHICLE_COASTGUARD:          return _SC("Coast Guard");
        case SQMOD_VEHICLE_DINGHY:              return _SC("Dinghy");
        case SQMOD_VEHICLE_HERMES:              return _SC("Hermes");
        case SQMOD_VEHICLE_SABRE:               return _SC("Sabre");
        case SQMOD_VEHICLE_SABRETURBO:          return _SC("Sabre Turbo");
        case SQMOD_VEHICLE_PHOENIX:             return _SC("Phoenix");
        case SQMOD_VEHICLE_WALTON:              return _SC("Walton");
        case SQMOD_VEHICLE_REGINA:              return _SC("Regina");
        case SQMOD_VEHICLE_COMET:               return _SC("Comet");
        case SQMOD_VEHICLE_DELUXO:              return _SC("Deluxo");
        case SQMOD_VEHICLE_BURRITO:             return _SC("Burrito");
        case SQMOD_VEHICLE_SPANDEXPRESS:        return _SC("Spand Express");
        case SQMOD_VEHICLE_MARQUIS:             return _SC("Marquis");
        case SQMOD_VEHICLE_BAGGAGEHANDLER:      return _SC("Baggage Handler");
        case SQMOD_VEHICLE_KAUFMANCAB:          return _SC("Kaufman Cab");
        case SQMOD_VEHICLE_MAVERICK:            return _SC("Maverick");
        case SQMOD_VEHICLE_VCNMAVERICK:         return _SC("VCN Maverick");
        case SQMOD_VEHICLE_RANCHER:             return _SC("Rancher");
        case SQMOD_VEHICLE_FBIRANCHER:          return _SC("FBI Rancher");
        case SQMOD_VEHICLE_VIRGO:               return _SC("Virgo");
        case SQMOD_VEHICLE_GREENWOOD:           return _SC("Greenwood");
        case SQMOD_VEHICLE_CUBANJETMAX:         return _SC("Cuban Jetmax");
        case SQMOD_VEHICLE_HOTRINGRACER1:       return _SC("Hotring Racer #1");
        case SQMOD_VEHICLE_SANDKING:            return _SC("Sandking");
        case SQMOD_VEHICLE_BLISTACOMPACT:       return _SC("Blista Compact");
        case SQMOD_VEHICLE_POLICEMAVERICK:      return _SC("Police Maverick");
        case SQMOD_VEHICLE_BOXVILLE:            return _SC("Boxville");
        case SQMOD_VEHICLE_BENSON:              return _SC("Benson");
        case SQMOD_VEHICLE_MESAGRANDE:          return _SC("Mesa Grande");
        case SQMOD_VEHICLE_RCGOBLIN:            return _SC("RC Goblin");
        case SQMOD_VEHICLE_HOTRINGRACER2:       return _SC("Hotring Racer #2");
        case SQMOD_VEHICLE_HOTRINGRACER3:       return _SC("Hotring Racer #3");
        case SQMOD_VEHICLE_BLOODRINGBANGER1:    return _SC("Bloodring Banger #1");
        case SQMOD_VEHICLE_BLOODRINGBANGER2:    return _SC("Bloodring Banger #2");
        case SQMOD_VEHICLE_POLICECHEETAH:       return _SC("Vice Squad Cheetah");
        default:                                return _SC("");
    }
}

SQInt32 GetAutomobileID(const SQChar * name)
{
    // Clone the string into an editable version
    String str(name);
    // Strip non alphanumeric characters from the name
    str.erase(std::remove_if(str.begin(), str.end(), std::not1(std::ptr_fun(::isalnum))), str.end());
    // Convert the string to lowercase
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    // See if we still have a valid name after the cleanup
    if(str.empty()) return SQMOD_UNKNOWN;
    // Grab the actual length of the string
    unsigned len = str.length();
    // Get the most significant characters used to identify a vehicle
    SQChar a = str[0], b = 0, c = 0, d = str[len-1];
    // Look for deeper specifiers
    if(str.length() >= 3)
    {
        c = str[2];
        b = str[1];
    }
    else if(str.length() >= 2)
    {
        b = str[1];
    }
    // Search for a pattern in the name
    switch (a)
    {
        // [A]dmiral
        // [A]irtrain
        // [A]mbulance
        // [A]ngel
        case 'a':
            switch (b)
            {
                // [Ad]miral
                case 'd': return SQMOD_VEHICLE_ADMIRAL;
                // [Ai]rtrain
                case 'i': return SQMOD_VEHICLE_AIRTRAIN;
                // [Am]bulance
                case 'm': return SQMOD_VEHICLE_AMBULANCE;
                // [An]gel
                case 'n': return SQMOD_VEHICLE_ANGEL;
                // Default to unknwon
                default: return SQMOD_UNKNOWN;
            }

        // [B]aggage Handler
        // [B]anshee
        // [B]arracks OL
        // [B]enson
        // [B]F Injection
        // [B]lista Compact
        // [B]loodring Banger #1
        // [B]loodring Banger #2
        // [B]obcat
        // [B]oxville
        // [B]urrito
        // [B]us
        case 'b':
            switch (b)
            {
                // [Ba]ggage Handler
                // [Ba]nshee
                // [Ba]rracks OL
                case 'a':
                    // [Bag]gage Handler
                    if (c == 'g') return SQMOD_VEHICLE_BAGGAGEHANDLER;
                    // [Ban]shee
                    else if (c == 'n') return SQMOD_VEHICLE_BANSHEE;
                    // [Bar]racks OL
                    else if (c == 'r') return SQMOD_VEHICLE_BARRACKS;
                    // Default to unknwon
                    else return SQMOD_UNKNOWN;
                // [Be]nson
                case 'e': return SQMOD_VEHICLE_BENSON;
                // [BF] [I]njection
                case 'f':
                case 'i': return SQMOD_VEHICLE_BFINJECTION;
                // [Bl]ista Compact
                // [Bl]oodring Banger #1
                // [Bl]oodring Banger #2
                case 'l':
                    // [Bli]sta [C]ompact
                    if (b == 'c' || c == 'i') return SQMOD_VEHICLE_BLISTACOMPACT;
                    // [Blo]odring [B]anger (#1|A)
                    else if ((b == 'b' || c == 'o') && (d == '1' || d == 'a')) return SQMOD_VEHICLE_BLOODRINGBANGER1;
                    // [Blo]odring [B]anger (#2|B)
                    else if ((b == 'b' || c == 'o') && (d == '2' || d == 'b')) return SQMOD_VEHICLE_BLOODRINGBANGER2;
                    // Default to unknwon
                    else return SQMOD_UNKNOWN;
                // [Bo]bcat
                // [Bo]xville
                case 'o':
                    // [Bob]cat
                    if (c == 'b') return SQMOD_VEHICLE_BOBCAT;
                    // [Box]ville
                    else if (c == 'x') return SQMOD_VEHICLE_BOXVILLE;
                    // Default to unknwon
                    else return SQMOD_UNKNOWN;
                // [Bu]rrito
                // [Bu]s
                case 'u':
                    // [Bur]rito
                    if (c == 'r') return SQMOD_VEHICLE_BURRITO;
                    // [Bus]
                    else if (c == 's') return SQMOD_VEHICLE_BUS;
                    // Default to unknwon
                    else return SQMOD_UNKNOWN;
                // Default to unknwon
                default: return SQMOD_UNKNOWN;
            }
        // [C]abbie
        // [C]addy
        // [C]heetah
        // [C]oach
        // [C]oast Guard
        // [C]omet
        // [C]uban Hermes
        // [C]uban Jetmax
        case 'c':
            switch (b)
            {
                // [Ca]bbie
                // [Ca]ddy
                case 'a':
                    // [Cab]bie
                    if (c == 'b') return SQMOD_VEHICLE_CABBIE;
                    // [Cad]dy
                    else if (c == 'd') return SQMOD_VEHICLE_CADDY;
                    // Default to unknwon
                    else return SQMOD_UNKNOWN;
                // [Ch]eetah
                case 'h': return SQMOD_VEHICLE_CHEETAH;
                // [Co]ach
                // [Co]ast Guard
                // [Co]met
                case 'o':
                    // [Coa]c[h]
                    if (c == 'a' && d == 'h') return SQMOD_VEHICLE_COACH;
                    // [Coa]s[t] Guar[d]
                    else if (c == 'a' && (d == 't' || d == 'd')) return SQMOD_VEHICLE_COASTGUARD;
                    // [Co]met
                    else if (c == 'm') return SQMOD_VEHICLE_COMET;
                    // Default to unknwon
                    else return SQMOD_UNKNOWN;
                // [Cu]ban Hermes
                // [Cu]ban Jetmax
                case 'u':
                    // [Cub]an [H]erme[s]
                    if ((len > 4 && str[5] == 'h') || (d == 's' || d == 'h')) return SQMOD_VEHICLE_CUBANHERMES;
                    // [Cub]an [J]etma[x]
                    if ((len > 4 && str[5] == 'j') || (d == 'x' || d == 'j')) return SQMOD_VEHICLE_CUBANJETMAX;
                    // Default to unknwon
                    else return SQMOD_UNKNOWN;
                // Default to unknwon
                default: return SQMOD_UNKNOWN;
            }
        // [D]eaddodo
        // [D]eluxo
        // [D]inghy
        case 'd':
            switch (b)
            {
                // [De]addodo
                // [De]luxo
                case 'e':
                    // [Dea]ddodo
                    if (c == 'a') return SQMOD_VEHICLE_DEADDODO;
                    // [Del]uxo
                    else if (c == 'l') return SQMOD_VEHICLE_DELUXO;
                    // Default to unknwon
                    else return SQMOD_UNKNOWN;
                // [Di]nghy
                case 'i': return SQMOD_VEHICLE_DINGHY;
                // Default to unknwon
                default: return SQMOD_UNKNOWN;
            }
        // [E]speranto
        // [E]nforcer
        case 'e':
            // [Es]peranto
            if (b && b == 's') return SQMOD_VEHICLE_ESPERANTO;
            // [En]forcer
            else if (b && b == 'n') return SQMOD_VEHICLE_ENFORCER;
            // Default to unknwon
            else return SQMOD_UNKNOWN;
        // [F]aggio
        // [F]BI Rancher
        // [F]BI Washington
        // [F]iretruck
        // [F]latbed
        // [F]reeway
        case 'f':
            switch (b)
            {
                // [Fa]ggio
                case 'a': return SQMOD_VEHICLE_FAGGIO;
                // [FB]I Rancher
                // [FB]I Washington
                case 'b':
                    // [FB]I [R]anche[r]
                    if ((len > 2 && str[3] == 'r') || d == 'r') return SQMOD_VEHICLE_FBIRANCHER;
                    // [FB]I [W]ashingto[n]
                    else if ((len > 2 && str[3] == 'w') || (d == 'n' || d == 'w')) return SQMOD_VEHICLE_FBIWASHINGTON;
                    // Default to unknwon
                    else return SQMOD_UNKNOWN;
                // [Fi]retruck
                case 'i': return SQMOD_VEHICLE_FIRETRUCK;
                // [Fl]atbed
                case 'l': return SQMOD_VEHICLE_FLATBED;
                // [Fr]eeway
                case 'r': return SQMOD_VEHICLE_FREEWAY;
                // Default to unknwon
                default: return SQMOD_UNKNOWN;
            }
        // [G]ang Burrito
        // [G]lendale
        // [G]reenwood
        case 'g':
            switch (b)
            {
                // [Ga]ng Burrito
                case 'a': return SQMOD_VEHICLE_GANGBURRITO;
                // [Gl]endale
                case 'l': return SQMOD_VEHICLE_GLENDALE;
                // [Gr]eenwood
                case 'r': return SQMOD_VEHICLE_GREENWOOD;
                // Default to unknwon
                default: return SQMOD_UNKNOWN;
            }
        // [H]elicopter
        // [H]ermes
        // [H]otring Racer (#1|A)(#2|B)(#3|C)
        // [H]unter
        case 'h':
            switch (b)
            {
                // [He]licopter
                // [He]rmes
                case 'e':
                    // [Hel]icopter
                    if (c == 'l') return SQMOD_VEHICLE_HELICOPTER;
                    // [Her]mes
                    else if (c == 'r') return SQMOD_VEHICLE_HERMES;
                    // Default to unknwon
                    else return SQMOD_UNKNOWN;
                // [Ho]tring Racer (#1|A)(#2|B)(#3|C)
                case 'o':
                    switch (d)
                    {
                        case '1':
                        case 'a': return SQMOD_VEHICLE_HOTRINGRACER1;
                        case '2':
                        case 'b': return SQMOD_VEHICLE_HOTRINGRACER2;
                        case '3':
                        case 'c': return SQMOD_VEHICLE_HOTRINGRACER3;
                        // Default to unknwon
                        default: return SQMOD_UNKNOWN;
                    }
                // [Hu]nter
                case 'u': return SQMOD_VEHICLE_HUNTER;
                // Default to unknwon
                default: return SQMOD_UNKNOWN;
            }
        // [I]daho
        // [I]nfernus
        case 'i':
            // [Id]aho
            if (b && b == 'd') return SQMOD_VEHICLE_IDAHO;
            // [In]fernus
            else if (b && b == 'n') return SQMOD_VEHICLE_INFERNUS;
            // Default to unknwon
            else return SQMOD_UNKNOWN;
        // [K]aufman Cab
        case 'k': return SQMOD_VEHICLE_KAUFMANCAB;
        // [L]andstalker
        // [L]inerunner
        // [L]ove Fist
        case 'l':
            switch (b)
            {
                // [La]ndstalker
                case 'a': return SQMOD_VEHICLE_LANDSTALKER;
                // [Li]nerunner
                case 'i': return SQMOD_VEHICLE_LINERUNNER;
                // [Lo]ve Fist
                case 'o': return SQMOD_VEHICLE_LOVEFIST;
                // Default to unknwon
                default: return SQMOD_UNKNOWN;

            }
        // [M]anana
        // [M]arquis
        // [M]averick
        // [M]esa Grande
        // [M]oonbeam
        // [M]r. Whoopee
        // [M]ule
        case 'm':
            switch (b)
            {
                // [Ma]nana
                // [Ma]rquis
                // [Ma]verick
                case 'a':
                    // [Man]ana
                    if (c == 'n') return SQMOD_VEHICLE_MANANA;
                    // [Mar]quis
                    else if (c == 'r') return SQMOD_VEHICLE_MARQUIS;
                    // [Mav]erick
                    else if (c == 'v') return SQMOD_VEHICLE_MAVERICK;
                    // Default to unknwon
                    else return SQMOD_UNKNOWN;
                // [Me]sa Grande
                case 'e': return SQMOD_VEHICLE_MESAGRANDE;
                // [Mo]onbeam
                case 'o': return SQMOD_VEHICLE_MOONBEAM;
                // [Mr]. Whoopee
                case 'r': return SQMOD_VEHICLE_MRWHOOPEE;
                // [Mu]le
                case 'u': return SQMOD_VEHICLE_MULE;
                // Default to unknwon
                default: return SQMOD_UNKNOWN;
            }
        // [O]ceanic
        case 'o': return SQMOD_VEHICLE_OCEANIC;
        // [P]acker
        // [P]atriot
        // [P]CJ-600
        // [P]erennial
        // [P]hoenix
        // [P]izza Boy
        // [P]olice
        // [P]olice Maverick
        // [P]ony
        // [P]redator
        case 'p':
            switch (b)
            {
                // [Pa]cker
                // [Pa]triot
                case 'a':
                    // [Pac]ker
                    if (c == 'c') return SQMOD_VEHICLE_PACKER;
                    // [Pat]riot
                    else if (c == 't') return SQMOD_VEHICLE_PATRIOT;
                    // Default to unknwon
                    else return SQMOD_UNKNOWN;
                // [PC]J-600
                case 'c': return SQMOD_VEHICLE_PCJ600;
                // [Pe]rennial
                case 'e': return SQMOD_VEHICLE_PERENNIAL;
                // [Ph]oenix
                case 'h': return SQMOD_VEHICLE_PHOENIX;
                // [Pi]zza Boy
                case 'i':
                // [Po]lice
                // [Po]lice Maverick
                // [Po]ny
                case 'o':
                    // [Po]lice
                    if (d == 'e') return SQMOD_VEHICLE_POLICE;
                    // [Po]lice Maverick
                    else if ((len > 5 && str[6] == 'm') || (d == 'k' || d == 'm')) return SQMOD_VEHICLE_POLICEMAVERICK;
                    // [Po]ny
                    else if (c == 'n') return SQMOD_VEHICLE_PONY;
                    // Default to unknwon
                    else return SQMOD_UNKNOWN;
                // [Pr]edator
                case 'r': return SQMOD_VEHICLE_PREDATOR;
                // Default to unknwon
                default: return SQMOD_UNKNOWN;
            }
        // [R]ancher
        // [R]C Bandit
        // [R]C Baron
        // [R]C Goblin
        // [R]C Raider
        // [R]eefer
        // [R]egina
        // [R]hino
        // [R]io
        // [R]omero's Hearse
        // [R]umpo
        case 'r':
            switch (b)
            {
                // [Ra]ncher
                case 'a': return SQMOD_VEHICLE_RANCHER;
                // [RC] Bandit
                // [RC] Baron
                // [RC] Goblin
                // [RC] Raider
                case 'c':
                    // [RC] [B]andi[t]
                    if (c == 'b' && d == 't') return SQMOD_VEHICLE_RCBANDIT;
                    // [RC] [B]aro[n]
                    else if (c == 'b' && d == 'n') return SQMOD_VEHICLE_RCBARON;
                    // [RC] [G]oblin
                    else if (c == 'g') return SQMOD_VEHICLE_RCGOBLIN;
                    // [RC] [R]aide[r]
                    else if (c == 'r' || d == 'r') return SQMOD_VEHICLE_RCRAIDER;
                    // Default to unknwon
                    else return SQMOD_UNKNOWN;
                // [Re]efer
                // [Re]gina
                case 'e':
                    // [Ree]fer
                    if (c == 'e' || d == 'r') return SQMOD_VEHICLE_REEFER;
                    // [Reg]ina
                    else if (c == 'g' || d == 'a') return SQMOD_VEHICLE_REGINA;
                    // Default to unknwon
                    else return SQMOD_UNKNOWN;
                // [Rh]ino
                case 'h': return SQMOD_VEHICLE_RHINO;
                // [Ri]o
                case 'i': return SQMOD_VEHICLE_RIO;
                // [Ro]mero's Hearse
                case 'o': return SQMOD_VEHICLE_HEARSE;
                // [Ru]mpo
                case 'u': return SQMOD_VEHICLE_RUMPO;
                // Default to unknwon
                default: return SQMOD_UNKNOWN;
            }
        // [S]abre
        // [S]abre Turbo
        // [S]anchez
        // [S]andking
        // [S]ea Sparrow
        // [S]ecuricar
        // [S]entinel
        // [S]entinel XS
        // [S]kimmer
        // [S]pand Express
        // [S]parrow
        // [S]peeder
        // [S]qualo
        // [S]tallion
        // [S]tinger
        // [S]tretch
        case 's':
            switch (b)
            {
                // [Sa]bre
                // [Sa]bre Turbo
                // [Sa]nchez
                // [Sa]ndking
                case 'a':
                    // [Sab]r[e]
                    if (c == 'b' && d == 'e') return SQMOD_VEHICLE_SABRE;
                    // [Sab]re [T]urb[o]
                    else if ((c == 'b' && d == 'o') || (len > 4 && str[5 ]== 't')) return SQMOD_VEHICLE_SABRETURBO;
                    // [Sa]n[c]he[z]
                    else if (d == 'c' || d == 'z') return SQMOD_VEHICLE_SANCHEZ;
                    // [Sa]n[d]kin[g]
                    else if (d == 'd' || d == 'g') return SQMOD_VEHICLE_SANDKING;
                    // Default to unknwon
                    else return SQMOD_UNKNOWN;
                // [Se]a Sparrow
                // [Se]curicar
                // [Se]ntinel
                // [Se]ntinel XS
                case 'e':
                    // [Sea] Sparro[w]
                    if (c == 'e' || d == 'w') return SQMOD_VEHICLE_SEASPARROW;
                    // [Sec]urica[r]
                    else if (c == 'c' || d == 'r') return SQMOD_VEHICLE_SECURICAR;
                    // [Sen]tine[l]
                    else if (c == 'n' && d == 'l') return SQMOD_VEHICLE_SENTINEL;
                    // [Sen]tinel X[S]
                    else if (c == 'n' && d == 's') return SQMOD_VEHICLE_SENTINELXS;
                    // Default to unknwon
                    else return SQMOD_UNKNOWN;
                // [Sk]immer
                case 'k': return SQMOD_VEHICLE_SKIMMER;
                // [Sp]and Express
                // [Sp]arrow
                // [Sp]eeder
                case 'p':
                    // [Spa]nd [E]xpres[s]
                    if (c == 'a' || ((len > 4 && str[5] == 'e') || d == 's')) return SQMOD_VEHICLE_SPANDEXPRESS;
                    // [Spa]rro[w]
                    else if (d == 'w' && (c == 'a' && d == 'w')) return SQMOD_VEHICLE_SPARROW;
                    // [Spe]ede[r]
                    else if (c == 'e' || d == 'r') return SQMOD_VEHICLE_SPEEDER;
                    // Default to unknwon
                    else return SQMOD_UNKNOWN;
                // [Sq]ualo
                case 'q': return SQMOD_VEHICLE_SQUALO;
                // [St]allion
                // [St]inger
                // [St]retch
                case 't':
                    // [Sta]llion
                    if (c == 'a') return SQMOD_VEHICLE_STALLION;
                    // [Sti]nger
                    if (c == 'i') return SQMOD_VEHICLE_STINGER;
                    // [Str]etch
                    if (c == 'r') return SQMOD_VEHICLE_STRETCH;
                    // Default to unknwon
                    else return SQMOD_UNKNOWN;
                // Default to unknwon
                default: return SQMOD_UNKNOWN;
            }
        // [T]axi
        // [T]op Fun
        // [T]rashmaster
        // [T]ropic
        case 't':
            switch (b)
            {
                // [Ta]xi
                case 'a': return SQMOD_VEHICLE_TAXI;
                // [To]p Fun
                case 'o': return SQMOD_VEHICLE_TOPFUN;
                // [Tr]ashmaster
                // [Tr]opic
                case 'r':
                    // [Tr]ashmaster
                    if (c == 'a') return SQMOD_VEHICLE_TRASHMASTER;
                    // [Tr]opic
                    if (c == 'o') return SQMOD_VEHICLE_TROPIC;
                    // Default to unknwon
                    else return SQMOD_UNKNOWN;
                // Default to unknwon
                default: return SQMOD_UNKNOWN;
            }
        // [V]CN Maverick
        // [V]ice Squad Cheetah
        // [V]irgo
        // [V]oodoo
        case 'v':
            switch (b)
            {
                // [VC]N Maverick
                case 'c': return SQMOD_VEHICLE_VCNMAVERICK;
                // [Vi]ce Squad Cheetah
                // [Vi]rgo
                case 'i':
                    // [Vic]e Squad Cheetah
                    if (c == 'a') return SQMOD_VEHICLE_VICECHEE;
                    // [Vir]go
                    if (c == 'o') return SQMOD_VEHICLE_VIRGO;
                    // Default to unknwon
                    else return SQMOD_UNKNOWN;
                // [Vo]odoo
                case 'o': return SQMOD_VEHICLE_VOODOO;
                // Default to unknwon
                default: return SQMOD_UNKNOWN;
            }
        // [W]alton
        // [W]ashington
        case 'w':
            // [Wa]lton
            if (b == 'a' && c == 'l') return SQMOD_VEHICLE_WALTON;
            // [Wa]shington
            else if (b == 'a' && c == 's') return SQMOD_VEHICLE_WASHINGTON;
            // Default to unknwon
            else return SQMOD_UNKNOWN;
        // [Y]ankee
        case 'y': return SQMOD_VEHICLE_YANKEE;
        // [Z]ebra Cab
        case 'z': return SQMOD_VEHICLE_ZEBRACAB;
        // Default to unknwon
        default: return SQMOD_UNKNOWN;
    }
}

bool IsAutomobileValid(SQInt32 id)
{
    return (strcmp(GetAutomobileName(id), _SC("")) != 0) ? true : false;
}

// ------------------------------------------------------------------------------------------------
const SQChar * GetWeaponName(SQInt32 id)
{
    switch (id)
    {
        case SQMOD_WEAPON_UNARMED:          return _SC("Unarmed");
        case SQMOD_WEAPON_BRASSKNUCKLES:    return _SC("Brass Knuckles");
        case SQMOD_WEAPON_SCREWDRIVER:      return _SC("Screwdriver");
        case SQMOD_WEAPON_GOLFCLUB:         return _SC("Golf Club");
        case SQMOD_WEAPON_NIGHTSTICK:       return _SC("Nightstick");
        case SQMOD_WEAPON_KNIFE:            return _SC("Knife");
        case SQMOD_WEAPON_BASEBALLBAT:      return _SC("Baseball Bat");
        case SQMOD_WEAPON_HAMMER:           return _SC("Hammer");
        case SQMOD_WEAPON_MEATCLEAVER:      return _SC("Meat Cleaver");
        case SQMOD_WEAPON_MACHETE:          return _SC("Machete");
        case SQMOD_WEAPON_KATANA:           return _SC("Katana");
        case SQMOD_WEAPON_CHAINSAW:         return _SC("Chainsaw");
        case SQMOD_WEAPON_GRENADE:          return _SC("Grenade");
        case SQMOD_WEAPON_REMOTE:           return _SC("Remote Detonation Grenade");
        case SQMOD_WEAPON_TEARGAS:          return _SC("Tear Gas");
        case SQMOD_WEAPON_MOLOTOV:          return _SC("Molotov Cocktails");
        case SQMOD_WEAPON_ROCKET:           return _SC("Rocket");
        case SQMOD_WEAPON_COLT45:           return _SC("Colt .45");
        case SQMOD_WEAPON_PYTHON:           return _SC("Python");
        case SQMOD_WEAPON_SHOTGUN:          return _SC("Pump-Action Shotgun");
        case SQMOD_WEAPON_SPAS12:           return _SC("SPAS-12 Shotgun");
        case SQMOD_WEAPON_STUBBY:           return _SC("Stubby Shotgun");
        case SQMOD_WEAPON_TEC9:             return _SC("TEC-9");
        case SQMOD_WEAPON_UZI:              return _SC("Uzi");
        case SQMOD_WEAPON_INGRAM:           return _SC("Silenced Ingram");
        case SQMOD_WEAPON_MP5:              return _SC("MP5");
        case SQMOD_WEAPON_M4:               return _SC("M4");
        case SQMOD_WEAPON_RUGER:            return _SC("Ruger");
        case SQMOD_WEAPON_SNIPER:           return _SC("Sniper Rifle");
        case SQMOD_WEAPON_LASERSCOPE:       return _SC("Laserscope Sniper Rifle");
        case SQMOD_WEAPON_ROCKETLAUNCHER:   return _SC("Rocket Launcher");
        case SQMOD_WEAPON_FLAMETHROWER:     return _SC("Flamethrower");
        case SQMOD_WEAPON_M60:              return _SC("M60");
        case SQMOD_WEAPON_MINIGUN:          return _SC("Minigun");
        case SQMOD_WEAPON_BOMB:             return _SC("Bomb");
        case SQMOD_WEAPON_HELICANNON:       return _SC("Helicannon");
        case SQMOD_WEAPON_CAMERA:           return _SC("Camera");
        case SQMOD_WEAPON_VEHICLE:          return _SC("Vehicle");
        case SQMOD_WEAPON_EXPLOSION1:       return _SC("Explosion");
        case SQMOD_WEAPON_DRIVEBY:          return _SC("Driveby");
        case SQMOD_WEAPON_DROWNED:          return _SC("Drowned");
        case SQMOD_WEAPON_FALL:             return _SC("Fall");
        case SQMOD_WEAPON_EXPLOSION2:       return _SC("Explosion");
        case SQMOD_WEAPON_SUICIDE:          return _SC("Suicide");
        default:                            return _SC("");
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 GetWeaponID(const SQChar * name)
{
    // Clone the string into an editable version
    String str(name);
    // Strip non alphanumeric characters from the name
    str.erase(std::remove_if(str.begin(), str.end(), std::not1(std::ptr_fun(::isalnum))), str.end());
    // Convert the string to lowercase
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    // See if we still have a valid name after the cleanup
    if(str.length() < 1) return SQMOD_UNKNOWN;
    // Grab the actual length of the string
    unsigned len = str.length();
    // Get the most significant characters used to identify a weapon
    SQChar a = str[0], b = 0, c = 0, d = str[len-1];
    // Look for deeper specifiers
    if(str.length() >= 3)
    {
        c = str[2];
        b = str[1];
    }
    else if(str.length() >= 2)
    {
        b = str[1];
    }
    // Search for a pattern in the name
    switch(a)
    {
        // [B]aseball Bat
        // [B]omb
        // [B]rass Knuckles
        case 'b':
            // [Ba]seball Bat
            if (b == 'a') return SQMOD_WEAPON_BASEBALLBAT;
            // [Bo]mb
            else if (b == 'o') return SQMOD_WEAPON_BOMB;
            // [Br]ass Knuckles
            else if (b == 'r') return SQMOD_WEAPON_BRASSKNUCKLES;
            // Default to unknwon
            else return SQMOD_UNKNOWN;
        // [C]amera
        // [C]hainsaw
        // [C]olt .45
        case 'c':
            // [Ca]mera
            if (b == 'a') return SQMOD_WEAPON_CAMERA;
            // [Ch]ainsaw
            else if (b == 'h') return SQMOD_WEAPON_CHAINSAW;
            // [Co]lt .45
            else if (b == 'o') return SQMOD_WEAPON_COLT45;
            // Default to unknwon
            else return SQMOD_UNKNOWN;
        // [D]riveby
        // [D]rowned
        case 'd':
            // [Dr]iveby
            if (b == 'r' && (c == 'i' || d == 'y')) return SQMOD_WEAPON_DRIVEBY;
            // [Dr]owned
            else if (b == 'r' && (c == 'o' || d == 'd')) return SQMOD_WEAPON_DROWNED;
            // Default to unknwon
            else return SQMOD_UNKNOWN;
        // [E]xplosion
        case 'e': return SQMOD_WEAPON_EXPLOSION2;
        // [F]all
        // [F]lamethrower
        case 'f':
            // [Fa]ll
            if (b == 'a') return SQMOD_WEAPON_FALL;
            // [Fl]amethrower
            else if (b == 'l') return SQMOD_WEAPON_FLAMETHROWER;
            // Default to unknwon
            else return SQMOD_UNKNOWN;
        // [G]olf Club
        // [G]renade
        case 'g':
            // [Go]lf Club
            if (b == 'o') return SQMOD_WEAPON_GOLFCLUB;
            // [Gr]enade
            else if (b == 'r') return SQMOD_WEAPON_GRENADE;
            // Default to unknwon
            else return SQMOD_UNKNOWN;
        // [H]ammer
        // [H]elicannon
        case 'h':
            // [Ha]mmer
            if (b == 'a') return SQMOD_WEAPON_HAMMER;
            // [He]licannon
            else if (b == 'e') return SQMOD_WEAPON_HELICANNON;
            // Default to unknwon
            else return SQMOD_UNKNOWN;
        // [K]atana
        // [K]nife
        case 'k':
            // [Ka]tana
            if (b == 'a') return SQMOD_WEAPON_KATANA;
            // [Kn]ife
            else if (b == 'n') return SQMOD_WEAPON_KNIFE;
            // Default to unknwon
            else return SQMOD_UNKNOWN;
        // [L]aserscope Sniper Rifle
        case 'l': return SQMOD_WEAPON_LASERSCOPE;
        // [M]4
        // [M]60
        // [M]achete
        // [M]eat Cleaver
        // [M]inigun
        // [M]olotov Cocktails
        // [M]P5
        case 'm':
            // [M4]
            if (b == '4') return SQMOD_WEAPON_M4;
            // [M6]0
            else if (b == '6') return SQMOD_WEAPON_M60;
            // [Ma]chete
            else if (b == 'a') return SQMOD_WEAPON_MACHETE;
            // [Me]at Cleaver
            else if (b == 'e') return SQMOD_WEAPON_MEATCLEAVER;
            // [Mi]nigun
            else if (b == 'i') return SQMOD_WEAPON_MINIGUN;
            // [Mo]lotov Cocktails
            else if (b == 'o') return SQMOD_WEAPON_MOLOTOV;
            // [MP]5
            else if (b == 'p') return SQMOD_WEAPON_MP5;
            // Default to unknwon
            else return SQMOD_UNKNOWN;
        // [N]ightstick
        case 'n': return SQMOD_WEAPON_NIGHTSTICK;
        // [P]ump-Action Shotgun
        // [P]ython
        case 'p':
            // [Pu]mp-Action Shotgun
            if (b == 'u') return SQMOD_WEAPON_SHOTGUN;
            // [Py]thon
            else if (b == 'y') return SQMOD_WEAPON_PYTHON;
            // Default to unknwon
            else return SQMOD_UNKNOWN;
        // [R]emote Detonation Grenade
        // [R]ocket
        // [R]ocket Launcher
        // [R]uger
        case 'r':
            // [Re]mote Detonation Grenade
            if (b == 'e') return SQMOD_WEAPON_REMOTE;
            // [Ro]cke[t]
            else if (b == 'o' && d == 't') return SQMOD_WEAPON_ROCKET;
            // [Ro]cket [L]aunche[r]
            else if (b == 'o' && (d == 'r' || d == 'l' || (len > 5 && str[6] == 'l'))) return SQMOD_WEAPON_ROCKETLAUNCHER;
            // [Ru]ger
            else if (b == 'u') return SQMOD_WEAPON_RUGER;
            // Default to unknwon
            else return SQMOD_UNKNOWN;
        // [S]crewdriver
        // [S]ilenced Ingram
        // [S]niper Rifle
        // [S]PAS-12 Shotgun
        // [S]tubby Shotgun
        // [S]uicide
        case 's':
            // [Sc]rewdriver
            if (b == 'c') return SQMOD_WEAPON_SCREWDRIVER;
            // [Si]lenced Ingram
            else if (b == 'i') return SQMOD_WEAPON_INGRAM;
            // [Sn]iper Rifle
            else if (b == 'n') return SQMOD_WEAPON_SNIPER;
            // [SP]AS-12 Shotgun
            else if (b == 'p') return SQMOD_WEAPON_SPAS12;
            // [St]ubby Shotgun
            else if (b == 't') return SQMOD_WEAPON_STUBBY;
            // [Su]icide
            else if (b == 'u') return SQMOD_WEAPON_SUICIDE;
            // Default to unknwon
            else return SQMOD_UNKNOWN;
        // [T]ear Gas
        // [T]EC-9
        case 't':
            // [Tea]r Ga[s]
            if (b == 'e' && (c == 'a' || d == 's')) return SQMOD_WEAPON_TEARGAS;
            // [TEC]-[9]
            else if (b == 'e' && (c == 'c' || d == '9')) return SQMOD_WEAPON_TEC9;
            // Default to unknwon
            else return SQMOD_UNKNOWN;
        // [U]narmed
        // [U]zi
        case 'u':
            // [Un]armed
            if (b == 'n') return SQMOD_WEAPON_UNARMED;
            // [Uz]i
            else if (b == 'z') return SQMOD_WEAPON_UZI;
            // Default to unknwon
            else return SQMOD_UNKNOWN;
        // [V]ehicle
        case 'v': return SQMOD_WEAPON_VEHICLE;
        // Default to unknwon
        default: return SQMOD_UNKNOWN;
    }
}

// ------------------------------------------------------------------------------------------------
bool IsWeaponValid(SQInt32 id)
{
    return (strcmp(GetWeaponName(id), _SC("")) != 0) ? true : false;
}

// ------------------------------------------------------------------------------------------------
bool IsWeaponNatural(SQInt32 id)
{
    switch (id)
    {
        case SQMOD_WEAPON_VEHICLE:
        case SQMOD_WEAPON_DRIVEBY:
        case SQMOD_WEAPON_DROWNED:
        case SQMOD_WEAPON_FALL:
        case SQMOD_WEAPON_EXPLOSION2:
        case SQMOD_WEAPON_SUICIDE: return true;
        default: return false;
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 WeaponToModel(SQInt32 id)
{
    switch (id)
    {
        case SQMOD_WEAPON_UNARMED:          return 293;
        case SQMOD_WEAPON_BRASSKNUCKLES:    return 259;
        case SQMOD_WEAPON_SCREWDRIVER:      return 260;
        case SQMOD_WEAPON_GOLFCLUB:         return 261;
        case SQMOD_WEAPON_NIGHTSTICK:       return 262;
        case SQMOD_WEAPON_KNIFE:            return 263;
        case SQMOD_WEAPON_BASEBALLBAT:      return 264;
        case SQMOD_WEAPON_HAMMER:           return 265;
        case SQMOD_WEAPON_MEATCLEAVER:      return 266;
        case SQMOD_WEAPON_MACHETE:          return 267;
        case SQMOD_WEAPON_KATANA:           return 268;
        case SQMOD_WEAPON_CHAINSAW:         return 269;
        case SQMOD_WEAPON_GRENADE:          return 270;
        case SQMOD_WEAPON_REMOTE:           return 291;
        case SQMOD_WEAPON_TEARGAS:          return 271;
        case SQMOD_WEAPON_MOLOTOV:          return 272;
        case SQMOD_WEAPON_ROCKET:           return 273;
        case SQMOD_WEAPON_COLT45:           return 274;
        case SQMOD_WEAPON_PYTHON:           return 275;
        case SQMOD_WEAPON_SHOTGUN:          return 277;
        case SQMOD_WEAPON_SPAS12:           return 278;
        case SQMOD_WEAPON_STUBBY:           return 279;
        case SQMOD_WEAPON_TEC9:             return 281;
        case SQMOD_WEAPON_UZI:              return 282;
        case SQMOD_WEAPON_INGRAM:           return 283;
        case SQMOD_WEAPON_MP5:              return 284;
        case SQMOD_WEAPON_M4:               return 280;
        case SQMOD_WEAPON_RUGER:            return 276;
        case SQMOD_WEAPON_SNIPER:           return 285;
        case SQMOD_WEAPON_LASERSCOPE:       return 286;
        case SQMOD_WEAPON_ROCKETLAUNCHER:   return 287;
        case SQMOD_WEAPON_FLAMETHROWER:     return 288;
        case SQMOD_WEAPON_M60:              return 289;
        case SQMOD_WEAPON_MINIGUN:          return 290;
        case SQMOD_WEAPON_BOMB:             return SQMOD_UNKNOWN;
        case SQMOD_WEAPON_HELICANNON:       return 294;
        case SQMOD_WEAPON_CAMERA:           return 292;
        case SQMOD_WEAPON_VEHICLE:          return SQMOD_UNKNOWN;
        case SQMOD_WEAPON_EXPLOSION1:       return SQMOD_UNKNOWN;
        case SQMOD_WEAPON_DRIVEBY:          return SQMOD_UNKNOWN;
        case SQMOD_WEAPON_DROWNED:          return SQMOD_UNKNOWN;
        case SQMOD_WEAPON_FALL:             return SQMOD_UNKNOWN;
        case SQMOD_WEAPON_EXPLOSION2:       return SQMOD_UNKNOWN;
        case SQMOD_WEAPON_SUICIDE:          return SQMOD_UNKNOWN;
        default:                            return SQMOD_UNKNOWN;
    }
}

bool Register_Misc(HSQUIRRELVM vm)
{
    // Output debugging information
    LogDbg("Beginning registration of <Miscellaneous> API");
    // Attempt to register the specified API
    Sqrat::RootTable(vm)
    .Func(_SC("GetKeyCodeName"), &GetKeyCodeName)
    .Func(_SC("GetModelName"), &GetModelName)
    .Func(_SC("IsModelWeapon"), &IsModelWeapon)
    .Func(_SC("IsModelActuallyWeapon"), &IsModelActuallyWeapon)
    .Func(_SC("IsModelValid"), &IsModelValid)
    .Func(_SC("GetSkinName"), &GetSkinName)
    .Func(_SC("GetSkinID"), &GetSkinID)
    .Func(_SC("IsSkinValid"), &IsSkinValid)
    .Func(_SC("GetAutomobileName"), &GetAutomobileName)
    .Func(_SC("GetAutomobileID"), &GetAutomobileID)
    .Func(_SC("IsAutomobileValid"), &IsAutomobileValid)
    .Func(_SC("GetWeaponName"), &GetWeaponName)
    .Func(_SC("GetWeaponID"), &GetWeaponID)
    .Func(_SC("IsWeaponValid"), &IsWeaponValid)
    .Func(_SC("IsWeaponNatural"), &IsWeaponNatural)
    .Func(_SC("WeaponToModel"), &WeaponToModel);

    /* END REGISTRATION STATEMENT */ ;

    // Output debugging information
    LogDbg("Registration of <Miscellaneous> API was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
