#pragma once

#include "ivideotarget.h"

extern "C" {
#include "libavutil/opt.h"
#include "libavutil/imgutils.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h" // for BGRA to YUV420p conversion
}

namespace gg
{

class VideoTargetFFmpeg : public IVideoTarget
{
protected:
    //!
    //! \brief
    //!
    std::string _filepath;

    //!
    //! \brief
    //!
    AVFormatContext *_format_context;

    //!
    //! \brief
    //!
    AVStream * _stream;

    //!
    //! \brief
    //!
    AVCodecID _codec_id;

    //!
    //! \brief
    //!
    AVCodec * _codec;

    //!
    //! \brief
    //!
    AVFrame * _frame;

    //!
    //! \brief
    //!
    int64_t _frame_index;

    //!
    //! \brief
    //!
    int _framerate;

    //!
    //! \brief Packet for use when encoding and
    //! subsequently writing frames
    //!
    AVPacket _packet;

    //!
    //! \brief This is for converting pixel format
    //! (e.g. from BGRA to YUV420p)
    //!
    SwsContext * _sws_context;

public:
    //!
    //! \brief Constructor defining what \c codec to
    //! use for encoding
    //! \param codec
    //!
    VideoTargetFFmpeg(const std::string codec = "H265");

public:
    void init(const std::string filepath, const float framerate);

    void append(const VideoFrame_BGRA & frame);

    void finalise();

protected:
    //!
    //! \brief Convenience function for code reuse when
    //! writing delayed frames
    //! \param packet
    //! \param frame leave \c NULL if writing delayed
    //! frames
    //! \param got_output used as \c get_packet_ptr
    //! parameter of \c avcodec_encode_video2() function
    //! of FFmpeg
    //! \throw VideoTargetError propagates any exception
    //! to caller
    //! \sa finalise
    //!
    void encode_and_write(AVPacket * packet,
                          AVFrame * frame,
                          int & got_output);
};

}
