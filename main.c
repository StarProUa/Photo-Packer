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
	snprintf(binPath, sizeof(binPath), "%s.asset", path);

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
			snprintf(filePath, sizeof(filePath), "%s/%s", path, files->d_name);

			FILE *photo = fopen(filePath, "rb");

			if(photo == NULL)
			{
				perror("Error opening a photo file");
				return -1;
			}

			fseek(photo, 0, SEEK_END);
			long size = ftell(photo);
			fseek(photo, 0, SEEK_SET);

			fwrite(&size, sizeof(size), 1, bin);

			char data[size];
			fread(data, 1, size, photo);

			fwrite(files->d_name, sizeof(files->d_name), 1, bin);
			fwrite(data, 1, size, bin);

			fclose(photo);
		}
	}

	closedir(dir);
	fclose(bin);
	return 0;
}

int unpack(const char *path)
{
	char binPath[MAX_PATH_LENGTH];
	snprintf(binPath, sizeof(binPath), "%s.asset", path);

	FILE *bin = fopen(binPath, "rb");

	if(bin == NULL)
	{
		perror("Error opening binary file");
		return -1;
	}

	struct stat st = {0};

	if(stat(path, &st) == -1) mkdir(path);

	while(!feof(bin))
	{
		long size;
		fread(&size, sizeof(size), 1, bin);

		char name[256];
		fread(name, sizeof(name), 1, bin);

		char filePath[MAX_PATH_LENGTH];
		snprintf(filePath, sizeof(filePath), "%s/%s", path, name);

		FILE *photo = fopen(filePath, "wb");

		if(photo == NULL)
		{
			perror("Error creating photo file");
			return -1;
		}

		char data[size];
		fread(data, 1, size, bin);

		fwrite(data, 1, size, photo);
		fclose(photo);
	}

	fclose(bin);
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
}
