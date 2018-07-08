int
strcmp(char *l, char *r){
	unsigned char *p1 = l, *p2 = r;
	while (*p1 == *p2){
		p1++;
		p2++;
	}
	return (*p1)-(*p2);
}
