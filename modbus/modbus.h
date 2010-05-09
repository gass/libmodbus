/*
 * Copyright © 2001-2010 Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
/**
 * SECTION:modbus
 * @short_description: library's symbols reference
 * @title: LibModbus API - Reference
 * @section_id:
 * @see_also:
 * @stability: Stable
 * @include: modbus.h
 * @Image:
 *
 * The application class handles ...
 */


#ifndef _MODBUS_H_
#define _MODBUS_H_

#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif
#include <termios.h>
#if defined(__FreeBSD__ ) && __FreeBSD__ < 5
#include <netinet/in_systm.h>
#endif
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#include <modbus/version.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MODBUS_TCP_DEFAULT_PORT   502
#define MODBUS_BROADCAST_ADDRESS  255

/* Slave index */
#define HEADER_LENGTH_RTU           1
#define PRESET_QUERY_LENGTH_RTU     6
#define PRESET_RESPONSE_LENGTH_RTU  2

#define HEADER_LENGTH_TCP           7
#define PRESET_QUERY_LENGTH_TCP    12
#define PRESET_RESPONSE_LENGTH_TCP  8

#define CHECKSUM_LENGTH_RTU         2
#define CHECKSUM_LENGTH_TCP         0

/* It's not really the minimal length (the real one is report slave ID
 * in RTU (4 bytes)) but it's a convenient size to use in RTU or TCP
 * communications to read many values or write a single one.
 * Maximum between :
 * - HEADER_LENGTH_TCP (7) + function (1) + address (2) + number (2)
 * - HEADER_LENGTH_RTU (1) + function (1) + address (2) + number (2) + CRC (2)
*/
#define MIN_QUERY_LENGTH           12

/* Modbus_Application_Protocol_V1_1b.pdf Chapter 4 Section 1 Page 5:
 *  - RS232 / RS485 ADU = 253 bytes + slave (1 byte) + CRC (2 bytes) = 256 bytes
 *  - TCP MODBUS ADU = 253 bytes + MBAP (7 bytes) = 260 bytes
 */
#define MAX_PDU_LENGTH            253
#define MAX_ADU_LENGTH_RTU        256
#define MAX_ADU_LENGTH_TCP        260

/* Kept for compatibility reasons (deprecated) */
#define MAX_MESSAGE_LENGTH        260

#define EXCEPTION_RESPONSE_LENGTH_RTU  5

/* Modbus_Application_Protocol_V1_1b.pdf (chapter 6 section 1 page 12)
 * Quantity of Coils (2 bytes): 1 to 2000 (0x7D0)
 */
#define MAX_STATUS               2000

/* Modbus_Application_Protocol_V1_1b.pdf (chapter 6 section 3 page 15)
 * Quantity of Registers (2 bytes): 1 to 125 (0x7D)
 */
#define MAX_REGISTERS             125

#define REPORT_SLAVE_ID_LENGTH     75

/* Time out between trames in microsecond */
#define TIME_OUT_BEGIN_OF_TRAME 500000
#define TIME_OUT_END_OF_TRAME   500000

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef OFF
#define OFF 0
#endif

#ifndef ON
#define ON 1
#endif

/* Function codes */
/**
 * FC_READ_COIL_STATUS:
 *
 * Reads the ON/OFF status of discrete outputs (0X references, coils) in the slave.
 * Broadcast is not supported.
 */
#define FC_READ_COIL_STATUS          0x01
/**
 * FC_READ_INPUT_STATUS:
 *
 * Reads the ON/OFF status of discrete inputs (1X references) in the slave.
 * Broadcast is not supported.
 */
#define FC_READ_INPUT_STATUS         0x02  /* discretes outputs */
/**
 * FC_READ_HOLDING_REGISTERS:
 *
 * Reads the binary contents of holding registers (4X references) in the slave.
 * Broadcast is not supported.
 */
#define FC_READ_HOLDING_REGISTERS    0x03
/**
 * FC_READ_INPUT_REGISTERS:
 *
 *Reads the binary contents of input registers (3X references) in the slave.
 * Broadcast is not supported.

 */
