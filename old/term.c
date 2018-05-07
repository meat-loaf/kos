void term_dump_error(char *msg){	
//	char * msg2 = "x86_64 is not supported on your machine...\0";
	char *term = 0xB8000;
	int i = 0;
	while (msg[i]){
		term[i*2] = msg[i];
		i++;
	}
	return;
}
