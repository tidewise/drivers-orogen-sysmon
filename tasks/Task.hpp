/* Generated from orogen/lib/orogen/templates/tasks/Task.hpp */

#ifndef SYSMON_TASK_TASK_HPP
#define SYSMON_TASK_TASK_HPP

#include "sysmon/TaskBase.hpp"

namespace sysmon {

    class Task : public TaskBase {
        friend class TaskBase;

    protected:
        std::vector<std::string> m_devices_to_monitor;
        std::string m_diskstats_path;

    public:
        Task(std::string const& name = "sysmon::Task");

        ~Task();

        bool configureHook();

        bool startHook();

        void updateHook();

        void errorHook();

        void stopHook();

        void cleanupHook();

    private:
        /**
         * @brief Reads disk statistics from the configured diskstats file.
         *
         * @return SystemStats containing the parsed disk information and the current
         * time.
         */
        sysmon::SystemStats readStats();

        /**
         * @brief Parses a single line from the diskstats file.
         *
         * @param line The line from the diskstats file to parse.
         * @param disk_info The DiskInfo structure to populate with the parsed data.
         * @return true if the line was successfully parsed and the device is being
         * monitored; false otherwise.
         */
        bool parseDiskstatsLine(std::string const& line, sysmon::DiskInfo& disk_info);
    };
}

#endif
