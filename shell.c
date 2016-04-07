int equal(char*, char*);
void consoleLog(char*);

int main()
{
	char buffer[150];
	char command[50];
	char fileName1[50];
	char fileName2[50];
	char result[13112];
	int i, j;
	
	
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
			fileName1[j++] = buffer[i++];
		fileName1[j] = '\0';

		j = 0;
		if(buffer[i] == ' ')
			++i;
		while(buffer[i] != ' ' && buffer[i] != 0xd)
			fileName2[j++] = buffer[i++];
		fileName2[j] = '\0';
		
		// consoleLog(command);
		if(equal("view\0", command))
		{
			if(fileName1[0] == '\0')
			{
				consoleLog("No File entered.\0");
				continue;
			}
			interrupt(0x21, 3, fileName1, result, 0);
			if(equal("Error!\0", result))
				consoleLog("File Not Found.\0");
			else
				consoleLog(result);
		}
		else if(equal("view\0", command))



	}
}

int equal(char* x, char* y)
{
	int i;
	for(i = 0; x[i] != '\0' && x[i] == y[i]; ++i);
	return x[i] == y[i];
}

void consoleLog(char* feedback)
{
	int i = 0;
	while(feedback[i] != '\0')
		++i;
	feedback[i++] = 0xd;
	feedback[i++] = 0xa;
	feedback[i] = '\0';
	interrupt(0x21, 0, feedback, 0, 0);
	feedback[i-2] = '\0';
}