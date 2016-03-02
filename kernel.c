


void readString(char*);
void printString(char*);

int main() 
{

	char c[80];
	int i = 0 ;
	
	// TASK 1 : 
	//STEP 1 :

	// printString("Hello World\0");
	// //STEP 2:

	// while(1)
	// {

	// 		char cc = interrupt(0x16,0,0,0,0);
	// 		if(cc == 0xd)	
	// 			break;
	// 			interrupt(0x10, 0xE*256+cc, 0, 0, 0);
	// }
	//TASK 2 : 
	readString(c);
	printString(c);
	while(1); // sagheer mesh fahem 7aga 

	// return 1;
}


/**




*/
void readString(char* line){
	int i = 0;
	while(1)
	{

			char cc = interrupt(0x16,0,0,0,0);

			if(cc ==  0xd)
			{

				line[i++] = 0xa;
				line[i++] = '\0';
				break;
			}
			else if(cc == 0x8)
				i--;
			interrupt(0x10, 0xE*256+cc, 0, 0, 0);
			line[i++] = cc;
	}
	interrupt(0x10, 0xE*256+0xd, 0, 0, 0);
}
// TASK 1 :
void printString(char* chars){

	int i = 0;
	while(chars[i] != '\0')
		interrupt(0x10, 0xE*256+chars[i++], 0, 0, 0);
}




