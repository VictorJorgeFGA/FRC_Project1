#ifndef _APPLICATION_H_
#define _APPLICATION_H_

// Recebe o caminho do arquivo, abre e envia o arquivo para camada n-1 via fila
void process_message(char *file_path, int PDU_size);

#endif