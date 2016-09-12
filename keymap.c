/* * alyi's ErgoDox EZ layout file!  */

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
#define NMDIA 1

/* Macros */
enum {
  // Diagonal mouse movement
  A_MUL,
  A_MUR,
  A_MDL,
  A_MDR,

  // Mouse acceleration
  A_ACL0,
  A_ACL1,
  A_ACL2,

  // Mail
  //M_WORK,
  //M_PERSO,

  // VIM
  //V_AUTOINDENT,
  //V_SEARCH,

  // APP
  AP_ITERM,
  AP_CHROME,
  AP_SAFARI,
  AP_MESSAGE,
  AP_FINDER,
  AP_ITUNES,
  AP_SLEEP,
  AP_SEQUEL,

  NP,
  KILL,

  // Modifier
  F_ALT,
  F_SFT,
  F_CTL,

  // Tap dace
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
  CT_COPY_PASTE,
  CT_CTRLP_TMUX,
  CT_CTRLN_ZOOM,
  CT_PROJ,
  CT_TEST,
  CT_GIT_CLOSE,
  CT_TAB_ESC,
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
 * |         ~ |   1  |   2  |   3  |   4  |   5  | ir/mu|           | proj |   6  |   7  |   8  |   9  |  0   | kill      |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |           |   Q  |   W  | E/SFT|   R  |   T  |   (  |           |  )   |   Y  |   U  | I/SFT|   O  |  P   | \         |
 * |-----------+------+------+------+------+------|   [  |           |  ]   |------+------+------+------+------+-----------|
 * |  Next/Prev|   A  | S/ALT| D/CTL| F/GUI|   G  |------|           |------|   H  | J/GUI| K/CTL| L/ALT|  '   | = / Arrow |
 * |-----------+------+------+------+------+------| C-p  |           | C-n  |------+------+------+------+------+-----------|
 * | Play/Pause|   Z  |   X  |   C  |   V  |   B  | tmux |           | zoom |   N  |   M  |   ,  |   .  |  /   |      Stop |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *     |cop/pas|      |      |   :  |  ctrl|                                       |   -  |test  |git/cl|      |       |
 *     `-----------------------------------'                                       `-----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         | LAlt | GUI  |           | MDIA | Del  |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      |tabesc|           | LEAD |      |      |
 *                                  |Backsp|LShift|------|           |------| Enter| Space|
 *                                  |      |      |  {   |           |  }   |      |      |
 *                                  `--------------------'           `--------------------'
 */
[BASE] = KEYMAP(
// left hand
 KC_TAB           ,TD(KF_1),TD(KF_2)   ,TD(KF_3)   ,TD(KF_4)   ,TD(KF_5),TD(CT_IRC_MUTT)
,KC_NO            ,KC_Q    ,KC_W       ,SFT_T(KC_E),KC_R       ,KC_T    ,TD(CT_LBP)
,M(NP)            ,KC_A    ,ALT_T(KC_S),CTL_T(KC_D),GUI_T(KC_F),KC_G
,KC_MPLY          ,KC_Z    ,KC_X       ,KC_C       ,KC_V       ,KC_B    ,TD(CT_CTRLP_TMUX)
,TD(CT_COPY_PASTE),KC_NO   ,KC_NO      ,TD(CT_CLN) ,F(F_CTL)

,F(F_ALT),OSM(MOD_LGUI)
,TD(CT_TAB_ESC)
,KC_BSPC ,F(F_SFT)     ,KC_LCBR

// right hand
,TD(CT_PROJ)      ,TD(KF_6)   ,TD(KF_7)        ,TD(KF_8)   ,TD(KF_9)   ,TD(KF_0)   ,M(KILL)
,TD(CT_RBP)       ,KC_Y       ,SFT_T(KC_U)     ,KC_I       ,KC_O       ,KC_P       ,KC_BSLS
,KC_H             ,GUI_T(KC_J),CTL_T(KC_K)     ,ALT_T(KC_L),TD(KF_QUOT),TD(KF_EQL)
,TD(CT_CTRLN_ZOOM),KC_N       ,KC_M            ,TD(KF_COMM),TD(KF_DOT) ,TD(KF_SLSH),KC_MSTP
,TD(KF_MINS)      ,TD(CT_TEST),TD(CT_GIT_CLOSE),KC_NO      ,KC_NO

,OSL(NMDIA),KC_DEL
,KC_LEAD
,KC_RCBR   ,KC_ENT ,KC_SPC
    ),
/* Keymap 1: Navigation & Media layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * | MS Slow   |sleep |sequel|      |      |      |ScrLCK|           |ScrLCK|itunes|iterm |safari|chrome|finder|messages   |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * | MS Normal |      | Home |  Up  | PgUp |      |      |           |Scroll|      |MsUpL | MsUp |MsUpR |      |PrintScreen|
 * |-----------+------+------+------+------+------|      |           |  Up  |------+------+------+------+------+-----------|
 * | MS Fast   |      | Left | Down | Right|      |------|           |------|      |MsLeft| MsDn |MsRght|      |           |
 * |-----------+------+------+------+------+------|      |           |Scroll|------+------+------+------+------+-----------|
 * | Play/Pause|      | End  | Down | PgDn |      |      |           | Down |      |MsDnL | MsDn |MsDnR |      |      Stop |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *      |      |      |      |      |      |                                       |      |      |      |      |      |
 *      `----------------------------------'                                       `----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         | Mute | VlUp |           | BASE | MClk |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      | VlDn |           | Prev |Left  |Right |
 *                                  |  SPC | Enter|------|           |------| Click| Click|
 *                                  |      |      | ESC  |           | Next |      |      |
 *                                  `--------------------'           `--------------------'
 */
[NMDIA] = KEYMAP(
// left hand
 M(A_ACL0)  ,M(AP_SLEEP) ,M(AP_SEQUEL),KC_NO   ,KC_NO   ,KC_NO   ,LGUI(KC_L)
,M(A_ACL1)  ,KC_NO       ,KC_HOME     ,KC_UP   ,KC_PGUP ,KC_NO   ,KC_NO
,M(A_ACL2)  ,KC_NO       ,KC_LEFT     ,KC_DOWN ,KC_RIGHT,KC_NO
,KC_MPLY    ,KC_NO       ,KC_END      ,KC_DOWN ,KC_PGDN ,KC_NO   ,KC_NO
,KC_NO      ,KC_NO       ,KC_NO       ,KC_NO   ,KC_NO

,KC_MUTE ,KC_VOLU
,KC_VOLD
,KC_SPC  ,KC_ENTER,KC_ESC

// right hand
,LGUI(KC_L),M(AP_ITUNES),M(AP_ITERM),M(AP_SAFARI),M(AP_CHROME),M(AP_FINDER),M(AP_MESSAGE)
,KC_WH_U   ,KC_NO       ,M(A_MUL)   ,KC_MS_U     ,M(A_MUR)    ,KC_NO       ,KC_PSCR
,KC_NO     ,KC_MS_L     ,KC_MS_D    ,KC_MS_R     ,KC_NO       ,KC_NO
,KC_WH_D   ,KC_NO       ,M(A_MDL)   ,KC_MS_D     ,M(A_MDR)    ,KC_NO       ,KC_MSTP
,KC_NO     ,KC_NO       ,KC_NO      ,KC_NO       ,KC_NO

,KC_TRNS,KC_MS_BTN3
,KC_MPRV
,KC_MNXT,KC_BTN1 ,KC_BTN2
    ),
};

