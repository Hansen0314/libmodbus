#include <stdio.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <modbus.h>
#define SERVER_ID       01
const uint16_t UT_BITS_ADDRESS = 0;
const uint16_t UT_BITS_NB = 1;
/* At each loop, the program works in the range ADDRESS_START to
 * ADDRESS_END then ADDRESS_START + 1 to ADDRESS_END and so on.
 */
int main(void)
{
    modbus_t *ctx = NULL;

    ctx = modbus_new_rtu("/dev/ttyACM0", 9600, 'N', 8, 1);
    if (ctx == NULL) {
        fprintf(stderr, "Unable to allocate libmodbus context\n");
        return -1;
    }
    modbus_set_debug(ctx, TRUE);
    modbus_set_error_recovery(ctx,
                              MODBUS_ERROR_RECOVERY_LINK |
                              MODBUS_ERROR_RECOVERY_PROTOCOL);
    modbus_set_slave(ctx, SERVER_ID);
    // modbus_get_response_timeout(ctx, &old_response_to_sec, &old_response_to_usec);
    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }    
    uint16_t fan_reg;
    /* End single */
    modbus_read_registers(ctx,UT_BITS_ADDRESS,UT_BITS_NB,&fan_reg);
    fprintf(stdout,"fan_reg is %x\r\n",fan_reg);

    modbus_close(ctx);
    modbus_free(ctx);
    ctx = NULL;
}