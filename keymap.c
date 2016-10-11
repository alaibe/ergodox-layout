/* * alyi's ErgoDox EZ layoutreadme.md file!  */

#include <stdarg.h>
#include "ergodox.h"
#include "led.h"
#include "debug.h"
#include "action_layer.h"
#include "action_util.h"
#include "mousekey.h"
#include "timer.h"
#include "eeconfig.h"
#include "wait.h"
#include "version.h"

/* Layers */
#define BASE 0
#define GUI_MOVE 1

/* Macros */
enum {
  // APP
  AP_ITERM,
  AP_SAFARI,
  AP_MESSAGE,
  AP_FINDER,
  AP_SPOTIFY,
  AP_SLEEP,
  AP_SEQUEL,
  AP_SLACK,

  NP,

  // Tap dance
  KF_0,
  KF_1,
  KF_2,
  KF_3,
  KF_4,
  KF_5,
  KF_6,
  KF_7,
  KF_8,
  KF_9,
  KF_QUOT,
  KF_COMM,
  KF_DOT,
  KF_SLSH,
  KF_MINS,
  KF_EQL,

  CT_LBP,
  CT_RBP,
  CT_CLN,
  CT_IRC_MUTT,
  CT_PROJ,
  CT_GIT_CLOSE,
  CT_TMUX_ZOOM
};
/* Fn keys */

/* States & timers */

uint16_t gui_timer = 0;

uint16_t kf_timers[12];

#if KEYLOGGER_ENABLE
# ifdef AUTOLOG_ENABLE
bool log_enable = true;
# else
bool log_enable = false;
# endif
#endif

/* The Keymap */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Base Layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * |         ~ |   1  |   2  |   3  |   4  |   5  | C-n  |           | C-p  |   6  |   7  |   8  |   9  |  0   |           |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |      C-c  |   Q  |   W  | D/SFT|   F  |   K  |   (  |           |  )   |   J  |   U  | R/SFT|   L  |  '   | \         |
 * |-----------+------+------+------+------+------|   [  |           |  ]   |------+------+------+------+------+-----------|
 * |  Next/Prev|   A  | S/ALT| E/CTL| T/GUI| G/gui|------|           |------|   Y  | N/GUI| I/CTL| O/ALT|  H   | = /       |
 * |-----------+------+------+------+------+------|   {  |           |  }   |------+------+------+------+------+-----------|
 * | Play/Pause|   Z  |   X  |   C  |   V  |   B  |      |           |      |   P  |   M  |   ,  |   .  |  :   | KC_VOLU   |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *     | cut   | copy |paste | C-k  |  /   |                                       |   -  | C-j  |      |      |KC_VOLD|
 *     `-----------------------------------'                                       `-----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         | esc  |proj  |           | tmux | Del  |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      |mail  |           |      |      |      |
 *                                  |Backsp| tab  |------|           |------| Enter| Space|
 *                                  |      |      | git  |           |      |      |      |
 *                                  `--------------------'           `--------------------'
 */
[BASE] = KEYMAP(
// left hand
 KC_GRAVE         ,TD(KF_1)   ,TD(KF_2)   ,TD(KF_3)   ,TD(KF_4)         ,TD(KF_5),LCTL(KC_N)
,LCTL(KC_C)       ,KC_Q       ,KC_W       ,SFT_T(KC_D),KC_F             ,KC_K    ,TD(CT_LBP)
,M(NP)            ,KC_A       ,ALT_T(KC_S),CTL_T(KC_E),GUI_T(KC_T)      ,LT(GUI_MOVE, KC_G)
,KC_MPLY          ,KC_Z       ,KC_X       ,KC_C       ,KC_V             ,KC_B    ,KC_LCBR
,LGUI(KC_X)       ,LGUI(KC_C) ,LGUI(KC_V) ,LCTL(KC_K) ,TD(KF_SLSH)

,KC_ESC ,TD(CT_PROJ)
,TD(CT_IRC_MUTT)
,KC_BSPC,KC_TAB,TD(CT_GIT_CLOSE)


// right hand
,LCTL(KC_P)        ,TD(KF_6)   ,TD(KF_7)        ,TD(KF_8)   ,TD(KF_9)        ,TD(KF_0)   ,KC_NO
,TD(CT_RBP)        ,KC_J       ,KC_U            ,SFT_T(KC_R),KC_L            ,TD(KF_QUOT),KC_BSLS
,KC_Y              ,GUI_T(KC_N),CTL_T(KC_I)     ,ALT_T(KC_O),KC_H            ,TD(KF_EQL)
,KC_RCBR           ,KC_P       ,KC_M            ,TD(KF_COMM),TD(KF_DOT)      ,TD(CT_CLN) ,KC_VOLU
,TD(KF_MINS)       ,LCTL(KC_J) ,KC_NO           ,KC_NO      ,KC_VOLD

,TD(CT_TMUX_ZOOM),KC_DEL
,KC_LEAD
,KC_NO   ,KC_ENT ,KC_SPC
    ),
