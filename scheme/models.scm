/*   

Programmer's resource file  
  For objects & textures that should be loaded for every level


*/

(setf texture_format       "textures/%s.jpg")
(setf building_format      "objects/%s.gmod")
(setf object_format        "objects/%s.gmod")


(textures
 texture_format
 "pink"
 ("grndsand1"                       (save_name "sand_center_1"))
  "cloud2"
  "cloud"
)


/* default models, loaded for every level */
(models
 object_format
  
  "cloud" "cloud2"
  "supertanktop"    "supertankbase"  "chunk_supertankbase"  "supertanktop_full"
  "guided_missile"
  "buster_rocket"  "buster_body_lod"
  "heavy_rocket"
  "vortex_missile"
  "nuke_missile"

  "bomb_truck"  "bomb_truck_lod"

  "bridger_road"     "bridger_wheels"
  
  "peon_body"        "peon_turret"  "peon_body_shadow" "peon_turret_shadow"
  "peon_body_lod"    "peon_turret_lod"

  "trike_body"      "trike_wheels"  "trike_shadow"  "trike_lod"

  "rocket_truck"    "rocket_truck_top"  "rocket_truck_shadow"
  "rocket_truck_top_lod"  "rocket_truck_lod"

  "jet_body"        "jet_engines"  "jet_shadow"  "jet_body_lod"
  
  "elec_body"       "elec_wheel"            "elec_gun"  "elec_shadow"  "elec_body_lod"

  "heli_body"       "heli_blades"           "heli_blades-alpha"
  "heli_body_shadow" "heli_blades_shadow"   "heli_body_lod"

  "engineer_body"   "engineer_back_wheels"  "engineer_front_wheels" "engineer_shadow"
  "engineer_lod"
  
  "bird_body" "bird_wingr" "bird_shadow" "bird_wingl" "bird_body-original"
  "bird_wingr-original" "bird_wingl-original"
  
  "cobra" "cobra_body" "cobra_main_barrel" "cobra_main_turret" "cobra_mini_turret"
  "gunport_base" "gunport_barrel"
  "buster_body"     "buster_body_shadow"

  "bomber" "bomber_shadow" "bomber_lod" 
  "shield_building" "shield_building_shield"
  

  "moneyplane" "moneyplane_lod"
  "moneycrate"
 
  "turret"          "turret_muzzle"
  "aagun_base"      "aagun_barrels"         "aagun_turret"
  "popup_housing"   "popup_mount"           "popup_barrel"

  "tower_missile_base" "tower_missile_launcher" "tower_missile_bottom"
  "tower_electric_base" "tower_electric_pod" "tower_electric_fingers" "tower_electric_bottom"
  "tower_gun_base" "tower_gun_barrel" "tower_gun_bottom"
  "supergun_base"   "supergun_barrel"
  "repair_base"     "repair_turret"         "repair_boom"     "repair_tip"
  "fire_chunk"
  "jetbomb"
  "takeover_pad"
  "lightbulb"
  "speaker"
  "gunshell"
  "shockwave"
;  "test"

  "b120mm" "b90mm" "napalm"  "acid" "chain_gun"

  "bank" "bank_lod"

  "selection_arrow"
  "target"

  "metal_chunk" "fire_chunk"
  "sky"
  "cloud"


  "testsphere"
  "sun_and_moon"
  "pad_cursor"
  "cobra"
  "trigger"
  "powerup_shields"     
  "powerup_bank"     
  "powerup_lawyer"   
  "powerup_weapons"
  "powerup_engine"  
  "powerup_repair"
  "garage" "garage_lod"
  "airbase"
  
  "mainbase"
  "mainbasepad"
  "camera"
  "tube"

  ; vehicle blow-chunks
  "chunk_chopper_blade"          "chunk_missiletruck_tread"
  "chunk_chopper_body"           "chunk_peon_barrel"
  "chunk_chopper_tail"           "chunk_peon_body"
  "chunk_engineer_front"         "chunk_peon_turret"
  "chunk_engineer_rear"          "chunk_trike_body"
  "chunk_engineer_tire"          "chunk_trike_tread"
  "chunk_missiletruck_body"      "chunk_trike_wheels"
  "chunk_missiletruck_launcher"
  "team_flag"
  "norwayflag"
  "egyptflag"
;  "englandflag"
  "finlandflag"
  "franceflag"
  "germanyflag"
  "golgflag"
  "nephflag"
  "greeceflag"
  "iraqflag"
  "israelflag"
  "luxembourgflag"
  "swedenflag"
  "scotlandflag"
  "blackred"

  "lock_on"
  "lock_own"
  "lock_on_guns"

  "crate_money"
  "crate_bullets"
  "crate_missiles"
  "crate_health"
  "crate_minigun"


  "powerup_flare"
  "powerup_flare_white"
  "powerup_flare_green"
  "powerup_flare_blue"
  "powerup_flare_red"
  "powerup_flare_yellow"
  "command-stop"
  "command-self_destruct"
  "command-control"
  )
  
(load "scheme/objects.scm")
  
(setf world_scaling 1.0)

;(models
; building_format
; "floorpad" ; This is actually an alternate name for factory_pad.
 ; Replace if needed.
; "tube"
;)

/* these textures are used by programmers only - they cannot be used as game tiles */
(textures
 texture_format

 ("charredvehicle"       (selectable nil))
 ("bullet_particle"      (selectable nil))
 ("smoke_particle"       (selectable nil))
 ("smoke_ring"           (selectable nil))
 ("explosions2"          (selectable nil))
 ("vertical_plasma"      (selectable nil))
 ("pad_type_bank"        (selectable nil))
 ("pad_type_lawfirm"     (selectable nil))
 ("frame_metal"          (selectable nil))
 ("happy"                (selectable nil))
 ("usa"                  (selectable nil))
 ("russia"               (selectable nil))
 ("crackdotcom")
 ("golglogo")
 ("red_flare"            (selectable nil))
; ("blue_flare"           (selectable nil))
 ("acid"                 (selectable nil))
 ("auto1"                (selectable nil))
 ("auto2"                (selectable nil))
 ("tron_grid")
 ("target_cursor")
  "gun_target_cursor"
  "nuk_sky"
  "nuk_sky1"
  "nuk_sky2"
  "skycloudyblue1"
  "skycloudyblue2"
  "lil_flak"
  "fire_gradient"
)

(def_buildings
  ("bridge_end" deco_invulnerable deco_usepolycoldet)
  "sacred_ring"
  "bridger_road"
  ("tunnel_entrance" deco_invulnerable deco_usepolycoldet)
  "secret_room_small"
  ("takeover_tunnel" deco_invulnerable deco_usepolycoldet)
  ("tunnel" deco_invulnerable deco_usepolycoldet)
  "garage_charred"
  ("floorpad" deco_invulnerable deco_usepolycoldet)
    ("mainbasefull010698" deco_usepolycoldet)
    "mainbasefull010698_lod"
)


(def_skys 
  texture_format
  "nuk_sky"
  "skycloudyblue"
  "greecesky"
  "sky1_farmland" 
  "sky1bluehills-a" 
  "sky2ssetsunsetdunes-a" 
  "sky1"
  "sky2"
  "sky3"
  "sky5"
  "sky6"
  "sky7"
  "sky6mistymorn-a"
;;  "sky_62498_lod"
;;  "skycloudyblue1"
;;  "skycloudyblue2"

)



