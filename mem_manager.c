#include <stdlib.h>

int buffer_size;
char *buffer;

void init_manager(void){
	buffer_size=4096; //4KB
	buffer=(char*)malloc(buffer_size*sizeof(char));
}

char* get_buffer_format(void){
	//This would be the way to go, but the size is fixed
	//	so there is no need to add overheads yet nor
	//	waste time on it.
	/*int n=0, aux=buffer_size;
	while (aux>0){
		aux/=10;
		n++;
	}
	*/
	
	return "%4096s";
}

char* get_buffer(void){
	return buffer;
}

void deallocate_manager(void){
	free(buffer);
}
