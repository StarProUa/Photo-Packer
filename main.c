#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_PATH_LENGTH 256

int pack(const char *path)
{
	DIR *dir;
	struct dirent *files;
	dir = opendir(path);

	if(dir == NULL)
	{
		perror("Error opening directory");
		return -1;
	}

	char binPath[MAX_PATH_LENGTH];
	char *slash = strrchr(path, '\\');
	snprintf(binPath, sizeof(binPath), "%s%s.asset", path, slash);

	FILE *bin = fopen(binPath, "wb");

	if(bin == NULL)
	{
		perror("Error creating binary file");
		return -1;
	}

	while((files = readdir(dir)) != NULL)
	{
		if(strstr(files->d_name, ".png") != NULL)
		{
			char filePath[MAX_PATH_LENGTH];
			snprintf(filePath, sizeof(filePath), "%s\\%s", path, files->d_name);

			FILE *file = fopen(filePath, "rb");

			if(file == NULL)
			{
				perror("Error opening a file");
				return -1;
			}

			char skip = '\O';
			fwrite(&skip, sizeof(skip), 1, bin);

			char name[255];
			strncpy(name, files->d_name, sizeof(name));
			fwrite(&name, sizeof(name), 1, bin);

			fseek(file, 0, SEEK_END);
			long size = ftell(file);
			fseek(file, 0, SEEK_SET);
			fwrite(&size, sizeof(size), 1, bin);

			char data[size];
			fread(&data, 1, sizeof(data), file);
			fwrite(&data, 1, sizeof(data), bin);

			fclose(file);
		}
	}

	closedir(dir);
	fclose(bin);
	return 0;
}

int unpack(const char *path)
{
	DIR *dir;
	struct dirent *bins;
	dir = opendir(path);

	if(dir == NULL)
	{
		perror("Error opening directory");
		return -1;
	}

	while((bins = readdir(dir)) != NULL)
	{
		if(strstr(bins->d_name, ".asset") != NULL)
		{
			char binPath[MAX_PATH_LENGTH];
			snprintf(binPath, sizeof(binPath), "%s\\%s", path, bins->d_name);

			FILE *bin = fopen(binPath, "rb");

			if(bin == NULL)
			{
				perror("Error opening binary file");
				return -1;
			}

			while(1)
			{
				char skip;
				if((fread(&skip, sizeof(skip), 1, bin)) != 1) break;

				char name[255];
				if((fread(&name, sizeof(name), 1, bin)) != 1) break;

				char filePath[MAX_PATH_LENGTH];
				snprintf(filePath, sizeof(filePath), "%s\\%s", path, name);

				FILE *file = fopen(filePath, "wb");

				if(file == NULL)
				{
					perror("Error creating a file");
					return -1;
				}

				long size;
				fread(&size, sizeof(size), 1, bin);

				char data[size];
				fread(&data, 1, sizeof(data), bin);

				fwrite(&data, 1, sizeof(data), file);
				fclose(file);
			}

			fclose(bin);
		}
	}


	return 0;
}

int main(int argc, char *argv[])
{
	if(argc == 1)
	{
		printf("Usage: %s --pack|--unpack [path]\n", argv[0]);
		return -1;
	}

	char path[MAX_PATH_LENGTH];

	if(argc == 3)
	{
		if(strlen(argv[2]) < MAX_PATH_LENGTH)
		{
			strncpy(path, argv[2], MAX_PATH_LENGTH);
			strncat(path, "\\", MAX_PATH_LENGTH - strlen(path));
		}
		else
		{
			fprintf(stderr, "Path is too long.\n");
			return -1;
		}
	}
	else
	{
		if(getcwd(path, sizeof(path)) == NULL)
		{
			perror("Error getting current directory");
			return -1;
		}
	}

	if(strcmp(argv[1], "--pack") == 0)
	{
		return pack(path);
	}
	else if(strcmp(argv[1], "--unpack") == 0)
	{
		return unpack(path);
	}
	else
	{
		fprintf(stderr, "Unknown command: %s\n", argv[1]);
		return -1;
	}

	system("pause");
}
