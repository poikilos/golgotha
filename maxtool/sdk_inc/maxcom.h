/*********************************************************************

 *<

	FILE: #define MAXCOM.h



    DESCRIPTION: Command IDs passed to Interface::Execute#define MAXCOMmand()



	CREATED BY:	Rolf Berteig



	HISTORY: Created 3/13/96



 *>	Copyright (c) 1994 All Rights Reserved.

 **********************************************************************/



#ifndef __MAXCOM__

#define __MAXCOM__



#define MAXCOM_RESET_FILE				0	

#define MAXCOM_TIME_CONFIG				1

#define MAXCOM_UNFREEZE_BY_HIT			2

#define MAXCOM_BOX_TOGGLE				3

#define MAXCOM_CYCLE_SELECT_METHOD		4

#define MAXCOM_ZOOM_OUT_2X_ALL			5

#define MAXCOM_ZOOM_IN_2X_ALL			6

#define MAXCOM_IZOOM_OUT				7

#define MAXCOM_IZOOM_IN					8

#define MAXCOM_IPAN 					9

#define MAXCOM_SHOW_LAST_IMG 			10

#define MAXCOM_APPLY_IK 				11

#define MAXCOM_KEY_MODE 				12

#define MAXCOM_TOGGLE_IK 				13

#define MAXCOM_SHADE_SELECTED 			14

#define MAXCOM_SELECT_BY_COLOR 			15

#define MAXCOM_ZOOMEXT_SEL 				16

#define MAXCOM_ZOOMEXT_SEL_ALL 			17

#define MAXCOM_CREATE_MODE 				18

#define MAXCOM_MODIFY_MODE 				19

#define MAXCOM_HIERARCHY_MODE 			20

#define MAXCOM_MOTION_MODE 				21

#define MAXCOM_DISPLAY_MODE 			22

#define MAXCOM_UTILITY_MODE 			23

#define MAXCOM_TEXTURE_CORRECT 			24

#define MAXCOM_ZOOM_OUT_2X 				25

#define MAXCOM_ZOOM_IN_2X 				26

#define MAXCOM_DEF_LGT_TOGGLE 			27

#define MAXCOM_VPT_SHAPE 				28

#define MAXCOM_GROUP_ATTACH 			29

#define MAXCOM_GROUP_DETACH 			30

#define MAXCOM_PREV_MOD 				31

#define MAXCOM_NEXT_MOD 				32

#define MAXCOM_SAVEPLUS 				33

#define MAXCOM_VIEW_FILE 				34

#define MAXCOM_UNHIDE_BY_NAME 			35

#define MAXCOM_UNFREEZE_BY_NAME 		36

#define MAXCOM_SPINSNAP_TOGGLE 			37

#define MAXCOM_HIDE_INV 				38

#define MAXCOM_FREEZE_INV 				39

#define MAXCOM_UNFREEZE_ALL 			40

#define MAXCOM_WIRE_SMOOTH 				41

#define MAXCOM_WIRE_FACET 				42

#define MAXCOM_BOX_MODE 				43

#define MAXCOM_BACKFACE  				44

#define MAXCOM_TRAJECTORIES 			45

#define MAXCOM_UNHIDE_ALL  				46

#define MAXCOM_SCALE_CYCLE				47

#define MAXCOM_IK_TERMINATOR			48

#define MAXCOM_RENDER_SCENE 			49

#define MAXCOM_RENDER_LAST 				50

#define MAXCOM_QUICK_RENDER 			51

#define MAXCOM_GRID_NUDGE_UP 			52

#define MAXCOM_GRID_NUDGE_DOWN 			53

#define MAXCOM_CYCLE_SUBLEVEL 			54

#define MAXCOM_HIDE_SELECTION 			55

#define MAXCOM_FREEZE_SELECTION 		56

#define MAXCOM_SHADE_TOGGLE				57

#define MAXCOM_MIRROR 					58

#define MAXCOM_ARRAY 					60

#define MAXCOM_ALIGN 					61

#define MAXCOM_ALIGNNORMALS 			62

#define MAXCOM_HOLD  					63

#define MAXCOM_FETCH 					64

#define MAXCOM_SWAP_LAYOUTS 			65

#define MAXCOM_SAFEFRAME_TOGGLE 		66

#define MAXCOM_FILE_MERGE 				67

#define MAXCOM_TIME_BACK 				68

#define MAXCOM_TIME_FORWARD 			69

#define MAXCOM_TIME_PLAY 				70

#define MAXCOM_VIEWS_REDRAW 			71

#define MAXCOM_UNITSETUP				72

#define MAXCOM_DRAWINGAIDS				73

#define MAXCOM_SHOWHOMEGRID				74

#define MAXCOM_ACTHOMEGRID				75

#define MAXCOM_ACTGRIDOBJ				76

#define MAXCOM_GRIDS_ALIGN				77

#define MAXCOM_BACKGROUND				78

#define MAXCOM_SHOWAXISICON				79

#define MAXCOM_FULLINTERACT				80

#define MAXCOM_VPTCONFIG				81

#define MAXCOM_VIDEOPOST				82

