#include "../include/bmp_io.h"

void pixels_free(Pixels *pixels)
{
    if (pixels == NULL)
        return;

    free_pixels_arr(pixels->pix_arr);

    free(pixels);
}

void bmp_free(BMP *bmp)
{
    if (bmp == NULL)
        return;

    pixels_free(bmp->pixels);

    if (bmp->file_header != NULL)
        free(bmp->file_header);
    if (bmp->info_header != NULL)
        free(bmp->info_header);

    free(bmp);
}

size_t get_padding(const BMP *bmp)
{
    size_t padding = (4 - (bmp->info_header->width * sizeof(RGB)) % 4) % 4;

    return padding;
}

int bmp_is_format_correct(const BMP *bmp)
{
    if (bmp->info_header->headerSize != sizeof(BitmapInfoHeader)) {
        fprintf(stderr,
                "bmp_is_format_correct: unsupported headerSize value: %d, "
                "expected: %zu\n",
                bmp->info_header->headerSize, sizeof(BitmapInfoHeader));
        return 0;
    }
    if (bmp->info_header->bitsPerPixel != sizeof(RGB) * 8) {
        fprintf(stderr,
                "bmp_is_format_correct: unsupported bitsPerPixel value: %d, "
                "expected: %zu\n",
                bmp->info_header->bitsPerPixel, sizeof(RGB) * 8);
        return 0;
    }
    if (bmp->info_header->compression != 0) {
        fprintf(stderr,
                "bmp_is_format_correct: unsupported compression value: %d, "
                "expected: %d\n",
                bmp->info_header->compression, 0);
        return 0;
    }

    return 1;
}

BMP *bmp_read(const char file_name[])
{
    BMP *bmp = bmp_create();
    if (bmp == NULL) {
        fprintf(stderr, "bmp_read: bmp_create returned NULL\n");
        return NULL;
    }

    FILE *file = fopen(file_name, "rb");

    if (!file) {
        perror("bmp_read: Cannot open file");
        bmp_free(bmp);
        return NULL;
    }

    if (!fread(bmp->file_header, 1, sizeof(BitmapFileHeader), file) ||
        !fread(bmp->info_header, 1, sizeof(BitmapInfoHeader), file)) {
        fprintf(stderr, "bmp_read: Failed to read headers\n");
        fclose(file);
        bmp_free(bmp);
        return NULL;
    }

    if (!bmp_is_format_correct(bmp)) {
        fprintf(stderr, "bmp_read: wrong bmp format\n");
        fclose(file);
        bmp_free(bmp);
        return NULL;
    }

    Pixels *pixels = malloc(sizeof(Pixels));

    if (!pixels) {
        perror("bmp_read: Pixels struct malloc failed");
        fclose(file);
        bmp_free(bmp);
        return NULL;
    }

    bmp->pixels = pixels;

    pixels->H = bmp->info_header->height;
    pixels->W = bmp->info_header->width;

    size_t H = pixels->H;
    size_t W = pixels->W;

    RGB **arr = allocate_pixels(H, W);

    pixels->pix_arr = arr;

    if (!arr) {
        perror("bmp_read: allocate_pixels returned NULL.\n");
        fclose(file);
        bmp_free(bmp);
        return NULL;
    }

    size_t padding = get_padding(bmp);

    for (int i = H - 1; i >= 0; i--) {
        if (fread(arr[i], 1, W * sizeof(RGB), file) < (W * sizeof(RGB))) {
            fprintf(stderr, "bmp_read: unexpected EOF at row %d\n", i);
            fclose(file);
            bmp_free(bmp);
            return NULL;
        }

        if (padding > 0)
            fseek(file, padding, SEEK_CUR);
    }

    fclose(file);

    return bmp;
}

BMP *bmp_create()
{
    BMP *bmp = malloc(sizeof(BMP));
    if (!bmp) {
        perror("bmp_init: BMP struct malloc failed");
        return NULL;
    }

    BitmapFileHeader *bmfh = calloc(1, sizeof(BitmapFileHeader));
    if (!bmfh) {
        perror("bmp_init: BitmapFileHeader struct malloc failed");
        free(bmp);
        return NULL;
    }

    BitmapInfoHeader *bmih = calloc(1, sizeof(BitmapInfoHeader));
    if (!bmih) {
        perror("bmp_init: BitmapInfoHeader struct malloc failed");
        free(bmp);
        free(bmfh);
        return NULL;
    }

    bmp->file_header = bmfh;
    bmp->info_header = bmih;
    bmp->pixels = NULL;

    return bmp;
}

int bmp_write(const BMP *bmp, const char file_name[])
{
    if (bmp == NULL || file_name == NULL) {
        fprintf(stderr, "bmp_write: invalid arguments\n");
        return 0;
    }

    FILE *file = fopen(file_name, "wb");

    if (!file) {
        perror("bmp_write: cannot open file");
        return 0;
    }

    if (fwrite(bmp->file_header, sizeof(BitmapFileHeader), 1, file) != 1 ||
        fwrite(bmp->info_header, sizeof(BitmapInfoHeader), 1, file) != 1) {
        fprintf(stderr, "bmp_write: failed to write headers\n");
        fclose(file);
        return 0;
    }

    RGB **curr_arr = bmp->pixels->pix_arr;
    int counter = bmp->info_header->height - 1;

    size_t padding = get_padding(bmp);
    size_t row_size = bmp->pixels->W * sizeof(RGB);

    while (counter >= 0 && curr_arr[counter] != NULL) {
        if (fwrite(curr_arr[counter--], row_size, 1, file) != 1) {
            fprintf(stderr, "bmp_write: pixels writing failed on iteration %d\n", counter);
            fclose(file);
            return 0;
        }

        if (padding > 0) {
            uint8_t zeros[3] = {0, 0, 0};
            fwrite(zeros, 1, padding, file);
        }
    }

    fclose(file);

    return 1;
}

void bmp_print_info(const BMP *bmp)
{
    if (bmp == NULL || bmp->file_header == NULL || bmp->info_header == NULL) {
        printf("No data available.\n");
        return;
    }
    BitmapFileHeader *bfh = bmp->file_header;
    BitmapInfoHeader *bih = bmp->info_header;

    printf("\n=== BitmapFileHeader ===\n");
    printf("%-20s: 0x%04X\n", "signature", bfh->signature);
    printf("%-20s: %-10u bytes\n", "filesize", bfh->filesize);
    printf("%-20s: %-10u bytes\n", "pixelArrOffset", bfh->pixelArrOffset);

    printf("\n=== BitmapInfoHeader ===\n");
    printf("%-20s: %-10u bytes\n", "headerSize", bih->headerSize);
    printf("%-20s: %-10d px\n", "width", bih->width);
    printf("%-20s: %-10d px\n", "height", bih->height);
    printf("%-20s: %-10u bits\n", "bitsPerPixel", bih->bitsPerPixel);
    printf("%-20s: %-10u\n", "compression", bih->compression);
    printf("%-20s: %-10u bytes\n", "imageSize", bih->imageSize);

    if (bmp->pixels) {
        printf("\n=== Pixel Data ===\n");
        printf("%-20s: %-10u\n", "stored height", bmp->pixels->H);
        printf("%-20s: %-10u\n", "stored width", bmp->pixels->W);
        printf("%-20s: %-10zu bytes\n", "padding", get_padding(bmp));
    }
    printf("========================\n\n");
}