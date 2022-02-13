#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <malloc.h>

typedef char *fileName_t;      // Тип ключа
typedef int countCharacters_t; // Тип значения

struct li
{
    fileName_t key;
    countCharacters_t value;
    struct li *next;
};

struct list
{
    struct li *first;
    int size;
};

struct list *new_list(void)
{
    struct list *res = (struct list *)malloc(sizeof(struct list));
    res->first = NULL;
    res->size = 0;
    return res;
}

struct li *new_list_item(void)
{
    struct li *res = (struct li *)malloc(sizeof(struct li));
    res->next = 0;
    return res;
}

int insert_item(struct list *l, fileName_t key, countCharacters_t value)
{
    struct li *it = new_list_item();
    if (it == NULL)
        return 0;
    it->key = key;
    it->value = value;
    it->next = l->first;
    l->first = it;
    l->size++;
    return 1;
}

int insert_item_uniq(struct list *l, fileName_t key, countCharacters_t value)
{
    if (l)
    {
        struct li *it;
        for (it = l->first; it != NULL; it = it->next)
        {
            if (it->key == key)
            {
                it->value = value;
                return 1;
            }
        }
        return insert_item(l, key, value);
    }
    else
    {
        return 0;
    }
}

void print_list(struct list *l)
{
    struct li *it;
    printf("Size=%d\n", l->size);
    for (it = l->first; it != NULL; it = it->next)
    {
        printf("(%s, %d) -> ", it->key, it->value);
    }

    printf("NULL\n");
}

int find_item(struct list *l, fileName_t key, countCharacters_t *value)
{
    struct li *it;
    for (it = l->first; it != NULL; it = it->next)
    {
        if (it->key == key)
        {
            *value = it->value;
            return 1;
        }
    }

    return 0;
}

int countCharactersInFile(char *filename, char *dir)
{
    char *slesh = "/";
    char resultPath[1024];
    strcpy(resultPath, dir);
    strcat(resultPath, slesh);
    strcat(resultPath, filename);

    int in;
    int nread;
    char block[1024];
    int count = 0;

    in = open(resultPath, O_RDONLY);
    while ((nread = read(in, block, sizeof(block))) > 0)
        for (int i = 0; i < nread; i++)
        {
            if (isalpha(block[i]) != 0)
            {
                count = count + 1;
            }
        }

    close(in);
    printf("%s количество букв: %d\n", filename, count);
    return count;
}

countCharacters_t getMaxCount(struct list *l)
{
    struct li *it;
    countCharacters_t maxCount = 0;
    for (it = l->first; it != NULL; it = it->next)
    {
        if (it->value > maxCount)
        {
            maxCount = it->value;
        }
    }
    return maxCount;
}

void writeMaxCountAndFileNamesInFile(struct list *l)
{
    countCharacters_t maxCount = getMaxCount(l);
    struct li *it;
    int out;
    int nread;
    out = open("output/result2.txt", O_WRONLY | O_CREAT);
    for (it = l->first; it != NULL; it = it->next)
    {
        if (it->value == maxCount)
        {
            char bl[sizeof(maxCount) + sizeof(it->key)];
            sprintf(bl, "%d - %s", maxCount, it->key);
            write(out, bl, sizeof(bl));
        }
    }
}

void sumCharactersInDirFiles(char *dir)
{
    DIR *dp;
    struct dirent *entry;
    struct list *l = new_list();
    fileName_t key;
    countCharacters_t value;

    if ((dp = opendir(dir)) == NULL)
    {
        fprintf(stderr, "cannot open directory: %s\n", dir);
        return;
    }

    int sumCharaters = 0;

    while ((entry = readdir(dp)) != NULL)
        if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
            ;
        else
        {
            insert_item_uniq(l, entry->d_name, countCharactersInFile(entry->d_name, dir));
        }

    print_list(l);
    writeMaxCountAndFileNamesInFile(l);
    closedir(dp);
}

void writeResultSumCharacters(int result)
{
    if (result == -1)
    {
        return;
    }
    int out;
    int nread;
    char bl[5];
    sprintf(bl, "%d", result);
    out = open("output/result1.txt", O_WRONLY | O_CREAT);
    write(out, bl, sizeof(bl));
}

int main()
{
    printf("Directory scan of /home:\n");
    sumCharactersInDirFiles("/home/daniil/Рабочий стол/daniil6306/lab2/input");
    printf("done.\n");
    exit(0);
}