/* Keymap 1: Gui and move
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * |           |      |      |      |      |      |      |           | sleep|iterm |slack |safari|finder|spoti | message   |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |      |           |      |      | home | pgdn | pgup |  end | sequel    |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |      |      |      |      | base |------|           |------|      | left | down | top  |right |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |      |           |      |      | wleft| wdn  | wup  |wrgt  |           |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *      |      |      |      |      |      |                                       |      |      |      |      |      |
k*      `----------------------------------'                                       `----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         |      |      |           |      |      |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      |      |           |      |Left  |Right |
 *                                  |      |      |------|           |------| Click| Click|
 *                                  |      |      |      |           |      |      |      |
 *                                  `--------------------'           `--------------------'
 */
[GUI_MOVE] = KEYMAP(
// left hand
 KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO     ,KC_NO
,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO     ,KC_NO
,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_TRNS
,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO     ,KC_NO
,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO

,KC_NO ,KC_NO
,KC_NO
,LGUI(KC_SPC),KC_ENT,KC_TRNS

// right hand
,M(AP_SLEEP),M(AP_ITERM),M(AP_SLACK)   ,M(AP_SAFARI),M(AP_FINDER),M(AP_SPOTIFY) ,M(AP_MESSAGE)
,KC_NO      ,KC_NO      ,KC_HOME       ,KC_PGDN     ,KC_PGUP     ,KC_END        ,M(AP_SEQUEL)
,KC_TRNS    ,KC_LEFT    ,KC_DOWN       ,KC_UP       ,KC_RGHT     ,M(AP_FINDER)
,KC_NO      ,KC_NO      ,LALT(KC_LEFT) ,KC_WH_D     ,KC_WH_U     ,LALT(KC_RGHT) ,KC_NO
,KC_NO      ,KC_NO      ,KC_NO         ,KC_NO       ,KC_NO

,KC_NO,KC_NO
,KC_NO
,KC_NO,KC_BTN1 ,KC_BTN2
    ),
};

#define TAP_ONCE(code)  \
  register_code (code); \
  unregister_code (code)


const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  switch(id) {
  case NP:
    if (record->event.pressed) {
      if (keyboard_report->mods & MOD_BIT(KC_LSFT) ||
          ((get_oneshot_mods() & MOD_BIT(KC_LSFT)) && !has_oneshot_mods_timed_out())) {
        int oneshot = ((get_oneshot_mods() & MOD_BIT(KC_LSFT)) && !has_oneshot_mods_timed_out());

        if (oneshot)
          clear_oneshot_mods ();
        unregister_code (KC_LSFT);

        register_code (KC_MPRV);
        unregister_code (KC_MPRV);

        if (!oneshot)
            register_code (KC_LSFT);
      } else {
        return MACRO (T(MNXT), END);
      }
    }
    break;
  case AP_ITERM:
    if (record->event.pressed) {
      register_code (KC_LGUI);
      register_code (KC_SPC);
      unregister_code (KC_LGUI);
      unregister_code (KC_SPC);
      wait_ms(255);
      return MACRO(T(I), T(T), T(E), T(R), T(M), T(ENT), END);
    }
  case AP_SAFARI:
    if (record->event.pressed) {
      register_code (KC_LGUI);
      register_code (KC_SPC);
      unregister_code (KC_LGUI);
      unregister_code (KC_SPC);
      wait_ms(255);
      return MACRO(T(S), T(A), T(F), T(A), T(R), T(I), T(ENT), END);
    }
  case AP_FINDER:
    if (record->event.pressed) {
      register_code (KC_LGUI);
      register_code (KC_SPC);
      unregister_code (KC_LGUI);
      unregister_code (KC_SPC);
      wait_ms(255);
      return MACRO(T(F), T(I), T(N), T(D), T(E), T(R), T(ENT), END);
    }
  case AP_MESSAGE:
    if (record->event.pressed) {
      register_code (KC_LGUI);
      register_code (KC_SPC);
      unregister_code (KC_LGUI);
      unregister_code (KC_SPC);
      wait_ms(255);
      return MACRO(T(M), T(E), T(S), T(S), T(A), T(G), T(E), T(ENT), END);
    }
  case AP_SPOTIFY:
    if (record->event.pressed) {
      register_code (KC_LGUI);
      register_code (KC_SPC);
      unregister_code (KC_LGUI);
      unregister_code (KC_SPC);
      wait_ms(255);
      return MACRO(T(S), T(P), T(O), T(T), T(I), T(F), T(Y), T(ENT), END);
    }
  case AP_SLEEP:
    if (record->event.pressed) {
      register_code (KC_LGUI);
      register_code (KC_SPC);
      unregister_code (KC_LGUI);
      unregister_code (KC_SPC);
      wait_ms(255);
      return MACRO(T(S), T(L), T(E), T(E), T(P), T(ENT), END);
    }
  case AP_SEQUEL:
    if (record->event.pressed) {
      register_code (KC_LGUI);
      register_code (KC_SPC);
      unregister_code (KC_LGUI);
      unregister_code (KC_SPC);
      wait_ms(255);
      return MACRO(T(S), T(E), T(Q), T(U), T(E), T(L), T(ENT), END);
    }
  case AP_SLACK:
    if (record->event.pressed) {
      register_code (KC_LGUI);
      register_code (KC_SPC);
      unregister_code (KC_LGUI);
      unregister_code (KC_SPC);
      wait_ms(255);
      return MACRO(T(S), T(L), T(A), T(C), T(K), T(ENT), END);
    }
  }

  return MACRO_NONE;
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
  set_unicode_input_mode(UC_LNX);

  ergodox_led_all_on();
  for (int i = LED_BRIGHTNESS_HI; i > LED_BRIGHTNESS_LO; i--) {
    ergodox_led_all_set (i);
    wait_ms (5);
  }
  wait_ms(1000);
  for (int i = LED_BRIGHTNESS_LO; i > 0; i--) {
    ergodox_led_all_set (i);
    wait_ms (10);
  }
  ergodox_led_all_off();

  if (!eeconfig_is_enabled())
    eeconfig_init();
};