#define MAXCOM_PREVIEW					83

#define MAXCOM_VIEWPREVIEW				84

#define MAXCOM_RENAMEPREVIEW			85

#define MAXCOM_TOOL_DUALPLANES 			86

#define MAXCOM_LINK						87

#define MAXCOM_UNLINK					88

#define MAXCOM_BINDWSM					89

#define MAXCOM_SELECT					90

#define MAXCOM_MOVE						91

#define MAXCOM_ROTATE					92

#define MAXCOM_SCALE					93

#define MAXCOM_TREEVIEW					94

#define MAXCOM_MTLEDIT					95

#define MAXCOM_PANVIEW					96

#define MAXCOM_DOLLY					97

#define MAXCOM_PERSP					98

#define MAXCOM_ROLL						99

#define MAXCOM_FOV						100

#define MAXCOM_TRUCK					101

#define MAXCOM_PANCAMERA				102

#define MAXCOM_ANGLE_SNAP_TOGGLE 		103

#define MAXCOM_EDIT_REDO 				104

#define MAXCOM_VIEW_REDO 				105

#define MAXCOM_VPT_TRACK 				106

#define MAXCOM_VPT_BOTTOM 				107

#define MAXCOM_SUBOBJECT_SEL 			108

#define MAXCOM_VPT_CAMERA 				109

#define MAXCOM_VPT_SPOT 				110

#define MAXCOM_HIDE_CAMERA_TOGGLE 		111

#define MAXCOM_VPT_DISABLE 				112

#define MAXCOM_VPT_FRONT 				113

#define MAXCOM_VPT_GRID 				114

#define MAXCOM_GRID_TOGGLE 				115

#define MAXCOM_TOOL_HLIST 				116

#define MAXCOM_HIDE_HELPER_TOGGLE 		117

#define MAXCOM_VPT_ISO_USER 			118

#define MAXCOM_VPT_BACK 				119

#define MAXCOM_VPT_LEFT 				120

#define MAXCOM_HIDE_LIGHT_TOGGLE 		121

#define MAXCOM_TOOL_ANIMMODE 			122

#define MAXCOM_FILE_NEW 				123

#define MAXCOM_OVERRIDE 				124

#define MAXCOM_FILE_OPEN 				125

#define MAXCOM_HIDE_OBJECT_TOGGLE 		126

#define MAXCOM_VPT_PERSP_USER 			127

#define MAXCOM_ACCEL_PAN 				128

#define MAXCOM_HIDE_SHAPE_TOGGLE 		129

#define MAXCOM_VPT_RIGHT 				130

#define MAXCOM_ROTATEVIEW 				131

#define MAXCOM_SNAP_TOGGLE 				132

#define MAXCOM_FILE_SAVE 				133

#define MAXCOM_FILE_SAVEAS 				134

#define MAXCOM_FILE_IMPORT 				135

#define MAXCOM_FILE_PREFERENCES			136

#define MAXCOM_HIDE_SYSTEM_TOGGLE 		137

#define MAXCOM_VPT_TOP 					138

#define MAXCOM_EDIT_DELETE 				139

#define MAXCOM_EDIT_SELECTALL			140

#define MAXCOM_EDIT_SELECTNONE			141

#define MAXCOM_EDIT_SELECTINVERT		142

#define MAXCOM_RNS						143

#define MAXCOM_TTI						144

#define MAXCOM_PROPERTIES				145

#define MAXCOM_GROUP_GROUP				146

#define MAXCOM_GROUP_OPEN				147

#define MAXCOM_GROUP_CLOSE				148

#define MAXCOM_GROUP_UNGROUP			149

#define MAXCOM_TIME_END 				150

#define MAXCOM_HELP_ABOUT 				151

#define MAXCOM_TOOL_X 					152

#define MAXCOM_TOOL_Y 					153

#define MAXCOM_TOOL_Z 					154

#define MAXCOM_TOOL_XY 					155

#define MAXCOM_TIME_START 				156

#define MAXCOM_SELECT_CHILD 			157

#define MAXCOM_SELECT_PARENT 			158

#define MAXCOM_SPACEBAR 				159

#define MAXCOM_TOOL_MAXIMIZE 			160

#define MAXCOM_TOOL_ZOOMREGION 			161

#define MAXCOM_HIDE_WSM_TOGGLE 			162

#define MAXCOM_TOOL_CENTER 				163

#define MAXCOM_TOOL_ZOOM 				164

#define MAXCOM_TOOL_ZOOMALL				165

#define MAXCOM_EDIT_UNDO 				166

#define MAXCOM_TOOL_ZOOMEXTENTS 		167

#define MAXCOM_VIEWS_UNDO 				168

#define MAXCOM_TOOL_ZOOMEXTENTS_ALL 	169

#define MAXCOM_TOGGLE_SOUND 			170

#define MAXCOM_VPT_TAB 					171

#define MAXCOM_VPT_SHIFTTAB 			172

#define MAXCOM_CONFIGURE_PATHS 			173

#define MAXCOM_ADAPTIVE_PERSP_GRID_TOGGLE	174

										  

#endif									   	 