const uint16_t PROGMEM fn_actions[] = {
  [F_SFT]  = ACTION_MODS_ONESHOT (MOD_LSFT),
  [F_ALT]  = ACTION_MODS_ONESHOT (MOD_LALT),
  [F_CTL]  = ACTION_MODS_ONESHOT (MOD_LCTL),
};

static struct {
  uint8_t idx;
} m_accel_state;

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
  case KILL:
    if (record->event.pressed) {
      register_code (KC_LCTRL);
      register_code (KC_C);
      unregister_code (KC_C);
      unregister_code (KC_LCTRL);
      register_code (KC_LCTRL);
      register_code (KC_C);
      unregister_code (KC_C);
      unregister_code (KC_LCTRL);
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
  case AP_CHROME:
    if (record->event.pressed) {
      register_code (KC_LGUI);
      register_code (KC_SPC);
      unregister_code (KC_LGUI);
      unregister_code (KC_SPC);
      wait_ms(255);
      return MACRO(T(C), T(H), T(R), T(O), T(M), T(E), T(ENT), END);
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

#if MOUSEKEY_ENABLE
  /* Mouse movement */
  case A_MUL:
    if (record->event.pressed) {
      mousekey_on(KC_MS_UP);
      mousekey_on(KC_MS_LEFT);
    } else {
      mousekey_off(KC_MS_UP);
      mousekey_off(KC_MS_LEFT);
    }
    mousekey_send();
    break;

  case A_MUR:
    if (record->event.pressed) {
      mousekey_on(KC_MS_UP);
      mousekey_on(KC_MS_RIGHT);
    } else {
      mousekey_off(KC_MS_UP);
      mousekey_off(KC_MS_RIGHT);
    }
    mousekey_send();
    break;

  case A_MDL:
    if (record->event.pressed) {
      mousekey_on(KC_MS_DOWN);
      mousekey_on(KC_MS_LEFT);
    } else {
      mousekey_off(KC_MS_DOWN);
      mousekey_off(KC_MS_LEFT);
    }
    mousekey_send();
    break;

  case A_MDR:
    if (record->event.pressed) {
      mousekey_on(KC_MS_DOWN);
      mousekey_on(KC_MS_RIGHT);
    } else {
      mousekey_off(KC_MS_DOWN);
      mousekey_off(KC_MS_RIGHT);
    }
    mousekey_send();
    break;

  case A_ACL0 ... A_ACL2:
    if (record->event.pressed) {
      uint8_t idx = id - A_ACL0;
      if (m_accel_state.idx == id) {
        mousekey_off(m_accel_state.idx - A_ACL0 + KC_ACL0);
        m_accel_state.idx = 0;
      } else {
        if (m_accel_state.idx) {
          mousekey_off(m_accel_state.idx - A_ACL0 + KC_ACL0);
          m_accel_state.idx = 0;
        }
        mousekey_on(KC_ACL0 + idx);
        m_accel_state.idx = id;
      }
    }
    break;
#endif
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
  }
}

