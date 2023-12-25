#define _GNU_SOURCE
#include <arpa/inet.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/statvfs.h>
#include <time.h>
#include <unistd.h>

#ifndef BATTERY_DIRECTORY
#define BATTERY_DIRECTORY "/sys/class/power_supply/BAT0"
#endif
#define IS_ORDINAL(ordinal) !((date->tm_mday - ordinal) % 10)

/**
 * @brief It counts the total of digits of a number.
 * @param number The number to be analyzed.
 * @returns It returns the total of digits of the number.
 */
static int countDigits(int number);
/** @brief It writes the battery percentage module. */
static void writeBatteryPercentage(void);
/**
 * @brief It writes the calendar module.
 * @param date The current date.
 */
static void writeCalendar(struct tm *date);
/**
 * @brief It writes the clock module.
 * @param date The current date.
 */
static void writeClock(struct tm *date);
/**
 * @brief It writes the commands separator line.
 * @param size The terminal window size.
 */
static void writeCommandsLine(struct winsize *size);
/** @brief It writes the disk use percentage module. */
static void writeDiskUsePercentage(void);
/** @brief It writes the local IPV4 address module. */
static void writeLocalIPV4Address(void);
/**
 * @brief It writes the modules separator line.
 * @param size The terminal window size.
 */
static void writeModulesLine(struct winsize *size);

static int modulesLength_g = 41;

static int countDigits(int number) {
  int totalOfDigits;
  for (totalOfDigits = !number; number; number /= 10) {
    totalOfDigits++;
  }
  return totalOfDigits;
}

static void writeBatteryPercentage(void) {
  int capacityFile = open(BATTERY_DIRECTORY "/capacity", O_RDONLY);
  int statusFile = open(BATTERY_DIRECTORY "/status", O_RDONLY);
  char capacityBuffer[5];
  char statusBuffer[1];
  int percentage;
  if (statusFile < 0) {
    return;
  } else if (capacityFile > 0) {
    read(capacityFile, capacityBuffer, sizeof(capacityBuffer));
  }
  read(statusFile, statusBuffer, sizeof(statusBuffer));
  close(statusFile);
  close(capacityFile);
  percentage = atoi(capacityBuffer);
  printf("%s%s%%f%d%%%%  ", *statusBuffer == 'C' ? "%F{3}󱐋 " : "",
         percentage <= 5    ? "%F{1}  "
         : percentage <= 25 ? "%F{3}  "
         : percentage <= 50 ? "%F{2}  "
                            : "%F{2}  ",
         percentage);
  modulesLength_g += countDigits(percentage) + 6 + (*statusBuffer == 'C') * 2;
}

static void writeCalendar(struct tm *date) {
  char calendar[13];
  strftime(calendar, sizeof(calendar), "(%a) %b %d", date);
  printf("%%F{1}󰃭 %%f%s%s  ", calendar,
         IS_ORDINAL(1)   ? "st"
         : IS_ORDINAL(2) ? "nd"
         : IS_ORDINAL(3) ? "rd"
                         : "th");
}

static void writeClock(struct tm *date) {
  printf("%s%%f%02dh%02dm",
         date->tm_hour < 6    ? "%F{6}󰭎 "
         : date->tm_hour < 12 ? "%F{1}󰖨 "
         : date->tm_hour < 18 ? "%F{4} "
                              : "%F{3}󰽥 ",
         date->tm_hour, date->tm_min);
}

static void writeCommandsLine(struct winsize *size) {
  int column;
  for (column = 0; column < size->ws_col; column++) {
    printf(column % 2 ? "%%F{1}⊼" : "%%F{3}⊵");
  }
}

static void writeDiskUsePercentage(void) {
  fsblkcnt_t remainingBytes;
  fsblkcnt_t totalBytes;
  int percentage;
  struct statvfs status;
  statvfs("/", &status);
  totalBytes = status.f_frsize * status.f_blocks;
  remainingBytes = status.f_frsize * status.f_bavail;
  percentage = ((float)(totalBytes - remainingBytes) / totalBytes) * 100;
  printf("%%F{%d}󰋊 %%f%d%%%%  ",
         percentage < 70   ? 2
         : percentage < 80 ? 3
                           : 1,
         percentage);
  modulesLength_g += countDigits(percentage);
}

static void writeLocalIPV4Address(void) {
  char ip[16] = "127.0.0.1";
  struct ifaddrs *address;
  struct ifaddrs *temporaryAddress;
  getifaddrs(&address);
  for (temporaryAddress = address; temporaryAddress;
       temporaryAddress = temporaryAddress->ifa_next) {
    if (temporaryAddress->ifa_addr &&
        temporaryAddress->ifa_addr->sa_family & AF_INET &&
        temporaryAddress->ifa_flags & IFF_RUNNING &&
        !(temporaryAddress->ifa_flags & IFF_LOOPBACK)) {
      inet_ntop(AF_INET,
                &((struct sockaddr_in *)temporaryAddress->ifa_addr)->sin_addr,
                ip, sizeof(ip));
      break;
    }
  }
  freeifaddrs(address);
  printf("%%F{4} %%f%s  ", ip);
  modulesLength_g += strlen(ip);
}

static void writeModulesLine(struct winsize *size) {
  int column;
  for (column = 0; column < size->ws_col - modulesLength_g; column++) {
    printf(column % 2 ? "%%F{1}-" : "%%F{3}=");
  }
}

int main(void) {
  time_t epoch = time(NULL);
  struct tm date;
  struct winsize size;
  localtime_r(&epoch, &date);
  ioctl(STDERR_FILENO, TIOCGWINSZ, &size);
  writeCommandsLine(&size);
  printf("%%F{3}:«(");
  writeLocalIPV4Address();
  writeDiskUsePercentage();
  writeBatteryPercentage();
  writeCalendar(&date);
  writeClock(&date);
  printf("%%F{3})»:");
  writeModulesLine(&size);
  printf("%%F{3}%%(#.{%%F{1}#%%F{3}}.){%%(?.≗.%%F{1}⨲)%%F{3}}⤐  %%F{1}%%~ "
         "%%F{6}✗%%f  ");
  return 0;
}
