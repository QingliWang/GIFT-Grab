#ifndef __OPENCV_DATA_SOURCE_H__
#define __OPENCV_DATA_SOURCE_H__

#include "ivideosource.h"
#include "macros.h"
#include "except.h"

// OpenCV includes
#include "opencv2/opencv.hpp"

//!
//! \brief OpenCV video source
//!
//! Can be used for reading files as well as streaming
//! from devices
//!
class VideoSourceOpenCV: public IVideoSource
{
public:
    //!
    //! \brief Read in video file specified by \c path
    //! \param path
    //!
    VideoSourceOpenCV(const char *path);

    //!
    //! \brief Connects to device with specified \c deviceId,
    //! e.g. for streaming
    //! \param deviceId for instance \c 0 for \c /dev/video0
    //! (assuming your device is mapped to \c video0 on Linux)
    //! \throw gg::DeviceNotFound if cannot connect to device
    //! with \c deviceId
    //!
    VideoSourceOpenCV(const int deviceId);

    bool get_frame_dimensions(int & width, int & height);

    bool get_frame(VideoFrame_BGRA &frame);

    double get_frame_rate();

    void set_sub_frame(int x, int y, int width, int height)
    {
        _sub_frame = cv::Rect(x, y, width, height);
        _get_sub_frame = true;
        _buffer_sub_bgra = cv::Mat::zeros(height, width, CV_8UC4);
    }

private:
    //!
    //! \brief Frame rate (fps), needed as a
    //! variable esp. in case it needs to be
    //! estimated -- see GIFT-Surg/GiftGrab#15
    //!
    float                   _frame_rate;

    //!
    //! \brief OpenCV capture object
    //!
    cv::VideoCapture        _cap;

    //!
    //! \brief Buffer for raw frames
    //!
    cv::Mat                 _buffer;

    //!
    //! \brief Buffer for BGRA conversion
    //!
    cv::Mat                 _buffer_bgra;

    //!
    //! \brief Buffer for frame ROI, in case
    //! specified
    //! \sa _sub_frame
    //!
    cv::Mat                 _buffer_sub_bgra;

    //!
    //! \brief Frame ROI
    //!
    cv::Rect                _sub_frame;

    DISALLOW_COPY_AND_ASSIGNMENT(VideoSourceOpenCV);
};

#endif
