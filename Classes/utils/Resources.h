#ifndef RESOURCES_H
#define RESOURCES_H

/* ANIMATION */
#define ANI_MAP "bg%d_%d_%d.png"
#define ANI_JOYSTICK_BACKGROUND "analog_ring.png"
#define ANI_JOYSTICK_THUMB "analog.png"
#define ANI_BUTTON_A "button-a.png"
#define ANI_BUTTON_B "button-b.png"
#define ANI_SHADOW_1 "shadow1.png"
#define ANI_SHADOW_2 "shadow2.png"
#define ANI_SHADOW_3 "shadow3.png"
#define ANI_BAR_HP "bar-hp.png"
#define ANI_BAR_HP_CONTENT "bar-hp-content.png"
#define ANI_BAR_WP "bar-wp.png"
#define ANI_BAR_WP_CONTENT "bar-wp-content.png"
#define ANI_BAR_ANGRY "bar-angry.png"
#define ANI_BAR_ANGRY_CONTENT "bar-angry-content.png"
#define ANI_ICON_HP "icon-hp.png"
#define ANI_ICON_ANGRY "icon-angry.png"
#define ANI_ICON_WP "icon-wp.png"
#define ANI_NEXT_ARROW "next-arrow.png"
#define ANI_END_ZONE "end_zone.png"
#define ANI_BARREL "barrel1.png"

#define ANI_BOX_CHAPTER "bg/box-chapter.png"
#define ANI_BOX_CONFIRM "bg/box-confirm.png"
#define ANI_BOX_SHARE "bg/box-share.png"
#define ANI_BOX_CHAPTER_LOCK "bg/box-chapter-lock.png"
#define ANI_BOX_MAP "bg/box-map.png"
#define ANI_BOX_MAP_LOCK "bg/box-map-lock.png"
#define ANI_BOX_OPTION "bg/box-option.png"
#define ANI_BOX_PAUSE "bg/box-pause.png"
#define ANI_PAUSE_BUTTON "bg/pause-button.png"
#define ANI_BUTTON "bg/button.png"
#define ANI_BUTTON_SMALL "bg/button-small.png"
#define ANI_CLOSE_BUTTON "bg/button-close.png"
#define ANI_CHAPTER_TITLE "bg/chapter-title.png"
#define ANI_LEVEL_TITLE "bg/level-title.png"
#define ANI_LOGO "bg/logo.png"
#define ANI_PAUSE "button/pause.png"
#define ANI_CHAPTER_BG "chapter-%d.png"
#define ANI_BACKGROUND "title_bkg-hd.png"
#define ANI_BACKGROUND_FADE "fade-background.png"
#define ANI_CHAPTER_THUMB "bg/chapter-%d-thumb.png"
#define ANI_INTRO "bg/intro.png"

#define ANI_REMOVE_ADS_BUTTON "bg/button-remove-ads.png"
#define ANI_RESTORE_ADS_BUTTON "bg/button-restore-ads.png"
#define ANI_SHARE_FB_BUTTON "bg/button-share-fb.png"
#define ANI_SHARE_TW_BUTTON "bg/button-share-tw.png"
#define ANI_RATE_BUTTON "bg/button-rate.png"

/* CONFIG_FAKE */
#define CONF_FAKE_1 0.5f

