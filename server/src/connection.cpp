#include "connection.h"
#include "log.h"
#include <arpa/inet.h>
#include <cstdlib>
#include <err.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#define ERROR_SOCKET -2
#define ERROR_BIND -3
#define ERROR_LISTEN -4
#define ERROR_RECV -5
#define ERROR_SEND -6
#define QUEUE_LISTEN 10

Log* logger = nullptr;

//Init server ip, server port, socket descriptor, client ip, temperature and status air
Connection::Connection(string server_ip, int server_port)
	: server_ip(server_ip), server_port(server_port), socket_descriptor(0), client_ip(""),
		temperature(0.0), status_air(false)
{
	logger = Log::get_instance();

	control_uart_air(status_air);
}

//Close socket
Connection::~Connection()
{
	if (socket_descriptor)
		close(socket_descriptor);
}

//Open socket with tcp
int
Connection::do_connect(struct sockaddr_in* server_addr)
{
	int descriptor = socket(AF_INET, SOCK_STREAM, 0);

	if (descriptor == -1)
	{
		logger->write("Fail in socket function!");
		errx(ERROR_SOCKET, "Fail in socket function!");
	}

	bzero((char *) server_addr, sizeof(*server_addr));

	server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(server_port);
    server_addr->sin_addr.s_addr = inet_addr(server_ip.c_str());

    return descriptor;
}

//Hability connection to clients
void
Connection::server_connection()
{
	struct sockaddr_in server_addr;
	socket_descriptor = do_connect(&server_addr);

	if (bind(socket_descriptor, (struct sockaddr *) &server_addr, sizeof(struct sockaddr)) < 0)
	{
		logger->write("Fail in bind function!");
		errx(ERROR_BIND, "Fail in bind function!");
	}

	if (listen(socket_descriptor, QUEUE_LISTEN) < 0)
	{
		logger->write("Fail in listen function!");
		errx(ERROR_LISTEN, "Fail in listen function!");
	}

	logger->write("Startup Server Complet!");
}

//Accept connections to clients
void
Connection::accept_connections()
{
	struct sockaddr_in client;
	socklen_t client_len;
	
	while(true)
	{
		int client_id = accept(socket_descriptor, (struct sockaddr *) &client, &client_len);

		if (client_id < 0)
		{
			logger->write("Fail in accept function!");
			continue;
		}
		else
		{
			client_ip = inet_ntoa(client.sin_addr);
			logger->write("Client connection with ip: " + client_ip);

			receive_messages(client_id);
		}

		close(client_id);
	}
}

//Receive messages to temperature and air conditioning control
void
Connection::receive_messages(int client_id)
{
	string message = receive(client_id);

	if (message == "get_temperature")
	{
		logger->write("Request temperature from client " + client_ip);
		temperature = get_uart_temperature();

		if (send(client_id, &temperature, sizeof(temperature), 0) < 0)
			logger->write("Fail in send temperature!");
	}
	else if (message == "air_control")
	{
		logger->write("Request air_control from client " + client_ip);

		if (control_uart_air(not status_air))
			status_air = not status_air;

		string status;
		if (status_air)
		{
			logger->write("Turning on air conditioning");
			status = "turn_on";
		}
		else
		{
			logger->write("Turning off air conditioning");
			status = "turn_off";
		}

		string response = status;
		int size = response.size() + 1;
		send_message(client_id, size, response);
	}
	else if (message == "get_humidity")
	{
		logger->write("Request uart from client " + client_ip);
		float humidity = get_uart_relative_humidity();

		if (send(client_id, &humidity, sizeof(humidity), 0) < 0)
			logger->write("Fail in send humidity!");
	}
}

//Send client message
void
Connection::send_message(int id, int size, string message)
{
	if (send(id, &size, sizeof(size), 0) < 0)
	{
		logger->write("Fail in send message size ");
		return;
	}

	if (send(id, message.c_str(), size, 0) < 0)
		logger->write("Fail in send message " + message);
}

//Receive client message and return this message
string
Connection::receive(int id)
{
	int size;
	if (recv(id, &size, sizeof(size), 0) <= 0)
		logger->write("Fail in receive message size from client " + client_ip);

	char* message = (char*) malloc(size);

	if (recv(id, message, size, 0) <= 0)
		logger->write("Fail in receive message from client " + client_ip);

	string result = message;
	free(message);

	return result;
}

//Get temperature from uart
float
Connection::get_uart_temperature()
{
	int uart_descriptor = open_uart();

	if (uart_descriptor == -1)
		return 0;

	unsigned char buffer[1];
	buffer[0] = 0x05;

	if (write_uart(uart_descriptor, buffer, 1) == -1)
	{
		logger->write("Fail in write 0x05 in uart");
		return 0;
	}

	char temp[4];
	if (read_uart(uart_descriptor, (void*) temp, 4) == -1)
	{
		logger->write("Fail in read temperature from uart");
		return 0;
	}

	float temperature;
	memcpy(&temperature, &temp, 4);
	close(uart_descriptor);

	return temperature;
}

float
Connection::get_uart_relative_humidity()
{
	int uart_descriptor = open_uart();

	if (uart_descriptor == -1)
		return 0;

	unsigned char buffer[5];
	buffer[0] = 0x06;
	buffer[1] = 3;
	buffer[2] = 2;
	buffer[3] = 9;
	buffer[4] = 8;

	if (write_uart(uart_descriptor, buffer, 5) == -1)
		return 0;

	float humidity;
	if (read_uart(uart_descriptor, (void*) &humidity, 4) == -1)
		return 0;

	close(uart_descriptor);

	return humidity;
}

//Send turn_on or turn_off air conditioning to uart
bool
Connection::control_uart_air(bool action)
{
	int uart_descriptor = open_uart();

	if (uart_descriptor == -1)
		return false;

	unsigned char buffer[1];
	buffer[0] = 0xA0;

	if (write_uart(uart_descriptor, buffer, 1) == -1)
	{
		logger->write("Fail in write 0xA0 in uart");
		return false;
	}

	if (action)
		buffer[0] = 0x01;
	else
		buffer[0] = 0x00;

	if (write_uart(uart_descriptor, buffer, 1) == -1)
	{
		logger->write("Fail in write turn_on or turn_off in uart");
		return false;
	}

	unsigned char result;
	if (read_uart(uart_descriptor, (void*) &result, sizeof(result)) == -1)
	{
		logger->write("Fail in read result air conditioning uart");
		return false;
	}

	if (result != 0xA1)
	{
		logger->write("Fail in change state air conditioning");
		return false;
	}

	close(uart_descriptor);

	logger->write("Success in change state air conditioning");
	return true;
}

//Open uart to comunication
int
Connection::open_uart()
{
	int uart_descriptor = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);

	if (uart_descriptor < 0)
	{
		logger->write("Fail in open ttyAMA0");
		return -1;
	}

	fcntl(uart_descriptor, F_SETFL, 0);

	return uart_descriptor;
}

//Write a buffer in uart
int
Connection::write_uart(int uart_descriptor, unsigned char buffer[], int size)
{
	if (write(uart_descriptor, buffer, size) < 0)
	{
		close(uart_descriptor);
		return -1;
	}

	return 0;
}

//Read a byte from uart
int
Connection::read_uart(int uart_descriptor, void* buffer, int size)
{
	int size_readed = read(uart_descriptor, buffer, size);

	if (size_readed <= 0)
	{
		close(uart_descriptor);
		return -1;
	}

	return 0;
}
