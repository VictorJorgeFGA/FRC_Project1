#ifndef _APPLICATION_H_
#define _APPLICATION_H_

/*
Recebe o caminho do arquivo, quebra este arquivo em múltiplas chunks
e os envia para a camada N-1
*/
void process_file(char *file_path);

/*
Recebe o nome do arquivo e monta os pacotes, presentes na fila, de tamanho
chunk_size em um arquivo com esse nome
*/
void mount_file(char *filename);

#endif