/* CONFIGURATION */
#define CONF_LEVELS_PER_CHAPTER 4
#define CONF_SCALE_GAME 2.0
#define CONF_PAD_PADDING_X 80
#define CONF_PAD_PADDING_Y 50
#define CONF_RANGE_TO_TOUCH 40
#define CONF_JOYSTICK_OFFSET_X 5.0f
#define CONF_JOYSTICK_OFFSET_Y 5.0f
#define CONF_JOYSTICK_RADIUS 43.0f
#define CONF_THUMB_RADIUS 43.0f
#define CONF_MIN_VELOCITY_RATE 0.3f
#define CONF_MAX_Y 166
#define CONF_MIN_Y 62
#define CONF_MAN_VELOCITY 150
#define CONF_ZOMBIE_VELOCITY 100
#define CONF_AIR_TO_TARGET_VELOCITY 300
#define CONF_MAN_MIN_VECTOR_Y 20.0f
#define CONF_MAN_MAX_VECTOR_Y 150.0f
#define CONF_SPEED_JUMP 300.0f
#define CONF_DELAY_TO_NEXT_PUNCH 0.2f
#define CONF_DELAY_ZOMBIE_ATTACK 1
#define CONF_DELAY_HURTING 0.5f
#define CONF_DELAY_KOING 1.5f
#define CONF_SHADOW_MAX 1
#define CONF_SHADOW_MIN 0.3f
#define CONF_FALL_VELOCITY_X 300
#define CONF_FALL_VELOCITY_Y 200
#define CONF_STAR_VECTOR_X 16.0f
#define CONF_STAR_VECTOR_Y 30.0f
#define CONF_DELAY_TO_DISAPPEAR 2.0f
#define CONF_MAN_HP 100
#define CONF_MAN_ANGRY 90
#define CONF_MAN_PUNCH_SCORE 50
#define CONF_MAN_WP 100
#define CONF_DELAY_TO_CHANGE_DIRECTION 1
#define CONF_DELAY_TO_NEXT_MAP 1
#define CONF_DISTANCE_TO_APPEAR 100
#define CONF_MAN_OFF_SET_X -100
#define CONF_MAN_APPEAR_VELOCITY 150
#define CONF_MAN_DISAPPEAR_VELOCITY 150
#define CONF_DISTANCE_PER_CHAPTER 250
#define CONF_TOUCH_RANGE 35
#define CONF_UP_ANGRY 0.1f
#define CONF_DOWN_ANGRY 0.2f
#define CONF_END_ZONE_Y 127
#define CONF_DOWN_AP_SPEED 10
#define CONF_TRANSFORMER_RATE 1.5f
#define CONF_DELAY_ZOMBIE_AIMING 5
#define CONF_DELAY_ZOMBIE_DEFENDING 0.2f
#define CONF_DOOR_X 32
#define CONF_DOOR_Y 11

/* DOCUMENT */
#define DOC_LEVEL "level_%d_%d.json"
#define DOC_BG_MAP "bg%d_%d.plist"
#define DOC_FX "fx.plist"
#define DOC_GUI "gui.plist"
#define DOC_GAME "game.plist"
#define DOC_USER "user.dat"
#define DOC_MAPS "maps.dat"
#define DOC_PICKUPS "pickups.plist"

/* DAMAGE TYPE */
#define DAMAGE_MAN 1
#define DAMAGE_ZOMBIE 2
#define DAMAGE_SHOT 3

/* EVENT */
#define EVT_START_GAME 1
#define EVT_STOP_GAME 2
#define EVT_MAP_CREATED 3
#define EVT_JOYSTICK_MOVE 4
#define EVT_JOYSTICK_STAND 5
#define EVT_NEXT_CHECK_POINT 6
#define EVT_CREATE_END_ZONE 7
#define EVT_PUNCH 8
#define EVT_JUMP 9
#define EVT_NEW_HIT_PUNCH 11
#define EVT_STAR_ROTATE 12
#define EVT_ZOMBIE_ALMOST_DIE 13
#define EVT_REPLAY_MAP 14
#define EVT_START_MAP 15
#define EVT_START_PLAY 16
#define EVT_BACK_BEGIN 17
#define EVT_QUIT_GAME 18
#define EVT_PAUSE_GAME 19
#define EVT_RESUME_GAME 20
#define EVT_CHANGE_SFX 21
#define EVT_CHANGE_MUSIC 22
#define EVT_SHARE_FB 23
#define EVT_SHARE_TW 24
#define EVT_RATE 25
#define EVT_REMOVE_ADS 26
#define EVT_RESTORE_ADS 27
#define EVT_SHOW_SHARE_REQUIRED 28
#define EVT_COMPLETED_GAME 29
#define EVT_NEXT_MAP 30
#define EVT_NEW_SHOT 31
#define EVT_DELETE_SHOT 32