static void ang_tap_dance_ctrlp_tmux (qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    register_code (KC_LCTRL);
    register_code (KC_P);
    unregister_code (KC_P);
    unregister_code (KC_LCTRL);
  } else {
    register_code (KC_LCTRL);
    register_code (KC_S);
    unregister_code (KC_S);
    unregister_code (KC_LCTRL);
  }
}

static void ang_tap_dance_ctrln_zoom (qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    register_code (KC_LCTRL);
    register_code (KC_N);
    unregister_code (KC_N);
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

static void ang_tap_dance_test (qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    register_code (KC_SPC);
    unregister_code (KC_SPC);
    register_code (KC_A);
    unregister_code (KC_A);
  } else {
    register_code (KC_SPC);
    unregister_code (KC_SPC);
    register_code (KC_L);
    unregister_code (KC_L);
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

LEADER_EXTERNS();

qk_tap_dance_action_t tap_dance_actions[] = {
  [CT_CLN]        = ACTION_TAP_DANCE_DOUBLE (KC_COLN, KC_SCLN),
  [CT_LBP]        = ACTION_TAP_DANCE_DOUBLE (KC_LBRC, KC_LPRN),
  [CT_RBP]        = ACTION_TAP_DANCE_DOUBLE (KC_RBRC, KC_RPRN),
  [CT_COPY_PASTE] = ACTION_TAP_DANCE_DOUBLE (LGUI(KC_C), LGUI(KC_V)),
  [CT_TAB_ESC]    = ACTION_TAP_DANCE_DOUBLE (KC_TAB, KC_ESC),
  [CT_IRC_MUTT]   = ACTION_TAP_DANCE_FN (ang_tap_dance_irc_mutt),
  [CT_CTRLP_TMUX] = ACTION_TAP_DANCE_FN (ang_tap_dance_ctrlp_tmux),
  [CT_CTRLN_ZOOM] = ACTION_TAP_DANCE_FN (ang_tap_dance_ctrln_zoom),
  [CT_PROJ]       = ACTION_TAP_DANCE_FN (ang_tap_dance_proj),
  [CT_TEST]       = ACTION_TAP_DANCE_FN (ang_tap_dance_test),
  [CT_GIT_CLOSE]  = ACTION_TAP_DANCE_FN (ang_tap_dance_git_close),
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

  if (layer == NMDIA) {
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
    if (layer != NMDIA)
      ergodox_right_led_2_off ();
  }

  if (keyboard_report->mods & MOD_BIT(KC_LCTRL) ||
      ((get_oneshot_mods() & MOD_BIT(KC_LCTRL)) && !has_oneshot_mods_timed_out())) {
    ergodox_right_led_3_set (LED_BRIGHTNESS_HI);
    ergodox_right_led_3_on ();
  } else {
    ergodox_right_led_3_set (LED_BRIGHTNESS_LO);
    if (layer != NMDIA)
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

