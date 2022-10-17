int getNLines(const char *file);
void getData(const char *file, int *array);
void saveData(int *array, int size);

int getNLines(const char *file) {
    FILE *fp = fopen(file, "r");
    if (fp == NULL) {
        perror("Falha ao abrir arquivo.");
        exit(EXIT_FAILURE);
    }

    int n = 0;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, fp)) != -1) {
        if (line[0] != '\n')
            n++;
    }

    if (line)
        free(line);
    fclose(fp);
    return n;
}

void getData(const char *file, int *array) {
    FILE *fp = fopen(file, "r");
    if (fp == NULL) {
        perror("Falha ao abrir arquivo.");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, fp)) != -1)
        if (line[0] != '\n')
            sscanf(line, "%d", &array[i++]);

    if (line)
        free(line);
    fclose(fp);
}

void saveData(int *array, int size) {
    const char *file = "saida.txt";
    FILE *fp = fopen(file, "w+");
    if (fp == NULL) {
        perror("Erro ao criar arquivo.");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++)
        fprintf(fp, "%d\n", array[i]);

    fclose(fp);
}