/* ENEMY */
#define KEY_AGENT "agent"
#define KEY_AXE_FIREMAN "axe_fireman"
#define KEY_BASEBALL_BATTER "baseball_batter"
#define KEY_BASEBALL_PITCHER "baseball_pitcher"
#define KEY_BATON_COP "baton_cop"
#define KEY_BIKER "biker"
#define KEY_BOSS1 "boss1"
#define KEY_BOSS2 "boss2"
#define KEY_BOSS3 "boss3"
#define KEY_BOSS4 "boss4"
#define KEY_BOSS5 "boss5"
#define KEY_CODER "coder"
#define KEY_DOORS "doors"
#define KEY_FIST_COP "fist_cop"
#define KEY_FIST_FIREMAN "fist_fireman"
#define KEY_FIST_SOLDIER "fist_soldier"
#define KEY_FORKLIFT "forklift"
#define KEY_FX "fx"
#define KEY_GOLFER "golfer"
#define KEY_GORILLA "gorilla"
#define KEY_GUARD_BIG "guard_big"
#define KEY_GUARD "guard"
#define KEY_LASER_NERD "laser_nerd"
#define KEY_MANLY_MAN "manly_man"
#define KEY_MIME "mime"
#define KEY_NINJA "ninja"
#define KEY_PISTOL_COP "pistol_cop"
#define KEY_PLAYER "player"
#define KEY_PROJECTILES "projectiles"
#define KEY_RIFLE_SOLDIER "rifle_soldier"
#define KEY_RIOT_COP "riot_cop"
#define KEY_RUNNER "runner"
#define KEY_SOLDIER_BIG "soldier_big"
#define KEY_SPIKES "spikes"
#define KEY_TANK "tank"
#define KEY_WATER_SPRAY "water_spray"
#define KEY_WEAPONS "weapons"
#define KEY_WORKER "worker"
#define KEY_ZOMBIE "zombie"

/* FACE */
#define FACE_CHAR "char"
#define FACE_MAIN "main"
#define FACE_SHADOW "shadow"

/* FONT */
#define FONT_ARIAL "Arial"
#define FONT_GAME_SMALL "upheaval_36.fnt"
#define FONT_GAME_LARGE "upheaval_60.fnt"
#define FONT_GAME_BIG "upheaval_60-hd.fnt"

/* GAME STATE */
#define GS_LOAD 1
#define GS_PLAY 2
#define GS_PAUSE 3
#define GS_WAIT 4
#define GS_FINISH 5
#define GS_REPLAY 6

/* NODE */
#define NODE_BUTTON_PAUSE "buttonpause"
#define NODE_BUTTON_RESUME "buttonresume"
#define NODE_BUTTON_QUIT_GAME "buttonquitgame"
#define NODE_BUTTON_BACK "buttonback"
#define NODE_BUTTON_START_GAME "buttonstartgame"
#define NODE_BUTTON_OPTION "buttonoption"
#define NODE_PAUSE_BOARD "pauseboard"
#define NODE_SHARE_BOARD "shareboard"
#define NODE_FADE_BG "fadebackground"
#define NODE_OPTION_LAYER "optionlayer"
#define NODE_CHAPTER_LEVEL_LABEL "chapterlevellabel"
#define NODE_BUTTON_FACEBOOK "buttonfacebook"
#define NODE_BUTTON_TWITTER "buttontwitter"
#define NODE_BUTTON_OK "buttonok"
#define NODE_LABEL_SHARE "labelshare"
#define NODE_WP_ICON "wpicon"

#define NODE_BUTTON_REMOVE_AD "buttonremovead"
#define NODE_BUTTON_RESTORE_AD "buttonrestoread"
#define NODE_BUTTON_FB "buttonfacebook"
#define NODE_BUTTON_TW "buttontwitter"
#define NODE_BUTTON_RATE "buttonrate"

/* SCHEDULE */
#define SCHEDULE_GAME_MODEL 1
#define SCHEDULE_SCENE 2
#define SCHEDULE_LAYER 3

