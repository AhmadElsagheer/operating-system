int equal(char*, char*);

int main()
{
	char buffer[150];
	char command[50];
	
	char fileName2[50];
	char result[13000];
	char fileName1[25];
	char fileName1Tmp[25];
	char terminateString[3];
	int i, j;
	terminateString[0] = 0xd;
	terminateString[1] = 0xa;
	terminateString[2] = '\0';
	
	// interrupt(0x21,0, "x", 0, 0);	
	// interrupt(0x21,0, "y", 0, 0);	

	// interrupt(0x21,0, "z", 0, 0);


	while(1)
	{

		interrupt(0x21, 0, "Shell:>\0", 0, 0);
		interrupt(0x21, 1, buffer, 0, 0);
		
		
		i = 0; j = 0;
		while(buffer[i] != ' ' && buffer[i] != 0xd)
			command[j++] = buffer[i++];
		command[j] = '\0';

		j = 0;
		if(buffer[i] == ' ')
			++i;
		while(buffer[i] != ' ' && buffer[i] != 0xd)
		{
			fileName1[j] = buffer[i];
			fileName1Tmp[j++] = buffer[i++];

		}
		fileName1[j] = '\0';
		fileName1Tmp[j] = '\0';

		j = 0;
		if(buffer[i] == ' ')
			++i;
		while(buffer[i] != ' ' && buffer[i] != 0xd)
			fileName2[j++] = buffer[i++];
		fileName2[j] = '\0';
		
		
		if(equal("view\0", command))
		{
			if(fileName1[0] == '\0')
			{
				interrupt(0x21, 0, "No file entered", 0, 0);
				interrupt(0x21, 0, terminateString, 0, 0);
				continue;
			}
			interrupt(0x21, 3, fileName1, result, 0);
			if(equal("Error!\0", result))
			{
				interrupt(0x21, 0, "File not found", 0, 0);
				interrupt(0x21, 0, terminateString, 0, 0);
			}
			else
			{
				interrupt(0x21, 0, result, 0, 0);
				interrupt(0x21, 0, terminateString, 0, 0);
			}
		}
		else if(equal("execute\0", command))
		{
			
			if(fileName1[0] == '\0')
			{
				interrupt(0x21, 0, "No file entered", 0, 0);
				interrupt(0x21, 0, terminateString, 0, 0);
				continue;
			}
			else{
				interrupt(0x21, 3, fileName1, result, 0);
				if(equal("Error!\0", result))
				{
					interrupt(0x21, 0, "File not found", 0, 0);
					interrupt(0x21, 0, terminateString, 0, 0);
				}
				else
					interrupt(0x21, 4, fileName1, 0x2000, 0);
			}
		}
		else if(equal("delete\0", command))
		{

			if(fileName1[0] == '\0')
			{
				interrupt(0x21, 0, "No file entered", 0, 0);
				interrupt(0x21, 0, terminateString, 0, 0);
				continue;
			}
			interrupt(0x21, 3, fileName1, result, 0);
			if(equal("Error!\0", result))
			{
				interrupt(0x21, 0, "File not found", 0, 0);
				interrupt(0x21, 0, terminateString, 0, 0);
			}
			else{
				interrupt(0x21, 7, "message\0", 0, 0);	
			}
		}




	}
}

int equal(char* x, char* y)
{
	int i;
	for(i = 0; x[i] != '\0' && x[i] == y[i]; ++i);
		return x[i] == y[i];
}

