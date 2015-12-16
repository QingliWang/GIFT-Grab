#include "opencv_video_source.h"
#include <string>
#include <iostream>

VideoSourceOpenCV::VideoSourceOpenCV(const char * path)
    :IVideoSource()
{
    _cap.open(path);
    if (!_cap.isOpened()) {
        std::string error = std::string("Could not open the video source: ")
                            + path;
        throw std::runtime_error(error);
    }
}

VideoSourceOpenCV::VideoSourceOpenCV(int deviceId)
    : IVideoSource()
{
    _cap.open(deviceId);
    if (!_cap.isOpened()) {
        std::string error("Device with id ");
        error.append(std::to_string(deviceId));
        error.append(" could not be opened");
        throw gg::DeviceNotFound(error);
    }
}

double VideoSourceOpenCV::get_frame_rate()
{
    if (!_cap.isOpened()) return 0;

    _frame_rate = _cap.get(CV_CAP_PROP_FPS);

    if (_frame_rate <= 0) // i.e. not a file, see:
    // http://www.learnopencv.com/how-to-find-frame-rate-or-frames-per-second-fps-in-opencv-python-cpp/
    {
        // Number of frames to capture
        int num_frames = 120;

        // Start and end times
        time_t start, end;

        VideoFrame_BGRA buffer(true);

        // Start time
        time(&start);

        // Grab a few frames
        for(int i = 0; i < num_frames; i++)
        {
            get_frame(buffer);
        }

        // End Time
        time(&end);

        // Time elapsed
        float seconds = difftime(end, start);
        std::cout << "Time taken : " << seconds << " seconds" << std::endl;

        // Calculate frames per second
        _frame_rate  = num_frames / seconds;
        std::cout << "Estimated frames per second : " << _frame_rate << std::endl;
    }

    return _frame_rate;
}

bool VideoSourceOpenCV::get_frame_dimensions(int &width, int & height)
{
    width = height = 0;
    if (!_cap.isOpened()) return false;

    width = (int)_cap.get(CV_CAP_PROP_FRAME_WIDTH);
    height = (int)_cap.get(CV_CAP_PROP_FRAME_HEIGHT);

    return (width != 0 && height != 0);
}

bool VideoSourceOpenCV::get_frame(VideoFrame_BGRA & frame)
{
    if (!_cap.isOpened()) return false;

    bool has_read = _cap.read(_buffer);

    if (has_read) {
        // Initialize the BGRA buffer, if not only done so.
        if (_buffer_bgra.empty() ||
            _buffer_bgra.rows != _buffer.rows ||
            _buffer_bgra.cols != _buffer.cols) {
            _buffer_bgra = cv::Mat::zeros(_buffer.rows, _buffer.cols, CV_8UC4);
        }
        // Convert to BGRA
        cv::cvtColor(_buffer, _buffer_bgra, CV_BGR2BGRA);
        if (_get_sub_frame) {
            _buffer_bgra(_sub_frame).copyTo(_buffer_sub_bgra);
            frame.init_from_opencv_mat(_buffer_sub_bgra);
        }

        else {
            frame.init_from_opencv_mat(_buffer_bgra);
        }
    }
    return has_read;
}
