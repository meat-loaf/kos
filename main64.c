void
main64(){
	asm volatile("mov %rax, (0xDEADBEEF << 32)");
	while(1);
	return;
}
