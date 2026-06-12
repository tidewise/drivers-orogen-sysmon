#ifndef SYSMON_TYPES_HPP
#define SYSMON_TYPES_HPP

#include <base/Time.hpp>
#include <string>
#include <vector>

namespace sysmon {

    struct DiskInfo {
        unsigned int major_number = 0;
        unsigned int minor_number = 0;
        std::string device_name;
        unsigned long long reads_completed = 0;
        unsigned long long reads_merged = 0;
        unsigned long long sectors_read = 0;
        base::Time time_reading = base::Time();
        unsigned long long writes_completed = 0;
        unsigned long long writes_merged = 0;
        unsigned long long sectors_written = 0;
        base::Time time_writing = base::Time();
        unsigned long long io_in_progress = 0;
        base::Time time_doing_io = base::Time();
        base::Time weighted_time_doing_io = base::Time();
        unsigned long long discards_completed = 0;
        unsigned long long discards_merged = 0;
        unsigned long long sectors_discarded = 0;
        base::Time time_discarding = base::Time();
        unsigned long long flush_requests_completed = 0;
        base::Time time_flushing = base::Time();
    };

    struct SystemStats {
        base::Time time;
        std::vector<DiskInfo> disks;
    };

}

#endif
