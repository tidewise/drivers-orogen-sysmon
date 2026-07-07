#ifndef SYSMON_TYPES_HPP
#define SYSMON_TYPES_HPP

#include <base/Time.hpp>
#include <cstdint>
#include <string>
#include <vector>

namespace sysmon {

    struct DiskInfo {
        unsigned int major_number = 0;
        unsigned int minor_number = 0;
        std::string device_name;
        uint64_t reads_completed = 0;
        uint64_t reads_merged = 0;
        uint64_t sectors_read = 0;
        base::Time time_reading = base::Time();
        uint64_t writes_completed = 0;
        uint64_t writes_merged = 0;
        uint64_t sectors_written = 0;
        base::Time time_writing = base::Time();
        uint64_t io_in_progress = 0;
        base::Time time_doing_io = base::Time();
        base::Time weighted_time_doing_io = base::Time();
        uint64_t discards_completed = 0;
        uint64_t discards_merged = 0;
        uint64_t sectors_discarded = 0;
        base::Time time_discarding = base::Time();
        uint64_t flush_requests_completed = 0;
        base::Time time_flushing = base::Time();
    };

    struct SystemStats {
        base::Time time;
        std::vector<DiskInfo> disks;
    };

}

#endif
