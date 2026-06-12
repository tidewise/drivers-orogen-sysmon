/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Task.hpp"
#include <fstream>
#include <sstream>

using namespace sysmon;

Task::Task(std::string const& name)
    : TaskBase(name)
{
}

Task::~Task()
{
}

bool Task::configureHook()
{
    if (!TaskBase::configureHook())
        return false;

    m_devices_to_monitor = _devices.get();
    m_diskstats_path = _diskstats_path.get();
    return true;
}
bool Task::startHook()
{
    if (!TaskBase::startHook())
        return false;
    return true;
}

void Task::updateHook()
{
    TaskBase::updateHook();

    sysmon::SystemStats stats = readStats();
    if (stats.time.isNull()) {
        return;
    }

    _stats.write(stats);
}
sysmon::SystemStats Task::readStats()
{
    sysmon::SystemStats stats;

    std::ifstream file(m_diskstats_path.c_str());
    if (!file.is_open()) {
        exception(INVALID_DISKSTATS_PATH);
        return stats;
    }

    stats.time = base::Time::now();

    std::string line;
    while (std::getline(file, line)) {
        sysmon::DiskInfo disk_info;
        if (parseDiskstatsLine(line, disk_info)) {
            stats.disks.push_back(disk_info);
        }
    }

    return stats;
}

bool Task::parseDiskstatsLine(std::string const& line, sysmon::DiskInfo& disk_info)
{
    std::istringstream iss(line);

    if (!(iss >> disk_info.major_number >> disk_info.minor_number >>
            disk_info.device_name)) {
        return false;
    }

    if (!m_devices_to_monitor.empty()) {
        if (std::find(m_devices_to_monitor.begin(),
                m_devices_to_monitor.end(),
                disk_info.device_name) == m_devices_to_monitor.end()) {
            return false;
        }
    }

    unsigned long long t_reading = 0, t_writing = 0, t_doing_io = 0, t_weighted_io = 0,
                       t_discarding = 0, t_flushing = 0;

    iss >> disk_info.reads_completed >> disk_info.reads_merged >>
        disk_info.sectors_read >> t_reading >> disk_info.writes_completed >>
        disk_info.writes_merged >> disk_info.sectors_written >> t_writing >>
        disk_info.io_in_progress >> t_doing_io >> t_weighted_io >>
        disk_info.discards_completed >> disk_info.discards_merged >>
        disk_info.sectors_discarded >> t_discarding >>
        disk_info.flush_requests_completed >> t_flushing;

    disk_info.time_reading = base::Time::fromMilliseconds(t_reading);
    disk_info.time_writing = base::Time::fromMilliseconds(t_writing);
    disk_info.time_doing_io = base::Time::fromMilliseconds(t_doing_io);
    disk_info.weighted_time_doing_io = base::Time::fromMilliseconds(t_weighted_io);
    disk_info.time_discarding = base::Time::fromMilliseconds(t_discarding);
    disk_info.time_flushing = base::Time::fromMilliseconds(t_flushing);

    return true;
}

void Task::errorHook()
{
    TaskBase::errorHook();
}
void Task::stopHook()
{
    TaskBase::stopHook();
}
void Task::cleanupHook()
{
    TaskBase::cleanupHook();
}
