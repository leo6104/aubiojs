#include <emscripten/bind.h>
#include "aubio.h"

using namespace emscripten;

class Notes {
public:
  Notes(uint_t buf_size, uint_t hop_size, uint_t sample_rate) {
    buffer = new_fvec(buf_size);
    aubio_notes = new_aubio_notes("default", buf_size, hop_size, sample_rate);
  }

  ~Notes() {
    del_aubio_notes(aubio_notes);
    del_fvec(buffer);
    del_fvec(output);
  }

  int Silence(val input) {
    return aubio_notes_set_silence(aubio_notes, input.as<float>());
  }

  float Do(val input) {
    for (int i = 0; i < buffer->length; i += 1) {
      buffer->data[i] = input[i].as<float>();
    }
    aubio_notes_do(aubio_notes, buffer, output);
    return output->data[0];
  }

private:
  aubio_notes_t *aubio_notes;
  fvec_t *buffer;
  fvec_t *output = new_fvec(1);
};

EMSCRIPTEN_BINDINGS(Notes) {
  class_<Notes>("Notes")
    .constructor<uint_t, uint_t, uint_t>()
    .function("do", &Notes::Do)
    .function("silence", &Notes::Silence);
}