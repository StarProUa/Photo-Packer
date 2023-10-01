#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <dirent.h>
#include <getopt.h>
#include <libgen.h>

#define MAX_PATH_LENGTH 256

#define WRONG_SYNTAX \
fprintf(stderr, "Wrong syntax use required arg [pack,unpack] and additional [filter][name][remove][dir]\n")

#define FILE_OPEN(file, path) \
if(file == NULL) { fprintf(stderr, "Error opening file: %s", path); getchar(); return ;}

void getDirFromPath(const char *path, char *dir)
{
	char *copy = strdup(path);
	strcpy(dir, dirname(copy));
	free(copy);
}

void getNameFromPath(const char *path, char *name)
{
	char *copy = strdup(path);
	strcpy(name, basename(copy));
	free(copy);
}

void getNameFromDir(const char *dir, char *name)
{
	char *copy = strdup(dir);
	char *token = strtok(copy, "\\");

	while(token != NULL)
	{
		strcpy(name, token);
		token = strtok(NULL, "\\");
	}
	free(copy);
}

void getSizeFile(FILE *file, long *size)
{
	fseek(file, 0, SEEK_END);
	*size = ftell(file);
	fseek(file, 0, SEEK_SET);
}

void pack(FILE *don, const char *path, char *name)
{
	FILE *file = fopen(path, "rb");

	FILE_OPEN(file, path);

	char skip = '$';

	long size;
	getSizeFile(file, &size);

	char data[size];
	fread(&data, sizeof(data), 1, file);

	fwrite(&skip, 1, sizeof(skip), don);

	char fname[MAX_PATH_LENGTH];
	strcpy(fname, name);

	fwrite(&fname, 1, MAX_PATH_LENGTH, don);

	fwrite(&size, 1, sizeof(size), don);

	fwrite(&data, 1, sizeof(data), don);

	fclose(file);
}

void unpack(const char *dpath)
{
	FILE *don = fopen(dpath, "rb");

	FILE_OPEN(don, dpath);

	while(1)
	{
		char skip;
		if((fread(&skip, sizeof(skip), 1, don)) != 1) break;

		char name[MAX_PATH_LENGTH];
		if((fread(&name, sizeof(name), 1, don)) != 1) break;

		long size;
		if((fread(&size, sizeof(size), 1, don)) != 1) break;

		char data[size];
		if((fread(&data, sizeof(data), 1, don)) != 1) break;

		char dir[MAX_PATH_LENGTH];

		getDirFromPath(dpath, dir);

		char path[strlen(dir) + strlen(name) + 6];
		snprintf(path, sizeof(path), "%s\\%s", dir, name);

		FILE *file = fopen(path, "wb");

		if(file == NULL)
		{
			fclose(don);
			FILE_OPEN(file, path);
		}

		fwrite(&data, 1, sizeof(data), file);

		fclose(file);
	}

	fclose(don);
}

void withArg(const int *action, char *dir, const int *argc, char *argv[],
			 int start, char *name, char *filter, const int *erase)
{
	DIR *folder;
	struct dirent *files;

	if(strcmp(dir, "") == 0) getcwd(dir, MAX_PATH_LENGTH);

	folder = opendir(dir);

	FILE_OPEN(folder, dir);

	if((*action) == 1)
	{
		FILE *don;

		if(strcmp(name, "") == 0) getNameFromDir(dir, name);

		char dpath[strlen(dir) + strlen(name) + 6];
		snprintf(dpath, sizeof(dpath), "%s\\%s.don", dir, name);

		don = fopen(dpath, "wb");

		FILE_OPEN(don, dpath);

		while((files = readdir(folder)) != NULL)
		{
			if(strstr(files->d_name, name) != NULL) continue;

			if(strcmp(files->d_name, ".") == 0 || strcmp(files->d_name, "..") == 0) continue;

			if(filter == NULL ? "1" : strstr(files->d_name, filter))
			{
				char fpath[strlen(dir) + strlen(files->d_name) + 5];
				snprintf(fpath, sizeof(fpath), "%s\\%s", dir, files->d_name);

				pack(don, fpath, files->d_name);

				if(*erase) remove(fpath);
			}
		}

		fclose(don);
	}
	else
	{
		while((files = readdir(folder)) != NULL)
		{
			if((strstr(files->d_name, (filter == NULL) ? "" : filter)) != NULL
				&& (strstr(files->d_name, ".don")) != NULL)
			{
				char dpath[strlen(dir) + strlen(files->d_name) + 5];
				snprintf(dpath, sizeof(dpath), "%s\\%s", dir, files->d_name);

				unpack(dpath);

				if(*erase) remove(dpath);
			}
		}
	}

	closedir(folder);
}

