// #This code was written by Robin Diddams, Nils Steinbugl, Chiara Piazzolla, and Rachel Lewis
// #nobody cheated
// #CSI-230-03
// #this is tested and works on archlinux(native)
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <grp.h>
#include <time.h>
#include <pwd.h>
#include <stdlib.h>

void longFormat (char* name, const char* path) {
	struct stat fileInfo;
	char appendedPath[512];
	sprintf(appendedPath, "%s/%s", path, name);
	stat(appendedPath, &fileInfo);

	//thanks stack exchange
	printf( (S_ISDIR(fileInfo.st_mode)) ? "d" : "-");
	printf( (fileInfo.st_mode & S_IRUSR) ? "r" : "-");
	printf( (fileInfo.st_mode & S_IWUSR) ? "w" : "-");
	printf( (fileInfo.st_mode & S_IXUSR) ? "x" : "-");
	printf( (fileInfo.st_mode & S_IRGRP) ? "r" : "-");
	printf( (fileInfo.st_mode & S_IWGRP) ? "w" : "-");
	printf( (fileInfo.st_mode & S_IXGRP) ? "x" : "-");
	printf( (fileInfo.st_mode & S_IROTH) ? "r" : "-");
	printf( (fileInfo.st_mode & S_IWOTH) ? "w" : "-");
	printf( (fileInfo.st_mode & S_IXOTH) ? "x" : "-");

	printf(" %d", fileInfo.st_nlink);


	struct passwd *pwd;
	if ((pwd = getpwuid(fileInfo.st_uid)) != NULL);
		printf(" %s", pwd->pw_name);

	struct group *grp;
	if ((grp = getgrgid(fileInfo.st_gid)) != NULL)
		printf(" %s", grp->gr_name);
	// printf(" %d", group);

	printf(" %d", fileInfo.st_size);

	struct tm ts;
	ts = *localtime(&fileInfo.st_mtime);
	char theTime[80];
	strftime(theTime, sizeof(theTime), "%b %d %H:%M", &ts);
	printf(" %s", theTime);
	printf(" %s\n", name);
}

void lsfunc(int ls, int lsa, int lsl, int lsla, const char* path) {
	// printf("%d %d %d %d %s\n",ls, lsa, lsl, lsla, path);
	struct dirent * dp;

	DIR* dirp = opendir(path);
	if (dirp == NULL) {
		printf("ls: cannot access '%s': No such file or directory\n", path);
		exit(1);
	}
	// dp = readdir(dirp);

	if (ls == 1) { //ls
		while ((dp = readdir(dirp)) != NULL) {
			if (dp->d_name[0] != '.') {
				printf("%s\n", dp->d_name);
			}
		}
	}
	else if (lsa == 1) { //ls -a
		while ((dp = readdir(dirp)) != NULL) {
			printf("%s\n", dp->d_name);
		}
	}
	else if(lsla == 1) {
		while ((dp = readdir(dirp)) != NULL) {
			longFormat(dp->d_name, path);
		}
	}
	else if(lsl == 1) {
		while ((dp = readdir(dirp)) != NULL) {
			if (dp->d_name[0] != '.') {
				longFormat(dp->d_name, path);
			}
		}
	}


}

int main(int argc, char* argv[]) {
	// const char* path = "./";
	char buffer[256];

	int ls = 0;
	int lsa = 0;
	int lsl = 0;
	int lsla = 0;

	int l = 0;
	int a = 0;
	int p = 0;


	if (argc > 1) {
		// printf("argc: %d\n", argc);
		int i = 1;
		for (i; i < argc && argv[i][0] == '-'; i++) {
			// printf("%d, %s\n", i, argv[i]);
			for(int q = 0; argv[i][q] != 0; q++) {
				if(argv[i][q] == 'a') {
					a = 1;
				} else if(argv[i][q] == 'l') {
					l = 1;
				}
			}
		}
		// printf("%d %d\n", argc, i);
		if (i == argc - 1) {
			if (argv[i][0] != '-') {
				sprintf(buffer, "%s", argv[i]);
				p = 1;
				// printf("%s\n", argv[i]);
			}
		}
	}

	char* path;
	if (p == 1) {
		path = buffer;
		// printf("path given: %s\n", path);
	} else {
		// printf("no path given\n");
		path = "./";
	}
	// printf("%s\n", buffer);

	// printf("%d %d %s\n", l, a,  path);

	if (l && a) {
		lsla = 1;
		// printf("no \n");
	} else if (l) {
		lsl = 1;
	} else if (a) {
		lsa = 1;
	} else if (!l && !a) {
		ls = 1;
	}

	//lets check if its a file
	struct stat file;
	stat(path, &file);
	if (S_ISREG(file.st_mode)) {
		printf("%s\n", path);
		exit(0);
	}

	lsfunc(ls, lsa, lsl, lsla, path);

	return 0;
}