LEADER_EXTERNS();

static void ang_tap_dance_irc_mutt (qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    register_code (KC_LCTRL);
    register_code (KC_S);
    unregister_code (KC_LCTRL);
    unregister_code (KC_S);
    register_code (KC_I);
    unregister_code (KC_I);
  } else {
    register_code (KC_LCTRL);
    register_code (KC_S);
    unregister_code (KC_LCTRL);
    unregister_code (KC_S);
    register_code (KC_M);
    unregister_code (KC_M);
  }
}

static void ang_tap_dance_tmux_zoom (qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    register_code (KC_LCTRL);
    register_code (KC_S);
    unregister_code (KC_S);
    unregister_code (KC_LCTRL);
  } else {
    register_code (KC_LCTRL);
    register_code (KC_S);
    unregister_code (KC_LCTRL);
    unregister_code (KC_S);
    register_code (KC_Z);
    unregister_code (KC_Z);
  }
}

static void ang_tap_dance_proj (qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    register_code (KC_LCTRL);
    register_code (KC_S);
    unregister_code (KC_S);
    register_code (KC_J);
    unregister_code (KC_J);
    unregister_code (KC_LCTRL);
  } else {
    register_code (KC_LCTRL);
    register_code (KC_S);
    unregister_code (KC_S);
    register_code (KC_T);
    unregister_code (KC_T);
    unregister_code (KC_LCTRL);
  }
}

static void ang_tap_dance_git_close (qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    register_code (KC_LCTRL);
    register_code (KC_S);
    unregister_code (KC_LCTRL);
    unregister_code (KC_S);
    register_code (KC_G);
    unregister_code (KC_G);
  } else {
    register_code (KC_LCTRL);
    register_code (KC_S);
    unregister_code (KC_LCTRL);
    unregister_code (KC_S);
    register_code (KC_X);
    unregister_code (KC_X);
  }
}

static void ang_tap_dance_bspc_bk (qk_tap_dance_state_t *state, void *user_data) {
  register_code (KC_LCTRL);
  register_code (KC_S);
  unregister_code (KC_LCTRL);
  unregister_code (KC_S);
  register_code (KC_A);
  unregister_code (KC_A);
}

