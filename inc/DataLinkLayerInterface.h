#ifndef _DATA_LINK_INTERFACE_H_
#define _DATA_LINK_INTERFACE_H_

#define CQ_HEADER_LEN 4
#define CQ_MESSAGE_LEN 100
#define CQ_DATA_MAX_LEN (CQ_HEADER_LEN + CQ_MESSAGE_LEN)
#define CQ_MAX_DATA_AMOUNT 10

// Inicializa a fila de comunicação interprocessos
void initialize_dll_interface();

// Finaliza a fila de comunicacao interprocessos
void shut_down_dll_interface();

// Envia dados para a camada de enlace de dados (N-1)
// A função bloqueará a execução até que os dados sejam enviados
//
// data - array que contem os dados a serem enviados
// data_len - o tamanho em bytes do array a ser enviado (deve ser menor que CQ_DATA_MAX_LEN)
void send_data_to_dll(char * data, int data_len);

// Recebe dados da camada de enlace de dados (N-1)
// data - o array que será preenchido com os dados
// data_len - a quantidade de bytes que o array recebeu
// data_len é sempre menor ou igual que CQ_DATA_MAX_LEN
void get_data_from_dll(char * data, int * data_len);

// Envia dados para a camada de aplicacao (N)
// A função bloqueará a execução até que os dados sejam enviados
//
// data - array que contem os dados a serem enviados
// data_len - o tamanho em bytes do array a ser enviado (deve ser menor que CQ_DATA_MAX_LEN)
// data_len é sempre menor ou igual que CQ_DATA_MAX_LEN
void send_data_to_app(char * data, int data_len);

// Recebe dados da camada da aplicao (N)
// A função bloqueará a execução até que os dados sejam recebidos
//
// data - o array que será preenchido com os dados
// data_len - a quantidade de bytes que o array recebeu
// data_len é sempre menor ou igual que CQ_DATA_MAX_LEN
void get_data_from_app(char * data, int * data_len);

#endif