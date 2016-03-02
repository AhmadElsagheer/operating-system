


void readString(char*);
void printString(char*);
void readSector(char*, int);
void handleInterrupt21(int, int, int, int);
int mod(int, int);
int div(int, int);

int main() 
{

	char c[80];
	
	
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
	// printString("Enter a line: \0");
	// readString(c);
	// printString(c);

	// TASK 3 :

	// char buffer[512];
	// readSector(buffer, 30);
	// printString(buffer);


	//Task 4 :
	
	makeInterrupt21();

	interrupt(0x21,0,0,0,0);
	while(1); // sagheer mesh fahem 7aga 

	// return 1;
}






//Task 2

void readString(char* line){
	int i = 0;
	while(1)
	{

			char cc = interrupt(0x16,0,0,0,0);

			if(cc ==  0xd)
			{
				line[i++] = cc;			
				line[i++] = 0xa;
				line[i++] = '\0';	
				break;
			}
			else if(cc == 0x8)
				i--;
			interrupt(0x10, 0xE*256+cc, 0, 0, 0);
			line[i++] = cc;
	}
	interrupt(0x10, 0xE*256+0xa, 0, 0, 0); // line feed
	interrupt(0x10, 0xE*256+0xd, 0, 0, 0); // cursor return
	// interrupt(0x10, 0xE*256+0xa, 0, 0, 0);
}


// TASK 1 :
void printString(char* chars){

	int i = 0;
	while(chars[i] != '\0')
		interrupt(0x10, 0xE*256+chars[i++], 0, 0, 0);
}

int div(int a, int b)
{
	int i = 0;
	while(a>=b)
	{
		a = a - b;
		i++;
	}
	return i;
}

int mod(int a, int b) {

	while(a>=b)
	{
		a = a - b;
	}
	return a;
}

/*

relative sector = ( sector MOD 18 ) + 1
head = ( sector / 18 ) MOD 2
/*this is integer division, so the result should be rounded down
track = ( sector / 36 )

*/

void readSector(char* buffer, int sector)
{
	int rSector = mod(sector,18) + 1;
	int head = mod(div(sector,18),2);
	int track = div(sector,36);
	interrupt(0x13,2*256+1,buffer,track*256+rSector,head*256);
}



void handleInterrupt21(int ax, int bx, int cx, int dx)
{
	printString("The Factory of Chairs + Kammola !\0");
}