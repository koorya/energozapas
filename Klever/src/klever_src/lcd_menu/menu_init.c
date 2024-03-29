#include "screen.h"

void menuInit(void){
	static ITEM_OBJ obj[35];
	static WINDOW_TYPE menu_window[20];
	static ITEM_TYPE item[55];
	static uint32_t item_index = 0;
	static uint32_t obj_index = 0;
	
		initWindow(W(MAIN), "�������", 0);
		initWindow(W(DATA_MANAGE), "���� ������", W(MAIN));
		initWindow(W(LIMITS), "���� �������", W(MAIN));
		initWindow(W(MODE), "���� �������", W(MAIN));	
			initWindow(W(FORMULAS), "����", W(MODE));
			initWindow(W(PID_MODE), "����� ���", W(MODE));
			initWindow(W(PULS_MODE), "����� ���������", W(MODE));
			initWindow(W(LAZY_PID_MODE), "����� ������� ���", W(MODE));


	initItemParameter(&(item[item_index++]), "*������*", W(MAIN), initObjNotchangeable(&(obj[obj_index++]), show_status), 0, 0, unvisible_name);
	initItemParameter(&(item[item_index++]), "*������ ������*", W(MAIN), initObjNotchangeable(&(obj[obj_index++]), show_interface_error_state), 8, 0, unvisible_name);
	initItemParameter(&(item[item_index++]), "*alarm*", W(MAIN), initObjNotchangeable(&(obj[obj_index++]), show_alarm), 0, 1, unvisible_name);  
	initItemParameter(&(item[item_index++]), "������", W(MAIN), initObjNotchangeable(&(obj[obj_index++]), show_resurs), 0, 6, column);  
//	initItemParameter(&(item[item_index++]), "�����������", W(MAIN), initObjNotchangeable(&(obj[obj_index++]), show_tenzo), 0, 6, column);  
	
	initItemParameter(&(item[item_index++]), "��� ", W(MAIN), initObjNotchangeable(&(obj[obj_index++]), show_log_state), 11, 0, line);  
	initItemParameter(&(item[item_index++]), "��� ", W(MAIN), initObjNotchangeable(&(obj[obj_index++]), show_save_state), 11, 1, line);  

	initItemParameter(&(item[item_index++]), "�����", W(MAIN), initObjNotchangeable(&(obj[obj_index++]), show_mode), 11, 2, column);  
	initItemParameter(&(item[item_index++]), "�����", W(MAIN), initObjNotchangeable(&(obj[obj_index++]), show_up_limit_brackets), 11, 4, column);  
	initItemParameter(&(item[item_index++]), "�����", W(MAIN), initObjNotchangeable(&(obj[obj_index++]), show_cur_tension), 11, 6, column);  

	initItemSubmenu(&(item[item_index++]), "������  ", W(MAIN), W(DATA_MANAGE), 0, 2);  
		initItemLabel(&(item[item_index++]), "���� ������", W(DATA_MANAGE), 0, 0);
		initItemParameter(&(item[item_index++]), 	"����������� ", W(DATA_MANAGE), initObjChangeable(&(obj[obj_index++]), switch_log_state, switch_log_state, show_log_state_yes_no), 1, 2, line);  
		initItemParameter(&(item[item_index++]), 	"���������   ", W(DATA_MANAGE), initObjChangeable(&(obj[obj_index++]), switch_save_state, switch_save_state, show_save_state_yes_no), 1, 3, line);  
		initItemParameter(&(item[item_index++]), 	"�����       ", W(DATA_MANAGE), initObjChangeable(&(obj[obj_index++]), switch_default_state, switch_default_state, show_default_state), 1, 4, line);  
		initItemSubmenu(&(item[item_index++]), "<�����", W(DATA_MANAGE), W(MAIN), 0, 7);  
	initItemSubmenu(&(item[item_index++]), "������  ", W(MAIN), W(LIMITS), 0, 3);  
		initItemLabel(&(item[item_index++]), "���� �������", W(LIMITS), 0, 0);
		initItemParameter(&(item[item_index++]), 	"�������  ", W(LIMITS), initObjChangeable(&(obj[obj_index++]), inc_rope_tension_up_limit, dec_rope_tension_up_limit, show_rope_tension_up_limit), 1, 2, line);  
		initItemParameter(&(item[item_index++]), 	"������   ", W(LIMITS), initObjChangeable(&(obj[obj_index++]), inc_rope_tension_bottom_limit, dec_rope_tension_bottom_limit, show_rope_tension_bottom_limit), 1, 4, line);  
		initItemSubmenu(&(item[item_index++]), "<�����", W(LIMITS), W(MAIN), 0, 7);  
	
	initItemSubmenu(&(item[item_index++]), 		"������  ", W(MAIN), W(MODE), 0, 4);  
		initItemLabel(&(item[item_index++]), "���� �������", W(MODE), 0, 0);
		initItemSubmenu(&(item[item_index++]), 		"�������  ", W(MODE), W(FORMULAS), 1, 1);
			initItemLabel(&(item[item_index++]), "�������", W(FORMULAS), 0, 0);
			initItemParameter(&(item[item_index++]),"a ���.   ", W(FORMULAS), initObjChangeable(&(obj[obj_index++]), inc_angle_left, dec_angle_left, show_angle_left), 1, 2, line);
			initItemParameter(&(item[item_index++]),"a ����.  ", W(FORMULAS), initObjChangeable(&(obj[obj_index++]), inc_angle_right, dec_angle_right, show_angle_right), 1, 3, line);
			initItemSubmenu(&(item[item_index++]), "<�����", W(FORMULAS), W(MODE), 0, 7);
			initItemParameter(&(item[item_index++]), "����.", W(FORMULAS), initObjNotchangeable(&(obj[obj_index++]), show_cur_sensor), 11, 4, column);
			initItemParameter(&(item[item_index++]), "�����", W(FORMULAS), initObjNotchangeable(&(obj[obj_index++]), show_cur_tension), 11, 6, column);

		initItemParameter(&(item[item_index++]), 	"�� �����  ", W(MODE), initObjChangeable(&(obj[obj_index++]), switch_mode_not_reg_state, switch_mode_not_reg_state, show_mode_not_reg_state), 1, 2, line);  
		initItemSubmenu(&(item[item_index++]), 		"��� ����� ", W(MODE), W(PID_MODE), 1, 3);  
			initItemLabel(&(item[item_index++]), "����� ���", W(PID_MODE), 0, 0);
			initItemParameter(&(item[item_index++]), 	"�����     ", W(PID_MODE), initObjChangeable(&(obj[obj_index++]), switch_mode_pid_state, switch_mode_pid_state, show_mode_pid_state), 1, 2, line);  
			initItemParameter(&(item[item_index++]), 	"��������  ", W(PID_MODE), initObjChangeable(&(obj[obj_index++]), inc_mode_pid_value, dec_mode_pid_value, show_mode_pid_value), 1, 3, line);  
			initItemSubmenu(&(item[item_index++]), "<�����", W(PID_MODE), W(MODE), 0, 7);  
			initItemParameter(&(item[item_index++]), "�����", W(PID_MODE), initObjNotchangeable(&(obj[obj_index++]), show_cur_tension), 11, 6, column);  
		initItemSubmenu(&(item[item_index++]), 		"��� �����.", W(MODE), W(LAZY_PID_MODE), 1, 4);  
			initItemLabel(&(item[item_index++]), "����� ��� �����.", W(LAZY_PID_MODE), 0, 0);
			initItemParameter(&(item[item_index++]), 	"�����     ", W(LAZY_PID_MODE), initObjChangeable(&(obj[obj_index++]), switch_mode_lazy_pid_state, switch_mode_lazy_pid_state, show_mode_lazy_pid_state), 1, 2, line);  
			initItemParameter(&(item[item_index++]), 	"�������   ", W(LAZY_PID_MODE), initObjChangeable(&(obj[obj_index++]), inc_mode_lazy_pid_value, dec_mode_lazy_pid_value, show_mode_lazy_pid_value), 1, 3, line);  
			initItemParameter(&(item[item_index++]), 	"����      ", W(LAZY_PID_MODE), initObjChangeable(&(obj[obj_index++]), inc_mode_lazy_pid_err, dec_mode_lazy_pid_err, show_mode_lazy_pid_err), 1, 4, line);
			initItemParameter(&(item[item_index++]), 	"������    ", W(LAZY_PID_MODE), initObjChangeable(&(obj[obj_index++]), inc_mode_lazy_pid_period, dec_mode_lazy_pid_period, show_mode_lazy_pid_period), 1, 5, line);
			initItemSubmenu(&(item[item_index++]), "<�����", W(LAZY_PID_MODE), W(MODE), 0, 7);  
			initItemParameter(&(item[item_index++]), "�����", W(LAZY_PID_MODE), initObjNotchangeable(&(obj[obj_index++]), show_cur_tension), 11, 6, column);  
		initItemSubmenu(&(item[item_index++]), 		"��������� ", W(MODE), W(PULS_MODE), 1, 5);  
			initItemLabel(&(item[item_index++]), "����� ���������", W(PULS_MODE), 0, 0);
			initItemParameter(&(item[item_index++]), 	"�����     ", W(PULS_MODE), initObjChangeable(&(obj[obj_index++]), switch_mode_puls_state, switch_mode_puls_state, show_mode_puls_state), 1, 2, line);  
			initItemParameter(&(item[item_index++]), 	"��        ", W(PULS_MODE), initObjChangeable(&(obj[obj_index++]), inc_mode_puls_from_value, dec_mode_puls_from_value, show_mode_puls_from_value), 1, 3, line);  
			initItemParameter(&(item[item_index++]), 	"��        ", W(PULS_MODE), initObjChangeable(&(obj[obj_index++]), inc_mode_puls_to_value, dec_mode_puls_to_value, show_mode_puls_to_value), 1, 4, line);  
			initItemParameter(&(item[item_index++]), 	"������    ", W(PULS_MODE), initObjChangeable(&(obj[obj_index++]), inc_mode_puls_period_value, dec_mode_puls_period_value, show_mode_puls_period_value), 1, 5, line);  
			initItemSubmenu(&(item[item_index++]), "<�����", W(PULS_MODE), W(MODE), 0, 7);  
			initItemParameter(&(item[item_index++]), "�����", W(PULS_MODE), initObjNotchangeable(&(obj[obj_index++]), show_cur_tension), 11, 6, column);  
		initItemSubmenu(&(item[item_index++]), "<�����", W(MODE), W(MAIN), 0, 7);  
		initItemParameter(&(item[item_index++]), "�����", W(MODE), initObjNotchangeable(&(obj[obj_index++]), show_cur_tension), 11, 6, column);  

	initItemParameter(&(item[item_index++]), 	"����. ", W(MAIN), initObjChangeable(&(obj[obj_index++]), inc_freq, dec_freq, show_freq), 0, 5, line);  

	initScreen(200, &(item[0]));

}
