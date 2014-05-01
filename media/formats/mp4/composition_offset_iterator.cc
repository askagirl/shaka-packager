// Copyright 2014 Google Inc. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#include "media/formats/mp4/composition_offset_iterator.h"

#include "base/logging.h"

namespace media {
namespace mp4 {

CompositionOffsetIterator::CompositionOffsetIterator(
    const CompositionTimeToSample& composition_time_to_sample)
    : sample_index_(0),
      composition_offset_table_(composition_time_to_sample.composition_offset),
      iterator_(composition_offset_table_.begin()) {}
CompositionOffsetIterator::~CompositionOffsetIterator() {}

bool CompositionOffsetIterator::AdvanceSample() {
  ++sample_index_;
  if (sample_index_ >= iterator_->sample_count) {
    ++iterator_;
    if (iterator_ == composition_offset_table_.end())
      return false;
    sample_index_ = 0;
  }
  return true;
}

bool CompositionOffsetIterator::IsValid() const {
  return iterator_ != composition_offset_table_.end() &&
         sample_index_ < iterator_->sample_count;
}

int32 CompositionOffsetIterator::SampleOffset(uint32 sample) const {
  uint32 current_sample = 0;
  std::vector<CompositionOffset>::const_iterator it =
      composition_offset_table_.begin();
  for (; it != composition_offset_table_.end(); ++it) {
    current_sample += it->sample_count;
    if (current_sample >= sample)
      return it->sample_offset;
  }
  DCHECK_LE(sample, current_sample) << " Sample is invalid";
  return 0;
}

uint32 CompositionOffsetIterator::NumSamples() const {
  uint32 num_samples = 0;
  std::vector<CompositionOffset>::const_iterator it =
      composition_offset_table_.begin();
  for (; it != composition_offset_table_.end(); ++it) {
    num_samples += it->sample_count;
  }
  return num_samples;
}

}  // namespace mp4
}  // namespace media