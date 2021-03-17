// myls.c ... my very own "ls" implementation

#include <sys/types.h>
#include <sys/stat.h>

#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __linux__
# include <bsd/string.h>
#endif
#include <sysexits.h>
#include <unistd.h>

#define MAXDIRNAME 256
#define MAXFNAME 256
#define MAXNAME 24

char *rwxmode (mode_t, char *);
char *username (uid_t, char *);
char *groupname (gid_t, char *);

int main (int argc, char *argv[])
{
	// string buffers for various names
	 char uname[MAXNAME+1]; // UNCOMMENT this line
	 char gname[MAXNAME+1]; // UNCOMMENT this line
	 char mode[MAXNAME+1]; // UNCOMMENT this line

	// collect the directory name, with "." as default
	char dirname[MAXDIRNAME] = ".";
	if (argc >= 2)
		strlcpy (dirname, argv[1], MAXDIRNAME);

	// check that the name really is a directory
	struct stat info;
	if (stat (dirname, &info) < 0)
		err (EX_OSERR, "%s", dirname);

	if (! S_ISDIR (info.st_mode)) {
		errno = ENOTDIR;
		err (EX_DATAERR, "%s", dirname);
	}

	// open the directory to start reading
	 DIR *df = opendir(dirname); // UNCOMMENT this line
	// ... TODO ...
	

	// read directory entries
	 struct dirent *entry; // UNCOMMENT this line
	// ... TODO ...
	while ((entry = readdir(df)) != NULL) {
	    if (entry->d_name[0] == '.') {
	        continue;
	    }
	    struct stat new;
	    
	    char buffer[BUFSIZ] = {0};
	    strcpy(buffer, dirname);
	    strcat(buffer, "/");
	    strcat(buffer, entry->d_name);
	    
	    stat (buffer, &new);
	    printf (
	        "%s  %-8.8s %-8.8s %8lld  %s\n",
	        rwxmode (new.st_mode, mode),
	        username (new.st_uid, uname),
	        groupname(new.st_gid, gname),
	        (long long) new.st_size,
	        entry->d_name
        );
	}

	// finish up
	 closedir(df); // UNCOMMENT this line

	return EXIT_SUCCESS;
}

// convert octal mode to -rwxrwxrwx string
char *rwxmode (mode_t mode, char *str)
{

    if ((mode & S_IFMT) == S_IFLNK){
        str[0] = 'l';
    }
    else if ((mode & S_IFMT) == S_IFDIR){
        str[0] = 'd';
    }
    else if ((mode & S_IFMT) == S_IFREG){
        str[0] = '-';
    }
    else {
        str[0] = '?';
    }
    
    if ((mode & S_IRUSR) == S_IRUSR)
        str[1] = 'r';
    
    if ((mode & S_IWUSR) == S_IWUSR)
        str[2] = 'w';

    if ((mode & S_IXUSR) == S_IXUSR)
        str[3] = 'x';
    
    if ((mode & S_IRGRP) == S_IRGRP)
        str[4] = 'r';
    
    if ((mode & S_IWGRP) == S_IWGRP)
        str[5] = 'w';

    if ((mode & S_IXGRP) == S_IXGRP)
        str[6] = 'x';
    
    if ((mode & S_IROTH) == S_IROTH)
        str[7] = 'r';
    
    if ((mode & S_IWOTH) == S_IWOTH)
        str[8] = 'w';

    if ((mode & S_IXOTH) == S_IXOTH)
        str[9] = 'x';
    
    for (int i = 0; i < 10; i++){
        if  ((str[i] == 'l') || (str[i] == 'd') ||
             (str[i] == '?') || (str[i] == 'r') ||
             (str[i] == 'x') || (str[i] == 'w')) {
             continue;
        }
        str[i] = '-';
    }
    
    str[10] = 0;
    
    
       
	// ... TODO ...
	
	
	
	return str;
}

// convert user id to user name
char *username (uid_t uid, char *name)
{
	struct passwd *uinfo = getpwuid (uid);
	if (uinfo != NULL)
		snprintf (name, MAXNAME, "%s", uinfo->pw_name);
	else
		snprintf (name, MAXNAME, "%d?", (int) uid);
	return name;
}

// convert group id to group name
char *groupname (gid_t gid, char *name)
{
	struct group *ginfo = getgrgid (gid);
	if (ginfo != NULL)
		snprintf (name, MAXNAME, "%s", ginfo->gr_name);
	else
		snprintf (name, MAXNAME, "%d?", (int) gid);
	return name;
}
