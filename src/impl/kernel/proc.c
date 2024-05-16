#include "system.h"

proc task_struct[NPROC]; // 0 for current running process!

int currentPid = 1;

int getPid() {
  return ++ currentPid;
}

uint8 initcode[] = {
  0x55, 0x48, 0x89, 0xe5, 0x48, 0x83, 0xec, 0x10, 0x48, 0xb8, 0x5d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xd0, 0x89, 0x45, 0xfc, 0x83, 0x7d, 0xfc, 0x00, 0x75, 0x20, 0x48, 0xb8, 0x30, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x89, 0xc7, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x48, 0xba, 0x71, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xd2, 0xeb, 0xe0, 0x48, 0xb8, 0x35, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x89, 0xc7, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x48, 0xba, 0x71, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xd2, 0xeb, 0xe0, 0x49, 0xc7, 0xc1, 0x01, 0x00, 0x00, 0x00, 0xcd, 0x80, 0xc3, 0x49, 0xc7, 0xc1, 0x02, 0x00, 0x00, 0x00, 0xcd, 0x80, 0xc3, 0x49, 0xc7, 0xc1, 0x03, 0x00, 0x00, 0x00, 0xcd, 0x80, 0xc3, 0x49, 0xc7, 0xc1, 0x04, 0x00, 0x00, 0x00, 0xcd, 0x80, 0xc3, 0x49, 0xc7, 0xc1, 0x05, 0x00, 0x00, 0x00, 0xcd, 0x80, 0xc3, 0x49, 0xc7, 0xc1, 0x10, 0x00, 0x00, 0x00, 0xcd, 0x80, 0xc3, 0x49, 0xc7, 0xc1, 0x15, 0x00, 0x00, 0x00, 0xcd, 0x80, 0xc3, 0x49, 0xc7, 0xc1, 0x06, 0x00, 0x00, 0x00, 0xcd, 0x80, 0xc3, 0x49, 0xc7, 0xc1, 0x07, 0x00, 0x00, 0x00, 0xcd, 0x80, 0xc3, 0x49, 0xc7, 0xc1, 0x0f, 0x00, 0x00, 0x00, 0xcd, 0x80, 0xc3, 0x49, 0xc7, 0xc1, 0x11, 0x00, 0x00, 0x00, 0xcd, 0x80, 0xc3, 0x49, 0xc7, 0xc1, 0x12, 0x00, 0x00, 0x00, 0xcd, 0x80, 0xc3, 0x49, 0xc7, 0xc1, 0x08, 0x00, 0x00, 0x00, 0xcd, 0x80, 0xc3, 0x49, 0xc7, 0xc1, 0x13, 0x00, 0x00, 0x00, 0xcd, 0x80, 0xc3, 0x49, 0xc7, 0xc1, 0x14, 0x00, 0x00, 0x00, 0xcd, 0x80, 0xc3, 0x49, 0xc7, 0xc1, 0x09, 0x00, 0x00, 0x00, 0xcd, 0x80, 0xc3, 0x49, 0xc7, 0xc1, 0x0a, 0x00, 0x00, 0x00, 0xcd, 0x80, 0xc3, 0x49, 0xc7, 0xc1, 0x0b, 0x00, 0x00, 0x00, 0xcd, 0x80, 0xc3, 0x49, 0xc7, 0xc1, 0x0c, 0x00, 0x00, 0x00, 0xcd, 0x80, 0xc3, 0x49, 0xc7, 0xc1, 0x0d, 0x00, 0x00, 0x00, 0xcd, 0x80, 0xc3, 0x49, 0xc7, 0xc1, 0x0e, 0x00, 0x00, 0x00, 0xcd, 0x80, 0xc3, 0x49, 0xc7, 0xc1, 0x16, 0x00, 0x00, 0x00, 0xcd, 0x80, 0xc3, 0x55, 0x48, 0x89, 0xe5, 0x48, 0x83, 0xec, 0x30, 0x89, 0x7d, 0xdc, 0x89, 0x75, 0xd8, 0x89, 0x55, 0xd4, 0x83, 0x7d, 0xd4, 0x00, 0x74, 0x1c, 0x8b, 0x45, 0xdc, 0xc1, 0xe8, 0x1f, 0x0f, 0xb6, 0xc0, 0x89, 0x45, 0xd4, 0x83, 0x7d, 0xd4, 0x00, 0x74, 0x0a, 0x8b, 0x45, 0xdc, 0xf7, 0xd8, 0x89, 0x45, 0xfc, 0xeb, 0x06, 0x8b, 0x45, 0xdc, 0x89, 0x45, 0xfc, 0xc7, 0x45, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x8b, 0x4d, 0xd8, 0x8b, 0x45, 0xfc, 0xba, 0x00, 0x00, 0x00, 0x00, 0xf7, 0xf1, 0x89, 0xd1, 0x8b, 0x45, 0xf8, 0x8d, 0x50, 0x01, 0x89, 0x55, 0xf8, 0x48, 0xba, 0xa0, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x89, 0xc9, 0x0f, 0xb6, 0x14, 0x0a, 0x48, 0x98, 0x88, 0x54, 0x05, 0xe8, 0x8b, 0x75, 0xd8, 0x8b, 0x45, 0xfc, 0xba, 0x00, 0x00, 0x00, 0x00, 0xf7, 0xf6, 0x89, 0x45, 0xfc, 0x83, 0x7d, 0xfc, 0x00, 0x75, 0xbc, 0x83, 0x7d, 0xd4, 0x00, 0x74, 0x2d, 0x8b, 0x45, 0xf8, 0x8d, 0x50, 0x01, 0x89, 0x55, 0xf8, 0x48, 0x98, 0xc6, 0x44, 0x05, 0xe8, 0x2d, 0xeb, 0x1b, 0x8b, 0x45, 0xf8, 0x48, 0x98, 0x0f, 0xb6, 0x44, 0x05, 0xe8, 0x0f, 0xbe, 0xc0, 0x89, 0xc7, 0x48, 0xb8, 0x2f, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xd0, 0x83, 0x6d, 0xf8, 0x01, 0x83, 0x7d, 0xf8, 0x00, 0x79, 0xdb, 0x90, 0x90, 0xc9, 0xc3, 0x55, 0x48, 0x89, 0xe5, 0x48, 0x83, 0xec, 0x20, 0x48, 0x89, 0x7d, 0xe8, 0xbf, 0x30, 0x00, 0x00, 0x00, 0x48, 0xb8, 0x2f, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xd0, 0xbf, 0x78, 0x00, 0x00, 0x00, 0x48, 0xb8, 0x2f, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xd0, 0xc7, 0x45, 0xfc, 0x00, 0x00, 0x00, 0x00, 0xeb, 0x30, 0x48, 0x8b, 0x45, 0xe8, 0x48, 0xc1, 0xe8, 0x3c, 0x48, 0xba, 0xa0, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xb6, 0x04, 0x02, 0x0f, 0xbe, 0xc0, 0x89, 0xc7, 0x48, 0xb8, 0x2f, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xd0, 0x48, 0xc1, 0x65, 0xe8, 0x04, 0x83, 0x45, 0xfc, 0x01, 0x8b, 0x45, 0xfc, 0x83, 0xf8, 0x0f, 0x76, 0xc8, 0x90, 0x90, 0xc9, 0xc3, 0x55, 0x48, 0x89, 0xe5, 0x48, 0x83, 0xec, 0x70, 0x48, 0x89, 0x7d, 0x98, 0x48, 0x89, 0x75, 0xd8, 0x48, 0x89, 0x55, 0xe0, 0x48, 0x89, 0x4d, 0xe8, 0x4c, 0x89, 0x45, 0xf0, 0x4c, 0x89, 0x4d, 0xf8, 0x48, 0x83, 0x7d, 0x98, 0x00, 0x75, 0x20, 0x48, 0xb8, 0x3d, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x89, 0xc7, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x48, 0xba, 0x71, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xd2, 0xeb, 0xfe, 0xc7, 0x45, 0xa0, 0x08, 0x00, 0x00, 0x00, 0x48, 0x8d, 0x45, 0x10, 0x48, 0x89, 0x45, 0xa8, 0x48, 0x8d, 0x45, 0xd0, 0x48, 0x89, 0x45, 0xb0, 0xc7, 0x45, 0xcc, 0x00, 0x00, 0x00, 0x00, 0x8b, 0x45, 0xcc, 0x48, 0x63, 0xd0, 0x48, 0x8b, 0x45, 0x98, 0x48, 0x01, 0xd0, 0x0f, 0xb6, 0x00, 0x88, 0x45, 0xbf, 0x80, 0x7d, 0xbf, 0x00, 0x0f, 0x84, 0x2f, 0x02, 0x00, 0x00, 0x80, 0x7d, 0xbf, 0x25, 0x74, 0x18, 0x0f, 0xbe, 0x45, 0xbf, 0x89, 0xc7, 0x48, 0xb8, 0x2f, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xd0, 0x83, 0x45, 0xcc, 0x01, 0xeb, 0xc5, 0x83, 0x45, 0xcc, 0x01, 0x8b, 0x45, 0xcc, 0x48, 0x63, 0xd0, 0x48, 0x8b, 0x45, 0x98, 0x48, 0x01, 0xd0, 0x0f, 0xb6, 0x00, 0x88, 0x45, 0xbe, 0x0f, 0xbe, 0x45, 0xbe, 0x83, 0xf8, 0x78, 0x0f, 0x84, 0xe2, 0x00, 0x00, 0x00, 0x83, 0xf8, 0x78, 0x0f, 0x8f, 0xb5, 0x01, 0x00, 0x00, 0x83, 0xf8, 0x73, 0x0f, 0x84, 0x1e, 0x01, 0x00, 0x00, 0x83, 0xf8, 0x73, 0x0f, 0x8f, 0xa3, 0x01, 0x00, 0x00, 0x83, 0xf8, 0x70, 0x74, 0x2e, 0x83, 0xf8, 0x70, 0x0f, 0x8f, 0x95, 0x01, 0x00, 0x00, 0x83, 0xf8, 0x64, 0x74, 0x66, 0x83, 0xf8, 0x64, 0x0f, 0x8f, 0x87, 0x01, 0x00, 0x00, 0x83, 0xf8, 0x25, 0x0f, 0x84, 0x67, 0x01, 0x00, 0x00, 0x83, 0xf8, 0x30, 0x0f, 0x84, 0x9e, 0x01, 0x00, 0x00, 0xe9, 0x70, 0x01, 0x00, 0x00, 0x8b, 0x45, 0xa0, 0x83, 0xf8, 0x2f, 0x77, 0x17, 0x48, 0x8b, 0x45, 0xb0, 0x8b, 0x55, 0xa0, 0x89, 0xd2, 0x48, 0x01, 0xd0, 0x8b, 0x55, 0xa0, 0x83, 0xc2, 0x08, 0x89, 0x55, 0xa0, 0xeb, 0x0c, 0x48, 0x8b, 0x45, 0xa8, 0x48, 0x8d, 0x50, 0x08, 0x48, 0x89, 0x55, 0xa8, 0x48, 0x8b, 0x00, 0x48, 0x89, 0xc7, 0x48, 0xb8, 0xfe, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xd0, 0x83, 0x45, 0xcc, 0x01, 0xe9, 0x54, 0x01, 0x00, 0x00, 0x8b, 0x45, 0xa0, 0x83, 0xf8, 0x2f, 0x77, 0x17, 0x48, 0x8b, 0x45, 0xb0, 0x8b, 0x55, 0xa0, 0x89, 0xd2, 0x48, 0x01, 0xd0, 0x8b, 0x55, 0xa0, 0x83, 0xc2, 0x08, 0x89, 0x55, 0xa0, 0xeb, 0x0c, 0x48, 0x8b, 0x45, 0xa8, 0x48, 0x8d, 0x50, 0x08, 0x48, 0x89, 0x55, 0xa8, 0x8b, 0x00, 0xba, 0x01, 0x00, 0x00, 0x00, 0xbe, 0x0a, 0x00, 0x00, 0x00, 0x89, 0xc7, 0x48, 0xb8, 0x39, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xd0, 0x83, 0x45, 0xcc, 0x01, 0xe9, 0x06, 0x01, 0x00, 0x00, 0x8b, 0x45, 0xa0, 0x83, 0xf8, 0x2f, 0x77, 0x17, 0x48, 0x8b, 0x45, 0xb0, 0x8b, 0x55, 0xa0, 0x89, 0xd2, 0x48, 0x01, 0xd0, 0x8b, 0x55, 0xa0, 0x83, 0xc2, 0x08, 0x89, 0x55, 0xa0, 0xeb, 0x0c, 0x48, 0x8b, 0x45, 0xa8, 0x48, 0x8d, 0x50, 0x08, 0x48, 0x89, 0x55, 0xa8, 0x8b, 0x00, 0xba, 0x01, 0x00, 0x00, 0x00, 0xbe, 0x10, 0x00, 0x00, 0x00, 0x89, 0xc7, 0x48, 0xb8, 0x39, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xd0, 0x83, 0x45, 0xcc, 0x01, 0xe9, 0xb8, 0x00, 0x00, 0x00, 0x8b, 0x45, 0xa0, 0x83, 0xf8, 0x2f, 0x77, 0x17, 0x48, 0x8b, 0x45, 0xb0, 0x8b, 0x55, 0xa0, 0x89, 0xd2, 0x48, 0x01, 0xd0, 0x8b, 0x55, 0xa0, 0x83, 0xc2, 0x08, 0x89, 0x55, 0xa0, 0xeb, 0x0c, 0x48, 0x8b, 0x45, 0xa8, 0x48, 0x8d, 0x50, 0x08, 0x48, 0x89, 0x55, 0xa8, 0x48, 0x8b, 0x00, 0x48, 0x89, 0x45, 0xc0, 0x48, 0x83, 0x7d, 0xc0, 0x00, 0x75, 0x2d, 0x48, 0xb8, 0x4d, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x89, 0x45, 0xc0, 0xeb, 0x1d, 0x48, 0x8b, 0x45, 0xc0, 0x0f, 0xb6, 0x00, 0x0f, 0xbe, 0xc0, 0x89, 0xc7, 0x48, 0xb8, 0x2f, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xd0, 0x48, 0x83, 0x45, 0xc0, 0x01, 0x48, 0x8b, 0x45, 0xc0, 0x0f, 0xb6, 0x00, 0x84, 0xc0, 0x75, 0xd8, 0x83, 0x45, 0xcc, 0x01, 0xeb, 0x41, 0xbf, 0x25, 0x00, 0x00, 0x00, 0x48, 0xb8, 0x2f, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xd0, 0x83, 0x45, 0xcc, 0x01, 0xeb, 0x2a, 0xbf, 0x25, 0x00, 0x00, 0x00, 0x48, 0xb8, 0x2f, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xd0, 0x0f, 0xbe, 0x45, 0xbe, 0x89, 0xc7, 0x48, 0xb8, 0x2f, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xd0, 0x83, 0x45, 0xcc, 0x01, 0xeb, 0x01, 0x90, 0xe9, 0xb4, 0xfd, 0xff, 0xff, 0x90, 0x90, 0xc9, 0xc3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x73, 0x6f, 0x6e, 0x0a, 0x00, 0x66, 0x61, 0x74, 0x68, 0x65, 0x72, 0x0a, 0x00, 0x6e, 0x75, 0x6c, 0x6c, 0x20, 0x70, 0x6f, 0x69, 0x6e, 0x74, 0x65, 0x72, 0x2e, 0x2e, 0x0a, 0x00, 0x28, 0x6e, 0x75, 0x6c, 0x6c, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x7a, 0x52, 0x00, 0x01, 0x78, 0x10, 0x01, 0x1b, 0x0c, 0x07, 0x08, 0x90, 0x01, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0xe0, 0xef, 0xff, 0xff, 0x5d, 0x00, 0x00, 0x00, 0x00, 0x41, 0x0e, 0x10, 0x86, 0x02, 0x43, 0x0d, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0xf9, 0xf0, 0xff, 0xff, 0xc5, 0x00, 0x00, 0x00, 0x00, 0x41, 0x0e, 0x10, 0x86, 0x02, 0x43, 0x0d, 0x06, 0x02, 0xc0, 0x0c, 0x07, 0x08, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x5c, 0x00, 0x00, 0x00, 0x9e, 0xf1, 0xff, 0xff, 0x73, 0x00, 0x00, 0x00, 0x00, 0x41, 0x0e, 0x10, 0x86, 0x02, 0x43, 0x0d, 0x06, 0x02, 0x6e, 0x0c, 0x07, 0x08, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0xf1, 0xf1, 0xff, 0xff, 0xb5, 0x02, 0x00, 0x00, 0x00, 0x41, 0x0e, 0x10, 0x86, 0x02, 0x43, 0x0d, 0x06, 0x03, 0xb0, 0x02, 0x0c, 0x07, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x00, 0x47, 0x43, 0x43, 0x3a, 0x20, 0x28, 0x47, 0x4e, 0x55, 0x29, 0x20, 0x31, 0x33, 0x2e, 0x32, 0x2e, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0xf1, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x04, 0x00, 0xf1, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x11, 0x00, 0x04, 0x00, 0xa0, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0xc1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x27, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0x1b, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2d, 0x00, 0x00, 0x00, 0x12, 0x00, 0x01, 0x00, 0x71, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb5, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 0x00, 0x00, 0x00, 0x12, 0x00, 0x01, 0x00, 0xfe, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0x7b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0x8f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4a, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0xd5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0xa3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0xf3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5b, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0xad, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0x71, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6a, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0xcb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x71, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0x5d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x12, 0x00, 0x01, 0x00, 0x39, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0x11, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x85, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0x25, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8c, 0x00, 0x00, 0x00, 0x12, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x91, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0x2f, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x96, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6c, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0xdf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9a, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9f, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0xb7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa4, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0xe9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0x99, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x75, 0x73, 0x65, 0x72, 0x2e, 0x63, 0x00, 0x75, 0x70, 0x72, 0x69, 0x6e, 0x74, 0x66, 0x2e, 0x63, 0x00, 0x75, 0x64, 0x69, 0x67, 0x69, 0x74, 0x73, 0x00, 0x6d, 0x6b, 0x6e, 0x6f, 0x64, 0x00, 0x67, 0x65, 0x74, 0x70, 0x69, 0x64, 0x00, 0x73, 0x6c, 0x65, 0x65, 0x70, 0x00, 0x75, 0x70, 0x72, 0x69, 0x6e, 0x74, 0x66, 0x00, 0x75, 0x70, 0x72, 0x69, 0x6e, 0x74, 0x70, 0x74, 0x72, 0x00, 0x70, 0x69, 0x70, 0x65, 0x00, 0x77, 0x72, 0x69, 0x74, 0x65, 0x00, 0x66, 0x73, 0x74, 0x61, 0x74, 0x00, 0x6b, 0x69, 0x6c, 0x6c, 0x00, 0x63, 0x68, 0x64, 0x69, 0x72, 0x00, 0x65, 0x78, 0x65, 0x63, 0x00, 0x77, 0x61, 0x69, 0x74, 0x00, 0x72, 0x65, 0x61, 0x64, 0x00, 0x75, 0x6e, 0x6c, 0x69, 0x6e, 0x6b, 0x00, 0x66, 0x6f, 0x72, 0x6b, 0x00, 0x75, 0x70, 0x72, 0x69, 0x6e, 0x74, 0x69, 0x6e, 0x74, 0x00, 0x73, 0x62, 0x72, 0x6b, 0x00, 0x75, 0x70, 0x74, 0x69, 0x6d, 0x65, 0x00, 0x6d, 0x61, 0x69, 0x6e, 0x00, 0x70, 0x75, 0x74, 0x63, 0x00, 0x64, 0x75, 0x70, 0x00, 0x65, 0x78, 0x69, 0x74, 0x00, 0x6f, 0x70, 0x65, 0x6e, 0x00, 0x6d, 0x6b, 0x64, 0x69, 0x72, 0x00, 0x63, 0x6c, 0x6f, 0x73, 0x65, 0x00, 0x00, 0x2e, 0x73, 0x79, 0x6d, 0x74, 0x61, 0x62, 0x00, 0x2e, 0x73, 0x74, 0x72, 0x74, 0x61, 0x62, 0x00, 0x2e, 0x73, 0x68, 0x73, 0x74, 0x72, 0x74, 0x61, 0x62, 0x00, 0x2e, 0x74, 0x65, 0x78, 0x74, 0x00, 0x2e, 0x72, 0x6f, 0x64, 0x61, 0x74, 0x61, 0x00, 0x2e, 0x65, 0x68, 0x5f, 0x66, 0x72, 0x61, 0x6d, 0x65, 0x00, 0x2e, 0x64, 0x61, 0x74, 0x61, 0x00, 0x2e, 0x62, 0x73, 0x73, 0x00, 0x2e, 0x63, 0x6f, 0x6d, 0x6d, 0x65, 0x6e, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x26, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x26, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa6, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xda, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x39, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb1, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc8, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
}; //

