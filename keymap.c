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
#define SHT_MOVE 2

/* Macros */
enum {
  // APP
  AP_ITERM,
  AP_SAFARI,
  AP_MESSAGE,
  AP_FINDER,
  AP_ITUNES,
  AP_SLEEP,
  AP_SEQUEL,
  AP_INTELLIJ,
  AP_RAMBOX,

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
 * |         ~ |   1  |   2  |   3  |   4  |   5  |      |           |      |   6  |   7  |   8  |   9  |  0   |           |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |           |   Q  |   W  | D/SFT|   F  |   K  |   (  |           |  )   |   J  |   U  | R/SFT|   L  |  '   | \         |
 * |-----------+------+------+------+------+------|   [  |           |  ]   |------+------+------+------+------+-----------|
 * |  Next/Prev|   A  | S/ALT| E/CTL| T/GUI| G/gui|------|           |------| Y/Gui| N/GUI| I/CTL| O/ALT|  H   | = /       |
 * |-----------+------+------+------+------+------|   {  |           |  }   |------+------+------+------+------+-----------|
 * | Play/Pause|   Z  |   X  |   C  |V/SHTL|   B  |      |           |      |   P  |   M  |   ,  |   .  |      | KC_VOLU   |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *     | cut   | copy |paste |   :  |  /   |                                       |   -  |      |      |      |KC_VOLD|
 *     `-----------------------------------'                                       `-----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         | esc  |      |           |      | Del  |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      |      |           | LEAD |      |      |
 *                                  |Backsp| tab  |------|           |------| Enter| Space|
 *                                  |      |      |gui   |           |      |      |      |
 *                                  `--------------------'           `--------------------'
 */
[BASE] = KEYMAP(
// left hand
 KC_TILD          ,TD(KF_1)   ,TD(KF_2)   ,TD(KF_3)   ,TD(KF_4)         ,TD(KF_5),KC_NO
,KC_NO            ,KC_Q       ,KC_W       ,SFT_T(KC_D),KC_F             ,KC_K    ,TD(CT_LBP)
,M(NP)            ,KC_A       ,ALT_T(KC_S),CTL_T(KC_E),GUI_T(KC_T)      ,LT(GUI_MOVE, KC_G)
,KC_MPLY          ,KC_Z       ,KC_X       ,KC_C       ,LT(SHT_MOVE,KC_V),KC_B    ,KC_LCBR
,LGUI(KC_X)       ,LGUI(KC_C) ,LGUI(KC_V) ,TD(CT_CLN) ,TD(KF_SLSH)

,KC_ESC ,KC_NO
,KC_NO
,KC_BSPC,KC_TAB,TG(GUI_MOVE)


// right hand
,KC_NO             ,TD(KF_6)   ,TD(KF_7)        ,TD(KF_8)   ,TD(KF_9)        ,TD(KF_0)   ,KC_NO
,TD(CT_RBP)        ,KC_J       ,KC_U            ,SFT_T(KC_R),KC_L            ,TD(KF_QUOT),KC_BSLS
,LT(GUI_MOVE, KC_Y),GUI_T(KC_N),CTL_T(KC_I)     ,ALT_T(KC_O),KC_H            ,TD(KF_EQL)
,KC_RCBR           ,KC_P       ,KC_M            ,TD(KF_COMM),TD(KF_DOT)      ,KC_NO      ,KC_VOLU
,TD(KF_MINS)       ,KC_NO      ,KC_NO           ,KC_NO      ,KC_VOLD

,KC_NO,KC_DEL
,KC_LEAD
,KC_NO   ,KC_ENT ,KC_SPC
    ),
/* Keymap 1: Gui and move
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * |           |      |      |      |      |      |      |           | sleep|iterm |inteli|safari|rambox|itunes| message   |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |           | G_Q  | G_W  |      | G_R  | G_T  |      |           |      |      | home | pgdn | pgup |  end | sequel    |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           | G_A  | G_S  |      | G_F  | base |------|           |------| base | left | down | top  |right | finder    |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           | G_Z  | G_X  | G_C  | G_V  |      | G_{  |           | G_}  |      | wleft| wdn  | wup  |wrgt  |           |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *      |      |      |      |      |      |                                       |      |      |      |      |      |
 *      `----------------------------------'                                       `----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         |      |      |           |      |      |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      |      |           |      |Left  |Right |
 *                                  | G_Spc|enter |------|           |------| Click| Click|
 *                                  |      |      | base |           |      |      |      |
 *                                  `--------------------'           `--------------------'
 */
[GUI_MOVE] = KEYMAP(
// left hand
 KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO     ,KC_NO
,KC_NO      ,LGUI(KC_Q) ,LGUI(KC_W) ,KC_NO      ,LGUI(KC_R) ,LGUI(KC_T),KC_NO
,KC_NO      ,LGUI(KC_A) ,LGUI(KC_S) ,KC_NO      ,LGUI(KC_F) ,KC_TRNS
,KC_NO      ,LGUI(KC_Z) ,LGUI(KC_X) ,LGUI(KC_C) ,LGUI(KC_V) ,KC_NO     ,KC_NO
,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO

,KC_NO ,KC_NO
,KC_NO
,LGUI(KC_SPC),KC_ENT,KC_TRNS

// right hand
,M(AP_SLEEP),M(AP_ITERM),M(AP_INTELLIJ),M(AP_SAFARI),M(AP_RAMBOX),M(AP_ITUNES) ,M(AP_MESSAGE)
,KC_NO      ,KC_NO      ,KC_HOME       ,KC_PGDN     ,KC_PGUP     ,KC_END       ,M(AP_SEQUEL)
,KC_TRNS    ,KC_LEFT    ,KC_DOWN       ,KC_UP       ,KC_RGHT     ,M(AP_FINDER)
,KC_NO      ,KC_NO      ,LALT(KC_LEFT) ,KC_WH_D     ,KC_WH_U     ,LALT(KC_RGHT),KC_NO
,KC_NO      ,KC_NO      ,KC_NO         ,KC_NO       ,KC_NO

,KC_NO,KC_NO
,KC_NO
,KC_NO,KC_BTN1 ,KC_BTN2
    ),
/* Keymap 2: Shift and move
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * |           |      |      |      |      |      |      |           |      |      |      |      |      |      |           |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |      |           |      |      | home | pgdn | pgup |  end |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |------|           |------|      | left | down | top  |right |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |      |      |      | base |      |      |           |      |      | wleft|      |      |wrgt  |           |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *      |      |      |      |      |      |                                       |      |      |      |      |      |
 *      `----------------------------------'                                       `----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         |      |      |           |      |      |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      |      |           |      |      |      |
 *                                  |      |      |------|           |------|      |      |
 *                                  |      |      | base |           |      |      |      |
 *                                  `--------------------'           `--------------------'
 */
[SHT_MOVE] = KEYMAP(
// left hand
 KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO     ,KC_NO
,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO     ,KC_NO
,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO
,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_TRNS    ,KC_NO     ,KC_NO
,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO

,KC_NO ,KC_NO
,KC_NO
,KC_NO ,KC_NO,KC_TRNS

// right hand
,KC_NO      ,KC_NO        ,KC_NO               ,KC_NO         ,KC_NO         ,KC_NO              ,KC_NO
,KC_NO      ,KC_NO        ,LSFT(KC_HOME)       ,LSFT(KC_PGDN) ,LSFT(KC_PGUP) ,LSFT(KC_END)       ,KC_NO
,KC_NO      ,LSFT(KC_LEFT),LSFT(KC_DOWN)       ,LSFT(KC_UP)   ,LSFT(KC_RGHT) ,KC_NO
,KC_NO      ,KC_NO        ,LSFT(LALT(KC_LEFT)) ,KC_NO         ,KC_NO         ,LSFT(LALT(KC_RGHT)),KC_NO
,KC_NO      ,KC_NO        ,KC_NO               ,KC_NO         ,KC_NO

,KC_NO,KC_NO
,KC_NO
,KC_NO,KC_NO,KC_NO
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
  case AP_ITUNES:
    if (record->event.pressed) {
      register_code (KC_LGUI);
      register_code (KC_SPC);
      unregister_code (KC_LGUI);
      unregister_code (KC_SPC);
      wait_ms(255);
      return MACRO(T(I), T(T), T(U), T(N), T(E), T(S), T(ENT), END);
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
  case AP_INTELLIJ:
    if (record->event.pressed) {
      register_code (KC_LGUI);
      register_code (KC_SPC);
      unregister_code (KC_LGUI);
      unregister_code (KC_SPC);
      wait_ms(255);
      return MACRO(T(I), T(N), T(T), T(E), T(L), T(L), T(I), T(J), T(ENT), END);
    }
  case AP_RAMBOX:
    if (record->event.pressed) {
      register_code (KC_LGUI);
      register_code (KC_SPC);
      unregister_code (KC_LGUI);
      unregister_code (KC_SPC);
      wait_ms(255);
      return MACRO(T(R), T(A), T(M), T(B), T(O), T(X), T(ENT), END);
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

  if (layer == SHT_MOVE) {
    ergodox_right_led_1_on();
    ergodox_right_led_2_on();
  }

  if (keyboard_report->mods & MOD_BIT(KC_LSFT) ||
      ((get_oneshot_mods() & MOD_BIT(KC_LSFT)) && !has_oneshot_mods_timed_out())) {
    ergodox_right_led_1_set (LED_BRIGHTNESS_HI);
    ergodox_right_led_1_on ();
  } else {
    ergodox_right_led_1_set (LED_BRIGHTNESS_LO);
    if (layer != SHT_MOVE)
      ergodox_right_led_1_off ();
  }

  if (keyboard_report->mods & MOD_BIT(KC_LALT) ||
      ((get_oneshot_mods() & MOD_BIT(KC_LALT)) && !has_oneshot_mods_timed_out())) {
    ergodox_right_led_2_set (LED_BRIGHTNESS_HI);
    ergodox_right_led_2_on ();
  } else {
    ergodox_right_led_2_set (LED_BRIGHTNESS_LO);
    if (layer != GUI_MOVE && layer != SHT_MOVE)
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

