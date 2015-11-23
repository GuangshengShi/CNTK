#pragma once

#include <vector>
#include "reader_interface.h"

// Defines the encoding of a frame.
struct frame_description
{
    std::vector<size_t> frame_dimensions;
    size_t element_size;
};

// Defines identifier and length of a sequence.
struct sequence_description
{
    size_t id;
    size_t length;
};

// Defines a sequences, which consists of sequences description and a number
// of frames, which have the same encoding and are layed out in memory contiguously.
struct sequence
{
    sequence_description* description;
    frame_description* frame_description;
    char* data;
    size_t number_of_frames;
};

// Low-level input interface (for file, network, etc.).
// Memory buffers to fill data into are provided by the caller.
class block_reader
{
public:
    virtual void get(char* buffer, size_t offset, size_t size) = 0;
    virtual ~block_reader() = 0 {}
};

// Timeline specifies a vector of sequence IDs and lengths.
// This information is exposed by a source, e.g., to be used by the randomizer.
typedef std::vector<sequence_description> timeline;

// Timeline offsets specify file offset of sequences. These are used internally
// to sources in a reader-specific way.
typedef std::vector<size_t> timeline_offsets;

// A source composes timeline information and a number of sequence readers, providing
// random-access to the timeline as well as the composed sequence readers.
class source
{
public:
    virtual timeline& get_timeline() const = 0;
    virtual std::vector<input_description_ptr> get_inputs() const = 0;
    virtual std::map<size_t /*input*/, sequence> get_sequence_by_id(size_t id) = 0;
    virtual ~source() = 0 {};
};

// Defines context augmentation (to the left and to the right).
// Augmentation is implemented
struct augmentation_descriptor
{
    size_t context_left;
    size_t context_right;
};

class sequencer
{
public:
    virtual std::vector<input_description_ptr> get_inputs() const = 0;
    virtual std::map<size_t /*per input descriptor*/, sequence> get_next_sequence() = 0;
    virtual ~sequencer() = 0 {}
};


// A randomizer implements a sequence randomization for a source and
// additional parameters given at construction time.
// Note: chunk-level randomization can be implemented based on sequence lengths
// exposed through the source's timeline method.
class randomizer : sequencer
{
};

class image_cropper : sequencer
{
};