qk_tap_dance_action_t tap_dance_actions[] = {
  [CT_CLN]        = ACTION_TAP_DANCE_DOUBLE (KC_COLN, KC_SCLN),
  [CT_LBP]        = ACTION_TAP_DANCE_DOUBLE (KC_LBRC, KC_LPRN),
  [CT_RBP]        = ACTION_TAP_DANCE_DOUBLE (KC_RBRC, KC_RPRN),
  [KF_1]          = ACTION_TAP_DANCE_DOUBLE (KC_1, KC_EXLM),
  [KF_2]          = ACTION_TAP_DANCE_DOUBLE (KC_2, KC_AT),
  [KF_3]          = ACTION_TAP_DANCE_DOUBLE (KC_3, KC_HASH),
  [KF_4]          = ACTION_TAP_DANCE_DOUBLE (KC_4, KC_DLR),
  [KF_5]          = ACTION_TAP_DANCE_DOUBLE (KC_5, KC_PERC),
  [KF_6]          = ACTION_TAP_DANCE_DOUBLE (KC_6, KC_CIRC),
  [KF_7]          = ACTION_TAP_DANCE_DOUBLE (KC_7, KC_AMPR),
  [KF_8]          = ACTION_TAP_DANCE_DOUBLE (KC_8, KC_ASTR),
  [KF_9]          = ACTION_TAP_DANCE_DOUBLE (KC_9, KC_LPRN),
  [KF_0]          = ACTION_TAP_DANCE_DOUBLE (KC_0, KC_RPRN),
  [KF_QUOT]       = ACTION_TAP_DANCE_DOUBLE (KC_QUOT, KC_DQUO),
  [KF_COMM]       = ACTION_TAP_DANCE_DOUBLE (KC_COMM, KC_LABK),
  [KF_DOT]        = ACTION_TAP_DANCE_DOUBLE (KC_DOT, KC_RABK),
  [KF_SLSH]       = ACTION_TAP_DANCE_DOUBLE (KC_SLSH, LSFT(KC_SLSH)),
  [KF_MINS]       = ACTION_TAP_DANCE_DOUBLE (KC_MINS, KC_UNDS),
  [KF_EQL]        = ACTION_TAP_DANCE_DOUBLE (KC_EQL, KC_PLUS),
  [CT_GIT_CLOSE]  = ACTION_TAP_DANCE_FN (ang_tap_dance_git_close),
  [CT_IRC_MUTT]   = ACTION_TAP_DANCE_FN (ang_tap_dance_irc_mutt),
  [CT_PROJ]       = ACTION_TAP_DANCE_FN (ang_tap_dance_proj),
  [CT_TMUX_ZOOM]  = ACTION_TAP_DANCE_FN (ang_tap_dance_tmux_zoom),
};

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
  static uint32_t prev_layer_state;
  uint8_t layer = biton32(layer_state);
  static char *layer_lookup[] = {"Qwerty", "Nav/Media"};

  if (layer_state != prev_layer_state) {
    prev_layer_state = layer_state;
    if (layer_lookup[layer])
      xprintf("LAYER: %s\n", layer_lookup[layer]);
  }

  if (layer == GUI_MOVE) {
    ergodox_right_led_2_on();
    ergodox_right_led_3_on();
  }

  if (keyboard_report->mods & MOD_BIT(KC_LSFT) ||
      ((get_oneshot_mods() & MOD_BIT(KC_LSFT)) && !has_oneshot_mods_timed_out())) {
    ergodox_right_led_1_set (LED_BRIGHTNESS_HI);
    ergodox_right_led_1_on ();
  } else {
    ergodox_right_led_1_set (LED_BRIGHTNESS_LO);
    ergodox_right_led_1_off ();
  }

  if (keyboard_report->mods & MOD_BIT(KC_LALT) ||
      ((get_oneshot_mods() & MOD_BIT(KC_LALT)) && !has_oneshot_mods_timed_out())) {
    ergodox_right_led_2_set (LED_BRIGHTNESS_HI);
    ergodox_right_led_2_on ();
  } else {
    ergodox_right_led_2_set (LED_BRIGHTNESS_LO);
    if (layer != GUI_MOVE)
      ergodox_right_led_2_off ();
  }

  if (keyboard_report->mods & MOD_BIT(KC_LCTRL) ||
      ((get_oneshot_mods() & MOD_BIT(KC_LCTRL)) && !has_oneshot_mods_timed_out())) {
    ergodox_right_led_3_set (LED_BRIGHTNESS_HI);
    ergodox_right_led_3_on ();
  } else {
    ergodox_right_led_3_set (LED_BRIGHTNESS_LO);
    if (layer != GUI_MOVE)
      ergodox_right_led_3_off ();
  }

  LEADER_DICTIONARY() {
    leading = false;
    leader_end ();

    SEQ_ONE_KEY (KC_1) {
      TAP_ONCE (KC_COLN);
      TAP_ONCE (KC_PLUS);
      TAP_ONCE (KC_1);
      TAP_ONCE (KC_COLN);
    }

    SEQ_ONE_KEY (KC_Y) {
      TAP_ONCE (KC_BSLS);
      TAP_ONCE (KC_O);
      TAP_ONCE (KC_SLSH);
    }

    SEQ_ONE_KEY (KC_L) {
      TAP_ONCE (KC_L);
      TAP_ONCE (KC_O);
      TAP_ONCE (KC_L);
    }
  }
}

