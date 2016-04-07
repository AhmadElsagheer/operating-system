int equal(char*, char*);
int div(int, int);

int main()
{
	char buffer[150];
	char command[50];
	
	char fileName2[50];
	char result[13000];
	char fileName1[25];
	char fileName1Tmp[25];
	char terminateString[3];
	int i, j, c;
	terminateString[0] = 0xd;
	terminateString[1] = 0xa;
	terminateString[2] = '\0';
	


	while(1)
	{

		interrupt(0x21, 0, "Shell:>\0", 0, 0);
		interrupt(0x21, 1, buffer, 0, 0);
		
		
		i = 0; j = 0;
		//read the command from the buffer
		while(buffer[i] != ' ' && buffer[i] != 0xd)
			command[j++] = buffer[i++];
		command[j] = '\0';

		j = 0;
		//make the pointer point to first character of fileName1 in the buffer
		if(buffer[i] == ' ')
			++i;
		//read fileName1 from the buffer
		while(buffer[i] != ' ' && buffer[i] != 0xd)
		{
			fileName1[j] = buffer[i];
			fileName1Tmp[j++] = buffer[i++];

		}
		fileName1[j] = '\0';
		fileName1Tmp[j] = '\0';

		j = 0;
		//make the pointer point to first character of fileName2 in the buffer
		if(buffer[i] == ' ')
			++i;
		//read fileName2 from the buffer
		while(buffer[i] != ' ' && buffer[i] != 0xd)
			fileName2[j++] = buffer[i++];
		fileName2[j] = '\0';
		
		

		//process view command
		if(equal("view\0", command))
		{
			//check if the file name is entered
			if(fileName1[0] == '\0')
			{
				interrupt(0x21, 0, "No file entered", 0, 0);
				interrupt(0x21, 0, terminateString, 0, 0);
				continue;
			}
			
			interrupt(0x21, 3, fileName1, result, 0);
			//check if the file exists or not
			if(equal("Error!\0", result))
			{
				interrupt(0x21, 0, "File not found", 0, 0);
				interrupt(0x21, 0, terminateString, 0, 0);
			}
			else
			{
				//print the file followed by a new line and carriage return
				interrupt(0x21, 0, result, 0, 0);
				interrupt(0x21, 0, terminateString, 0, 0);
			}
		}
		//process the execute command
		else if(equal("execute\0", command))
		{
			//check if the file name is entered
			if(fileName1[0] == '\0')
			{
				interrupt(0x21, 0, "No file entered", 0, 0);
				interrupt(0x21, 0, terminateString, 0, 0);
				continue;
			}
			//read the file to check if file exists or not
			interrupt(0x21, 3, fileName1, result, 0);
			if(equal("Error!\0", result))
			{
				interrupt(0x21, 0, "File not found", 0, 0);
				interrupt(0x21, 0, terminateString, 0, 0);
			}
			else
				interrupt(0x21, 4, fileName1, 0x2000, 0);
			
		}
		//process the delete command
		else if(equal("delete\0", command))
		{
			//check if the file name is entered
			if(fileName1[0] == '\0')
			{
				interrupt(0x21, 0, "No file entered", 0, 0);
				interrupt(0x21, 0, terminateString, 0, 0);
				continue;
			}
			//read the file to check if the file exists or not
			interrupt(0x21, 3, fileName1, result, 0);
			if(equal("Error!\0", result))
			{
				interrupt(0x21, 0, "File not found", 0, 0);
				interrupt(0x21, 0, terminateString, 0, 0);
			}
			else{
				interrupt(0x21, 7, fileName1, 0, 0);	
			}
		}
		//process the copy command
		else if(equal("copy\0", command))
		{
			//check if the file 1 name is entered
			if(fileName1[0] == '\0')
			{
				interrupt(0x21, 0, "File 1 not entered", 0, 0);
				interrupt(0x21, 0, terminateString, 0, 0);
				continue;
			}
			//check if the file 2 name is entered
			else if(fileName2[0] == '\0')
			{
				interrupt(0x21, 0, "File 2 not entered", 0, 0);
				interrupt(0x21, 0, terminateString, 0, 0);
				continue;
			}
			//read the file
			interrupt(0x21, 3, fileName1, result, 0);
			//check if the file exists
			if(equal("Error!\0", result))
			{
				interrupt(0x21, 0, "File not found", 0, 0);
				interrupt(0x21, 0, terminateString, 0, 0);
			}
			else{
				//count the number of sectors
				for(j = 0; result[j] != '\0'; j++);
				j = div(j+511,512);

				interrupt(0x21, 8, fileName2, result, j);	
			}
		}
		//process the dir command
		else if(equal("dir\0", command))
		{
			interrupt(0x21,9,result,0,0);
			interrupt(0x21, 0, result, 0, 0);
			interrupt(0x21, 0, terminateString, 0, 0);

		}
		//process the create command
		else if(equal("create\0", command))
		{
			//check if the file name is entered
			if(fileName1[0] == '\0')
			{
				interrupt(0x21, 0, "No name entered", 0, 0);
				interrupt(0x21, 0, terminateString, 0, 0);
				continue;
			}
			else
			{

				c=0;
				interrupt(0x21, 1, buffer, 0, 0);
				while(buffer[0] != 0xd )
				{
					
					for (j = 0; buffer[j] != '\0'; ++j)
					{
						result[c++] = buffer[j];
					}
					
					interrupt(0x21, 1, buffer, 0, 0);
				}
				
				result[c-2] = '\0';
				interrupt(0x21,8,fileName1,result,1);
			}

		}
		else
		{
			interrupt(0x21, 0, "Bad command", 0, 0);
			interrupt(0x21, 0, terminateString, 0, 0);
		}



	}
}

int equal(char* x, char* y)
{
	int i;
	for(i = 0; x[i] != '\0' && x[i] == y[i]; ++i);
		return x[i] == y[i];
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