#define FC_READ_INPUT_REGISTERS      0x04
/**
 * FC_FORCE_SINGLE_COIL:
 *
 * Forces a single coil (0X reference) to either ON or OFF. When broadcast, the
 * function forces the same coil reference in all attached slaves.
 * <note> The function will override the controller’s memory protect state
 * and the coil’s disable state. The forced state will remain valid until the
 * controller’s logic next solves the coil. The coil will remain forced if it is
 * not programmed in the controller’s logic. </note>
 */
#define FC_FORCE_SINGLE_COIL         0x05
/**
 * FC_PRESET_SINGLE_REGISTER:
 *
 * Presets a value into a single holding register (4X reference). When broadcast, the
 * function presets the same register reference in all attached slaves.
 * <note> The function will override the controller’s memory protect state.
 * The preset value will remain valid in the register until the controller’s
 * logic next solves the register contents. The register’s value will remain
 * if it is not programmed in the controller’s logic.</note>
 */
#define FC_PRESET_SINGLE_REGISTER    0x06
/**
 * FC_READ_EXCEPTION_STATUS:
 *
 * Reads the contents of eight Exception Status coils within the slave controller.
 * Certain coils have predefined assignments in the various controllers. Other coils
 * can be programmed by the user to hold information about the contoller’s status,
 * for example, ‘machine ON/OFF’, ‘heads retracted’, ‘safeties satisfied’, ‘error
 * conditions exist’, or other user–defined flags. Broadcast is not supported.
 */
#define FC_READ_EXCEPTION_STATUS     0x07
/**
 * FC_FORCE_MULTIPLE_COILS:
 *
 * Forces each coil (0X reference) in a sequence of coils to either ON or OFF. When
 * broadcast, the function forces the same coil references in all attached slaves.
 * <note> The function will override the controller’s memory protect state
 * and a coil’s disable state. The forced state will remain valid until the
 * controller’s logic next solves each coil. Coils will remain forced if they
 * are not programmed in the controller’s logic.</note>
 */
#define FC_FORCE_MULTIPLE_COILS      0x0F
/**
 * FC_PRESET_MULTIPLE_REGISTERS:
 * 
 * Presets values into a sequence of holding registers (4X references). When
 * broadcast, the function presets the same register references in all attached slaves.
 * <note> The function will override the controller’s memory protect state.
 * The preset values will remain valid in the registers until the controller’s
 * logic next solves the register contents. The register values will remain
 * if they are not programmed in the controller’s logic.</note>
 */
#define FC_PRESET_MULTIPLE_REGISTERS 0x10
/**
 * FC_REPORT_SLAVE_ID:
 *
 * Returns a description of the type of controller present at the slave address, the
 * current status of the slave Run indicator, and other information specific to the
 * slave device. Broadcast is not supported.
 */
#define FC_REPORT_SLAVE_ID           0x11

/* Protocol exceptions */
#define ILLEGAL_FUNCTION        -0x01
#define ILLEGAL_DATA_ADDRESS    -0x02
#define ILLEGAL_DATA_VALUE      -0x03
#define SLAVE_DEVICE_FAILURE    -0x04
#define SERVER_FAILURE          -0x04
#define ACKNOWLEDGE             -0x05
#define SLAVE_DEVICE_BUSY       -0x06
#define SERVER_BUSY             -0x06
#define NEGATIVE_ACKNOWLEDGE    -0x07
#define MEMORY_PARITY_ERROR     -0x08
#define GATEWAY_PROBLEM_PATH    -0x0A
#define GATEWAY_PROBLEM_TARGET  -0x0B

/* Local */
#define INVALID_DATA            -0x10
#define INVALID_CRC             -0x11
#define INVALID_EXCEPTION_CODE  -0x12

#define SELECT_TIMEOUT          -0x13
#define SELECT_FAILURE          -0x14
#define SOCKET_FAILURE          -0x15
#define CONNECTION_CLOSED       -0x16
#define MB_EXCEPTION             -0x17

/* Internal using */
#define MSG_LENGTH_UNDEFINED -1

typedef enum { RTU=0, TCP } type_com_t;
typedef enum { FLUSH_OR_CONNECT_ON_ERROR, NOP_ON_ERROR } error_handling_t;

