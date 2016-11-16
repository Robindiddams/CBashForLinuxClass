#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <grp.h>
#include <time.h>
#include <pwd.h>


void longFormat (char* name, const char* path) {
	struct stat fileInfo;
	char appendedPath[512];
	sprintf(appendedPath, "%s/%s", path, name);
	stat(appendedPath, &fileInfo);

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

int main(int argc, char* argv[]) {
	const char* path = "./";
	struct dirent * dp;

	int ls = 0;
	int lsa = 0;
	int lsl = 1;


	DIR* dirp = opendir(path);
	if (dirp == NULL) {
		printf("error finding path\n");
		return 1;
	}
	printf("hi@!\n");
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
	else if(lsl == 1) {
		while ((dp = readdir(dirp)) != NULL) {
			longFormat(dp->d_name, path);
		}
	}


	return 0;
}