/* STATE */
#define STATE_IDLE "idle"
#define STATE_RUN "run"
#define STATE_ATTACK "attack"
#define STATE_JUMP "jump"
#define STATE_DIE "die"
#define STATE_SHOOT "shoot"
#define STATE_HIT_HIGH "hit_high"
#define STATE_HIT_LOW "hit_low"
#define STATE_KO "ko"
#define STATE_GET_UP "get_up"
#define STATE_DRIVE "drive"
#define STATE_SPIN "spin"
#define STATE_DASH "dash"
#define STATE_BLOCK "block"
#define STATE_SPAWN_UP "spawn_up"
#define STATE_SPAWN_DOWN "spawn_down"
#define STATE_HEAD1 "head1"
#define STATE_HEAD2 "head2"
#define STATE_WALL1_IDLE "wall1_idle"
#define STATE_WALL2_IDLE "wall2_idle"
#define STATE_WALL3_IDLE "wall3_idle"
#define STATE_DOOR1_IDLE "door1_idle"
#define STATE_DOOR1_OPEN "door1_open"
#define STATE_DOOR1_CLOSE "door1_close"
#define STATE_DOOR2_IDLE "door2_idle"
#define STATE_DOOR2_OPEN "door2_open"
#define STATE_DOOR2_CLOSE "door2_close"
#define STATE_DOOR3_IDLE "door3_idle"
#define STATE_DOOR3_OPEN "door3_open"
#define STATE_DOOR3_CLOSE "door3_close"
#define STATE_BLOOD "blood"
#define STATE_BOMB_BEACON "bomb_beacon"
#define STATE_EXPLOSION "explosion"
#define STATE_EYE_FLASH "eye_flash"
#define STATE_GROUND_EXPLOSION "ground_explosion"
#define STATE_HIT_FLASH "hit_flash"
#define STATE_HIT_FLASH_2 "hit_flash2"
#define STATE_SHADOW "shadow"
#define STATE_STARS "stars"
#define STATE_WALL_EXPLOSION "wall_explosion"
#define STATE_EYE_LASER "eye_laser"
#define STATE_GRENADE "grenade"
#define STATE_ATTACK2 "attack2"
#define STATE_HIT "hit"
#define STATE_PICKUP "pickup"
#define STATE_AERIAL_ATTACK "aerial_attack"
#define STATE_ATTACK_1 "attack_1"
#define STATE_ATTACK_2 "attack_2"
#define STATE_ATTACK_3 "attack_3"
#define STATE_ATTACK_4 "attack_4"
#define STATE_ATTACK_5 "attack_5"
#define STATE_FRENZY_ATTACK_1 "frenzy_attack_1"
#define STATE_FRENZY_ATTACK_2 "frenzy_attack_2"
#define STATE_FRENZY_ATTACK_3 "frenzy_attack_3"
#define STATE_FRENZY_ATTACK_4 "frenzy_attack_4"
#define STATE_FRENZY_ATTACK_5 "frenzy_attack_5"
#define STATE_ARMED_IDLE "armed_idle"
#define STATE_ARMED_RUN "armed_run"
#define STATE_ARMED_ATTACK_1 "armed_attack_1"
#define STATE_ARMED_ATTACK_2 "armed_attack_2"
#define STATE_ENRAGE_IDLE "enrage_idle"
#define STATE_ENRAGE "enrage"
#define STATE_BULLET "bullet"
#define STATE_FIRE_LEFT "fire_left"
#define STATE_FIRE_RIGHT "fire_right"
#define STATE_WATER_POINT "water_point"
#define STATE_WATER_SPRAY "water_spray"
#define STATE_FRAGS "frags"
#define STATE_SPAWN "spawn"

#define STATE_ALMOST_DIE "almost-die"
#define STATE_FADE_IN "fadein"
#define STATE_FADE_OUT "fadeout"
#define STATE_MOVE_DOWN "movedown"
#define STATE_MOVE_UP "moveup"
#define STATE_FRENZY_ON "frenzyon"
#define STATE_FRENZY_OFF "frenzyoff"

/* SHOT */
#define SHOT_TYPE_LASER 1

/* TEXT */
#define TXT_START_GAME "START GAME"
#define TXT_OPTIONS "OPTIONS"
#define TEXT_TELL_FRIEND "TELL A FRIEND"
#define TXT_REMOVE_ADS "REMOVE ADS"
#define TXT_MORE_GAMES "MORE GAMES"
#define TXT_BACK "BACK"
#define TXT_OK "OK"
#define TXT_CONFIRM_TEXT "ARE YOU SURE TO EXIT?"
#define TXT_CANCEL "CANCEL"
#define TXT_COPY_RIGHT ""
#define TXT_CHAPTER_LEVEL "CHAPTER %d - LEVEL %d"
#define TXT_MUSIC_PROGRESS "MUSIC - %s"
#define TXT_SFX_PROGRESS "SFX - %s"
#define TXT_RESUME "RESUME"
#define TXT_QUIT_GAME "QUIT GAME"
#define TXT_SAVE_TEXT "%d$%d$%d$%d"
#define TXT_REQUIRE_SHARE "IF YOU LIKE THIS GAME, PLEASE SHARE WITH YOUR FRIENDS. THANKS!"
#define TXT_CONRATULATION_SHARE "YOU WON THIS GAME, PLEASE SHARE WITH YOUR FRIENDS. THANKS!"
#define TXT_SHARE_FACEBOOK "FACEBOOK :)"
#define TXT_SHARE_TWITTER "TWITTER ^_^"
#define TXT_SHARE_OK "CLOSE :(("
#define TXT_SCORE "SCORE"

/* UG */
#define UG_KEY "ug"
#endif



	