/**
 * modbus_param_t:
 * @slave: Slave address.
 * @fd: Descriptor (tty or socket).
 * @type_com: The communication mode: RTU or TCP.
 * @debug: Debug flag.
 * @port: The TCP port to use.
 * @device: "/dev/ttyS0", "/dev/ttyUSB0" or "/dev/tty.USA19*"
 *         on Mac OS X for KeySpan USB<->Serial adapters this string
 *         had to be made bigger on OS X as the directory+file name
 *         was bigger than 19 bytes. Making it 67 bytes for now, but
 *         OS X does support 256 byte file names. May become a problem
 *         in the future.
 * @baud: BaudRate :Bauds: 9600, 19200, 57600, 115200, etc.
 * @data_bit: Data bit.
 * @stop_bit: Stop bit.
 * @parity: Parity: "even", "odd", "none".
 * @error_handling: In error_treat with TCP, do a reconnect or just dump the error.
 * @ip: IP Address.
 * @old_tios: Save old termios settings. 
 *
 * The communication definition data structure.
 * This structure is byte-aligned.
 */

typedef struct {
        /*< public >*/
        int slave;
        int fd;
        type_com_t type_com;
        int debug;
        int port;
#ifdef __APPLE_CC__
        char device[64];
#else
        char device[16];
#endif
        int baud;
        uint8_t data_bit;
        uint8_t stop_bit;
        char parity[5];
        uint8_t error_handling;
        char ip[16];
        struct termios old_tios;
} modbus_param_t;

typedef struct {
        int nb_coil_status;
        int nb_input_status;
        int nb_input_registers;
        int nb_holding_registers;
        uint8_t *tab_coil_status;
        uint8_t *tab_input_status;
        uint16_t *tab_input_registers;
        uint16_t *tab_holding_registers;
} modbus_mapping_t;


/* All functions used for sending or receiving data return:
   - the numbers of values (bits or word) if success (0 or more)
   - less than 0 for exceptions errors
*/

/**
 * read_coil_status:
 * @mb_param: The connection parameters #modbus_param_t
 * @start_addr: the start address to be read
 * @nb: number of bytes to read
 * @dest: destination array
 *
 * Reads the boolean status of coils and sets the array elements in
 * the destination to TRUE or FALSE.
 *
 * Returns: The number of bits or words if success (>0) or an error exception (<0).
 */
int read_coil_status(modbus_param_t *mb_param, int start_addr, int nb,
                     uint8_t *dest);

/**
 * read_input_status:
 * @mb_param: The connection parameters #modbus_param_t
 * @start_addr: the start address to be read
 * @nb: number of bytes to read
 * @dest: destination array
 *
 *  Same as read_coil_status but reads the slaves input table.
 *
 * Returns: The number of bits or words if success (>0) or an error exception (<0).
 */
int read_input_status(modbus_param_t *mb_param, int start_addr, int nb,
                      uint8_t *dest);

/**
 * read_holding_registers:
 * @mb_param: The connection parameters #modbus_param_t
 * @start_addr: the start address to be read
 * @nb: number of bytes to read
 * @dest: destination array
 *
 * Reads the holding registers in a slave and put the data into an
 * array.
 *
 * Returns: The number of bits or words if success (>0) or an error exception (<0).
 */
int read_holding_registers(modbus_param_t *mb_param, int start_addr, int nb,
                           uint16_t *dest);

/**
 * read_input_registers:
 * @mb_param: The connection parameters #modbus_param_t
 * @start_addr: the start address to be read
 * @nb: number of bytes to read
 * @dest: destination array
 *
 * Reads the input registers in a slave and put the data into an
 * array.
 *
 * Returns: The number of bits or words if success (>0) or an error exception (<0).
 */
int read_input_registers(modbus_param_t *mb_param, int start_addr, int nb,
                         uint16_t *dest);

/**
 * force_single_coil:
 * @mb_param: The connection parameters #modbus_param_t
 * @coil_addr:
 * @state:
 *
 * Turns ON or OFF a single coil in the slave device
 *
 * Returns: The number of bits or words if success (>0) or an error exception (<0).
 */
int force_single_coil(modbus_param_t *mb_param, int coil_addr, int state);

/**
 * preset_single_register:
 * @mb_param: The connection parameters #modbus_param_t
 *
 * Sets a value in one holding register in the slave device.
 *
 * Returns: The number of bits or words if success (>0) or an error exception (<0).
 */
