#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    float x;
    float y;
    float z;
    float intensity;
} Point;

int main(void)
{
    FILE *csv = fopen("points.csv", "r");
    FILE *obj = fopen("mesh.obj", "w");

    if (!csv || !obj)
    {
        printf("Failed to open file\n");
        return -1;
    }

    int capacity = 10000;
    int count = 0;

    Point *points = malloc(capacity * sizeof(Point));

    char line[256];

    /* Skip header */
    fgets(line, sizeof(line), csv);

    while (fgets(line, sizeof(line), csv))
    {
        if (count >= capacity)
        {
            capacity *= 2;
            points = realloc(points, capacity * sizeof(Point));
        }

        sscanf(line,
               "%f,%f,%f,%f",
               &points[count].x,
               &points[count].y,
               &points[count].z,
               &points[count].intensity);

        count++;
    }

    printf("Loaded %d points\n", count);

    /* Write vertices */
    for (int i = 0; i < count; i++)
    {
        fprintf(obj,
                "v %f %f %f\n",
                points[i].x,
                points[i].y,
                points[i].z);
    }

    /* Create triangles */
    for (int i = 0; i + 2 < count; i += 3)
    {
        fprintf(obj,
                "f %d %d %d\n",
                i + 1,
                i + 2,
                i + 3);
    }

    fclose(csv);
    fclose(obj);

    free(points);

    printf("Created mesh.obj\n");

    return 0;
}