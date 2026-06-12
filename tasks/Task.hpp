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
    };
}

#endif
