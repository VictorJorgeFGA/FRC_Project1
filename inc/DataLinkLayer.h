#ifndef _DATA_LINK_LAYER_H_
#define _DATA_LINK_LAYER_H_

#define SENDER 0
#define RECEIVER 1

// INTERFACE DA CAMADA

// Inicializa a camada de enlace de dados
//
// operation_mode - SENDER se a camada deve se comportar como remetente, RECEIVER se a camada deve se comportar como destinatário
// host_address - Endereço da máquina que está executando este código
// host_port - Porta aberta para comunicação da camada que está executando este código
// receiver_address - Endereço do par para enviar dados
// receiver_port - Porta aberta pelo par para comunicação
void initialize_dll(int operation_mode, char * host_address, char * host_port, char * receiver_address, char * receiver_port);

// Roda o loop infinito da camada
void run_dll();


// CORE DA CAMADA

// FUNCOES PARA INTERFACE COM CAMADA N ---------
// Interrompe a execução do programa até receber algum dado da fila e preenche o buffer com dados
static void get_data_from_queue();

// Interrompe a execução da camda até que seja possível empurrar os
// dados recebidos para a camada acima
static void send_data_to_queue();
// ---------------------------------------------

// FUNCOES PARA TRATAMENTO DE ERROS DE COMUNICACAO

// Checa se o frame recebido possui erros
// Retorna 1 caso existam erros, 0 caso contrario
static int check_incoming_frame();

// Envia um frame confirmando que houve um erro para o par
// Retorna 1 se o frame não pôde ser enviado,
// retorna 0 caso contrário
static int send_error_confirmation_frame();

// Envia um frame para o par confirmando que não houveram erros
// Retorna 1 se o frame não pôde ser enviado
// Retorna 0 caso contrário
static int send_ok_confirmation_frame();
// --------------------------------------------


// FUNCOES PARA NEGOCIACAO -------------------

// -------------------------------------------

// FUNCOES PARA FLUXO DE DADOS ------
// Envia o frame salvo no buffer para o destinatário.
// Bloqueia a execução da camada até que o frame seja enviado
// com sucesso, i.e., o frame seja enviado e a confirmação seja
// recebida.
static void delivery_frame();

// Envia o frame salvo no buffer para o destinatario
// Retorna 1 se não foi possível enviar o quadro completo
// Retorna 0 em caso de sucesso
static int send_frame();

// Interrompe a execução do programa até receber a confirmação de recebimento do pacote pelo receptor
// Retorna 0 se o receptor confirmou que recebeu o pacote sem erros
// Retorna um inteiro diferente de 0 caso o receptor sinalize erros no pacote, ou não envie um
// pacote de confirmação dentro da janela de tempo configurada (timeout)
static int get_confirmation_frame();

// Recebe um frame do destinatário e salva no buffer
// Retorna 1 se não foi possível enviar o quadro completo
// Retorna 0 em caso de sucesso
static int receive_frame();

// Interrompe a execução da camada até receber um frame valido do par
// Quadros de confirmação são enviados
static void get_data_from_sender();
// ------------------------------------------

// FUNCOES PARA REALIZAR ENQUADRAMENTO ------
// Realiza enquadramento da mensagem contida no buffer da fila interprocesso
static void pack_message_from_queue_buffer();

// Desfaz o enquadramento do frame recebido via socket e insere no buffer da fila interprocesso
static void unpack_message_from_frame_buffer();
// ------------------------------------------


#endif