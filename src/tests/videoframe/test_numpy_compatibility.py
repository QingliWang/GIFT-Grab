from pytest import mark, yield_fixture, fail
from pygiftgrab import VideoFrame, ColourSpace
# TODO: what if no NumPy?
import numpy as np

# TODO: Include value tests

frame = None
cols = 1920
rows = 1080


@yield_fixture(autouse=True)
def peri_test(colour_space):
    global frame
    frame = VideoFrame(colour_space, cols, rows)
    assert frame is not None

    yield


@mark.numpy_compatibility
def test_data():
    global frame, cols, rows
    data_np = frame.data()
    assert data_np.dtype == np.int8
    assert frame.data_length() == data_np.size


@mark.numpy_compatibility
def test_access(colour_space):
    global frame
    global cols, rows
    data_np = frame.data()

    # general indices sampled at the boundaries only
    # intermediate indices should be accessible,
    # provided that these are accessible
    indices = [col * rows + row
               for col in [0, cols - 1]
               for row in [0, rows - 1]]

    # currently performing no value checks
    for index in indices:
        try:
            if colour_space == ColourSpace.BGRA:
                data_np[index * 4 + 0]  # Blue channel
                data_np[index * 4 + 1]  # Green channel
                data_np[index * 4 + 2]  # Red channel
                data_np[index * 4 + 3]  # Alpha channel
            elif colour_space == ColourSpace.I420:
                # see https://wiki.videolan.org/YUV/#I420
                data_np[index]         # Y plane
                data_np[index * 1.25]  # U plane
                data_np[index * 1.5]   # V plane
        except IndexError as e:
            fail(e.message)
