#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "stb_image.h"
#include "stb_image_write.h"

#define IS_NBIT_SET(data, pos) ((data >> pos) & 1U)
#define SET_NBIT(data, n, val) do { data ^= (-val ^ data) & (1UL << n); } while(0);
#define GET_NBIT(data, n) IS_NBIT_SET(data, n)
#define TOGGLE_NBIT(data, n) (data ^= 1UL << n)

// Note: LSB technique is requires 4 bytes of data to store 1 byte of message.
int32_t EncodeImage(const char* filePath, const char* message);
int32_t DecodeImage(const char* filePath);

int main(int argc, char** argv) {
	if (argc < 2) {
		printf("Usage: %s encode/decode [arguments]\n\n[arguments]\n   <filename>  Input File To encode/decode message into/from\n   <message>   If Encoding, A Message To Encode Is REQUIRED\n", argv[0]);
		return 1;
	}

	if (strncmp(argv[1], "encode", 6) == 0 && argc >= 4) {
		return EncodeImage(argv[2], argv[3]);
	} else if (strncmp(argv[1], "decode", 6) == 0 && argc >= 3) {
		return DecodeImage(argv[2]);
	} else {
		printf("Usage: %s encode/decode [arguments]\n\n[arguments]\n   <filename>  Input File To encode/decode message into/from\n   <message>  If Encoding, A Message To Encode Is REQUIRED\n", argv[0]);
	}

	return 0;
}

int32_t EncodeImage(const char* filePath, const char* message) {
	int32_t result = 0;

	printf("- Input Image: %s\n", filePath);
	printf("- Message: %s\n", message);

    int w = -1, h = -1, channels = -1;
    uint8_t* imgData = stbi_load(filePath, &w, &h, &channels, 0);

	if (imgData == NULL || w < 1 || h < 1 || channels < 1) {
		printf("Error occured while reading the image! %dx%d - %d\n", w, h, channels);
		result = 1;
		goto end;
	}

	{
		int32_t len = strlen(message) + 1;
		int32_t imgCap = w * h * channels;
		if ((len * 4) > imgCap) {
			printf("Image too small! %d more bytes required!\n", (len * 4) - imgCap);
		} else {
			printf("- Opened Image %dx%d with %d channels\n", w, h, channels);
		}
	}

	int32_t msgLen = strlen(message) + 1;
	for (int32_t i = 0; i < msgLen; ++i) {
		uint8_t msgByte = *(message + i);
		uint8_t* compPtr = imgData + (i * 4);

		SET_NBIT(*(compPtr + 0), 0, IS_NBIT_SET(msgByte, 0));
		SET_NBIT(*(compPtr + 0), 1, IS_NBIT_SET(msgByte, 1));

		SET_NBIT(*(compPtr + 1), 0, IS_NBIT_SET(msgByte, 2));
		SET_NBIT(*(compPtr + 1), 1, IS_NBIT_SET(msgByte, 3));

		SET_NBIT(*(compPtr + 2), 0, IS_NBIT_SET(msgByte, 4));
		SET_NBIT(*(compPtr + 2), 1, IS_NBIT_SET(msgByte, 5));

		SET_NBIT(*(compPtr + 3), 0, IS_NBIT_SET(msgByte, 6));
		SET_NBIT(*(compPtr + 3), 1, IS_NBIT_SET(msgByte, 7));
	}

	stbi_write_png("output.png", w, h, channels, imgData, 0);

end:
    if (imgData) stbi_image_free(imgData);
    return result;
}

int32_t DecodeImage(const char* filePath) {
	int32_t result = 0;

	printf("- Input Image: %s\n", filePath);

    int w = -1, h = -1, channels = -1;
    uint8_t* imgData = stbi_load(filePath, &w, &h, &channels, 0);

	if (imgData == NULL || w < 1 || h < 1 || channels < 1) {
		printf("Error occured while reading the image! %dx%d - %d\n", w, h, channels);
		result = 1;
		goto end;
	}

	int32_t messageLenMax = (w * h * channels) / 4;
	uint8_t* message = (uint8_t*)calloc(messageLenMax, 1);

	for (int32_t i = 0; i < messageLenMax; ++i) {
		uint8_t msgByte = *(message + i);
		uint8_t* compPtr = imgData + (i * 4);

		SET_NBIT(msgByte, 0, IS_NBIT_SET(*(compPtr + 0), 0));
		SET_NBIT(msgByte, 1, IS_NBIT_SET(*(compPtr + 0), 1));

		SET_NBIT(msgByte, 2, IS_NBIT_SET(*(compPtr + 1), 0));
		SET_NBIT(msgByte, 3, IS_NBIT_SET(*(compPtr + 1), 1));

		SET_NBIT(msgByte, 4, IS_NBIT_SET(*(compPtr + 2), 0));
		SET_NBIT(msgByte, 5, IS_NBIT_SET(*(compPtr + 2), 1));

		SET_NBIT(msgByte, 6, IS_NBIT_SET(*(compPtr + 3), 0));
		SET_NBIT(msgByte, 7, IS_NBIT_SET(*(compPtr + 3), 1));

		*(message + i) = msgByte;

		if (msgByte == '\0') break;
	}

	printf("- Message: %s\n", message);

end:
    if (imgData) stbi_image_free(imgData);
    if (message) free(message);
    return result;
}

