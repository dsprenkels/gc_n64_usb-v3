#include <string.h>
#include "eeprom.h"

#include "requests.h"

struct eeprom_data_struct g_eeprom_data;

/* Called by the eeprom driver if the content
 * was invalid and it needs to write defaults
 * values.  */
void eeprom_app_write_defaults(void)
{
	int i;
	const char default_serial[SERIAL_NUM_LEN] = { '0','0','0','0','0','1' };

	memcpy(g_eeprom_data.cfg.serial, default_serial, SERIAL_NUM_LEN);
	g_eeprom_data.cfg.mode = CFG_MODE_STANDARD;

	for (i=0; i<NUM_CHANNELS; i++) {
		g_eeprom_data.cfg.poll_interval[i] = 5; // 5ms default
	}
}

static void config_set_serial(char serial[SERIAL_NUM_LEN])
{
	memcpy(g_eeprom_data.cfg.serial, serial, SERIAL_NUM_LEN);
	eeprom_commit();
}

unsigned char config_getParam(unsigned char param, unsigned char *value, unsigned char max_len)
{
	switch (param)
	{
		case CFG_PARAM_MODE:
			*value = g_eeprom_data.cfg.mode;
			return 1;
		case CFG_PARAM_SERIAL:
			memcpy(value, g_eeprom_data.cfg.serial, SERIAL_NUM_LEN);
			return SERIAL_NUM_LEN;
		case CFG_PARAM_POLL_INTERVAL0:
			*value = g_eeprom_data.cfg.poll_interval[0];
			return 1;
#if NUM_CHANNELS > 1
		case CFG_PARAM_POLL_INTERVAL1:
			*value = g_eeprom_data.cfg.poll_interval[1];
			return 1;
#endif
#if NUM_CHANNELS > 2
		case CFG_PARAM_POLL_INTERVAL2:
			*value = g_eeprom_data.cfg.poll_interval[2];
			return 1;
#endif
#if NUM_CHANNELS > 3
		case CFG_PARAM_POLL_INTERVAL3:
			*value = g_eeprom_data.cfg.poll_interval[3];
			return 1;
#endif
	}

	return 0;
}

unsigned char config_setParam(unsigned char param, const unsigned char *value)
{
	if (!value)
		return 0;

	switch (param)
	{
		case CFG_PARAM_MODE:
			g_eeprom_data.cfg.mode = value[0];
			break;
		case CFG_PARAM_SERIAL:
			config_set_serial((char*)value);
			break;
		case CFG_PARAM_POLL_INTERVAL0:
			g_eeprom_data.cfg.poll_interval[0] = value[0];
			break;
#if NUM_CHANNELS > 1
		case CFG_PARAM_POLL_INTERVAL1:
			g_eeprom_data.cfg.poll_interval[1] = value[0];
			break;
#endif
#if NUM_CHANNELS > 2
		case CFG_PARAM_POLL_INTERVAL2:
			g_eeprom_data.cfg.poll_interval[2] = value[0];
			break;
#endif
#if NUM_CHANNELS > 3
		case CFG_PARAM_POLL_INTERVAL3:
			g_eeprom_data.cfg.poll_interval[3] = value[0];
			break;
#endif
		default:
			return 0;
	}

	eeprom_commit();

	return 1;
}


