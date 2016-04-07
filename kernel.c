void readFile(char*, char*);
void readString(char*);
void printString(char*);
void readSector(char*, int);
void handleInterrupt21(int, int, int, int);
int mod(int, int);
int div(int, int);
void executeProgram(char*, int);
void terminateProgram();
void writeSector(char*, int);
void deleteFile(char*);
void writeFile(char*, char*, int);
void storeError( char*);

int main() 
{
	char buffer[13112];
	// int k;
	// char clean[512];
	// int i;
	// for(i = 0; i < 512; ++i)
	// 	clean[i] = 0x00;
	// for(k = 1; k < 256; ++k)
	// 	writeSector(clean, k);

	makeInterrupt21();
	interrupt(0x21, 4, "shell\0", 0x2000, 0);
	// char c[80];
	
	
	// TASK 1 : 
	// printString("Hello World\0");

	// while(1)
	// {

	// 		char cc = interrupt(0x16,0,0,0,0);
	// 		if(cc == 0xd)	
	// 			break;w
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


	//TASK 4 :
	// makeInterrupt21();
	// interrupt(0x21,0,0,0,0);

	//TASK 5
	// char line[80];
	// makeInterrupt21();
	// interrupt(0x21,1,line,0,0);
	// interrupt(0x21,0,line,0,0);
	// interrupt(0x21,3,line,0,0);

	// M3 TASK 1
	// makeInterrupt21();
	// char buffer[13312]; 
	
	
	// interrupt(0x21, 3, "messag\0", buffer, 0); /*read the file into buffer*/
	// interrupt(0x21, 0, buffer, 0, 0); /*print out the file*/

	// M3 TASK 2
	// interrupt(0x21, 4, "tstprg\0", 0x2000, 0);

	// M3 TASK 3
	// interrupt(0x21, 4, "tstpr2\0", 0x2000, 0);	
	
	// M3 TASK 4 & 5
	// char buffer[13312];
	// int i;
	// makeInterrupt21();
	interrupt(0x21, 3, "messag\0", buffer, 0); // try to read messag
	interrupt(0x21, 0, buffer, 0, 0); //print out the contents of buffer
	// interrupt(0x21, 7, "messag\0", 0, 0); //delete messag
	// for(i = 0; i < 13312; ++i)
	// 	buffer[i] = 0x00;
	// interrupt(0x21, 3, "messag\0", buffer, 0); // try to read messag
	// interrupt(0x21, 0, buffer, 0, 0); //print out the contents of buffer
	
	// M3 TASK 6
	// int i=0;
	// char buffer1[13312];
	// char buffer2[13312];
	// buffer2[0]='h'; 
	// buffer2[1]='e'; 
	// buffer2[2]='l';
	// buffer2[3]='l';
	// buffer2[4]='o';
	// for(i=5; i<13312; i++) buffer2[i]=0x0;
	// makeInterrupt21();
	// interrupt(0x21,8, "testW\0", buffer2, 1); //write file testW
	// interrupt(0x21,3, "testW\0", buffer1, 0); //read file testW
	// interrupt(0x21,0, buffer1, 0, 0); // print out contents of testW


	while(1); /*hang up*/

	
}


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
			else if(cc == 0x8 && i > 0)
			{
				interrupt(0x10, 0xE*256+cc, 0, 0, 0);
				interrupt(0x10, 0xE*256+'\0', 0, 0, 0);
				i--;
			}	
			else
				line[i++] = cc;
			interrupt(0x10, 0xE*256+cc, 0, 0, 0);
			
	}
	interrupt(0x10, 0xE*256+0xa, 0, 0, 0); // line feed
	interrupt(0x10, 0xE*256+0xd, 0, 0, 0); // carriage return
}

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
	this is integer division, so the result should be rounded down
	track = ( sinterrupt(0x21,0,"de\0",0,0);ector / 36 )
*/