void withoutArg(const int *action, char *dir, const int *argc, char *argv[], int start)
{
	if((*action) == 1)
	{
		FILE *don;

		getcwd(dir, MAX_PATH_LENGTH);

		char name[MAX_PATH_LENGTH];
		getNameFromPath(dir, name);

		char path[strlen(dir) + strlen(name) + 5];
		sprintf(path, "%s\\%s.don", dir, name);

		don = fopen(path, "wb");

		FILE_OPEN(don, path);

		for(int i = start; i < (*argc); i++)
		{
			getNameFromPath(argv[i], name);
			pack(don, argv[i], name);
		}

		fclose(don);
	}
	else
	{
		for(int i = start; i < (*argc); i++)
		{
			if((strstr(argv[i], ".don")) != NULL)
			{
				unpack(argv[i]);
			}
		}
	}
}

#define EMPTY_ARG (erase == 0 && filter == NULL && strcmp(name, "") == 0 && strcmp(dir, "") == 0)
#define NOT_EMPTY_ARG (erase != 0 || filter != NULL || strcmp(name, "") != 0 || strcmp(dir, "") != 0)

int main(int argc, char *argv[])
{
	int action = 0;
	char *filter = NULL;
	int erase = 0;

	char name[MAX_PATH_LENGTH];
	memset(name, 0, sizeof(name));

	char dir[MAX_PATH_LENGTH];
	memset(dir, 0, sizeof(dir));

	struct option data[] = {
		{"pack", no_argument, 0, 'p'},
		{"unpack", no_argument, 0, 'u'},
		{"name", required_argument, 0, 'n'},
		{"filter", required_argument, 0, 'f'},
		{"erase", no_argument, 0, 'e'},
		{"dir", required_argument, 0, 'd'},
		{0, 0, 0, 0}
	};

	int opt;
	while((opt = getopt_long(argc, argv, "pun:f:ed:", data, NULL)) != -1)
	{
		switch(opt)
		{
			case 'p':
				action = 1;
				break;
			case 'u':
				action = 2;
				break;
			case 'n':
				strncpy(name, optarg, sizeof(name));
				break;
			case 'f':
				filter = optarg;
				break;
			case 'd':
				strncpy(dir, optarg, sizeof(dir));
				break;
			case 'e':
				erase = 1;
				break;
			default:
				fprintf(stderr, "Unknown option: %c\n", opt);
				return -1;
		}
	}

	if(action == 0 && argc == 1) // не вказан
	{
		WRONG_SYNTAX;
		return -1;
	}
	else
	{
		if(action != 0 && NOT_EMPTY_ARG) // вказан + аргументи
		{
			withArg(&action, dir, &argc, argv, 2, name, filter, &erase);
		}
		else if(action != 0 && argc == 2) // вказаний
		{
			withArg(&action, dir, &argc, argv, 2, name, filter, &erase);
		}
		else if(action != 0 && EMPTY_ARG && argc > 2) // вказан + файли
		{
			withoutArg(&action, dir, &argc, argv, 2);
		}
		else if(action == 0 && EMPTY_ARG) // не вказан + файли
		{
			action = 1;
			for(int i = 1; i < argc; i++)
			{
				if(strstr(argv[i], ".don"))
				{
					action = 2;
					break;
				}
			}
			withoutArg(&action, dir, &argc, argv, 1);
		}
		else if(action == 0 && NOT_EMPTY_ARG) // не вказан + аргументи
		{
			WRONG_SYNTAX;
			return -1;
		}
	}

	return 0;
}