int preset_single_register(modbus_param_t *mb_param, int reg_addr, int value);

/**
 * force_multiple_coils:
 * @mb_param: The connection parameters #modbus_param_t
 *
 * Sets/resets the coils in the slave from an array in argument.
 *
 * Returns: The number of bits or words if success (>0) or an error exception (<0).
 */
int force_multiple_coils(modbus_param_t *mb_param, int start_addr, int nb,
                         const uint8_t *data);

/**
 * preset_multiple_registers:
 * @mb_param: The connection parameters #modbus_param_t
 *
 * Copies the values in the slave from the array given in argument.
 *
 * Returns: The number of bits or words if success (>0) or an error exception (<0).
 */
int preset_multiple_registers(modbus_param_t *mb_param, int start_addr, int nb,
                              const uint16_t *data);

/**
 * report_slave_id:
 * @mb_param: The connection parameters #modbus_param_t
 *
 * Returns the slave id!.
 *
 * Returns: The slave id!
 */
int report_slave_id(modbus_param_t *mb_param, uint8_t *dest);

/**
 * modbus_init_rtu:
 * @mb_param: The connection parameters #modbus_param_t
 * @device: The device to be used for comunication.
 * Example: "/dev/ttyS0"
 * @baud: The communication's BaudRate: 9600, 19200, 57600, 115200, etc.
 * @parity: The parity definition: "even", "odd" or "none".
 * @data_bit: Comunication databits: 5, 6, 7, 8.
 * @stop_bit: Number of stop bits: 1, 2.
 *
 * Initializes the #modbus_param_t structure for RTU.
 */

void modbus_init_rtu(modbus_param_t *mb_param, const char *device,
                     int baud, const char *parity, int data_bit,
                     int stop_bit, int slave);

/**
 * modbus_init_tcp:
 * @mb_param: The connection parameters #modbus_param_t
 * @ip_address:
 * @port:
 * @slave:
 *
 *Initializes the #modbus_param_t structure for TCP.
 * - ip: "192.168.0.5"
 * - port: 1099
 * - slave: 5
 *
 * Set the port to MODBUS_TCP_DEFAULT_PORT to use the default one
 * (502). It's convenient to use a port number greater than or equal
 * to 1024 because it's not necessary to be root to use this port
 * number.
 */
void modbus_init_tcp(modbus_param_t *mb_param, const char *ip_address, int port,
                     int slave);

/**
 * modbus_set_slave:
 * @mb_param: The connection parameters #modbus_param_t
 * @slave: The slave address.
 *
 * Define the slave number.
 * The special value MODBUS_BROADCAST_ADDRESS can be used.
 */
void modbus_set_slave(modbus_param_t *mb_param, int slave);

/**
 * modbus_set_error_handling:
 * @mb_param: The connection parameters #modbus_param_t
 * @error_handling:
 *
 * By default, the error handling mode used is CONNECT_ON_ERROR.
 *
 * With FLUSH_OR_CONNECT_ON_ERROR, the library will attempt an immediate
 * reconnection which may hang for several seconds if the network to
 * the remote target unit is down.
 *
 * With NOP_ON_ERROR, it is expected that the application will
 * check for network error returns and deal with them as necessary.
 *
 * This function is only useful in TCP mode.
 */
void modbus_set_error_handling(modbus_param_t *mb_param, error_handling_t error_handling);

/**
 * modbus_connect:
 * @mb_param: The connection parameters #modbus_param_t
 *
 * Establishes a modbus connexion.
 *
 * Returns: 0 on success or -1 on failure.
 */
int modbus_connect(modbus_param_t *mb_param);

/**
 * modbus_close:
 * @mb_param: The connection parameters #modbus_param_t
 *
 * Closes a modbus connection.
 */
void modbus_close(modbus_param_t *mb_param);

/**
 * modbus_flush:
 * @mb_param: The connection parameters #modbus_param_t
 *
 * Flush the pending request.
 */
void modbus_flush(modbus_param_t *mb_param);

/**
 *modbus_set_debug:
 * @mb_param: The connection parameters #modbus_param_t
 * @boolean:
 *
 * Activates the debug messages.
 */