void readSector(char* buffer, int sector)
{
	int rSector = mod(sector,18) + 1;
	int head = mod(div(sector,18),2);
	int track = div(sector,36);
	interrupt(0x13,2*256+1,buffer,track*256+rSector,head*256);
}


void readFile(char* fileName , char* buffer)
{
	char directory[512];
	int i, j, match;
	readSector(directory, 2);


	for ( i = 0; i < 16; i++)
	{
		match = 1;
		for(j = 0; j < 6; j++)
			if(directory[i*32 + j] != fileName[j])
			{
				match = 0;
				break;
			}

		if(!match)
			continue;
	
		for(j = 6; j < 32; j++)
			readSector(buffer + (j - 6)*512, directory[i*32 + j]);
		return;
	}
	storeError(buffer);

}

void executeProgram(char* name, int segment)
{
	char buffer[13312];
	int i;
	i = 0;
	readFile(name, buffer);
	while(i < 13312)
	{
		putInMemory(segment, i, buffer[i]);
		i++;
	}
	launchProgram(segment);
}


void terminateProgram()
{
	interrupt(0x21, 4, "shelll\0", 0x2000, 0);	
}

void writeSector(char* buffer, int sector)
{
	int rSector = mod(sector,18) + 1;
	int head = mod(div(sector,18),2);
	int track = div(sector,36);
	interrupt(0x13,3*256+1,buffer,track*256+rSector,head*256);
}

void deleteFile(char* name)
{
	char map[512];
	char directory[512];
	int i, j, match;
	readSector(map, 1);
	readSector(directory, 2);

	for ( i = 0; i < 16; i++)
	{
		match = 1;
		for(j = 0; j < 6; j++)
			if(directory[i*32 + j] != name[j])
			{
				match = 0;
				break;
			}
		if(!match)
			continue;
		directory[i*32] = 0x00;
		for(j = 6; j < 32; j++)
		{
			map[directory[i*32+j]] = 0x00;
		}
		
		break;
	}
	writeSector(map, 1);
	writeSector(directory, 2);
}

void writeFile(char* name, char* buffer, int secNum)
{
	char map[512];
	char directory[512];
	int i, j, k;
	readSector(map, 1);
	readSector(directory, 2);
	for ( i = 0; i < 16; i++)
	{
		if(directory[i*32] == 0x00)
		{
			for (j= 0; j < 6 && name[j] != '\0'; ++j)
			{
				directory[i*32 + j] = name[j];
			}
			for (; j < 6; ++j)
			{
				directory[i*32 + j] = 0x00;
			}
			k = 0;
			for (;j < secNum + 6; ++j)
			{
				while(k < 512 && map[k] != 0x00)
					++k;
				if(k == 512)
				{
					printString("Error !\0");
					return;
				}
				map[k] = 0xFF;
				writeSector(buffer + (j - 6)*512, directory[i*32 + j] = k);			
			}
			for(; j < 32; ++j)
				directory[i*32+j] = 0x00;
			writeSector(map, 1);
			writeSector(directory, 2);
			return;
		}
	}
}



void handleInterrupt21(int ax, int bx, int cx, int dx)
{	

	if(ax == 0)
		printString(bx);
	else if(ax == 1)
		readString(bx);
	else if (ax == 2)
		readSector(bx, cx);
	else if(ax == 3)
		readFile(bx,cx);
	else if(ax == 4)
		executeProgram(bx, cx);
	else if(ax == 5)
		terminateProgram();
	else if(ax == 6)
		writeSector(bx, cx);
	else if(ax == 7)
		deleteFile(bx);
	else if(ax == 8)
		writeFile(bx,cx,dx);
	else
		storeError(cx);
}


void storeError(char* out)
{
	out[0] = 'E';
	out[1] = 'r';
	out[2] = 'r';
	out[3] = 'o';
	out[4] = 'r';
	out[5] = '!';
	out[6] = '\0';
}