void procinit() {
  uint64 cr3_v;
  asm volatile("mov %%cr3, %0" : "=r" (cr3_v));
  cr3_v = (cr3_v >> 4) << 4;
  for (int i = 0; i < NPROC; i ++) {
    task_struct[i].state = PROC_UNUSED;
    task_struct[i].knlStk = 0x8000000 - i * PGSIZE;
    task_struct[i].id = i;
    task_struct[i].kpgtbl = cr3_v;
  }
  task_struct[0].id = 0;
  task_struct[0].pgtbl = task_struct[0].kpgtbl;
  // we need to consider the very first user-proc.
  // 0. allocate a pgtbl for our first proc
  uint64 *pgtbl = allocPgtbl();
  task_struct[1].pgtbl = pgtbl;
  // 1. prepare a user-stack, 2 PAGES SIZE.
  uint64 stk = (uint64)kalloc();
  mappages(pgtbl, 0x7000000 - PGSIZE, stk);
  stk = (uint64)kalloc();
  mappages(pgtbl, 0x7000000 - PGSIZE - PGSIZE, stk);
  // 2. iretq values
  task_struct[1].ctx.stk[0] = 0; // rip start at 0x0000000
  task_struct[1].ctx.stk[1] = 0x2b; // CS
  task_struct[1].ctx.stk[2] = 0x202; // CR0
  task_struct[1].ctx.stk[3] = 0x7000000; // this page, is not mapped. we map it at a high space of our proc.
  task_struct[1].ctx.stk[4] = 0x23; // SS;
  task_struct[1].state = PROC_RUNNABLE;
  // 3. copy codes inside
  for (uint64 base = 0; base < sizeof (initcode); base += PGSIZE) {
    uint64 *page = (uint64 *)kalloc();
    memcpy(page, initcode + base, PGSIZE);
    mappages(pgtbl, base, page);
  }
  // それでわ
}

