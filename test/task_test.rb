# frozen_string_literal: true

using_task_library "sysmon"

describe OroGen.sysmon.Task do
    run_live

    attr_reader :task

    before do
        @mock_diskstats = Tempfile.new("diskstats")
        @mock_diskstats.write(<<~DISKSTATS)
            259       0 nvme0n1 123 456 789 100 223 356 489 200 0 150 150 11 22 33 300 44 400
              8       0 sda 10 20 30 40 50 60 70 80 0 90 90 1 2 3 50 4 60
        DISKSTATS
        @mock_diskstats.flush

        @task = syskit_deploy(
            OroGen.sysmon.Task.deployed_as("sysmon")
        )
        syskit_start_execution_agents(task)
    end

    it "publishes disk statistics for all devices when device list is empty" do
        task.properties.diskstats_path = @mock_diskstats.path
        task.properties.devices = []

        syskit_configure_and_start(task)

        sample = expect_execution.to { have_one_new_sample(task.stats_port) }

        assert_equal 2, sample.disks.size

        nvme = sample.disks.find { |d| d.device_name == "nvme0n1" }
        assert_equal 259, nvme.major_number
        assert_equal 0, nvme.minor_number
        assert_equal 123, nvme.reads_completed
        assert_equal 456, nvme.reads_merged
        assert_equal 789, nvme.sectors_read
        assert_equal 0.1, nvme.time_reading.to_f
        assert_equal 223, nvme.writes_completed
        assert_equal 356, nvme.writes_merged
        assert_equal 489, nvme.sectors_written
        assert_equal 0.2, nvme.time_writing.to_f
        assert_equal 0, nvme.io_in_progress
        assert_equal 0.15, nvme.time_doing_io.to_f
        assert_equal 0.15, nvme.weighted_time_doing_io.to_f
        assert_equal 11, nvme.discards_completed
        assert_equal 22, nvme.discards_merged
        assert_equal 33, nvme.sectors_discarded
        assert_equal 0.3, nvme.time_discarding.to_f
        assert_equal 44, nvme.flush_requests_completed
        assert_equal 0.4, nvme.time_flushing.to_f

        sda = sample.disks.find { |d| d.device_name == "sda" }
        assert_equal 8, sda.major_number
        assert_equal 0, sda.minor_number
        assert_equal 10, sda.reads_completed
        assert_equal 20, sda.reads_merged
        assert_equal 30, sda.sectors_read
        assert_equal 0.04, sda.time_reading.to_f
        assert_equal 50, sda.writes_completed
        assert_equal 60, sda.writes_merged
        assert_equal 70, sda.sectors_written
        assert_equal 0.08, sda.time_writing.to_f
        assert_equal 0, sda.io_in_progress
        assert_equal 0.09, sda.time_doing_io.to_f
        assert_equal 0.09, sda.weighted_time_doing_io.to_f
        assert_equal 1, sda.discards_completed
        assert_equal 2, sda.discards_merged
        assert_equal 3, sda.sectors_discarded
        assert_equal 0.05, sda.time_discarding.to_f
        assert_equal 4, sda.flush_requests_completed
        assert_equal 0.06, sda.time_flushing.to_f
    end

    it "filters statistics for specific devices" do
        task.properties.diskstats_path = @mock_diskstats.path
        task.properties.devices = ["sda"]

        syskit_configure(task)
        expect_execution { task.start! }.to { emit task.start_event }

        sample = expect_execution.to { have_one_new_sample(task.stats_port) }

        assert_equal 1, sample.disks.size

        sda = sample.disks.first
        assert_equal "sda", sda.device_name
    end

    it "supports older kernels with fewer fields" do
        mock_old_diskstats = Tempfile.new("old_diskstats")
        mock_old_diskstats.write(<<~DISKSTATS)
            8       0 sda 10 20 30 40 50 60 70 80 0 90 90
        DISKSTATS
        mock_old_diskstats.flush

        task.properties.diskstats_path = mock_old_diskstats.path
        task.properties.devices = []

        syskit_configure_and_start(task)

        sample = expect_execution.to { have_one_new_sample(task.stats_port) }

        assert_equal 1, sample.disks.size

        sda = sample.disks.first
        assert_equal "sda", sda.device_name
        assert_equal 8, sda.major_number
        assert_equal 0, sda.minor_number
        assert_equal 10, sda.reads_completed
        assert_equal 20, sda.reads_merged
        assert_equal 30, sda.sectors_read
        assert_equal 0.04, sda.time_reading.to_f
        assert_equal 50, sda.writes_completed
        assert_equal 60, sda.writes_merged
        assert_equal 70, sda.sectors_written
        assert_equal 0.08, sda.time_writing.to_f
        assert_equal 0, sda.io_in_progress
        assert_equal 0.09, sda.time_doing_io.to_f
        assert_equal 0.09, sda.weighted_time_doing_io.to_f

        # Missing fields should default to 0
        assert_equal 0, sda.discards_completed
        assert_equal 0, sda.discards_merged
        assert_equal 0, sda.sectors_discarded
        assert_equal 0.0, sda.time_discarding.to_f
        assert_equal 0, sda.flush_requests_completed
        assert_equal 0.0, sda.time_flushing.to_f
    end

    it "emits INVALID_DISKSTATS_PATH error state if file cannot be opened" do
        task.properties.diskstats_path = @mock_diskstats.path
        task.properties.devices = []

        syskit_configure_and_start(task)
        expect_execution.to { have_one_new_sample(task.stats_port) }

        File.delete(@mock_diskstats.path)

        expect_execution.to { emit task.invalid_diskstats_path_event }
    end

    it "fails to configure if the file cannot be opened" do
        task.properties.diskstats_path = "/path/to/non/existent/diskstats/file"

        assert_raises(Roby::EmissionFailed) do
            syskit_configure(task)
        end
    end
end
