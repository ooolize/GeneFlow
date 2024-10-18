/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-09
 * @LastEditors: lize
 */
#pragma once
#include <sched.h>
#include <unistd.h>

#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
namespace lz {
using size_t = std::size_t;

// if not instruction is executed in order
// average cost 20 cycle(5ns)
inline uint64_t rdtsc() {
  uint32_t lo, hi;
  __asm__ volatile("rdtsc" : "=a"(lo), "=d"(hi));
  return (uint64_t)hi << 32 | lo;
}

// if use cpuid ensure the instruction is executed in order
// in virtual machine, maybe cost more time
// average cost 2000 cycle(500ns)
inline uint64_t cpuid_rdtsc() {
  uint32_t lo, hi;
  // Use CPUID to serialize instruction execution
  __asm__ volatile("cpuid" : : : "%rax", "%rbx", "%rcx", "%rdx");
  // Execute RDTSC instruction
  __asm__ volatile("rdtsc" : "=a"(lo), "=d"(hi));
  return (uint64_t)hi << 32 | lo;
}

// if use mfence ensure the instruction is executed in order
// average cost 88 cycle(22ns)
inline uint64_t mfence_rdtsc() {
  uint32_t lo, hi;
  __asm__ volatile("mfence;rdtsc" : "=a"(lo), "=d"(hi));
  return (uint64_t)hi << 32 | lo;
}

// if use rdtscp ensure the instruction is executed in order
// average cost 60 cycle(15ns)
inline uint64_t rdtscp() {
  uint32_t lo, hi;
  __asm__ volatile("rdtscp" : "=a"(lo), "=d"(hi));
  return (uint64_t)hi << 32 | lo;
}

inline std::size_t rdtsc2nanoTime(std::size_t rdtsc, float frequencyGHz) {
  return rdtsc / frequencyGHz;
}

inline std::size_t nanoTime2rdtsc(std::size_t nanoTime, float frequencyGHz) {
  return nanoTime * frequencyGHz;
}
inline std::size_t spendTimeNs(std::size_t start,
                               std::size_t end,
                               float frequencyGHz) {
  return (end - start) / frequencyGHz;
}

// get CPU frequency in GHz
// average cost 127562 cycle(31.89us)
inline float getFrequencyGHz() {
  std::ifstream cpuinfo("/proc/cpuinfo");
  std::string line;
  std::string target = "cpu MHz";

  if (cpuinfo.is_open()) {
    while (std::getline(cpuinfo, line)) {
      if (line.find(target) != std::string::npos) {
        // 使用 remove-erase 习惯用法移除所有的制表符
        line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
        // 找到包含 "cpu MHz" 的行
        std::istringstream iss(line);
        std::string label;
        double mhz;
        // 读取 "cpu MHz" 和数值
        iss >> label >> label >> mhz;

        return mhz / 1000;
      }
    }
    cpuinfo.close();
  } else {
    std::cerr << "无法打开 /proc/cpuinfo 文件" << std::endl;
  }
  return 0;
  // return 3.7;
}

// get current time stamp in ns
// average cost 70 cycle(17.5ns)
inline std::size_t getTimeStampNs() {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(
           std::chrono::system_clock::now().time_since_epoch())
    .count();
}

// transform time string to nano seconds
// format "12:34:56 123456"
inline uint64_t timeToNanoseconds(const std::string& timeStr) {
  // 解析时间字符串
  int hours, minutes, seconds, microseconds;
  char colon;
  std::stringstream ss(timeStr);
  ss >> hours >> colon >> minutes >> colon >> seconds >> microseconds;

  // 创建一个表示当天时间的时间点
  using namespace std::chrono;
  auto timeSinceMidnight =
    hours * 1h + minutes * 1min + seconds * 1s + microseconds * 1us;

  // 转换为纳秒
  uint64_t totalNanoseconds =
    duration_cast<nanoseconds>(timeSinceMidnight).count();
  return totalNanoseconds;
}

}  // namespace lz