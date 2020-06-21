int checkSearch(char string[], char search[]){
	int len = strlen(search);
	
	int equal = 1;

	for(int i = 0; i< len && equal; i++)
		equal *= toupper(string[i]) == toupper(search[i]);
	return equal;
}