int allocProcStruct() {
  for (int i = 1; i < NPROC; i ++) {
    if (task_struct[i].state == PROC_UNUSED) {
      task_struct[i].state = PROC_RUNNABLE;
      return i;
    }
  }
  return -1;
}

uint64 sys_fork() {
  // -1 indicates fork failure.
  // 0 indicate it is a son
  // > 0 indicate its son pid (father)
  int p = allocProcStruct();
  if (p == -1) // no more task_struct.
    return -1;
  uint64 *pgtbl = allocPgtbl(); // allocate a new pgtbl, kernel already done.
  for (uint64 pa = 0; pa < 0x7000000; pa += PGSIZE) {
    uint64 adr = fwalk(task_struct[task_struct[0].id].pgtbl, pa);
    if (adr != 0x8000000) { // walk just return a valid pgtbl addr, if valid. 0x8000000 indicates that is not valid.
      // fork is called by a syscall, current id is saved in task_struct[0].
      uint64 *npage = (uint64 *)kalloc();
      mappages(pgtbl, pa, npage);
      // you should copy the stk. ret addr is null because stk is missing.
      memcpy(npage, adr, PGSIZE);
    }
  }
  // todo copy everything under 0x7000000.
  task_struct[p].pgtbl = pgtbl; // you only set up the kernel, but the pgtbl
  // now we have the same pgtbl.
  // we copy the current ctx.
  task_struct[p].ctx = task_struct[0].ctx; // now we have the exact ctx. im not sure if this is able to assign?
  task_struct[task_struct[0].id].ctx.rax = p; // father return its id.
  task_struct[p].ctx.rax = 0; // son, fork return 0!
  task_struct[p].state = PROC_RUNNABLE;
  return 0;
}