void modbus_set_debug(modbus_param_t *mb_param, int boolean);

/*
 * SLAVE/CLIENT FUNCTIONS
 **/

/**
 * modbus_mapping_new:
 * @mb_mapping:
 * @nb_coil_status:
 * @nb_input_status:
 * @nb_holding_registers:
 * @nb_input_registers:
 *
 * Allocates 4 arrays to store coils, input status, input registers and
 * holding registers. The pointers are stored in modbus_mapping structure.
 *
 * Returns: 0 on success and -1 on failure
 */
int modbus_mapping_new(modbus_mapping_t *mb_mapping,
                       int nb_coil_status, int nb_input_status,
                       int nb_holding_registers, int nb_input_registers);

/**
 * modbus_mapping_free:
 * @mb_mapping:
 *
 Frees the 4 arrays.
 */
void modbus_mapping_free(modbus_mapping_t *mb_mapping);

/**
 * modbus_slave_listen_tcp:
 * @mb_param: The connection parameters #modbus_param_t
 * @nb_connection:
 *
 * Listens for any query from one or many modbus masters in TCP.
 *
 * Returns: socket
 */
int modbus_slave_listen_tcp(modbus_param_t *mb_param, int nb_connection);

/**
 * modbus_slave_accept_tcp:
 * @mb_param: The connection parameters #modbus_param_t
 * @socket:
 *
 * Waits for a connection.
 *
 * Returns:
 */
int modbus_slave_accept_tcp(modbus_param_t *mb_param, int *socket);

/**
 * modbus_slave_receive:
 * @mb_param: The connection parameters #modbus_param_t
 * @sockfd:
 * @query:
 *
 * Listens for any query from a modbus master in TCP, requires the socket file
 * descriptor etablished with the master device in argument or -1 to use the
 * internal one of modbus_param_t.
 *
 * Returns:
 * - byte length of the message on success, or a negative error number if the
 *   request fails
 * - query, message received
 */
int modbus_slave_receive(modbus_param_t *mb_param, int sockfd, uint8_t *query);

/**
 * modbus_slave_manage:
 * @mb_param: The connection parameters #modbus_param_t
 * @query:
 * @query_length:
 * @mb_mapping:
 *
 * Manages the received query.
 * Analyses the query and constructs a response.
 *
 *  If an error occurs, this function construct the response
 *  accordingly.
 */
void modbus_slave_manage(modbus_param_t *mb_param, const uint8_t *query,
                         int query_length, modbus_mapping_t *mb_mapping);

/**
 * modbus_slave_close_tcp:
 * @socket: Socket fd.
 *
 * Closes a TCP socket.
 *
 */
void modbus_slave_close_tcp(int socket);

/*
 * UTILS FUNCTIONS
 **/

/**
 * set_bits_from_byte:
 * @dest:
 * @address:
 * @value:
 *
 * Sets many input/coil status from a single byte value (all 8 bits of
 * the byte value are set).
 *
 */
void set_bits_from_byte(uint8_t *dest, int address, const uint8_t value);

/**
 * set_bits_from_bytes:
 * @dest:
 * @address:
 * @nb_bits:
 * @tab_byte:
 *
 * Sets many input/coil status from a table of bytes (only the bits
 * between address and address + nb_bits are set).
 *
 */
void set_bits_from_bytes(uint8_t *dest, int address, int nb_bits,
                         const uint8_t *tab_byte);

/**
 * get_byte_from_bits:
 * @src:
 * @nb_bits:
 *
 * Gets the byte value from many input/coil status.
 * To obtain a full byte, set nb_bits to 8.
 *
 * Returns:
 */
uint8_t get_byte_from_bits(const uint8_t *src, int address, int nb_bits);

/**
 * modbus_read_float:
 * @src:
 *
 * Read a float from 4 bytes in Modbus format.
 *
 * Returns: the value read.
 */
float modbus_read_float(const uint16_t *src);

/**
 * modbus_write_float:
 * @real:
 * @dest:
 *
 * Write a float to 4 bytes in Modbus format.
 */
void modbus_write_float(float real, uint16_t *dest);

#ifdef __cplusplus
}
#endif

#endif  /* _MODBUS_H_ */
