#include "system.h"

#define IDE_DATA        0x1F0
#define IDE_ERROR       0x1F1
#define IDE_SECTOR_COUNT 0x1F2
#define IDE_SECTOR_NUMBER 0x1F3
#define IDE_CYLINDER_LOW 0x1F4
#define IDE_CYLINDER_HIGH 0x1F5
#define IDE_DRIVE_SELECT 0x1F6
#define IDE_COMMAND     0x1F7
#define IDE_STATUS      0x1F7

#define IDE_CMD_READ    0x20
#define IDE_CMD_WRITE   0x30
#define IDE_DRDY        0x40
#define IDE_BSY         0x80

void read_sectors(uint8 *buffer, uint32 sector, uint8 count, int is_master) {
  uint8 drive = is_master ? 0xE0 : 0xF0;

  while (count > 0) {
    // Select drive (Master or Slave)
    asm volatile ("outb %%al, %%dx" : : "a" (drive | ((sector >> 24) & 0x0F)), "d" (IDE_DRIVE_SELECT));

    // Set sector count
    asm volatile ("outb %%al, %%dx" : : "a" (1), "d" (IDE_SECTOR_COUNT));

    // Set sector number (LBA bits 0-7)
    asm volatile ("outb %%al, %%dx" : : "a" (sector & 0xFF), "d" (IDE_SECTOR_NUMBER));

    // Set cylinder low byte (LBA bits 8-15)
    asm volatile ("outb %%al, %%dx" : : "a" ((sector >> 8) & 0xFF), "d" (IDE_CYLINDER_LOW));

    // Set cylinder high byte (LBA bits 16-23)
    asm volatile ("outb %%al, %%dx" : : "a" ((sector >> 16) & 0xFF), "d" (IDE_CYLINDER_HIGH));

    // Send read command
    asm volatile ("outb %%al, %%dx" : : "a" (IDE_CMD_READ), "d" (IDE_COMMAND));

    // Wait for the drive to be ready
    uint8 status;
    do {
      asm volatile ("inb %%dx, %%al" : "=a" (status) : "d" (IDE_STATUS));
    } while (status & IDE_BSY);

    // Check for readiness
    do {
      asm volatile ("inb %%dx, %%al" : "=a" (status) : "d" (IDE_STATUS));
    } while (!(status & IDE_DRDY));

    // Read the data into the buffer
    for (int i = 0; i < 256; i++) { // 256 * 2 bytes = 512 bytes
      uint16 data;
      asm volatile ("inw %%dx, %%ax" : "=a" (data) : "d" (IDE_DATA));
      ((uint16 *)buffer)[i] = data;
    }

    buffer += 512;
    sector ++;
    count --;
  }
}

void write_sectors(uint8 *buffer, uint32 sector, uint8 count, int is_master) {
  uint8 drive = is_master ? 0xE0 : 0xF0;

  while (count > 0) {
    // Select drive (Master or Slave)
    asm volatile ("outb %%al, %%dx" : : "a" (drive | ((sector >> 24) & 0x0F)), "d" (IDE_DRIVE_SELECT));

    // Set sector count
    asm volatile ("outb %%al, %%dx" : : "a" (1), "d" (IDE_SECTOR_COUNT));

    // Set sector number (LBA bits 0-7)
    asm volatile ("outb %%al, %%dx" : : "a" (sector & 0xFF), "d" (IDE_SECTOR_NUMBER));

    // Set cylinder low byte (LBA bits 8-15)
    asm volatile ("outb %%al, %%dx" : : "a" ((sector >> 8) & 0xFF), "d" (IDE_CYLINDER_LOW));

    // Set cylinder high byte (LBA bits 16-23)
    asm volatile ("outb %%al, %%dx" : : "a" ((sector >> 16) & 0xFF), "d" (IDE_CYLINDER_HIGH));

    // Send write command
    asm volatile ("outb %%al, %%dx" : : "a" (IDE_CMD_WRITE), "d" (IDE_COMMAND));


    // Write the data from the buffer
    for (int i = 0; i < 256; i++) { // 256 * 2 bytes = 512 bytes
      uint16 data = ((uint16 *)buffer)[i];
      asm volatile ("outw %%ax, %%dx" : : "a" (data), "d" (IDE_DATA));
    }

    // Wait for the drive to be ready
    uint8 status;
    do {
      asm volatile ("inb %%dx, %%al" : "=a" (status) : "d" (IDE_STATUS));
    } while (status & IDE_BSY);

    // Check for readiness
    do {
      asm volatile ("inb %%dx, %%al" : "=a" (status) : "d" (IDE_STATUS));
    } while (!(status & IDE_DRDY));

    buffer += 512;
    sector ++;
    count --;
  }
}
