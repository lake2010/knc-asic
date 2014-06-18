#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <arpa/inet.h>

#include "neptune.h"
#include "eeprom.h"
#include "asic.h"

/* If port_num < 0 then:
 *   if port_num = -1 => use control-board eeprom
 *   if port_num = -2 => use BBB eeprom
 */
static int open_dev(int port_num, char *dev_fname, int oflag)
{
	int fd;

	if (port_num >= 0) {
		if (port_num >= MAX_ASICS) {
exit_wrong_port:
			fprintf(stderr, "Wrong port %d\n", port_num);
			return -1;
		}
		sprintf(dev_fname, EEPROM_FILE, port_num + FIRST_ASIC_I2C_BUS);
	} else {
		switch (port_num) {
		case -1:
			strcpy(dev_fname, CONTROL_BOARD_EEPROM_FILE);
			break;
		case -2:
			strcpy(dev_fname, BBB_EEPROM_FILE);
			break;
		default:
			goto exit_wrong_port;
		}
	}

	fd = open(dev_fname, oflag);
	if (fd < 0) {
		fprintf(stderr, "Error opening %s: %m\n", dev_fname);
		return fd;
	}

	return fd;
}

static bool read_buf(int fd, uint8_t *buf, unsigned int len)
{
	unsigned int tot_size = 0;
	int cur_size;

	while (tot_size < len) {
		cur_size = read(fd, buf, len - tot_size);
		if (cur_size <= 0)
			return false;
		tot_size += cur_size;
		buf += cur_size;
	};
	return true;
}

bool read_eeprom(int port_num, struct eeprom_neptune *data)
{
	int fd;
	char dev_fname[PATH_MAX];

	if (0 > (fd = open_dev(port_num, dev_fname, O_RDONLY)))
		return false;
	if (!read_buf(fd, (uint8_t *)data, sizeof(*data))) {
		close(fd);
		fprintf(stderr, "Error reading %s\n", dev_fname);
		return false;
	}
	close(fd);

	return true;
}

