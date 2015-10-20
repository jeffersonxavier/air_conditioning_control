#include <fcntl.h>
#include <stdio.h>

float get_umidade_relativa();

int main(int argc, char const *argv[])
{
	printf("Umidade lida %f\n", get_umidade_relativa());
	return 0;
}

int open_uart()
{
	int uart_descriptor = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);

	if (uart_descriptor < 0)
	{
		printf("Erro ao abrir uart\n");
		return -1;
	}

	fcntl(uart_descriptor, F_SETFL, 0);

	return uart_descriptor;
}

int write_uart(int uart_descriptor, unsigned char buffer[], int size)
{
	if (write(uart_descriptor, buffer, size) < 0)
	{
		printf("Erro ao escrever na uart\n");
		return -1;
	}

	return 0;
}

int read_uart(int uart_descriptor, void* buffer, int size)
{
	int size_readed = read(uart_descriptor, buffer, size);

	if (size_readed <= 0)
	{
		printf("Erro ao ler da uart %d bytes lidos\n", size_readed);
		return -1;
	}

	return 0;
}

float get_umidade_relativa()
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

	printf("Escrita completa!\n");

	float umidade;
	if (read_uart(uart_descriptor, (void*) &umidade, 4) == -1)
		return 0;

	printf("Leitura completa!\n");	

	close(uart_descriptor);

	return umidade;
}