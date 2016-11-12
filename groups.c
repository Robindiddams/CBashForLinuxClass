// #This code was written by Robin Diddams, Nils Steinbugl, Chiara Piazzolla, and Rachel Lewis
// #nobody cheated
// #CSI-230-03
// #this is tested and works on linux mint(vm), archlinux(native), and centos(hawk)

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>

char* getGroupsLines(char * name) {
	int fd;
	fd = open("/etc/group", O_RDONLY);

	if (fd < 0) {
		perror("open fd: ");
		exit(-1);
	}

	char buffer[1500];
	int rd = read(fd, buffer, 1500);

	//get name length
	int length = 0, backwardsi = 0;
	while (name[length] != 0) {
		length++;
	}

	const int NAMELENGTH = length;

	// make reverse name
	char unameBackwards[NAMELENGTH];
	length--;
	while (length >= 0) {
		unameBackwards[backwardsi] = name[length];
		length--;
		backwardsi++;
	}
	unameBackwards[backwardsi] = 0;

	// setup for big loop
	int i = rd, colonCounter = 0, newline = 1, namefound = 0, groupnamesIndex = 0;
	char groupnames[100];

	//ignore whitespace at the end
	while(buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == 0) {
		i--;
	}

	//read rd backwards
	while (i > 0) {
			if (buffer[i] == ':') {
			//if group is empty we skip to next line
			while(buffer[i] != '\n') {
				i--;
			}
		} else {
			//look for usernames
			while (buffer[i] != ':') {
				int k = 0;
				while (buffer[i] == unameBackwards[k]) {
					//compare buffer to name 1 char at a time
					i--;
					k++;
				}
				if (k == NAMELENGTH) {
					//if it gets all chars right
					namefound = 1;
				}
				//loop until colon
				i--;
			}

			//once we get a colon we got to skip to the beginging of the line
			while(buffer[i] != '\n' && i >= 0) {
				i--;
			}

			//above loop allows for i to reach zero and then be decremented,
			//making it -1, this is ok only for the very first element in buffer, (buffer[0])
			//so we make sure if it gets below -1 we error out
			if (i < -1) {
				perror("i is less than zero\n");
				exit(1);
			}

			//we are reading backwards and we hit a \n
			//so we're at the end of the next line and if we
			//found the name in the last line that means
			//we just gotta read forward until we hit a :
			//and then we will have the group name
			if (namefound) {
				int placey = i + 1;
				while (buffer[placey] != ':') {
					// concat the group into the groupnames string
					groupnames[groupnamesIndex] = buffer[placey];
					placey++;
					groupnamesIndex++;
				}
				groupnames[groupnamesIndex] = ' ';
				groupnamesIndex++;
				namefound = 0;
			}
		}

		i--;
	}


	groupnames[groupnamesIndex] = 0;
	char* groups = groupnames;
	return groups;
}

int checkForName(char* name) {
	int fd;
	fd = open("/etc/group", O_RDONLY);

	char buffer[2000];
	int rd = read(fd, buffer, 2000);
	int i = 0, j = 0, length;
	while (name[i] != 0) {
		i++;
	}
	length = i;

	i = 0;
	while (i < rd) {
		int k = 0;
		while (buffer[i] == name[k]) {
			//compare buffer to name 1 char at a time
			i++;
			k++;
		}
		if (k == length) {
			//printf("found name at position: %d\n", i);
			//if it gets all chars right
			return 1;
		}
		i++;
	}
	return 0;
}


int main(int argc, char *argv[]) {

	if(argc == 1)
	{
		char* groups = getGroupsLines(getenv("USER"));
		printf("%s\n", groups);
	}
	else
	{
		int i = 0;
		for (i; i < argc-1; i++)
		{
			if(checkForName(argv[i+1]))
			{
				printf("%s: ", argv[i+1]);
				char* groups = getGroupsLines(argv[i+1]);
				if (groups[0] == 0) {
					printf("ERROR: not in a group\n");
				} else {
					printf("%s\n", groups);
				}
			}
			else
			{
				printf("ERROR: Did not find name\n");
				exit(1);
			}
		}
	}
	return 0;
}
