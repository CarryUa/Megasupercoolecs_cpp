#pragma once
#include <MSCE/math.h>
#include <cstdint>
#include <map>

namespace msce
{

class IAnimation
{
public:
  virtual ~IAnimation() = default;
  virtual size_t frame_count() const noexcept = 0;

  virtual void play_frame(float time) = 0;
};

template <typename T, typename Easing = EasingLinear>
class Animation : public IAnimation
{
private:
  float nf_time_ = 0;
  float time_ = 0;
  std::size_t cf_;
  std::size_t nf_;
  std::vector<std::pair<float, T>> frames_;
  T *tar_ = nullptr;

  void move_frames() noexcept;

public:
  bool playing;

  Animation(T *target = nullptr, bool start_playing = false);

  void set_target(T *);
  void add_frame(float timestamp, T frame);
  virtual size_t frame_count() const noexcept override;

  virtual void play_frame(float time) override;
};
template <typename T, typename Easing>
inline void Animation<T, Easing>::move_frames() noexcept
{

  *tar_ = frames_[nf_].second;
  nf_++;
  if (nf_ == frames_.size())
  {
    cf_ = 0;
    nf_ = 1;
    time_ = 0;
  }
  else
    cf_++;

  nf_time_ = frames_[nf_].first;
}
template <typename T, typename Easing>
inline Animation<T, Easing>::Animation(T *target, bool start_playing)
{
  tar_ = target;
  time_ = 0.0;
  playing = start_playing;
  cf_ = 0;
  nf_ = 0;
}
template <typename T, typename Easing>
inline void Animation<T, Easing>::set_target(T *t)
{
  tar_ = t;
}
template <typename T, typename Easing>
inline void Animation<T, Easing>::add_frame(float timestamp, T frame)
{
  auto it =
      std::upper_bound(frames_.begin(), frames_.end(), timestamp,
                       [](float t, const auto &f) { return t < f.first; });
  frames_.insert(it, std::move(std::pair(timestamp, frame)));

  if (frames_.size() == 2)
  {
    cf_ = 0;
    nf_ = 1;
    nf_time_ = frames_[nf_].first;
    time_ = 0;
  }
}
template <typename T, typename Easing>
inline size_t Animation<T, Easing>::frame_count() const noexcept
{
  return frames_.size();
}
template <typename T, typename Easing>
inline void Animation<T, Easing>::play_frame(float delta_time)
{
  if (!playing) return;
  if (!tar_) return;
  if (cf_ == frames_.size() || nf_ == frames_.size() || cf_ == nf_) return;

  time_ += delta_time;
  if (time_ >= nf_time_ || nf_time_ == 0)
  {
    move_frames();
    return;
  };
  if (nf_time_ - frames_[cf_].first == 0) return;

  float ftf_progress =
      (time_ - frames_[cf_].first) / (nf_time_ - frames_[cf_].first);
  *tar_ = Interpolate<T, Easing>{}(frames_[cf_].second, frames_[nf_].second,
                                   ftf_progress);
}
} // namespace msce