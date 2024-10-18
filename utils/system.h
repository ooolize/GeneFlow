/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-28
 * @LastEditors: lize
 */

#pragma once
#include <pthread.h>
#include <sched.h>
#include <sys/syscall.h>
#include <unistd.h>
namespace lz {
namespace system {

inline pid_t gettid() {
  return static_cast<pid_t>(syscall(SYS_gettid));
}

inline void setCPUAffinity(int cpu) {
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);      // 清空 CPU 集合
  CPU_SET(cpu, &cpuset);  // NOLINT

  // 获取线程 ID
  pthread_t current_thread = pthread_self();

  // 设置线程的 CPU 亲和性
  if (pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset) != 0) {
    std::cout << "Error setting CPU affinity" << std::endl;
  }
}
}  // namespace system
}  // namespace lz