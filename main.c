#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <dirent.h>
#include <getopt.h>
#include <libgen.h>

#define MAX_PATH_LENGTH 256
#define WRONG_SYNTAX fprintf(stderr, "Wrong syntax use required arg [pack,unpack] and additional [filter][name][remove][dir]\n")

//int pack(const char *path, const char *filter)
//{
//	DIR *dir;
//	struct dirent *files;
//	dir = opendir(path);

//	if(dir == NULL)
//	{
//		perror("Error opening directory");
//		return -1;
//	}

//	char binPath[MAX_PATH_LENGTH];
//	char binName[MAX_PATH_LENGTH - strlen(path) - strlen(filter)];

//	char temp[strlen(path)];
//	strncpy(temp, path, sizeof(temp));
//	char *token = strtok(temp, "\\");

//	while(token != NULL)
//	{
//		snprintf(binName, sizeof(binName), "\\%s", token);
//		//strncpy(binName, token, sizeof(binName));
//		token = strtok(NULL, "\\");
//	}

//	snprintf(binPath, sizeof(binPath), "%s%s.asset", path, binName);

//	FILE *file = fopen("debug.txt", "w");
//	fprintf(file, "%s", binName);
//	fclose(file);

//	FILE *bin = fopen(binPath, "wb");

//	if(bin == NULL)
//	{
//		perror("Error creating binary file");
//		return -1;
//	}

//	while((files = readdir(dir)) != NULL)
//	{
//		if(strstr(files->d_name, filter) != NULL)
//		{
//			char filePath[MAX_PATH_LENGTH];
//			snprintf(filePath, sizeof(filePath), "%s\\%s", path, files->d_name);

//			FILE *file = fopen(filePath, "rb");

//			if(file == NULL)
//			{
//				perror("Error opening a file");
//				return -1;
//			}

//			char skip = '\O';
//			fwrite(&skip, sizeof(skip), 1, bin);

//			char name[255];
//			strncpy(name, files->d_name, sizeof(name));
//			fwrite(&name, sizeof(name), 1, bin);

//			fseek(file, 0, SEEK_END);
//			long size = ftell(file);
//			fseek(file, 0, SEEK_SET);
//			fwrite(&size, sizeof(size), 1, bin);

//			char data[size];
//			fread(&data, 1, sizeof(data), file);
//			fwrite(&data, 1, sizeof(data), bin);

//			fclose(file);
//		}
//	}

//	closedir(dir);
//	fclose(bin);
//	return 0;
//}

//int unpack(const char *path)
//{
//	DIR *dir;
//	struct dirent *bins;
//	dir = opendir(path);

//	if(dir == NULL)
//	{
//		perror("Error opening directory");
//		return -1;
//	}

//	while((bins = readdir(dir)) != NULL)
//	{
//		if(strstr(bins->d_name, ".asset") != NULL)
//		{
//			char binPath[MAX_PATH_LENGTH];
//			snprintf(binPath, sizeof(binPath), "%s\\%s", path, bins->d_name);

//			FILE *bin = fopen(binPath, "rb");

//			if(bin == NULL)
//			{
//				perror("Error opening binary file");
//				return -1;
//			}

//			while(1)
//			{
//				char skip;
//				if((fread(&skip, sizeof(skip), 1, bin)) != 1) break;

//				char name[255];
//				if((fread(&name, sizeof(name), 1, bin)) != 1) break;

//				char filePath[MAX_PATH_LENGTH];
//				snprintf(filePath, sizeof(filePath), "%s\\%s", path, name);

//				FILE *file = fopen(filePath, "wb");

//				if(file == NULL)
//				{
//					perror("Error creating a file");
//					return -1;
//				}

//				long size;
//				fread(&size, sizeof(size), 1, bin);

//				char data[size];
//				fread(&data, 1, sizeof(data), bin);

//				fwrite(&data, 1, sizeof(data), file);
//				fclose(file);
//			}

//			fclose(bin);
//		}
//	}


//	return 0;
//}

char *getpPathFromName(const char *name)
{
	char path[MAX_PATH_LENGTH];

	char temp[strlen(name)];
	strncpy(temp, name, sizeof(temp));
	char *token = strtok(temp, "\\");

	while(token != NULL)
	{
		snprintf(path, sizeof(path), "\\%s", token);
		token = strtok(NULL, "\\");
	}

	return path;
}

#define EMPTY_ARG (remove == 0 && filter == NULL && name == NULL && path == NULL)
#define NOT_EMPTY_ARG (remove != 0 || filter != NULL || name != NULL || path != NULL)

int main(int argc, char *argv[])
{
	int action = 0;
	char *filter = NULL;
	char *name = NULL;
	int remove = 0;
	int size = 10;

	char *path = NULL;

	struct option data[] = {
		{"pack", no_argument, 0, 'p'},
		{"unpack", no_argument, 0, 'u'},
		{"name", required_argument, 0, 'n'},
		{"size", required_argument, 0, 's'},
		{"filter", required_argument, 0, 'f'},
		{"remove", no_argument, &remove, 'r'},
		{"dir", required_argument, 0, 'd'},
		{0, 0, 0, 0}
	};

	int opt;
	while((opt = getopt_long(argc, argv, "pun:f:s:rd:", data, NULL)) != -1)
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
				name = optarg;
				break;
			case 'f':
				filter = optarg;
				break;
			case 's':
				size = atoi(optarg);
			case 'd':
				path = optarg;
				break;
			default:
				fprintf(stderr, "Unknown option: %c\n", opt);
				return -1;
		}
	}

	char *arg[(action == 0) ? argc : 1];

	if(action == 0 && argc == 1) // не вказан
	{
		WRONG_SYNTAX;
		return -1;
	}
	else
	{
		if(action != 0 && NOT_EMPTY_ARG) // вказан + аргументи
		{
			// туто дивимось чого не хватає чи є path

			if(path == NULL)
			{

			}

			//
		}
		else if(action != 0 && argc == 2) // вказаний
		{
			if(action == 1)
			{

			}
			else
			{

			}
		}
		else if(action != 0 && EMPTY_ARG && argc > 2) // вказан + файли
		{

			if(action == 1)
			{

			}
			else
			{

			}
			for(int i = optind + 1; i < argc; i++)
			{
				arg[i] = argv[i];

				if((strstr(argv[i], ".don")) != NULL) action = 2;
			}
		}
		else if(action == 0 && EMPTY_ARG) // не вказан + файли
		{
			action = 1;
			for(int i = optind; i < argc; i++)
			{
				arg[i] = argv[i];
										// переделать шоб або те або те
				if((strstr(argv[i], ".don")) != NULL) action = 2;
			}
		}
		else if(action == 0 && NOT_EMPTY_ARG) // не вказан + аргументи
		{
			WRONG_SYNTAX;
			return -1;
		}
	}

	return 0;
}
